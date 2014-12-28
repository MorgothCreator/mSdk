/*
 * board_init.c
 *
 * Created: 2/8/2013 12:22:54 AM
 *  Iulian Gheorghiu <morgoth.creator@gmail.com>
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
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
#include "api/adc_def.h"
#include "api/adc_api.h"
//#include "api/lcd_def.h"
//#include "api/lcd_api.h"
//#include "api/mmcsd_api.h"
//#include "lib/gfx/controls_definition.h"
//#include "lib/fs/fat.h"
//#include "device/mi0283.h"
/*#####################################################*/
new_uart* Uart[6] = {NULL,NULL,NULL,NULL,NULL,NULL};
new_uart* DebugCom = NULL;
new_twi* TWI[2] = {NULL,NULL};
new_adc* ADC[2] = {NULL, NULL};
new_gpio* LED[4] = {NULL,NULL,NULL,NULL};
new_gpio* HARDBTN1 = NULL;
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
	Uart[5] = new_(new_uart);
	Uart[5]->BaudRate = 115200;
	Uart[5]->RxBuffSize = 20;
	Uart[5]->TxBuffSize = 10;
	//DebugCom->Mode = UsartCom_Mode_Asynchronus;
	Uart[5]->Priority = 0;
	Uart[5]->UartNr = 5;
	uart_open(Uart[5]);
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
	UARTPuts(DebugCom, "Setup TWI 0 with RxBuff = 258, TxBuff = 258....." , -1);
	TWI[0] = new_(new_twi);
	TWI[0]->BaudRate = 100000;
	TWI[0]->TwiNr = 0;
	TWI[0]->Priority = 0;
		//TWI[0]->WithInterrupt = TRUE;
	TWI[0]->RxBuffSize = 258;
	TWI[0]->TxBuffSize = 258;
	TWI[0]->BusyTimeOut = 1000;
	twi_open(TWI[0]);
	UARTPuts(DebugCom, "OK.\n\r" , -1);
/*-----------------------------------------------------*/
/* Set up the ADC 0 */
	UARTPuts(DebugCom, "Setup ADC 0....." , -1);
	ADC[0] = new_(new_adc);
	ADC[0]->Align = ADC_ALIGN_RIGHT;
	ADC[0]->ContinuousMode = ADC_CONTINUOUS_MODE_CONTINUOUS;
	ADC[0]->ConvMode = ADC_CONV_MODE_MULTICHANNEL;
	ADC[0]->DmaChannel = 0;
	ADC[0]->DmaUnitNr = 0;
	ADC[0]->EnCh[0] = 1;
	ADC[0]->EnCh[1] = 2;
	ADC[0]->EnCh[2] = 17;
	ADC[0]->ExtTrig = ADC_EXT_TRIG_NONE;
	ADC[0]->ExtTrigEdge = ADC_EXT_TRIG_ADGE_NONE;
	ADC[0]->IntRefEn = true;
	ADC[0]->Mode = ADC_MODE_INDEPENDENT;
	ADC[0]->SampleTime[0] = ADC_SAMPLE_TIME_5;
	ADC[0]->SampleTime[1] = ADC_SAMPLE_TIME_5;
	ADC[0]->SampleTime[2] = ADC_SAMPLE_TIME_6;
	ADC[0]->TempSensorEn = true;
	ADC[0]->UnitNr = 0;
	ADC[0]->Prescaller = 2;
	if(adc_init(ADC[0])) UARTPuts(DebugCom, "OK.\n\r" , -1);
	else  UARTPuts(DebugCom, "FAILED.\n\r" , -1);
/*-----------------------------------------------------*/
	HARDBTN1 = gpio_assign(0, 1, GPIO_DIR_INPUT, false);
	gpio_up_dn(HARDBTN1, 1);
/*-----------------------------------------------------*/
	LED[0] = gpio_assign(3, 12, GPIO_DIR_OUTPUT, false);
	LED[1] = gpio_assign(3, 13, GPIO_DIR_OUTPUT, false);
	LED[2] = gpio_assign(3, 14, GPIO_DIR_OUTPUT, false);
	LED[3] = gpio_assign(3, 15, GPIO_DIR_OUTPUT, false);
//*-----------------------------------------------------*/
	return true;
}
