/*
 *  lib/api/twi_api.c
 *
 *  Copyright (C) 2013  Iulian Gheorghiu <morgoth.creator@gmail.com>
 *
 *  This file is part of Multiplatform SDK.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdbool.h>
#include "twi_api.h"
#include "twi_def.h"
#include "interface/twi_interface.h"

const twi_t twi = {
		_twi_open,
		_twi_close,
		_I2C_trx,
		_I2C_tx,
};
#if 0
/*#####################################################*/
bool SetupI2CTransmit(new_twi* TwiStruct, unsigned int TransmitBytes)
{
	return _SetupI2CTransmit(TwiStruct, TransmitBytes);
}
/*#####################################################*/
static bool send_receive(struct Twi_s* param, unsigned char addr, unsigned char *buff_send, unsigned int bytes_send, unsigned char *buff_receive, unsigned int bytes_receive)
{
	if(!param)
		return false;
	unsigned char tmp3 = param->MasterSlaveAddr;
	param->MasterSlaveAddr = addr;
	unsigned char *tmp1 = param->TxBuff;
	param->TxBuff = buff_send;
	bool response = false;
	if(bytes_receive)
	{
		unsigned char *tmp2 = param->RxBuff;
		param->RxBuff = buff_receive;
		response = _SetupI2CReception(param, bytes_send, bytes_receive);
		param->RxBuff = tmp2;
	}
	else
		response = _SetupI2CTransmit(param, bytes_send);
	param->TxBuff = tmp1;
	param->MasterSlaveAddr = tmp3;
	return response;
}
bool SetupI2CReception(new_twi* TwiStruct, unsigned int TransmitBytes, unsigned int ReceiveBytes)
{
	return _SetupI2CReception(TwiStruct, TransmitBytes, ReceiveBytes);
}
/*#####################################################*/
bool twi_open(new_twi* TwiStruct)
{
	if(!TwiStruct)
		return false;
	TwiStruct->close = twi_close;
	TwiStruct->send_receive = send_receive;
	return _twi_open(TwiStruct);
}
/*#####################################################*/
void twi_close(new_twi* TwiStruct)
{
	if(!TwiStruct)
		return;
	_twi_close(TwiStruct);
}
/*#####################################################*/
#endif
