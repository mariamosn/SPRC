RPC = rpcgen
CC = g++
CFLAGS += -g -I/usr/include/tirpc
# CFLAGS = -std=c++14 -Wall -Wextra -Wpedantic -Werror \
	-Wno-unused-variable -Wno-unused-parameter -Wno-cast-function-type
LDLIBS += -lnsl -ltirpc
PROGRAM = tema1
SRC_SERVER = $(PROGRAM)_svc.c $(PROGRAM)_xdr.c
SRC_CLIENT = $(PROGRAM)_clnt.c $(PROGRAM)_xdr.c

.PHONY: build clean

build: server client

server: $(SRC_SERVER) $(PROGRAM)_rpc_server.cpp db.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

client: $(SRC_CLIENT) $(PROGRAM)_rpc_client.cpp db.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(SRC_SERVER) $(SRC_CLIENT): $(PROGRAM).x
	$(RPC) -C $^

db.o: db.cpp

without_rpcgen: db.o
	$(CC) $(CFLAGS) $(SRC_SERVER) $(PROGRAM)_rpc_server.cpp db.o -o server $(LDFLAGS)
	$(CC) $(CFLAGS) $(SRC_CLIENT) $(PROGRAM)_rpc_client.cpp -o client $(LDFLAGS)

clean:
	rm -f client server $(PROGRAM).h $(SRC_SERVER) $(SRC_CLIENT)