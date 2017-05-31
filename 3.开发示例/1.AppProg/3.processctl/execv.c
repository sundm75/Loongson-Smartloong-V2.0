#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>

int main()
{
	pid_t pid;
	char *arg[]={NULL};
	pid = fork();
	if(pid == -1)
		perror("fork:");
	else if(pid ==0)
	{
		if(execv("/home/A8_test/print_something",arg)<0)
			perror("execv:");
	}
}
