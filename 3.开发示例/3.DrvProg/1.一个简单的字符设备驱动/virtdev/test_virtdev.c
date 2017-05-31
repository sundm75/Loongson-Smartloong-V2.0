#include <stdio.h>  
#include <errno.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
#include <string.h>  
#include <unistd.h>  
  
#define BUF_SIZE  60  
#define MEM_CLEAR    0x1  
#define MEM_PRINTER    0x2  
  
int main(void)  
{  
    int fd;  
    int len;  
    int ret;  
    char buf1[BUF_SIZE] = "hello";  
    char buf2[BUF_SIZE] = " world";  
      
    fd = open("/dev/virtdev", O_RDWR);  
    if (fd < 0)  
    {  
        perror("open error");  
        return -1;  
    }  
      
    len = write(fd, buf1, strlen(buf1));  
    len = write(fd, buf2, strlen(buf2));  
    lseek(fd, 0, SEEK_SET);  
    len = read(fd, buf1, BUF_SIZE);  
      
    printf("the string is : %s\n", buf1);  
      
    ret = ioctl(fd, MEM_CLEAR, NULL);  
    if (ret == -1)  
    {  
        perror("ioctl error");  
        return -1;  
    } 
    close(fd); 
      
    return 0;  
}  
