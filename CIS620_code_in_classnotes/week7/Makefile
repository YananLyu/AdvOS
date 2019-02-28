all: serv cli

serv: server.o
	gcc -pthread server.o -o serv
server.o: server.c
	gcc -c server.c
cli: client.o
	gcc -pthread client.o -o cli
client.o: client.c
	gcc -c client.c
clean:
	rm *.o serv cli
