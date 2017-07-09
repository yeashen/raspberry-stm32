#ifndef __HMC5883L_H
#define __HMC5883L_H

#ifdef __cplusplus
 extern "C" {
#endif
	 
#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"

#if defined(SW_I2C)
	#include "sw_i2c.h"
#else 
	#include "hw_i2c.h"
#endif

#define HMC_DEVADDR		0x3C	//HMC5883 I2C address

//#define HMC5883_PDEBUG
#ifdef HMC5883_PDEBUG
#define HMC5883_DEBUG(string,args...)	printf("[HMC5883_DEBUG] ");	\
									printf(string, ##args)
#else
#define HMC5883_DEBUG(string,args...)
#endif	

//HMC5883L register address define
#define REG_BASE_ADDR		0x00
#define CTRL_A_ADDR			REG_BASE_ADDR
#define CTRL_B_ADDR			REG_BASE_ADDR+1
#define MODE_ADDR			REG_BASE_ADDR+2
#define DATA_X_MSB			REG_BASE_ADDR+3
#define DATA_X_LSB			REG_BASE_ADDR+4
#define DATA_Z_MSB			REG_BASE_ADDR+5
#define DATA_Z_LSB			REG_BASE_ADDR+6
#define DATA_Y_MSB			REG_BASE_ADDR+7
#define DATA_Y_LSB			REG_BASE_ADDR+8
#define STATUS_ADDR			REG_BASE_ADDR+9
#define IDEN_A_ADDR			REG_BASE_ADDR+10
#define IDEN_B_ADDR			REG_BASE_ADDR+11
#define IDEN_C_ADDR			REG_BASE_ADDR+12

#define MODE_0				0x0	//连续测量
#define MODE_1				0x1	//单一测量
#define P_SELT_TEST			0x1	//正偏压自测
#define N_SETL_TEST			0x2	//负偏压自测


void hmc_init(void);
bool hmc_selt_test(int16_t *st_x, int16_t *st_y, int16_t *st_z);
bool hmc_getDataReady(void);
bool hmc_getMagnetic(int16_t *mag_x, int16_t *mag_y, int16_t *mag_z);

#ifdef __cplusplus
}
#endif

#endif /* __HMC5883L_H */
