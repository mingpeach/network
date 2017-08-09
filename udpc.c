#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	int sockfd, n;
	struct sockaddr_in servaddr, cliaddr;
	char sendline[1000];
	char recvline[1000];

	if (argc!=2)
	{
		printf("usage: udpc <IP address> \n");
		exit(1);
	}

	sockfd =socket(AF_INET,SOCK_DGRAM,0);
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr=inet_addr(argv[1]);
	servaddr.sin_port = htons(50000);
	
	while(fgets(sendline, 1000, stdin) !=NULL)
	{
		if (sendto(sockfd, sendline, strlen(sendline),0,
			(struct sockaddr *) &servaddr, sizeof(servaddr))<0)
			printf(" Error \n");
		n=recvfrom(sockfd, recvline, 1000,0,NULL,NULL);
		printf(" R:%d\n",n);
		recvline[n]=0;
		fputs(recvline,stdout);
	}
}
