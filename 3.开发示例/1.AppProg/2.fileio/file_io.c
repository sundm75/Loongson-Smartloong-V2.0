#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include <unistd.h>
#include<stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc ,char **argv)
{
	int fd,size;
	char *buf = "hello,I'm Webee,This is  file io test!";
	char buf_r[20];
	int len = strlen(buf);

	/* 首先调用open函数，如果当前目录下没有hello.c则创建
	 * 以读写方式打开文件，并指定权限为可读可写
	 */
	if((fd = open("./hello.c",O_CREAT | O_TRUNC | O_RDWR,0666))<0)
	{
		/* 错误处理 */
		printf("open fail\n");
		exit(1);
	}
	else
		printf("open file:hello.c fd = %d\n",fd);

	/* 调用write函数，将buf中的内容写入hello.c */
	if((size = write(fd,buf,len) ) < 0)
	{
		printf("write fail\n");
		exit(1);
	}
	else
		printf("write: %s\n",buf);

	/* 调用lseek函数将文件指针移到文件起始位置，并读出文件中的15个字节 */
	lseek(fd,0,SEEK_SET);
	if((size = read(fd,buf_r,15)) <0)
	{
		printf("read fail\n");
		exit(1);
	}
	else
	{
		buf_r[15] = '\0';
		printf("read from hello.c and the content is %s\n",buf_r);
	}

	/* 最后调用close函数关闭打开的文件 */
	if(close(fd) < 0)
	{
		printf("close fail\n");
		exit(1);
	}
	else
		printf("close hello.c\n");

	return 0;	
}

