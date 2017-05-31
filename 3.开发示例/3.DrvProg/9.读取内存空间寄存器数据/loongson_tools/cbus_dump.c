#include<stdio.h>  
#include<unistd.h>  
#include<sys/mman.h>  
#include<sys/types.h>  
#include<sys/stat.h>  
#include<fcntl.h>  
#include "ls1c_regs.h"

int main()  
{  
	unsigned char * map_base;  
	FILE *f;  
	int n, fd;  

	fd = open("/dev/mem", O_RDWR|O_SYNC);  
	if (fd == -1)  {  
		return (-1);  
	}  
/* 把bfd01000开始0x1000字节，映射到map_base */
	map_base = mmap(NULL, 0x1000, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0x1fd01000);  

	if (map_base == 0)  {  
		printf("NULL pointer!\n");  
	}  

	unsigned long addr;  
	unsigned char content;  
	int i = 0,i1 = 0;
printf("[gpio_number]:    0   8  16  24  32  40  48  56  64  72  80  88  96 104 112 120"); 
	for(i1 = 0 ; i1 < 5 ; i1 ++) { //每个gpio5个功能的定义分别是bfd0110c0,bfd0110d0,bfd0110e0,bfd0110f0,bfd011200开始的16个字节，
		printf("\nfun%d[1fd01%03x]:",i1 + 1,LS1X_CBUS_FIRST0 + i1 * 0x10);
		for(i = 0 ; i < 0x10 ; i ++) {
			printf("  %02X",*(volatile unsigned long *)(map_base+ LS1X_CBUS_FIRST0 + i1 * 0x10 + i)&0xFF);
		}
	}  
	close(fd);  
	printf("\n");
	munmap(map_base, 0x1000);  
	return (0);
}  
