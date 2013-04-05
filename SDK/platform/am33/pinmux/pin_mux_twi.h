/*
 * pin_mux_twi.h
 *
 *  Created on: Aug 15, 2012
 *      Author: Iulian Gheorghiu morgoth2600@gmail.com
 */

#ifndef PIN_MUX_TWI_H_
#define PIN_MUX_TWI_H_
/**********************************************/
#include "../include/hw/soc_AM335x.h"
#include "../include/hw/hw_control_AM335x.h"
#include "../include/hw/hw_types.h"
/**********************************************/
#define Twi0_Sda_PinMux_C17	0

#define Twi0_Scl_PinMux_C16	0
/**********************************************/
#define Twi1_Sda_PinMux_H17	0
#define Twi1_Sda_PinMux_E18	1
#define Twi1_Sda_PinMux_D16	2
#define Twi1_Sda_PinMux_B16	3

#define Twi1_Scl_PinMux_J15	0
#define Twi1_Scl_PinMux_E17	1
#define Twi1_Scl_PinMux_D15	2
#define Twi1_Scl_PinMux_A16 3
/**********************************************/
#define Twi2_Sda_PinMux_E15	0
#define Twi2_Sda_PinMux_D18	1
#define Twi2_Sda_PinMux_A17	2

#define Twi2_Scl_PinMux_E16	0
#define Twi2_Scl_PinMux_D17	1
#define Twi2_Scl_PinMux_B17	2
/**********************************************/
unsigned int pin_mux_twi_sda_beaglebone(unsigned int TwiNr);
unsigned int pin_mux_twi_scl_beaglebone(unsigned int TwiNr);
/**********************************************/
unsigned int pin_mux_twi0_sda(unsigned int PinNr);
unsigned int pin_mux_twi0_scl(unsigned int PinNr);
/**********************************************/
unsigned int pin_mux_twi1_sda(unsigned int PinNr);
unsigned int pin_mux_twi1_scl(unsigned int PinNr);
/**********************************************/
unsigned int pin_mux_twi2_sda(unsigned int PinNr);
unsigned int pin_mux_twi2_scl(unsigned int PinNr);
/**********************************************/
unsigned int I2CPinMuxSetup(unsigned int instance);
/**********************************************/
#ifdef HEADER_INCLUDE_C_FILES
#include "pin_mux_twi.c"
#endif
/**********************************************/
#endif /* PIN_MUX_TWI_H_ */
