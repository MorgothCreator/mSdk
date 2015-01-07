/*
 * board_init.h
 *
 * Created: 2/8/2013 12:22:37 AM
 *  Author: Iulian Gheorghiu <morgoth.creator@gmail.com>
 */
/*#####################################################*/
#ifndef BOARD_INIT_H_
#define BOARD_INIT_H_
/*#####################################################*/
#include "stdbool.h"
#include "string.h"
#include "project.h"
#ifdef BIOLOID_TEST
#include "bioloid_test.h"
#endif
#ifdef SENSORS_TEST
#include "sensors_test.h"
#endif
#include "stm32f10x_conf.h"
#include "sys/system_stm32f10x.h"
#include "api/core_init_api.h"
#include "api/gpio_def.h"
#include "api/gpio_api.h"
#include "api/uart_def.h"
#include "api/uart_api.h"
#include "api/twi_def.h"
#include "api/twi_api.h"
#include "api/adc_def.h"
#include "api/adc_api.h"
//#include "api/lcd_def.h"
//#include "api/lcd_api.h"
//#include "api/mmcsd_api.h"
//#include "device/ar1020.h"
//#include "driver/dma.h"
//#include "lib/fs/fat.h"
//#include "device/mi0283.h"
#include "device/dxl.h"
#include "device/sht11.h"
#include "device/srf02.h"
#include "device/mhc5883.h"
#include "device/mpu60x0.h"
#include "device/ms5611.h"
/*#####################################################*/
#define BOARD_MESSAGE "STM32F10x undefined"
/*#####################################################*/
extern new_uart* Uart[];
extern new_uart* DebugCom;
extern new_twi* TWI[];
extern new_adc* ADC[];
extern new_gpio* LED1;
extern new_dxl_actuator *DXL;
extern USE_SHT11;
extern USE_SRF02;
extern USE_MHC5883;
extern USE_MS5611;
extern USE_MPU60x0;
/*-----------------------------------------------------*/
//extern new_touchscreen* TouchScreen;
//extern new_screen* ScreenBuff;
/*-----------------------------------------------------*/
//extern SD_Struct_t SD_StructDisk1;
//extern SD_Struct_t SD_StructDisk2;
/*#####################################################*/
bool board_init();
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "board_init.c"
#endif
/*#####################################################*/
/*#####################################################*/
#endif /* BOARD_INIT_H_ */
/*#####################################################*/
