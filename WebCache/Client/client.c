#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<string.h>
int main()
{
	struct sockaddr_in serveraddr;
	int sock;
	serveraddr.sin_port=htons(8080);
	serveraddr.sin_family=AF_INET;
	inet_pton(AF_INET,"127.0.0.1",&serveraddr.sin_addr);
	sock=socket(AF_INET,SOCK_STREAM,0);
	int addrlen=sizeof(serveraddr);
	connect(sock,(struct sockaddr *)&serveraddr,(socklen_t )addrlen);
	send(sock,"sample.txt",strlen("sample.txt"),0);
	char content[1000]={0};
	recv(sock,content,1000,0);
	printf("%s",content);
	return 0;
}