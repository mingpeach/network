
#include <stdio.h>
#include <stdlib.h>
#include <string.h>           // strcpy, memset(), and memcpy()
#include <arpa/inet.h>        // inet_pton() and inet_ntop()
#include <linux/if_packet.h>  // struct sockaddr_ll 
#include <net/ethernet.h>

// Define a struct for ARP header
struct arphdr {
  uint16_t htype;
  uint16_t ptype;
  uint8_t hlen;
  uint8_t plen;
  uint16_t opcode;
  uint8_t sender_mac[6];
  uint8_t sender_ip[4];
  uint8_t target_mac[6];
  uint8_t target_ip[4];
};

// Define some constants.
#define ETH_HDRLEN 14      // Ethernet header length
#define IP4_HDRLEN 20      // IPv4 header length
#define ARP_HDRLEN 28      // ARP header length
#define ARPOP_REQUEST 1    // Taken from <linux/if_arp.h>

#define SRC_MAC "\x00\x0c\x29\x53\xb3\xdd"
#define DEST_MAC "\xff\xff\xff\xff\xff\xff"

int main (int argc, char **argv)
{
  unsigned char frame[ETH_HDRLEN+ARP_HDRLEN];
  int i, sd, bytes;
  char *interface;
  struct arphdr *arphdr;
  struct sockaddr_ll device;

  memset(&device, 0, sizeof(device));
  device.sll_ifindex = if_nametoindex("ens33");

  arphdr = (struct arphdr *) (frame + 14);

  // ARP header
  arphdr->htype = htons (1);
  arphdr->ptype = htons (ETH_P_IP);
  arphdr->hlen = 6;
  arphdr->plen = 4;
  arphdr->opcode = htons (ARPOP_REQUEST);

  memcpy(arphdr-> sender_mac,SRC_MAC,6);
  memset((char*) arphdr->target_mac, 0, 6);

  inet_pton(AF_INET,"192.168.242.185", arphdr->sender_ip);
  inet_pton(AF_INET,"172.16.242.147", arphdr->target_ip);

  // ethernet frame
  memcpy(frame, DEST_MAC, 6);
  memcpy(frame+6, SRC_MAC, 6);
  memcpy(frame+12, "\x08", 1);
  memcpy(frame+13, "\x06", 1);

  printf("[DEBUG] "); 
  for (i=0;i<42;i++)
	printf("%02x ", frame[i]);
  printf("\n");
   if ((sd = socket (PF_PACKET, SOCK_RAW, htons (ETH_P_ALL))) < 0) {
    perror ("socket() failed ");
   exit (-1);
 }

  if ((bytes = sendto (sd, &frame, sizeof(frame), 0, (struct sockaddr *) &device, sizeof (device))) <= 0) {
    perror ("sendto() failed");
    exit (-1);
  }
  printf("[DEBUG] %d\n", bytes);

  close (sd);

}
