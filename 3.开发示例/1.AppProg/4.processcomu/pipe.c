#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>


int main()
{
	int pipe_fd[2];
	if(pipe(pipe_fd)<0)
	{
		printf("Pipe creat error\n");
		exit(1);
	}
	else
	{
		printf("Pipe creat success\n");
	}
	close(pipe_fd[0]);
	close(pipe_fd[1]);
}
