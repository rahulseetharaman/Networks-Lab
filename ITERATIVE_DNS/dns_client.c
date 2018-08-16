/*
 * dns_client.c
 *
 *  Created on: 09-Aug-2018
 *      Author: rahul
 */

#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
int main()
{
	int sock;
	struct sockaddr_in tldserv;
	sock=socket(AF_INET,SOCK_STREAM,0);
	tldserv.sin_family=AF_INET;
	tldserv.sin_port=htons(8080);
	inet_pton(AF_INET,"127.0.0.1",&tldserv.sin_addr);
	connect(sock,(struct sockaddr *)&tldserv,(socklen_t)sizeof(tldserv));	
    char url[100]="www.google.com";   
    char buf[100];
	send(sock,url,100,0);
	recv(sock,buf,100,0);
	printf("%s",buf);
}
