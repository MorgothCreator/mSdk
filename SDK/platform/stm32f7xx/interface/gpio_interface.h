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
#include "driver/stm32f7xx_hal_gpio.h"
extern GPIO_TypeDef* GET_GPIO_PORT_ADDR[];
extern const unsigned int GET_PORT_CLK_ADDR[];
/*#####################################################*/
#define _GPIO_FAST_SET_PIN(base_addr, pin)\
		((GPIO_TypeDef*)base_addr)->ODR |= 1 << (pin)

#define _GPIO_FAST_CLEAR_PIN(base_addr, pin)\
		((GPIO_TypeDef*)base_addr)->ODR &= ~(1 << (pin))

#define _GPIO_FAST_WRITE_MULTI_PIN(base_addr, mask, pin_mask)\
		((GPIO_TypeDef*)base_addr)->ODR = (((GPIO_TypeDef*)base_addr)->ODR & ~mask) | ((pin_mask) & mask)

/*#####################################################*/
void _gpio_init(gpio_port_enum GpioModuleNr);
new_gpio *_gpio_assign(gpio_port_enum PortNr, unsigned char Pin, gpio_type_enum function, bool Multipin);
void _gpio_free(Gpio_t *gpio_struct);
bool _gpio_out(Gpio_t *gpio_struct, unsigned int State);
bool _gpio_direction(Gpio_t *gpio_struct, gpio_type_enum function);
signed int _gpio_in(Gpio_t *gpio_struct);
bool _gpio_up_dn_enable(Gpio_t *gpio_struct, bool value);
bool _gpio_up_dn(Gpio_t *gpio_struct, unsigned char value);
bool _gpio_function_set(Gpio_t *gpio_struct, gpio_type_enum function);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "gpio_interface.c"
#endif
/*#####################################################*/
#endif /* GPIO_INTERFACE_H_ */
/*#####################################################*/
