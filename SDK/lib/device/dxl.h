/*
 *  lib/device/ax_1x_actuator.h
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

#ifndef DXL_1X_ACTUATOR_H_
#define DXL_1X_ACTUATOR_H_
#include "stdbool.h"
#include "api/gpio_def.h"
#include "api/gpio_api.h"
#include "api/uart_def.h"
#include "api/uart_api.h"
//*******************************************************
//* INSTRUCTIONS
//*******************************************************
typedef enum {
	DXL_PING = 0x01, 				//obtain a status packet
	DXL_READ_DATA, 			 		//read Control Table values
	DXL_WRITE_DATA, 			 	//write Control Table values
	DXL_REG_WRITE, 			 		//write and wait for ACTION
	DXL_ACTION,				 		//triggers REG_WRITE
	DXL_RESET, 				 		//set factory defaults
	DXL_SYNC_WRITE = 0x83, 			//control multiple. actuators
}DXL_COMM;
//#####################################################
#define DXL_ERR_MASK_VOLTAGE		(1)
#define DXL_ERR_MASK_ANGLE			(2)
#define DXL_ERR_MASK_OVERHEAT		(4)
#define DXL_ERR_MASK_RANGE			(8)
#define DXL_ERR_MASK_CHECKSUM		(16)
#define DXL_ERR_MASK_OVERLOAD		(32)
#define DXL_ERR_MASK_INSTRUCTION	(64)
//#####################################################
typedef enum {
	DXL_COMM_SUCCESS = 0,
	DXL_COMM_RXTIMEOUT,
	DXL_COMM_RXCORRUPT,
	DXL_COMM_ERROR,
	DXL_COMM_OVERFLOW,
	DXL_COMM_CHECKSUM,
	DXL_COMM_BUFF_ALLOC
}DXL_COMM_ERR;
//#####################################################
enum{
	DXL_MODEL_NUMBER_L, 		// 0x00
	DXL_MODEL_NUMBER_H, 		// 0x01
	DXL_VERSION, 				// 0x02
	DXL_ID, 					// 0x03
	DXL_BAUD_RATE, 				// 0x04
	DXL_RETURN_DELAY_TIME, 		// 0x05
	DXL_CW_ANGLE_LIMIT_L, 		// 0x06
	DXL_CW_ANGLE_LIMIT_H, 		// 0x07
	DXL_CCW_ANGLE_LIMIT_L, 		// 0x08
	DXL_CCW_ANGLE_LIMIT_H, 		// 0x09
	DXL_RESERVED1, 				// 0x0A
	DXL_LIMIT_TEMPERATURE, 		// 0x0B
	DXL_DOWN_LIMIT_VOLTAGE, 	// 0x0C
	DXL_UP_LIMIT_VOLTAGE, 		// 0x0D
	DXL_MDXL_TORQUE_L, 			// 0x0E
	DXL_MDXL_TORQUE_H, 			// 0x0F
	DXL_STATUS_RETURN_LEVEL, 	// 0x10
	DXL_ALARM_LED, 				// 0x11
	DXL_ALARM_SHUTDOWN, 		// 0x12
	DXL_RESERVED2, 				// 0x13
	DXL_DOWN_CALIBRATION_L, 	// 0x14
	DXL_DOWN_CALIBRATION_H, 	// 0x15
	DXL_UP_CALIBRATION_L, 		// 0x16
	DXL_UP_CALIBRATION_H, 		// 0x17
	DXL_TORQUE_ENABLE, 			// 0x18
	DXL_LED, 					// 0x19
	DXL_CW_COMPLIANCE_MARGIN, 	// 0x1A
	DXL_CCW_COMPLIANCE_MARGIN, 	// 0x1B
	DXL_CW_COMPLIANCE_SLOPE, 	// 0x1C
	DXL_CCW_COMPLIANCE_SLOPE, 	// 0x1D
	DXL_GOAL_POSITION_L, 		// 0x1E
	DXL_GOAL_POSITION_H, 		// 0x1F
	DXL_MOVING_SPEED_L, 		// 0x20
	DXL_MOVING_SPEED_H, 		// 0x21
	DXL_TORQUE_LIMIT_L, 		// 0x22
	DXL_TORQUE_LIMIT_H, 		// 0x23
	DXL_PRESENT_POSITION_L, 	// 0x24
	DXL_PRESENT_POSITION_H, 	// 0x25
	DXL_PRESENT_SPEED_L, 		// 0x26
	DXL_PRESENT_SPEED_H, 		// 0x27
	DXL_PRESENT_LOAD_L, 		// 0x28
	DXL_PRESENT_LOAD_H, 		// 0x29
	DXL_PRESENT_VOLTAGE, 		// 0x2A
	DXL_PRESENT_TEMPERATURE, 	// 0x2B
	DXL_REGISTERED_INSTRUCTION, // 0x2C
	DXL_RESERVE3, 				// 0x2D
	DXL_MOVING, 				// 0x2E
	DXL_LOCK, 					// 0x2F
	DXL_PUNCH_L, 				// 0x30
	DXL_PUNCH_H 				// 0x31
};
//#####################################################
typedef struct DXL_DATA_s{
	unsigned short model_number; 			// 0x00
	unsigned char  version; 				// 0x02
	unsigned char  id; 						// 0x03
	unsigned char  baud_rate; 				// 0x04
	unsigned char  return_delay_time; 		// 0x05
	unsigned short cw_angle_limit; 			// 0x06
	unsigned short ccw_angle_limit; 		// 0x08
	unsigned char  reserved1; 				// 0x0A
	unsigned char  limit_temperature; 		// 0x0B
	unsigned char  down_limit_voltage; 		// 0x0C
	unsigned char  up_limit_voltage; 		// 0x0D
	unsigned short mdxl_torque; 			// 0x0E
	unsigned char  status_return_level; 	// 0x10
	unsigned char  alarm_led; 				// 0x11
	unsigned char  alarm_shutdown; 			// 0x12
	unsigned char  reserved2; 				// 0x13
	unsigned short down_calibration; 		// 0x14
	unsigned short up_calibration; 			// 0x16
	unsigned char  torque_enable; 			// 0x18
	unsigned char  led; 					// 0x19
	unsigned char  cw_compliance_margin; 	// 0x1A
	unsigned char  ccw_compliance_margin; 	// 0x1B
	unsigned char  cw_compliance_slope; 	// 0x1C
	unsigned char  ccw_compliance_slope; 	// 0x1D
	unsigned short goal_position; 			// 0x1E
	unsigned short moving_speed; 			// 0x20
	unsigned short torque_limit; 			// 0x22
	unsigned short present_position; 		// 0x24
	unsigned short present_speed; 			// 0x26
	unsigned short present_load; 			// 0x28
	unsigned char  present_voltage; 		// 0x2A
	unsigned char  present_temperature; 	// 0x2B
	unsigned char  registered_instruction; 	// 0x2C
	unsigned char  reserved3; 				// 0x2D
	unsigned char  moving; 					// 0x2E
	unsigned char  lock; 					// 0x2F
	unsigned short punch; 					// 0x30
}DXL_DATA_t;
//#####################################################
typedef struct DXL_SYNK_IND_PACKET_s {
	unsigned char id;
	unsigned char data[12];
}DXL_SYNK_IND_PACKET_t;
//#####################################################
typedef struct DXL_ACTUATOR_s {
	Gpio_t *TxEnGpio;
	Uart_t* Uart;
	unsigned long timeout;
}DXL_ACTUATOR_t;
//#####################################################
DXL_COMM_ERR dxl_ping(DXL_ACTUATOR_t *settings, unsigned char id, unsigned char* err);
DXL_COMM_ERR dxl_read(DXL_ACTUATOR_t *settings, unsigned char* buff, unsigned char id, unsigned char reg, unsigned char read_nr, unsigned char *dxl_err);
DXL_COMM_ERR dxl_write_data(DXL_ACTUATOR_t *settings, unsigned char id, unsigned char reg, unsigned char* send_data, unsigned char write_nr, unsigned char *aditional_info, unsigned char *aditional_info_len, unsigned char *dxl_err);
DXL_COMM_ERR dxl_reg_write(DXL_ACTUATOR_t *settings, unsigned char id, unsigned char reg, unsigned char* send_data, unsigned char write_nr, unsigned char *aditional_info, unsigned char *aditional_info_len, unsigned char *dxl_err);
DXL_COMM_ERR dxl_reset(DXL_ACTUATOR_t *settings, unsigned char id, unsigned char *dxl_err);
DXL_COMM_ERR dxl_synk_write(DXL_ACTUATOR_t *settings, unsigned char reg, DXL_SYNK_IND_PACKET_t *send_data_table, unsigned char write_nr, unsigned char nr_of_pockets);
DXL_COMM_ERR dxl_action(DXL_ACTUATOR_t *settings);
//#####################################################
//#####################################################
#define new_dxl_actuator DXL_ACTUATOR_t
#ifndef new_
#define new_(structure) (structure*)calloc(1,sizeof(structure));
#endif
#define free_dxl_actuator(address) free(address);
//#####################################################
#ifdef HEADER_INCLUDE_C_FILES
#include "ax_1x_actuator.c"
#endif
//#####################################################
#endif /* DXL_1X_ACTUATOR_H_ */
//#####################################################
