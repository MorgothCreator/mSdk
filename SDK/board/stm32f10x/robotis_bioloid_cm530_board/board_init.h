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
#include "stm32f10x_conf.h"
#include "sys/system_stm32f10x.h"
#include "api/core_init_api.h"
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
//#include "device/ar1020.h"
//#include "driver/dma.h"
//#include "lib/fs/fat.h"
//#include "device/mi0283.h"
/*#####################################################*/
#define BOARD_MESSAGE "STM32F10x undefined"
/*#####################################################*/
extern new_uart* DebugCom;
extern new_twi* TWI[2];
extern new_adc* ADC[2];
/*-----------------------------------------------------*/
/* Led pins */
extern new_gpio *SW_UP;
extern new_gpio *SW_DOWN;
extern new_gpio *SW_RIGHT;
extern new_gpio *SW_LEFT;
extern new_gpio *SW_START;
/*-----------------------------------------------------*/
/* Switch pins */
extern new_gpio *LED_AUX;
extern new_gpio *LED_MANAGE;
extern new_gpio *LED_PROGRAM;
extern new_gpio *LED_PLAY;
extern new_gpio *LED_POWER;
extern new_gpio *LED_TX;
extern new_gpio *LED_RX;
/*-----------------------------------------------------*/
/* ADC mux select pins */
extern new_gpio *ADC_SELECT[2];
/*-----------------------------------------------------*/
extern new_gpio *DXL_EN_RX;
extern new_gpio *DXL_EN_TX;
extern new_uart* DXL_COM;
/*-----------------------------------------------------*/
extern new_gpio *ZBEE_RESET;
extern new_uart* ZBEE_COM;
/*-----------------------------------------------------*/
//extern new_touchscreen* TouchScreen;
//extern new_screen* ScreenBuff;
/*-----------------------------------------------------*/
//extern SD_Struct_t SD_StructDisk1;
//extern SD_Struct_t SD_StructDisk2;
/*#####################################################*/
bool board_init();
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "board_init.c"
#endif
/*#####################################################*/
/*#####################################################*/
#endif /* BOARD_INIT_H_ */
/*#####################################################*/
