/******************** (C) COPYRIGHT 2007 STMicroelectronics ********************
* File Name          : stm32f10x_encoder.h
* Author             : IMS Systems Lab 
* Date First Issued  : 21/11/07
* Description        : This file contains the software implementation for the
*                      encoder position and speed reading.
********************************************************************************
* History:
* 21/11/07 v1.0
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ENCODER_H
#define __ENCODER_H

/* Private typedef -----------------------------------------------------------*/
typedef enum {Printf_TIMCNT = 0,Printf_THETA,Printf_W} PrintfType;

/* Includes ------------------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SPEED_SAMPLING_TIME  9     // (9+1)*500usec = 10msec

/* Exported functions ------------------------------------------------------- */
void Encoder_Init(void);
s16 ENC_Get_Electrical_Angle(void);
void ENC_Clear_Speed_Buffer(void);
void ENC_Calc_Average_Speed(void);
void Printf_encoder(PrintfType PrintfStatus);
s16 ENC_Get_Speed(void);
void TIM3_IRQHandler(void);

#endif  /*__ENCODER_H*/
/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
