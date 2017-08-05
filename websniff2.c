	#include <stdio.h>
	#include <string.h>
	#include <stdlib.h>
        #include <pcap.h>
        #include <sys/socket.h>
        #include <netinet/in_systm.h>
        #include <netinet/in.h>
        #include <netinet/ip.h>
       // #include <netinet/tcp.h>
	#include <netinet/ip_icmp.h>
        #include <net/if.h>
        #include <netinet/if_ether.h>
        #include <arpa/inet.h>
	#include <linux/tcp.h>
        // filtering rule
        #define FILTER_RULE "tcp and port 80"
        #define PCAP_SNAPSHOT 1024
        #define PCAP_TIMEOUT 100
        
        void packet_view(unsigned char *, const struct pcap_pkthdr *, const unsigned char *);
        
        int main(int argc, char *argv[]) {
                char *dev;
                char errbuf[PCAP_ERRBUF_SIZE];
                bpf_u_int32 net;
                bpf_u_int32 netmask;
                struct in_addr net_addr, mask_addr;
                pcap_t *pd;
                struct bpf_program fcode;
		char source[100];

        
         	if (argc < 2) {



		        if(!(dev = pcap_lookupdev(errbuf))) {
                        perror(errbuf);
                        exit(1);
                        }


	                if((pd = pcap_open_live(dev, PCAP_SNAPSHOT, 1, PCAP_TIMEOUT, errbuf)) == NULL) 				{
                        perror(errbuf);
                        exit(1);
                	}

		}
	
	    else 
	    {if ((pd=pcap_open_offline(argv[1], errbuf))
		<0)
		{ 
		fprintf(stderr,"\nUnable to open the file %s.\n", argv[1]);
		exit(-1);
		}
	    }

                if(pcap_compile(pd, &fcode, FILTER_RULE, 0, netmask) < 0) {
                        perror(pcap_geterr(pd));
                        exit(1);
                }
                // set filtering rule
                if(pcap_setfilter(pd, &fcode) < 0) {
                        perror(pcap_geterr(pd));
                        exit(1);
                }
                // read packet forever
		if(pcap_loop(pd, 0, packet_view, 0) <0) {
                        perror(pcap_geterr(pd));
                        exit(1);
                }
        
                pcap_close(pd);
        
                return 1;
        }
        /*
         * packet_view
         * print packets
         */
        void packet_view(unsigned char *user, const struct pcap_pkthdr *h, const unsigned char *p) {
                struct ip *iph;
                struct tcphdr *tcph;
		int size_tcpdata=0;
		int matchp=-1;
		int i;
        
                // pê°€ ip headerë¥¼ ê°€ë¦¬í‚¤ë„ë¡ ethernet headerì˜ í¬ê¸°ë§Œí¼ ì¦ê°€ ì‹œí‚´
                p += sizeof(struct ether_header);
                iph = (struct ip *) p;
		
		p+= iph->ip_hl * 4;
		tcph = (struct tcphdr *) p;

                // ip header, tcp headerì˜ ì •ë³´ë¥¼ ì¶œë ¥

                p += 20;
		size_tcpdata = ntohs(iph->ip_len) - iph->ip_hl*4 - 20;

	        printf("SRC = %s:%d -> ", inet_ntoa(iph->ip_src), ntohs(tcph->source));
                printf("DST = %s:%d \n", inet_ntoa(iph->ip_dst), ntohs(tcph->dest));
		if (size_tcpdata>0) {

			printf("---------------------------------------------------------\n");	
			for (i=0;i<size_tcpdata;i++)
				printf("%c",*(p+i));
		
				printf("\n");
			}
			printf("---------------------------------------------------------\n");

        }
