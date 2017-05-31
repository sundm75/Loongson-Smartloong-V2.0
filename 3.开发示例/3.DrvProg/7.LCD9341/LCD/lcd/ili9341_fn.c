#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <fcntl.h>  
#include <sys/ioctl.h>   
#include <stdint.h>  
#include <string.h>  
#include <sys/mman.h>  
#include <sys/types.h>  
#include  <sys/stat.h>   
#include <linux/types.h>  
#include <linux/stat.h>  
#include "gpio_fun.h"
#include "ili9341.h"


void LCD_BK_LED(void)//背光打开 
{
	gpio_set(LCD_BL); //背光输出高
}

void LCD_gpio_init(void)//设置需要用的GPIO口
{
	//int set_GPIO_CFG;
	//配置74 77 88 89 90 使能端口
	//set_GPIO_CFG=readl(GPIO_CFG2);set_GPIO_CFG|=0x7002400;writel(set_GPIO_CFG,GPIO_CFG2);
	  gpio_cfg_init( 77, GPIO_CFG_GPIO);
	  gpio_cfg_init( 88, GPIO_CFG_GPIO);
	  gpio_cfg_init( 90, GPIO_CFG_GPIO);
	  gpio_cfg_init( 89, GPIO_CFG_GPIO);
	  gpio_cfg_init( 74, GPIO_CFG_GPIO);	
	//74 77 88 89 90 EN置0 输出模式
	//set_GPIO_CFG=readl(GPIO_EN2);set_GPIO_CFG&=0xf8ffdbff;writel(set_GPIO_CFG,GPIO_EN2);
	  gpio_en_init( 77, GPIO_EN_OUTPUT);
	  gpio_en_init( 88, GPIO_EN_OUTPUT);
	  gpio_en_init( 90, GPIO_EN_OUTPUT);
	  gpio_en_init( 89, GPIO_EN_OUTPUT);
	  gpio_en_init( 74, GPIO_EN_OUTPUT);
	  
	//DATA数据口 58:63
	//set_GPIO_CFG=readl(GPIO_CFG1);set_GPIO_CFG|=0xfc000000;writel(set_GPIO_CFG,GPIO_CFG1);
	gpio_cfg_data(58, 6, GPIO_CFG_GPIO);
	gpio_cfg_data(64, 10, GPIO_CFG_GPIO);
	
	//DATA数据口 64:73
	//set_GPIO_CFG=readl(GPIO_CFG2);set_GPIO_CFG|=0x3ff;writel(set_GPIO_CFG,GPIO_CFG2);
	gpio_en_data(58, 6, GPIO_EN_OUTPUT);
	gpio_en_data(64, 10, GPIO_EN_OUTPUT);

	LCD_DATA_gpio_out_mod();
}

u16 read_data(void)
{
	u32 read_data;
	read_data = data_get(LCD_d0, 6);
	read_data = (data_get(LCD_d6, 10)<<6) | read_data ;
	return read_data;
}

u8 Init_ili9341(void)
{
	lcddev.id=0;
	printf("Init LCD\r\n"); 
	LCD_gpio_init();
	mdelay(50);	// delay 50 ms 
	LCD_Init();
	LCD_BK_LED();	//点亮背光
	printf("DRIVER GET LCD ID:0x%x\r\n",lcddev.id); //打印LCD ID
	if(lcddev.id<0XFF||lcddev.id==0XFFFF||lcddev.id==0X9300)
	{
		printf("ili9341.c-Init_ili9341():ILI9341 init failed!\n");
		return 1;
	}
	else
	{
		LCD_Clear(BLUE);
		printf("ili9341.c-Init_ili9341():ILI9341 init Successful!\n");
	 	return 0;  
	}
}

