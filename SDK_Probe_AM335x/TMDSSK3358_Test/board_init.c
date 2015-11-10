/*
 * board_init.c
 *
 *  Created on: Oct 17, 2015
 *      Author: John Smith
 */




#include "board_init.h"
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

new_uart* Uart[6] = {NULL};
new_uart* DebugCom = NULL;
new_twi* TWI[3] = {NULL};
new_mcspi* SPI[2] = {NULL};
new_touchscreen* TouchScreen = NULL;
new_screen* ScreenBuff = NULL;
new_gpio* LED[LEDS_NR] = {NULL};
new_gpio* HARDBTN[HARDBTNS_NR] = {NULL};


new_gpio* MmcSd_Present = NULL;
new_gpio* eMMC_Res = NULL;
mmcsdCtrlInfo sdCtrl[2];

#ifdef USE_MPU60x0_9150
USE_MPU60x0_9150;
#endif
#ifdef USE_AK8975
USE_AK8975;
#endif
#ifdef USE_BMP180
USE_BMP180;
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
#ifdef USE_MPL3115A2
USE_MPL3115A2;
#endif
#ifdef USE_MS5611
USE_MS5611;
#endif
#ifdef USE_ADXL345
USE_ADXL345;
#endif
#ifdef USE_HIH613x
USE_HIH613x;
#endif
#ifdef USE_MPR121
USE_MPR121;
#endif
#ifdef USE_LEPTON_FLIR
USE_LEPTON_FLIR;
#endif

inline bool board_init()
{
	core_init();
	timer_init();
	RtcInit();
/*-----------------------------------------------------*/
	UART_0_INIT(0);
/*-----------------------------------------------------*/
	/* Display board message*/
#if defined(BOARD_MESSAGE)
	UARTprintf(DebugCom, "Use %s Board.\n\r", BOARD_MESSAGE);
#endif
/*-----------------------------------------------------*/
#if defined(HARDBTN1_PORT) && defined(HARDBTN1_PIN)
	HARDBTN[0] = gpio_assign(HARDBTN1_PORT, HARDBTN1_PIN, GPIO_DIR_INPUT, false);
	gpio_up_dn(HARDBTN[0], 0);
#endif
#if defined(HARDBTN2_PORT) && defined(HARDBTN2_PIN)
	HARDBTN[1] = gpio_assign(HARDBTN2_PORT, HARDBTN2_PIN, GPIO_DIR_INPUT, false);
	gpio_up_dn(HARDBTN[1], 0);
#endif
#if defined(HARDBTN3_PORT) && defined(HARDBTN3_PIN)
	HARDBTN[2] = gpio_assign(HARDBTN3_PORT, HARDBTN3_PIN, GPIO_DIR_INPUT, false);
	gpio_up_dn(HARDBTN[2], 0);
#endif
#if defined(HARDBTN4_PORT) && defined(HARDBTN4_PIN)
	HARDBTN[3] = gpio_assign(HARDBTN4_PORT, HARDBTN4_PIN, GPIO_DIR_INPUT, false);
	gpio_up_dn(HARDBTN[3], 0);
#endif
/*-----------------------------------------------------*/
#if defined(LED1_PORT) && defined(LED1_PIN)
	LED[0] = gpio_assign(LED1_PORT, LED1_PIN, GPIO_DIR_OUTPUT, false);
#endif
#if defined(LED2_PORT) && defined(LED2_PIN)
	LED[1] = gpio_assign(LED2_PORT, LED2_PIN, GPIO_DIR_OUTPUT, false);
#endif
#if defined(LED3_PORT) && defined(LED3_PIN)
	LED[2] = gpio_assign(LED3_PORT, LED3_PIN, GPIO_DIR_OUTPUT, false);
#endif
#if defined(LED4_PORT) && defined(LED4_PIN)
	LED[3] = gpio_assign(LED4_PORT, LED4_PIN, GPIO_DIR_OUTPUT, false);
#endif
/*-----------------------------------------------------*/
	TWI_INIT(0);
/*-----------------------------------------------------*/
SPI_INIT(0);
/*-----------------------------------------------------*/
/* Set up the ADC 0 */
#if _USE_INT_ADC == 1
	ADC_0_INIT
#endif
/*-----------------------------------------------------*/
#if _USE_MPU60x0_9150 == 1
	MPU60x0_9150_INIT(0);
#endif
/*-----------------------------------------------------*/
#if _USE_AK8975 == 1
	AK8975_INIT(0);
#endif
/*-----------------------------------------------------*/
#if _USE_BMP180 == 1
	BMP180_INIT(0);
#endif
/*-----------------------------------------------------*/
#if _USE_SHT11 == 1
	SHT11_INIT();
#endif
/*-----------------------------------------------------*/
#if _USE_ADXL345 == 1
	ADXL345_INIT(0);
#endif
/*-----------------------------------------------------*/
#if _USE_HIH613x == 1
	HIH613x_INIT(0);
#endif
/*-----------------------------------------------------*/
#if _USE_MPL3115A2 == 1
	MPL3115A2_INIT(0);
#endif
/*-----------------------------------------------------*/
#if _USE_MPR121 == 1
	MPR121_INIT(0);
#endif
/*-----------------------------------------------------*/
#if _USE_LEPTON_FLIR == 1
	LEPTON_FLIR_INIT(1, 0);
#endif
/*-----------------------------------------------------*/
	MmcSd_Present = gpio_assign(0, 6, GPIO_DIR_OUTPUT, false);
	UARTPuts(DebugCom, "Init MMCSD0 .......", -1);
	sdCtrl[0].SdNr = 0;
	mmcsd_init(&sdCtrl[0], MmcSd_Present, LED[0]);
	UARTPuts(DebugCom, "OK.\n\r", -1);
	mmcsd_idle(&sdCtrl[0]);
/*-----------------------------------------------------*/
	eMMC_Res = gpio_assign(1, 20, GPIO_DIR_OUTPUT, false);
	gpio_out(eMMC_Res, 0);
	Sysdelay(10);
	gpio_out(eMMC_Res, 1);
	UARTPuts(DebugCom, "Init eMMC on MMCSD1 interface.......", -1);
	sdCtrl[1].SdNr = 1;
	mmcsd_init(&sdCtrl[1], (Gpio_t*)NULL, LED[0]);
	UARTPuts(DebugCom, "OK.\n\r", -1);
	mmcsd_idle(&sdCtrl[1]);
/*-----------------------------------------------------*/
	ScreenBuff = new_(new_screen);
	ScreenBuff->raster_timings = &lcd_TFT43_TMDSSK3358;
	ScreenBuff->BackLightLevel = 60;
	ScreenBuff->PmicTwiModuleStruct = TWI[0];
	screen_init(ScreenBuff);
	UARTprintf(DebugCom, "LCD display initialize successful for %dx%d resolution, %d Bit bus.\n\r" , ScreenBuff->raster_timings->X, ScreenBuff->raster_timings->Y, ScreenBuff->raster_timings->bus_size);

	TouchScreen->screen_max_x = (double)ScreenBuff->raster_timings->X;
	TouchScreen->screen_max_y = (double)ScreenBuff->raster_timings->Y;
	TouchScreen->pDisplay = ScreenBuff;
	InitTouchScreen(TouchScreen);
	UARTPuts(DebugCom, "Init calibration of LCD resistive touch screen....." , -1);
	TouchCalibrate(TouchScreen, ScreenBuff);
	UARTPuts(DebugCom, "OK.\n\r" , -1);
	put_rectangle(ScreenBuff, 0, 0, ScreenBuff->raster_timings->X, ScreenBuff->raster_timings->Y, true, controls_color.Scren);
	box_cache_clean(ScreenBuff, 0, 0, ScreenBuff->raster_timings->X, ScreenBuff->raster_timings->Y);
	return true;
}
