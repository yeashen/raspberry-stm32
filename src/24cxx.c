/******************************************************************************
  File			: 24cxx.c
  Description	: eeprom driver
  Author		: Xiaoming Li
*******************************************************************************
  Modify List:
-------------------------------------------------------------------------------
  2014/8/17 21:00 PM	| Created
******************************************************************************/

#include "24cxx.h"
#include "delay.h"
#include "usart.h"

/*-------------------------------------------------------------------
 * FUNC : at24cxx_init
 * DESC : at24cxx initial
 * PARM : N/A
 * RET	: N/A
 *-----------------------------------------------------------------*/
void at24cxx_init(void)
{
	sw_i2c_init();
}

/*-------------------------------------------------------------------
 * FUNC : at24cxx_readByte
 * DESC : at24cxx read one data from a address
 * PARM : raddr - read addr
 * RET	: res - read data
 *-----------------------------------------------------------------*/
u8 at24cxx_readByte(u16 raddr)
{
	u8 res;
	sw_i2c_start();
	sw_i2c_send_byte(0xA0);
	sw_i2c_wait_ack();
	sw_i2c_send_byte(raddr >> 8);
	sw_i2c_wait_ack();
	sw_i2c_send_byte(raddr%256);
	sw_i2c_wait_ack();
	sw_i2c_start();
	sw_i2c_send_byte(0xA1);
	sw_i2c_wait_ack();
	res = sw_i2c_recv_byte(0);
	sw_i2c_stop();
	return res;
}

/*-------------------------------------------------------------------
 * FUNC : at24cxx_writeByte
 * DESC : at24cxx write one data at a address
 * PARM : waddr - write addr
 * 		  wdata - write data
 * RET	: N/A
 *-----------------------------------------------------------------*/
void at24cxx_writeByte(u16 waddr, u8 wdata)
{
	sw_i2c_start();
	sw_i2c_send_byte(0xA0);
	sw_i2c_wait_ack();
	sw_i2c_send_byte(waddr >> 8);
	sw_i2c_wait_ack();
	sw_i2c_send_byte(waddr%256);
	sw_i2c_wait_ack();
	sw_i2c_start();
	sw_i2c_send_byte(wdata);
	sw_i2c_wait_ack();
	sw_i2c_stop();
	delay_ms(10);
}

void at24cxx_writeData(u16 waddr, u8 wdata, u8 len)
{
	u8 i;
	for(i = 0; i < len; i++)
		at24cxx_writeByte(waddr+i, (wdata>>(8*i))&0xff);
}

u32 at24cxx_readData(u16 raddr, u8 len)
{
	u8 i;
	u32 res = 0;
	for(i = 0; i < len; i++){
		res <<= 8;
		res += at24cxx_readByte(raddr + len-i-1); 	 				   
	}
	return res;	
}

u8 at24cxx_check(void)
{
	u8 res;
	res = at24cxx_readByte(255);
	printf("check res: %d\r\n", res);
	if(res == 0x55)
		return 0;
	else{
		at24cxx_writeByte(255, 0x55);
		res = at24cxx_readByte(255);
		if(res == 0x55)
			return 0;
	}
	return 1;
}

void at24cxx_read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++ = at24cxx_readByte(ReadAddr++);	
		NumToRead--;
	}
} 

void at24cxx_write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
	while(NumToWrite--)
	{
		at24cxx_writeByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}
