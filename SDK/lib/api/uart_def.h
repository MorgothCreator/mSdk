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

#include "gpio_def.h"
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
typedef struct Uart_s
{
	bool is_virtual;
	unsigned char TxPin;
	unsigned char RxPin;
	gpio_port_enum TxPort;
	gpio_port_enum RxPort;
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

	void (*putc)(struct Uart_s* UartSettings, unsigned char byteTx);
	unsigned char (*getc)(struct Uart_s* UartSettings);
	bool (*putc_no_blocking)(struct Uart_s* UartSettings, unsigned char byteTx);
	signed short (*getc_no_blocking)(struct Uart_s* UartSettings);
	/*unsigned int (*UARTRxGetError)(Uart_t* UartSettings);
	void (*UARTBaudRateSet)(Uart_t* UartSettings, unsigned long BaudRate);
	unsigned int (*UARTPuts)(Uart_t* UartSettings, char *pTxBuffer, int numBytesToWrite);
	unsigned int (*UARTGets)(Uart_t* UartSettings, char *pRxBuffer, int numBytesToRead);
	void (*UARTPutNum)(Uart_t* UartSettings, int value);
	void (*UARTPutHexNum)(Uart_t* UartSettings, unsigned long hexValue);
	long (*UARTGetNum)(Uart_t* UartSettings);
	unsigned long (*UARTGetHexNum)(Uart_t* UartSettings);
	unsigned int (*UARTwrite)(Uart_t* UartSettings, const char *pcBuf, unsigned int len);
	int (*UARTscanf)(Uart_t* UartSettings, const char *format, ...);
	Uart_t* (*UARTprintf)(Uart_t* UartSettings,const char *pcString, ...);*/
	bool (*open)(struct Uart_s *UartSettings);
	bool (*close)(struct Uart_s *UartSettings);
}Uart_t;

typedef struct uart_s
{
	bool (*open)(Uart_t *UartSettings);
	bool (*close)(Uart_t *UartSettings);
	void (*putc)(Uart_t* UartSettings, unsigned char byteTx);
	unsigned char (*getc)(Uart_t* UartSettings);
	bool (*putc_no_blocking)(Uart_t* UartSettings, unsigned char byteTx);
	signed short (*getc_no_blocking)(Uart_t* UartSettings);
	void (*set_baud)(Uart_t* UartSettings, unsigned long BaudRate);
	unsigned int (*puts)(Uart_t* UartSettings, char *pTxBuffer, int numBytesToWrite);
	unsigned int (*gets)(Uart_t* UartSettings, char *pRxBuffer, int numBytesToRead);
	unsigned int (*write)(Uart_t* UartSettings, const char *pcBuf, unsigned int len);
	int (*scanf)(Uart_t* UartSettings, const char *format, ...);
	Uart_t* (*printf)(Uart_t* UartSettings,const char *pcString, ...);
}uart_t;

/*#####################################################*/
#define new_uart Uart_t
#ifndef new_
#define new_(structure) (structure*)calloc(1, sizeof(structure));
#endif
#define free_uart(address) free(address);
/*#####################################################*/
#endif /* UART_DEF_H_ */
/*#####################################################*/
