RPC = rpcgen
CC = gcc
CFLAGS += -g -I/usr/include/tirpc
LDLIBS += -lnsl -ltirpc
PROGRAM = check
SRC_CLIENT = $(PROGRAM)_clnt.c $(PROGRAM)_xdr.c

.PHONY: build clean

build: client

client: $(SRC_CLIENT) $(PROGRAM)_rpc_client.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(SRC_CLIENT): $(PROGRAM).x
	$(RPC) -C $^

clean:
	rm -f client $(PROGRAM).h $(SRC_CLIENT)
