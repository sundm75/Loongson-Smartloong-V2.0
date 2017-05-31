
CFLAGS += -Wall
obj := led 
src := led.c
CC  := mipsel-linux-gcc 

$(obj): $(src)
	$(CC) $(CFLAGS) $^ -o $@ -g

.PHONY: clean
clean:
	-rm $(obj) 
