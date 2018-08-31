#include<stdio.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
int main()
{
	struct sockaddr_in serveraddr;
    int sock=socket(AF_INET,SOCK_STREAM,0);
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(8080);
	inet_pton(AF_INET,"127.0.0.1",&serveraddr.sin_addr);
	perror("Create Socket");
	int addrlen=sizeof(serveraddr);
	connect(sock,(struct sockaddr*)&serveraddr,(socklen_t)addrlen);
	printf("Enter a client id");
	int id;
	scanf("%d",&id);
	perror("Connect");
    char hi[100]={0};
    sprintf(hi,"Hi, this is %d",id);
	send(sock,hi,strlen(hi),0);
	perror("Send");
	char buf[100]={0};
	recv(sock,buf,100,0);
	perror("Recv");
	fprintf(stderr,"%s",buf);
	return 0;
}
