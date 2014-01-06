#f your application is in a file named myapp.cpp or myapp.c
# this is the line you will need to build the binary.
all: sendmsg

sendmsg: sendmsg.c
	$(CC) -o $@ $< -g -DDEBUG -lpthread
clean: 
	rm -f sendmsg sendmsg.o
