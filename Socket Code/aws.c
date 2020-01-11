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
	char *c="compute";
	struct sockaddr_in client_addr;
	struct sockaddr_in local1_addr;
	struct sockaddr_in local2_addr;
	struct sockaddr_in local3_addr;
	struct sockaddr_in local4_addr;
	struct sockaddr_in serverA_addr;
	struct sockaddr_in serverB_addr;
	struct sockaddr_in monitor_addr;
	int sin_size=sizeof(struct sockaddr_in);
	memset(&local1_addr,0,sizeof(local1_addr));
	local1_addr.sin_family = AF_INET;
	local1_addr.sin_port = htons(23037);
	local1_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(&local2_addr,0,sizeof(local2_addr));
	local2_addr.sin_family = AF_INET;
	local2_addr.sin_port = htons(23037);
	local2_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(&local3_addr,0,sizeof(local3_addr));
	local3_addr.sin_family = AF_INET;
	local3_addr.sin_port = htons(24037);
	local3_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(&local4_addr,0,sizeof(local4_addr));
	local4_addr.sin_family = AF_INET;
	local4_addr.sin_port = htons(25037);
	local4_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverA_addr.sin_family = AF_INET;
	serverA_addr.sin_port = htons(21037);
	serverA_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverB_addr.sin_family = AF_INET;
	serverB_addr.sin_port = htons(22037);
	serverB_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	int aws1_socket = socket(AF_INET, SOCK_DGRAM,0);
	int aws2_socket = socket(AF_INET, SOCK_DGRAM,0);
	int aws3_socket = socket(AF_INET, SOCK_STREAM,0);
	int aws4_socket = socket(AF_INET, SOCK_STREAM,0);
	int bind3_socket = bind(aws3_socket,(struct sockaddr*)&local3_addr,sizeof(struct sockaddr));
	int bind4_socket = bind(aws4_socket,(struct sockaddr*)&local4_addr,sizeof(struct sockaddr));
	int listen3_socket = listen(aws3_socket, 5);
	int listen4_socket = listen(aws4_socket, 5);
	int client_socket = accept(aws3_socket, (struct sockaddr*)&client_addr, &sin_size);
	int monitor_socket = accept(aws4_socket, (struct sockaddr*)&monitor_addr, &sin_size);
	if (aws1_socket == -1)
	{
		perror("socket");
		return -1;
	}
	if (aws2_socket == -1)
	{
		perror("socket");
		return -1;
	}
	if (aws3_socket == -1)
	{
		perror("socket");
		return -1;
	}
	if (aws4_socket == -1)
	{
		perror("socket");
		return -1;
	}
	printf("the AWS is up and running\n");
	if (bind3_socket == -1)
	{
		perror("bind");
		return -1;
	}
	if (listen3_socket == -1)
	{
		perror("listen");
		return -1;
	}
	if (bind4_socket == -1)
	{
		perror("bind");
		return -1;
	}
	if (listen4_socket == -1)
	{
		perror("listen");
		return -1;
	}
	if (client_socket <0)
	{
		perror("accept");
		return -1;
	}
	if (monitor_socket <0)
	{
		perror("accept");
		return -1;
	}
	while(1)
	{
		int num=0;
		recv(client_socket,&num,sizeof(num),0);
		recv(client_socket,buf,sizeof(buf),0);
		send(monitor_socket,&num,sizeof(num),0);
		send(monitor_socket,buf,sizeof(buf),0);
		sendto(aws1_socket,&num,sizeof(num),0,(struct sockaddr*)&serverA_addr,sizeof(struct sockaddr));
		sendto(aws1_socket,buf,sizeof(buf),0,(struct sockaddr*)&serverA_addr,sizeof(struct sockaddr));
		if (strcmp(buf,w)==0)
		{
			printf("The AWS received operation <write> from the client using TCP over port <24037>\n");
			for (int i=0;i<num-2;i++)
			{
				recv(client_socket,buf,sizeof(buf),0);
				send(monitor_socket,buf,sizeof(buf),0);
				sendto(aws1_socket,buf,sizeof(buf),0,(struct sockaddr*)&serverA_addr,sizeof(struct sockaddr));
			}
			printf("The AWS sent operation <write> and arguments to the monitor using TCP over port <25037>\n");
			printf("The AWS sent operation <write> to Backend-Server A using UDP over port <23037>\n");
			recvfrom(aws1_socket,buf,sizeof(buf),0,(struct sockaddr*)&serverA_addr,&sin_size);
			printf("The AWS received response from Backend-Server A from writing using UDP over port <23037>\n");
			send(monitor_socket,buf,sizeof(buf),0);
			printf("The AWS sent write response to the monitor using TCP over port <25037>\n");
			send(client_socket,buf,sizeof(buf),0);
			printf("The AWS sent result to client for operation <write> using TCP over port <24037>\n");
			close(client_socket);
			int listen3_socket = listen(aws3_socket, 5);
			int client_socket = accept(aws3_socket, (struct sockaddr*)&client_addr, &sin_size);
		}
		else
		{	
			printf("The AWS received operation <compute> from the client using TCP over port <24037>\n");
			char k[1000]={0};
			char s[1000]={0};
			char p[1000]={0};
			double tt;
			double tp;
			double delay;		
			recv(client_socket,buf,sizeof(buf),0);
			strcpy(k,buf);
			send(monitor_socket,buf,sizeof(buf),0);	
			sendto(aws1_socket,buf,sizeof(buf),0,(struct sockaddr*)&serverA_addr,sizeof(struct sockaddr));
			recv(client_socket,buf,sizeof(buf),0);
			strcpy(s,buf);
			send(monitor_socket,buf,sizeof(buf),0);	
			sendto(aws1_socket,buf,sizeof(buf),0,(struct sockaddr*)&serverA_addr,sizeof(struct sockaddr));
			recv(client_socket,buf,sizeof(buf),0);
			strcpy(p,buf);
			send(monitor_socket,buf,sizeof(buf),0);	
			sendto(aws1_socket,buf,sizeof(buf),0,(struct sockaddr*)&serverA_addr,sizeof(struct sockaddr));
			printf("The AWS sent operation <compute> and arguments to the monitor using TCP over port <25037>\n");
			printf("The AWS sent operation <compute> to Backend-Server A using UDP over port <23037>\n");
			int m=0;
			recvfrom(aws1_socket,&m,sizeof(m),0,(struct sockaddr*)&serverA_addr,&sin_size);
			send(monitor_socket,&m,sizeof(m),0);
			if (m==0)
			{
				printf("Link ID not found\n");	
			}
			else
			{
				recvfrom(aws1_socket,buf,sizeof(buf),0,(struct sockaddr*)&serverA_addr,&sin_size);
				printf("The AWS received link information from Backend-ServerA using UDP over port <23037>\n");
				sendto(aws2_socket,buf,sizeof(buf),0,(struct sockaddr*)&serverB_addr,sizeof(struct sockaddr));
				sendto(aws2_socket,k,sizeof(k),0,(struct sockaddr*)&serverB_addr,sizeof(struct sockaddr));
				sendto(aws2_socket,s,sizeof(s),0,(struct sockaddr*)&serverB_addr,sizeof(struct sockaddr));
				sendto(aws2_socket,p,sizeof(p),0,(struct sockaddr*)&serverB_addr,sizeof(struct sockaddr));
				printf("The AWS sent link ID = <%s>, size = <%s>, power= <%s>, and link information to Backend-Server B using UDP over port <23037>\n",k,s,p);
				recvfrom(aws2_socket,&tt,sizeof(tt),0,(struct sockaddr*)&serverB_addr,&sin_size);
				recvfrom(aws2_socket,&tp,sizeof(tp),0,(struct sockaddr*)&serverB_addr,&sin_size);
				recvfrom(aws2_socket,&delay,sizeof(delay),0,(struct sockaddr*)&serverB_addr,&sin_size);
				printf("The AWS received outputs from Backend-Server B using UDP over port <23037>\n");
				send(client_socket,&delay,sizeof(delay),0);
				send(monitor_socket,&tt,sizeof(tt),0);	
				send(monitor_socket,&tp,sizeof(tp),0);	
				send(monitor_socket,&delay,sizeof(delay),0);
				printf("The AWS sent result to client for operation <compute> using TCP over port <24037>\n");
				printf("The AWS sent compute results to the monitor using TCP over port <25037>\n");		
			}
			close(client_socket);
			int listen3_socket = listen(aws3_socket, 5);
			int client_socket = accept(aws3_socket, (struct sockaddr*)&client_addr, &sin_size);
		}	
	}
	return 0;
}
