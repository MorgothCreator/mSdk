/*
 * pin_mux_adc.h
 *
 *  Created on: Aug 15, 2012
 *      Author: Iulian Gheorghiu morgoth2600@gmail.com
 */
/**********************************************/
#ifndef PIN_MUX_ADC_H_
#define PIN_MUX_ADC_H_
/**********************************************/
#include "../include/hw/soc_AM335x.h"
#include "../include/hw/hw_control_AM335x.h"
#include "../include/hw/hw_types.h"
/**********************************************/
unsigned int pin_mux_adc_all(void);
/**********************************************/
#ifdef HEADER_INCLUDE_C_FILES
#include "pin_mux_adc.c"
#endif
/**********************************************/
#endif /* PIN_MUX_ADC_H_ */
