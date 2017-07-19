        #include <stdio.h>
        #include <stdlib.h>
        #include <fcntl.h>
	#include <netinet/ether.h>
        #include <arpa/inet.h>
        #define BUFFSIZE 65535

        int main(int argc, char *argv[]) {



                int fd;
		struct ethhdr *ethhdr; 
                char buff[BUFFSIZE];
                char *p = buff;
		int i;

                if ((fd=open("./icmp2.pcap", O_RDONLY))>0)
                {
                        read(fd, buff, BUFFSIZE);
                }

		p+=24+16;
		ethhdr =(struct ethhdr *) p; 
		
		/*
		for(i=0;i<46;i++) {
			printf("%02x", (unsigned char)p[i]);
			if(i!=45) printf(":");
		}
		*/

		printf("\n");
		printf("DEST MAC=%s\n",ether_ntoa((struct ether_addr *) ethhdr->h_dest));
		printf("SRC  MAC=%s\n",ether_ntoa((struct ether_addr *) ethhdr->h_source));
		printf("PROTOCOL=%04x\n",ntohs(ethhdr->h_proto));


		
        }
