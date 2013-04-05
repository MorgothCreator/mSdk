/*
 * clk_uart.h
 *
 *  Created on: Dec 1, 2012
 *      Author: XxXx
 */
/*#####################################################*/
#ifndef CLK_UART_H_
#define CLK_UART_H_
/*#####################################################*/
void UARTModuleClkConfig(unsigned char instanceNum);
void UARTModuleClkUnConfig(unsigned char instanceNum);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "clk_uart.c"
#endif
/*#####################################################*/
#endif /* CLK_UART_H_ */
/*#####################################################*/
