#ifndef __SW_I2C_H
#define __SW_I2C_H

#ifdef __cplusplus
 extern "C" {
#endif
	 
#include "stm32f10x_i2c.h" 

//#define I2C_PDEBUG
	 
#ifdef I2C_PDEBUG
#define I2C_DEBUG(string,args...)	printf("[HWI2C_DEBUG] ");	\
									printf(string, ##args)
#else
#define I2C_DEBUG(string,args...)
#endif

void HwI2C_Init(void);
bool HwI2C_WriteByte(uint8_t addr, uint8_t reg, uint8_t data);
uint8_t HwI2C_ReadByte(uint8_t addr, uint8_t reg);
bool HwI2C_ReadBytes(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf);

#ifdef __cplusplus
}
#endif

#endif /* __SW_I2C_H */
