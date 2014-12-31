/*
 * board_init.c
 *
 * Created: 2/8/2013 12:22:54 AM
 *  Author: XxXx
 */

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
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
/*#####################################################*/
new_uart* DebugCom = NULL;
new_twi* TWI[2] = {NULL,NULL};
new_adc* ADC[2] = {NULL};
/*-----------------------------------------------------*/
/* Led pins */
new_gpio *SW_UP = NULL;
new_gpio *SW_DOWN = NULL;
new_gpio *SW_RIGHT = NULL;
new_gpio *SW_LEFT = NULL;
new_gpio *SW_START = NULL;
/*-----------------------------------------------------*/
/* Switch pins */
new_gpio *LED_AUX = NULL;
new_gpio *LED_MANAGE = NULL;
new_gpio *LED_PROGRAM = NULL;
new_gpio *LED_PLAY = NULL;
new_gpio *LED_POWER = NULL;
new_gpio *LED_TX = NULL;
new_gpio *LED_RX = NULL;
/*-----------------------------------------------------*/
/* ADC mux select pins */
new_gpio *ADC_SELECT[2] = {NULL,NULL};
/*-----------------------------------------------------*/
new_gpio *DXL_EN_RX = NULL;
new_gpio *DXL_EN_TX = NULL;
new_uart* DXL_COM = NULL;
/*-----------------------------------------------------*/
new_gpio *ZBEE_RESET = NULL;
new_uart* ZBEE_COM = NULL;
/*-----------------------------------------------------*/
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
/* Set up the Uart 0 like debug interface with RxBuff = 256, TxBuff = 256, 115200b/s*/
	DebugCom = new_(new_uart);
	DebugCom->BaudRate = 115200;
	DebugCom->RxBuffSize = 20;
	DebugCom->TxBuffSize = 10;
	DebugCom->TxPort = IOB;
	DebugCom->RxPort = IOB;
	DebugCom->TxPin = 10;
	DebugCom->RxPin = 11;
	//Uart[0]->Mode = UsartCom_Mode_Asynchronus;
	DebugCom->Priority = 0;
	DebugCom->UartNr = 2;
	uart_open(DebugCom);
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
	TWI[0]->TwiNr = 1;
	TWI[0]->Priority = 0;
	//TWI[0]->WithInterrupt = TRUE;
	TWI[0]->RxBuffSize = 258;
	TWI[0]->TxBuffSize = 258;
	TWI[0]->BusyTimeOut = 1000;
	twi_open(TWI[0]);
	UARTPuts(DebugCom, "OK.\n\r" , -1);
/*-----------------------------------------------------*/
/* Set up the ADC 0 (VDD VOLT and INT TEMPERATURE)*/
	UARTPuts(DebugCom, "Setup ADC 0....." , -1);
	ADC[0] = new_(new_adc);
	ADC[0]->Align = ADC_ALIGN_RIGHT;
	ADC[0]->ContinuousMode = ADC_CONTINUOUS_MODE_CONTINUOUS;
	ADC[0]->ConvMode = ADC_CONV_MODE_MULTICHANNEL;
	ADC[0]->DmaChannel = 0;
	ADC[0]->DmaUnitNr = 0;
	ADC[0]->ExtTrig = ADC_EXT_TRIG_NONE;
	ADC[0]->IntRefEn = true;
	ADC[0]->Mode = ADC_MODE_INDEPENDENT;
	ADC[0]->TempSensorEn = true;
	ADC[0]->UnitNr = 0;
	ADC[0]->EnCh[0] = 1; // SIG ADC0
	ADC[0]->EnCh[1] = 10;// SIG ADC1
	ADC[0]->SampleTime[0] = ADC_SAMPLE_TIME_5;
	ADC[0]->SampleTime[1] = ADC_SAMPLE_TIME_5;
	if(adc_init(ADC[0])) UARTPuts(DebugCom, "OK.\n\r" , -1);
	else  UARTPuts(DebugCom, "FAILED.\n\r" , -1);
/*-----------------------------------------------------*/
/* Set up the ADC 1 (VDD VOLT and INT TEMPERATURE)*/
	UARTPuts(DebugCom, "Setup ADC 1....." , -1);
	ADC[1] = new_(new_adc);
	ADC[1]->Align = ADC_ALIGN_RIGHT;
	ADC[1]->ContinuousMode = ADC_CONTINUOUS_MODE_CONTINUOUS;
	ADC[1]->ConvMode = ADC_CONV_MODE_MULTICHANNEL;
	ADC[1]->DmaChannel = 1;
	ADC[1]->DmaUnitNr = 0;
	ADC[1]->ExtTrig = ADC_EXT_TRIG_NONE;
	ADC[1]->IntRefEn = true;
	ADC[1]->Mode = ADC_MODE_INDEPENDENT;
	ADC[1]->TempSensorEn = true;
	ADC[1]->UnitNr = 0;
	ADC[1]->EnCh[0] = 2;// VDD VOLT
	ADC[1]->EnCh[1] = 17;// INT TEMPERATURE
	ADC[1]->SampleTime[0] = ADC_SAMPLE_TIME_5;
	ADC[1]->SampleTime[1] = ADC_SAMPLE_TIME_6;
	if(adc_init(ADC[0])) UARTPuts(DebugCom, "OK.\n\r" , -1);
	else  UARTPuts(DebugCom, "FAILED.\n\r" , -1);
/*-----------------------------------------------------*/
	/* Setup led pins */
	LED_AUX = 		gpio_assign(IOB, 12, GPIO_DIR_OUTPUT, false);
	LED_MANAGE = 	gpio_assign(IOB, 13, GPIO_DIR_OUTPUT, false);
	LED_PROGRAM = 	gpio_assign(IOB, 14, GPIO_DIR_OUTPUT, false);
	LED_PLAY = 		gpio_assign(IOB, 15, GPIO_DIR_OUTPUT, false);
	LED_POWER = 	gpio_assign(IOC, 13, GPIO_DIR_OUTPUT, false);
	LED_TX = 		gpio_assign(IOC, 14, GPIO_DIR_OUTPUT, false);
	LED_RX = 		gpio_assign(IOC, 15, GPIO_DIR_OUTPUT, false);
/*-----------------------------------------------------*/
	/* Setup switch pins */
	SW_UP = 		gpio_assign(IOC, 11, GPIO_DIR_INPUT, false);
	SW_DOWN = 		gpio_assign(IOC, 10, GPIO_DIR_INPUT, false);
	SW_RIGHT = 		gpio_assign(IOA, 14, GPIO_DIR_INPUT, false);
	SW_LEFT = 		gpio_assign(IOA, 15, GPIO_DIR_INPUT, false);
	SW_START = 		gpio_assign(IOB,  3, GPIO_DIR_INPUT, false);
/*-----------------------------------------------------*/
	/* Setup ADC mux select pins */
	ADC_SELECT[0] = gpio_assign(IOC,  1, GPIO_DIR_INPUT, false);
	ADC_SELECT[1] = gpio_assign(IOC,  2, GPIO_DIR_INPUT, false);
/*-----------------------------------------------------*/
	/* Setup DXL RX-TX Enable pins */
	DXL_EN_RX = 	gpio_assign(IOB,  5, GPIO_DIR_OUTPUT, false);
	DXL_EN_TX = 	gpio_assign(IOB,  4, GPIO_DIR_OUTPUT, false);
	/* Setup DXL Uart */
	DXL_COM = new_(new_uart);
	DXL_COM->BaudRate = 1000000;
	DXL_COM->TxPort = IOB;
	DXL_COM->RxPort = IOB;
	DXL_COM->TxPin = 6;
	DXL_COM->RxPin = 7;
	DXL_COM->Priority = 0;
	DXL_COM->UartNr = 1;
	uart_open(DXL_COM);
/*-----------------------------------------------------*/
	/* Setup DXL RX-TX Enable pins */
	ZBEE_RESET = 	gpio_assign(IOA,  12, GPIO_DIR_OUTPUT, false);
	/* Setup DXL Uart */
	ZBEE_COM = new_(new_uart);
	ZBEE_COM->BaudRate = 115200;
	ZBEE_COM->TxPort = IOB;
	ZBEE_COM->RxPort = IOB;
	ZBEE_COM->TxPin = 6;
	ZBEE_COM->RxPin = 7;
	ZBEE_COM->Priority = 0;
	ZBEE_COM->UartNr = 1;
	uart_open(ZBEE_COM);
/*-----------------------------------------------------*/
	return true;
}
