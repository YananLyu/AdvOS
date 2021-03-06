// Internet domain, connection-oriented CLIENT
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <wait.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>

#define MSG     "GET BANK620"  
#define UDP_PORT	35639    //3 + last 4 digits of ID 
#define MAX    		1024
#define BUFMAX  	1024              

static char buf[MAX];

struct msg500 {
	uint32_t cmd;
	uint32_t acc;
};

struct msg501 {
	uint32_t cmd;
	uint32_t acc;
	float amount;
};

char * get_ip_and_port( );

int
main( int argc, char *argv[] ) {
	char * ip_port;
	ip_port = get_ip_and_port();
	if ( ip_port == NULL ) {
		perror("Failed to get server's IP and Port number from servicemap");
		free(ip_port);
		return 1;
	}
	// if receive server's ip and port successfully, print it out
	char * ip_addr, *port_num;
	ip_addr = strtok(ip_port, ":");
	port_num = strtok(NULL, ":");
	printf("Service provided by %s at port %s\n", ip_addr, port_num);


	/* ***  TCP: connect to the server  *** */
	int				orig_sock,		// Original socket in client
					len;			// Misc. counter
	struct sockaddr_in
					serv_adr;		// Internet addr of server
	struct hostent 	*host;			// The host (server) info
    
    /* For IPv4*/
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(ip_addr);
	serv_adr.sin_port = htons(atoi(port_num));

	if ( (orig_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("generate error");
		return 1;
	}

	free(ip_port);		// free the dynamic address
		
	// CONNECT
	if ( connect(orig_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) <0 ) {
		perror("connect error");
		return 2;
	}

	do {		// Process
		write(1, "> ", 2);
		memset(buf, '\0', sizeof(buf));
		if ( (len=read(0, buf, MAX)) > 0) {
			
			if ( strcmp(buf, "quit\n") == 0 ) {	// quit
				exit(0);
			}

			char * token;
			token = strtok(buf, " ");
			if ( strcmp(token, "query") == 0 ) {
				token = strtok(NULL, " ");	// get acc
				struct msg500 message500;
				message500.cmd = htonl(500);
				message500.acc = htonl(atoi(token));
				send(orig_sock, &message500, sizeof(struct msg500), 0);		// Send 500 acct
			} else if ( strcmp(token, "update") == 0 ) {
				struct msg501 message501;
				message501.cmd = htonl(501);

				token = strtok(NULL, " ");
				message501.acc = htonl(atoi(token));
				
				token = strtok(NULL, " ");
				message501.amount = htonl(atoi(token));

				send(orig_sock, &message501, sizeof(struct msg501), 0);		// Send 501, acct, amount
			} else {
				printf("USAGE:\n  query actnum\n  update acctnum amount\n  quit\n");
				continue;
			}

			memset(buf, '\0', sizeof(buf));
			if ( (len=recv(orig_sock, buf, MAX, 0)) >0 )	{	// receive msg from server
				write(1, buf, MAX);
				write(1, "\n", strlen("\n"));
				memset(buf, '\0', sizeof(buf));
			}
		}
	} while ( 1 );
	
	close(orig_sock);

	return 0;
}

char	*
get_ip_and_port( ) {
	/* UDP: get server's info: IP and port number
	*/
	int sk_udp;
	
	struct sockaddr_in remote;
	struct hostent *hp;		// hostent: this structure is used to keep info related to host

	/* Create an Internet domain datagram socket */
	if ( (sk_udp = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("Generating error");
		return NULL;
	}

	remote.sin_family = AF_INET;		// Internet-based applications

	// CSU Grail address: 137.148.204.40. So the broadcast address is 137.148.204.255
	remote.sin_addr.s_addr = inet_addr("137.148.255.255");

	// Set the wellknown port number: 3 + last 4digits of ID
	remote.sin_port = ntohs(UDP_PORT);

	/* setsockopt is required on Linux, but not on Solaris */
	setsockopt(sk_udp,SOL_SOCKET,SO_BROADCAST,(struct sockaddr *)&remote,sizeof(remote));

	// Send the message to servicemap: GET BANK620
	if ( sendto(sk_udp, MSG, strlen(MSG)+1, 0, (struct sockaddr *) &remote, sizeof(remote)) == -1) {
		perror("Sending GET message error!");
		close(sk_udp);
		return NULL;
	}

	struct sockaddr_in fromAddr;
	int len_fromAddr = sizeof(fromAddr);
	char *buf = (char * ) malloc(BUFMAX);
	if ( recvfrom(sk_udp, buf, BUFMAX, 0, (struct sockaddr *) &fromAddr, &len_fromAddr) == -1) {
		perror("Failed to receive IP_AND_PORT from servicemap");
		close(sk_udp);
		return NULL;
	}

	// Check the msg received
	if (*buf == '*') {
		printf("%s\n", buf);
		return NULL;
	}
	// close socket of UDP
	close(sk_udp);

	return buf;
}
