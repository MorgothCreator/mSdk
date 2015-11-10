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
//#include "api/lcd_def.h"
//#include "api/lcd_api.h"
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
new_adc* ADC[2] = {NULL, NULL};
new_gpio* LED[LEDS_NR] = {NULL};
new_gpio* HARDBTN[HARDBTNS_NR] = {NULL};
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
//*-----------------------------------------------------*/
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
/* Set up the Uart 0 like debug interface with RxBuff = 256, TxBuff = 256, 115200b/s*/
	UART_INIT(5, 921600, IOC, 6, IOC, 7);
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
	TWI_INIT(0, 400000, IOB, 8, IOB, 9);
/*-----------------------------------------------------*/
	SPI_INIT(1, IOB, 10, IOC, 3, IOC, 2, IOG, 10);
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
	mmcsd_init((void *)&SDCardInfo, (Gpio_t *)NULL, LED[0]);
	mmcsd_idle((void *)&SDCardInfo);
/*-----------------------------------------------------*/
	return true;
}
