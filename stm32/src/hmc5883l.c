/******************************************************************************
  File			: hmc5883l.c
  Description	: hmc5883l digtal magnetic sensor driver
  Author		: Xiaoming Li
*******************************************************************************
  Modify List:
-------------------------------------------------------------------------------
  2014/9/7 21:00 PM	| Created
******************************************************************************/

#include "hmc5883l.h"

uint8_t hbuffer[6];

void hmc_init(void)
{
	SwI2C_WriteByte(HMC_DEVADDR, MODE_ADDR, MODE_0);
	SwI2C_WriteByte(HMC_DEVADDR, CTRL_A_ADDR, 0x78);	//clear bit7, 8,75Hz
	SwI2C_WriteByte(HMC_DEVADDR, CTRL_B_ADDR, 0x40);
}

u8 hmc_selt_test(int16_t *st_x, int16_t *st_y, int16_t *st_z)
{
	u8 tmp, res;
	tmp = SwI2C_ReadByte(HMC_DEVADDR, CTRL_A_ADDR);
	tmp &= ~(0x3);
	tmp |= P_SELT_TEST;
	SwI2C_WriteByte(HMC_DEVADDR, CTRL_A_ADDR, tmp);
	//设置进入单一测量模式
	SwI2C_WriteByte(HMC_DEVADDR, MODE_ADDR, MODE_1);
	delay_ms(100);
	res = hmc_getMagnetic(st_x, st_y, st_z);
	tmp &= ~(0x3);
	SwI2C_WriteByte(HMC_DEVADDR, CTRL_A_ADDR, tmp);
	//设置进入连续测量模式
	SwI2C_WriteByte(HMC_DEVADDR, MODE_ADDR, MODE_0);
	return res;
}

bool hmc_getDataReady(void)
{
	u8 status;
	status = SwI2C_ReadByte(HMC_DEVADDR, STATUS_ADDR);
	if(status & 0x1){
		HMC5883_DEBUG("data load ready\r\n");
		return TRUE;
	}
	else{
		HMC5883_DEBUG("data unload ready\r\n");
		return FALSE;
	}
}

bool hmc_getMagnetic(int16_t *mag_x, int16_t *mag_y, int16_t *mag_z)
{
	if(!SwI2C_ReadBytes(HMC_DEVADDR, DATA_X_MSB, 6, hbuffer)){
		HMC5883_DEBUG("read data fail!\r\n");
		return FALSE;
	}
	*mag_x = (((int16_t)hbuffer[0]) << 8) | hbuffer[1];
	*mag_y = (((int16_t)hbuffer[4]) << 8) | hbuffer[5];
	*mag_z = (((int16_t)hbuffer[2]) << 8) | hbuffer[3];
	
	return TRUE;
}

