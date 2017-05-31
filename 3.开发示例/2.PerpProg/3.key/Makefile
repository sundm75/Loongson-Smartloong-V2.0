
CFLAGS += -Wall
obj := key 
src := key.c
CC  := mipsel-linux-gcc 

$(obj): $(src)
	$(CC) $(CFLAGS) $^ -o $@ -g

.PHONY: clean
clean:
	-rm $(obj) 
