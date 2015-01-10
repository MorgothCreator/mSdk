/*
 *  lib/api/twi_def.h
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

#ifndef TWI_DEF_H_
#define TWI_DEF_H_
#include "gpio_def.h"
#include "api/timer_api.h"
/*#####################################################*/
#define twi_timeout	500000
/*#####################################################*/
typedef struct
{
	unsigned char MasterSlaveAddr;
	unsigned char TwiNr;
	volatile unsigned char flag;
	volatile unsigned char error_flag;
	unsigned char Priority;
	bool UseInterrupt;
	bool UseDma;
	volatile unsigned char result;
	volatile unsigned char status;
	unsigned char SdaPin;
	unsigned char SclPin;
	gpio_port_enum SdaPort;
	gpio_port_enum SclPort;
	unsigned int BaseAddr;
	unsigned int RxBuffSize;
	unsigned int TxBuffSize;
	unsigned int tCount;
	unsigned int rCount;
	unsigned int numOfBytes;
	unsigned int bytesToWrite;
	unsigned int bytesToRead;
	unsigned long BaudRate;
	unsigned char* RxBuff;
	unsigned char* TxBuff;
	unsigned long BusyTimeOut;
}Twi_t;
/*#####################################################*/
#define new_twi Twi_t
#ifndef new_
#define new_(structure) (structure*)calloc(1,sizeof(structure));
#endif
#define free_twi(address) free(address);
/*#####################################################*/
#endif /* TWI_DEF_H_ */
/*#####################################################*/
