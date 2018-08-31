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
	struct sockaddr_in serveraddr;
	serveraddr.sin_addr.s_addr=INADDR_ANY;
	serveraddr.sin_port=htons(8000);
	serveraddr.sin_family=AF_INET;
	int sock=socket(AF_INET,SOCK_DGRAM,0);
	char mesg[]="Hello Server";
	int addrlen=sizeof(serveraddr);
	sendto(sock,mesg,strlen(mesg),0,(struct sockaddr *)&serveraddr,addrlen);
	recvfrom(sock,mesg,strlen(mesg),0,(struct sockaddr *)&serveraddr,&addrlen);
	fprintf(stderr,"Server says %s",mesg);
	return 0;
}
