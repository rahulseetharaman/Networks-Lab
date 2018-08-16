#include<iostream>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<algorithm>
#include<string>
#include<stdio.h>
#define PORT 8080
using namespace std;
int main()
{
	struct sockaddr_in serveraddr;
	int sock,addrlen;
	sock=socket(AF_INET,SOCK_STREAM,0);
	perror(" ");
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(PORT);
	inet_pton(AF_INET,"127.0.0.1",&serveraddr.sin_addr);
	addrlen=sizeof(struct sockaddr_in);
	perror(" ");
	int result;
	result=connect(sock,(struct sockaddr *)&serveraddr,(socklen_t)addrlen);
	perror(" ");
	while(true)
	{
		cout<<"Enter a filename"<<endl;
		string s;
		cin>>s;
		char buf[100]={0};
		send(sock,s.c_str(),s.length(),0);
		perror(" ");
		recv(sock,buf,100,0);
		printf("%s\n",buf);
	}
	close(sock);
	return 0;
}