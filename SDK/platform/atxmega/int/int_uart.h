/*
 * int_uart.h
 *
 * Created: 2/8/2013 1:34:36 PM
 *  Author: XxXx
 */ 
/*#####################################################*/
#ifndef INT_UART_H_
#define INT_UART_H_
//#####################################################
#include <avr/io.h>
#include <avr/interrupt.h>
#include "board_properties.h"
#include "api/uart_def.h"
/*#####################################################*/
typedef struct
{
#ifdef use_usart0
	Uart_t* Uart0;
#endif
#ifdef use_usart1
	Uart_t* Uart1;
#endif
#ifdef use_usart2
	Uart_t* Uart2;
#endif
#ifdef use_usart3
	Uart_t* Uart3;
#endif
#ifdef use_usart4
	Uart_t* Uart4;
#endif
#ifdef use_usart5
	Uart_t* Uart5;
#endif
#ifdef use_usart6
	Uart_t* Uart6;
#endif
#ifdef use_usart7
	Uart_t* Uart7;
#endif
}Uart_Int_Service_t;
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "int_uart.c"
#endif
//#####################################################
#ifdef use_usart0
ISR(USARTC0_RXC_vect)
{
	usart_rx_int(Uart_Int_Service.Uart0);
}
ISR(USARTC0_DRE_vect)
{
	usart_tx_int(Uart_Int_Service.Uart0);
}
#endif
//-----------------------------------------------------
#ifdef use_usart1
ISR(USARTC1_RXC_vect)
{
	usart_rx_int(Uart_Int_Service.Uart1);
}
ISR(USARTC1_DRE_vect)
{
	usart_tx_int(Uart_Int_Service.Uart1);
}
#endif
//-----------------------------------------------------
#ifdef use_usart2
ISR(USARTD0_RXC_vect)
{
	usart_rx_int(Uart_Int_Service.Uart2);
}
ISR(USARTD0_DRE_vect)
{
	usart_tx_int(Uart_Int_Service.Uart2);
}
#endif
//-----------------------------------------------------
#ifdef use_usart3
ISR(USARTD1_RXC_vect)
{
	usart_rx_int(Uart_Int_Service.Uart3);
}
ISR(USARTD1_DRE_vect)
{
	usart_tx_int(Uart_Int_Service.Uart3);
}
#endif
//-----------------------------------------------------
#ifdef use_usart4
ISR(USARTE0_RXC_vect)
{
	usart_rx_int(Uart_Int_Service.Uart4);
}
ISR(USARTE0_DRE_vect)
{
	usart_tx_int(Uart_Int_Service.Uart4);
}
#endif
//-----------------------------------------------------
#ifdef use_usart5
ISR(USARTE1_RXC_vect)
{
	usart_rx_int(Uart_Int_Service.Uart5);
}
ISR(USARTE1_DRE_vect)
{
	usart_tx_int(Uart_Int_Service.Uart5);
}
#endif
//-----------------------------------------------------
#ifdef use_usart6
ISR(USARTF0_RXC_vect)
{
	usart_rx_int(Uart_Int_Service.Uart6);
}
ISR(USARTF0_DRE_vect)
{
	usart_tx_int(Uart_Int_Service.Uart6);
}
#endif
//-----------------------------------------------------
#ifdef use_usart7
ISR(USARTF1_RXC_vect)
{
	usart_rx_int(Uart_Int_Service.Uart7);
}
ISR(USARTF1_DRE_vect)
{
	usart_tx_int(Uart_Int_Service.Uart7);
}
#endif
//#####################################################
#endif /* INT_UART_H_ */
/*#####################################################*/
