#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <string.h>

#define TRIGGER				"trigger"
#define LED_PATH 			"/sys/class/leds/"
#define LED_STATUS			"brightness"
#define TRIGGER_NONE			"none"

int main(int argc,char **argv)
{
	char path[20],data[2];
	int fd, ret, flag;
	if(argv[1] == NULL) {
		printf("usage : ./led led_***  ");
		return 0;
	}

	strcpy(path, LED_PATH);
	strcat(path, argv[1]);
	strcat(path, "/" TRIGGER);
	fd = open(path, O_RDWR);
	if(fd < 0) {
		perror("open");
		return -1;
	}
	ret = write(fd, TRIGGER_NONE, strlen(TRIGGER_NONE));
	if(ret < 0) {
		perror("write");
		return -1;
	}
	close(fd);	
	strcpy(path, LED_PATH);
	strcat(path, argv[1]);
	strcat(path, "/" LED_STATUS);
	fd = open(path, O_WRONLY);
        if(fd < 0) {
                perror("open");
                return -1;
        }
	for(;;)
	{
		data[0] = flag ? '0': '1'; 
		ret = write(fd, data, 1);
		if(ret < 0) {
		perror("write");
		return -1;
	}
		flag = !flag;
		sleep(1);
	}
	return 0;
}

