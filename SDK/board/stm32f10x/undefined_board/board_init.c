/*
 * board_init.c
 *
 * Created: 2/8/2013 12:22:54 AM
 *  Author: XxXx
 */

#include "stdbool.h"
#include "string.h"
#include "stdlib.h"
#include "project.h"
#ifdef BIOLOID_TEST
#include "bioloid_test.h"
#endif
#ifdef SENSORS_TEST
#include "sensors_test.h"
#endif
#include "stm32f10x_conf.h"
#include "sys/system_stm32f10x.h"
#include "board_init.h"
#include "api/core_init_api.h"
#include "api/timer_api.h"
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
//#include "lib/gfx/controls_definition.h"
//#include "lib/fs/fat.h"
//#include "device/mi0283.h"
#include "device/dxl.h"
#include "device/sht11.h"
#include "device/srf02.h"
#include "device/mhc5883.h"
#include "device/mpu60x0.h"
#include "device/ms5611.h"
/*#####################################################*/
new_uart* Uart[3] = {NULL,NULL,NULL};
new_uart* DebugCom = NULL;
new_twi* TWI[2] = {NULL,NULL};
new_adc* ADC[2] = {NULL};
new_gpio* LED1 = NULL;
#ifdef DXL_INTERFACE_INIT
new_gpio* ENTX = NULL;
new_dxl_actuator *DXL;
#endif
#ifdef USE_SHT11
USE_SHT11;
#endif
#ifdef USE_SRF02
USE_SRF02;
#endif
#ifdef USE_MHC5883
USE_MHC5883;
#endif
#ifdef USE_MS5611
USE_MS5611;
#endif
#ifdef USE_MPU60x0
USE_MPU60x0;
#endif
/*-----------------------------------------------------*/
//new_touchscreen* TouchScreen = NULL;
//new_screen* ScreenBuff = NULL;
/*-----------------------------------------------------*/
//SD_Struct_t SD_StructDisk1;
//SD_Struct_t SD_StructDisk2;
/*#####################################################*/
bool board_init()
{
	//RtcStruct.Rtc_ClkSource = _Rtc_Clk_Source_RCOSC_gc;
	core_init();
	timer_init();
/*-----------------------------------------------------*/
#ifdef UART_0_INIT
	UART_0_INIT
#endif
/*-----------------------------------------------------*/
#ifdef DXL_INTERFACE_INIT
	DXL_INTERFACE_INIT
#endif
/*-----------------------------------------------------*/
/* Display board message*/
#if defined(BOARD_MESSAGE)
	UARTPutc(DebugCom, 0xFF);
	UARTPutc(DebugCom, 0xFF);
	UARTPutc(DebugCom, '\n');
	UARTPutc(DebugCom, '\r');
	UARTprintf(DebugCom, "Use %s Board.\n\r", BOARD_MESSAGE);
#endif
/*-----------------------------------------------------*/
#ifdef TWI_1_INIT
	TWI_1_INIT
#endif
/*-----------------------------------------------------*/
#ifdef ADC_0_INIT
	ADC_0_INIT
#endif
/*-----------------------------------------------------*/
#ifdef SHT11_INIT
	SHT11_INIT
#endif
/*-----------------------------------------------------*/
#ifdef SRF02_INIT
	SRF02_INIT
#endif
/*-----------------------------------------------------*/
#ifdef MHC5883_INIT
	MHC5883_INIT
#endif
/*-----------------------------------------------------*/
#ifdef MS5611_INIT
	MS5611_INIT
#endif
/*-----------------------------------------------------*/
#ifdef MPU60x0_INIT
	MPU60x0_INIT
#endif
/*-----------------------------------------------------*/
	LED1 = gpio_assign(IOC, 13, GPIO_OUT_PUSH_PULL, false);
	return true;
}
