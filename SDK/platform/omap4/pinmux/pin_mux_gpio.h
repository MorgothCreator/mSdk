/*
 * pin_mux_gpio.h
 *
 *  Created on: Jan 13, 2013
 *      Author: XxXx
 */

#ifndef PIN_MUX_GPIO_H_
#define PIN_MUX_GPIO_H_

bool gpio_mux_setup_complete(unsigned char PortNr, unsigned char PinNr, unsigned char Mode,
		signed char pulludenable, signed char pulltypeselect, signed char inputenable);
bool gpio_mux_setup(unsigned char PortNr, unsigned char PinNr, unsigned char Mode);

#ifdef HEADER_INCLUDE_C_FILES
#include "pin_mux_gpio.c"
#endif

#endif /* PIN_MUX_GPIO_H_ */
