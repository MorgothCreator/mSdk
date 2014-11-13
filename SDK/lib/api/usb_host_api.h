/*
 *  lib/api/usb_msc_host_api.h
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

#ifndef USB_MSC_HOST_API_H_
#define USB_MSC_HOST_API_H_

#include "gpio_api.h"
#include "lib/gfx/controls_definition.h"

void usb_msc_host_init(unsigned int instance, new_gpio* StatusLed);
void usb_msc_host_idle(unsigned int instance);
void usb_mouse_host_init(unsigned int instance);
void usb_mouse_host_idle(unsigned int instance, tControlCommandData *control_comand);

#ifdef HEADER_INCLUDE_C_FILES
#include "usb_msc_host_api.c"
#endif

#endif /* USB_MSC_HOST_API_H_ */
