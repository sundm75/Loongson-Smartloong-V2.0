# 指令编译器和选项
CC=mipsel-linux-gcc
CFLAGS=-Wall -std=gnu99 -Wstrict-prototypes 


# 目标文件
TARGET=testlcd
SRCS = testlcd.c \
  ./gpio_fun/gpio_fun.c \
  ./lcd/ili9341_fn.c\
  ./lcd/ili93xx.c

INC = -I./gpio_fun -I./lcd

OBJS = $(SRCS:.c=.o)

$(TARGET):$(OBJS)
	@echo TARGET:$@
	@echo OBJECTS:$^
	$(CC) -o $@ $^

clean:
	rm -rf $(TARGET) $(OBJS)

%.o:%.c
	$(CC) $(CFLAGS) $(INC) -o $@ -c $<
