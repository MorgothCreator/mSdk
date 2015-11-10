/*
 * board_init.h
 *
 *  Created on: Oct 17, 2015
 *      Author: John Smith
 */

#ifndef BOARD_INIT_H_
#define BOARD_INIT_H_
/*#####################################################*/
#include <stdbool.h>
#include <string.h>
#include "main.h"
#include "api/core_init_api.h"
#include "api/uart_api.h"
#include "api/twi_api.h"
#include "api/mcspi_api.h"
#include "api/lcd_def.h"
#include "api/lcd_api.h"
#include "api/touchscreen_def.h"
#include "api/mmcsd_api.h"

#include "lib/gfx/controls_definition.h"
#include "lib/fs/fat.h"

#include "device/ADXL345.h"

#include "pinmux/pin_mux_uart.h"
#include "pinmux/pin_mux_spi.h"
#include "pinmux/pin_mux_twi.h"

#include "interface/int_touchscreen_interface.h"
#include "interface/rtc_interface.h"

#include "lib/fs/fat.h"

#include "interface/mmcsdlib/mmcsd_proto.h"

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


extern new_uart* Uart[];
extern new_uart* DebugCom;
extern new_twi* TWI0;
extern new_mcspi* MCSPI0;
extern new_touchscreen* TouchScreen;
extern new_screen* ScreenBuff;
extern new_gpio* LED[];
extern new_gpio* HARDBTN[];


extern new_gpio* MmcSd_Present;
extern new_gpio* eMMC_Res;
extern mmcsdCtrlInfo sdCtrl[];

#ifdef USE_MPU60x0_9150
extern USE_MPU60x0_9150;
#endif
#ifdef USE_AK8975
extern USE_AK8975;
#endif
#ifdef USE_BMP180
extern USE_BMP180;
#endif
#ifdef USE_SHT11
extern USE_SHT11;
#endif
#ifdef USE_SRF02
extern USE_SRF02;
#endif
#ifdef USE_MHC5883
extern USE_MHC5883;
#endif
#ifdef USE_MPL3115A2
extern USE_MPL3115A2;
#endif
#ifdef USE_MS5611
extern USE_MS5611;
#endif
#ifdef USE_ADXL345
extern USE_ADXL345;
#endif
#ifdef USE_HIH613x
extern USE_HIH613x;
#endif
#ifdef USE_MPR121
extern USE_MPR121;
#endif
#ifdef USE_LEPTON_FLIR
extern USE_LEPTON_FLIR;
#endif


inline bool board_init();

/*#####################################################*/
#endif /* BOARD_INIT_H_ */
