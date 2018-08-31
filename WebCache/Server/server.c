#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include<sys/types.h>
int main()
{
	struct sockaddr_in serveraddr;
	int sock;
	serveraddr.sin_port=htons(8000);
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_addr.s_addr=INADDR_ANY;
	sock=socket(AF_INET,SOCK_STREAM,0);
	int addrlen=sizeof(serveraddr);
	bind(sock,(struct sockaddr*)&serveraddr,(socklen_t)addrlen);
	listen(sock,5);
	int client=accept(sock,(struct sockaddr*)&serveraddr,(socklen_t *)&addrlen);
	char filename[100]={0};
	recv(client,filename,100,0);
	struct stat statbuf;
	stat(filename,&statbuf);
	int lmd=statbuf.st_ctime;
	send(client,&lmd,sizeof(int),0);
	return 0;
}