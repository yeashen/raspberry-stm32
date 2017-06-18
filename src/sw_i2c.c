/******************************************************************************
  File			: sw_i2c.c
  Description	: soft i2c driver
  Author		: Xiaoming Li
*******************************************************************************
  Modify List:
-------------------------------------------------------------------------------
  2014/8/16 21:00 PM	| Created
******************************************************************************/

#include "sw_i2c.h"
#include "delay.h"
//#include "usart.h"

/*-------------------------------------------------------------------
 * FUNC : sw_i2c_init
 * DESC : soft i2c init, configure pin
 * PARM : N/A
 * RET	: N/A
 *-----------------------------------------------------------------*/
void sw_i2c_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	I2C_SCK = 1;
	I2C_SDA = 1;
}

/*-------------------------------------------------------------------
 * FUNC : sw_i2c_start
 * DESC : soft i2c make start signal
 * PARM : N/A
 * RET	: N/A
 *-----------------------------------------------------------------*/
void sw_i2c_start(void)
{
	SDA_OUT();
	I2C_SCK = 1;
	I2C_SDA = 1;
	delay_us(4);
	I2C_SDA = 0;
	delay_us(4);
	I2C_SCK = 0;
}

/*-------------------------------------------------------------------
 * FUNC : sw_i2c_stop
 * DESC : soft i2c make stop signal
 * PARM : N/A
 * RET	: N/A
 *-----------------------------------------------------------------*/
void sw_i2c_stop(void)
{
	SDA_OUT();
	I2C_SCK = 1;
	I2C_SDA = 0;
	delay_us(4);
	I2C_SCK = 1;
	I2C_SDA = 1;
	delay_us(4);
}

/*-------------------------------------------------------------------
 * FUNC : sw_i2c_wait_ack
 * DESC : soft i2c wait ack signal
 * PARM : N/A
 * RET	: ret - 0: no ack
 *			  - 1: ack
 *-----------------------------------------------------------------*/
u8 sw_i2c_wait_ack(void)
{
	u8 timeout = 0;
	SDA_IN();
	I2C_SDA  = 1;
	delay_us(1);
	I2C_SCK = 1;
	delay_us(1);
	while(READ_SDA()){
		timeout++;
		if(timeout > 250){
			sw_i2c_stop();
			return 0;
		}
	}
	I2C_SCK = 0;
	return 1;
}

/*-------------------------------------------------------------------
 * FUNC : sw_i2c_ack
 * DESC : soft i2c make acknowlege signal
 * PARM : N/A
 * RET	: N/A
 *-----------------------------------------------------------------*/
void sw_i2c_ack(void)
{
	I2C_SCK = 0;
	SDA_OUT();
	I2C_SDA = 0;
	delay_us(2);
	I2C_SCK = 1;
	delay_us(2);
	I2C_SCK = 0;
}

/*-------------------------------------------------------------------
 * FUNC : sw_i2c_noack
 * DESC : soft i2c don't make acknowlege signal
 * PARM : N/A
 * RET	: N/A
 *-----------------------------------------------------------------*/
void sw_i2c_noack(void)
{
	I2C_SCK = 0;
	SDA_OUT();
	I2C_SDA = 1;
	delay_us(2);
	I2C_SCK = 1;
	delay_us(2);
	I2C_SCK = 0;	
}

/*-------------------------------------------------------------------
 * FUNC : sw_i2c_send_byte
 * DESC : soft i2c send one byte data to i2c bus
 * PARM : by - sendding data
 * RET	: N/A
 *-----------------------------------------------------------------*/
void sw_i2c_send_byte(u8 by)
{
	u8 i;
	SDA_OUT();
	I2C_SCK = 0;
	for(i = 0; i < 8; i++){
		I2C_SDA = (by & 0x80)>>7;
		by <<= 1;
		delay_us(2);
		I2C_SCK = 1;
		delay_us(2);
		I2C_SCK = 0;
		delay_us(2);
	}
}

/*-------------------------------------------------------------------
 * FUNC : sw_i2c_recv_byte
 * DESC : soft i2c recivet one byte data to i2c bus
 * PARM : ack - 0: don't make ack signal
 *				1: make ack signal
 * RET	: res - recive one byte data
 *-----------------------------------------------------------------*/
u8 sw_i2c_recv_byte(u8 ack)
{
	u8 i;
	u8 res = 0;
	SDA_IN();
	for(i = 0; i < 8; i++){
		I2C_SCK = 0;
		delay_us(2);
		I2C_SCK = 1;
		res <<= 1;
		if(READ_SDA())
			res++;
		delay_us(1);
	}
	if(!ack)
		sw_i2c_noack();
	else
		sw_i2c_ack();
	return res;
}

u8 sw_i2c_write_byte(u8 addr, u8 reg, u8 data)
{
	sw_i2c_send_byte(addr << 1 | I2C_WR);
	if(!sw_i2c_wait_ack()){
		sw_i2c_stop();
		return 0;
	}
	sw_i2c_send_byte(reg);
	sw_i2c_wait_ack();
	sw_i2c_send_byte(data);
	sw_i2c_wait_ack();
	sw_i2c_stop();
	return 1;
} 

u8 sw_i2c_read_bytes(u8 addr, u8 reg, u8 len, u8 *buf)
{
	sw_i2c_send_byte(addr << 1 | I2C_WR);
	if(!sw_i2c_wait_ack()){
		sw_i2c_stop();
		return 0;
	}
	sw_i2c_send_byte(reg);
	sw_i2c_wait_ack();
	sw_i2c_start();
	sw_i2c_send_byte(addr << 1 | I2C_RD);
	sw_i2c_wait_ack();
	while(len){
		*buf = sw_i2c_recv_byte(0);
		if(len == 1)
			sw_i2c_noack();
		else
			sw_i2c_ack();
		buf++;
		len--;
	}
	sw_i2c_stop();
	return 1;
}

u8 sw_i2c_write_buffer(uint8_t addr, uint8_t reg, uint8_t len, uint8_t * data)
{
    int i;
    sw_i2c_send_byte(addr << 1 | I2C_WR);
    if (!sw_i2c_wait_ack()) {
        sw_i2c_stop();
        return 0;
    }
    sw_i2c_send_byte(reg);
    sw_i2c_wait_ack();
    for (i = 0; i < len; i++) {
        sw_i2c_send_byte(data[i]);
        if (!sw_i2c_wait_ack()) {
            sw_i2c_stop();
            return 0;
        }
    }
    sw_i2c_stop();
    return 1;
}

