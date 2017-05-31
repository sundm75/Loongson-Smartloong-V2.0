#include<sys/ipc.h>
#include<sys/shm.h>
#include<stdio.h>
#include<stdlib.h>

#define BUFSIZE 2048
int main()
{
	int shmid;
	char *shmadd;
	if((shmid=shmget(IPC_PRIVATE,BUFSIZE,0666))<0)
	{
		perror("shmget:");
		exit(1);
	}
	else
	{
		printf("Create share memory success,the shmid is:%d\n",shmid);
	}
	system("ipcs -m");
	if((shmadd=shmat(shmid,0,0))<(char *)0)
	{
		peror("shmat:");
		exit(1);
	}
	else
	{
		printf("Mat memory success,the share address is matted to %x\n",shmadd);
	}
	system("ipcs -m");
	if((shmdt(shmadd))<0)
	{
		perror("shmdt:");
		exit(1);
	}
	else
	{
		printf("Delete memory success\n");
	}
	system("ipcs -m");
	exit(0);
}
