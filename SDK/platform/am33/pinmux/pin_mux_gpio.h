/*
 * pin_mux_gpio.h
 *
 *  Created on: Nov 20, 2012
 *      Author: XxXx
 */
/*#####################################################*/
#ifndef PIN_MUX_GPIO_H_
#define PIN_MUX_GPIO_H_
#include "../include/hw/soc_AM335x.h"
#include "../include/hw/hw_control_AM335x.h"
#include "../include/hw/hw_types.h"
/*#####################################################*/
void gpio_mux_setup(unsigned char PortNr, unsigned char PinNr, unsigned char Mode, unsigned char PUD_Enable, unsigned char PuTypeSel, unsigned char RxActive, unsigned char SlevCtrl);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "pin_mux_gpio.c"
#endif
/*#####################################################*/
#endif /* PIN_MUX_GPIO_H_ */
/*#####################################################*/
