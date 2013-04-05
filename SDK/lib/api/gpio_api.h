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
#include <stdlib.h>
#include "gpio_def.h"
#include "interface/gpio_interface.h"
/*#####################################################*/
void gpio_init(unsigned int GpioModuleNr);
new_gpio *gpio_assign(unsigned char PortNr, unsigned char PinNr, unsigned char Direction);
void gpio_free(new_gpio *gpio_struct);
bool gpio_out(new_gpio *gpio_struct, unsigned char State);
bool gpio_direction(new_gpio *gpio_struct, unsigned char Direction);
signed int gpio_in(new_gpio *gpio_struct);
bool gpio_up_dn_enable(new_gpio *gpio_struct, bool enable);
bool gpio_up_dn(new_gpio *gpio_struct, unsigned char value);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "gpio_api.c"
#endif
/*#####################################################*/
#endif /* GPIO_API_H_ */
