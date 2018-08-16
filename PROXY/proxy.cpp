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
#include<unordered_map>
#include<fstream>
#include<ctime>
#define PORT 8080
#define SPORT 8000
using namespace std;
long int unix_timestamp()
{
    time_t t = std::time(0);
    long int now = static_cast<long int> (t);
    return now;
}
void update_cache(unordered_map<string,int> &cache,string newfile)
{
	if(cache.size() < 5)
	{
		cache[newfile]=unix_timestamp();
		return;
	}
	int mints=unix_timestamp();
	string filename;
	for(auto it=cache.begin();it!=cache.end();it++)
	{
		if(it->second < mints)
		{
			mints=it->second;
			filename=it->first;
		}
	}
	cache.erase(filename);
	cache[newfile]=unix_timestamp();
}
string read_from_file(string filename)
{
	ifstream inf(filename);
	string content="";
	while(inf)
	{
		string temp;
		inf>>temp;
		content+=temp;
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
	perror(" ");
	ifstream inf("cache");
	unordered_map<string,int>cmap;
	while(inf)
	{
		string filename;
		inf>>filename;
		if(filename.length() == 0)
			break;
		string ts;
		inf>>ts;
		int timestamp=stoi(ts);
		cmap[filename]=timestamp;
	}
	while(true)
	{
		char filename[100]={0};
		recv(clientsock,filename,100,0);
		perror("Error:");
		printf("Requested for %s\n",filename);
		string s(filename);
		if(cmap.find(s)==cmap.end())
		{
			printf("File not in cache\n");
			update_cache(cmap,filename);	
		}
		else
		{
			printf("File found in cache\n");
		}
		string content=read_from_file(filename);
		send(clientsock,content.c_str(),content.length(),0);
	}
	return 0;
}