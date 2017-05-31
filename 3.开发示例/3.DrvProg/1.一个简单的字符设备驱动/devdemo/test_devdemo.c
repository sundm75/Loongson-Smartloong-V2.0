#include <stdio.h>  
#include <errno.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
#include <string.h>  
#include <unistd.h>  
  
#define BUF_SIZE  60  

  
int main(void)  
{  
    int fd;  
    int len;  
    int ret;  
    char buf1[BUF_SIZE] = "hello";  
      
    fd = open("/dev/cdevdemo", O_RDWR);  
    if (fd < 0)  
    {  
        perror("open error");  
        return -1;  
    }  

    len = read(fd, buf1, BUF_SIZE);  
      
    printf("the string is : %s\n", buf1);  


    close(fd);
      
    return 0;  
}  
