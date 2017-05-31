#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main()
{
	int pipe_fd[2];
	pid_t pid;
	char buf_r[100];
	char *p_wbuf;
	int r_num;
	memset(buf_r,0,sizeof(buf_r));
	
	if(pipe(pipe_fd)<0)
	{
		printf("Creat pipe error\n");
		return 1;
	}
	if((pid=fork())==0)
	{
		close(pipe_fd[1]);
		sleep(2);
		if((r_num=read(pipe_fd[0],buf_r,100))>0)
		{
			printf("%d numbers read from pipe is %s\n",r_num,buf_r);	
		}
		close(pipe_fd[0]);
		exit(0);
	}
	else
	{
		close(pipe_fd[0]);
		if(write(pipe_fd[1],"Hello",5)!=-1)
		{
			printf("Pipe write1 success.\n");
		}
		if(write(pipe_fd[1]," pipe",5)!=-1)
		{
			printf("Pipe write2 success.\n");
		}
		close(pipe_fd[1]);
		sleep(3);
		exit(0);
	}
	return 0;
}
