#f your application is in a file named myapp.cpp or myapp.c
# this is the line you will need to build the binary.
all: sendmsg sendmsgTCP

sendmsg: sendmsg.c
	$(CC) -o $@ $< -g -DDEBUG -lpthread
sendmsgTCP: sendmsgTCP.c
	$(CC) -o $@ $< -g -DDEBUG
clean: 
	rm -f sendmsg sendmsg.o sendmsgTCP sendmsgTCP.o
