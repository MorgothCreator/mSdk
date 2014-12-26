/*
 *  lib/api/uart_api.h
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

#ifndef UART_API_H_
#define UART_API_H_
/*#####################################################*/
#include <stdbool.h>
#include "uart_def.h"
/*#####################################################*/
/*#####################################################*/
void UARTPutc(Uart_t* UartSettings, unsigned char byteTx);
unsigned char UARTGetc(Uart_t* UartSettings);
bool UARTPutcNoBlocking(Uart_t* UartSettings, unsigned char byteTx);
signed short UARTGetcNoBlocking(Uart_t* UartSettings);
unsigned int UARTRxGetError(Uart_t* UartSettings);
void UARTBaudRateSet(Uart_t* UartSettings, unsigned long BaudRate);
unsigned int UARTPuts(Uart_t* UartSettings, char *pTxBuffer, int numBytesToWrite);
unsigned int UARTGets(Uart_t* UartSettings, char *pRxBuffer, int numBytesToRead);
void UARTPutNum(Uart_t* UartSettings, int value);
void UARTPutHexNum(Uart_t* UartSettings, unsigned long hexValue);
long UARTGetNum(Uart_t* UartSettings);
unsigned long UARTGetHexNum(Uart_t* UartSettings);
unsigned int UARTwrite(Uart_t* UartSettings, const char *pcBuf, unsigned int len);
Uart_t* UARTprintf(Uart_t* UartSettings,const char *pcString, ...);
bool uart_open(Uart_t *UartSettings);
bool uart_close(Uart_t *UartSettings);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "uart_api.c"
#endif
/*#####################################################*/
#endif /* UART_H_ */
/*#####################################################*/
