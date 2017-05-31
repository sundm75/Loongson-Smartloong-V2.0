#include<stdio.h>  
#include<stdlib.h>  
#include<stdint.h>  
#include<string.h>  
#include<unistd.h>  
#include<sys/mman.h>  
#include<sys/types.h>  
#include<sys/stat.h>  
#include<fcntl.h>   



#define GPIO_CFG0 	0xC0
#define GPIO_EN0 	0xD0
#define GPIO_IN0 	0xE0
#define GPIO_OUT0 	0xF0

#define CBUS_FIRST0 	0x1C0

unsigned char * map_base;  
int  fd;  
	
int gpio_open(void)
{
	fd = open("/dev/mem", O_RDWR|O_SYNC);  
	if (fd == -1)  {  
		return (-1);  
	}  

	/* °Ñ 0x1fd01000 ¿ªÊŒ0x1000×ÖœÚ£¬Ó³Éäµœmap_base */
	map_base = mmap(NULL, 0x1000, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0x1fd01000);  

	if (map_base == 0)  {  
		printf("NULL pointer!\n");  
	}
	return 0;
	
}

void gpio_close(void)
{
	close(fd);  
	munmap(map_base, 0x1000);  	
}

void gpio_cfg_init(int gpio_number, int function)
{
	uint16_t addr_offset;
	uint16_t bit_offset;
	int index;
	uint32_t data_temp;
	
	index = gpio_number / 32;
	bit_offset = gpio_number % 32;
	addr_offset = GPIO_CFG0 + index*4;	
	
	data_temp = *(volatile unsigned long *)(map_base+addr_offset);
	if(function == 0)
	{
		data_temp = data_temp & (~(1<<bit_offset));//clr
	}
	else
	{
		data_temp = data_temp | ((1<<bit_offset));//set
	}
 
	*(volatile unsigned long *)(map_base+addr_offset)=data_temp;
}
void gpio_en_init(int gpio_number, int function)
{
	uint16_t addr_offset;
	uint16_t bit_offset;
	int index;
	uint32_t data_temp;
	
	index = gpio_number / 32;
	bit_offset = gpio_number % 32;
	addr_offset = GPIO_EN0 + index*4;	
	
	data_temp = *(volatile unsigned long *)(map_base+addr_offset);
	if(function == 0)
	{
		data_temp = data_temp & (~(1<<bit_offset));//clr
	}
	else
	{
		data_temp = data_temp | ((1<<bit_offset));//set
	}
 
	*(volatile unsigned long *)(map_base+addr_offset)=data_temp;
}
void gpio_func_init(int gpio_number, int function)
{
	uint16_t addr_offset;
	uint16_t bit_offset;
	int index,i;
	uint32_t data_temp;
	
	index = gpio_number / 32;
	bit_offset = gpio_number % 32;
	addr_offset = CBUS_FIRST0 + index*4;	
	
	data_temp = *(volatile unsigned long *)(map_base+addr_offset);
	for(i=1;i<6;i++)
	{
		data_temp = *(volatile unsigned long *)(map_base+addr_offset+(i-1)*0x10);
		if(i == function)//set
		{
			data_temp = data_temp | ((1<<bit_offset));//set
		}
		else//clr
		{
			data_temp = data_temp & (~(1<<bit_offset));//clr
		}
		*(volatile unsigned long *)(map_base+addr_offset)=data_temp;
	} 
}

void gpio_set(int gpio_number)
{
	uint16_t addr_offset;
	uint16_t bit_offset;
	int index;
	uint32_t data_temp;
	
	index = gpio_number / 32;
	bit_offset = gpio_number % 32;
	addr_offset = GPIO_OUT0 + index*4;	
	
	data_temp = *(volatile unsigned long *)(map_base+addr_offset);

	{
		data_temp = data_temp | ((1<<bit_offset));//set
	}
 
	*(volatile unsigned long *)(map_base+addr_offset)=data_temp;	
}
uint32_t gpio_get(int gpio_number)
{
	uint16_t addr_offset;
	uint16_t bit_offset;
	int index;
	uint32_t data_temp;
	
	index = gpio_number / 32;
	bit_offset = gpio_number % 32;
	addr_offset = GPIO_IN0 + index*4;	
	
	data_temp = *(volatile unsigned long *)(map_base+addr_offset);

	return 	(data_temp >> bit_offset)&0x01;
}
void gpio_clr(int gpio_number)
{
	uint16_t addr_offset;
	uint16_t bit_offset;
	int index;
	uint32_t data_temp;
	
	index = gpio_number / 32;
	bit_offset = gpio_number % 32;
	addr_offset = GPIO_OUT0 + index*4;	
	
	data_temp = *(volatile unsigned long *)(map_base+addr_offset);
	{
		data_temp = data_temp & (~(1<<bit_offset));//clr
	}
 
	*(volatile unsigned long *)(map_base+addr_offset)=data_temp;	
}
uint32_t data_get(int gpio_start_number, int len)
{
	uint16_t addr_offset;
	uint16_t bit_offset;
	int index,i;
	uint32_t data_temp;
	
	index = gpio_start_number / 32;
	bit_offset = gpio_start_number % 32;
	addr_offset = GPIO_IN0 + index*4;	
	if((bit_offset+len)>32)
	{
		len = 32 - bit_offset;
	}	
	data_temp = *(volatile unsigned long *)(map_base+addr_offset);

	for( i=bit_offset+len;i<32;i++)
	{
		data_temp = data_temp & (~(1<<i));//clr
	}
	
	return data_temp>>bit_offset;
}
void data_write(int gpio_start_number, int len ,uint32_t data)
{
	uint16_t addr_offset;
	uint16_t bit_offset;
	int index,i;
	uint32_t data_temp;
	
	index = gpio_start_number / 32;
	bit_offset = gpio_start_number % 32;
	addr_offset = GPIO_OUT0 + index*4;	
	if((bit_offset+len)>32)
	{
		len = 32 - bit_offset;
	}
	data_temp = *(volatile unsigned long *)(map_base+addr_offset);
	{
		for( i=0;i<len;i++)
		{
			if(0x00 == (0x01&(data>>i))){		
				data_temp = data_temp & (~(1<<(bit_offset+i)));//clr
			}
			else{
				data_temp = data_temp | (1<<(bit_offset+i)) ;//set
			}
		}
	}
	*(volatile unsigned long *)(map_base+addr_offset)=data_temp;		
}
void gpio_cfg_data(int gpio_start_number, int len ,uint32_t data)
{
	uint16_t addr_offset;
	uint16_t bit_offset;
	int index,i;
	uint32_t data_temp;
	
	index = gpio_start_number / 32;
	bit_offset = gpio_start_number % 32;
	addr_offset = GPIO_CFG0 + index*4;	
	if((bit_offset+len)>32)
	{
		len = 32 - bit_offset;
	}
	data_temp = *(volatile unsigned long *)(map_base+addr_offset);
	{
		for( i=0;i<len;i++)
		{
			if(0x00 == data){		
				data_temp = data_temp & (~(1<<(bit_offset+i)));//clr
			}
			else if(0x01 == data){
				data_temp = data_temp | (1<<(bit_offset+i)) ;//set
			}
		}
	}
	*(volatile unsigned long *)(map_base+addr_offset)=data_temp;		
}
void gpio_en_data(int gpio_start_number, int len ,uint32_t data)
{
	uint16_t addr_offset;
	uint16_t bit_offset;
	int index,i;
	uint32_t data_temp;
	
	index = gpio_start_number / 32;
	bit_offset = gpio_start_number % 32;
	addr_offset = GPIO_EN0 + index*4;	
	if((bit_offset+len)>32)
	{
		len = 32 - bit_offset;
	}
	data_temp = *(volatile unsigned long *)(map_base+addr_offset);
	{
		for( i=0;i<len;i++)
		{
			if(0x00 == data){		
				data_temp = data_temp & (~(1<<(bit_offset+i)));//clr
			}
			else if(0x01 == data){
				data_temp = data_temp | (1<<(bit_offset+i)) ;//set
			}
		}
	}
	*(volatile unsigned long *)(map_base+addr_offset)=data_temp;		
}
/* Usage:  
 * ./gpio_fun cfg  [gpio_num] [0] // 用法  
 * ./gpio_fun en  [gpio_num] [0]  
 * ./gpio_fun out  [gpio_num] [0]  
 * ./gpio_fun in  [gpio_num] [0]
 * ./gpio_fun dataout  [gpio_num] [len] data//len bit
 * ./gpio_fun datain  [gpio_num] [len] data// len bit
 * ./gpio_fun cfgdata  [gpio_num] [len] 0/1// 8 bit
 * ./gpio_fun endata  [gpio_num] [len] 0/1// 8 bit
 * ./gpio_fun function  [gpio_num] functionnum 
 */  
int maintest(int argc, char *argv[])  
{  
	unsigned int buf[3];  
	if(argc<3) {
		printf("gpio_fun cfg/en/out/in 50 0\r\n");
		printf("gpio_fun dataout 58 6 85\r\n");
		printf("gpio_fun datain 58 6\r\n");
		printf("gpio_fun cfgdata 58 6 0/1 \r\n");
		printf("gpio_fun endata 58 6 0/1 \r\n");
		printf("gpio_fun function 4 85 ;set gpio85 to function 4 \r\n");
		return (-2);
	}
	
	if(gpio_open()<0)
		return (-1);		


	/* gpio_num */  
    buf[0] = strtoul(argv[2], NULL, 0); 

	/* function  */  
if(argc>=4)
{
	buf[1] = strtoul(argv[3], NULL, 0); 
}
	/* data  */  
if(argc==5)
{
	buf[2] = strtoul(argv[4], NULL, 0); 
}
    if (strcmp(argv[1], "cfg") == 0)  
    {  
		gpio_cfg_init(buf[0], buf[1]);
    }  
    else if (strcmp(argv[1], "en") == 0)  
    {  
		gpio_en_init(buf[0], buf[1]);
	}
    else if (strcmp(argv[1], "out") == 0)  
    {  
		if(1 == buf[1]){
			gpio_set(buf[0]);
		}
		else if(0 == buf[1]){
			gpio_clr(buf[0]);
		}
	}
    else if (strcmp(argv[1], "in") == 0)  
    {  
		printf("gpio%d = %d\r\n",buf[0], gpio_get(buf[0]) );
	}
    else if (strcmp(argv[1], "dataout") == 0)  
    {  
		if(argc>=5)
		{
			data_write(buf[0],buf[1],buf[2]);
		}
		else
		{
			printf("gpio_fun argc<5 !\r\n" );
		}
	}
    else if (strcmp(argv[1], "datain") == 0)  
    {  
		printf("gpio%d read %d bit= 0x%02x\r\n",buf[0], buf[1], data_get(buf[0],buf[1]));
	}
    else if (strcmp(argv[1], "cfgdata") == 0)  
    {  
		if(argc>=5)
		{
			gpio_cfg_data(buf[0],buf[1],buf[2]);
		}
		else
		{
			printf("gpio_fun argc<5 !\r\n" );
		}
	}
    else if (strcmp(argv[1], "endata") == 0)  
    {  
		if(argc>=5)
		{
			gpio_en_data(buf[0],buf[1],buf[2]);
		}
		else
		{
			printf("gpio_fun argc<5 !\r\n" );
		}
	}
    else if (strcmp(argv[1], "function") == 0)  
    {  
		if(argc>=4)
		{
			gpio_func_init(buf[0],buf[1]);
		}
		else
		{
			printf("gpio_fun argc<4 !\r\n" );
		}
	}
	gpio_close();
	return (0);
}  
