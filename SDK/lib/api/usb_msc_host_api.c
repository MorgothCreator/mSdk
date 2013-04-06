/*
 *  lib/api/usb_msc_host_api.c
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

#include "usb_msc_host_api.h"
#include "interface/usb_interface.h"
#include "api/gpio_api.h"

void usb_host_init(unsigned int instance, new_gpio* StatusLed)
{
	_usb_host_init(instance, StatusLed);
}

void usb_host_idle(unsigned int instance)
{
	_usb_host_idle(instance);
}

