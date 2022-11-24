struct request_t {
	string id<>;
	int type;
	string token<>;
	string additional<>;
	string cmd<>;
};

struct response_t {
	string result<>;
	string req_token<>;
	string acc_token<>;
	string ref_token<>;
	int valability;
};

program TEMA1_PROG {
	version TEMA1_VERS {
		response_t request_authorization(request_t) = 1;
		response_t request_access_token(request_t) = 2;
		response_t validate_delegated_action(request_t) = 3;
		response_t approve_request_token(request_t) = 4;
	} = 1;
} = 1;
