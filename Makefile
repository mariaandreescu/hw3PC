CC=gcc
CFLAGS=-I.

client: client.c requests.c helpers.c cjson.c  
		$(CC) -g -o client client.c requests.c helpers.c cjson.c -Wall
		
run: client
		./client

clean:
		rm -f *.o client
