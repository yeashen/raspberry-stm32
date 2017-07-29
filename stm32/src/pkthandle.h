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

/* set motor direction,  param[0]: left/right, param[1]: direction*/
#define CMD_SET_MOTOR_DIR		0x02

/* get motor direction, no param */
#define CMD_GET_MOTOR_DIR		0x03

/* set motor speed, param[0]: pwm0 lsb, param[1]: pwm0 msb
 * param[2]: pwm1 lsb, param[3]: pwm1 msb
 * */
#define CMD_SET_MOTOR_SPEED		0x04

/* get motor speed, no param */
#define CMD_GET_MOTOR_SPEED		0x05

#define PKG_PDEBUG
#ifdef PKG_PDEBUG
#define PKG_DEBUG(string,args...)	printf("[PKG_DEBUG] ");	\
									printf(string, ##args)
#else
#define PKG_DEBUG(string,args...)
#endif

int packet_handle(uart_pkt_s *pkt);

#endif /* _PKTHANDLE_H */
