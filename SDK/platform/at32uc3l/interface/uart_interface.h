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
#include "api/uart_def.h"
#include "driver/usart.h"
/*#####################################################*/
bool _uart_open(Uart_t* UartSettings);
bool _uart_close(Uart_t *UartSettings);
void _UARTBaudSetRate(unsigned int BaseAddr, unsigned long BaudRate);
void _UARTCharPut (unsigned int UartSettings, unsigned char byteTx);
void _UARTCharPutNonBlocking(unsigned int UartSettings,unsigned char byteTx);
signed int _UARTCharGet(unsigned int BaseAddr);
signed char _UARTCharGetNonBlocking(unsigned int BaseAddr);
unsigned int _UARTRxErrorGet(unsigned int BaseAddr);
/*#####################################################*/
#include "uart_interface.c"
/*#####################################################*/
#endif /* UART_INTERFACE_H_ */
/*#####################################################*/
