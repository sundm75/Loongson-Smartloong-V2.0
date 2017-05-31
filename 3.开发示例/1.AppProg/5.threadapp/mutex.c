/* mutex.c */
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

/* 创建快速互斥锁 */
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
/* 待观察计数变量 */
int lock_var;	
timer_t end_time;

/* 线程一 */
void thread1(void)
{
	int i = 0;
	while(time(NULL) < end_time)
	{
		if(pthread_mutex_lock(&mutex)!=0)
		{
			perror("pthread_mutex_lock");
		}
		else
		{
			printf("pthread1:pthread1 lock the variable\n");
			for(i =0;i < 2;i++)
			{
				sleep(1);
				lock_var++;
			}
		}
		/* 互斥锁解锁 */
		if(pthread_mutex_unlock(&mutex)!=0)
		{
			perror("pthread_mutex_unlock");
		}
		else
		{
			printf("pthread1:pthread1 unlock the variable\n");
			sleep(1);
		}	
	}
}
/* 线程二 */
void thread2(void)
{
	int ret;
	while(time(NULL) < end_time)
	{
		/* 判断是否已经被上锁 */
		ret = pthread_mutex_trylock(&mutex);
		if(ret==EBUSY)	
		{
			/* 忙 */
			printf("pthread2:the variable is lock by pthread1\n");
		}	
		else
		{
			/* 不忙 */
			if(ret!=0)
			{
				perror("pthread_mutex_trylock");
				exit(1);
			}
			else
			{
				printf("pthread2:pthread2 got lock. The variable is %d\n",lock_var);
			}
			/* 互斥锁解锁 */
			if(pthread_mutex_unlock(&mutex)!=0)
			{
				perror("pthread_mutex_unlock");
			}
			else
			{
				printf("pthread2:pthread2 unlock the variable\n");
				
			}
		}
	sleep(3);
	}
}
int main()
{
	pthread_t id1,id2;
	int ret;
	end_time = time(NULL) + 10;
	
	/* 快速互斥锁的初始化 */
	pthread_mutex_init(&mutex,NULL);
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

