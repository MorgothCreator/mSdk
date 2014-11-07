/*
 * aintc_tsc_adc.h
 *
 *  Created on: Dec 11, 2012
 *      Author: XxXx
 */
/*#####################################################*/
#ifndef AINTC_TSC_ADC_H_
#define AINTC_TSC_ADC_H_
/*#####################################################*/
#define analog_touch_filter_level	32
/*#####################################################*/
void TouchIntEnable(void);
void TouchIntRegister(void);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "aintc_tsc_adc.c"
#endif
/*#####################################################*/
#endif /* AINTC_TSC_ADC_H_ */
/*#####################################################*/
