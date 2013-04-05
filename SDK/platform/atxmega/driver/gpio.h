/*
 * gpio.h
 *
 * Created: 2/8/2013 12:48:12 AM
 *  Author: XxXx
 */ 
/*#####################################################*/
#ifndef GPIO_H_
#define GPIO_H_
/*#####################################################*/
#include <avr/io.h>
#include <stdbool.h>
/*#####################################################*/
void GPIODirModeSet(unsigned int base_addr, unsigned char pin_nr, unsigned char direction);
void GPIOPinWrite(unsigned int base_addr, unsigned char pin_nr, unsigned char state);
unsigned char GPIOPinRead(unsigned int base_addr, unsigned char pin_nr);
bool GPIOUpDnEnable(unsigned int base_addr, unsigned char pin_nr, bool enable);
bool GPIOUpDn(unsigned int base_addr, unsigned char pin_nr, unsigned char value);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "gpio.c"
#endif
/*#####################################################*/
#endif /* GPIO_H_ */
/*#####################################################*/
