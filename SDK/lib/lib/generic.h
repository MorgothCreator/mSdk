/*
 *  lib/generic.h
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

#ifndef __Generic_H__
#define __Generic_H__
//#####################################################
typedef struct ToPercentageWithDecimals_Struct
{
	signed int Value;
	signed char Decimal;
}ToPercentageWithDecimals_t;
//#####################################################
signed int to_percentage(signed int MinValue, signed int MaxValue, signed int MaxPercentageValue, signed int Value);
double to_percentage_double(double MinValue, double MaxValue, double MaxPercentageValue, double Value);
ToPercentageWithDecimals_t to_percentage_with_decimals(signed int MinValue, signed int MaxValue, signed int MaxPercentageValue, signed int Value);
signed int percentage_to(signed int MinValue, signed int MaxValue, signed int MaxPercentageValue, signed int Value);
double percentage_to_double(double MinValue, double MaxValue, double MaxPercentageValue, double Value);
//#####################################################
#ifdef HEADER_INCLUDE_C_FILES
#include "generic.c"
#endif
//#####################################################
#endif
