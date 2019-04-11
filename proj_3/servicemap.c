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

#define MSG "OK"

#define BUFMAX 2048
#define UDP_PORT	35896   //3 + last 4 digits of ID

int main()
{
  struct sockaddr_in local, remote;
  int sk,rlen=sizeof(remote),len=sizeof(local);
  char buf[BUFMAX];

  /* Create an Internet domain datagram socket from which to read */
  sk = socket(AF_INET,SOCK_DGRAM,0);

  local.sin_family = AF_INET; /* Define the socket domain */
  local.sin_addr.s_addr = INADDR_ANY; /* Wildcard mach. addr */
  local.sin_port = UDP_PORT; /* Let the system assign a port */
  printf("PORT: %d\n", htons(local.sin_port));
  if(bind(sk,(struct sockaddr *)&local,sizeof(local)) <0) {
       printf("bind fails!\n");
       exit(1); 
  }

  //getsockname(sk,(struct sockaddr *)&local,&len); /* Get the port number assigned */
  printf("socket has port %d\n",htons(local.sin_port)); /* Publish the number */


  /* Read from the socket */
  recvfrom(sk,buf,BUFMAX,0,(struct sockaddr *) &remote,&rlen);
  printf("Received from %s: %s\n", inet_ntoa(remote.sin_addr), buf);
  sendto(sk,MSG,strlen(MSG)+1,0,(struct sockaddr *) &remote,sizeof(remote)); 

  close(sk);
}