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


inline bool board_init();

/*#####################################################*/
#endif /* BOARD_INIT_H_ */
