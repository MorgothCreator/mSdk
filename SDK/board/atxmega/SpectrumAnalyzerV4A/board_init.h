/*
 * board_init.h
 *
 * Created: 2/8/2013 12:22:37 AM
 *  Author: XxXx
 */ 
/*#####################################################*/
#ifndef BOARD_INIT_H_
#define BOARD_INIT_H_
/*#####################################################*/
#include <stdbool.h>
#include <string.h>
#include "board_init.h"
#include "api/core_init_api.h"
#include "api/gpio_def.h"
#include "api/gpio_api.h"
#include "api/uart_def.h"
#include "api/uart_api.h"
#include "api/twi_def.h"
#include "api/twi_api.h"
#include "api/lcd_def.h"
#include "api/lcd_api.h"
#include "driver/dma.h"
#include "driver/adc.h"
/*#####################################################*/
//#define BOARD_MESSAGE "SpectrumAnalyzerV4.0A"
/*#####################################################*/
extern new_uart* DebugCom;
extern new_uart* Uart1;
extern new_twi* TWI0;
extern new_adc *ADC1;
extern new_adc_ch *ADC1_CH0;
extern new_adc_ch *ADC1_CH1;
extern new_gpio* HARDBTNOK;
extern new_gpio* HARDBTN2;
extern new_gpio* HARDBTN3;
/*-----------------------------------------------------*/
extern new_dma_gen* Dma;
/*-----------------------------------------------------*/
extern new_screen* ScreenBuff;
/*#####################################################*/
bool board_init();
/*#####################################################*/
#include "board_init.c"
/*#####################################################*/
/*#####################################################*/
#endif /* BOARD_INIT_H_ */
/*#####################################################*/
