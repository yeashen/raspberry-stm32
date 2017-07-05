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

int main(int argc, char* argv[])
{
	int ret = 0;
	FILE *diag = stdout;
	struct port_interface *port = NULL;
	unsigned char rx_buf[256];
	int i = 0;
	int count = 10;
	uart_pkt_s pkts;

	fprintf(diag, "--------------------------------------------\n\n");
	fprintf(diag, "rasp-stm32 " VERSION "\n\n");
	fprintf(diag, "build @ %s %s\n\n", __DATE__,__TIME__);
	fprintf(diag, "https://github.com/yeashen/raspberry-stm32\n\n");
	fprintf(diag, "--------------------------------------------\n\n");

	if (parse_options(argc, argv) != 0){
		ret = -1;
		goto close;
	}

	if (port_open(&port_opts, &port) != PORT_ERR_OK) {
		fprintf(stderr, "Failed to open port: %s\n", port_opts.device);
		ret = -1;
		goto close;
	}

	fprintf(diag, "Interface %s: %s\n", port->name, port->get_cfg_str(port));

	pkts.type = PKT_REQ_SET;
	pkts.cmd = CMD_SET_SYS_STAT;

	while(count--){
		memset(rx_buf, 0, 256);
		pkts.params[0] = count;
		pkts.params[1] = count+1;
		pkts.len = 2;
		/* write */
		packet_send(port, &pkts);

		/* read */
		ret = port->read(port, rx_buf, (pkts.len+6));
		if (ret != PORT_ERR_OK) {
			printf("read failed, ret=%d\n", ret);
			ret = -1;
			goto close;
		}

		for(i = 0; i < (pkts.len+6); i++)
			printf("0x%x ", rx_buf[i]);
		printf("\n");
		usleep(1000000);
	}

close:
	if (port)
		port->close(port);

	return ret;
}
