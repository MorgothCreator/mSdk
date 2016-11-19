/*
 *  lib/api/mcspi_api.c
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

#include <api/spi_api.h>
#include <stdbool.h>
#include "api/gpio_api.h"
//#include "aintc/aintc_mcspi.h"
//#include "clk/clk_mcspi.h"
#include "interface/mcspi_interface.h"


const spi_t spi = {
		_mcspi_open,
		_mcspi_close,
		_mcspi_assert,
		_mcspi_deassert,
		_mcspi_set_baud,
		_mcspi_SendByte,
		_mcspi_transfer,
		_mcspi_receive,
		_mcspi_transmit

};
#if 0
bool mcspi_open(Mcspi_t *McspiStruct)
{
	McspiStruct->Gpio_Miso = gpio_assign(McspiStruct->MisoPort, McspiStruct->MisoPin, GPIO_IN_PULL_UP, false);
	McspiStruct->Gpio_Mosi = gpio_assign(McspiStruct->MosiPort, McspiStruct->MosiPin, GPIO_OUT_PUSH_PULL, false);
	McspiStruct->Gpio_Sck = gpio_assign(McspiStruct->SckPort, McspiStruct->SckPin, GPIO_OUT_PUSH_PULL, false);
	if(McspiStruct->CsPort[0] != 0 && McspiStruct->CsPin[0] != 0)
	{
	    _gpio_init(McspiStruct->CsPort[0]);
		McspiStruct->Gpio_Cs[0] = gpio_assign(McspiStruct->CsPort[0], McspiStruct->CsPin[0], GPIO_OUT_PUSH_PULL, false);
		gpio_out(McspiStruct->Gpio_Cs[0], 1);
	}
	if(McspiStruct->CsPort[1] != 0 && McspiStruct->CsPin[1] != 0)
	{
	    _gpio_init(McspiStruct->CsPort[1]);
		McspiStruct->Gpio_Cs[1] = gpio_assign(McspiStruct->CsPort[1], McspiStruct->CsPin[1], GPIO_OUT_PUSH_PULL, false);
		gpio_out(McspiStruct->Gpio_Cs[1], 1);
	}
	if(McspiStruct->CsPort[2] != 0 && McspiStruct->CsPin[2] != 0)
	{
	    _gpio_init(McspiStruct->CsPort[2]);
		McspiStruct->Gpio_Cs[2] = gpio_assign(McspiStruct->CsPort[2], McspiStruct->CsPin[2], GPIO_OUT_PUSH_PULL, false);
		gpio_out(McspiStruct->Gpio_Cs[2], 1);
	}
	if(McspiStruct->CsPort[3] != 0 && McspiStruct->CsPin[3] != 0)
	{
	    _gpio_init(McspiStruct->CsPort[3]);
		McspiStruct->Gpio_Cs[3] = gpio_assign(McspiStruct->CsPort[3], McspiStruct->CsPin[3], GPIO_OUT_PUSH_PULL, false);
		gpio_out(McspiStruct->Gpio_Cs[3], 1);
	}
	return _mcspi_open(McspiStruct);
}

void mcspi_close(Mcspi_t *McspiStruct)
{
	_mcspi_close(McspiStruct);
	if(McspiStruct->Gpio_Miso)
		free(McspiStruct->Gpio_Miso);
	if(McspiStruct->Gpio_Mosi)
		free(McspiStruct->Gpio_Mosi);
	if(McspiStruct->Gpio_Sck)
		free(McspiStruct->Gpio_Sck);
	if(McspiStruct->Gpio_Cs[0])
	{
		gpio_out(McspiStruct->Gpio_Cs[0], 1);
		free(McspiStruct->Gpio_Cs[0]);
	}
	if(McspiStruct->Gpio_Cs[1])
	{
		gpio_out(McspiStruct->Gpio_Cs[1], 1);
		free(McspiStruct->Gpio_Cs[1]);
	}
	if(McspiStruct->Gpio_Cs[2])
	{
		gpio_out(McspiStruct->Gpio_Cs[2], 1);
		free(McspiStruct->Gpio_Cs[2]);
	}
	if(McspiStruct->Gpio_Cs[3])
	{
		gpio_out(McspiStruct->Gpio_Cs[3], 1);
		free(McspiStruct->Gpio_Cs[3]);
	}
}

void mcspi_assert(Mcspi_t *McspiStruct)
{
	_mcspi_assert(McspiStruct);
}

void mcspi_deassert(Mcspi_t *McspiStruct)
{
	_mcspi_deassert(McspiStruct);
}

/*bool mcspi_transfer(Mcspi_t *McspiStruct, unsigned int NumOfBytesSend, unsigned int NumOfBytesReceive)
{
	McspiStruct->numOfBytes = NumOfBytesSend + NumOfBytesReceive;
	_mcspi_transfer(McspiStruct);
	return true;
}*/

unsigned char mcspi_send_byte(Mcspi_t *McspiStruct, unsigned char byte)
{
	return _mcspi_SendByte(McspiStruct, byte);
}

bool mcspi_set_baud(Mcspi_t *McspiStruct, unsigned long baud)
{
	return _mcspi_set_baud(McspiStruct, baud);
}

bool mcspi_transfer(Mcspi_t *McspiStruct, unsigned char *buff_send, unsigned char *buff_receive, unsigned int size)
{
	return _mcspi_transfer(McspiStruct, buff_send, buff_receive, size);
}

bool mcspi_receive(Mcspi_t *McspiStruct, unsigned char *buff_receive, unsigned int bytes_receive)
{
	return _mcspi_receive(McspiStruct, buff_receive, bytes_receive);
}

bool mcspi_transmit(Mcspi_t *McspiStruct, unsigned char *buff_send, unsigned int bytes_send)
{
	return _mcspi_transmit(McspiStruct, buff_send, bytes_send);
}
#endif
