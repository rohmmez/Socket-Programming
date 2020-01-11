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
	char *a[4];
	char *w="write";
	char *b[2];
	int num;
	FILE *fp;
	FILE *fpr;
	int j=1;
	struct sockaddr_in serverA_addr;
	struct sockaddr_in aws_addr;
	int sin_size=sizeof(struct sockaddr_in);
	memset(&serverA_addr,0,sizeof(serverA_addr));
	serverA_addr.sin_family = AF_INET;
	serverA_addr.sin_port = htons(21037);
	serverA_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	int serverA_socket = socket(AF_INET, SOCK_DGRAM,0);
	int bind_socket = bind(serverA_socket,(struct sockaddr*)&serverA_addr,sizeof(struct sockaddr));
	if (serverA_socket == -1)
	{
		perror("socket");
		return -1;
	}
	printf("the ServerA is up and running using UDP on port <21037>\n");
	if (bind_socket == -1)
	{
		perror("bind");
		return -1;
	}
	while(1)
	{
		recvfrom(serverA_socket,&num,sizeof(num),0,(struct sockaddr*)&aws_addr,&sin_size);
		recvfrom(serverA_socket,buf,sizeof(buf),0,(struct sockaddr*)&aws_addr,&sin_size);
		if (strcmp(buf,w)==0)
		{	
			fp=fopen("database.txt","r");
			int line=0;
			int c;
			if (fp)
			{
				while ((c=fgetc(fp))!= EOF)
				{	
					if (c=='\n')
					{
						line++;
					}
				}
				fclose(fp);
			}
			fp=fopen("database.txt","a+");
			if (fp==NULL)
			{
				printf("file can not open");
				exit(0);
			}
			fprintf(fp,"%d\t",line+1);
			for (int i=0;i<num-2;i++)
			{
				recvfrom(serverA_socket,buf,sizeof(buf),0,(struct sockaddr*)&aws_addr,&sin_size);
				fprintf(fp,"%s\t", buf);
			}
			fprintf(fp,"\n");
			fclose(fp);
			printf("The Server A received input for writing\n");
			int ack=0;
			ack=line+1;
			sendto(serverA_socket,&ack,sizeof(ack),0,(struct sockaddr*)&aws_addr,sizeof(struct sockaddr));
			printf("The Server A wrote link <%d> to database\n",ack);
		}		
		else
		{	
			recvfrom(serverA_socket,buf,sizeof(buf),0,(struct sockaddr*)&aws_addr,&sin_size);
			printf("The Server A received input <%s> for computing\n",buf);
			int whichlin=strtod(buf,NULL);
			recvfrom(serverA_socket,buf,sizeof(buf),0,(struct sockaddr*)&aws_addr,&sin_size);
			recvfrom(serverA_socket,buf,sizeof(buf),0,(struct sockaddr*)&aws_addr,&sin_size);
			fpr=fopen("database.txt","r");
			int line=0;
			int c;
			if (fpr)
			{
				while ((c=fgetc(fpr))!= EOF)
				{	
					if (c=='\n')
					{
						line++;
					}
				}
			}
			int m;
			if (whichlin>line)
			{
				m=0;
				sendto(serverA_socket,&m,sizeof(m),0,(struct sockaddr*)&aws_addr,sizeof(struct sockaddr));
				printf("Link ID not found\n");
			}
			else
			{	
				fclose(fpr);
				fpr=fopen("database.txt","r");
				m=1;
				sendto(serverA_socket,&m,sizeof(m),0,(struct sockaddr*)&aws_addr,sizeof(struct sockaddr));
				int i=1;
				char strline[1000]={0};
				while(1)
				{
					if (i==whichlin)
					{
						fgets(strline,1000,fpr);
						break;
					}
					fgets(strline,1000,fpr);
					i=i+1;
				}
				sendto(serverA_socket,strline,sizeof(strline),0,(struct sockaddr*)&aws_addr,sizeof(struct sockaddr));
				printf("The Server A finished sending the search result to AWS\n");
			}
			fclose(fpr);
		}	
	}
	return 0;
}
