CC=gcc
all:
	$(CC) Client/client.c -o client
	$(CC) Server/server.c -o server
clean:
	rm -rf client server

.PHONY: all clean
