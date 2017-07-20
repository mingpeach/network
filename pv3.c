#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <netinet/ether.h>
#include <arpa/inet.h>
#include <linux/ip.h>
#define BUFFSIZE 65535

int main(int argc, char *argv[]) {



        int fd;
	struct iphdr *iphdr; 
        char buff[BUFFSIZE];
        char *p = buff;
	int i;

        if ((fd=open("./icmp2.pcap", O_RDONLY))>0)
        {
                read(fd, buff, BUFFSIZE);
        }
	p += 24 + 16 + 14;
	/* global header + pcap header + ethernet header */
	iphdr =(struct iphdr *) p; 
	

	printf("\n");
	printf("DEST MAC=%x\n",ntohl(iphdr->daddr));
	printf("SRC  MAC=%x\n",ntohl(iphdr->saddr));
	printf("PROTOCOL=%d\n",iphdr->protocol);


	
}
