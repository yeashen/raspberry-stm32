/******************************************************************************
  File			: sw_i2c.c
  Description	: soft i2c driver
  Author		: Xiaoming Li
*******************************************************************************
  Modify List:
-------------------------------------------------------------------------------
  2014/8/16 21:00 PM	| Created
******************************************************************************/

#include "hw_i2c.h"
#include "delay.h"
#include "usart.h"

static bool is_I2c_Init = FALSE;

/*-------------------------------------------------------------------
 * FUNC : hw_i2c_init
 * DESC : hardward i2c init, configure pin
 * PARM : N/A
 * RET	: N/A
 *-----------------------------------------------------------------*/
void HwI2C_Init(void)
{
	I2C_InitTypeDef I2C_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	if(is_I2c_Init){
		I2C_DEBUG("I2C has already Init...\r\n");
		return;
	}else{
		I2C_DEBUG("I2C unInit...\r\n");
	}
	I2C_DEBUG("HW I2C Init...\r\n");
	
	/* 使能与 I2C1 有关的时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE ); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);  

	/* PB8-I2C1_SCL、PB9-I2C1_SDA*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD; 
	GPIO_Init(GPIOB, &GPIO_InitStructure); 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE);

	/* Initialize the I2C1 according to the I2C_InitStructure members */ 
	 
	  /* I2C 配置 */
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C ; 
	//I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2; 
	//I2C_InitStructure.I2C_OwnAddress1 = SlaveAddress; 
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable; 
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; 
	I2C_InitStructure.I2C_ClockSpeed = 50000; 


	/* I2C1 初始化 */
	I2C_Init(I2C1, &I2C_InitStructure);	   
	
	/* 使能 I2C1 */
	I2C_Cmd(I2C1, ENABLE); 
	/*允许应答模式*/
	I2C_AcknowledgeConfig(I2C1, ENABLE);
}


/*-------------------------------------------------------------------
 * FUNC : hw_i2c_write_byte
 * DESC : write one byte to I2C Bus
 * PARM : addr: decive I2C address
 *		  reg : the register written
 *		  data: the data written
 * RET	: N/A
 *-----------------------------------------------------------------*/
bool HwI2C_WriteByte(uint8_t addr, uint8_t reg, uint8_t data)
{

	I2C_GenerateSTART(I2C1, ENABLE);
	
	I2C_DEBUG("[WR] I2C_GenerateSTART\r\n");
	
	I2C_DEBUG("[WR] wait master select...\r\n");

	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	
	I2C_DEBUG("[WR] send address\r\n");

	I2C_Send7bitAddress(I2C1, addr, I2C_Direction_Transmitter);
	
	I2C_DEBUG("[WR] wait master transfer select...\r\n");

	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	
	I2C_DEBUG("[WR] send reg\r\n");

	I2C_SendData(I2C1, reg);
	
	I2C_DEBUG("[WR] wait reg send ok?\r\n");

	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	I2C_DEBUG("[WR] send data\r\n");

	I2C_SendData(I2C1, data);
	
	I2C_DEBUG("[WR] wait data send ok?\r\n");

	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	I2C_DEBUG("[WR] I2C_GenerateSTOP\r\n");

	I2C_GenerateSTOP(I2C1, ENABLE);
	
	I2C_DEBUG("[WR] I2C write OK!\r\n");

	return TRUE;

}

/*-------------------------------------------------------------------
 * FUNC : HwI2C_ReadByte
 * DESC : read one byte from I2C Bus
 * PARM : addr: decive I2C address
 *		  reg : the register written
 *		  data: the data written
 * RET	: N/A
 *-----------------------------------------------------------------*/
uint8_t HwI2C_ReadByte(uint8_t addr, uint8_t reg)
{
	uint8_t data;
	
	I2C_DEBUG("[RD] I2C_wait busy?\r\n");

	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
	
	I2C_DEBUG("[RD] I2C_GenerateSTART\r\n");

	I2C_GenerateSTART(I2C1, ENABLE);//起始信号
	
	I2C_DEBUG("[RD] wait master select...\r\n");

	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	
	I2C_DEBUG("[RD] send address\r\n");

	I2C_Send7bitAddress(I2C1, addr, I2C_Direction_Transmitter);//发送设备地址+写信号
	
	I2C_DEBUG("[RD] wait master transfer select...\r\n");

	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));//

	I2C_Cmd(I2C1, ENABLE);
	
	I2C_DEBUG("[RD] send reg\r\n");

	I2C_SendData(I2C1, reg);//发送存储单元地址，从0开始
	
	I2C_DEBUG("[RD] wait reg send ok?\r\n");

	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	I2C_DEBUG("[RD] I2C_GenerateSTART\r\n");

	I2C_GenerateSTART(I2C1, ENABLE);//起始信号
	
	I2C_DEBUG("[RD] wait master select...\r\n");

	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	
	I2C_DEBUG("[RD] send address\r\n");

	I2C_Send7bitAddress(I2C1, addr, I2C_Direction_Receiver);//发送设备地址+读信号
	
	I2C_DEBUG("[RD] wait master receive select...\r\n");

	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	
	I2C_AcknowledgeConfig(I2C1, DISABLE);
	
	I2C_DEBUG("[WR] I2C_GenerateSTOP\r\n");

	I2C_GenerateSTOP(I2C1, ENABLE);
	
	I2C_DEBUG("[WR] I2C wait receive ok?\r\n");

	while(!(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)));
	
	I2C_DEBUG("[WR] I2C read OK!\r\n");

	data = I2C_ReceiveData(I2C1);//读出寄存器数据

	return data;

}

bool HwI2C_ReadBytes(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
	I2C_DEBUG("[RD] I2C_wait busy?\r\n");
	
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
	
	I2C_DEBUG("[RD] I2C_GenerateSTART\r\n");

	I2C_GenerateSTART(I2C1, ENABLE);//起始信号
	
	I2C_DEBUG("[RD] wait master select...\r\n");

	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	
	I2C_DEBUG("[RD] send address\r\n");

	addr = 0xD0;

	I2C_Send7bitAddress(I2C1, addr, I2C_Direction_Transmitter);//发送设备地址+写信号
	
	I2C_DEBUG("[RD] wait master transfer select...\r\n");

	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));//

	I2C_Cmd(I2C1, ENABLE);
	
	I2C_DEBUG("[RD] send reg\r\n");

	I2C_SendData(I2C1, reg);//发送存储单元地址，从0开始
	
	I2C_DEBUG("[RD] wait master transfer select...\r\n");

	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	I2C_GenerateSTART(I2C1, ENABLE);//起始信号
	
	I2C_DEBUG("[RD] I2C_GenerateSTART\r\n");

	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	
	I2C_DEBUG("[RD] send address\r\n");

	I2C_Send7bitAddress(I2C1, addr, I2C_Direction_Receiver);//发送设备地址+读信号
	
	I2C_DEBUG("[RD] wait master receive select...\r\n");

	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

	I2C_AcknowledgeConfig(I2C1, DISABLE);
	
	I2C_GenerateSTOP(I2C1, ENABLE);
	
	I2C_DEBUG("[RD] start read datas...\r\n");

	while(len){
		while(!(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)));
		I2C_DEBUG("[RD] ready recieve ok!\r\n");
		*buf = I2C_ReceiveData(I2C1);//读出寄存器数据
		if(len == 1)
			I2C_AcknowledgeConfig(I2C1, DISABLE);
		else
			I2C_AcknowledgeConfig(I2C1, ENABLE);
		len--;
		buf++;
	}

	I2C_DEBUG("[WR] I2C_GenerateSTOP\r\n");
	
	I2C_GenerateSTOP(I2C1, ENABLE);
	
	I2C_DEBUG("[WR] I2C reads OK!\r\n");

	return TRUE;
}

