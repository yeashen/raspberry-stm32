#ifndef __SW_I2C_H
#define __SW_I2C_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "sys.h"	 
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h" 
 
#define I2C_SCK    		PCout(12)
#define I2C_SDA    		PCout(11)
#define READ_SDA()   	PCin(11)

#define SDA_IN()	{GPIOC->CRH &= 0XFFFF0FFF; GPIOC->CRH |= 8<<12;}
#define SDA_OUT()	{GPIOC->CRH &= 0XFFFF0FFF; GPIOC->CRH |= 3<<12;}

#define I2C_WR	0x00
#define I2C_RD	0x01

void sw_i2c_init(void);
void sw_i2c_start(void);
void sw_i2c_stop(void);
u8 sw_i2c_wait_ack(void);
void sw_i2c_ack(void);
void sw_i2c_noack(void);
void sw_i2c_send_byte(u8 by);
u8 sw_i2c_recv_byte(u8 ack);

u8 sw_i2c_write_byte(u8 addr, u8 reg, u8 data);
u8 sw_i2c_read_bytes(u8 addr, u8 reg, u8 len, u8 *buf);
u8 sw_i2c_write_buffer(uint8_t addr, uint8_t reg, uint8_t len, uint8_t * data);

#ifdef __cplusplus
}
#endif

#endif /* __SW_I2C_H */
