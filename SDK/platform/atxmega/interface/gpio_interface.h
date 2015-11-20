/*
 * gpio_interface.h
 *
 * Created: 2/8/2013 12:33:03 AM
 *  Author: XxXx
 */ 
/*#####################################################*/
#ifndef GPIO_INTERFACE_H_
#define GPIO_INTERFACE_H_
/*#####################################################*/
#include <stdbool.h>
#include "api/gpio_def.h"
/*#####################################################*/
#define dir_in	GPIO_DIR_INPUT
#define dir_out	GPIO_DIR_OUTPUT
/*#####################################################*/
void _gpio_init(unsigned int GpioModuleNr);
new_gpio *_gpio_assign(unsigned int PortNr, unsigned int PinNr, unsigned int Direction, bool Multipin);
void _gpio_free(new_gpio *gpio_struct);
bool _gpio_out(new_gpio *gpio_struct, unsigned int State);
bool _gpio_direction(new_gpio *gpio_struct, unsigned int Direction);
signed int _gpio_in(new_gpio *gpio_struct);
bool _gpio_up_dn_enable(new_gpio *gpio_struct, bool value);
bool _gpio_up_dn(new_gpio *gpio_struct, unsigned char value);
bool _gpio_function_set(new_gpio *gpio_struct, gpio_type_enum function);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "gpio_interface.c"
#endif
/*#####################################################*/
#endif /* GPIO_INTERFACE_H_ */
/*#####################################################*/
