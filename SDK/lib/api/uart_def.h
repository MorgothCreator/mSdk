/*
 *  lib/api/uart_def.h
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

#ifndef UART_DEF_H_
#define UART_DEF_H_
/*#####################################################*/
typedef enum 
{
	UsartCom_Mode_Asynchronus = 0,
	UsartCom_Mode_Synchronus,
	UsartCom_Mode_IRCom,
	UsartCom_Mode_Spi
}UsartCom_Mode_enum;
//-----------------------------------------------------
typedef enum 
{
	UsartCom_Rx_Buff_OVF = 0,
	UsartCom_Rx_Pocket
}UsartCom_Status_enum;
/*#####################################################*/
typedef struct
{
	unsigned int TxPin;
	unsigned int RxPin;
	unsigned char UartNr;
	signed char Priority;
	unsigned char WithInterrupt;
	unsigned char UseDma;
	unsigned char FlagTxComplete;
	unsigned char Mode;
	unsigned char CS_PinMask;
	unsigned char Status;
	unsigned short txFifoTrigLevel;
	unsigned short rxFifoTrigLevel;
	unsigned int BaseAddr;
	unsigned int RxBuffSize;
	unsigned int TxBuffSize;
	unsigned int tCount;
	unsigned int rCount;
	unsigned int numOfBytes;
	unsigned int NrOfPocketsInBuff;
	unsigned long BaudRate;
	void* CS_Port;
	void *SelfFile;
	unsigned char* RxBuff;
	unsigned char* TxBuff;
	void *udata;
	void(*OnTxComplete)(void *);
	void(*OnRxPocketReceived)(void *);// Function to call when pointed item was changed.
}Uart_t;
/*#####################################################*/
#define new_uart Uart_t
#ifndef new_
#define new_(structure) (structure*)calloc(1, sizeof(structure));
#endif
#define free_uart(address) free(address);
/*#####################################################*/
#endif /* UART_DEF_H_ */
/*#####################################################*/
