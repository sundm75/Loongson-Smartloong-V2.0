#ifndef __GPIO_FUN_H
#define __GPIO_FUN_H 
#include<stdio.h>  
#include<stdlib.h>  
#include<stdint.h>  
#include<string.h>  
#include<unistd.h>  
#include<sys/mman.h>  
#include<sys/types.h>  
#include<sys/stat.h>  
#include<fcntl.h>  
#define GPIO_CFG_GPIO 	1
#define GPIO_CFG_VOID 	0
#define GPIO_EN_INPUT 	1
#define GPIO_EN_OUTPUT 	0
#define GPIO_OUT_HIGH 	1
#define GPIO_OUT_LOW 	0

/*常用GPIO操作函数*/
int gpio_open(void);
void gpio_close(void);
void gpio_cfg_init(int gpio_number, int function);
void gpio_en_init(int gpio_number, int function); 
void gpio_set(int gpio_number);
uint32_t gpio_get(int gpio_number);
void gpio_clr(int gpio_number);
uint32_t data_get(int gpio_start_number, int len);
void data_write(int gpio_start_number, int len ,uint32_t data);
void gpio_cfg_data(int gpio_start_number, int len ,uint32_t data);
void gpio_en_data(int gpio_start_number, int len ,uint32_t data);
void gpio_func_init(int gpio_number, int function);

#endif
