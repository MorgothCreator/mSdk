/*
 * board_init.c
 *
 * Created: 2/16/2013 9:33:48 PM
 *  Author: XxXx
 */ 

#include <stdbool.h>
#include "board_init.h"
#include "board_properties.h"
#include "sys/core_init.h"
#include "api/timer_api.h"
#include "api/gpio_api.h"
#include "api/gpio_def.h"
#include "api/uart_api.h"
#include "api/uart_def.h"
//#include "api/twi_api.h"
//#include "api/twi_def.h"
#include "api/lcd_def.h"
//#include "api/lcd_api.h"
#include "interface/adc_interface.h"
#include "device/ph7_62_matrix_led.h"
/*#####################################################*/
new_uart* DebugCom = NULL;
new_uart* Uart1 = NULL;
new_uart* Uart2 = NULL;
new_uart* Uart3 = NULL;
new_twi* TWI0 = NULL;
new_gpio* LED1 = NULL;
new_gpio* LED2 = NULL;
new_gpio* LED3 = NULL;
new_gpio* LED4 = NULL;
new_gpio* LED5 = NULL;

new_screen* ph_7_62_ScreenBuff = NULL;
new_ph7_62 *ph_7_62_settings = NULL;
/*#####################################################*/
bool board_init(void)
{
	core_init();
	timer_init();
/*-----------------------------------------------------*/
/* Set up the Uart 0 like debug interface with RxBuff = 256, TxBuff = 256, 115200b/s*/
	DebugCom = new_(new_uart);
	DebugCom->BaudRate = 115200;
	DebugCom->RxPin = 1;
	DebugCom->TxPin = 1;
	DebugCom->rxFifoTrigLevel = 1;
	DebugCom->txFifoTrigLevel = 1;
	DebugCom->UartNr = 2;
	uart_open(DebugCom);
/*-----------------------------------------------------*/
/* Display board message*/
#if defined(BOARD_MESSAGE)
	UARTprintf(DebugCom, "Use %s Board.\n\r", BOARD_MESSAGE);
#endif
/*-----------------------------------------------------*/
/* Set up the Twi 1 to communicate with PMIC and the Onboard serial EEprom memory */
	/*UARTPuts(DebugCom, "Setup TWI 0 with RxBuff = 258, TxBuff = 258....." , -1);
	TWI0 = new_(new_twi);
	TWI0->SdaPin = 1;
	TWI0->SclPin = 3;
	TWI0->BaudRate = 100000;
	TWI0->TwiNr = 1;
	TWI0->Priority = 0;
	//TWI0->WithInterrupt = true;
	TWI0->RxBuffSize = 258;
	TWI0->TxBuffSize = 258;
	twi_open(TWI0);
	UARTPuts(DebugCom, "OK.\n\r" , -1);*/
/*-----------------------------------------------------*/
	LED1 = gpio_assign(0, 11, GPIO_DIR_OUTPUT);
	LED2 = gpio_assign(1,  4, GPIO_DIR_OUTPUT);
	//LED3 = gpio_assign(0, 13, GPIO_DIR_OUTPUT);
	//LED4 = gpio_assign(0, 17, GPIO_DIR_OUTPUT);
	//LED5 = gpio_assign(0, 20, GPIO_DIR_OUTPUT);
/*-----------------------------------------------------*/
	adc_init(1 << 6, AVR32_ADCIFB_TRGR_TRGMOD_CM, 0);
/*-----------------------------------------------------*/
	new_ph7_62 *ph_7_62_settings = new_(new_ph7_62);
	ph_7_62_settings->RGB_Port = ph7_62_RGB_Port;
	ph_7_62_settings->B1_PinMask = ph7_62_B1_PinMask;
	ph_7_62_settings->B2_PinMask = ph7_62_B2_PinMask;
	ph_7_62_settings->G1_PinMask = ph7_62_G1_PinMask;
	ph_7_62_settings->G2_PinMask = ph7_62_G2_PinMask;
	ph_7_62_settings->R1_PinMask = ph7_62_R1_PinMask;
	ph_7_62_settings->R2_PinMask = ph7_62_R2_PinMask;
	ph_7_62_settings->B1_PinNr = ph7_62_B1_PinPosition;
	ph_7_62_settings->B2_PinNr = ph7_62_B2_PinPosition;
	ph_7_62_settings->G1_PinNr = ph7_62_G1_PinPosition;
	ph_7_62_settings->G2_PinNr = ph7_62_G2_PinPosition;
	ph_7_62_settings->R1_PinNr = ph7_62_R1_PinPosition;
	ph_7_62_settings->R2_PinNr = ph7_62_R2_PinPosition;
	ph_7_62_settings->Sclk_PinNr = ph7_62_Sclk_PinPosition;
	ph_7_62_settings->Sclk_PinMask = ph7_62_Sclk_PinMask;
	ph_7_62_settings->Sclk_Port = ph7_62_Sclk_Port;
	ph_7_62_settings->Latch_PinNr = ph7_62_Latch_PinPosition;
	ph_7_62_settings->Latch_PinMask = ph7_62_Latch_PinMask;
	ph_7_62_settings->Latch_Port = ph7_62_Latch_Port;
	ph_7_62_settings->Oe_PinNr = ph7_62_Oe_PinPosition;
	ph_7_62_settings->Oe_PinMask = ph7_62_Oe_PinMask;
	ph_7_62_settings->Oe_Port = ph7_62_Oe_Port;
	ph_7_62_settings->ABC_PinPosition = ph7_62_ABC_PinPosition;
	ph_7_62_settings->ABC_Port = ph7_62_ABC_Port;
	ph_7_62_settings->UsedTimerChanel = 1;
	ph_7_62_settings->UsedTimerNr = 0;
	ph_7_62_settings->PanelsNr = ph7_62_PanelsNr;
	ph_7_62_settings->ColorNr = 16;
	ph_7_62_settings->FpsNr = 60;

	ph_7_62_ScreenBuff = new_(new_screen);
	ph_7_62_ScreenBuff->Height = ph7_62_Height;
	ph_7_62_ScreenBuff->Width = ph7_62_Width;
	ph_7_62_ScreenBuff->UserData = ph_7_62_settings;
	ph7_62_init(ph_7_62_ScreenBuff);
	screen_clear(ph_7_62_ScreenBuff, 0x0000);
	return true;
}	
