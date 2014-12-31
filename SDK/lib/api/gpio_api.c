/*
 *  lib/api/gpio_api.c
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

#include <stdbool.h>
//#include <stdlib.h>
#include <stdio.h>
#include "gpio_def.h"
#include "gpio_api.h"
#include "interface/gpio_interface.h"
/*#####################################################*/
void gpio_init(gpio_port_enum GpioModuleNr)
{
	_gpio_init(GpioModuleNr);
}
/*#####################################################*/
//- GPIO_DIR_INPUT - to configure the pin as an input pin\n
//- GPIO_DIR_OUTPUT - to configure the pin as an output pin\n
new_gpio *gpio_assign(gpio_port_enum PortNr, unsigned char PinNr, unsigned char Direction, bool Multipin)
{
	return _gpio_assign(PortNr, PinNr, Direction, Multipin);
}
/*#####################################################*/
void gpio_free(new_gpio *gpio_struct)
{
	_gpio_free(gpio_struct);
}
/*#####################################################*/
bool gpio_out(new_gpio *gpio_struct, unsigned char State)
{
	return _gpio_out(gpio_struct, State);
}
/*#####################################################*/
//- GPIO_DIR_INPUT - to configure the pin as an input pin\n
//- GPIO_DIR_OUTPUT - to configure the pin as an output pin\n
bool gpio_direction(new_gpio *gpio_struct, unsigned char Direction)
{
	return _gpio_direction(gpio_struct, Direction);
}
/*#####################################################*/
signed int gpio_in(new_gpio *gpio_struct)
{
	return _gpio_in(gpio_struct);
}
/*#####################################################*/
bool gpio_up_dn_enable(new_gpio *gpio_struct, bool enable)
{
	return _gpio_up_dn_enable(gpio_struct, enable);
}
/*#####################################################*/
bool gpio_up_dn(new_gpio *gpio_struct, unsigned char value)
{
	return _gpio_up_dn(gpio_struct, value);
}
/*#####################################################*/
bool gpio_get_state(new_gpio *gpio_struct)
{
	return gpio_struct->LastState;
}
/*#####################################################*/
void gpio_idle(new_gpio *gpio_struct)
{
	if(gpio_in(gpio_struct) == 0 && gpio_struct->LastState == true) {
		gpio_struct->LastState = false;
		if(gpio_struct->event.on_state_changed) gpio_struct->event.on_state_changed(gpio_struct->event.on_state_changed_data, false);
	} else if(gpio_in(gpio_struct) != 0 && gpio_struct->LastState == true) {
		gpio_struct->LastState = true;
		if(gpio_struct->event.on_state_changed) gpio_struct->event.on_state_changed(gpio_struct->event.on_state_changed_data, true);
	}
}
/*#####################################################*/

