/*gpio.c*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <string.h>
#define DEV_PATH "/sys/class/gpio/gpio49/value" //输入输出电平值设备
#define EXPORT_PATH "/sys/class/gpio/export" //GPIO 设备导出设备
#define DIRECT_PATH "/sys/class/gpio/gpio49/direction" //GPIO 输入输出控制设备
#define OUT "out"
#define IN "in"
#define GPIO "49" //GPIO49
#define HIGH_LEVEL "1"
#define LOW_LEVEL "0"
int main(int argc, char ** argv)
{
	static int fd_dev, fd_export, fd_dir, ret;
	char buf[10], direction[4];
	fd_export = open(EXPORT_PATH, O_WRONLY); //打开 GPIO 设备导出设备
	if(fd_export < 0) {
		perror("open export:");
		return -1;
	}
	write(fd_export, GPIO, strlen(GPIO));
	fd_dev = open(DEV_PATH, O_RDWR); //打开输入输出电平值设备
	if(fd_dev < 0) {
		perror("export write:");
		return -1;
	}
	fd_dir = open(DIRECT_PATH, O_RDWR); //打开 GPIO 输入输出控制设备
	if(fd_dir < 0) {
		perror("export write:");
		return -1;
	}
	ret = read(fd_dir, direction, sizeof(direction)); //读取 GPIO2_4 输入输出方向
	if(ret < 0) {
		perror("dir read:");
		close(fd_export);
		close(fd_dir);
		close(fd_dev);
		return -1;
	}
	printf("default directions:%s",direction);
	strcpy(buf, IN);
	ret = write(fd_dir, buf, strlen(IN));
	if(ret < 0) {
		perror("dir read:");
		close(fd_export);
		close(fd_dir);
		close(fd_dev);
		return -1;
	}
	ret = read(fd_dir, direction, sizeof(direction));
	if(ret < 0) {
		perror("dir read:");
		close(fd_export);
		close(fd_dir);
		close(fd_dev);
		return -1;
	}
	ret = read(fd_dev, buf, sizeof(buf)); //读取 GPIO2_4 输入电平值
	if(ret < 0) {
		perror("dir read:");
		close(fd_export);
		close(fd_dir);
		close(fd_dev);
		return -1;
	}
	printf("now directions:%sinput level:%s",direction,buf);
	close(fd_export);
	close(fd_dir);
	close(fd_dev);
	return 0;
}
