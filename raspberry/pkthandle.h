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
	CHECK_CHECKSUM = 0,
	GEN_CHECKSUM
}pkt_type_checksum;

typedef enum{
	PKT_REQ_SET = 0x00,
	PKT_REQ_GET = 0x01,
	PKT_RSP_SET = 0x10,
	PKT_RSP_GET = 0x11
}pkt_type_e;

#define PACKET_START 		0xA5

/* command define */
#define CMD_GET_SYS_INFO		0x00
#define CMD_SET_SYS_STAT		0x10

#endif /* _PKTHANDLE_H */
