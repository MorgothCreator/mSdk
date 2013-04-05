/*
 * adc_interface.h
 *
 * Created: 2/24/2013 5:40:39 PM
 *  Author: XxXx
 */ 
//#####################################################
#ifndef ADC_INTERFACE_H_
#define ADC_INTERFACE_H_
//#####################################################
void adc_init(unsigned long chanels_mask, unsigned char mode, unsigned short period);
void adc_channels_enable_pio(unsigned long chanels_mask);
void adc_channels_enable(unsigned long chanels_mask);
void adc_channels_disable(unsigned long chanels_mask);
void adc_start_conversion(void);
unsigned short adc_get_latest_data(void);
unsigned char adc_get_latest_converted_channel(void);
void adc_enable_interrupts(unsigned long mask, void (*service_function)(void));
void adc_disable_interrupts(unsigned long mask);
//#####################################################
#include "adc_interface.c"
//#####################################################
#endif /* ADC_INTERFACE_H_ */
//#####################################################
