#include <rpc/rpc.h>
#include <stdio.h>

#include <unordered_map>
#include <vector>
#include "db.h"
#include "token.h"
#include "utils.h"

#include "tema1.h"

using namespace std;

response_t *request_authorization_1_svc(struct request_t *data, struct svc_req *cl) {
	static struct response_t resp;

	string id = data->id;

	printf("BEGIN %s AUTHZ\n", id.c_str());
	fflush(stdout);

	if (id_to_token.find(id) != id_to_token.end()) {
		char *id_str = strdup(id.c_str());
		resp.req_token = strdup(generate_access_token(id_str));
		free(id_str);

		resp.result = strdup("");
		resp.acc_token = strdup("");
		resp.ref_token = strdup("");
		resp.valability = 0;
	} else {
		resp.result = strdup("USER_NOT_FOUND");
		resp.req_token = strdup("");
		resp.acc_token = strdup("");
		resp.ref_token = strdup("");
		resp.valability = 0;
	}

	return &resp;
}

response_t *request_access_token_1_svc(request_t *data, struct svc_req *cl) {
	static struct response_t resp;

	string id = data->id;
	int type = data->type;
	string token = data->token;
	string additional = data->additional;

	if (data->type == REQUEST) {

		// not signed
		if (data->token[0] != '+') {
			// REQUEST DENIED.
			resp.result = strdup("REQUEST_DENIED");
			printf("  RequestToken = %s\n", data->token);
			fflush(stdout);
			resp.req_token = strdup("");
			resp.acc_token = strdup("");
			resp.ref_token = strdup("");
			resp.valability = 0;
			// next_approval++;
			token_valab[id_to_token[id]] = 0;
			acc_to_perm[id_to_token[id]] = empty_perm;
			id_to_token[id] = "";

			return &resp;
		}

		// request auth token (unsigned)
		char *req_tok = strdup(token.substr(1, token.size() - 1).c_str());
		// access token
		string acc_token = generate_access_token(req_tok);
		// invalidate the previous access token of the user
		token_valab[id_to_token[id]] = 0;
		acc_to_perm[id_to_token[id]] = empty_perm;
		// associate the new access token with the user id
		id_to_token[id] = acc_token;
		// associate the access token with the perms from the request auth token
		acc_to_perm[acc_token] = req_to_perm[token.substr(1, token.size() - 1)];
		// reset the valability of the new access token
		token_valab[acc_token] = default_val;

		resp.result = strdup("");
		resp.req_token = strdup(req_tok);
		free(req_tok);
		resp.acc_token = strdup(acc_token.c_str());
		resp.ref_token = strdup("");
		resp.valability = default_val;
		// if renewable token
		if (additional.at(0) == '1') {
			free(resp.ref_token);
			resp.ref_token = strdup(generate_access_token(resp.acc_token));
		}

		printf("  RequestToken = %s\n", resp.req_token);
		printf("  AccessToken = %s\n", resp.acc_token);
		fflush(stdout);
		string rf = resp.ref_token;
		if (rf != "") {
			printf("  RefreshToken = %s\n", resp.ref_token);
			fflush(stdout);
		}

	} else if (data->type == REFRESH) {
		printf("BEGIN %s AUTHZ REFRESH\n", id.c_str());
		fflush(stdout);

		// invalidate the old token
		string old_acc_token = id_to_token[id];
		token_valab[old_acc_token] = 0;
		char *ref_token = strdup(token.c_str());
		// generate new access token
		char *new_acc_token_char = generate_access_token(ref_token);
		string new_acc_token = new_acc_token_char;
		free(ref_token);
		// generate new refresh token
		string new_ref_token = generate_access_token(new_acc_token_char);

		// replace the token linked to the user id
		id_to_token[id] = new_acc_token;
		// set the new token's valability
		token_valab[new_acc_token] = default_val;
		// keep the same perms
		acc_to_perm[new_acc_token] = acc_to_perm[old_acc_token];
		acc_to_perm[old_acc_token] = empty_perm;

		resp.result = strdup("");
		resp.req_token = strdup("");
		resp.acc_token = strdup(new_acc_token.c_str());
		resp.ref_token = strdup(new_ref_token.c_str());
		resp.valability = default_val;

		printf("  AccessToken = %s\n", resp.acc_token);
		printf("  RefreshToken = %s\n", resp.ref_token);
		fflush(stdout);
	}

	return &resp;
}

response_t *validate_delegated_action_1_svc(request_t *data, struct svc_req *cl) {
	static struct response_t resp;

	string id = data->id;
	int type = data->type;
	string token = data->token;
	string additional = data->additional;
	string cmd = data->cmd;

	resp.req_token = strdup("");
	resp.acc_token = strdup("");
	resp.ref_token = strdup("");
	resp.valability = 0;
	resp.result = NULL;

	// update token valability
	token_valab[token]--;
	
	if (token == "" || id_to_token[id] != token) {
		// access token doesn't match id
		resp.result = strdup("PERMISSION_DENIED");
	
	} else if (token_valab[token] < 0) {
		// token expired
		resp.result = strdup("TOKEN_EXPIRED");
		token = "";

	} else if (resources.find(additional) == resources.end()) {
		// unavailable resource
		resp.result = strdup("RESOURCE_NOT_FOUND");

	} else if (acc_to_perm[token].find(additional) == acc_to_perm[token].end() ||
				type > EXECUTE ||
				acc_to_perm[token][additional][type] == 0) {
		// operation not permitted
		resp.result = strdup("OPERATION_NOT_PERMITTED");
	}

	if (resp.result != NULL) {
		int val = token_valab[token];
		if (val < 0)
			val = 0;
		resp.valability = val;
		printf("DENY (%s,%s,%s,%d)\n", cmd.c_str(),
				additional.c_str(), token.c_str(), val);
		fflush(stdout);

		return &resp;
	}

	printf("PERMIT (%s,%s,%s,%d)\n", cmd.c_str(),
				additional.c_str(), token.c_str(), token_valab[token]);
	fflush(stdout);
	resp.valability = token_valab[token];
	resp.result = strdup("PERMISSION_GRANTED");
	return &resp;
}

response_t *approve_request_token_1_svc(request_t *data, struct svc_req *cl) {
	static struct response_t resp;

	string token = data->token;

	if (approvals[next_approval].size() == 0) {
		// DENIED
		resp.result = strdup("");
		resp.req_token = strdup(data->token);
		resp.acc_token = strdup("");
		resp.ref_token = strdup("");
		resp.valability = 0;
		next_approval++;

		return &resp;
	}

	// add perms
	req_to_perm[token] = approvals[next_approval];
	next_approval++;

	resp.result = strdup("");
	string stamp = "+";
	resp.req_token = strdup(stamp.append(token).c_str());
	resp.acc_token = strdup("");
	resp.ref_token = strdup("");
	resp.valability = 0;

	return &resp;
}
