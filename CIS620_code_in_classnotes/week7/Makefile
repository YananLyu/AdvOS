all: serv_tcp cli_tcp cli_udp serv_udp

serv_tcp: Server_TCP.o
	gcc -pthread Server_TCP.o -o serv_tcp
Server_TCP.o: Server_TCP.c
	gcc -c Server_TCP.c
cli_tcp: Client_TCP.o
	gcc -pthread Client_TCP.o -o cli_tcp
Client_TCP.o: Client_TCP.c
	gcc -c Client_TCP.c

serv_udp: server_udp.o
	gcc -pthread server_udp.o -o serv_udp
server_udp.o: server_udp.c
	gcc -c server_udp.c
cli_udp: cli_udp_brd.o
	gcc -pthread cli_udp_brd.o -o cli_udp
cli_udp_brd.o: cli_udp_brd.c
	gcc -c cli_udp_brd.c

clean:
	rm *.o serv_tcp cli_tcp serv_udp cli_udp 
