/*
 * gpio_interface.h
 *
 * Created: 2/16/2013 9:59:43 PM
 *  Author: XxXx
 */ 
/*#####################################################*/
#ifndef GPIO_INTERFACE_H_
#define GPIO_INTERFACE_H_
/*#####################################################*/
void _gpio_init(unsigned int GpioModuleNr);
new_gpio *_gpio_assign(unsigned int PortNr, unsigned int PinNr, unsigned int Direction);
void _gpio_free(new_gpio *gpio_struct);
bool _gpio_out(new_gpio *gpio_struct, unsigned int State);
bool _gpio_direction(new_gpio *gpio_struct, unsigned int Direction);
signed int _gpio_in(new_gpio *gpio_struct);
bool _gpio_up_dn_enable(new_gpio *gpio_struct, bool value);
bool _gpio_up_dn(new_gpio *gpio_struct, unsigned char value);
/*#####################################################*/
#include "gpio_interface.c"
/*#####################################################*/
#endif /* GPIO_INTERFACE_H_ */
/*#####################################################*/
