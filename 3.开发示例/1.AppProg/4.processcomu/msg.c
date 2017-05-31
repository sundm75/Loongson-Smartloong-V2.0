#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define BUFSIZE 512
struct message{
long msg_type;
char msg_text[BUFSIZE];
};
int main()
{
	int qid;
	key_t key;
	int len;
	struct message msg;
	if((key=ftok(".",'a'))==-1)
	{
		perror("ftok:");
		exit(1);
	}	
	if((qid=msgget(key,IPC_CREAT|066))==-1)
	{
		perror("msgget:");
		exit(1);
	}
	printf("Open queue %d\n",qid);
	puts("Please enter the message to the queue:");
	if((fgets((&msg)->msg_text,BUFSIZE,stdin))==NULL)
	{
		puts("no message.");
		exit(1);
	}
	msg.msg_type=getpid();
	len=strlen(msg.msg_text);
	if((msgsnd(qid,&msg,len,0))<0)
	{
		perror("msgsnd:");
		exit(1);
	}
	if(msgrcv(qid,&msg,BUFSIZE,0,0)<0)
	{
		perror("msgrcv");
		exit(1);
	}
	printf("Message is %s\n",(&msg)->msg_text);
	if((msgctl(qid,IPC_RMID,NULL))<0)
	{
		perror("msgctl:");
		exit(1);
	}
	exit(0);
}
