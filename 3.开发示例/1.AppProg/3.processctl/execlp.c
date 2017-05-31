#include<unistd.h>
#include<sys/types.h>
#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
int main()
{
	if(fork()==0)
	{
		if(execlp("ps","ps","-ef",NULL)<0)
		{
			perror("execlp:");
			exit(1);
		}
	}
	return 0;
}
