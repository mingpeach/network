/* Sample UDP server */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#define SERVER_PORT 53

int main(int argc, char**argv)
{
   int sockfd,n;
   struct sockaddr_in servaddr,cliaddr, spoofaddr;
   socklen_t len;
   unsigned char mesg[1000];
   unsigned char buf[1000];
   unsigned long * sip;
   int i;
   int idx;

   sockfd=socket(AF_INET,SOCK_DGRAM,0);

   //initialize
   bzero(&servaddr,sizeof(servaddr));
   bzero(&cliaddr, sizeof(cliaddr));
   bzero(buf,1000);


   // address 
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
   servaddr.sin_port=htons(SERVER_PORT);
   bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));

   for (;;)
   {
      len = sizeof(cliaddr);
      n = recvfrom(sockfd,mesg,1000,0,(struct sockaddr *)&cliaddr,&len);
      printf("-------------------------------------------------------\n");
      mesg[n] = 0;
      printf("Received [%d] \n",n);
      printf("-------------------------------------------------------\n");
          for (i=0; i<n; i++) {
                        printf("[%02x]", mesg[i]);
                        if ((i+1) % 16==0) printf("\n");
                        }

      printf("\n");
      /*
      printf("\n"); 
      printf("-------------------------------------------------------\n");

      printf("Send [%d] \n",n);
      printf("-------------------------------------------------------\n");
          for (i=0; i<n; i++) {
                        printf("[%02x]", buf[i]);
                        if ((i+1) % 16==0) printf("\n");
                        }

   sendto(sockfd,buf,n,0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
      */

   }
}
