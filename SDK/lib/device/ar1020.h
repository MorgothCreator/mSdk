/*
 *  lib/device/ar1020.h
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

#ifndef AR1020_H_
#define AR1020_H_

//#####################################################
//#include "../general/unions.h"
#include "../api/lcd_def.h"
#include "../api/touchscreen_def.h"
//#include "Util/Generic.h"
//#####################################################
#define AR1020_TWI_DeviceAddr					(0x4D)
#define AR1020_TWI_HeaderValue					85//(0x55)
//-----------------------------------------------------
#define AR1020_TWI_Address_Length					(0)
//-----------------------------------------------------
#define AR1020_Eep_Page_Size					(0x08)
#define AR1020_Eep_Start						(0x00)
#define AR1020_Eep_Size							(0x80)
#define AR1020_Eep_End							(0x80)
//#####################################################
typedef enum
{
	AR1020_TouchThreshold	= 0x02,
	AR1020_SensitivityFilter,
	AR1020_SamplingFast,
	AR1020_SamplingSlow,
	AR1020_AccuracyFilterFast,
	AR1020_AccuracyFilterSlow,
	AR1020_SpeedThreshold,
	AR1020_SleepDelay,
	AR1020_PenUpDelay,
	AR1020_TouchMode,
	AR1020_TouchOptions,
	AR1020_CalibrationInset,
	AR1020_PenStateReportDelay,
	AR1020_TouchReportDelay
}AR1020_enum;
//-----------------------------------------------------
//Touch Threshold
typedef enum
{
	AR1020_TouchThreshold_Min = 0,
	AR1020_TouchThreshold_Max = 255
}AR1020_TouchThreshold_enum;
//-----------------------------------------------------
//Sensitivity Filter
typedef enum
{
	AR1020_SensitivityFilter_Min = 0,
	AR1020_SensitivityFilter_Max = 255
}AR1020_SensitivityFilter_enum;
//-----------------------------------------------------
//Sampling Fast
typedef enum
{
	AR1020_SamplingFast_1 = 1,
	AR1020_SamplingFast_2 = 2,
	AR1020_SamplingFast_4 = 4,
	AR1020_SamplingFast_8 = 8,
	AR1020_SamplingFast_16 = 16,
	AR1020_SamplingFast_32 = 32,
	AR1020_SamplingFast_64 = 64,
	AR1020_SamplingFast_128 = 128
}AR1020_SamplingFast_enum;
//-----------------------------------------------------
//Sampling Slow
typedef enum
{
	AR1020_SamplingSlow_1 = 1,
	AR1020_SamplingSlow_2 = 2,
	AR1020_SamplingSlow_4 = 4,
	AR1020_SamplingSlow_8 = 8,
	AR1020_SamplingSlow_16 = 16,
	AR1020_SamplingSlow_32 = 32,
	AR1020_SamplingSlow_64 = 64,
	AR1020_SamplingSlow_128 = 128
}AR1020_SamplingSlow_enum;
//-----------------------------------------------------
//Accuracy Filter Fast
typedef enum
{
	AR1020_SamplingFast_Min = 1,
	AR1020_SamplingFast_Max = 8
}AR1020_AccuracyFilterFast_enum;
//-----------------------------------------------------
//Accuracy Filter Slow
typedef enum
{
	AR1020_SamplingSlow_Min = 1,
	AR1020_SamplingSlow_Max = 8
}AR1020_AccuracyFilterSlow_enum;
//-----------------------------------------------------
//Speed Threshold
typedef enum
{
	AR1020_SpeedThreshold_Min = 0,
	AR1020_SpeedThreshold_Max = 255
}AR1020_SpeedThreshold_enum;
//-----------------------------------------------------
//Sleep Delay
typedef enum
{
	AR1020_SleepDelay_Min = 0,
	AR1020_SleepDelay_Max = 255
}AR1020_SleepDelay_enum;
//-----------------------------------------------------
//Pen Up Delay
typedef enum
{
	AR1020_PenUpDelay_Min = 0,
	AR1020_PenUpDelay_Max = 255
}AR1020_PenUpDelay_enum;
//-----------------------------------------------------
//Touch Mode
//Touch Mode Pen down
typedef enum
{
	AR1020_TouchMode_PenDown_gp = 5,
	AR1020_TouchMode_PenDown_gm = 7<<5,
	AR1020_TouchMode_PenDown_NoTouch_gc = 0<<5,
	AR1020_TouchMode_PenDown_P0_gc = 1<<5,
	AR1020_TouchMode_PenDown_P1_gc = 2<<5,
	AR1020_TouchMode_PenDown_P10_gc = 3<<5,
	AR1020_TouchMode_PenDown_P010_gc = 4<<5,
	AR1020_TouchMode_PenDown_P01_gc = 5<<5
}AR1020_TouchMode_PenDown_enum;
//------------------------
//Touch Mode Pen movement
typedef enum
{
	AR1020_TouchMode_PenMove_gp = 3,
	AR1020_TouchMode_PenMove_gm = 3<<3,
	AR1020_TouchMode_PenMove_NoTouch_gc = 0<<3,
	AR1020_TouchMode_PenMove_P0_gc = 1<<3,
	AR1020_TouchMode_PenMove_P1_gc = 2<<3
}AR1020_TouchMode_PenMove_enum;
//------------------------
//Touch Mode Pen up
typedef enum
{
	AR1020_TouchMode_PenUp_gp = 0,
	AR1020_TouchMode_PenUp_gm = 7<<0,
	AR1020_TouchMode_PenUp_NoTouch_gc = 0<<0,
	AR1020_TouchMode_PenUp_P0_gc = 1<<0,
	AR1020_TouchMode_PenUp_P1_gc = 2<<0,
	AR1020_TouchMode_PenUp_P10_gc = 3<<0,
	AR1020_TouchMode_PenUp_P010_gc = 4<<0,
	AR1020_TouchMode_PenUp_P01_gc = 5<<0
}AR1020_TouchMode_PenUp_enum;
//-----------------------------------------------------
//Touch Options
typedef enum
{
	AR1020_TouchMode_48W_bp = 1,
	AR1020_TouchMode_48W_bm = 1<<1,
	AR1020_TouchMode_CCE_bp = 0,
	AR1020_TouchMode_CCE_bc = 1<<0
}AR1020_TouchOption_enum;
//-----------------------------------------------------
//Calibration Inset
typedef enum
{
	AR1020_CalibrationInset_Min = 0,
	AR1020_CalibrationInset_Max = 40
}AR1020_CalibrationInset_enum;
//-----------------------------------------------------
//Pen State Report Delay
typedef enum
{
	AR1020_PenStateReportDelay_Min = 0,
	AR1020_PenStateReportDelay_Max = 255
}AR1020_PenStateReportDelay_enum;
//-----------------------------------------------------
//Touch Report Delay
typedef enum
{
	AR1020_TouchReportDelay_Min = 0,
	AR1020_TouchReportDelay_Max = 255
}AR1020_TouchReportDelay_enum;
//#####################################################
//Responses
typedef enum
{
	AR1020_Response_Success = 0x00,
	AR1020_Response_UnrecognizedCommand = 0x01,
	AR1020_Response_UnrecognizedHeader = 0x03,
	AR1020_Response_CommandTimeout = 0x04,
	AR1020_Response_CalibrationCancel = 0xFC
}AR1020_Response_enum;
//#####################################################
//#####################################################
typedef enum
{
	Gfx_ar1020_Touch_MouseNoAction = 0x0,
	Gfx_ar1020_MouseUp = 0x01,
	Gfx_ar1020_MouseDn = 0x02,
	Gfx_ar1020_MouseMove = 0x03
}gfx_ar1020_mouse_state_t;
//#####################################################
typedef struct AR1020_s {
	unsigned short X_Coordonate;
	unsigned short Y_Coordonate;
	unsigned short X_ScreenSize;
	unsigned short Y_ScreenSize;
	unsigned char OldPenState;
	unsigned char Pen;
}AR1020_t;
//#####################################################
#define new_touch AR1020_t
#ifndef new_
#define new_(structure) (structure*)calloc(1,sizeof(structure));
#endif
#define free_touch(address) free(address);
//#####################################################
void ar1020_free(new_touchscreen* structure);
bool ar1020_init(new_touchscreen* structure, unsigned char Port, unsigned char Pin);
bool ar1020_enable_touch(new_touchscreen* structure);
bool ar1020_disable_touch(new_touchscreen* structure);
unsigned char ar1020_register_offset_read(new_touchscreen* structure);
bool ar1020_pen_up_delay_set(new_touchscreen* structure);
void ar1020_read_coordonate(new_touchscreen* structure);
bool ar1020_calibration_start(new_touchscreen* structure, tDisplay *pDisplay);
//#####################################################
#ifdef HEADER_INCLUDE_C_FILES
#include "ar1020.c"
#endif
//#####################################################
#endif /* AR1020_H_ */
//#####################################################
