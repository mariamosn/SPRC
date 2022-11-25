/*
 * Maria Moșneag
 * 343C1
 * Tema 1 SPRC
 */

#include <stdio.h>
#include <rpc/rpc.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <unordered_map>

#include "tema1.h"
#include "token.h"
#include "utils.h"

#define PROTOCOL "tcp"

using namespace std;

unordered_map<string, string> id_to_access;
unordered_map<string, string> id_to_refresh;
unordered_map<string, int> id_to_valab;
CLIENT *handle;

void free_req(struct request_t *request) {
	free(request->id);
	free(request->token);
	free(request->additional);
	free(request->cmd);
}

void free_resp(struct response_t *response) {
	free(response->result);
	free(response->req_token);
	free(response->acc_token);
	free(response->ref_token);
}

void get_input(char line[], string &id, string &req, string &additional) {
	if (line[strlen(line) - 1] == '\n')
		line[strlen(line) - 1] = '\0';

	char *str = strtok(line, ",");
	id = str;
	str = strtok(NULL, ",");
	req = str;
	str = strtok(NULL, ",");
	additional = str;

	if (id_to_access.find(id) == id_to_access.end())
		id_to_access[id] = "";
	if (id_to_refresh.find(id) == id_to_refresh.end())
		id_to_refresh[id] = "";
}

string _request_authorization(string id, string req, string &crt_req_tok) {
	struct request_t request;
	struct response_t *response;

	// user ID
	request.id = strdup(id.c_str());
	request.type = REQUEST;
	request.token = strdup("");
	request.additional = strdup("");
	request.cmd = strdup(req.c_str());
	response = request_authorization_1(&request, handle);
	free_req(&request);

	string res = response->result;
	crt_req_tok = response->req_token;
	free_resp(response);

	return res;
}

string _approve_request_token(string crt_req_tok, string req) {
	struct request_t request;
	struct response_t *response;

	request.id = strdup("");
	request.type = REQUEST;
	// auth token
	request.token = strdup(crt_req_tok.c_str());
	request.additional = strdup("");
	request.cmd = strdup(req.c_str());
	response = approve_request_token_1(&request, handle);
	free_req(&request);

	string res = response->req_token;
	free_resp(response);
	return res;
}

string _request_access_token(string id, string additional, string res,
								string req) {
	struct request_t request;
	struct response_t *response;

	// user ID
	request.id = strdup(id.c_str());
	request.type = REQUEST;
	// with or without token refresh
	request.additional = strdup(additional.c_str());
	// (signed) request auth token
	request.token = strdup(res.c_str());
	request.cmd = strdup(req.c_str());
	response = request_access_token_1(&request, handle);
	free_req(&request);

	res = response->result;
	if (res == "") {
		id_to_access[id] = response->acc_token;
		id_to_refresh[id] = response->ref_token;
		id_to_valab[id] = response->valability;
	}

	free_resp(response);
	return res;
}

void _check_refresh(string id) {
	if (id_to_valab[id] > 0 || id_to_refresh[id] == "")
		return;
		
	struct request_t renew_request;
	struct response_t *response;
	// user ID
	renew_request.id = strdup(id.c_str());
	renew_request.type = REFRESH;
	// refresh token
	renew_request.token = strdup(id_to_refresh[id].c_str());
	renew_request.additional = strdup("");
	renew_request.cmd = strdup("REFRESH");

	response = request_access_token_1(&renew_request, handle);
	free_req(&renew_request);
	id_to_access[id] = response->acc_token;
	id_to_refresh[id] = response->ref_token;
	id_to_valab[id] = response->valability;
	free_resp(response);
}

void _send_action_request(string id, string additional, string req) {
	struct request_t request;
	struct response_t *response;

	request.id = strdup(id.c_str());
	// access token
	request.token = strdup(id_to_access[id].c_str());
	// resource
	request.additional = strdup(additional.c_str());
	request.cmd = strdup(req.c_str());

	if (req == "READ") {
		request.type = READ;
	} else if (req == "INSERT") {
		request.type = INSERT;
	} else if (req == "MODIFY") {
		request.type = MODIFY;
	} else if (req == "DELETE") {
		request.type = DELETE;
	} else if (req == "EXECUTE") {
		request.type = EXECUTE;
	} else {
		request.type = ERROR;
	}

	response = validate_delegated_action_1(&request, handle);

	id_to_valab[id] = response->valability;
	printf("%s\n", response->result);
	free_req(&request);
	free_resp(response);
}

int main(int argc, char const *argv[]) {
	if (argc != 3) {
		fprintf(stderr, "Usage:\n\t%s <SERVER_ADDRESS> <CLIENT_INPUT>\n",
			argv[0]);
		return -1;
	}

	handle = clnt_create(argv[1], TEMA1_PROG, TEMA1_VERS, PROTOCOL);
	if (!handle) {
		perror("Failed to create client handle");
		clnt_pcreateerror(argv[0]);
		return -2;
	}

	// read line by line from the client input file
	FILE *in;
    char line[CLIENT_IN_LEN];
    string id, req, additional;
	struct request_t request;
	struct response_t *response;

	in = fopen(argv[2], "r");
	while (fgets(line, CLIENT_IN_LEN, in)) {
		get_input(line, id, req, additional);

		// build the request
		if (req == "REQUEST") {
			string crt_req_tok, res;

			// Request Authorization
			res = _request_authorization(id, req, crt_req_tok);
			if (res != "") {
				// USER_NOT_FOUND
				printf("%s\n", res.c_str());
				continue;
			}

			// Approve Request Token
			res = _approve_request_token(crt_req_tok, req);

			// Request Access Token
			res = _request_access_token(id, additional, res, req);

			if (res != "") {
				// REQUEST_DENIED
				printf("%s\n", res.c_str());
				continue;
			}

			if (id_to_refresh[id] != "") {
				printf("%s -> %s,%s\n", crt_req_tok.c_str(),
					id_to_access[id].c_str(), id_to_refresh[id].c_str());
			} else {
				printf("%s -> %s\n",
					crt_req_tok.c_str(), id_to_access[id].c_str());
			}

		} else {
			_check_refresh(id);
			_send_action_request(id, additional, req);
		}
    }
    fclose(in);

	clnt_destroy(handle);

	return 0;
}
