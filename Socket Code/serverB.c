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
#include <math.h> 

#define MAXDATASIZE 1000 // max number of bytes we can get at once 

int main(int argc, char *argv[])
{
	char buf[MAXDATASIZE];
	struct sockaddr_in serverB_addr;
	struct sockaddr_in aws_addr;
	int sin_size=sizeof(struct sockaddr_in);
	memset(&serverB_addr,0,sizeof(serverB_addr));
	serverB_addr.sin_family = AF_INET;
	serverB_addr.sin_port = htons(22037);
	serverB_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	int serverB_socket = socket(AF_INET, SOCK_DGRAM,0);
	int bind_socket = bind(serverB_socket,(struct sockaddr*)&serverB_addr,sizeof(struct sockaddr));
	if (serverB_socket == -1)
	{
		perror("socket");
		return -1;
	}
	printf("The Server B is up and running on port <22037>\n");
	if (bind_socket == -1)
	{
		perror("bind");
		return -1;
	}
	int num;
	while(1)
	{	
		char b[1000]={0};
		char le[1000]={0};
		char v[1000]={0};
		char n[1000]={0};
		char l1[1000]={0};
		char l2[1000]={0};
		char s[1000]={0};
		char p[1000]={0};
		double bw;
		double leng;
		double vel;
		double np;
		double fs;
		double sp;
		double tt;
		double tp;
		double delay;
		double c;
		double m;
		char *buff;
		recvfrom(serverB_socket,buf,sizeof(buf),0,(struct sockaddr*)&aws_addr,&sin_size);
		recvfrom(serverB_socket,l1,sizeof(l1),0,(struct sockaddr*)&aws_addr,&sin_size);
		recvfrom(serverB_socket,s,sizeof(s),0,(struct sockaddr*)&aws_addr,&sin_size);
		recvfrom(serverB_socket,p,sizeof(p),0,(struct sockaddr*)&aws_addr,&sin_size);
		printf("The Server B received link information: link <%s>, file size <%s>, and signal power <%s>\n",l1,s,p);
		buff=strtok(buf,"\t");
		strcpy(l2,buff);
		buff=strtok(NULL,"\t");
		strcpy(b,buff);
		buff=strtok(NULL,"\t");
		strcpy(le,buff);
		buff=strtok(NULL,"\t");
		strcpy(v,buff);
		buff=strtok(NULL,"\t");
		strcpy(n,buff);
		bw=strtod(b,NULL);
		bw=bw*1000000;
		leng=strtod(le,NULL);
		leng=leng*1000;
		vel=strtod(v,NULL);
		vel=vel*1000;
		np=strtod(n,NULL);
		fs=strtod(s,NULL);
		fs=fs*1000;
		sp=strtod(p,NULL);
		m=1+sp/np;
		c=bw*(log(m)/log(2));
		tt=fs/c;
		tp=leng/vel*1000;
		delay=((int)((tt+tp)*100+0.5))/100.0;
		tt=((int)((fs/c)*100+0.5))/100.0;
		tp=((int)((leng/vel*1000)*100+0.5))/100.0;
		printf("The Server B finished the calculation for link <%s>\n",l1);
		sendto(serverB_socket,&tt,sizeof(tt),0,(struct sockaddr*)&aws_addr,sizeof(struct sockaddr));
		sendto(serverB_socket,&tp,sizeof(tp),0,(struct sockaddr*)&aws_addr,sizeof(struct sockaddr));
		sendto(serverB_socket,&delay,sizeof(delay),0,(struct sockaddr*)&aws_addr,sizeof(struct sockaddr));
		printf("The Server B finished sending the output to AWS\n");
			
	}
	return 0;
}
