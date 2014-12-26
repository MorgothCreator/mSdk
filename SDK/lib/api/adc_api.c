/*
 *  lib/api/adc_api.c
 *
 *  Copyright (C) 2014  Iulian Gheorghiu <morgoth.creator@gmail.com>
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

#include "adc_api.h"
#include "adc_def.h"
#include "interface/adc_interface.h"
#include "stdbool.h"
/*#####################################################*/
bool adc_init(Adc_t *Adc_s)
{
	return _adc_init(Adc_s);
}
/*#####################################################*/
