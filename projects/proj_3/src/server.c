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
#include <fcntl.h>

#define UDP_PORT	35639   //3 + last 4 digits of ID
#define MAX    		2048	// for TCP
#define BUFMAX		2048	// for UDP

#define MSG "PUT BANK620 "

struct record {
	int		acctnum; /* unique key in sorted order */
	char 	name[20];
	float	value;
	char	phone[16];
	int	age;
};

struct msg501 {
	uint32_t cmd;
	uint32_t acc;
	float amount;
};

int register_in_servicemap(unsigned int tcp_port);
int query_db19(struct record * row, int acctnum);
int update_db19(struct record * row, int acctnum, float val);

// signal handler
void signal_catcher(int the_sig) {
	wait(0);			// cleanup the zombie
}

int
main() {
	
	int		orig_sock,		// Original socket in server
			new_sock;		// New socket from connect
	socklen_t	clnt_len;		// Length of client address
	struct sockaddr_in  clnt_adr, serv_adr, my_adr; // client and server addresses
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

		perror("Bind error");
		close(orig_sock);		
		return 3;
	}

	// Get my ip address and port
	unsigned int tcp_port;
	int slen = sizeof(serv_adr);
	bzero(&my_adr, sizeof(my_adr));
	getsockname(orig_sock, (struct sockaddr *) &my_adr, &slen);
	tcp_port = ntohs(my_adr.sin_port);

	/* UDP */
	if( register_in_servicemap(tcp_port) != 0) {
		close(orig_sock);
		perror("Register ServiceMap error");
		return -1;		
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
	
	clnt_len = sizeof(clnt_adr);
	// ACCEPT a connect
	if ((new_sock = accept(orig_sock, 
		(struct sockaddr *) &clnt_adr,
		&clnt_len)) < 0) {
		close(orig_sock);
		perror("accept error");
		return 5;
	}

	do {
		// get ip
		char c_ip[16];
		inet_ntop(AF_INET, &(clnt_adr.sin_addr), c_ip, 16);

		struct msg501 buf;
		if (len=recv(new_sock, &buf, MAX,0) > 0) {  // receive the msg
			printf("Service Requested from %s\n", c_ip);
			if (fork() == 0) {  // Generate a CHILD to tackle task(query/update db19)
				/* ***   tackle db   *** */
				uint32_t cmd = ntohl(buf.cmd);	// change it from network order to host order
				if ( cmd == 500 ) {  	// QUERY
					char rep_buf[MAX];
					memset(rep_buf, '\0', sizeof(rep_buf));
					struct record row;
					if ( (query_db19(&row, ntohl(buf.acc))) == -1 ) {	// Query Failed
						strcpy(rep_buf, "Query Failed: the record you want to query is not in db file\n");
						send(new_sock, rep_buf, MAX, 0);
						//close(new_sock);
						break;
					} else {	// Query successfully
						char tmp[sizeof(struct record)];
						strcpy(rep_buf, row.name);
						strcat(rep_buf, " ");
						sprintf(tmp, "%d", row.acctnum);
						strcat(rep_buf, tmp);
						strcat(rep_buf, " ");
						sprintf(tmp, "%.1f", row.value);
						strcat(rep_buf, tmp);
						strcat(rep_buf, " ");
						strcat(rep_buf, row.phone);
						strcat(rep_buf, "\n");
					}
					int k = send(new_sock, rep_buf, strlen(rep_buf), 0);	// write back to client
					//close(new_sock);
					sleep(1);
					exit(0);
				} else if ( cmd == 501 ) {	// UPDATE

					char rep_buf[MAX];
					memset(rep_buf, '\0', sizeof(rep_buf));
					struct record row;
					if ( (update_db19(&row, ntohl(buf.acc), ntohl(buf.amount))) == -1 ) {	// Query Failed
						strcpy(rep_buf, "Update Failed: the record quired is not in db file\n");
						send(new_sock, rep_buf, strlen(rep_buf), 0);
						//close(new_sock);
					} else {	// Query successfully
						char tmp[sizeof(struct record)];
				
						strcpy(rep_buf, row.name);
						strcat(rep_buf, " ");
						sprintf(tmp, "%d", row.acctnum);
						strcat(rep_buf, tmp);
						strcat(rep_buf, " ");
						sprintf(tmp, "%.1f", row.value);
						strcat(rep_buf, tmp);

						int k = send(new_sock, rep_buf, strlen(rep_buf), 0);	// write back to client
						//close(new_sock);
					}
				} 
				exit(0);
			}
					
		} else
			close(new_sock);
	} while(1);						// FOREVER

	return 0;
}

int
register_in_servicemap(unsigned int tcp_port) {

	int sk;
	char buf[BUFMAX];

	// message: PUT BANK620 port-num
	char serv_port[10];
	sprintf(serv_port, "%d", tcp_port);
	strcpy(buf, MSG);
	strcat(buf, serv_port);

	struct sockaddr_in remote;
	struct hostent *hp;		// hostent: this structure is used to keep info related to host

	/* Create an Internet domain datagram socket */
	if ((sk = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("Generate error");
		return -2;
	}

	remote.sin_family = AF_INET;	// Internet-based applications

	// CSU Grail address: 137.148.204.40. So the broadcast address is 137.148.204.255
	remote.sin_addr.s_addr = inet_addr("137.148.255.255");

	// Set the wellknown port number: 3 + last 4 digits of ID
	remote.sin_port = ntohs(UDP_PORT);

	/* setsockopt is required on Linux, but not on Solaris */
	setsockopt(sk,SOL_SOCKET,SO_BROADCAST,(struct sockaddr *)&remote,sizeof(remote));

	if ( sendto(sk, buf, strlen(buf)+1, 0, (struct sockaddr *) &remote, sizeof(remote)) == -1) {	// Send the message to register
		perror("Sending Registration message ERROR!");
		close(sk);
		return -1;
	}

	// Get the reply "OK" from servicemap
	struct sockaddr_in fromAddr;
	int len_fromAdr = sizeof(fromAddr);
	if ( recvfrom(sk, buf, BUFMAX, 0, (struct sockaddr *)&fromAddr, &len_fromAdr) == -1) {
		perror("Failed to receive OK from servicemap");
		close(sk);
		return -1;
	}

	close(sk);	// close socket
	if (strcmp(buf, "OK") == 0) {	// register successfully
		printf("Registration OK from %s\n", inet_ntoa(fromAddr.sin_addr));
		return 0;
	} else {
		return -1; 	// Falied to register in servicemap
	}
}

int query_db19(struct record * row, int acctnum) {
	int fd;

	// try to Open db19 with read only mode, if fail, exit(1)
	if ( (fd = open("db19", O_RDONLY)) == -1 ) {
		perror("Failed to OPEN db19");
		exit(1);
	}

	lseek(fd, 0, SEEK_SET);		// JUST IN CASE
	while( read(fd, row, sizeof(struct record)) > 0 ) {
		// if query successfully, return the info string 
		if (row->acctnum == acctnum) {	
			close(fd);						// Close the opened file
			return 1;
		}
	}

	return -1;
}

int
update_db19(struct record * row, int acctnum, float val) {
	int fd;

	// try to Open db19 with read only mode, if fail, exit(1)
	// When the file is open, the current file offset is initialized to be 0
	if ( (fd = open("db19", O_RDWR)) == -1 ) {
		perror("Failed to OPEN db19");
		exit(1);
	}
	
	lseek(fd, 0, SEEK_SET);		// JUST IN CASE

	while( read(fd, row, sizeof(struct record)) > 0 ) {
		// if query successfully, add the val to the value

		if (row->acctnum == acctnum) {	// query seccessfully
			// move cfo to the matched record
			lseek(fd, -sizeof(struct record), SEEK_CUR);
			if( lockf(fd, F_LOCK, sizeof(struct record)) == -1 ) {
				perror("lockf Failed");
				exit(1);
       		}

			float new_val = row->value + val;
			lseek(fd, sizeof(int)+sizeof(char) * 20, SEEK_CUR);
	  		write(fd, &new_val, sizeof(float));

      		lseek(fd, -(sizeof(int) + sizeof(char) * 20 + sizeof(float)), SEEK_CUR);
      		read(fd, row, sizeof(struct record));	// read the new data for sending

			lockf(fd, F_ULOCK, -sizeof(struct record));
			
			close(fd);						// Close the opened file

			return 1;
		}
	}
	return -1;

}
