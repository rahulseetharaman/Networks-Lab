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
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(8080);
	serveraddr.sin_addr.s_addr=INADDR_ANY;
	int sock,addrlen;
	sock=socket(AF_INET,SOCK_STREAM,0);
	perror("Create Socket");
	addrlen=sizeof(serveraddr);
	bind(sock,(struct sockaddr *)&serveraddr,(socklen_t)addrlen);
	perror("Bind");
	listen(sock,5);
	perror("Listen");
	int clsock;
	while(1)
	{
		clsock=accept(sock,(struct sockaddr*)&serveraddr,(socklen_t *)&addrlen);
	    if(clsock < 0)
	    	printf("%d",clsock);
		perror("Accept");
		char mesg[100]={0};
		recv(clsock,mesg,100,0);
		fprintf(stderr,"%s",mesg);
		perror("Recv");
		fprintf(stdout,"Enter your id");
		int id;
		fscanf(stdin,"%d",&id);
		char hi[100];
		sprintf(hi,"Hello this is %d",id);
		send(clsock,hi,strlen(hi),0);
		perror("Send");
		fprintf(stderr,"%s",mesg);
		close(clsock);
	}
	return 0;
}
