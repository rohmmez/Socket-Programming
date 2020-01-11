#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAXDATASIZE 1000 // max number of bytes we can get at once 

int main(int argc, char *argv[])
{
	char buf[MAXDATASIZE]={0};
	char *w="write";
	struct sockaddr_in remote_addr;
	memset(&remote_addr,0,sizeof(remote_addr));
	remote_addr.sin_family=AF_INET;
	remote_addr.sin_port=htons(24037);
	remote_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	int client_socket=socket(AF_INET, SOCK_STREAM,0);
	int connect_socket=connect(client_socket,(struct sockaddr*)&remote_addr,sizeof(struct sockaddr));
	if (client_socket < 0)
	{
		perror("socket");
		return -1;
	}
	printf("The client is up and running\n");
	if (connect_socket < 0)
	{
		perror("connect");
		return -1;
	}
	strcpy(buf,argv[1]);
	send(client_socket,&argc,sizeof(argc),0);
	send(client_socket,buf,sizeof(buf),0);
	if (strcmp(argv[1],w)==0)
	{
		for(int i=0;i<argc-2;i++)
		{	
			strcpy(buf,argv[i+2]);
			send(client_socket,buf,sizeof(buf),0);
		}
		printf("The client sent write operation to AWS\n");
		recv(client_socket,buf,sizeof(buf),0);
		printf("The write operation has been completed successfully\n");
	}
	else
	{	
		double delay;
		for(int i=0;i<argc-2;i++)
		{	
			strcpy(buf,argv[i+2]);
			send(client_socket,buf,sizeof(buf),0);
		}
		printf("The client sent ID = <%s>, size = <%s>,and power = <%s> to AWS\n",argv[2],argv[3],argv[4]);
		recv(client_socket,&delay,sizeof(delay),0);
		printf("The delay for link <%s> is <%.2f>ms\n",argv[2],delay);
	}
	close(client_socket);	
	return 0;
		
}
