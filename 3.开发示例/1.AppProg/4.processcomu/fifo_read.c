#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define FIFO "/tmp/myfifo"

int main(int argc,char **argv)
{
	char buf_r[100];
	int fd,nread;
	if((mkfifo(FIFO,O_CREAT|O_EXCL)<0)&(errno!=EEXIST))
	{
		printf("Can not creat fifo\n");
	}
	else
	{
		printf("Creat fifo success\nPreparing for reading bytes..\n");
	}
	memset(buf_r,0,sizeof(buf_r));

	fd=open(FIFO,O_RDONLY|O_NONBLOCK,0);
	if(fd==-1)
	{
		perror("open:");
		exit(1);
	}
	while(1)
	{
		memset(buf_r,0,sizeof(buf_r));
		if((nread=read(fd,buf_r,100))==-1)
		{
			if(errno==EAGAIN)
			{
				printf("No data yet\n");
			}
		}
		else
		{
			printf("Read %s from FIFO\n",buf_r);
			sleep(1);
		}
	}
	pause();
	ulink(FIFO);
}
