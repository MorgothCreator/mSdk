/*
 * gpio_interface.h
 *
 *  Created on: Feb 8, 2016
 *      Author: John Smith
 */

#ifndef CC13XX_INTERFACE_GPIO_INTERFACE_H_
#define CC13XX_INTERFACE_GPIO_INTERFACE_H_
/*#####################################################*/
#include "api/gpio_def.h"
/*#####################################################*/
void _gpio_init(unsigned int GpioModuleNr);
new_gpio *_gpio_assign(unsigned int PortNr, unsigned int PinNr, gpio_type_enum function, bool Multipin);
void _gpio_free(new_gpio *gpio_struct);
bool _gpio_out(new_gpio *gpio_struct, unsigned int State);
bool _gpio_direction(new_gpio *gpio_struct, unsigned int Direction);
signed int _gpio_in(new_gpio *gpio_struct);
bool _gpio_up_dn_enable(new_gpio *gpio_struct, bool value);
bool _gpio_up_dn(new_gpio *gpio_struct, unsigned char value);
bool _gpio_function_set(new_gpio *gpio_struct, gpio_type_enum function);
/*#####################################################*/
#endif /* CC13XX_INTERFACE_GPIO_INTERFACE_H_ */
