#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "serial.h"
#include "port.h"
#include "pkthandle.h"

#define VERSION "0.1"

/* settings */
struct port_options port_opts = {
	.device			= NULL,
	.baudRate		= SERIAL_BAUD_115200,
	.serial_mode		= "8e1",
	.bus_addr		= 0,
	.rx_frame_max		= 256,
	.tx_frame_max		= 256,
};

void show_help(char *name) {
	fprintf(stderr,
		"Usage: %s [-bmh] [tty_device]\n"
		"	-b rate		Baud rate (default 115200)\n"
		"	-m mode		Serial port mode (default 8e1)\n"
		"	-h		Show this help\n"
		"\n"
		"Examples:\n",
		name
	);
}


int parse_options(int argc, char *argv[])
{
	int c;

	while ((c = getopt(argc, argv, "b:m:h")) != -1) {
		switch(c) {
			case 'b':
				port_opts.baudRate = serial_get_baud(strtoul(optarg, NULL, 0));
				if (port_opts.baudRate == SERIAL_BAUD_INVALID) {
					serial_baud_t baudrate;
					fprintf(stderr,	"Invalid baud rate, valid options are:\n");
					for (baudrate = SERIAL_BAUD_1200; baudrate != SERIAL_BAUD_INVALID; ++baudrate)
						fprintf(stderr, " %d\n", serial_get_baud_int(baudrate));
					return 1;
				}
				break;
			case 'm':
				if (strlen(optarg) != 3
					|| serial_get_bits(optarg) == SERIAL_BITS_INVALID
					|| serial_get_parity(optarg) == SERIAL_PARITY_INVALID
					|| serial_get_stopbit(optarg) == SERIAL_STOPBIT_INVALID) {
					fprintf(stderr, "Invalid serial mode\n");
					return 1;
				}
				port_opts.serial_mode = optarg;
				break;
			case 'h':
				show_help(argv[0]);
				exit(0);
		}
	}
	for (c = optind; c < argc; ++c) {
		if (port_opts.device) {
			fprintf(stderr, "ERROR: Invalid parameter specified\n");
			show_help(argv[0]);
			return 1;
		}
		port_opts.device = argv[c];
	}

	if (port_opts.device == NULL) {
		fprintf(stderr, "ERROR: Device not specified\n");
		show_help(argv[0]);
		return 1;
	}

	return 0;
}

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
			//printk("c_sum: 0x%x r_sum: 0x%x\n", sum, pkt->checksum);
			return -1;
		}
	}else{
		return 2;
	}
}

int packet_send(struct port_interface *port, uart_pkt_s *pkt)
{
	int ret = 0;
	unsigned int len = pkt->len;

	pkt->start = PACKET_START;

	checksum_check(GEN_CHECKSUM, pkt);

	pkt->params[len] = pkt->checksum & 0xFF;
	pkt->params[len+1] = (pkt->checksum & 0xFF00)>>8;

	/* send packet */
	ret = port->write(port, (unsigned char *)pkt, 6+len);
	if (ret != PORT_ERR_OK) {
		fprintf(stderr, "Failed to send message\n");
		return -1;
	}

	return 0;
}

void show_menu(void)
{
	printf("\n========================================\n");
	printf(" rasp-stm32 " VERSION "\n");
	printf(" build @ %s %s\n", __DATE__,__TIME__);
	printf(" https://github.com/yeashen/raspberry-stm32\n\n");
	printf("========================================\n");
	printf("0. set system status\n");
	printf("1. get system information\n");
	printf("2. set motor direction\n");
	printf("3. get motor direction\n");
	printf("4. set motor speed\n");
	printf("5. get motor speed\n");
	printf("q. Quit test\n");
	printf("========================================\n");
	printf("\033[32m Select: \033[m");
}

int packet_handle(struct port_interface *port, uart_pkt_s *pkt, int recv_len)
{
	int ret = 0;
	int i;

	unsigned char rx_buf[256];
	/* write */
	packet_send(port, pkt);

	/* read */
	ret = port->read(port, rx_buf, (recv_len+6));
	if (ret != PORT_ERR_OK) {
		printf("read failed, ret=%d\n", ret);
		return ret;
	}

	for(i = 0; i < (recv_len+6); i++)
		printf("0x%x ", rx_buf[i]);
	printf("\n");

	return ret;
}

int main(int argc, char *argv[])
{
	char input = 0;
	int ret = 0;
	int tmp;
	FILE *diag = stdout;
	struct port_interface *port = NULL;
	uart_pkt_s pkts;
	int recv_len = 0;

	fprintf(diag, "--------------------------------------------\n\n");
	fprintf(diag, "rasp-stm32 " VERSION "\n\n");
	fprintf(diag, "build @ %s %s\n\n", __DATE__,__TIME__);
	fprintf(diag, "https://github.com/yeashen/raspberry-stm32\n\n");
	fprintf(diag, "--------------------------------------------\n\n");

	if (parse_options(argc, argv) != 0){
		ret = -1;
		return ret;
	}

	if (port_open(&port_opts, &port) != PORT_ERR_OK) {
		fprintf(stderr, "Failed to open port: %s\n", port_opts.device);
		ret = -1;
		return ret;
	}

	fprintf(diag, "Interface %s: %s\n", port->name, port->get_cfg_str(port));

	while(1)
	{
		show_menu();
		input = getchar();
		//getchar();
		if((input == '0') || (input == '1') || (input == '2') || (input == '3') || 
			(input == '4') || (input == '5') || (input == 'q'))
		{
			if (input == 'q')
			{
				port->close(port);
				break;
			}
		
			switch (input)
			{
				case '0':
					printf("input status [0/1]: ");
					scanf("%d", &tmp);
					getchar();
					pkts.type = PKT_REQ_SET;
					pkts.cmd = CMD_SET_SYS_STAT;
					pkts.params[0] = (tmp?1:0);
					pkts.len = 1;
					recv_len = pkts.len;
					break;
				case '1':
					getchar();
					pkts.type = PKT_REQ_GET;
					pkts.cmd = CMD_GET_SYS_INFO;
					pkts.len = 0;
					recv_len = 7;
					break;
				case '2':
					printf("input side [0/1]: ");
					scanf("%d", &tmp);
					getchar();
					pkts.params[0] = (tmp?1:0);
					printf("input dir [0/1/2/3]: ");
					scanf("%d", &tmp);
					getchar();
					pkts.params[1] = tmp;
					pkts.type = PKT_REQ_SET;
					pkts.cmd = CMD_SET_MOTOR_DIR;
					pkts.len = 2;
					recv_len = pkts.len;
					break;
				case '3':
					getchar();
					pkts.type = PKT_REQ_GET;
					pkts.cmd = CMD_GET_MOTOR_DIR;
					pkts.len = 0;
					recv_len = 2;
					break;
				case '4':
					printf("input left pwm[0-999]: ");
					scanf("%d", &tmp);
					getchar();
					pkts.params[0] = tmp;
					printf("input right [0-999]: ");
					scanf("%d", &tmp);
					getchar();
					pkts.params[1] = tmp;
					pkts.type = PKT_REQ_SET;
					pkts.cmd = CMD_SET_MOTOR_SPEED;
					pkts.len = 2;
					recv_len = pkts.len;
					break;
				case '5':
					getchar();
					pkts.type = PKT_REQ_GET;
					pkts.cmd = CMD_GET_MOTOR_SPEED;
					pkts.len = 0;
					recv_len = 2;
					break;
			}
			packet_handle(port, &pkts, recv_len);
		}
	}

	return ret;
}
