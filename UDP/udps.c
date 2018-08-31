#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#define PORT 8000
int main()
{
	struct sockaddr_in serveraddr,cliaddr;
	serveraddr.sin_port=htons(8000);
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_addr.s_addr=INADDR_ANY;
	memset(&cliaddr, 0, sizeof(cliaddr));
	int sock=socket(AF_INET,SOCK_DGRAM,0);
	bind(sock,(const struct sockaddr *)&serveraddr,sizeof(serveraddr));
	char mesg[100];
	int addrlen;
	int n=recvfrom(sock,mesg,100,0,(struct sockaddr *)&cliaddr,&addrlen);
	fprintf(stderr,"Client says %s\n",mesg);
	strcpy(mesg,"Hello Client");
	sendto(sock,mesg,strlen(mesg),0,(struct sockaddr *)&cliaddr,addrlen);
	return 0;
}