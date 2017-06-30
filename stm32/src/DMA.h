#ifndef __DMA_H
#define __DMA_H

#ifdef __cplusplus
 extern "C" {
#endif
	 
#include "stm32f10x.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_rcc.h"
	 
void DMA_Config(DMA_Channel_TypeDef *DMA_CHx, u32 paddr, u32 saddr, u16 trscnt);
void DMA_Enable(DMA_Channel_TypeDef *DMA_CHx);

#ifdef __cplusplus
}
#endif

#endif /* __DMA_H */
