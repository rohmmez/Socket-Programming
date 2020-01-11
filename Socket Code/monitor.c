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

int main()
{
	char buf[MAXDATASIZE]={0};
	char *w="write";
	char *c="compute";
	char buff[MAXDATASIZE]={0};
	struct sockaddr_in remote_addr;
	memset(&remote_addr,0,sizeof(remote_addr));
	remote_addr.sin_family=AF_INET;
	remote_addr.sin_port=htons(25037);
	remote_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	int monitor_socket=socket(AF_INET, SOCK_STREAM,0);
	int connect_socket=connect(monitor_socket,(struct sockaddr*)&remote_addr,sizeof(struct sockaddr));
	if (monitor_socket < 0)
	{
		perror("socket");
		return -1;
	}
	printf("The monitor is up and running\n");
	if (connect_socket < 0)
	{
		perror("connect");
		return -1;
	}
	int num;
	while(1)
	{
		recv(monitor_socket,&num,sizeof(num),0);
		recv(monitor_socket,buf,sizeof(buf),0);
		if (strcmp(buf,w)==0)
		{	char a[1000]={0};
			char b[1000]={0};
			char c[1000]={0};
			char d[1000]={0};
			recv(monitor_socket,buf,sizeof(buf),0);
			strcpy(a,buf);
			recv(monitor_socket,buf,sizeof(buf),0);
			strcpy(b,buf);
			recv(monitor_socket,buf,sizeof(buf),0);
			strcpy(c,buf);
			recv(monitor_socket,buf,sizeof(buf),0);	
			strcpy(d,buf);
			printf("The monitor received BW = <%s>, L = <%s>, V = <%s> and P = <%s> from the AWS\n", a,b,c,d);
			recv(monitor_socket,buf,sizeof(buf),0);
			printf("The write operation has been completed successfully\n");
		}
		else if(strcmp(buf,c)==0)
		{	
			
			char a[1000]={0};
			char b[1000]={0};
			char c[1000]={0};
			int m;
			double tt;
			double tp;
			double delay;
			recv(monitor_socket,buf,sizeof(buf),0);
			strcpy(a,buf);
			recv(monitor_socket,buf,sizeof(buf),0);
			strcpy(b,buf);
			recv(monitor_socket,buf,sizeof(buf),0);
			strcpy(c,buf);
			printf("The monitor received input = <%s>, size = <%s> and power = <%s> from the AWS\n", a,b,c);
			recv(monitor_socket,&m,sizeof(m),0);
			if (m==0)
			{
				printf("Link ID not found\n");
			}
			else
			{
				recv(monitor_socket,&tt,sizeof(tt),0);
				recv(monitor_socket,&tp,sizeof(tp),0);
				recv(monitor_socket,&delay,sizeof(delay),0);
				printf("The result for link <%s>: Tt = <%.2f>ms Tp = <%.2f>ms Delay = <%.2f>ms\n",a,tt,tp,delay);
			}
		}
	}
	return 0;	
}
