/*
 * iterative_dns.c
 *
 *  Created on: 09-Aug-2018
 *      Author: rahul
 */

#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#define COM 5000
#define ORG 6000
int main()
{
	struct sockaddr_in serveraddr;
	int result,serversock;
	serversock=socket(AF_INET,SOCK_STREAM,0);
	perror("");
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_addr.s_addr=INADDR_ANY;
	serveraddr.sin_port=htons(8080);
	int addrlen=sizeof(serveraddr);
	result=bind(serversock,(struct sockaddr *)&serveraddr,(socklen_t)addrlen);
    result=listen(serversock,5);
    perror("");
    int clientsock=accept(serversock,(struct sockaddr *)&serveraddr,(socklen_t *)&addrlen);
    perror("");
    char url[100];
    recv(clientsock,url,100,0);
    perror("");
    char *name=strtok(url,".");
    char *tldname=strtok(NULL,".");
    int nextport;
    printf("The url is %s",url);
    printf("The TLD name is %s",tldname);
    if(strcmp(tldname,"com")==0)
    {
        nextport=5000;
        printf("%s",tldname);
    }
    else
    {
        nextport=6000;
        printf("%s",tldname);
    }
    char ans[100];
    for(int i=0;i<2;i++)
    {
    	// start a new child process and connect
    	int sock;
    	struct sockaddr_in tldserv;
    	sock=socket(AF_INET,SOCK_STREAM,0);
        printf("Querying %d",nextport);
    	tldserv.sin_family=AF_INET;
    	tldserv.sin_port=htons(nextport);
    	inet_pton(AF_INET,"127.0.0.1",&tldserv.sin_addr);
    	connect(sock,(struct sockaddr *)&tldserv,(socklen_t)sizeof(tldserv));
    	send(sock,url,100,0);
        if(i==0)
        {
            recv(sock,&nextport,sizeof(int),0);
        }   
        else
        {
            recv(sock,ans,100,0);
        }
    }
    printf("\n%s",ans);
    send(clientsock,ans,strlen(ans),0);
}

