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
#include<ctime>
#define PORT 8000
using namespace std;
int main()
{
	struct sockaddr_in serveraddr;
	int serversock,addrlen;
	serversock=socket(AF_INET,SOCK_STREAM,0);
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(PORT);
	serveraddr.sin_addr.s_addr=INADDR_ANY;
	addrlen=sizeof(struct sockaddr_in);
	bind(serversock,(struct sockaddr *)&serveraddr,(socklen_t)addrlen);
	perror(" ");
	listen(serversock,3);
	perror(" ");
	int clientsock;
	clientsock=accept(serversock,(struct sockaddr *)&serveraddr,(socklen_t *)&addrlen);
	while(true)
	{
		char buf[100]={0};
		recv(clientsock,buf,100,0);
		string filename(buf);
		ifstream inf(buf);
		if(!inf)
		{
			bool status=false;
			send(clientsock,&status,sizeof(status),0);
			send(clientsock,"File does not exist",20,0);
		}
		else
		{
			string content="";
			while(inf)
			{
				string temp;
				inf>>temp;
				content+=(" "+temp);
			}
			bool status=true;
			send(clientsock,&status,sizeof(status),0);
			send(clientsock,content.c_str(),content.length(),0);
		}
	}
	return 0;
}