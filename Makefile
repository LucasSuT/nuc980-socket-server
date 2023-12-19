.SUFFIXES : .x .o .c .s

CC := ~/MA35D1_Buildroot/output/host/bin/arm-linux-gcc
STRIP := ~/MA35D1_Buildroot/output/host/bin/arm-linux-strip

LED_TARGET = server
LED_SRCS := server.c

all: 
	$(CC) -static $(LED_SRCS) -o $(LED_TARGET)
	$(STRIP) $(LED_TARGET) 

clean:
	rm -f *.o 
	rm -f *.x 
	rm -f *.flat
	rm -f *.map
	rm -f temp
	rm -f *.img
	rm -f $(LED_TARGET)	
	rm -f $(KEY_TARGET)	
	rm -f *.gdb
	rm -f *.bak
