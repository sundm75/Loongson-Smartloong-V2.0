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
	uint16_t addr_offset,addr;
	uint8_t func,gpio,i,reg,bit;
	if(argc!=3) {
		printf("gpio_func 4 85 ;set gpio85 to function 4 \r\n");
		return (-2);
	}
	addr_base=0x1fd01000;
	addr_offset=0x1c0;
	func=atoi(argv[1]);
	gpio=atoi(argv[2]);
	reg=(uint8_t)gpio/8;
        bit=gpio%8;
	if(func>5) {
		printf("func <=5 \r\n");
		return(-3);
	}
	if(gpio>=128) {
		printf("gpio <128 \r\n");
		return(-4);
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
if(func>0) {
for(i=1;i<func;i++) //disable 前面的func功能
	map_base[addr_offset+(i-1)*0x10+reg] &= ~(1<<bit);
addr=addr_offset+(func-1)*0x10+reg;
printf("[%08x]:%02x",addr_base+addr,map_base[addr]);
map_base[addr] |= (1<<bit);
printf("=>%02x,bit%d set,%02x",map_base[addr],bit,1<<bit);


}else {
for(i=1;i<6;i++) //disable 全部功能
	map_base[addr_offset+i*0x10+reg] &= ~(1<<bit);

}

	close(fd);  
	munmap(map_base, 0x1000);  
	return (0);
}  
