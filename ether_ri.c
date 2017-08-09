#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <linux/ip.h>
#include <linux/icmp.h>
#include <linux/if_ether.h>

#define MAXCOUNT 65536 

int main()
{

        int raw_socket;

        struct sockaddr address;
	int size;
	unsigned char rbuf[MAXCOUNT];
	int s,i;
	int count=0;
	
        raw_socket = socket( AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
//	setsockopt(raw_socket, SOL_SOCKET, SO_BINDTODEVICE, "eth0", strlen("eth0")+1);


	if (raw_socket <0) perror("socket");

	memset(rbuf, 0, sizeof(rbuf));
	while (1) {
	s=recvfrom(raw_socket, rbuf, MAXCOUNT,0, &address, &size);
	if (s<0) perror("recvfrom");
	{
		if(rbuf[12]==8&&rbuf[23]==1) {
			printf("\n ===============================");
			printf("\n Packet #%d", ++count);
			printf("\n Length = %d \n", s);
			for (i=0; i<64; i++) {
				printf("[%02x]", rbuf[i]);
				if ((i+1) % 16==0) printf("\n");
			}
		}
	}
	}
}
