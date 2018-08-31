#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<time.h>
#include<string.h>

int checkLastModified(int server,char filename[])
{

	send(server,filename,100,0);
	time_t lmd;
	recv(server,&lmd,sizeof(int),0);
	struct stat statbuf;
	stat(filename,&statbuf);
	time_t pmd=statbuf.st_ctime;
	printf("Cache Last Modified Date %s\n Server Last Modified Date %s",ctime(&lmd),ctime(&pmd));
	return lmd==pmd;
}	
void fetchFromServer(char filename[])
{
	char buffer[100]="cp -p ";
	char source[100]=" ../Server/";
	char dest[100]=" ";
	strcat(source,filename);
	strcat(dest,filename);
	strcat(buffer,source);
	strcat(buffer,dest);
	system(buffer);
}
int main()
{
	struct sockaddr_in serveraddr;
	int sock;
	serveraddr.sin_port=htons(8080);
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_addr.s_addr=INADDR_ANY;
	sock=socket(AF_INET,SOCK_STREAM,0);
	int addrlen=sizeof(serveraddr);
	bind(sock,(struct sockaddr*)&serveraddr,(socklen_t)addrlen);
	listen(sock,5);
	int client=accept(sock,(struct sockaddr*)&serveraddr,(socklen_t *)&addrlen);
	char filename[100]={0};
	recv(client,filename,100,0);

	// now connect to main server running on 8000

	struct sockaddr_in main;
	int serversock;
	serveraddr.sin_port=htons(8000);
	serveraddr.sin_family=AF_INET;
	inet_pton(AF_INET,"127.0.0.1",&serveraddr.sin_addr);
	serversock=socket(AF_INET,SOCK_STREAM,0);
	addrlen=sizeof(serveraddr);
	connect(serversock,(struct sockaddr *)&serveraddr,(socklen_t)addrlen);
	int result=checkLastModified(serversock,filename);
	if(access(filename,F_OK)==-1 || result==0)
	{
		fetchFromServer(filename);
	}
	int fd=open(filename,O_RDWR);
	char buf[1000]={0};
	read(fd,buf,1000);
	send(client,buf,1000,0);
	return 0;

}