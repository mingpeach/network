#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#define BUFFSIZE 65535

int main(int argc, char *argv[]) {

        int fd;
	struct ip *iphdr; 
        char buff[BUFFSIZE];
        char *p = buff;
	int i;

        if ((fd=open("./icmp2.pcap", O_RDONLY))>0)
        {
                read(fd, buff, BUFFSIZE);
        }
	p += 24 + 16 + 14;
	/* global header + pcap header + ethernet header */
	iphdr =(struct ip *) p; 
	

	printf("\n");
	printf("DEST MAC=%s\n",inet_ntoa(iphdr->ip_src));
	printf("SRC  MAC=%s\n",inet_ntoa(iphdr->ip_dst));
	printf("PROTOCOL=%d\n",iphdr->ip_p);


	
}
