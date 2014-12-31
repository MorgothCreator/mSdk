/*
 * gpio_interface.h
 *
 *  Created on: Mar 21, 2013
 *      Author: XxXx
 */
/*#####################################################*/
#ifndef GPIO_INTERFACE_H_
#define GPIO_INTERFACE_H_
/*#####################################################*/
#include <stdbool.h>
#include "api/gpio_def.h"
#include "driver/stm32f10x_gpio.h"
extern GPIO_TypeDef* GET_GPIO_PORT_ADDR[];
extern const unsigned int GET_PORT_CLK_ADDR[];
/*#####################################################*/
bool _gpio_init(gpio_port_enum GpioModuleNr);
new_gpio *_gpio_assign(gpio_port_enum PortNr, unsigned int PinNr, unsigned int Direction, bool Multipin);
void _gpio_free(new_gpio *gpio_struct);
bool _gpio_out(new_gpio *gpio_struct, unsigned int State);
bool _gpio_direction(new_gpio *gpio_struct, unsigned int Direction);
signed int _gpio_in(new_gpio *gpio_struct);
bool _gpio_up_dn_enable(new_gpio *gpio_struct, bool value);
bool _gpio_up_dn(new_gpio *gpio_struct, unsigned char value);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "gpio_interface.c"
#endif
/*#####################################################*/
#endif /* GPIO_INTERFACE_H_ */
/*#####################################################*/
