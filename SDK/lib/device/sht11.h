/*
 * sht11.h
 *
 *  Created on: Jan 5, 2015
 *      Author: Iulian Gheorghiu (morgoth.creator@gmail.com)
 */
/*#####################################################*/
#ifndef SHT11_H_
#define SHT11_H_
/*#####################################################*/
#define SHT11_START_MEASURE_TEMPERATURE			0x03
#define SHT11_START_MEASURE_HUMIDITY			0x05
#define SHT11_READ_STATUS_REG					0x07
#define SHT11_WRITE_STATUS_REG					0x06
#define SHT11_RESET								0x1E
/*#####################################################*/
#define SH11_VDD_5V			-40
#define SH11_VDD_4V			-39.75
#define SH11_VDD_3_5V		-39.66
#define SH11_VDD_3V			-39.60
#define SH11_VDD_2_5V		-39.55
/*#####################################################*/
#include "stdbool.h"
#include "api/gpio_def.h"
#include "api/timer_api.h"
/*#####################################################*/
typedef struct SHT11_s{
	STimer_t Timeout_Timer;
	unsigned int state_delay;
	unsigned char reg_inst;
	float temperature;
	float humidity;
	float vdd_comp;
	bool busy;
	Gpio_t *Scl;
	Gpio_t *Sda;
}SHT11_t;
/*#####################################################*/
bool sht11_write(SHT11_t *structure, unsigned char cmd, unsigned char *status_reg);
bool sht11_read(SHT11_t *structure);
void sht11_display_data(SHT11_t *structure);
/*#####################################################*/
#define new_sht11 SHT11_t
#ifndef new_
#define new_(structure) (structure*)calloc(1,sizeof(structure));
#endif
#define free_sht11(address) free(address);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "sht11.c"
#endif
/*#####################################################*/
#endif /* SHT11_H_ */
/*#####################################################*/
