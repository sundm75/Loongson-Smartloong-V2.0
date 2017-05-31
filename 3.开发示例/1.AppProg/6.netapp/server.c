/* server.c */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT  4321
#define BUFFER_SIZE 1024
#define MAX_QUE_CONN_NM 5

int main(void)
{
	struct sockaddr_in server_addr,client_addr;
	int sin_size,recvbytes;
	int ser_fd,cli_fd;
	char buf[BUFFER_SIZE];
	
	/* 建立socket连接，IPv4协议，字节流套接字 */
	if((ser_fd = socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		perror("socket");
		exit(1);
	}
	printf("Socket id = %d\n",ser_fd);
	
	/* 初始化sockaddr_in结构体 */
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(server_addr.sin_zero),8);

	/* 绑定函数bind */
	if(bind(ser_fd,(struct sockaddr *)&server_addr,sizeof(struct sockaddr))==-1)
	{
		perror("bind");
		exit(1);		
	}
	printf("Bind success!\n");

	/* 调用listen函数，进行监听 */
	if(listen(ser_fd,MAX_QUE_CONN_NM)==-1)
	{
		perror("listen");
		exit(1);		
	}
	printf("Listening......\n");

	/* 调用accept函数，等待客户端的连接 */
	if((cli_fd = accept(ser_fd,(struct sockaddr *)&client_addr,&sin_size))==-1)
	{
		perror("accept");
		exit(1);		
	}
	printf("Have client ready for connecting\n");
	
	/* 调用recv函数接收客户端的请求 */
	memset(buf,0,sizeof(buf));
	if((recvbytes = recv(cli_fd,buf,BUFFER_SIZE,0))==-1)
	{
		perror("recv");
		exit(1);		
	}
	/* 将收到的信息(客服端发来的信息)打印出来 */
	printf("Received a message:%s\n",buf);	

	/* 关闭socket */
	close(ser_fd);
	return 0;
}


