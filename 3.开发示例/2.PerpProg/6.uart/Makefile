EXEC	= uart
OBJS    = uart.o serial.o

CROSS = mipsel-linux-
CC	= $(CROSS)gcc 

STRIP	= $(CROSS)strip
CFLAGS	= -g -O2

all:  clean $(EXEC)

$(EXEC):$(OBJS)
	$(CC) $(CFLAGS)	-o $@ $(OBJS)

clean:
	-rm -f $(EXEC) *.o
