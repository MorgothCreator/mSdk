/*
 * gpio_interface.h
 *
 *  Created on: Dec 2, 2012
 *      Author: XxXx
 */
/*#####################################################*/
#ifndef GPIO_INTERFACE_H_
#define GPIO_INTERFACE_H_
/*#####################################################*/
#include <stdbool.h>
#include "api/gpio_def.h"
#include "clk/clk_gpio.h"
#include "pinmux/pin_mux_gpio.h"
#include "include/gpio_v2.h"
/*#####################################################*/

#define dir_in	GPIO_DIR_INPUT
#define dir_out	GPIO_DIR_OUTPUT


void _gpio_init(unsigned int GpioModuleNr);
new_gpio *_gpio_assign(unsigned int PortNr, unsigned int PinNr, unsigned int Direction);
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
