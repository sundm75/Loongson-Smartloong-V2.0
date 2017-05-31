/* file_stdio.c */
#include<stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc ,char **argv)
{
	FILE *fp;
	char *buf = "hello,I'm Webee,This is  file standard io test!";
	char buf_r[20];
	int len = strlen(buf);	

	/* 调用fopen函数，如果当前目录下没有hello.c则创建，以可读可写方式打开文件*/
	if((fp = fopen("./hello.c","w+")) != NULL)
	{
		printf("fopen success\n");
	}

	/* 调用fwrite函数，将buf中的内容写入hello.c */
 	if((fwrite(buf,sizeof(char),len,fp) > 0))
 	{
 		printf("fwrite: %s\n",buf);
 	}

	/* 调用fseek函数将文件指针移到文件起始位置，并读出文件中的15个字节 */
	fseek(fp,0,SEEK_SET);
	if((fread(buf_r,sizeof(char),15,fp)) <0)
	{
		printf("fread fail\n");
		exit(1);
	}
	else
	{
		buf_r[15] = '\0';
		printf("fread from hello.c and the content is %s\n",buf_r);
	}

	/* 最后调用close函数关闭打开的文件 */
	if(fclose(fp) < 0)
	{
		printf("fclose fail\n");
		exit(1);
	}
	else
		printf("fclose hello.c\n");

	return 0;	
}

