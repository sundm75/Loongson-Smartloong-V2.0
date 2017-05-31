#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
int main(void)
{
	printf("The PID of the process is:%d\n",getpid());
	printf("The PPID of the process is:%d\n",getppid());
	return 0;
}
