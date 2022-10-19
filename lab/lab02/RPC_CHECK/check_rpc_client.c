#include <stdio.h>
#include <rpc/rpc.h>
#include <stdlib.h>
#include <string.h>

#include "check.h"

#define PROTOCOL "tcp"
#define SERVER_ADDR "rpc.sprc.dfilip.xyz"

int main(int argc, char const *argv[])
{
	CLIENT *handle;
	struct student data;

	if (argc != 3) {
		fprintf(stderr, "Usage:\n\t%s <NAME> <GROUP>\n",
			argv[0]);
		return -1;
	}

	handle = clnt_create(SERVER_ADDR, CHECKPROG, CHECKVERS, PROTOCOL);
	if (!handle) {
		perror("Failed to create client handle");
		clnt_pcreateerror(argv[0]);
		return -2;
	}

	data.name = strdup(argv[1]);
	data.group = strdup(argv[2]);

	printf("%s\n", *grade_1(&data, handle));

	clnt_destroy(handle);

	return 0;
}
