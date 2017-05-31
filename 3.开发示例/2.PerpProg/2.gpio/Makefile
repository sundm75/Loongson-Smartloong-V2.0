
CFLAGS += -Wall
obj := gpio 
src := gpio.c
CC  := mipsel-linux-gcc 

$(obj): $(src)
	$(CC) $(CFLAGS) $^ -o $@ -g

.PHONY: clean
clean:
	-rm $(obj) 
