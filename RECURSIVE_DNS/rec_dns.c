#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include<string.h>
#define PORT 8080
#define COM 5000
#define ORG 6000
int main()
{
	struct sockaddr_in serveraddr;
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_addr.s_addr=INADDR_ANY;
	serveraddr.sin_port=htons(PORT);
	int sock,result,addrlen;
	addrlen=sizeof(serveraddr);
	sock=socket(AF_INET,SOCK_STREAM,0);
	perror("Socket Creation");
	bind(sock,(struct sockaddr *)&serveraddr,(socklen_t)addrlen);
	perror("Bind");
	listen(sock,5);
	perror("Listen");
	int clientsock=accept(sock,(struct sockaddr *)&serveraddr,(socklen_t*)&addrlen);
	perror("Accept");
	fprintf(stderr,"Listening on port %d",PORT);
	char url[100]={0};
	char url2[100];
	recv(clientsock,url,100,0);
	strcpy(url2,url);
	perror("RECV");
	char *domain;
	char *tld;
	domain=strtok(url,".");
	tld=strtok(NULL,".");
	fprintf(stderr,"%s\n",tld);
	int nextport;
	if(strcmp(tld,"com")==0)
	{
		nextport=COM;
	}
	else 
	{
		nextport=ORG;
	}
	struct sockaddr_in tldserver;
	tldserver.sin_family=AF_INET;
	tldserver.sin_port=htons(nextport);
	inet_pton(AF_INET,"127.0.0.1",&tldserver.sin_addr);
	perror("INET_PTON");
	int tldsock;
	tldsock=socket(AF_INET,SOCK_STREAM,0);
	perror("TLD SOCKET");
	fprintf(stderr,"%d\n",nextport);
	connect(tldsock,(struct sockaddr *)&tldserver,(socklen_t)sizeof(tldserver));
	perror("CONNECT");
	send(tldsock,url2,100,0);
	perror("SEND");
	char ip[100]={0};
	recv(tldsock,ip,100,0);
	perror("TLD RECV");
	fprintf(stderr,"The IP is %s",ip);
	send(clientsock,ip,100,0);
	return 0;
}