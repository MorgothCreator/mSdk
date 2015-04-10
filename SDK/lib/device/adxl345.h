/*
 *  lib/string_lib.h
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

#ifndef ADXL345_H_
#define ADXL345_H_
/*#####################################################*/
#include <stdlib.h>
#include <stdbool.h>
#include "../api/twi_api.h"
/******************************************************************************
**              INTERNAL MACRO DEFINITIONS
******************************************************************************/
#define  GPIO_INSTANCE_PIN_NUMBER      		(16u)
/* I2C address of ADXL345 accelerometer*/
#define  ADXL345_SLAVE_ADDR_ALT_0       	(0x53)
#define  ADXL345_SLAVE_ADDR_ALT_1           (0x1D)

#define ADXL345_DevId						(0xE5)
/* Register 0x2A—TAP_AXES (Read/Write) */
#define ADXL345_TAP_AXES_TAP_Suppress		(1<<3)
#define ADXL345_TAP_AXES_TAP_X_enable		(1<<2)
#define ADXL345_TAP_AXES_TAP_Y_enable		(1<<1)
#define ADXL345_TAP_AXES_TAP_Z_enable		(1<<0)
/* Register 0x2B—ACT_TAP_STATUS (Read Only) */
#define ADXL345_ACT_TAP_STATUS_ACT_X_source	(1<<6)
#define ADXL345_ACT_TAP_STATUS_ACT_Y_source	(1<<5)
#define ADXL345_ACT_TAP_STATUS_ACT_Z_source	(1<<4)
#define ADXL345_ACT_TAP_STATUS_Asleep		(1<<3)
#define ADXL345_ACT_TAP_STATUS_TAP_X_source	(1<<2)
#define ADXL345_ACT_TAP_STATUS_TAP_Y_source	(1<<1)
#define ADXL345_ACT_TAP_STATUS_TAP_Z_source	(1<<0)
/* Register 0x2C—BW_RATE (Read/Write) */
#define ADXL345_BW_RATE_LOW_POWER			(1<<4)
#define ADXL345_BW_RATE_Rate_gm				(0x0F<<0)
#define ADXL345_BW_RATE_Rate_0_10Hz			(0x00)
#define ADXL345_BW_RATE_Rate_0_20Hz			(0x01)
#define ADXL345_BW_RATE_Rate_0_39Hz			(0x02)
#define ADXL345_BW_RATE_Rate_0_78Hz			(0x03)
#define ADXL345_BW_RATE_Rate_1_56Hz			(0x04)
#define ADXL345_BW_RATE_Rate_3_13Hz			(0x05)
#define ADXL345_BW_RATE_Rate_6_25Hz			(0x06)
#define ADXL345_BW_RATE_Rate_12_5Hz			(0x07)
#define ADXL345_BW_RATE_Rate_25Hz			(0x08)
#define ADXL345_BW_RATE_Rate_50Hz			(0x09)
#define ADXL345_BW_RATE_Rate_100Hz			(0x0A)
#define ADXL345_BW_RATE_Rate_200Hz			(0x0B)
#define ADXL345_BW_RATE_Rate_400Hz			(0x0C)
#define ADXL345_BW_RATE_Rate_800Hz			(0x0D)
#define ADXL345_BW_RATE_Rate_1600Hz			(0x0E)
#define ADXL345_BW_RATE_Rate_3200Hz			(0x0F)
/* Register 0x2D—POWER_CTL (Read/Write) */
#define ADXL345_POWER_CTL_Link				(1<<5)
#define ADXL345_POWER_CTL_AUTO_SLEEP		(1<<4)
#define ADXL345_POWER_CTL_Measure			(1<<3)
#define ADXL345_POWER_CTL_Sleep				(1<<2)
#define ADXL345_POWER_CTL_Wakeup_gm			(0x03<<0)
#define ADXL345_POWER_CTL_Wakeup_8Hz		(0x00<<0)
#define ADXL345_POWER_CTL_Wakeup_4Hz		(0x01<<0)
#define ADXL345_POWER_CTL_Wakeup_2Hz		(0x02<<0)
#define ADXL345_POWER_CTL_Wakeup_1Hz		(0x03<<0)
/* Register 0x2E—INT_ENABLE (Read/Write) */
#define ADXL345_INT_ENABLE_DATA_READY		(1<<7)
#define ADXL345_INT_ENABLE_SINGLE_TAP		(1<<6)
#define ADXL345_INT_ENABLE_DOUBLE_TAP		(1<<5)
#define ADXL345_INT_ENABLE_Activity			(1<<4)
#define ADXL345_INT_ENABLE_Inactivity		(1<<3)
#define ADXL345_INT_ENABLE_FREE_FALL		(1<<2)
#define ADXL345_INT_ENABLE_Watermark		(1<<1)
#define ADXL345_INT_ENABLE_Overrun			(1<<0)
/* Register 0x2F—INT_MAP (R/W) */
#define ADXL345_INT_MAP_DATA_READY			(1<<7)
#define ADXL345_INT_MAP_SINGLE_TAP			(1<<6)
#define ADXL345_INT_MAP_DOUBLE_TAP			(1<<5)
#define ADXL345_INT_MAP_Activity			(1<<4)
#define ADXL345_INT_MAP_Inactivity			(1<<3)
#define ADXL345_INT_MAP_FREE_FALL			(1<<2)
#define ADXL345_INT_MAP_Watermark			(1<<1)
#define ADXL345_INT_MAP_Overrun				(1<<0)
/* Register 0x30—INT_SOURCE (Read Only) */
#define ADXL345_INT_SOURCE_DATA_READY		(1<<7)
#define ADXL345_INT_SOURCE_SINGLE_TAP		(1<<6)
#define ADXL345_INT_SOURCE_DOUBLE_TAP		(1<<5)
#define ADXL345_INT_SOURCE_Activity			(1<<4)
#define ADXL345_INT_SOURCE_Inactivity		(1<<3)
#define ADXL345_INT_SOURCE_FREE_FALL		(1<<2)
#define ADXL345_INT_SOURCE_Watermark		(1<<1)
#define ADXL345_INT_SOURCE_Overrun			(1<<0)
/* Register 0x31—DATA_FORMAT (Read/Write) */
#define ADXL345_DATA_FORMAT_SELF_TEST		(1<<7)
#define ADXL345_DATA_FORMAT_SPI				(1<<6)
#define ADXL345_DATA_FORMAT_INT_INVERT		(1<<5)
#define ADXL345_DATA_FORMAT_FULL_RES		(1<<3)
#define ADXL345_DATA_FORMAT_Justify			(1<<2)
#define ADXL345_DATA_FORMAT_Range_gm		(0x03<<0)
#define ADXL345_DATA_FORMAT_Range_2g		(0x00<<0)
#define ADXL345_DATA_FORMAT_Range_4g		(0x01<<0)
#define ADXL345_DATA_FORMAT_Range_8g		(0x02<<0)
#define ADXL345_DATA_FORMAT_Range_16g		(0x03<<0)
/* Register 0x38—FIFO_CTL (Read/Write) */
#define ADXL345_FIFO_CTL_FIFO_MODE_gm		(0x03<<6)
#define ADXL345_FIFO_CTL_FIFO_MODE_Bypass	(0x00<<6)
#define ADXL345_FIFO_CTL_FIFO_MODE_FIFO		(0x01<<6)
#define ADXL345_FIFO_CTL_FIFO_MODE_Stream	(0x02<<6)
#define ADXL345_FIFO_CTL_FIFO_MODE_Trigger	(0x03<<6)
#define ADXL345_FIFO_CTL_Trigger			(1<<7)
#define ADXL345_FIFO_CTL_Samples_gm			(0x1F<<0)
/* Register 0x39—FIFO_STATUS (Read Only) */
#define ADXL345_FIFO_STATUS_FIFO_TRIG		(0x01<<7)
#define ADXL345_FIFO_STATUS_Entries_gm		(0x3F<<0)



#define  ADXL345_REG_DEVID         			(0x00)
#define  ADXL345_REG_THRESH_TAP         	(0x1D)
#define  ADXL345_REG_OFSX         			(0x1E)
#define  ADXL345_REG_OFSY         			(0x1F)
#define  ADXL345_REG_OFSZ         			(0x20)
#define  ADXL345_REG_DUR         			(0x21)
#define  ADXL345_REG_Latent         		(0x22)
#define  ADXL345_REG_Window         		(0x23)
#define  ADXL345_REG_THRESH_ACT         	(0x24)
#define  ADXL345_REG_THRESH_INACT         	(0x25)
#define  ADXL345_REG_TIME_INACT         	(0x26)
#define  ADXL345_REG_ACT_INACT_CTL         	(0x27)
#define  ADXL345_REG_THRESH_FF         		(0x28)
#define  ADXL345_REG_TIME_FF         		(0x29)
#define  ADXL345_REG_TAP_AXES         		(0x2A)
#define  ADXL345_REG_ACT_TAP_STATUS         (0x2B)
#define  ADXL345_REG_BW_RATE         		(0x2C)
#define  ADXL345_REG_POWER_CTL         		(0x2D)
#define  ADXL345_REG_INT_ENABLE         	(0x2E)
#define  ADXL345_REG_INT_MAP         		(0x2F)
#define  ADXL345_REG_INT_SOURCE         	(0x30)
#define  ADXL345_REG_DATA_FORMAT         	(0x31)
#define  ADXL345_REG_OUT_X_L_DATA         	(0x32)
#define  ADXL345_REG_OUT_X_H_DATA         	(0x33)
#define  ADXL345_REG_OUT_Y_L_DATA         	(0x34)
#define  ADXL345_REG_OUT_Y_H_DATA         	(0x35)
#define  ADXL345_REG_OUT_Z_L_DATA         	(0x36)
#define  ADXL345_REG_OUT_Z_H_DATA         	(0x37)
#define  ADXL345_REG_FIFO_CTL         		(0x38)
#define  ADXL345_REG_FIFO_STATUS         	(0x39)

#define  ENABLE_XYZ_AXIS              (0x07)
#define  OUTPUT_DAT_RATE              (0x08)
#define  NORMAL_POWER_MODE            (0x20)
#define  FULL_SCALE_2g                (0x00)
#define  BLOCK_DATA_UPDATE            (0x80)

#define  CONFIG_CNTL_REG1             (ENABLE_XYZ_AXIS       |  \
                                       OUTPUT_DAT_RATE       |   \
                                       NORMAL_POWER_MODE)

#define  CONFIG_CNTL_REG4             (FULL_SCALE_2g    |    \
                                       BLOCK_DATA_UPDATE)

#define  INTERNAL_FILTER_BYPASS       (0x00)

#define  DISABLE_SLEEP_TO_WAKEUP      (0x00)
/*#####################################################*/
typedef struct
{
	signed int* Buff_X;
	signed int* Buff_Y;
	signed int* Buff_Z;
	unsigned int Buff_X_Count;
	unsigned int Buff_Y_Count;
	unsigned int Buff_Z_Count;
}adxl345_data_filter_t;
/*#####################################################*/
typedef struct
{
	unsigned char MasterSlaveAddr;
	signed char Calibration_X;
	signed char Calibration_Y;
	signed char Calibration_Z;
	unsigned int FilterBuffSize;
	adxl345_data_filter_t OutputFilter;
	new_twi* TwiStruct;
}adxl345_t;
/*#####################################################*/
#define new_adxl345 adxl345_t
#ifndef new_
#define new_(structure) (structure*)calloc(1,sizeof(structure));
#endif
#define free_adxl345(address) free(address)\
		address = null
/*#####################################################*/
bool adxl345_bit_set(new_adxl345* adxl345_struct, unsigned char regOffset, unsigned char mask);
bool adxl345_bit_clr(new_adxl345* adxl345_struct, unsigned char regOffset, unsigned char mask);
bool adxl345_write(new_adxl345* adxl345_struct, unsigned char regOffset, unsigned char data);
bool adxl345_write_multiple(new_adxl345* adxl345_struct, unsigned int regOffset, unsigned char *Buff, unsigned int DataLen);
bool adxl345_read(new_adxl345* adxl345_struct, unsigned char regOffset, unsigned char* data);
bool adxl345_read_multiple(new_adxl345* adxl345_struct, unsigned int regOffset, unsigned char *Buff, unsigned int DataLen);
/*#####################################################*/
bool adxl345_device_id_corect(new_adxl345* adxl345_struct);
unsigned char adxl345_tap_status_read(new_adxl345* adxl345_struct);
bool adxl345_x_read(new_adxl345* adxl345_struct, signed int *Data);
bool adxl345_y_read(new_adxl345* adxl345_struct, signed int *Data);
bool adxl345_z_read(new_adxl345* adxl345_struct, signed int *Data);
bool adxl345_x_offset_set(new_adxl345* adxl345_struct, signed char Offset);
bool adxl345_y_offset_set(new_adxl345* adxl345_struct, signed char Offset);
bool adxl345_z_offset_set(new_adxl345* adxl345_struct, signed char Offset);
bool adxl345_power_ctl_set_wekup(new_adxl345* adxl345_struct, unsigned char Wekup);
bool adxl345_power_ctl_set_link(new_adxl345* adxl345_struct);
bool adxl345_power_ctl_clear_link(new_adxl345* adxl345_struct);
bool adxl345_power_ctl_set_autosleep(new_adxl345* adxl345_struct);
bool adxl345_power_ctl_clear_autosleep(new_adxl345* adxl345_struct);
bool adxl345_power_ctl_set_sleep(new_adxl345* adxl345_struct);
bool adxl345_power_ctl_clear_sleep(new_adxl345* adxl345_struct);
bool adxl345_data_format_set_full_resolution(new_adxl345* adxl345_struct);
bool adxl345_data_format_set_normal_resolution(new_adxl345* adxl345_struct);
bool adxl345_data_format_set(new_adxl345* adxl345_struct, unsigned int DataFormat);
bool adxl345_bw_rate_output_rate_set(new_adxl345* adxl345_struct, unsigned char Rate);
/*#####################################################*/
bool adxl345_init(new_adxl345* adxl345_struct);
void adxl345_free(new_adxl345* adxl345_struct);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "ADXL345.c"
#endif
/*#####################################################*/
#endif /* ADXL345_H_ */
/*#####################################################*/
