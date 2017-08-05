#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/if_ether.h>

#define MAXCOUNT 65536 

int main()
{

        int raw_socket;

//        struct sockaddr address;
	struct iphdr *iph ;
	struct tcphdr *tcph;
	struct sockaddr_in src, dest;

	int size;
	unsigned char rbuf[MAXCOUNT];
	int s,i;
	int iphdrlen;
	int count=0;
	
	raw_socket = socket( AF_INET, SOCK_RAW, IPPROTO_TCP);
	if (raw_socket <0) perror("socket");

	memset(rbuf, 0, sizeof(rbuf));
	while (1) 
	{
//		s=recvfrom(raw_socket, rbuf, MAXCOUNT,0, &address, &size);
		s=recv(raw_socket, rbuf, MAXCOUNT, 0);

		if (s<0) perror("recvfrom");
			printf("\n");	
			
			printf(" [RAW] -------------------------------\n");
			printf("   ");
                        for (i=0; i<50; i++) {
                        printf("[%02x]", rbuf[i]);
                        if ((i+1) % 8==0) printf("\n   ");
                        }

		
	}
}
