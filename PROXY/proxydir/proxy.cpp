#include<unistd.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<algorithm>
#include<string>
#include<iostream>
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<fstream>
#include<string.h>
#include<ctime>
#define PORT 8080
#define SPORT 8000
using namespace std;
string read_from_file(string filename)
{
	ifstream inf(filename);
	string content="";
	if(!inf)
	{
		content="File not found";
		return content;
	}
	while(inf)
	{
		string temp;
		inf>>temp;
		content+=(" "+temp);
	}

	return content;
}
int main()
{
	struct sockaddr_in serveraddr;
	int proxysock,addrlen;
	proxysock=socket(AF_INET,SOCK_STREAM,0);
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(PORT);
	serveraddr.sin_addr.s_addr=INADDR_ANY;
	addrlen=sizeof(struct sockaddr_in);
	bind(proxysock,(struct sockaddr *)&serveraddr,(socklen_t)addrlen);
	perror(" ");
	listen(proxysock,3);
	perror(" ");
	int clientsock;
	clientsock=accept(proxysock,(struct sockaddr *)&serveraddr,(socklen_t *)&addrlen);

	struct sockaddr_in serveraddr2;
	int sock;
	sock=socket(AF_INET,SOCK_STREAM,0);
	perror(" ");
	serveraddr2.sin_family=AF_INET;
	serveraddr2.sin_port=htons(SPORT);
	inet_pton(AF_INET,"127.0.0.1",&serveraddr2.sin_addr);
	addrlen=sizeof(struct sockaddr_in);
	perror(" ");
	int result;
	result=connect(sock,(struct sockaddr *)&serveraddr2,(socklen_t)addrlen);
	perror(" ");

	while(true)
	{
		char filename[100]={0};
		recv(clientsock,filename,100,0);
		printf("Requested for %s\n",filename);
		string s(filename);
		ifstream inf(s);
		if(!inf)
		{
			bool status;
			char buf[100]={0};
			send(sock,filename,strlen(filename),0);
			recv(sock,&status,sizeof(bool),0);
			recv(sock,buf,sizeof(buf),0);
			if(status==true)
			{
				char cmd[1000]={0};
				strcpy(cmd,"echo ");
				strcat(cmd,buf);
				strcat(cmd," > ");
				strcat(cmd,filename);
				system(cmd);
			}

		}
		string content=read_from_file(filename);
		cout<<content<<endl;
		send(clientsock,content.c_str(),content.length(),0);
		system("bash lru.sh");
	}
	return 0;
}