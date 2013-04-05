/*
 * usart_interface.h
 *
 *  Created on: Jan 14, 2013
 *      Author: XxXx
 */

#ifndef USART_INTERFACE_H_
#define USART_INTERFACE_H_

#include <stdbool.h>
#include "api/uart_def.h"

#define UART_MODULE_INPUT_CLK                (48000000)

bool _uart_open(Uart_t* UartSettings);
bool _uart_close(Uart_t *UartSettings);
void _UARTBaudSetRate(unsigned int BaseAddr, unsigned long BaudRate);
void _UARTCharPut(unsigned int BaseAddr, unsigned char byteTx);
unsigned char _UARTCharGet(unsigned int BaseAddr);
void _UARTCharPutNonBlocking(unsigned int BaseAddr, unsigned char byteTx);
signed char _UARTCharGetNonBlocking(unsigned int BaseAddr);
unsigned int _UARTRxErrorGet(unsigned int BaseAddr);

#ifdef HEADER_INCLUDE_C_FILES
#include "uart_interface.c"
#endif

#endif /* USART_INTERFACE_H_ */
