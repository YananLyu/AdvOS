#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


/* This program broadcasts a message in datagram to the server and waits
   for reply. */

#define MSG "CIS 620 is a great course!!!"

#define BUFMAX 2048


int main(int argc,char *argv[])
{

  int sk;
  char buf[BUFMAX];

  struct sockaddr_in remote;
  struct hostent *hp;

  /* Create an Internet domain datagram socket */
  sk = socket(AF_INET,SOCK_DGRAM,0);

  remote.sin_family = AF_INET;

  /* Get the remote machine address from its symbolic name 
     given in the command line argument */
  // remote.sin_addr.s_addr = inet_addr("255.255.255.255"); 
  remote.sin_addr.s_addr = inet_addr("137.148..205.255"); 

  /* get the remote port number from the command line */
  remote.sin_port = ntohs(atoi(argv[1]));
  /* setsockopt is required on Linux, but not on Solaris */
  setsockopt(sk,SOL_SOCKET,SO_BROADCAST,(struct sockaddr *)&remote,sizeof(remote));

  sendto(sk,MSG,strlen(MSG)+1,0,(struct sockaddr *)&remote,sizeof(remote));/* Send the message */
  read(sk,buf,BUFMAX); /* Get the reply */
  printf("%s\n",buf); 

  close(sk);
}
