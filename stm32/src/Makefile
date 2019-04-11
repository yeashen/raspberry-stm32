# src Makefile

include ../Makefile.common

# system
OBJS += adc.o
OBJS += control.o
#OBJS += DataScope_DP.o
OBJS += delay.o
OBJS += encoder.o
OBJS += exti.o
OBJS += filter.o
OBJS += inv_mpu.o
OBJS += inv_mpu_dmp_motion_driver.o
OBJS += i2c.o
OBJS += key.o
OBJS += led.o
OBJS += main.o
OBJS += motor.o
OBJS += mpu6050.o
#OBJS += newlib_stubs.o
OBJS += oled.o
OBJS += pkthandle.o
OBJS += show.o
OBJS += stmflash.o
OBJS += sys.o
OBJS += timer.o
OBJS += usart.o
OBJS += usart3.o
OBJS += startup.o

all: src

src: app.a

app.a: $(OBJS)
		$(AR) cr app.a $(OBJS)
.PHONY: src clean tshow

clean:
		rm -f app.a *.o 
tshow:
		@echo "######################################################################################################"
		@echo "################# optimize settings: $(InfoTextLib), $(InfoTextSrc)"
		@echo "######################################################################################################"
