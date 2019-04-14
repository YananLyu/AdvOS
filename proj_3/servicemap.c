#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


/* This program creates an Internet domain datagram socket, binds a
   name to it, reads from it, and then replies to the sender. */

#define MSG         "OK"
#define MSGFAILED   "*** ERROR: The server you want to GET does not exist in serivicemap ***"
#define MSGPUTGET   "Wrong! Please specify the PUT/GET"
#define TABLESIZE   100 // number of servers

#define BUFMAX 2048
#define UDP_PORT 35639 //3 + last 4 digits of ID

int main() {
    struct sockaddr_in local, remote;
    int sk, rlen = sizeof(remote), len = sizeof(local);
    char buf[BUFMAX];

    int i = 0;
    char name[TABLESIZE][32];
    char ip_and_port[TABLESIZE][32];

    /* Create an Internet domain datagram socket from which to read */
    sk = socket(AF_INET, SOCK_DGRAM, 0);

    local.sin_family = AF_INET; /* Define the socket domain */
    local.sin_addr.s_addr = INADDR_ANY; /* Wildcard mach. addr */
    local.sin_port = ntohs(UDP_PORT); /* Let the system assign a port */
    printf("PORT: %d\n", ntohs(local.sin_port));
    if (bind(sk, (struct sockaddr * ) & local, sizeof(local)) < 0) {
        printf("bind fails!\n");
        exit(1);
    }

    //getsockname(sk,(struct sockaddr *)&local,&len); /* Get the port number assigned */
    printf("socket has port %d\n", htons(local.sin_port)); /* Publish the number */


    /* Read from the socket */
    do {
        recvfrom(sk, buf, BUFMAX, 0, (struct sockaddr * ) & remote, & rlen);
        printf("Received from %s: %s\n", inet_ntoa(remote.sin_addr), buf);

        char * token;
        token = strtok(buf, " ");

        if (strcmp(token, "PUT") == 0) {

            // server name. e.g BANK620
            token = strtok(NULL, " ");
            strcpy(name[i], token);

            // IP:PORT
            token = strtok(NULL, " ");
            strcpy(ip_and_port[i], inet_ntoa(remote.sin_addr));
            strcat(ip_and_port[i], ":");
            strcat(ip_and_port[i], token);
            
            printf("name: %s\n", name[i]); // TODO: del

            printf("ipport: %s\n", ip_and_port[i]); // TODO: del
            i++;
            // register succeddfully, then reply OK to server.
            sendto(sk, MSG, strlen(MSG) + 1, 0, (struct sockaddr * ) & remote, sizeof(remote));

        } else if (strcmp(token, "GET") == 0) {
            // server name. e.g BANK620
            token = strtok(NULL, " ");
            for (int j = 0; j <= i; j++) {
                if(strcmp(token, name[j]) == 0) {
                    sendto(sk, ip_and_port[j], strlen(ip_and_port[j])+1, 0, (struct sockaddr * ) & remote, sizeof(remote));
                    break;
                }
            }
            sendto(sk, MSGFAILED, strlen(MSGFAILED)+1, 0, (struct sockaddr * ) & remote, sizeof(remote));
        } else {
            sendto(sk, MSGPUTGET, strlen(MSGPUTGET)+1, 0, (struct sockaddr * ) & remote, sizeof(remote));
        }
    } while (1);

    close(sk);
}