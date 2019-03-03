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
#include "sys.h"

/* global value define */

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

	uart1_send((u8 *)pkt, 6+len);

}

int packet_handle(uart_pkt_s *pkt)
{
	int ret = 0, i;
	u8 reg;
	u8 param[UART_PKT_LEN];
	uart_pkt_s *pkts = pkt;
	int tmp;

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
				PKG_DEBUG("CMD_SET_SYS_STAT : param[0]=0x%x\r\n", param[0]);
				break;
			case CMD_SET_FORWARD:		/* go forward */
				Flag_Qian = 1;
				Flag_Hou = 0;
				line_velocity = param[0]|(param[1]<<8);
				PKG_DEBUG("CMD_SET_FORWARD : param[0]=0x%x param[1]=0x%x \r\n", param[0], param[1]);
				break;
			case CMD_SET_GOBACK:	/* go back */
				Flag_Qian = 0;
				Flag_Hou = 1;
				line_velocity = param[0]|(param[1]<<8);
				PKG_DEBUG("CMD_SET_GOBACK : param[0]=0x%x param[1]=0x%x \r\n", param[0], param[1]);
				break;
			case CMD_SET_TURN_LEFT:		/* turn left */
				Flag_Left = 1;
				Flag_Right= 0;
				turn_velocity = param[0]|(param[1]<<8);
				PKG_DEBUG("CMD_SET_TURN_LEFT : param[0]=0x%x param[1]=0x%x \r\n\r\n", param[0], param[1]);
				break;
			case CMD_SET_TURN_RIGHT:		/* turn right*/
				Flag_Left = 0;
				Flag_Right= 1;
				turn_velocity = param[0]|(param[1]<<8);
				PKG_DEBUG("CMD_SET_TURN_RIGHT : param[0]=0x%x param[1]=0x%x \r\n\r\n", param[0], param[1]);
				break;
			case CMD_SET_BRAKE:		/* brake */
				Flag_Qian = 0;
				Flag_Hou = 0;
				Flag_Left = 0;
				Flag_Right= 0;
				line_velocity = 0;
				turn_velocity = 0;
				PKG_DEBUG("CMD_SET_BRAKE\r\n", param[0]);
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
				pkts->params[0] = (Voltage)&0xFF;	//µç³ØµçÑ¹
				pkts->params[1] = (Voltage&0xFF00)>>8;
				pkts->params[2] = (Voltage&0xFF0000)>>16;
				pkts->params[3] = (Voltage&0xFF000000)>>24;
				
				tmp = Angle_Balance*100;	//½Ç¶È·Å´ó100±¶			
				pkts->params[4] = (tmp&0xFF);
				pkts->params[5] = (tmp&0xFF00)>>8;;
				pkts->params[6] = (tmp&0xFF0000)>>16;
				pkts->params[7] = (tmp&0xFF000000)>>24;
				
				pkts->params[8] = (Distance)&0xFF;	//³¬Éù²¨¾àÀë
				pkts->params[9] = (Distance&0xFF00)>>8;
				pkts->params[10] = (Distance&0xFF0000)>>16;
				pkts->params[11] = (Distance&0xFF000000)>>24;

				pkts->params[12] = (Encoder_Left)&0xFF;	//×óµç»ú±àÂëÆ÷
				pkts->params[13] = (Encoder_Left&0xFF00)>>8;
				pkts->params[14] = (Encoder_Left&0xFF0000)>>16;
				pkts->params[15] = (Encoder_Left&0xFF000000)>>24;

				pkts->params[16] = (Encoder_Right)&0xFF;	//ÓÒµç»ú±àÂëÆ÷
				pkts->params[17] = (Encoder_Right&0xFF00)>>8;
				pkts->params[18] = (Encoder_Right&0xFF0000)>>16;
				pkts->params[19] = (Encoder_Right&0xFF000000)>>24;
				
				pkts->len = SYS_INFO_DATA_LEN;
				PKG_DEBUG("CMD_GET_SYS_INFO\r\n");
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

