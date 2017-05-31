#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>


int main()
{
	pid_t c_pid,get_pid;
	c_pid = fork();
	if(c_pid < 0)
		perror("fork");
	else if(c_pid == 0)
	{
		printf("sleep 3s in child\n");
		sleep(3);
		exit(0);
	}
	else
	{
		do
		{
			get_pid = waitpid(c_pid,NULL,WNOHANG);
			if(get_pid == 0)
			{
				printf("Child process has not exited\n");
				sleep(1);
			}
			
		}while(!get_pid);
		if(get_pid == c_pid)
			printf("Child process has exited\n");
	}
}
