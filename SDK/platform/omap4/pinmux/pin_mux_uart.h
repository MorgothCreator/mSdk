/*
 * pin_mux_uart.h
 *
 *  Created on: Jan 15, 2013
 *      Author: XxXx
 */

#ifndef PIN_MUX_UART_H_
#define PIN_MUX_UART_H_

unsigned int pin_mux_uart_tx(unsigned int PinFunction);
unsigned int pin_mux_uart_rx(unsigned int PinFunction);

#ifdef HEADER_INCLUDE_C_FILES
#include "pin_mux_uart.c"
#endif

#endif /* PIN_MUX_UART_H_ */
