/*
 * uart_interface.c
 *
 * Created: 2/8/2013 2:06:44 AM
 *  Author: XxXx
 */ 
/*#####################################################*/
#include "uart_interface.h"
#include "driver/uart.h"
#include "int/int_uart.h"
/*#####################################################*/
bool _uart_open(Uart_t* UartSettings)
{
	return UartOpen(UartSettings);
}
/*#####################################################*/
bool _uart_close(Uart_t *UartSettings)
{
	UartClose(UartSettings);
	return true;
}
/*#####################################################*/
void _UARTBaudSetRate(unsigned int BaseAddr, unsigned long BaudRate)
{
	uart_set_baud_rate(BaseAddr, BaudRate);
}
/*#####################################################*/
void _UARTCharPut(unsigned int BaseAddr, unsigned char byteTx)
{
	UARTCharPut(BaseAddr, byteTx);
}
/*#####################################################*/
unsigned char _UARTCharGet(unsigned int BaseAddr)
{
	return (signed char)UARTCharGet(BaseAddr);
}
/*#####################################################*/
bool _UARTCharPutNonBlocking(unsigned int BaseAddr, unsigned char byteTx)
{
	return UARTCharPutNonBlocking(BaseAddr, byteTx);
}
/*#####################################################*/
signed short _UARTCharGetNonBlocking(unsigned int BaseAddr)
{
	return UARTCharGetNonBlocking(BaseAddr);
}
/*#####################################################*/
unsigned int _UARTRxErrorGet(unsigned int BaseAddr)
{
	return UARTRxErrorGet(BaseAddr);
}
/*#####################################################*/
