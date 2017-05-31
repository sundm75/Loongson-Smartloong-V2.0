/*
 * Name        : key.c
 * Author      : sundm75
 * Version     : v1.0
 * Copyright   : 2016  by sundm75
 * Description : key in C, Ansi-style
 * Change Logs:
 * Date           Author       Notes
 * 2016-09-25     sundm75      first version
 */
/*key.c*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#define DEV_PATH "/dev/input/event0"   //difference is possible

int main(int argc, char ** argv)
{
    int keys_fd;
    struct input_event t;
    int direct = 0;
    keys_fd=open(DEV_PATH, O_RDONLY);
    if(keys_fd <= 0)
    {
        printf("open /dev/input/event0 device error!\n");
        return -1;
    }
    while(1)
    {
        if(read(keys_fd, &t, sizeof(t)) == sizeof(t))
        {
            if(t.type==EV_KEY)
                //if(t.value==0 || t.value==1)
                {
                    printf("key %d %s\n", t.code, (t.value) ? "Pressed" : "Released");
                    direct = (t.value) ? 1 : (-1);//1 : pressed -1:Released
                }
        }
    }
    close(keys_fd);
    return t.code * direct;//
    //2 -2 					11 -11					3 -3
    //KEY3 pressed relaese  KEY2 pressed relaese  KEY1 pressed relaese
}
