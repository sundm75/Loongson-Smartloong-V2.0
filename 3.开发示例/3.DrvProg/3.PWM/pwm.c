/*
 * 此测试程序用于控制pwm设备，对pwm的控制是通过配置占空比实现的，设置好所需要的高电平和低电平的所占的脉冲数目即可
 */
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#define CMD_PWM_GET     _IO('c',0x01)
#define CMD_PWM_START   _IO('c',0x02)
#define CMD_PWM_STOP    _IO('c',0x03)

int fd;
int pwminit(void)
{
	printf("cys: ready to open!\n");
	fd = open("/dev/ls1f-pwm",O_RDWR);
	return fd;
}
int pwmclose(void)
{
	close(fd);
	return 0;
}

//sel为选择PWM通道(0或者1);value为占空比(0-100);  freq为频率().
int pwmset(unsigned int sel, unsigned int value, unsigned int freq) {
	int fd;
    //data[0]的值代表的是高电平脉冲所占的时钟数，data[1]的值代表的是低电平脉冲所占的时钟数
	unsigned int data[2] = {0x7ffffff,0x7ffffff};

	if(value>100)
		value = 100;
	data[0] = freq / 100 * value;
	data[1] = freq / 100 * (100-value);

	if (fd != -1)
	{
	  printf("open ok!\n");
       //共有四路pwm(pwm0...pwm3)，四路pwm可同时工作，也可选择使用其中任何一路

		if (sel <0 || sel > 3)
		  return -1;
		switch(sel) {
		  case 0:
			//在驱动中没有用到ioctl的第三个参数，故直接赋予0就可以啦
		    ioctl(fd, CMD_PWM_START , 0);
		    write(fd, data, sizeof(data) );
		    break;
		  case 1:
            ioctl(fd, CMD_PWM_START , 1);
		    write(fd, data, sizeof(data) );
            break;
		  case 2:
            ioctl(fd, CMD_PWM_START , 2);
		    write(fd, data, sizeof(data) );
            break;
		  case 3:
            ioctl(fd, CMD_PWM_START , 3);
		    write(fd, data, sizeof(data) );
            break;
		  default:
		    break;
		}
	}
	else
	{
		printf("Device open failure\n");
	}
	return 0;
}
