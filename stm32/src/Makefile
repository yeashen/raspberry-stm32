# src Makefile

include ../Makefile.common

# system
OBJS += startup.o
OBJS += sys.o
OBJS += newlib_stubs.o

# main
OBJS += main.o

# peripheral
OBJS += delay.o

OBJS += led.o
OBJS += adc.o
OBJS += dac.o
OBJS += DMA.o
OBJS += exint.o
#OBJS += gps.o
OBJS += key.o
OBJS += mpu6050.o
OBJS += oled.o
OBJS += pwm.o
OBJS += motor.o
OBJS += spi.o
OBJS += standby.o
OBJS += sw_i2c.o
OBJS += timer.o
OBJS += usart.o
OBJS += PID_Ctrl.o
OBJS += pkthandle.o

all: src

src: app.a

app.a: $(OBJS)
		$(AR) cr app.a $(OBJS)
.PHONY: src clean tshow

clean:
		rm -f app.a $(OBJS)
tshow:
		@echo "######################################################################################################"
		@echo "################# optimize settings: $(InfoTextLib), $(InfoTextSrc)"
		@echo "######################################################################################################"
