/*
 * uart_interface.h
 *
 * Created: 2/8/2013 2:07:01 AM
 *  Author: XxXx
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
void _UARTBaudSetRate(unsigned int BaseAddr, unsigned long BaudRate);
void _UARTCharPut(unsigned int BaseAddr, unsigned char byteTx);
unsigned char _UARTCharGet(unsigned int BaseAddr);
bool _UARTCharPutNonBlocking(unsigned int BaseAddr, unsigned char byteTx);
signed char _UARTCharGetNonBlocking(unsigned int BaseAddr);
unsigned int _UARTRxErrorGet(unsigned int BaseAddr);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "uart_interface.c"
#endif
/*#####################################################*/
#endif /* UART_INTERFACE_H_ */
/*#####################################################*/
