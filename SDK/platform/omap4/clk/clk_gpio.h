/*
 * clk_gpio.h
 *
 *  Created on: Jan 13, 2013
 *      Author: XxXx
 */

#ifndef CLK_GPIO_H_
#define CLK_GPIO_H_

#include <stdbool.h>

bool GPIOModuleClkConfig(unsigned int GpioModuleNr);

#ifdef HEADER_INCLUDE_C_FILES
#include "clk_gpio.c"
#endif

#endif /* CLK_GPIO_H_ */
