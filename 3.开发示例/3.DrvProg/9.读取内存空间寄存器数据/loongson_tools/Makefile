all: gpio_func cbus_dump regs_write regs_read regs_bit_clr regs_bit_get regs_bit_set
clean:
	rm  -rf bin


cbus_dump: cbus_dump.c
	mkdir -p bin
	mipsel-linux-gcc -o bin/cbus_dump cbus_dump.c 

gpio_func: gpio_func.c
	mkdir -p bin
	mipsel-linux-gcc -o bin/gpio_func gpio_func.c 

regs_write: regs_write.c
	mkdir -p bin
	mipsel-linux-gcc -o bin/regs_write regs_write.c 

regs_read: regs_read.c
	mkdir -p bin
	mipsel-linux-gcc -o bin/regs_read regs_read.c 

regs_bit_clr: regs_bit_clr.c
	mkdir -p bin
	mipsel-linux-gcc -o bin/regs_bit_clr regs_bit_clr.c 

regs_bit_get: regs_bit_get.c
	mkdir -p bin
	mipsel-linux-gcc -o bin/regs_bit_get regs_bit_get.c 

regs_bit_set: regs_bit_set.c
	mkdir -p bin
	mipsel-linux-gcc -o bin/regs_bit_set regs_bit_set.c 

