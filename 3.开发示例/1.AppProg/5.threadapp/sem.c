/* sem.c */
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <semaphore.h>

/* 定义一个信号量 */
sem_t sem;
/* 待观察计数变量 */
int lock_var;	
timer_t end_time;

/* 线程一 */
void thread1(void)
{
	int i = 0;
	while(time(NULL) < end_time)
	{
		/* P操作，使得sem减一 */
		sem_wait(&sem);
		for(i =0;i < 2;i++)
		{
			sleep(1);
			lock_var++;
			printf("lock_var = %d\n",lock_var);
		}
		printf("pthread1:lock_var = %d\n",lock_var);
		/* V操作，使得sem加一 */
		sem_post(&sem);
		sleep(1);	
	}
}
/* 线程二 */
void thread2(void)
{
	int ret;
	while(time(NULL) < end_time)
	{
		/* P操作，使得sem减一 */
		sem_wait(&sem);
		printf("pthread2:pthread2 got lock;lock_var = %d\n",lock_var);
		/* V操作，使得sem加一 */
		sem_post(&sem);
		sleep(3);	
	}
}
int main()
{
	pthread_t id1,id2;
	int ret;
	end_time = time(NULL) + 10;
	
	/* 初始化信号量为1 */
	ret = sem_init(&sem,0,1);
	if(ret != 0)
	{
		printf("sem_init error\n");
		exit(1);
	}
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

