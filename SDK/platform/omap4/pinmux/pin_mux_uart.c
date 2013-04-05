/*
 * pin_mux_uart.c
 *
 *  Created on: Jan 15, 2013
 *      Author: XxXx
 */

#include <stdbool.h>
#include "pin_mux_uart.h"
#include "driver/control_module.h"

unsigned int pin_mux_uart_tx(unsigned int PinFunction)
{
	cm_pin_set(PinFunction, false, 0, false);
	return 1;
}

unsigned int pin_mux_uart_rx(unsigned int PinFunction)
{
	cm_pin_set(PinFunction, true, 1, true);
	return 1;
}

