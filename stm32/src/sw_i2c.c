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
#include "usart.h"

#define I2C_CLKP		2	//delay 2us
#define I2C_DELAY()		delay_us(2)

//#define SHOW_DATA

static bool is_I2c_Init = FALSE;

static void I2C_delay(void)
{
		
   u8 i=30; //这里可以优化速度	，经测试最低到5还能写入
   while(i){ 
		i--; 
   }  
}

/*-------------------------------------------------------------------
 * FUNC : sw_i2c_init
 * DESC : soft i2c init, configure pin
 * PARM : N/A
 * RET	: N/A
 *-----------------------------------------------------------------*/
void SwI2C_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	if(is_I2c_Init){
		I2C_DEBUG("I2C has already Init...\r\n");
		return;
	}else{
		I2C_DEBUG("I2C unInit...\r\n");
	}
	I2C_DEBUG("I2C Init...\r\n");
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	SCL_H;
	SDA_H ;

	is_I2c_Init = TRUE;
}

/*-------------------------------------------------------------------
 * FUNC : sw_i2c_start
 * DESC : soft i2c make start signal
 * PARM : N/A
 * RET	: FALSE/TRUE
 *-----------------------------------------------------------------*/
bool sw_i2c_start(void)
{
	SDA_H;
	SCL_H;
	I2C_delay();
	if(!SDA_read)return FALSE;	//SDA线为低电平则总线忙,退出
	SDA_L;
	I2C_delay();
	if(SDA_read) return FALSE;	//SDA线为高电平则总线出错,退出
	SDA_L;
	I2C_delay();
	return TRUE;
}

/*-------------------------------------------------------------------
 * FUNC : sw_i2c_stop
 * DESC : soft i2c make stop signal
 * PARM : N/A
 * RET	: N/A
 *-----------------------------------------------------------------*/
void sw_i2c_stop(void)
{
	SCL_L;
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SDA_H;
	I2C_delay();
}

/*-------------------------------------------------------------------
 * FUNC : sw_i2c_wait_ack
 * DESC : soft i2c wait ack signal
 * PARM : N/A
 * RET	: ret - 0: no ack
 *			  - 1: ack
 *-----------------------------------------------------------------*/
bool sw_i2c_wait_ack(void)
{
	SCL_L;
	I2C_delay();
	SDA_H;			
	I2C_delay();
	SCL_H;
	I2C_delay();
	if(SDA_read)
	{
      SCL_L;
	  I2C_delay();
      return FALSE;
	}
	SCL_L;
	I2C_delay();
	return TRUE;
}

/*-------------------------------------------------------------------
 * FUNC : sw_i2c_ack
 * DESC : soft i2c make acknowlege signal
 * PARM : N/A
 * RET	: N/A
 *-----------------------------------------------------------------*/
void sw_i2c_ack(void)
{
	SCL_L;
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();
}

/*-------------------------------------------------------------------
 * FUNC : sw_i2c_noack
 * DESC : soft i2c don't make acknowlege signal
 * PARM : N/A
 * RET	: N/A
 *-----------------------------------------------------------------*/
void sw_i2c_noack(void)
{
	SCL_L;
	I2C_delay();
	SDA_H;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();	
}

/*-------------------------------------------------------------------
 * FUNC : sw_i2c_send_byte
 * DESC : soft i2c send one byte data to i2c bus
 * PARM : by - sendding data
 * RET	: N/A
 *-----------------------------------------------------------------*/
void sw_i2c_send_byte(u8 by)
{
    u8 i=8;
    while(i--)
    {
        SCL_L;
        I2C_delay();
      if(by & 0x80)
        SDA_H;  
      else 
        SDA_L;   
        by <<= 1;
        I2C_delay();
		SCL_H;
        I2C_delay();
    }
    SCL_L;
}

/*-------------------------------------------------------------------
 * FUNC : sw_i2c_recv_byte
 * DESC : soft i2c recivet one byte data to i2c bus
 * PARM : ack - 0: don't make ack signal
 *				1: make ack signal
 * RET	: res - recive one byte data
 *-----------------------------------------------------------------*/
u8 sw_i2c_recv_byte()
{
    u8 i=8;
    u8 ReceiveByte=0;

    SDA_H;				
    while(i--)
    {
      ReceiveByte<<=1;      
      SCL_L;
      I2C_delay();
	  SCL_H;
      I2C_delay();	
      if(SDA_read)
      {
        ReceiveByte|=0x01;
      }
    }
    SCL_L;
    return ReceiveByte;
}

bool SwI2C_WriteByte(u8 addr, u8 reg, u8 data)
{
	if(!sw_i2c_start()){
		I2C_DEBUG("WriteByte I2C bus busy...\r\n");
		return FALSE;
	}
	sw_i2c_send_byte(addr | I2C_WR);
	if(!sw_i2c_wait_ack()){
		sw_i2c_stop();
		I2C_DEBUG("WriteByte I2C bus no ack...\r\n");
		return FALSE;
	}
	sw_i2c_send_byte(reg);
	if(!sw_i2c_wait_ack()){
		sw_i2c_stop();
		I2C_DEBUG("WriteByte I2C bus no ack for reg!\r\n");
		return FALSE;
	}
	sw_i2c_send_byte(data);
	if(!sw_i2c_wait_ack()){
		sw_i2c_stop();
		I2C_DEBUG("WriteByte I2C bus no ack for data!\r\n");
		return FALSE;
	}
	sw_i2c_stop();
	delay_ms(5);
#ifdef SHOW_DATA
	printf("[SWI2C_DEBUG] [WriteByte]");
	printf("0x%x ", data);
	printf("\r\n");
#endif

	return TRUE;
} 

u8 SwI2C_ReadByte(u8 addr, u8 reg)
{
	u8 readData;

	if(!sw_i2c_start()){
		I2C_DEBUG("ReadBytes I2C bus busy...\r\n");
		return FALSE;
	}
	sw_i2c_send_byte(addr | I2C_WR);
	if(!sw_i2c_wait_ack()){
		sw_i2c_stop();
		I2C_DEBUG("ReadBytes I2C bus no ack!\r\n");
		return FALSE;
	}
	sw_i2c_send_byte(reg);
	if(!sw_i2c_wait_ack()){
		sw_i2c_stop();
		I2C_DEBUG("ReadBytes I2C bus no ack for reg!\r\n");
		return FALSE;
	}
	sw_i2c_start();
	sw_i2c_send_byte(addr | I2C_RD);
	if(!sw_i2c_wait_ack()){
		sw_i2c_stop();
		I2C_DEBUG("ReadBytes I2C bus no ack for address!\r\n");
		return FALSE;
	}
	
	readData = sw_i2c_recv_byte();
	sw_i2c_noack();
	sw_i2c_stop();

#ifdef SHOW_DATA
	printf("[SWI2C_DEBUG] [ReadByte]");
	printf("0x%x ",readData);
	printf("\r\n");
#endif

	return readData;
}

bool SwI2C_ReadBytes(u8 addr, u8 reg, u8 len, u8 *buf)
{
	u8 j = len;
#ifdef SHOW_DATA
	u8 pbuf[30];
	u8 i = 0; 
#endif

	if(!sw_i2c_start()){
		I2C_DEBUG("ReadBytes I2C bus busy...\r\n");
		return FALSE;
	}
	sw_i2c_send_byte(addr | I2C_WR);
	if(!sw_i2c_wait_ack()){
		sw_i2c_stop();
		I2C_DEBUG("ReadBytes I2C bus no ack!\r\n");
		return FALSE;
	}
	sw_i2c_send_byte(reg);
	if(!sw_i2c_wait_ack()){
		sw_i2c_stop();
		I2C_DEBUG("ReadBytes I2C bus no ack for reg!\r\n");
		return FALSE;
	}
	sw_i2c_start();
	sw_i2c_send_byte(addr | I2C_RD);
	if(!sw_i2c_wait_ack()){
		sw_i2c_stop();
		I2C_DEBUG("ReadBytes I2C bus no ack for address!\r\n");
		return FALSE;
	}
	while(j){
		*buf = sw_i2c_recv_byte();
#ifdef SHOW_DATA
		pbuf[len - j] = *buf;
#endif
		if(j == 1)
			sw_i2c_noack();
		else
			sw_i2c_ack();
		buf++;
		j--;
	}
	sw_i2c_stop();

#ifdef SHOW_DATA
	printf("[SWI2C_DEBUG] [ReadBytes]");
	for(i = 0; i < len; i++){
		printf("0x%x ", pbuf[i]);
	}
	printf("\r\n");
#endif

	return TRUE;
}

bool SwI2C_WriteBytes(uint8_t addr, uint8_t reg, uint8_t len, uint8_t * data)
{
    int i = 0;
	if(!sw_i2c_start()){
		I2C_DEBUG("WriteBuffer I2C bus busy...\r\n");
		return FALSE;
	}
    sw_i2c_send_byte(addr | I2C_WR);
    if (!sw_i2c_wait_ack()) {
		I2C_DEBUG("ReadBytes I2C bus no ack!\r\n");
        sw_i2c_stop();
        return FALSE;
    }
    sw_i2c_send_byte(reg);
    if (!sw_i2c_wait_ack()) {
		I2C_DEBUG("ReadBytes I2C bus no reg!\r\n");
        sw_i2c_stop();
        return FALSE;
    }
    for (i = 0; i < len; i++) {
        sw_i2c_send_byte(data[i]);
        if (!sw_i2c_wait_ack()) {
            sw_i2c_stop();
			I2C_DEBUG("ReadBytes I2C bus no data!\r\n");
            return FALSE;
        }
    }
    sw_i2c_stop();
	delay_ms(5);
#ifdef SHOW_DATA
	printf("[SWI2C_DEBUG] [WriteBuffer]");
	for(i = 0; i < len; i++){
		printf("0x%x ", data[i]);
	}
	printf("\r\n");
#endif

    return TRUE;
}

