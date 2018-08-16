#include<unistd.h>
#include<fcntl.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define PORT 8080
int main()
{

	struct sockaddr_in serveraddr,ftpaddr;
	int sock,result,addrlen,ftpclient;
	
	sock=socket(AF_INET,SOCK_STREAM,0);
	addrlen=sizeof(serveraddr);
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(PORT);
	inet_pton(AF_INET,"127.0.0.1",&serveraddr.sin_addr);

	ftpclient=socket(AF_INET,SOCK_STREAM,0);
	ftpaddr.sin_port=htons(5000);
	ftpaddr.sin_family=AF_INET;
	result=inet_pton(AF_INET,"127.0.0.1",&ftpaddr.sin_addr);
	if(result == -1)
	{
		printf("Failed to resolve address\n");
		exit(-1);
	}
	result=connect(sock,(struct sockaddr *)&serveraddr,(socklen_t)addrlen);
	if(result == -1)
	{
		printf("Socket connect failed\n");
		exit(-1);
	}
	addrlen=sizeof(ftpaddr);
	if(fork())
	{
		result=connect(ftpclient,(struct sockaddr *)&ftpaddr,(socklen_t)addrlen);
		if(result == -1)
		{
			printf("FTP Socket connection failed\n");
			perror("");
			exit(-1);
		}
		while(1)
		{
			printf(">");
			char command[100]={0};
			gets(command);
			printf("You entered %s ",command);
			send(ftpclient,command,100,0);
			char *cmd=strtok(command," ");
			if(strcmp(cmd,"FETCH")==0)
			{
				system("pwd");
				printf("Hi");
				char *filename=strtok(NULL," ");
				int fd=open(filename,O_CREAT|O_RDWR);	
				char buf[10000]={0};
				recv(ftpclient,buf,10000,0);
				write(fd,buf,strlen(buf));
				close(fd);
			}
			else
			{
				char buf[10000];
				recv(ftpclient,buf,10000,0);
				printf("%s",buf);
			}
		}
	}
	else
	{
		wait(NULL);
		close(ftpclient);
	}
	return 0;
}