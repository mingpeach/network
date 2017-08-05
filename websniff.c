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
			printf("%s pcap_file \n", argv[0]);
			exit(1);
		}

	    if ((pd=pcap_open_offline(argv[1], errbuf))
		<0)
		{ 
		fprintf(stderr,"\nUnable to open the file %s.\n", argv[1]);
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
        
                // p媛� ip header瑜� 媛�由ы궎�꾨줉 ethernet header�� �ш린留뚰겮 利앷� �쒗궡
                p += sizeof(struct ether_header);
                iph = (struct ip *) p;
		
		p+= iph->ip_hl * 4;
		tcph = (struct tcphdr *) p;

                // ip header, tcp header�� �뺣낫瑜� 異쒕젰

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
