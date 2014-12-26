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
#include "include/armv7a/am335x/edma_event.h"
/*#####################################################*/
#define UART_MODULE_INPUT_CLK                (48000000)
/*#####################################################*/
bool _uart_open(Uart_t* UartSettings);
bool _uart_close(Uart_t *UartSettings);
void _UARTBaudSetRate(unsigned int BaseAddr, unsigned long BaudRate);
void _UARTBaudSetRate(unsigned int BaseAddr, unsigned long BaudRate);
void _UARTCharPut(unsigned int BaseAddr, unsigned char byteTx);
unsigned char _UARTCharGet(unsigned int BaseAddr);
bool _UARTCharPutNonBlocking(unsigned int BaseAddr, unsigned char byteTx);
signed short _UARTCharGetNonBlocking(unsigned int BaseAddr);
unsigned int _UARTRxErrorGet(unsigned int BaseAddr);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "uart_interface.c"
#endif

#endif /* UART_INTERFACE_H_ */
/*#####################################################*/
