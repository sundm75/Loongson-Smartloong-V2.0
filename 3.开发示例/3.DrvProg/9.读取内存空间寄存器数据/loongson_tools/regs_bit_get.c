#include<stdio.h>  
#include<stdlib.h>  
#include<stdint.h>  
#include<string.h>  
#include<unistd.h>  
#include<sys/mman.h>  
#include<sys/types.h>  
#include<sys/stat.h>  
#include<fcntl.h>  
#include "ls1c_regs.h"


uint32_t hextol(char * str) {
	uint8_t i=0,l;
	uint32_t ret=0;
	l=strlen(str);
	for(i=0;i<l;i++) str[i]=str[i]|0x20; //'A-Z' to 'a-z'
	for(i=2;i<l;i++){
		ret=(ret<<4);
		if(str[i]>'9') ret|=(str[i]-'a'+10);
		else ret|=(str[i]&0xf);
	}
	return ret;
}

int main(int argc, char *argv[])  
{  
	unsigned char * map_base;  
	FILE *f;  
	int n, fd;  
	uint32_t addr_base;
	uint16_t addr_offset;
	uint8_t dat;
	if(argc!=3) {
		printf("regs_bit_get 0x1fd011c0 0 \r\n");
		return (-2);
	}
	addr_base=hextol(argv[1]);
	addr_offset=addr_base&0xfff;
	addr_base=addr_base&0xfffff000;
	dat=atoi(argv[2]);
	if(dat>=32) {
		printf("bit < 32 \r\n");
		return(-3);
	}

	fd = open("/dev/mem", O_RDWR|O_SYNC);  
	if (fd == -1)  {  
		return (-1);  
	}  
	/* 把xxxxx000开始0x1000字节，映射到map_base */
	map_base = mmap(NULL, 0x1000, PROT_READ|PROT_WRITE, MAP_SHARED, fd, addr_base);  

	if (map_base == 0)  {  
		printf("NULL pointer!\n");  
	}
	dat = (*(volatile unsigned long *)(map_base+addr_offset) >> dat) & 0x1;  
	printf("%d",dat);
	close(fd);  
	munmap(map_base, 0x1000);  
	return (dat);
}  
