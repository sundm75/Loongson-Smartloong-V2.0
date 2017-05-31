#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>


int main()
{
	if(fork()==0)
	{
		if(execl("/bin/ls","ls","-l",NULL)<0)
			perror("execl:");
	}
	return 0;
}
