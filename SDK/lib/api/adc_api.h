/*
 * adc_api.h
 *
 *  Created on: Dec 26, 2014
 *      Author: Iulian Gheorghiu
 */
/*#####################################################*/
#ifndef ADC_API_H_
#define ADC_API_H_
#include "stdbool.h"
#include "adc_def.h"
/*#####################################################*/
bool adc_init(Adc_t *Adc_s);

/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "adc_api.c"
#endif
/*#####################################################*/
#endif /* ADC_API_H_ */
/*#####################################################*/
