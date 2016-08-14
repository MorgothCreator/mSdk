/*
 * board_init.h
 *
 * Created: 2/8/2013 12:22:37 AM
 *  Iulian Gheorghiu <morgoth.creator@gmail.com>
 */
/*#####################################################*/
#ifndef BOARD_INIT_H_
#define BOARD_INIT_H_
/*#####################################################*/
#include <stdbool.h>
#include <string.h>
#include "main.h"
#include "stm32f4xx_conf.h"
#include "sys/system_stm32f4xx.h"
#include "api/init_api_def.h"
#include "api/core_init_api.h"
#include "api/gpio_def.h"
#include "api/gpio_api.h"
#include "api/uart_def.h"
#include "api/uart_api.h"
#include "api/twi_def.h"
#include "api/twi_api.h"
#include "api/mcspi_def.h"
#include "api/mcspi_api.h"
#include "api/adc_def.h"
#include "api/adc_api.h"
//#include "api/lcd_def.h"
//#include "api/lcd_api.h"
//#include "api/mmcsd_api.h"
//#include "device/ar1020.h"
//#include "driver/dma.h"
//#include "lib/fs/fat.h"
//#include "device/mi0283.h"
#include "device/mpu60x0_9150.h"
#include "device/ak8975.h"
#include "device/bmp180.h"
#include "device/sht11.h"
#include "device/srf02.h"
#include "device/mhc5883.h"
#include "device/ms5611.h"
#include "device/adxl345.h"
#include "device/hih6130.h"
#include "device/mpl3115a2.h"
#include "device/mpr121.h"
#include "device/lepton_flir.h"
/*#####################################################*/
#define BOARD_MESSAGE "Olimex STM32-E407"
/*#####################################################*/
extern new_uart* Uart[];
extern new_uart* DebugCom;
extern new_twi* TWI[];
extern new_adc* _ADC[];
extern new_gpio* HARDBTN1;
extern new_gpio* LED[];



extern NEW_MPU60x0_9150(MPU60x0_9150);
extern NEW_AK8975(AK8975);
extern NEW_BMP180(BMP180);
extern NEW_SHT11(SHT11);
extern NEW_SRF02(SRF02);
extern NEW_MHC5883(MHC5883);
extern NEW_MPL3115A2(MPL3115A2);
extern NEW_MS5611(MS5611);
extern NEW_ADXL345(ADXL345);
extern NEW_HIH613x(HIH613x);
extern NEW_MPR121(MPR121);
extern NEW_LEPTON_FLIR(LEPTON_FLIR);

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
