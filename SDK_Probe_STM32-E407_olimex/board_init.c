/*
 * board_init.c
 *
 * Created: 2/8/2013 12:22:54 AM
 *  Iulian Gheorghiu <morgoth.creator@gmail.com>
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "main.h"
#include "stm32f4xx_conf.h"
#include "sys/system_stm32f4xx.h"
#include "board_init.h"

#include "api/init_api_def.h"
#include "api/core_init_api.h"
#include "api/timer_api.h"
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
#include "api/lcd_def.h"
#include "api/lcd_api.h"
#include "api/mmcsd_api.h"
#include "interface/hs_mmcsd_interface.h"
//#include "lib/gfx/controls_definition.h"
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
new_uart* Uart[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
new_uart* DebugCom = NULL;
new_twi* TWI[3] = {NULL, NULL, NULL};
new_mcspi* SPI[3] = {NULL, NULL, NULL};
new_adc* _ADC[2] = {NULL, NULL};
new_gpio* LED[LEDS_NR] = {NULL};
new_gpio* HARDBTN[HARDBTNS_NR] = {NULL};
new_gpio* LCD_BACKLIGHT;
new_screen* ScreenBuff = NULL;


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

//*-----------------------------------------------------*/
//new_touchscreen* TouchScreen = NULL;
//new_screen* ScreenBuff = NULL;
/*-----------------------------------------------------*/
//SD_Struct_t SD_StructDisk1;
//SD_Struct_t SD_StructDisk2;
extern HAL_SD_CardInfoTypedef SDCardInfo;
/*#####################################################*/
bool board_init()
{
	//RtcStruct.Rtc_ClkSource = _Rtc_Clk_Source_RCOSC_gc;
	core_init();
	timer_init();
/*-----------------------------------------------------*/
/* Set up the Uart 0 like debug interface with RxBuff = 256, TxBuff = 256, 115200b/s*/
	INIT_UART(5, 921600, IOC, 6, IOC, 7);
	DebugCom = Uart[5];

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
/* Set up the Twi 0 to communicate with PMIC and the Onboard serial EEprom memory */
	INIT_TWI(0, 1000000, IOB, 8, IOB, 9, 256, 256);
/*-----------------------------------------------------*/
	//INIT_SPI(1, 25000000, 8, IOB, 10, IOC, 3, IOC, 2, IOG, 10);
/*-----------------------------------------------------*/
	LCD_BACKLIGHT = gpio_assign(IOD, 13, GPIO_DIR_OUTPUT, false);
	ScreenBuff = new_(new_screen);
	ScreenBuff->raster_timings = &lcd_S035Q01_beaglebone_exp;
	ScreenBuff->BackLight = LCD_BACKLIGHT;
	screen_init(ScreenBuff);
	UARTprintf(DebugCom, "LCD display initialize successful for %dx%d resolution, %d Bit bus.\n\r" , ScreenBuff->raster_timings->X, ScreenBuff->raster_timings->Y, ScreenBuff->raster_timings->bus_size);
/*-----------------------------------------------------*/
	INIT_MPU60x0_9150(MPU60x0_9150, 0);
/*-----------------------------------------------------*/
	INIT_AK8975(AK8975, 0);
/*-----------------------------------------------------*/
	INIT_BMP180(BMP180, 0);
/*-----------------------------------------------------*/
	//INIT_SHT11(SHT11, IOB, 12, IOB, 13);
/*-----------------------------------------------------*/
	//INIT_ADXL345(ADXL345, 0);
/*-----------------------------------------------------*/
	INIT_HIH613x(HIH613x, 0);
/*-----------------------------------------------------*/
	INIT_MPL3115A2(MPL3115A2, 0);
/*-----------------------------------------------------*/
	//INIT_MPR121(MPR121, 0);
/*-----------------------------------------------------*/
	//INIT_LEPTON_FLIR(LEPTON_FLIR, 1, 0);
/*-----------------------------------------------------*/
#if defined(HARDBTN1_PORT) && defined(HARDBTN1_PIN)
	HARDBTN[0] = gpio_assign(HARDBTN1_PORT, HARDBTN1_PIN, GPIO_DIR_INPUT, false);
	gpio_up_dn(HARDBTN[0], 1);
#endif
#if defined(HARDBTN2_PORT) && defined(HARDBTN2_PIN)
	HARDBTN[1] = gpio_assign(HARDBTN2_PORT, HARDBTN2_PIN, GPIO_DIR_INPUT, false);
	gpio_up_dn(HARDBTN[1], 1);
#endif
#if defined(HARDBTN3_PORT) && defined(HARDBTN3_PIN)
	HARDBTN[2] = gpio_assign(HARDBTN3_PORT, HARDBTN3_PIN, GPIO_DIR_INPUT, false);
	gpio_up_dn(HARDBTN[2], 1);
#endif
#if defined(HARDBTN4_PORT) && defined(HARDBTN4_PIN)
	HARDBTN[3] = gpio_assign(HARDBTN4_PORT, HARDBTN4_PIN, GPIO_DIR_INPUT, false);
	gpio_up_dn(HARDBTN[3], 1);
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
	INIT_MMCSD(0, NULL, LED[0]);
/*-----------------------------------------------------*/
	return true;
}
