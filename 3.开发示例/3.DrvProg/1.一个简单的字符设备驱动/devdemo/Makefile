obj-m := devdemo.o     
#定义目录变量
KDIR := /Workstation/tools/kernel/linux-3.0.82-openloongson
PWD := $(shell pwd)
all:	
# make文件
	make -C $(KDIR) M=$(PWD) modules ARCH=mips CROSS_COMPILE=mipsel-linux- 
clean:
	rm -rf *.o  *.mod.c *.ko

