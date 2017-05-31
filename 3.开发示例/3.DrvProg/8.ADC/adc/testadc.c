/**************************************************************************
 Loongson1s1c hwmon-adc driver test
  device node is "/sys/class/hwmon/hwmon0/device/adcN_raw" 
 ***************************************************************************/
/*testadc.c*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/fcntl.h>

int main(int argc, char *argv[])
{
    int fd = -1, ret;
    int adc_ch, value;
    char buffer[5];
    char dev_name[50];
    
    if ( argc < 2 )
    {
        printf("usage adc_test chanel(0~7)\n");
        exit(1);
    }
    else
    {
        adc_ch = atoi(argv[1]);
    }
    
    if ( (adc_ch >= 0) && (adc_ch <= 7) )
    {
        sprintf(dev_name, "/sys/class/hwmon/hwmon0/device/adc%d_raw\0", adc_ch);
        printf("dev name %s\n", dev_name);
    
        if ((fd = open(dev_name, O_RDONLY)) < 0)
        {
            perror("open error");
            exit(1);
        }
    }
    else
    {
        printf("adc_ch is (0~7)\n");
        exit(1);
    }
    
    ret = read(fd, buffer, 4);
    if (ret < 0)
    {
        perror("read error");
        exit(1);
    }
    
    value = atoi(buffer);
    printf("ADC %d current value is %d\n", adc_ch, value);
        
    close(fd);
    return 0;
}