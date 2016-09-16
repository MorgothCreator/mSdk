/*
 * radio.c
 *
 *  Created on: Sep 1, 2016
 *      Author: John Smith
 */

#include "radio.h"

void radio_Delay (unsigned long a)
{
	volatile unsigned long b = a;
	do{

	}
	while (--b!=0);
}

bool radio_send_file(radio_t *param, unsigned char buff, unsigned long len, unsigned long addr, unsigned long try_to_send_limit)
{
	if(param->RADIO_TX_STATE == RADIO_TX_IDLE && param->RADIO_RX_STATE == RADIO_RX_IDLE)
	{
		unsigned char *buff = malloc(len);
		if(!buff)
			return -1;
		param->buff = buff;
		param->RADIO_TX_STATE = RADIO_TX_BUSY;
		param->buff_offset = 0;
		param->rtx_len = len;
		param->address = addr;
		param->try_to_send_limit = try_to_send_limit;
	}
	return false;
}

int radio_idle(radio_t *param)
{
	if(param->RADIO_TX_STATE == RADIO_TX_BUSY)
	{
		unsigned long try_to_send_limit = param->try_to_send_limit;
		while(--try_to_send_limit)
		{

			signed int bytes_send = param->device_driver_packet_send_func(param->device_driver_param_func, (unsigned char *)param->buff + param->buff_offset, param->rtx_len, param->address);
			if(bytes_send > 0)
			{
				volatile signed long timeout2 = 300;
				unsigned char response_packet[32];
				param->device_driver_enable_rx_func(param->device_driver_param_func, param->address);
				signed int rec_len = 0;
				while((rec_len = param->device_driver_packet_receive_func(param->device_driver_param_func, response_packet, param->fixed_packet_len_len, 0)) < 0 && --timeout2);
				if(timeout2 > 0 && rec_len > 0)
				{
					try_to_send_limit = param->try_to_send_limit;
					param->buff_offset += bytes_send;
					return bytes_send;
				}
			}
		}
		return -1;
	}
	else if(param->RADIO_RX_STATE == RADIO_RX_BUSY)
	{


	}
	return -2;
}


