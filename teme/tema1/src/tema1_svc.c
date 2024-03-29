/*
 * Maria Moșneag
 * 343C1
 * Tema 1 SPRC
 * This file was generated using rpcgen and
 * modified to add support for command line args.
 */

#include "tema1.h"
#include <stdio.h>
#include <stdlib.h>
#include <rpc/pmap_clnt.h>
#include <string.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "db.h"
#include "utils.h"
#include <unordered_map>
#include <vector>
using namespace std;

#ifndef SIG_PF
#define SIG_PF void(*)(int)
#endif

static void
tema1_prog_1(struct svc_req *rqstp, register SVCXPRT *transp)
{
    union {
        request_t request_authorization_1_arg;
        request_t request_access_token_1_arg;
        request_t validate_delegated_action_1_arg;
        request_t approve_request_token_1_arg;
    } argument;
    char *result;
    xdrproc_t _xdr_argument, _xdr_result;
    char *(*local)(char *, struct svc_req *);

    switch (rqstp->rq_proc) {
    case NULLPROC:
        (void) svc_sendreply (transp, (xdrproc_t) xdr_void, (char *)NULL);
        return;

    case request_authorization:
        _xdr_argument = (xdrproc_t) xdr_request_t;
        _xdr_result = (xdrproc_t) xdr_response_t;
        local = (char *(*)(char *, struct svc_req *)) request_authorization_1_svc;
        break;

    case request_access_token:
        _xdr_argument = (xdrproc_t) xdr_request_t;
        _xdr_result = (xdrproc_t) xdr_response_t;
        local = (char *(*)(char *, struct svc_req *)) request_access_token_1_svc;
        break;

    case validate_delegated_action:
        _xdr_argument = (xdrproc_t) xdr_request_t;
        _xdr_result = (xdrproc_t) xdr_response_t;
        local = (char *(*)(char *, struct svc_req *)) validate_delegated_action_1_svc;
        break;

    case approve_request_token:
        _xdr_argument = (xdrproc_t) xdr_request_t;
        _xdr_result = (xdrproc_t) xdr_response_t;
        local = (char *(*)(char *, struct svc_req *)) approve_request_token_1_svc;
        break;

    default:
        svcerr_noproc (transp);
        return;
    }
    memset ((char *)&argument, 0, sizeof (argument));
    if (!svc_getargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
        svcerr_decode (transp);
        return;
    }
    result = (*local)((char *)&argument, rqstp);
    if (result != NULL && !svc_sendreply(transp, (xdrproc_t) _xdr_result, result)) {
        svcerr_systemerr (transp);
    }
    if (!svc_freeargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
        fprintf (stderr, "%s", "unable to free arguments");
        exit (1);
    }
    return;
}

int
main (int argc, char **argv)
{
    // EXTRA SECTION START
    
    if (argc != 5) {
        fprintf(stderr, "Usage:\n\t%s <USER_IDS> <RESOURCES> <APPROVALS>"
                        " <VALABILITY>\n", argv[0]);
        return -1;
    }

    // get user IDs
    FILE *in = fopen(argv[1], "r");
    int users;
    char id[ID_LEN + 1];
    fscanf(in, "%d", &users);
    fgetc(in);
    for (int i = 1; i <= users; i++) {
        fgets(id, ID_LEN + 1, in);
        fgetc(in);
        string s = id;
        id_to_token[s] = "";
    }
    fclose(in);

    // get resources
    in = fopen(argv[2], "r");
    int res_cnt;
    char res[RES_LEN + 1];
    fscanf(in, "%d", &res_cnt);
    fgetc(in);
    for (int i = 1; i <= res_cnt; i++) {
        fgets(res, RES_LEN + 1, in);
        if (res[strlen(res) - 1] == '\n')
            res[strlen(res) - 1] = '\0';
        string s = res;
        resources.insert(s);
    }
    fclose(in);

    // get approvals
    in = fopen(argv[3], "r");
    char perm[PERM_LEN + 1];
    next_approval = 0;
    while (fgets(perm, PERM_LEN + 1, in)) {
        unordered_map<string, vector<int>> one_step_perm;

        if (perm[strlen(perm) - 1] == '\n')
            perm[strlen(perm) - 1] = '\0';
        char *crt_res = strtok(perm, ",");
        char *crt_perm;
        while (crt_res) {
            crt_perm = strtok(NULL, ",");
            string r = crt_res;
            vector<int> p(5, 0);
            if (resources.find(r) != resources.end()) {
                for (int j = 0; j < strlen(crt_perm); j++) {
                    if (crt_perm[j] == 'R')
                        p[READ] = 1;
                    else if (crt_perm[j] == 'I')
                        p[INSERT] = 1;
                    else if (crt_perm[j] == 'M')
                        p[MODIFY] = 1;
                    else if (crt_perm[j] == 'D')
                        p[DELETE] = 1;
                    else if (crt_perm[j] == 'X')
                        p[EXECUTE] = 1;
                }
                one_step_perm[r] = p;
            }
            
            crt_res = strtok(NULL, ",");   
        }
        approvals.push_back(one_step_perm);
    }
    fclose(in);

    // get valability
    in = fopen(argv[4], "r");
    char input[PERM_LEN + 1];
    fgets(input, PERM_LEN, in);
    char *aux = strtok(input, "=");
    aux = strtok(NULL, "=");
    aux[strlen(aux) - 2] = '\0';
    default_val = atoi(aux + 1);
    fclose(in);

    // EXTRA SECTION END

    register SVCXPRT *transp;

    pmap_unset (TEMA1_PROG, TEMA1_VERS);

    transp = svcudp_create(RPC_ANYSOCK);
    if (transp == NULL) {
        fprintf (stderr, "%s", "cannot create udp service.");
        exit(1);
    }
    if (!svc_register(transp, TEMA1_PROG, TEMA1_VERS, tema1_prog_1, IPPROTO_UDP)) {
        fprintf (stderr, "%s", "unable to register (TEMA1_PROG, TEMA1_VERS, udp).");
        exit(1);
    }

    transp = svctcp_create(RPC_ANYSOCK, 0, 0);
    if (transp == NULL) {
        fprintf (stderr, "%s", "cannot create tcp service.");
        exit(1);
    }
    if (!svc_register(transp, TEMA1_PROG, TEMA1_VERS, tema1_prog_1, IPPROTO_TCP)) {
        fprintf (stderr, "%s", "unable to register (TEMA1_PROG, TEMA1_VERS, tcp).");
        exit(1);
    }

    svc_run ();
    fprintf (stderr, "%s", "svc_run returned");
    exit (1);
    /* NOTREACHED */
}
