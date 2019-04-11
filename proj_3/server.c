// this is the server file
#include <sys/types.h>  // define many types, we use "pthread" stuff for this file.
#include <sys/socket.h>  // define type "scoklen_t", type "sa_family_t" and "sockaddr" structure
#include <sys/un.h>  // define "sockaddr_un" structure
#include <netdb.h>  // definitions for network database operations. We use "gethostbyname"
#include <unistd.h> // define miscellaneous symbolic contants and types, declare miscellaneous functions. we use "lseek, read write" etc.
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <wait.h>
#include <ctype.h>  // character types. e.g "toascii() isalum()"
#include <netinet/in.h>		// type in_port_t and in_addr_t
#include <arpa/inet.h>		// structure in_addr.  ntohs() may be declared as function or macros
#include <string.h>

#define UDP_PORT	35896   //3 + last 4 digits of ID
#define MAX    		1024	// for TCP
#define BUFMAX		2048	// for UDP

#define MSG "PUT BANK620 "

static char buf[MAX];

int
regist_in_servicemap(int tcp_port) {	// TODO: tcp_port type?

	int sk;
	char buf[BUFMAX];

	struct sockaddr_in remote;
	struct hostent *hp;		// hostent: this structure is used to keep info related to host

	/* Create an Internet domain datagram socket */
	if ((sk = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("generate error");
		return -2;
	}

	remote.sin_family = AF_INET;	// Internet-based applications

	// CSU Grail address: 137.148.204.40. So the broadcast address is 137.148.204.255
	remote.sin_addr.s_addr = inet_addr("137.148.205.255");

	// Set the wellknown port number: 3 + last 4 digits of ID
	remote.sin_port = ntohs(UDP_PORT);

	/* setsockopt is required on Linux, but not on Solaris */
	setsockopt(sk,SOL_SOCKET,SO_BROADCAST,(struct sockaddr *)&remote,sizeof(remote));

	sendto(sk, MSG, strlen(MSG)+1, 0, (struct sockaddr *) &remote, sizeof(remote));	// Send the message to register

	read(sk, buf, BUFMAX);	// Get the reply "OK" from servicemap

	close(sk);	// close socket
	if (buf == "OK")	// register successfully
		return 0;
	else				// Falied to register in servicemap
		return -1;
}

// signal handler
void signal_catcher(int the_sig) {
	wait(0);			// cleanup the zombie
}

int
main() {
	
	int		orig_sock,		// Original socket in server
			new_sock;		// New socket from connect
	socklen_t	clnt_len;		// Length of client address
	struct sockaddr_in  clnt_adr, serv_adr; // client and server addresses
	int		len, i;

	if (signal(SIGCHLD, signal_catcher) == SIG_ERR) {
		perror("SIGCHLD"); 
		return 1;
	}

	// get a socket for this server
	if ((orig_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("generate error");
		return 2;
	}
	serv_adr.sin_family			= AF_INET;				// Set address type: interface based
	serv_adr.sin_addr.s_addr	= INADDR_ANY;			// Any interface
	serv_adr.sin_port			= 0;					// Let OS assign port number

	// Bind IP and Port
	if (bind(orig_sock, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0) {
		getsockname(orig_sock, (struct sockaddr *) &serv_adr, sizeof(serv_adr));
		perror("Bind error");
		printf("Server Port number is: %d", ntohs(serv_adr.sin_port));
		close(orig_sock);		
		return 3;
	}

	/* Listen: converts an unconnected socket into a passive socket, 
	   indicating that the kernel should accept incoming connection 
	   requests directed to this socket.
	*/
	/* the second parameter specifies the maximum number of connections
	 the kernel should queue for this socket
	*/
	if (listen(orig_sock, 5) < 0) {
		close(orig_sock);
		perror("lesten error");
		return 4;
	}

	/* UDP */
	if(regist_in_servicemap(serv_adr.sin_port) != 0) {	// TODO: parameter right?
		close(orig_sock);
		perror("Register ServiceMap error");
		return -1;		
	}

	do {
		clnt_len = sizeof(clnt_adr);
		// ACCEPT a connect
		if ((new_sock = accept(orig_sock, 
			(struct sockaddr *) &clnt_adr,
			&clnt_len)) < 0) {
			close(orig_sock);
			perror("accept error");
			return 5;
		}

		if (fork() == 0) {  // Generate a CHILD to tackle task(query/update db19)
			close(orig_sock);
			while ( (len=recv(new_sock, buf, MAX,0)) > 0) {  // receive the msg
				// TODO: tackle db
				send(new_sock, buf, len, 0);		// write back to client
				if (buf[0]  == '.') break;
			}
			close(new_sock);
			return 0;
		} else
			close(new_sock);						// In PARENT process
	} while(1);						// FOREVER

	return 0;
}