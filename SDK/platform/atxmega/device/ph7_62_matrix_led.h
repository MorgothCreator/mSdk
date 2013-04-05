/*
 * ph7_62_matrix_led.h
 *
 * Created: 2/15/2013 11:29:11 PM
 *  Author: XxXx
 */ 
//#####################################################
#ifndef PH7_62_MATRIX_LED_H_
#define PH7_62_MATRIX_LED_H_
//#####################################################
#include "api/lcd_def.h"
//#####################################################
typedef struct  
{
	unsigned char R;
	unsigned char G;
	unsigned char B;
}ph7_62_RGB_t;
//#####################################################
typedef struct  
{
	unsigned int WidthCnt;
	unsigned int HeightCnt;
	//TC0_t *TimerBaseAddr;
	ph7_62_RGB_t *DisplayData;
	unsigned char UsedTimerNr;
	unsigned char UsedTimerChanel;
	PORT_t *Sclk_Port;
	unsigned char Sclk_PinMask;
	PORT_t *Oe_Port;
	unsigned char Oe_PinMask;
	PORT_t *Latch_Port;
	unsigned char Latch_PinMask;
	PORT_t *ABC_Port;
	unsigned char ABC_PinPosition;
	PORT_t *RGB_Port;
	unsigned char R1_PinMask;
	unsigned char R2_PinMask;
	unsigned char G1_PinMask;
	unsigned char G2_PinMask;
	unsigned char B1_PinMask;
	unsigned char B2_PinMask;
	unsigned char PanelsNr;
	unsigned char ColorNr;
	unsigned char ColorCnt;
	unsigned char V_Cnt;
	unsigned char V_Cnt_Limit;
	unsigned char RGB_Mask;
	unsigned char FpsNr;
}ph7_62_t;
//#####################################################
#define new_ph7_62 ph7_62_t
#ifndef new_
#define new_(structure) (structure*)calloc(1,sizeof(structure));
#endif
#define free_ph7_62(address) free(address);
//#####################################################
bool ph7_62_init(tDisplay *pDisplay);
//#####################################################
#ifdef HEADER_INCLUDE_C_FILES
#include "ph7_62_matrix_led.c"
#endif
//#####################################################
#endif /* PH7_62_MATRIX_LED_H_ */
//#####################################################
