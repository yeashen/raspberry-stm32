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
#OBJS += adc.o
#OBJS += dac.o
#OBJS += DMA.o
#OBJS += exint.o
#OBJS += gps.o
#OBJS += key.o
OBJS += oled.o
OBJS += pwm.o
OBJS += motor.o
OBJS += encoder.o
#OBJS += spi.o
#OBJS += standby.o
OBJS += sw_i2c.o
#OBJS += hw_i2c.o
OBJS += timer.o
OBJS += usart.o
OBJS += PID_Ctrl.o
OBJS += pkthandle.o
OBJS += attitude.o
OBJS += user_stdlib.o

# mpu6050
ifeq ($(CONFIG_USE_MPU6050),1)
ifeq ($(CONFIG_MPU6050_USE_DMP),1)
OBJS += inv_mpu.o
OBJS += inv_mpu_dmp_motion_driver.o
#OBJS += hmc5883l.o
else
OBJS += mpu6050.o
endif
endif

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
