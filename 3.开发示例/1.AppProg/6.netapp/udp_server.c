/* udp_server.c */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT  8888
#define BUFFER_SIZE 1024

int main(void)
{
	struct sockaddr_in server_addr,client_addr;
	int sockfd;
	char buf[BUFFER_SIZE];
	int ret,len;
	
	/* 建立socket连接，IPv4协议，数据报套接字 */
	if((sockfd = socket(AF_INET,SOCK_DGRAM,0))==-1)
	{
		perror("socket");
		exit(1);
	}
	printf("Socket id = %d\n",sockfd);
	
	/* 初始化sockaddr_in结构体 */
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	bzero(&(server_addr.sin_zero),8);

	len = sizeof(server_addr);

	/* 绑定函数bind */
	if(bind(sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr))==-1)
	{
		perror("bind");
		exit(1);		
	}
	printf("Bind success!\n");

	while(1)
	{
		/* 等待客户端发送过来的数据，一旦有数据就接收进来 */
		if((ret = recvfrom(sockfd,buf,sizeof(buf),0,(struct sockaddr *)&server_addr,&len))==-1)
		{
			perror("recvfrom");
			exit(1);		
		}
		buf[len] = '0';
		printf("The message is %s\n",buf);

		if(strncmp(buf,"stop",4)==0)
		{
			printf("Stop to run!\n");
			break;
		}
		/* 关闭socket */
		close(sockfd);
		exit(0);
	}
	return 0;
}


