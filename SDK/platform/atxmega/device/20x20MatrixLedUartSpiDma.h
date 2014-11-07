/*
 * _20x20MatrixLedUartSpiDma.h
 *
 * Created: 2/21/2013 9:23:49 AM
 *  Author: XxXx
 */ 
//#######################################################################################
#ifndef _20X20MATRIXLEDUARTSPIDMA_H_
#define _20X20MATRIXLEDUARTSPIDMA_H_
//#######################################################################################
#include "api/lcd_def.h"

#ifndef SCS_Port
#define SCS_Port	PORTA
#endif

#ifndef SCS_bm
#define SCS_bm		0
#endif

#ifndef SCLR_Port
#define SCLR_Port	PORTA
#endif

#ifndef SCLR_bm
#define SCLR_bm		0
#endif

//#######################################################################################
void screen_open(tDisplay* LcdStruct);
void screen_put_pixel16(tDisplay* LcdStruct, signed int X_Coordonate, signed int Y_Coordonate, unsigned int Color16);
void screen_fill_area(tDisplay* LcdStruct, signed int x1, signed int y1, signed int x2, signed int y2, unsigned int color);
void screen_clear(tDisplay* LcdStruct, unsigned int color);
void screen_draw_rectangle(tDisplay* LcdStruct, signed int x1, signed int y1, signed int x2, signed int y2, bool fill, unsigned int color);
void screen_put_horizontal_line(tDisplay *LcdStruct, signed int X1, signed int X2, signed int Y, unsigned char width, unsigned int color);
void screen_put_vertical_line(tDisplay *LcdStruct, signed int Y1, signed int Y2, signed int X, unsigned char width, unsigned int color);
//#######################################################################################
#ifdef HEADER_INCLUDE_C_FILES
#include "20x20MatrixLedUartSpiDma.c"
#endif
//#######################################################################################
#endif /* 20X20MATRIXLEDUARTSPIDMA_H_ */
//#######################################################################################
