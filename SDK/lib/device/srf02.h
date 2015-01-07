/*
 * srf02.h
 *
 *  Created on: Jan 6, 2015
 *      Author:  Iulian Gheorghiu <morgoth.creator@gmail.com>
 */
/*#####################################################*/
#ifndef SRF02_H_
#define SRF02_H_
/*#####################################################*/
#include "stdbool.h"
#include "api/twi_def.h"
#include "api/timer_api.h"
/*#####################################################*/
typedef enum {
	SRF02_START_RANGING = 0x50,
	SRF02_FAKE_RANGING = 0x56,
	SRF02_8CYCLES_BURST_NO_RANGING = 0x5C,
	SRF02_FORCE_AUTOTUNE_RESTART = 0x60,
	SRF02_SEQUENCE_ONE_CHANGE_I2C_ADDR = 0xA0,
	SRF02_SEQUENCE_THRE_CHANGE_I2C_ADDR = 0xA5,
	SRF02_SEQUENCE_TWO_CHANGE_I2C_ADDR = 0xAA,
}SRF02_COMMANDS;
/*#####################################################*/
typedef enum {
	SRF02_INCH = 0,
	SRF02_CENTIMETER,
	SRF02_MICROSECONDS,
}SRF02_UNIT_MEASURE;
/*#####################################################*/
typedef struct SRF02_s{
	STimer_t Timeout_Timer;
	SRF02_UNIT_MEASURE measure_unit;
	SRF02_COMMANDS reg_inst;
	unsigned short range_value;
	unsigned char addr;
	bool busy;
	Twi_t* TWI;
}SRF02_t;
/*#####################################################*/
bool srf02_start(SRF02_t *structure, SRF02_COMMANDS command);
bool srf02_read(SRF02_t *structure);
void srf02_display_data(SRF02_t *structure);
/*#####################################################*/
#define new_srf02 SRF02_t
#ifndef new_
#define new_(structure) (structure*)calloc(1,sizeof(structure));
#endif
#define free_srf02(address) free(address);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "srf02.c"
#endif
/*#####################################################*/
#endif /* SRF02_H_ */
