#f your application is in a file named myapp.cpp or myapp.c
# this is the line you will need to build the binary.
all: sendmsg_to_server sendmsg_to_peer

sendmsg_to_server: sendmsg_to_server.c
	$(CC) -o $@ $< -g -DDEBUG -lpthread
sendmsg_to_peer: sendmsg_to_peer.c
	$(CC) -o $@ $< -g -DDEBUG -lpthread
clean: 
	rm -f sendmsg_to_peer sendmsg_to_peer.o sendmsg_to_server sendmsg_to_server.o
