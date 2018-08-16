#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<stdio.h>
#define AOSA 8001
#define WIKIPEDIA 8002
int main()
{
	struct sockaddr_in serveraddr;
	int result,serversock;
	serversock=socket(AF_INET,SOCK_STREAM,0);
	perror("SOCKET");
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_addr.s_addr=INADDR_ANY;
	serveraddr.sin_port=htons(6000);
	int addrlen=sizeof(serveraddr);
	result=bind(serversock,(struct sockaddr *)&serveraddr,(socklen_t)addrlen);
    perror("BIND");
    result=listen(serversock,5);
    perror("LISTEN");
    int clientsock=accept(serversock,(struct sockaddr *)&serveraddr,(socklen_t *)&addrlen);
    perror("ACCEPT");
    char url[100]={0};
    char url2[100];
    int port;
    recv(clientsock,url,100,0);
    perror("RECV");
    char *tld,*name;
    name=strtok(url,".");
    tld=strtok(NULL,".");
    fprintf(stderr,"TLD is %s\n",tld);
    if(strcmp(name,"aosabook")==0)
    {
        port=AOSA;
    }
    else
    {
        port=WIKIPEDIA;
    }
    strcpy(url2,url);
    struct sockaddr_in webaddr;
    webaddr.sin_port=htons(port);
    webaddr.sin_family=AF_INET;
    inet_pton(AF_INET,"127.0.0.1",&webaddr.sin_addr);
    int websock=socket(AF_INET,SOCK_STREAM,0);
    addrlen=sizeof(webaddr);
    connect(websock,(struct sockaddr *)&webaddr,(socklen_t)addrlen);
    char ip[100];
    recv(websock,ip,100,0);
    send(clientsock,ip,100,0);
    perror("SEND");
    return 0;
}