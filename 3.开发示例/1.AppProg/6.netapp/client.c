/* client.c */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT  4321
#define BUFFER_SIZE 1024

int main(int argc,char *argv[])
{
	struct sockaddr_in server_addr;
	int sockfd,sendbytes;
	char buf[BUFFER_SIZE];
	struct hostent *host;

	/* 指定输入参数为3个，否则出错 */
	if(argc != 3)
	{
		printf("Usage: ./clinet IP address Text\n");
		exit(1);
	}

	/* 地址解析函数 */
	if((host = gethostbyname(argv[1]))==NULL)
	{
		perror("gethostbyname");
		exit(1);
	}	
	memset(buf,0,sizeof(buf));
	sprintf(buf,"%s",argv[2]);

	/* 建立socket连接，IPv4协议，字节流套接字 */
	if((sockfd = socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		perror("socket");
		exit(1);
	}
	printf("Socket id = %d\n",sockfd);
	
	/* 初始化sockaddr_in结构体 */
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr = *((struct in_addr *)host->h_addr);
	bzero(&(server_addr.sin_zero),8);

	/* 调用connect函数主动发起对服务器的连接 */
	if((connect(sockfd,(struct sockaddr *)&server_addr,sizeof(struct sockaddr)))==-1)
	{
		perror("connect");
		exit(1);		
	}
	printf("Connect server success!\n");

	/* 发送消息给服务器端 */
	if((sendbytes = send(sockfd,buf,strlen(buf),0))==-1)
	{
		perror("send");
		exit(1);		
	}
	sleep(3);
	/* 关闭socket */
	close(sockfd);
	return 0;
}


