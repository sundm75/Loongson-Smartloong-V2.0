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
	int fd;
	char buf_w[100];
	int nwrite;
	fd=open(FIFO,O_WRONLY|O_NONBLOCK,0);
	if(fd==-1)
	{
		if(errno==ENXIO)
		printf("open error:no reading process");
	}
	if(argc==1)
	{
		printf("Plese sent something\n");
	}
	strcpy(buf_w,argv[1]);
	if((nwrite=write(fd,buf_w,100))==-1)
	{
		if(errno==EAGAIN)
		printf("The FIFO has not been read yet.Please try latter.\n");
	}
	else
	{
		printf("write %s to the FIFO\n",buf_w);
	}

}
