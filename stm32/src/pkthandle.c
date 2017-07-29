/******************************************************************************

	Copyright (C), 2015-2025, SHARPNOW Co., Ltd.

 ******************************************************************************
  File Name     : pkthandle.c
  Version       : Initial Draft
  Author        : Juven
  Created       : 2016/08/23
  Last Modified : 
  Description   : 
  History       : 
  1.Date        : 2016/08/23
   Author       : Juven
   Modification : Created file

******************************************************************************/

#include "usart.h"
#include "pkthandle.h" 
#include "led.h"
#include "motor.h"
#include "pwm.h"

/* global value define */
static u8 left_dir = STOP;
static u8 right_dir = STOP;
static Motor_PWM pwm;

/*
  *  type: 0 --check packet checksum;		1 --generate packet checksum
  */
static int checksum_check(pkt_type_checksum type, uart_pkt_s *pkt)
{
	int i = 0;
	unsigned int sum = 0, param_s = 0;

	for(i = 0; i < (pkt->len); i++){
		param_s += pkt->params[i];
	}

	sum = pkt->start + pkt->type + pkt->cmd + pkt->len + param_s;
	sum &= 0xFFFF;

	if(type == GEN_CHECKSUM){
		pkt->checksum = sum;
		return 0;
	}else if(type == CHECK_CHECKSUM){
		if(sum == pkt->checksum){
			return 1;
		}else{
			PKG_DEBUG("c_sum: 0x%x r_sum: 0x%x\r\n", sum, pkt->checksum);
			return -1;
		}
	}else{
		return 2;
	}
}

void packet_send(uart_pkt_s *pkt)
{
	unsigned int len = pkt->len;

	checksum_check(GEN_CHECKSUM, pkt);

	pkt->params[len] = pkt->checksum & 0xFF;
	pkt->params[len+1] = (pkt->checksum & 0xFF00)>>8;
	
	/* send packet */
#if (DEBUG_UART == 1)
	uart2_send((u8 *)pkt, 6+len);
#elif (DEBUG_UART == 2)
	uart1_send((u8 *)pkt, 6+len);
#endif
}

int packet_handle(uart_pkt_s *pkt)
{
	int ret = 0, i;
	u8 reg;
	u8 param[UART_PKT_LEN];
	uart_pkt_s *pkts = pkt;

	if(NULL == pkt){
		ret = -1;
		PKG_DEBUG("packet is NULL\n");
		return ret;
	}

	ret = checksum_check(CHECK_CHECKSUM, pkts);
	if(ret < 0){
		PKG_DEBUG("receive packet checksum error!\r\n");
		return ret;
	}

	reg = pkts->cmd;
	if(pkts->type == PKT_REQ_SET){
		for(i = 0; i < pkt->len; i++){
			param[i] = pkts->params[i];
		}
		switch(reg){
			case CMD_SET_SYS_STAT:		/*system status*/
				led_set(param[0]);
				PKG_DEBUG("CMD_SET_SYS_STAT : param[0]=0x%x\r\n", param[0]);
				break;
			case CMD_SET_MOTOR_DIR:		/* motor direction */
				motor_direction_ctrl(param[0], param[1]);
				if(param[0] == LEFT){
					left_dir = param[1];
				}else{
					right_dir = param[1];
				}
				PKG_DEBUG("CMD_SET_MOTOR_DIR : param[0]=0x%x, param[1]=0x%x\r\n", 
						param[0], param[1]);
				break;
			case CMD_SET_MOTOR_SPEED:	/* motor speed */
				pwm.pwm1 = param[0]|(param[1]<<8);
				pwm.pwm2 = param[2]|(param[3]<<8);
				TIMER4_PWM_Refresh(&pwm);
				PKG_DEBUG("CMD_SET_MOTOR_SPEED : param[0]=0x%x, param[1]=0x%x, param[2]=0x%x, param[3]=0x%x\r\n",
						param[0], param[1], param[2], param[3]);
				break;
			default:
				PKG_DEBUG("invalid cmd!\r\n");
				break;
		}
		pkts->type = PKT_RSP_SET;
	}else if(pkts->type == PKT_REQ_GET){
		switch(reg){
			case CMD_GET_SYS_INFO:		/*system info*/
				/* fill packet */
				pkts->params[0] = 0x12;
				pkts->params[1] = 0x34;
				pkts->params[2] = 0x56;
				pkts->params[3] = 0x78;
				pkts->params[4] = 0x9a;
				pkts->params[5] = 0xbc;
				pkts->params[6] = 0xde;
				pkts->len = 7;
				PKG_DEBUG("CMD_GET_SYS_INFO\r\n");
				break;
			case CMD_GET_MOTOR_DIR:		/* motor direction */
				pkts->params[0] = left_dir;
				pkts->params[1] = right_dir;
				pkts->len = 2;
				PKG_DEBUG("CMD_GET_MOTOR_DIR : param[0]=0x%x, param[1]=0x%x\r\n", 
						param[0], param[1]);
				break;
			case CMD_GET_MOTOR_SPEED:	/* motor speed */
				pkts->params[0] = pwm.pwm1&0xFF;
				pkts->params[1] = (pwm.pwm1&0xFF00)>>8;
				pkts->params[2] = pwm.pwm2&0xFF;
				pkts->params[3] = (pwm.pwm2&0xFF00)>>8;
				pkts->len = 4;
				PKG_DEBUG("CMD_GET_MOTOR_SPEED : param[0]=0x%x, param[1]=0x%x, param[2]=0x%x, param[3]=0x%x\r\n",
						param[0], param[1], param[2], param[3]);
				break;
			default:
				PKG_DEBUG("invalid cmd!\r\n");
				break;
		}
		pkts->type = PKT_RSP_GET;
	}

	packet_send(pkt);

	//PKG_DEBUG("start: 0x%x, type: 0x%x, cmd: 0x%x, len: 0x%x, checksum: 0x%x\r\n", pkt->start,
	//	pkt->type, pkt->cmd, pkt->len, pkt->checksum);

	return ret;
}

