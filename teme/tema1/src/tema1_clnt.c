/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include <memory.h> /* for memset */
#include "tema1.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

response_t *
request_authorization_1(request_t *argp, CLIENT *clnt)
{
    static response_t clnt_res;

    memset((char *)&clnt_res, 0, sizeof(clnt_res));
    if (clnt_call (clnt, request_authorization,
        (xdrproc_t) xdr_request_t, (caddr_t) argp,
        (xdrproc_t) xdr_response_t, (caddr_t) &clnt_res,
        TIMEOUT) != RPC_SUCCESS) {
        return (NULL);
    }
    return (&clnt_res);
}

response_t *
request_access_token_1(request_t *argp, CLIENT *clnt)
{
    static response_t clnt_res;

    memset((char *)&clnt_res, 0, sizeof(clnt_res));
    if (clnt_call (clnt, request_access_token,
        (xdrproc_t) xdr_request_t, (caddr_t) argp,
        (xdrproc_t) xdr_response_t, (caddr_t) &clnt_res,
        TIMEOUT) != RPC_SUCCESS) {
        return (NULL);
    }
    return (&clnt_res);
}

response_t *
validate_delegated_action_1(request_t *argp, CLIENT *clnt)
{
    static response_t clnt_res;

    memset((char *)&clnt_res, 0, sizeof(clnt_res));
    if (clnt_call (clnt, validate_delegated_action,
        (xdrproc_t) xdr_request_t, (caddr_t) argp,
        (xdrproc_t) xdr_response_t, (caddr_t) &clnt_res,
        TIMEOUT) != RPC_SUCCESS) {
        return (NULL);
    }
    return (&clnt_res);
}

response_t *
approve_request_token_1(request_t *argp, CLIENT *clnt)
{
    static response_t clnt_res;

    memset((char *)&clnt_res, 0, sizeof(clnt_res));
    if (clnt_call (clnt, approve_request_token,
        (xdrproc_t) xdr_request_t, (caddr_t) argp,
        (xdrproc_t) xdr_response_t, (caddr_t) &clnt_res,
        TIMEOUT) != RPC_SUCCESS) {
        return (NULL);
    }
    return (&clnt_res);
}
