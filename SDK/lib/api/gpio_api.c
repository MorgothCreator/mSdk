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

const gpio_t gpio = {
		_gpio_init,
		_gpio_assign,
		_gpio_free,
		_gpio_out,
		_gpio_direction,
		_gpio_in,
		_gpio_up_dn_enable,
		_gpio_up_dn,
		gpio_get_state,
		_gpio_function_set,
		gpio_idle,
};
/*#####################################################*/
bool gpio_get_state(new_gpio *gpio_struct)
{
	return gpio_struct->LastState;
}
/*#####################################################*/
void gpio_idle(new_gpio *gpio_struct)
{
	if(_gpio_in(gpio_struct) == 0 && gpio_struct->LastState == true) {
		gpio_struct->LastState = false;
		gpio_struct->event.state_dn = true;
		if(gpio_struct->event.on_state_changed) gpio_struct->event.on_state_changed(gpio_struct->event.on_state_changed_data, false);
	} else if(_gpio_in(gpio_struct) != 0 && gpio_struct->LastState == false) {
		gpio_struct->LastState = true;
		gpio_struct->event.state_up = true;
		if(gpio_struct->event.on_state_changed) gpio_struct->event.on_state_changed(gpio_struct->event.on_state_changed_data, true);
	}
}
/*#####################################################*/
