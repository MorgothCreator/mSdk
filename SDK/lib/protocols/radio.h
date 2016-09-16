/*
 * radio.h
 *
 *  Created on: Sep 1, 2016
 *      Author: John Smith
 */

#ifndef LIB_PROTOCOLS_RADIO_H_
#define LIB_PROTOCOLS_RADIO_H_

#include "device/nrf24l01.h"
typedef struct radio_s {
	enum {
		RADIO_TX_IDLE,
		RADIO_TX_BUSY,
	}RADIO_TX_STATE;
	enum {
		RADIO_RX_IDLE,
		RADIO_RX_BUSY,
	}RADIO_RX_STATE;
	unsigned int fixed_packet_len_len;
	unsigned long rtx_len;
	unsigned long buff_offset;
	unsigned char *buff;
	unsigned long address;
	unsigned long try_to_send_limit;
	void *device_driver_param_func;
	bool (*device_driver_enable_rx_func)(void *param, unsigned long address);
	unsigned char (*device_driver_packet_receive_func)(void *param, unsigned char *packet, unsigned short max_len, unsigned long timeout);
	signed int (*device_driver_packet_send_func)(void *param, unsigned char *packet, unsigned char len, unsigned long address);
	bool (*device_driver_init)(void *param);
	bool (*device_driver_deinit)(void *param);
}radio_t;

#endif /* LIB_PROTOCOLS_RADIO_H_ */
