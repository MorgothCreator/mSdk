/*
 * board_init.c
 *
 * Created: 2/8/2013 12:22:54 AM
 *  Author: XxXx
 */ 

#include <stdbool.h>
#include <string.h>
#include "board_init.h"
#include "api/core_init_api.h"
#include "api/timer_api.h"
#include "api/gpio_def.h"
#include "api/gpio_api.h"
#include "api/uart_def.h"
#include "api/uart_api.h"
#include "api/twi_def.h"
#include "api/twi_api.h"
#include "device/20x20MatrixLedUartSpiDma.h"
#include "api/lcd_def.h"
#include "api/lcd_api.h"
#include "driver/dma.h"
#include "driver/adc.h"
#include "device/ds1307.h"
/*#####################################################*/
new_uart* DebugCom = NULL;
new_uart* Uart1 = NULL;
new_twi* TWI0 = NULL;
new_adc *ADC1 = NULL;
new_adc_ch *ADC1_CH0 = NULL;
new_adc_ch *ADC1_CH1 = NULL;
new_gpio* HARDBTNOK = NULL;
new_gpio* HARDBTN2 = NULL;
new_gpio* HARDBTN3 = NULL;
new_gpio* USBCONNECT = NULL;
new_gpio* STBYSIGNAL = NULL;
/*-----------------------------------------------------*/
new_dma_gen* Dma = NULL;
/*-----------------------------------------------------*/
new_screen* ScreenBuff = NULL;
extern void adc0_ch0_callback(void*data);
extern void adc0_ch1_callback(void*data);
/*#####################################################*/
bool board_init()
{
	RtcStruct.Rtc_ClkSource = _Rtc_Clk_Source_RCOSC_gc;
	core_init();
	timer_init();
/*-----------------------------------------------------*/
/* Set up the Uart 0 like debug interface with RxBuff = 256, TxBuff = 256, 115200b/s*/
	DebugCom = new_(new_uart);
	DebugCom->BaudRate = 19200;
	DebugCom->RxBuffSize = 20;
	DebugCom->TxBuffSize = 10;
	DebugCom->Mode = UsartCom_Mode_Asynchronus;
	DebugCom->Priority = Usart_Int_Med_Level;
	DebugCom->UartNr = 4;
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
	//UARTPuts(DebugCom, "Setup TWI 0 with RxBuff = 258, TxBuff = 258 at 100000b/s....." , -1);
	TWI0 = new_(new_twi);
	TWI0->BaudRate = 100000;
	TWI0->TwiNr = 0;
	TWI0->Priority = TWI_Int_Med_Level;
	TWI0->RxBuffSize = 20;
	TWI0->TxBuffSize = 20;
	twi_open(TWI0);
	//UARTPuts(DebugCom, "OK.\n\r" , -1);
/*-----------------------------------------------------*/
	SCLR_Port.DIRSET = SCLR_bm;
	SCLR_Port.OUTSET = SCLR_bm;
	SCS_Port.DIRSET = SCS_bm;
	SCS_Port.OUTSET = SCS_bm;
/*-----------------------------------------------------*/
	HARDBTNOK = gpio_assign(BTN1_Port, BTN1_bp, GPIO_DIR_INPUT);
	gpio_up_dn(HARDBTNOK, 1);
	HARDBTN2 = gpio_assign(BTN2_Port, BTN2_bp, GPIO_DIR_INPUT);
	gpio_up_dn(HARDBTN2, 1);
	HARDBTN3 = gpio_assign(BTN3_Port, BTN3_bp, GPIO_DIR_INPUT);
	gpio_up_dn(HARDBTN3, 1);
	USBCONNECT = gpio_assign(USB_CONNECT_Port, USB_CONNECT_bp, GPIO_DIR_INPUT);
	gpio_up_dn(USBCONNECT, 0);
	STBYSIGNAL = gpio_assign(PWR_STBY_Port, PWR_STBY_bp, GPIO_DIR_OUTPUT);
	gpio_out(STBYSIGNAL, 0);
/*-----------------------------------------------------*/
	Dma = new_(new_dma_gen);
	dma_enable(Dma);
/*-----------------------------------------------------*/
	ScreenBuff = new_(new_screen);
	//ScreenBuff->Orientation = 90;
	ScreenBuff->Height = 20;
	ScreenBuff->Width = 20;
	screen_init(ScreenBuff);
	screen_clear(ScreenBuff, 0x0000);
	//screen_draw_rectangle(ScreenBuff, 4, 5, 10, 10, true, 0xFFFF);
/*-----------------------------------------------------*/
	//UARTPuts(DebugCom, "\n\r" , -1);
/*-----------------------------------------------------*/
//-----------------------------------------------------
	ADC1 = new_(new_adc);
	ADC1->Adc_GenConvMode = _Adc_ConvMode_Signed;
	ADC1->Adc_GenConvRes = _Adc_ConvRes_8RightAdj;
	ADC1->Adc_GenFreeRun = _Adc_FreeRun_True;
	ADC1->Adc_GenPrescaller = _Adc_Prescaller_512;
	ADC1->Adc_GenRefSel = _Adc_RefSel_IntVcc;
	ADC1->Adc_GenChSweep = _Adc_ChSweep_01;
	//ADC1->Adc_GenBandgapEn = true;
	adc_gen_en(ADC1, 0, true);
//-----------------------------------------------------
	ADC1_CH0 = new_(new_adc_ch);
	ADC1_CH0->Adc_ChGain = _Adc_ChGain_16;
	ADC1_CH0->Adc_ChInMode = _Adc_ChInMode_DiffWGain;
	ADC1_CH0->Adc_ChInSrcNeg = _Adc_ChInSrcNeg_2;
	ADC1_CH0->Adc_ChInSrcPos = _Adc_ChInSrcPos_2;
	ADC1_CH0->Adc_ChIntLevel = _Adc_ChIntLevel_High;
	ADC1_CH0->Adc_ChIntMode = _Adc_ChIntMode_Complete;
	ADC1_CH0->CallbackData = ADC1;
	ADC1_CH0->ChannelCallback = adc0_ch0_callback;
	adc_ch_reg(ADC1, ADC1_CH0, 0);
//-----------------------------------------------------
	ADC1_CH1 = new_(new_adc_ch);
	ADC1_CH1->Adc_ChGain = _Adc_ChGain_16;
	ADC1_CH1->Adc_ChInMode = _Adc_ChInMode_DiffWGain;
	ADC1_CH1->Adc_ChInSrcNeg = _Adc_ChInSrcNeg_3;
	ADC1_CH1->Adc_ChInSrcPos = _Adc_ChInSrcPos_3;
	ADC1_CH1->Adc_ChIntLevel = _Adc_ChIntLevel_High;
	ADC1_CH1->Adc_ChIntMode = _Adc_ChIntMode_Complete;
	ADC1_CH1->CallbackData = ADC1;
	ADC1_CH1->ChannelCallback = adc0_ch1_callback;
	adc_ch_reg(ADC1, ADC1_CH1, 1);
//-----------------------------------------------------
	DS1307_Setup(TWI0);
	unsigned char RtcBuff[20];
	if(DS1307_ReadTime_Str(TWI0, RtcBuff)) UARTprintf(DebugCom, "%s\n\r" , RtcBuff);
	return true;
}