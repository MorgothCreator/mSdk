/*
 *  lib/api/gpio_api.h
 *
 *  Copyright (C) 2013  Iulian Gheorghiu <morgoth.creator@gmail.com>
 *
 *  This file is part of Multiplatform SDK.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef GPIO_API_H_
#define GPIO_API_H_
/*#####################################################*/
#include <stdbool.h>
#include <stdio.h>
//#include <stdlib.h>
#include "gpio_def.h"
#include "interface/gpio_interface.h"
/*#####################################################*/
#define GPIO_FAST_SET_PIN(base_addr, pin)\
		_GPIO_FAST_SET_PIN(base_addr, pin)

#define GPIO_FAST_CLEAR_PIN(base_addr, pin)\
		_GPIO_FAST_CLEAR_PIN(base_addr, pin)

#define GPIO_FAST_WRITE_MULTI_PIN(base_addr, mask, pin_mask)\
		_GPIO_FAST_WRITE_MULTI_PIN(base_addr, mask, pin_mask)

#define GPIO_FAST_READ_MULTI_PIN(base_addr, mask)\
		_GPIO_FAST_READ_MULTI_PIN(base_addr, mask)
/*#####################################################*/
bool gpio_get_state(new_gpio *gpio_struct);
void gpio_idle(new_gpio *gpio_struct);
/*#####################################################*/
const gpio_t gpio;
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "gpio_api.c"
#endif
/*#####################################################*/
#endif /* GPIO_API_H_ */
