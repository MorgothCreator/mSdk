/*
 * uart_interface.h
 *
 *  Created on: Dec 2, 2012
 *      Author: XxXx
 */
/*#####################################################*/
#ifndef UART_INTERFACE_H_
#define UART_INTERFACE_H_
/*#####################################################*/
#include <stdbool.h>
#include "api/uart_def.h"
/*#####################################################*/
bool _uart_open(Uart_t* UartSettings);
bool _uart_close(Uart_t *UartSettings);
void _UARTBaudSetRate(Uart_t* UartSettings, unsigned long BaudRate);
void  _UARTCharPut(unsigned int BaseAddr, char b);
unsigned char _UARTCharGet(unsigned int BaseAddr);
void  _UARTCharPutNonBlocking(unsigned int BaseAddr, char b);
signed char _UARTCharGetNonBlocking(unsigned int BaseAddr);
char _UARTRxErrorGet(unsigned int BaseAddr);
/*#####################################################*/
#include "uart_interface.c"
#endif /* UART_INTERFACE_H_ */
/*#####################################################*/
