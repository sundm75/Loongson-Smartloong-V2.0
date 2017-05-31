#include  <stdio.h>
#include  <sys/stat.h>
#include  <sys/ioctl.h>
#include  <fcntl.h>
#include  <errno.h>
/////////////////////////////////////////////////
int  main(int  argc,  char  **argv)
{
        int  fd;
        fd  =  open("/dev/miscdemo_node",  O_RDONLY);
        if  (fd  <  0)
        {
                printf("open  /dev/miscdemo_node  failed!\n");
                printf("%s\n",  strerror(errno));
                return  -1;
        }
        
        printf("open  /dev/miscdemo_node  ok!\n");
        if  (ioctl(fd,  6)  !=  0)
        {
                printf("ioctl  failed!\n");
                printf("%s\n",  strerror(errno));
        }
        else
                printf("ioctl  ok!\n");
        close(fd);
        return  0;
}
