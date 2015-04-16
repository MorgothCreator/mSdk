/*
 *  lib/device/lepton_flir.h
 *
 *  Copyright (C) 2015  Iulian Gheorghiu <morgoth.creator@gmail.com>
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

/*#####################################################*/
#ifndef LEPTON_FLIR_H_
#define LEPTON_FLIR_H_
/*#####################################################*/
#include <stdbool.h>
#include "api/twi_def.h"
#include "api/twi_api.h"
#include "api/mcspi_def.h"
#include "api/mcspi_api.h"
#include "interface/mcspi_interface.h"
/*#####################################################*/
#define LEPTON_FLIR_ADDR					(0x2A)

/* Block Data Buffers
*/
    #define LEP_DATA_BUFFER_0_BASE_ADDR         0xF800
    #define LEP_DATA_BUFFER_1_BASE_ADDR         0xFC00


/* The Lepton I2C Registers Sub-Addresses
*/
#define LEP_I2C_REG_BASE_ADDR               0x0000

/* Host On Switch when camera is in stand by of off
*/
#define LEP_I2C_POWER_REG                  (LEP_I2C_REG_BASE_ADDR + 0x0000 )

/* Host Command Interface over I2C
*/
#define LEP_I2C_STATUS_REG                 (LEP_I2C_REG_BASE_ADDR + 0x0002 )
#define LEP_I2C_COMMAND_REG                (LEP_I2C_REG_BASE_ADDR + 0x0004 )
#define LEP_I2C_DATA_LENGTH_REG            (LEP_I2C_REG_BASE_ADDR + 0x0006 )
#define LEP_I2C_DATA_0_REG                 (LEP_I2C_REG_BASE_ADDR + 0x0008 )
#define LEP_I2C_DATA_1_REG                 (LEP_I2C_REG_BASE_ADDR + 0x000A )
#define LEP_I2C_DATA_2_REG                 (LEP_I2C_REG_BASE_ADDR + 0x000C )
#define LEP_I2C_DATA_3_REG                 (LEP_I2C_REG_BASE_ADDR + 0x000E )
#define LEP_I2C_DATA_4_REG                 (LEP_I2C_REG_BASE_ADDR + 0x0010 )
#define LEP_I2C_DATA_5_REG                 (LEP_I2C_REG_BASE_ADDR + 0x0012 )
#define LEP_I2C_DATA_6_REG                 (LEP_I2C_REG_BASE_ADDR + 0x0014 )
#define LEP_I2C_DATA_7_REG                 (LEP_I2C_REG_BASE_ADDR + 0x0016 )
#define LEP_I2C_DATA_8_REG                 (LEP_I2C_REG_BASE_ADDR + 0x0018 )
#define LEP_I2C_DATA_9_REG                 (LEP_I2C_REG_BASE_ADDR + 0x001A )
#define LEP_I2C_DATA_10_REG                (LEP_I2C_REG_BASE_ADDR + 0x001C )
#define LEP_I2C_DATA_11_REG                (LEP_I2C_REG_BASE_ADDR + 0x001E )
#define LEP_I2C_DATA_12_REG                (LEP_I2C_REG_BASE_ADDR + 0x0020 )
#define LEP_I2C_DATA_13_REG                (LEP_I2C_REG_BASE_ADDR + 0x0022 )
#define LEP_I2C_DATA_14_REG                (LEP_I2C_REG_BASE_ADDR + 0x0024 )
#define LEP_I2C_DATA_15_REG                (LEP_I2C_REG_BASE_ADDR + 0x0026 )

#define LEP_I2C_DATA_CRC_REG               (LEP_I2C_REG_BASE_ADDR + 0x0028 )

#define LEP_I2C_DATA_BUFFER_0              (LEP_DATA_BUFFER_0_BASE_ADDR )
#define LEP_I2C_DATA_BUFFER_0_END          (LEP_DATA_BUFFER_0_BASE_ADDR + 0x03FF )
#define LEP_I2C_DATA_BUFFER_0_LENGTH        0x400

#define LEP_I2C_DATA_BUFFER_1              (LEP_DATA_BUFFER_1_BASE_ADDR )
#define LEP_I2C_DATA_BUFFER_1_END          (LEP_DATA_BUFFER_1_BASE_ADDR + 0x03FF )
#define LEP_I2C_DATA_BUFFER_1_LENGTH        0x400

#define LEP_I2C_STATUS_BUSY_BIT_MASK        0x0001   /* Bit 0 is the Busy Bit */

/*
 * VID Module Command IDs
 */
#define LEP_VID_MODULE_BASE                  0x0300

#define LEP_CID_VID_POLARITY_SELECT         (LEP_VID_MODULE_BASE + 0x0000 )
#define LEP_CID_VID_LUT_SELECT              (LEP_VID_MODULE_BASE + 0x0004 )
#define LEP_CID_VID_LUT_TRANSFER            (LEP_VID_MODULE_BASE + 0x0008 )
#define LEP_CID_VID_FOCUS_CALC_ENABLE       (LEP_VID_MODULE_BASE + 0x000C )
#define LEP_CID_VID_FOCUS_ROI               (LEP_VID_MODULE_BASE + 0x0010 )
#define LEP_CID_VID_FOCUS_THRESHOLD         (LEP_VID_MODULE_BASE + 0x0014 )
#define LEP_CID_VID_FOCUS_METRIC            (LEP_VID_MODULE_BASE + 0x0018 )
#define LEP_CID_VID_SBNUC_ENABLE            (LEP_VID_MODULE_BASE + 0x001C )
#define LEP_CID_VID_GAMMA_SELECT            (LEP_VID_MODULE_BASE + 0x0020 )
#define LEP_CID_VID_FREEZE_ENABLE           (LEP_VID_MODULE_BASE + 0x0024 )


/*
 * VID Module Attribute Limits
 */


/*
 * VID Module Object Sizes
 */
   #define LEPTON_COLOR_LUT_SIZE               256     /* 8-bit LUT as 256 x 8-bits */



#define LEPTON_FLIR_LINE_SIZE				164
#define LEPTON_FLIR_LINES_NR				60
#define LEPTON_I2C_COMMAND_BUSY_WAIT_COUNT	1000

/*#####################################################*/
/*
 * Represents the different result codes the camera can return.
 */
typedef enum Result
{
   LEP_OK                            = 0,     /* Camera ok */
   LEP_COMM_OK                       = LEP_OK, /* Camera comm ok (same as LEP_OK) */

   LEP_ERROR                         = -1,    /* Camera general error */
   LEP_NOT_READY                     = -2,    /* Camera not ready error */
   LEP_RANGE_ERROR                   = -3,    /* Camera range error */
   LEP_CHECKSUM_ERROR                = -4,    /* Camera checksum error */
   LEP_BAD_ARG_POINTER_ERROR         = -5,    /* Camera Bad argument  error */
   LEP_DATA_SIZE_ERROR               = -6,    /* Camera byte count error */
   LEP_UNDEFINED_FUNCTION_ERROR      = -7,    /* Camera undefined function error */
   LEP_FUNCTION_NOT_SUPPORTED        = -8,    /* Camera function not yet supported error */

   /* OTP access errors */
   LEP_OTP_WRITE_ERROR               = -15,   /*!< Camera OTP write error */
   LEP_OTP_READ_ERROR				    = -16,   /* double bit error detected (uncorrectible) */

   LEP_OTP_NOT_PROGRAMMED_ERROR      = -18,   /* Flag read as non-zero */

   /* I2C Errors */
   LEP_ERROR_I2C_BUS_NOT_READY       = -20,   /* I2C Bus Error - Bus Not Avaialble */
   LEP_ERROR_I2C_BUFFER_OVERFLOW     = -22,   /* I2C Bus Error - Buffer Overflow */
   LEP_ERROR_I2C_ARBITRATION_LOST    = -23,   /* I2C Bus Error - Bus Arbitration Lost */
   LEP_ERROR_I2C_BUS_ERROR           = -24,   /* I2C Bus Error - General Bus Error */
   LEP_ERROR_I2C_NACK_RECEIVED       = -25,   /* I2C Bus Error - NACK Received */
   LEP_ERROR_I2C_FAIL                = -26,   /* I2C Bus Error - General Failure */

   /* Processing Errors */
   LEP_DIV_ZERO_ERROR                = -80,   /* Attempted div by zero */

   /* Comm Errors */
   LEP_COMM_PORT_NOT_OPEN            = -101,  /* Comm port not open */
   LEP_COMM_INVALID_PORT_ERROR       = -102,  /* Comm port no such port error */
   LEP_COMM_RANGE_ERROR              = -103,  /* Comm port range error */
   LEP_ERROR_CREATING_COMM           = -104,  /* Error creating comm */
   LEP_ERROR_STARTING_COMM           = -105,  /* Error starting comm */
   LEP_ERROR_CLOSING_COMM            = -106,  /* Error closing comm */
   LEP_COMM_CHECKSUM_ERROR           = -107,  /* Comm checksum error */
   LEP_COMM_NO_DEV                   = -108,  /* No comm device */
   LEP_TIMEOUT_ERROR                 = -109,  /* Comm timeout error */
   LEP_COMM_ERROR_WRITING_COMM       = -110,  /* Error writing comm */
   LEP_COMM_ERROR_READING_COMM       = -111,  /* Error reading comm */
   LEP_COMM_COUNT_ERROR              = -112,  /* Comm byte count error */

   /* Other Errors */
   LEP_OPERATION_CANCELED            = -126,  /* Camera operation canceled */
   LEP_UNDEFINED_ERROR_CODE          = -127   /* Undefined error */

} LEP_RESULT;
/*#####################################################*/
/* SYS Module Command IDs
*/
   #define LEP_SYS_MODULE_BASE                     0x0200

   #define LEP_CID_SYS_PING                        (LEP_SYS_MODULE_BASE + 0x0000 )
   #define LEP_CID_SYS_CAM_STATUS                  (LEP_SYS_MODULE_BASE + 0x0004 )
   #define LEP_CID_SYS_FLIR_SERIAL_NUMBER          (LEP_SYS_MODULE_BASE + 0x0008 )
   #define LEP_CID_SYS_CAM_UPTIME                  (LEP_SYS_MODULE_BASE + 0x000C )
   #define LEP_CID_SYS_AUX_TEMPERATURE_KELVIN      (LEP_SYS_MODULE_BASE + 0x0010 )
   #define LEP_CID_SYS_FPA_TEMPERATURE_KELVIN      (LEP_SYS_MODULE_BASE + 0x0014 )
   #define LEP_CID_SYS_TELEMETRY_ENABLE_STATE      (LEP_SYS_MODULE_BASE + 0x0018 )
   #define LEP_CID_SYS_TELEMETRY_LOCATION          (LEP_SYS_MODULE_BASE + 0x001C )
   #define LEP_CID_SYS_EXECTUE_FRAME_AVERAGE       (LEP_SYS_MODULE_BASE + 0x0020 )
   #define LEP_CID_SYS_NUM_FRAMES_TO_AVERAGE       (LEP_SYS_MODULE_BASE + 0x0024 )
   #define LEP_CID_SYS_CUST_SERIAL_NUMBER          (LEP_SYS_MODULE_BASE + 0x0028 )
   #define LEP_CID_SYS_SCENE_STATISTICS            (LEP_SYS_MODULE_BASE + 0x002C )
   #define LEP_CID_SYS_SCENE_ROI                   (LEP_SYS_MODULE_BASE + 0x0030 )
   #define LEP_CID_SYS_THERMAL_SHUTDOWN_COUNT      (LEP_SYS_MODULE_BASE + 0x0034 )
   #define LEP_CID_SYS_SHUTTER_POSITION            (LEP_SYS_MODULE_BASE + 0x0038 )
   #define LEP_CID_SYS_FFC_SHUTTER_MODE_OBJ        (LEP_SYS_MODULE_BASE + 0x003C )
   #define FLR_CID_SYS_RUN_FFC                     (LEP_SYS_MODULE_BASE + 0x0042 )
   #define LEP_CID_SYS_FFC_STATUS                  (LEP_SYS_MODULE_BASE + 0x0044 )

/* SYS Module Attribute Limits
*/

   #define LEP_SYS_MAX_FRAMES_TO_AVERAGE               128          /* Scale is 1x    */
   #define LEP_SYS_MAX_SERIAL_NUMBER_CHAR_SIZE         32

typedef struct LEPTON_FLIR_s{
	//STimer_t Timeout_Timer;
	bool busy;
	Mcspi_t* SPI;
	Twi_t* TWI;
	unsigned char CsNr;
	//unsigned char Stage;
}LEPTON_FLIR_t;
/*#####################################################*/
bool lepton_run_command(LEPTON_FLIR_t *structure, unsigned short commandID);
bool lepton_reg_write(LEPTON_FLIR_t *structure, unsigned short commandID, unsigned short *attributePtr, unsigned short attributeWordLength);
bool lepton_reg_read(LEPTON_FLIR_t *structure, unsigned short commandID, unsigned short *attributePtr, unsigned short attributeWordLength);
bool lepton_flir_get_image(LEPTON_FLIR_t *structure, unsigned short *image);
bool lepton_direct_write_buff(LEPTON_FLIR_t *structure, unsigned short *attributePtr, unsigned short attributeWordLength);
bool lepton_direct_write_reg(LEPTON_FLIR_t *structure, unsigned short regAddress, unsigned short regValue);
bool lepton_flir_init(LEPTON_FLIR_t *structure);
/*#####################################################*/
#define new_lepton_flir LEPTON_FLIR_t
#ifndef new_
#define new_(structure) (structure*)calloc(1,sizeof(structure));
#endif
#define free_lepton_flir(address) free(address);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "lepton_flir.c"
#endif

/*#####################################################*/
#endif
/*#####################################################*/
