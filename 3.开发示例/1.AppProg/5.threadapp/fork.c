#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<error.h>
#include<stdlib.h>
int main()
{
	pid_t result;
	result = fork();
	if(result==-1)
	{
		perror("fork:");
		exit(1);
	}
	else if(result==0)
	{
		printf("Current value is %d. In child process, child PID = %d\n",result,getpid());
	}
	else 
	{	
		printf("Current value is %d. In father process, father PID = %d\n",result,getppid());
	}
	return 0;
}
