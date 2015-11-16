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


#include "api/init_api_def.h"
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


#include "pinmux/pin_mux_uart.h"
#include "pinmux/pin_mux_spi.h"
#include "pinmux/pin_mux_twi.h"

#include "interface/int_touchscreen_interface.h"
#include "interface/rtc_interface.h"

#include "lib/fs/fat.h"

#include "interface/mmcsdlib/mmcsd_proto.h"

#include "device/ADXL345.h"
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


inline bool board_init();

/*#####################################################*/
#endif /* BOARD_INIT_H_ */
