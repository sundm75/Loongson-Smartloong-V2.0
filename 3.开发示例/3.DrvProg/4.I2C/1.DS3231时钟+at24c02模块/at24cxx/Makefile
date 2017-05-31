KERN_DIR = /Workstation/tools/kernel/linux-3.0.82-openloongson

all:
	make -C $(KERN_DIR) M=`pwd` modules ARCH=mips CROSS_COMPILE=mipsel-linux-

clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm -rf modules.order

#obj-m    += at24cxx_dev.o
#obj-m    += at24cxx_dev1.o
obj-m    += at24cxx_drv.o

