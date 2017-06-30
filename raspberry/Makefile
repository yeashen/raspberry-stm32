CFLAGS += -Wall -g

OBJS =	main.o		\
	port.o		\
	serial_common.o	\
	serial_platform.o

all: rasp-stm32

serial_platform.o: serial_posix.c serial_w32.c

rasp-stm32: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

clean:
	rm -f $(OBJS) rasp-stm32
