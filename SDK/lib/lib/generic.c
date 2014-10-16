/*
 *  lib/generic.c
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

#ifndef __Generic__
#define __Generic__
//#####################################################
#include "generic.h"
//#####################################################
signed int to_percentage(signed int MinValue, signed int MaxValue, signed int MaxPercentageValue, signed int Value)
{
	signed int ReturnedValue = 0;
	if (MaxValue < 65536) ReturnedValue = ((Value- MinValue) * 0x10000)/(((MaxValue - MinValue) * 0x10000)/MaxPercentageValue);
	else ReturnedValue = (Value- MinValue)/((MaxValue-MinValue)/MaxPercentageValue);
	if(ReturnedValue > MaxPercentageValue) ReturnedValue = MaxPercentageValue;
	else if(ReturnedValue < 0) ReturnedValue = 0;
	return ReturnedValue;
}
//#####################################################
double to_percentage_double(double MinValue, double MaxValue, double MaxPercentageValue, double Value)
{
	double ReturnedValue = 0;
	ReturnedValue = (Value- MinValue)/((MaxValue-MinValue)/MaxPercentageValue);
	if(ReturnedValue > MaxPercentageValue) ReturnedValue = MaxPercentageValue;
	else if(ReturnedValue < 0) ReturnedValue = 0;
	return ReturnedValue;
}
//#####################################################
ToPercentageWithDecimals_t to_percentage_with_decimals(signed int MinValue, signed int MaxValue, signed int MaxPercentageValue, signed int Value)
{
	ToPercentageWithDecimals_t Return;
	if (MaxValue < 65536)
	{
		Return.Value = (Value * 0x10000)/(((MaxValue - MinValue) * 0x10000)/MaxPercentageValue);
	}
	Return.Value = Value/((MaxValue-MinValue)/MaxPercentageValue);
	return Return;
}
//#####################################################
signed int percentage_to(signed int MinValue, signed int MaxValue, signed int MaxPercentageValue, signed int Value)
{
	if(MaxValue < 65536)
	{
		signed int result = ((Value*(MaxValue - MinValue))/MaxPercentageValue) + MinValue;
		if(result > MaxValue) result = MaxValue;
		else if(result < MinValue) result = MinValue;
		return result;
	}
	else //return ((Value*(MaxValue / 0x10000))/MaxPercentageValue) * 0x10000;
	{
		signed int result = (signed long long)((Value*(MaxValue - MinValue))/MaxPercentageValue) + MinValue;
		if(result > MaxValue) result = MaxValue;
		else if(result < MinValue) result = MinValue;
		return (signed int)result;
	}
}
//#####################################################
double percentage_to_double(double MinValue, double MaxValue, double MaxPercentageValue, double Value)
{
	double result = ((Value*(MaxValue - MinValue))/MaxPercentageValue) + MinValue;
	if(result > MaxValue) result = MaxValue;
	else if(result < MinValue) result = MinValue;
	return result;
}
//#####################################################
#endif




