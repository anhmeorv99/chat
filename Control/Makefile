all: server client

server: server.o
	gcc server.o -o server

client: client.o
	gcc client.o -o client

client.o: client.c
	gcc -c -Wall client.c

server.o: server.c
	gcc -c -Wall server.c

clean:
	rm -f *.o
	rm server client