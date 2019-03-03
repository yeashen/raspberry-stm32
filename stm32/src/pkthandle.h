/******************************************************************************

  Copyright (C), 2019-2029, DIY Co., Ltd.

 ******************************************************************************
  File Name     : pkthandle.h
  Version       : Initial Draft
  Author        : Juven
  Created       : 2019/2/28
  Last Modified :
  Description   : pakage hadle header
  Function List :
  History       :
  1.Date        : 2019/2/28
    Author      : Juven
    Modification: Created file

******************************************************************************/

#ifndef _PKTHANDLE_H
#define _PKTHANDLE_H

#define UART_PKT_LEN		(256)

typedef struct{
	unsigned char start; 	/* packet head */
	unsigned char type;		/* packet type */
	unsigned char cmd; 		/* command */
	unsigned char len;		/* param length */
	unsigned char params[UART_PKT_LEN];		/* params data */
	unsigned short checksum;	/* packet checksum */
}uart_pkt_s;

typedef enum{
	CHECK_CHECKSUM = 0, /* check package checksum */
	GEN_CHECKSUM,		/* genate package checksum */
}pkt_type_checksum;

typedef enum{
	PKT_REQ_SET = 0x00, /* host to device set request */
	PKT_REQ_GET = 0x01, /* host to device get request */
	PKT_RSP_SET = 0x10, /* device to host set reply */
	PKT_RSP_GET = 0x11, /* device to host get reply */
}pkt_type_e;

/* package start symble */
#define PACKET_START 		0xA5

/* command define */


/* set system status */
#define CMD_SET_SYS_STAT		0x00

/* get system device information */
#define CMD_GET_SYS_INFO		0x01

/* set go forward,  param[0]: speed low 8bit, param[1]: speed high 8bit*/
#define CMD_SET_FORWARD		0x02

/* set go back,  param[0]: speed low 8bit, param[1]: speed high 8bit*/
#define CMD_SET_GOBACK		0x03

/* set brake */
#define CMD_SET_BRAKE		0x04

/* turn left */
#define CMD_SET_TURN_LEFT 	0x05

/* turn right */
#define CMD_SET_TURN_RIGHT 	0x06

#define SYS_INFO_DATA_LEN	(20)

#define CMD_SPEED_DATA_LEN	(2)

//#define PKG_PDEBUG
#ifdef PKG_PDEBUG
#define PKG_DEBUG(string,args...)	printf("[PKG_DEBUG] ");	\
									printf(string, ##args)
#else
#define PKG_DEBUG(string,args...)
#endif

int packet_handle(uart_pkt_s *pkt);

#endif /* _PKTHANDLE_H */
