/*
 * sx150x.c
 *
 *  Created on: Nov 12, 2016
 *      Author: John Smith
 */

#include <stdbool.h>
#include "sx150x.h"
#include "board_init.h"
#include "api/twi_api.h"
#include "api/twi_def.h"
#include "api/uart_api.h"
#include "api/uart_def.h"
#include "api/timer_api.h"

bool sx150x_read_reg(SX150x_t *param, unsigned char addr, unsigned char *data)
{
	Twi_t* TWI = param->TWI;
	return TWI->send_receive(TWI, param->Twi_Addr, &addr, 1, data, 1);
}

bool sx150x_write_reg(SX150x_t *param, unsigned char addr, unsigned char data)
{
	Twi_t* TWI = param->TWI;
	unsigned char buff[2];
	buff[0] = addr;
	buff[1] = data;
	return TWI->send_receive(TWI, param->Twi_Addr, buff, 2, NULL, 0);
}

bool sx150x_probe(SX150x_t *param)
{
	unsigned char addr = SX1507_RegReset;
	unsigned char data;
	bool response = sx150x_read_reg(param, addr, &data);
	if(response == true && data == 0)
		return true;
	else
		return false;
}

bool sx150x_reset(SX150x_t *param)
{
	unsigned char addr = SX1507_RegReset;
	unsigned char data = 0x12;
	bool response1 = sx150x_write_reg(param, addr, data);
	data = 0x34;
	bool response2 = sx150x_write_reg(param, addr, data);
	if(response1 == true && response2 == true)
		return true;
	else
		return false;
}

bool sx150x_open(SX150x_t *param)
{
	param->probe = sx150x_probe;
	param->reset = sx150x_reset;
	param->read_reg = sx150x_read_reg;
	param->write_reg = sx150x_write_reg;
	if(sx150x_probe(param))
	{
		sx150x_reset(param);
		return true;
	}
	return false;
}

