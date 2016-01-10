/*
 * board_init.c
 *
 *  Created on: Oct 17, 2015
 *      Author: John Smith
 */




#include "board_init.h"
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
#include "api/usb_api.h"

#include "lib/gfx/controls_definition.h"
#include "lib/fs/fat.h"


#include "pinmux/pin_mux_uart.h"
#include "pinmux/pin_mux_spi.h"
#include "pinmux/pin_mux_twi.h"

#include "interface/int_touchscreen_interface.h"
#include "interface/rtc_interface.h"
#include "interface/hs_mmcsd_interface.h"

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
//mmcsdCtrlInfo sdCtrl[2];

NEW_MPU60x0_9150(MPU60x0_9150);
NEW_AK8975(AK8975);
NEW_BMP180(BMP180);
NEW_SHT11(SHT11);
NEW_SRF02(SRF02);
NEW_MHC5883(MHC5883);
NEW_MPL3115A2(MPL3115A2);
NEW_MS5611(MS5611);
NEW_ADXL345(ADXL345);
NEW_HIH613x(HIH613x);
NEW_MPR121(MPR121);
NEW_LEPTON_FLIR(LEPTON_FLIR);

inline bool board_init()
{
	core_init();
	timer_init();
	RtcInit();
/*-----------------------------------------------------*/
	INIT_UART(0, 115200, IOE, 16, IOE, 15);
	DebugCom = Uart[0];
/*-----------------------------------------------------*/
	/* Display board message*/
#if defined(BOARD_MESSAGE)
	UARTprintf(DebugCom, "Use %s Board.\n\r", BOARD_MESSAGE);
#endif
/*-----------------------------------------------------*/
#if defined(HARDBTN1_PORT) && defined(HARDBTN1_PIN)
	HARDBTN[0] = gpio_assign(HARDBTN1_PORT, HARDBTN1_PIN, GPIO_IN_PULL_DOWN, false);
	gpio_up_dn(HARDBTN[0], 0);
#endif
#if defined(HARDBTN2_PORT) && defined(HARDBTN2_PIN)
	HARDBTN[1] = gpio_assign(HARDBTN2_PORT, HARDBTN2_PIN, GPIO_IN_PULL_DOWN, false);
	gpio_up_dn(HARDBTN[1], 0);
#endif
#if defined(HARDBTN3_PORT) && defined(HARDBTN3_PIN)
	HARDBTN[2] = gpio_assign(HARDBTN3_PORT, HARDBTN3_PIN, GPIO_IN_PULL_DOWN, false);
	gpio_up_dn(HARDBTN[2], 0);
#endif
#if defined(HARDBTN4_PORT) && defined(HARDBTN4_PIN)
	HARDBTN[3] = gpio_assign(HARDBTN4_PORT, HARDBTN4_PIN, GPIO_IN_PULL_DOWN, false);
	gpio_up_dn(HARDBTN[3], 0);
#endif
/*-----------------------------------------------------*/
#if defined(LED1_PORT) && defined(LED1_PIN)
	LED[0] = gpio_assign(LED1_PORT, LED1_PIN, GPIO_OUT_PUSH_PULL, false);
#endif
#if defined(LED2_PORT) && defined(LED2_PIN)
	LED[1] = gpio_assign(LED2_PORT, LED2_PIN, GPIO_OUT_PUSH_PULL, false);
#endif
#if defined(LED3_PORT) && defined(LED3_PIN)
	LED[2] = gpio_assign(LED3_PORT, LED3_PIN, GPIO_OUT_PUSH_PULL, false);
#endif
#if defined(LED4_PORT) && defined(LED4_PIN)
	LED[3] = gpio_assign(LED4_PORT, LED4_PIN, GPIO_OUT_PUSH_PULL, false);
#endif
/*-----------------------------------------------------*/
	INIT_TWI(0, 400000, IOC, 17, IOC, 16, 256, 256);
/*-----------------------------------------------------*/
	INIT_SPI(1, 25000000, 8, IOA, 17, IOB, 17, IOB, 16, IOA, 16);
/*-----------------------------------------------------*/
/* Set up the ADC 0 */
	//INIT_ADC(0);
/*-----------------------------------------------------*/
	INIT_MPU60x0_9150(MPU60x0_9150, 0);
/*-----------------------------------------------------*/
	INIT_AK8975(AK8975, 0);
/*-----------------------------------------------------*/
	INIT_BMP180(BMP180, 0);
/*-----------------------------------------------------*/
	INIT_SHT11(SHT11, IOB, 12, IOB, 13);
/*-----------------------------------------------------*/
	INIT_ADXL345(ADXL345, 0);
/*-----------------------------------------------------*/
	INIT_HIH613x(HIH613x, 0);
/*-----------------------------------------------------*/
	INIT_MPL3115A2(MPL3115A2, 0);
/*-----------------------------------------------------*/
	INIT_MPR121(MPR121, 0);
/*-----------------------------------------------------*/
	INIT_LEPTON_FLIR(LEPTON_FLIR, 1, 0);
/*-----------------------------------------------------*/
	/* Init uSD */
	//INIT_MMCSD(0, NULL, gpio_assign(0, 6, GPIO_IN_PULL_UP, false), LED[2]);
/*-----------------------------------------------------*/
	/* Init eMMC */
	//INIT_MMCSD(1, gpio_assign(1, 20, GPIO_OUT_PUSH_PULL, false), NULL, LED[2]);
/*-----------------------------------------------------*/
#ifdef usb_1_msc
	INIT_USB_MSC_HOST(1, LED[2]);
#elif defined( usb_1_mouse ) && !defined( touch )
/*-----------------------------------------------------*/
	INIT_USB_MOUSE_HOST(1);
#endif
/*-----------------------------------------------------*/
#if defined( usb_0_dev_msc ) && defined ( BridgeUsbDev0ToMmcSd0)
	/* Bridge USB0 client to uSD*/
	INIT_USB_DEV_MSC_TO_MMCSD_BRIDGE(0, 0, USB_MMCSD_DRV_STRUCT, NULL, gpio_assign(0, 6, GPIO_IN_PULL_UP, false), LED[2])
/*-----------------------------------------------------*/
#elif  defined( usb_0_dev_msc ) && defined ( BridgeUsbDev0ToMmcSd1)
	/* Bridge USB0 client to eMMC*/
	INIT_USB_DEV_MSC_TO_MMCSD_BRIDGE(0, 1, USB_EMMC_DRV_STRUCT, gpio_assign(1, 20, GPIO_OUT_PUSH_PULL, false), NULL, LED[2])
/*-----------------------------------------------------*/
#elif  defined( usb_0_dev_msc ) && defined ( BridgeUsbDev0ToUsbHost1) && defined(usb_1_msc) && !defined(usb_1_mouse)
	/* Bridge USB0 client to USB1 host*/
	INIT_USB_DEV_MSC_BRIDGE(0, 1, USB1_TO_USB0_BRIDGE, LED[2]);
#endif
/*-----------------------------------------------------*/
	ScreenBuff = new_(new_screen);
	ScreenBuff->raster_timings = &lcd_TFT43_TMDSSK3358;
	//ScreenBuff->BackLightLevel = 60;
	//ScreenBuff->PmicTwiModuleStruct = TWI[0];
	ScreenBuff->BackLight = gpio_assign(IOD, 17, GPIO_OUT_PUSH_PULL, false);
	screen_init(ScreenBuff);
	UARTprintf(DebugCom, "LCD display initialize successful for %dx%d resolution, %d Bit bus.\n\r" , ScreenBuff->raster_timings->X, ScreenBuff->raster_timings->Y, ScreenBuff->raster_timings->bus_size);

	TouchScreen = new_(new_touchscreen);
	//TouchScreen->TwiStruct = TWI[1];
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
