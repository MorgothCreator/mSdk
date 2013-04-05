/*
 * pin_mux_twi.h
 *
 *  Created on: Aug 15, 2012
 *      Author: Iulian Gheorghiu morgoth2600@gmail.com
 */

#ifndef PIN_MUX_TWI_H_
#define PIN_MUX_TWI_H_
/**********************************************/
unsigned int pin_mux_twi_scl(unsigned int PinFunction);
unsigned int pin_mux_twi_sda(unsigned int PinFunction);
/**********************************************/
#ifdef HEADER_INCLUDE_C_FILES
#include "pin_mux_twi.c"
#endif
/**********************************************/
#endif /* PIN_MUX_TWI_H_ */
