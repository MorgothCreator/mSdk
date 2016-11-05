/*
 * adc_interface.h
 *
 *  Created on: Dec 26, 2014
 *      Author: Iulian Gheorghiu
 */
/*#####################################################*/
#ifndef ADC_INTERFACE_H_
#define ADC_INTERFACE_H_
#include "stdbool.h"
#include "api/adc_def.h"
/*#####################################################*/
bool _adc_start_conversion(Adc_t *Adc_s);
bool _adc_init(Adc_t *Adc_s);


/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "adc_interface.c"
#endif
/*#####################################################*/
#endif /* ADC_INTERFACE_H_ */
/*#####################################################*/
