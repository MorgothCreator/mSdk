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
	unsigned int Sclk_Port;
	unsigned long Sclk_PinMask;
	unsigned long Sclk_PinNr;
	unsigned int  Oe_Port;
	unsigned long Oe_PinMask;
	unsigned long Oe_PinNr;
	unsigned int  Latch_Port;
	unsigned long Latch_PinMask;
	unsigned long Latch_PinNr;
	unsigned int  ABC_Port;
	unsigned long ABC_PinPosition;
	unsigned int  RGB_Port;
	unsigned long R1_PinMask;
	unsigned long R2_PinMask;
	unsigned long G1_PinMask;
	unsigned long G2_PinMask;
	unsigned long B1_PinMask;
	unsigned long B2_PinMask;
	unsigned long R1_PinNr;
	unsigned long R2_PinNr;
	unsigned long G1_PinNr;
	unsigned long G2_PinNr;
	unsigned long B1_PinNr;
	unsigned long B2_PinNr;
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
void screen_put_pixel16(tDisplay* LcdStruct, signed int X_Coordonate, signed int Y_Coordonate, unsigned int Color16);
void screen_fill_area(tDisplay* LcdStruct, signed int x1, signed int y1, signed int x2, signed int y2, unsigned int color);
void screen_clear(tDisplay* LcdStruct, unsigned int color);
void screen_draw_rectangle(tDisplay* LcdStruct, signed int x1, signed int y1, signed int x2, signed int y2, bool fill, unsigned int color);
void screen_put_horizontal_line(tDisplay *LcdStruct, signed int X1, signed int X2, signed int Y, unsigned char width, unsigned int color);
void screen_put_vertical_line(tDisplay *LcdStruct, signed int Y1, signed int Y2, signed int X, unsigned char width, unsigned int color);
//#####################################################
#include "ph7_62_matrix_led.c"
//#####################################################
#endif /* PH7_62_MATRIX_LED_H_ */
//#####################################################
