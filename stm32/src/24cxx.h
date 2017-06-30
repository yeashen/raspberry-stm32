#ifndef __24CXX_H
#define __24CXX_H

#ifdef __cplusplus
 extern "C" {
#endif
	 
#include "stm32f10x.h" 
#include "sw_i2c.h"
	 
void at24cxx_init(void);
u8 at24cxx_readByte(u16 raddr);
void at24cxx_writeByte(u16 waddr, u8 wdata);
void at24cxx_writeData(u16 waddr, u8 wdata, u8 len);
u32 at24cxx_readData(u16 raddr, u8 len);
u8 at24cxx_check(void);
void at24cxx_read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead);
void at24cxx_write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite);
	 

#ifdef __cplusplus
}
#endif

#endif /* __24CXX_H */
