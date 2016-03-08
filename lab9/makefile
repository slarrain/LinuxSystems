all: client server

server: server.c
				gcc -Wall -pthread -lm server.c -o server

client: client.c
				gcc -Wall client.c -o client

clean:
	rm client server
