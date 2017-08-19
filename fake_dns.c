/* Sample UDP server */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#define SPOOFED_IP "1.2.3.4"

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

   bzero(&servaddr,sizeof(servaddr));
   bzero(&cliaddr, sizeof(cliaddr));
   bzero(buf,1000);
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
   servaddr.sin_port=htons(53);
   bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));

   for (;;)
   {
      len = sizeof(cliaddr);
      n = recvfrom(sockfd,mesg,1000,0,(struct sockaddr *)&cliaddr,&len);
     // sendto(sockfd,mesg,n,0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
      printf("-------------------------------------------------------\n");
      mesg[n] = 0;
      printf("Received [%d] \n",n);
      printf("-------------------------------------------------------\n");
          for (i=0; i<n; i++) {
                        printf("[%02x]", mesg[i]);
                        if ((i+1) % 16==0) printf("\n");
                        }

      printf("\n");
      // byte 12
      idx = 12;
      while(mesg[idx]!=0) idx++;
      printf("[%d:%x]", idx, mesg[idx]); 

      memcpy(buf, mesg, idx+1);
      memcpy(&buf[2],"\x81\x80\x00\x01\x00\x01",6); 
      memcpy(&buf[idx+1],"\x00\x01\x00\x01\xc0\x0c\x00\x01\x00\x01\x00\x00\x00\xff\x00\x04",16);
      sip = (unsigned long *) &buf[idx+17];
//      *sip =inet_addr(SPOOFED_IP);
    
     memcpy(&buf[idx+17],"\xaf\x7e\x4a\xa4",4);

      n = idx+21;
     
      printf("\n"); 
      printf("-------------------------------------------------------\n");

      printf("Send [%d] \n",n);
      printf("-------------------------------------------------------\n");
          for (i=0; i<n; i++) {
                        printf("[%02x]", buf[i]);
                        if ((i+1) % 16==0) printf("\n");
                        }

   sendto(sockfd,buf,n,0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));

   }
}
