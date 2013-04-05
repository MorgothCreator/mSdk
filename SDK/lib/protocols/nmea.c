/*
 *  lib/protocols/nmea.c
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
#include <string.h>
#include "nmea.h"

NMEA_GPGSV nmea_gsv_data[nmea_max_satelite_nr];

bool nmea_read_data(unsigned int *start, unsigned int *end)
{
	*end = (unsigned int)strpbrk((char*)start, ",");
	if(*end)
	{
		return true;
	}
	return false;
}

unsigned char nmea_parse_response(char *gps_response)
{
	unsigned int start_ptr = (unsigned int)gps_response;
	unsigned int end_ptr = (unsigned int)gps_response;
	unsigned char SateliteCnt = 0;
	while((start_ptr = (unsigned int)strstr((char*)end_ptr, NMEA_GSV)) != 0)
	{
		end_ptr = start_ptr;
		start_ptr = end_ptr = start_ptr + 7;
		nmea_read_data(&start_ptr, &end_ptr);//Skip number of messages
		start_ptr = end_ptr + 1;
		nmea_read_data(&start_ptr, &end_ptr);//Skip message number
		start_ptr = end_ptr + 1;
		nmea_read_data(&start_ptr, &end_ptr);//Skip satelites in wiew
		start_ptr = end_ptr + 1;

		unsigned char Tmp = 0;
		for(Tmp = 0; Tmp < 4; Tmp++)
		{
			if(!strpbrk((char*)end_ptr, "*")) break;
			memset(&nmea_gsv_data[SateliteCnt], 0, sizeof(NMEA_GPGSV));

			nmea_read_data(&start_ptr, &end_ptr);
			start_ptr = end_ptr + 1;
			if((end_ptr - start_ptr) != 0) memcpy(nmea_gsv_data[SateliteCnt].Satelite_ID, (char*)start_ptr, end_ptr - start_ptr);

			nmea_read_data(&start_ptr, &end_ptr);
			start_ptr = end_ptr + 1;
			if((end_ptr - start_ptr) != 0) memcpy(nmea_gsv_data[SateliteCnt].Elevation, (char*)start_ptr, end_ptr - start_ptr);

			nmea_read_data(&start_ptr, &end_ptr);
			start_ptr = end_ptr + 1;
			if((end_ptr - start_ptr) != 0) memcpy(nmea_gsv_data[SateliteCnt].Azimuth, (char*)start_ptr, end_ptr - start_ptr);

			nmea_read_data(&start_ptr, &end_ptr);
			start_ptr = end_ptr + 1;
			if((end_ptr - start_ptr) != 0) memcpy(nmea_gsv_data[SateliteCnt].SNR, (char*)start_ptr, end_ptr - start_ptr);
			SateliteCnt++;
		}
	}
	return SateliteCnt;
}

