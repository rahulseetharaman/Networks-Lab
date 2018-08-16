#include<unistd.h>
#include<fcntl.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/sendfile.h>
#include<netinet/in.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define PORT 8080
int main()
{
	struct sockaddr_in serveraddr,ftpaddr;
	int server_fd,sock,clientsock,addrlen,ftpaddrlen,ftpsock,result;
	server_fd=socket(AF_INET,SOCK_STREAM,0);
	ftpsock=socket(AF_INET,SOCK_STREAM,0);
	addrlen=sizeof(struct sockaddr_in);
	ftpaddrlen=sizeof(struct sockaddr_in);
	if(server_fd == -1)
	{
		printf("Failed to create socket\n");
		exit(-1);
	}
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(PORT);
	serveraddr.sin_addr.s_addr=INADDR_ANY;
	ftpaddr.sin_family=AF_INET;
	ftpaddr.sin_port=htons(5000);
	ftpaddr.sin_addr.s_addr=INADDR_ANY;
	result=bind(server_fd,(struct sockaddr *)&serveraddr,(socklen_t)addrlen);
	printf("Hello\n");
	if(result == -1)
	{
		printf("Failed to bind\n");
		exit(-1);
	}
	result=listen(server_fd,3);
	if(result == -1)
	{
		perror("");
		printf("Listen failed for server fd\n");
		exit(-1);
	}
	sock=accept(server_fd,(struct sockaddr *)&serveraddr,(socklen_t *)&ftpaddrlen);
	if(sock == -1 )
	{
		printf("Failed to accept");
		perror("");
		exit(-1);
	}
	printf("Hello\n");
	result=bind(ftpsock,(struct sockaddr *)&ftpaddr,(socklen_t)ftpaddrlen);
	if(result == -1)
	{
		printf("Failed to bind data channel\n");
		exit(-1);
	}
	result=listen(ftpsock,3);
	if(result == -1)
	{
		perror("Listen failed");
		exit(-1);
	}
	printf("Hello\n");
	clientsock=accept(ftpsock,(struct sockaddr *)&ftpaddr,(socklen_t *)&addrlen);
	if(clientsock == -1)
	{
		printf("Failed to communicate to server\n");
		exit(-1);
	}
	printf("Hello\n");
	while(1)
	{
		char command[100]={0};
		printf("Hi");
		recv(clientsock,command,100,0);
		printf("Command is %s",command);
		char *cmd=strtok(command," ");
		char *args=strtok(NULL," ");
		printf("Args is %s",args);
		if(strcmp(cmd,"LIST")==0)
		{
			FILE *fp=popen("ls","r");
			char buf[10000]={0};
			char temp[100]={0};
			while((fgets(temp,100,fp)))
			{
				strcat(buf,temp);	
			}
			send(clientsock,buf,10000,0);
		}
		else if(strcmp(cmd,"CHDIR")==0)
		{
			chdir(args);
			char buf[1000]={0};
			getwd(buf);
			char mesg[1000];
			sprintf(mesg,"Changed working directory to %s\n",buf);
			send(clientsock,mesg,strlen(mesg)+1,0);
		}
		else if(strcmp(cmd,"PWD")==0)
		{
			char buf[1000];
			getwd(buf);
			char mesg[1000];
			sprintf(mesg,"Current directory is %s\n",buf);
			send(clientsock,mesg,strlen(mesg)+1,0);
		}
		else if(strcmp(cmd,"FETCH")==0)
		{
			char *filename=args;
			int fd=open(args,O_RDWR);
			if(fd < 0)
			{
				char error[]="Failed to fetch file";
				write(STDIN_FILENO,error,strlen(error)+1);
				sendfile(clientsock,STDIN_FILENO,0,strlen(error)+1);
			}
			else
			{
				sendfile(clientsock,fd,0,10000);
			}
		}
		else
		{
			printf("Hi");
			char error[]="Please enter a valid command";
			write(STDIN_FILENO,error,strlen(error)+1);
			sendfile(clientsock,STDIN_FILENO,0,strlen(error)+1);
		}
	}
	return 0;
}