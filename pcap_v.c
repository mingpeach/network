#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>	      // O_RDONLY
#include <unistd.h>           // close()
#include <time.h>
#include <string.h>           // strcpy, memset(), and memcpy()

typedef unsigned int guint32;
typedef unsigned short guint16;
typedef int gint32;

struct pcap_hdr {
        guint32 magic_number;   /* magic number */
        guint16 version_major;  /* major version number */
        guint16 version_minor;  /* minor version number */
        gint32  thiszone;       /* GMT to local correction */
        guint32 sigfigs;        /* accuracy of timestamps */
        guint32 snaplen;        /* max length of captured packets, in octets */
        guint32 network;        /* data link type */
};

struct pcaprec_hdr {
        guint32 ts_sec;         /* timestamp seconds */
        guint32 ts_usec;        /* timestamp microseconds */
        guint32 incl_len;       /* number of octets of packet saved in file */
        guint32 orig_len;       /* actual length of packet */
};

#define BUFF_SIZE 65535
int
main (int argc, char **argv)
{
  unsigned char buff[BUFF_SIZE]; 
  int i=0, fd;
  struct pcap_hdr *pcap_hdr;
  struct pcaprec_hdr *pcap_rec;
  int idx=0;
  int file_len;

  if (argc <2) { exit(0);}

  if ( 0 < ( fd = open( argv[1], O_RDONLY)))
   {
      file_len=read(fd, buff, BUFF_SIZE);
      close(fd);
   }
   else
   {
      printf( "file open failed.\n");
   }

  printf("PCAP File length = %d\n", file_len);
  idx=0;
  pcap_hdr = (struct pcap_hdr *) (buff+idx);

  printf("Magic Number = %x\n", pcap_hdr->magic_number);
  printf("Version = %x.%x\n", pcap_hdr-> version_major, pcap_hdr->version_minor);
  idx=24;

  do {
  pcap_rec = (struct pcaprec_hdr *) (buff+idx);

  printf("#%d\n", ++i);
  printf("Timestamp = %x\n", pcap_rec->ts_sec);
  printf("Capture Length = %x\n", pcap_rec->incl_len);
  printf("Orignal Length = %x\n", pcap_rec->orig_len);
  idx = idx+16; 
  idx = idx+pcap_rec->incl_len;
  } while (idx<file_len);

  printf("\n");
}
