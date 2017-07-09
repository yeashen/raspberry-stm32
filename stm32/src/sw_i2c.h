#ifndef __SW_I2C_H
#define __SW_I2C_H

#ifdef __cplusplus
 extern "C" {
#endif
	 
#include "stm32f10x.h" 
 
#define SCL_H         (GPIOB->BSRR = GPIO_Pin_8)
#define SCL_L         (GPIOB->BRR  = GPIO_Pin_8) 
                     
#define SDA_H         (GPIOB->BSRR = GPIO_Pin_9)
#define SDA_L         (GPIOB->BRR  = GPIO_Pin_9)
                      
#define SCL_read      (GPIOB->IDR  & GPIO_Pin_8)
#define SDA_read      (GPIOB->IDR  & GPIO_Pin_9)

#define I2C_WR	((uint8_t)0x00)
#define I2C_RD	((uint8_t)0x01)

//#define I2C_PDEBUG
#ifdef I2C_PDEBUG
#define I2C_DEBUG(string,args...)	printf("[SWI2C_DEBUG] ");	\
									printf(string, ##args)
#else
#define I2C_DEBUG(string,args...)
#endif

void SwI2C_init(void);
bool sw_i2c_start(void);
void sw_i2c_stop(void);
bool sw_i2c_wait_ack(void);
void sw_i2c_ack(void);
void sw_i2c_noack(void);
void sw_i2c_send_byte(u8 by);
u8 sw_i2c_recv_byte(void);

bool SwI2C_WriteByte(u8 addr, u8 reg, u8 data);
u8 SwI2C_ReadByte(u8 addr, u8 reg);
bool SwI2C_ReadBytes(u8 addr, u8 reg, u8 len, u8 *buf);
bool SwI2C_WriteBytes(uint8_t addr, uint8_t reg, uint8_t len, uint8_t * data);

#ifdef __cplusplus
}
#endif

#endif /* __SW_I2C_H */
