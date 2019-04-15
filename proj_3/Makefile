all: servicemap server client

servicemap: servicemap.o
	gcc -o servicemap servicemap.o
servicemap.o: servicemap.c
	gcc -c servicemap.c
server: server.o
	gcc -o server server.o
server.o: server.c
	gcc -c server.c
client: client.o
	gcc -o client client.o
client.o: client.c
	gcc -c client.c

clean:
	rm *.o servicemap server client
