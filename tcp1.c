  #include <stdio.h>
  #include <stdlib.h>
  #include <pcap.h>
  #include <sys/socket.h>
  #include <netinet/ether.h>
  #include <netinet/in.h>
  #include <netinet/ip.h>
  #include <netinet/ip_icmp.h>
  #include <linux/tcp.h>
  #include <arpa/inet.h>
        
  #define PCAP_CNT_MAX 10
  #define PCAP_SNAPSHOT 1024
  #define PCAP_TIMEOUT 100
  #define FILTER_RULE "icmp"  

        
  void packet_view(unsigned char *, const struct pcap_pkthdr *, const unsigned char *);
        
  int main(int argc, char *argv[]) {
                char *dev;
                char errbuf[PCAP_ERRBUF_SIZE];
                bpf_u_int32 net;
                bpf_u_int32 netmask;
                struct in_addr net_addr, mask_addr;
                pcap_t *pd;
        
                if(!(dev = pcap_lookupdev(errbuf))) {
                        perror(errbuf);
                        exit(1);
                }
        
                if(pcap_lookupnet(dev, &net, &netmask, errbuf) < 0) {
                        perror(errbuf);
                        exit(1);
                }
        
                net_addr.s_addr = net;
                mask_addr.s_addr = netmask;
       /* 
                printf("Device : %s\n", dev);
                printf("Net Address : %s\n", inet_ntoa(net_addr));
                printf("Netmask : %s\n", inet_ntoa(mask_addr));
	*/
        
                if((pd = pcap_open_offline("bob.pcap", errbuf)) == NULL) {
                        perror(errbuf);
                        exit(1);
                }
        
                if(pcap_loop(pd, PCAP_CNT_MAX, packet_view, 0) < 0) {
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
        void packet_view(unsigned char *user, const struct pcap_pkthdr *h, const unsigned char *p)
         {
                int len;
       
		struct ethhdr *ethhdr;
		struct iphdr *iphdr;
		struct icmphdr *icmphdr;
		struct tcphdr *tcphdr;
		struct in_addr src, dest;

		ethhdr = (struct ethhdr *) p;
		iphdr  = (struct iphdr *) (p+14);
		tcphdr = (struct tcphdr *) (p+14+iphdr->ihl*4);

		src.s_addr = iphdr->saddr;
		dest.s_addr = iphdr->daddr;

	
		printf("===============================================\n");	
		printf("SRC	  : %s:%d \n", inet_ntoa(src), ntohs(tcphdr->source));
		printf("DEST	  : %s:%d \n", inet_ntoa(dest), ntohs(tcphdr->dest));
		printf("PROTOCOL  : %d \n", iphdr->protocol);
		printf("SEQ	  : %ld \n", (unsigned int) ntohl(tcphdr->seq));
		printf("ACK	  : %ld \n", (unsigned int) ntohl(tcphdr->ack_seq));
		printf("syn(%d),ack(%d) \n", tcphdr->syn, tcphdr->ack);

		printf("-----------------------------------------------\n");		 
                len = 0;
                printf("PACKET\n");
                while(len < h->len) {
                        printf("%02x ", *(p++));
                        if(!(++len % 16))
                                printf("\n");
                }
                printf("\n");
        
                return ;
        }
