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
#define WIKIPEDIA 9003
#define AOSABOOK 9004
int main()
{
	struct sockaddr_in serveraddr;
	int result,serversock;
	serversock=socket(AF_INET,SOCK_STREAM,0);
	perror("");
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_addr.s_addr=INADDR_ANY;
	serveraddr.sin_port=htons(5000);
	int addrlen=sizeof(serveraddr);
	result=bind(serversock,(struct sockaddr *)&serveraddr,(socklen_t)addrlen);
    result=listen(serversock,5);
    int clientsock=accept(serversock,(struct sockaddr *)&serveraddr,(socklen_t *)&addrlen);
    char url[100];
    int port;
    recv(clientsock,url,100,0);
    if(strcmp(url,"wikipedia"))
    {
        port=WIKIPEDIA;
    }
    else
    {
        port=AOSABOOK;
    }
    send(clientsock,&port,sizeof(int),0);
    
}

