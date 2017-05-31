/* thread.c */
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/* 线程一 */
void thread1(void)
{
	int i = 0;
	while(i++ < 3)
	{
		printf("This is pthread1.\n");
		if(i == 2)
			pthread_exit(0);
		sleep(2);
	}
}
/* 线程二 */
void thread2(void)
{
	int i = 0;
	while(i++ < 3)
	{
		printf("This is pthread2.\n");
	}
	pthread_exit(0);
}
int main()
{
	pthread_t id1,id2;
	int ret;
	/* 分别创建线程1、2 */
	ret = pthread_create(&id1,NULL,(void *)thread1,NULL);
	if(ret != 0)
	{
		printf("Create pthread1 error\n");
		exit(1);
	}
	ret = pthread_create(&id2,NULL,(void *)thread2,NULL);
	if(ret != 0)
	{
		printf("Create pthread2 error\n");
		exit(1);
	}
	/* 等待线程结束 */
	pthread_join(id1,NULL);
	pthread_join(id2,NULL);
	return 0;
}

