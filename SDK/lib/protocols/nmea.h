/*
 *  lib/protocols/nmea.h
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

#ifndef NMEA_H_
#define NMEA_H_

#ifndef nmea_max_satelite_nr
#define nmea_max_satelite_nr	12
#endif

typedef struct
{
	char Satelite_ID[3];
	char Elevation[3];
	char Azimuth[4];
	char SNR[3];
}NMEA_GPGSV;

#define NMEA_GGA		"$GPGGA"
#define NMEA_GLL		"$GPGLL"
#define NMEA_GSA		"$GPGSA"
#define NMEA_GSV		"$GPGSV"
#define NMEA_MSS		"$GPMSS"
#define NMEA_RMC		"$GPRMC"
#define NMEA_VTG		"$GPVTG"
#define NMEA_ZDA		"$GPZDA"
#define NMEA_PSRF140	"$PSRF140"
#define NMEA_PSRF150	"$PSRF150"
#define NMEA_PSRF151	"$PSRF151"
#define NMEA_PSRF152	"$PSRF152"
#define NMEA_PSRF154	"$PSRF154"
#define NMEA_PSRF155	"$PSRF155"

#define NMEA_MAX_STR_LEN	16

#define PositionFixIndicator_FixNotAvailableOrInvalid				0
#define PositionFixIndicator_GpsSpsMode_FixValid					1
#define PositionFixIndicator_DifferentialGps_SpsMode_FixValid		2
#define PositionFixIndicator_DeadReckoningMode_FixValid				6

typedef struct
{
	struct
	{
		char UTC_Time[NMEA_MAX_STR_LEN];
		char Latitude[NMEA_MAX_STR_LEN];
		char NS_Indicator[2];
		char Longitude[NMEA_MAX_STR_LEN];
		char EW_Indicator[2];
		char Position_Fix_Indicator[2];
		char Satellites_Used[3];
		char HDOP[NMEA_MAX_STR_LEN];
		char MSL_Altitude[NMEA_MAX_STR_LEN];
		char MSL_Altitude_Units[2];
		char Geoid_Separation[NMEA_MAX_STR_LEN];
		char Geoid_Separation_Units[2];
		char Age_of_Diff_Corr[NMEA_MAX_STR_LEN];
		char Diff_Ref_Station_ID[NMEA_MAX_STR_LEN];
		char Checksum[NMEA_MAX_STR_LEN];
	}GPGGA;
	struct
	{
		char Latitude[NMEA_MAX_STR_LEN];
		char NS_Indicator[2];
		char Longitude[NMEA_MAX_STR_LEN];
		char EW_Indicator[2];
		char UTC_Time[NMEA_MAX_STR_LEN];
		char Status[2];
		char Mode[2];
		char Checksum[NMEA_MAX_STR_LEN];
	}GPGLL;
}nmea_t;


#ifdef HEADER_INCLUDE_C_FILES
#include "nmea.c"
#endif

#endif /* NMEA_H_ */
