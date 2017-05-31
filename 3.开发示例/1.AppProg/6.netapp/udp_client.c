/* udp_client.c */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT  8888
#define BUFFER_SIZE 1024

int main(int argc,char *argv[])
{
	struct sockaddr_in server_addr;
	int sockfd;
	char buf[BUFFER_SIZE];
	int sendbytes;
	char hostbuf[64];

	/* 指定输入参数为3个，否则出错 */
	if(argc != 3)
	{
		printf("Usage: ./clinet Text\n");
		exit(1);
	}
	
	/* 地址解析 */
	memset(hostbuf,0,sizeof(hostbuf));
	sprintf(hostbuf,"%s",argv[1]);

	memset(buf,0,sizeof(buf));
	sprintf(buf,"%s",argv[2]);

	/* 初始化sockaddr_in结构体 */
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	//server_addr.sin_addr.s_addr = inet_addr("193.169.2.215");
	server_addr.sin_addr.s_addr = inet_addr(hostbuf);
	bzero(&(server_addr.sin_zero),8);
	
	/* 建立socket连接，IPv4协议，数据报套接字 */
	if((sockfd = socket(AF_INET,SOCK_DGRAM,0))==-1)
	{
		perror("socket");
		exit(1);
	}
	printf("Socket id = %d\n",sockfd);
	
	/* 发送消息给服务器端 */
	if((sendbytes = sendto(sockfd,buf,strlen(buf),0,(struct sockaddr *)&server_addr,sizeof(server_addr)))==-1)
	{
		perror("sendto");
		exit(1);		
	}
	/* 关闭socket */
	close(sockfd);
	return 0;
}


