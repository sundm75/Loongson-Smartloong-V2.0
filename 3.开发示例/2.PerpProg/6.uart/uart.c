#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <asm/termios.h>

#include "serial.h"

#define DEV_NAME  "/dev/ttyS1"


int main (int argc, char *argv[])
{
	int fd;
	int len, i,ret;
        char buf[] = "Hello! I am Loongson!";


	fd = open(DEV_NAME, O_RDWR | O_NOCTTY);
        if(fd < 0) {
                perror(DEV_NAME);
                return -1;
        }


	 ret = set_port_attr (
                         	fd,  
                          	B115200,          // B1200 B2400 B4800 B9600 .. B115200
                         	8,           	  // 5, 6, 7, 8
                          	"1",    	  //  "1", "1.5", "2"
                          	'N',              // N(o), O(dd), E(ven)
                          	150,		  //VTIME
                          	255 );		  //VTIME

	if(ret < 0) {
		printf("set uart arrt faile \n");
		exit(-1);
	}

	len = write(fd, buf, sizeof(buf));
	if (len < 0) {
		printf("write data error \n");
		return -1;
	}
	
	len = read(fd, buf, sizeof(buf));
        if (len < 0) {
                printf("read error \n");
                return -1;
        }

	printf("%s \n", buf);

	return(0);
}

