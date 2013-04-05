/*
 * clk_usart.h
 *
 *  Created on: Jan 14, 2013
 *      Author: XxXx
 */

#ifndef CLK_USART_H_
#define CLK_USART_H_
#include <stdbool.h>

bool UARTModuleClkConfig(unsigned char instanceNum);
bool UARTModuleClkUnConfig(unsigned char instanceNum);

#ifdef HEADER_INCLUDE_C_FILES
#include "clk_uart.c"
#endif

#endif /* CLK_USART_H_ */
