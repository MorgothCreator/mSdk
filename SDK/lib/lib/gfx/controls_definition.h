/*
 *  lib/gfx/controls_definition.h
 *
 *  Copyright (C) 2013  Iulian Gheorghiu <morgoth.creator@gmail.com>
 *
 *  This file is part of Multiplatform SDK.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CONTROLS_DEFINITION_H_
#define CONTROLS_DEFINITION_H_
/*#####################################################*/
#ifdef FLASH_DEVICE
#include <avr/pgmspace.h>
#endif
#include <stdbool.h>
#include <string.h>
#include "sys/cache.h"
#include "../../api/timer_api.h"
#include "interface/lcd_interface.h"
#include "api/lcd_def.h"
#include "lib/string_lib.h"
/*#####################################################*/
#define FONT_FMT_UNCOMPRESSED   0x00
#define ABSENT_CHAR_REPLACEMENT '.'
/*#####################################################*/
#ifndef INT_SIZE_IS_2_BYTES
//#if 4 == sizeof(unsigned int)
#define ReturnCommand_gp  (unsigned)(29)
#define ReturnCommand_gm  (unsigned)(0x07 << ReturnCommand_gp)
#define ReturnCommand_NoCommand  (unsigned)(0x00<<ReturnCommand_gp)
#define ReturnCommand_GoUp  (unsigned)(0x01<<ReturnCommand_gp)
#define ReturnCommand_GoDn  (unsigned)(0x02<<ReturnCommand_gp)
#define ReturnCommand_GoLeft  (unsigned)(0x03<<ReturnCommand_gp)
#define ReturnCommand_GoRight  (unsigned)(0x04<<ReturnCommand_gp)
#define ReturnCommand_MaxValue  (unsigned)(0x08)
#else
#define ReturnCommand_gp  (unsigned)(13)
#define ReturnCommand_gm  (unsigned)(0x07 << ReturnCommand_gp)
#define ReturnCommand_NoCommand  (unsigned)(0x00<<ReturnCommand_gp)
#define ReturnCommand_GoUp  (unsigned)(0x01<<ReturnCommand_gp)
#define ReturnCommand_GoDn  (unsigned)(0x02<<ReturnCommand_gp)
#define ReturnCommand_GoLeft  (unsigned)(0x03<<ReturnCommand_gp)
#define ReturnCommand_GoRight  (unsigned)(0x04<<ReturnCommand_gp)
#define ReturnCommand_MaxValue  (unsigned)(0x08)
#endif
/*#####################################################*/
#define BORDER_LINE_ONE_LIGHT		2
#define BORDER_LINE_TWO_LIGHT		1.5
#define BORDER_LINE_ONE_DARK		3
#define BORDER_LINE_TWO_DARK		2
/*#####################################################*/
typedef struct
{
	unsigned int Control_Color_Enabled_Ink_Pull;
	unsigned int Control_Color_Enabled_Ink_Push;
	unsigned int Control_Color_Enabled_Buton_Pull;
	unsigned int Control_Color_Enabled_Buton_Push;
	unsigned int Control_Color_Enabled_Border_Pull;
	unsigned int Control_Color_Enabled_Border_Push;
	unsigned int Control_Color_Enabled_BackGround;
	unsigned int Control_Color_Enabled_WindowBorder;
	unsigned int Control_Color_Enabled_WindowHeader;

	unsigned int Control_Color_Disabled_Ink_Pull;
	//unsigned int Control_Color_Disabled_Ink_Push;
	unsigned int Control_Color_Disabled_Buton_Pull;
	//unsigned int Control_Color_Disabled_Buton_Push;
	unsigned int Control_Color_Disabled_Border_Pull;
	//unsigned int Control_Color_Disabled_Border_Push;
	unsigned int Control_Color_Disabled_BackGround;
	unsigned int Control_Color_Disabled_WindowBorder;
	unsigned int Control_Color_Disabled_WindowHeader;
	unsigned int Scren;
	tFont* DefaultFont;
}controlscolor;
/*#####################################################*/
typedef enum
{
	Control_Nop,
	Control_Refresh,
	Control_Entire_Refresh,
	Control_Entire_Repaint,
	Control_Add_Children,
	Control_Init_Childrens,

	Control_Set_Cursor_Data,
	Control_Set_Position_X,
	Control_Set_Position_Y,
	Control_Set_Size_X,
	Control_Set_Size_Y,
	Control_Set_Caption_Text,
	Control_Set_Caption_Align,
	Control_Set_Font,
	Control_Set_WordWrap,
	Control_Set_Color_Enabled_Ink_Pull,
	Control_Set_Color_Enabled_Ink_Push,
	Control_Set_Color_Enabled_Buton_Pull,
	Control_Set_Color_Enabled_Buton_Push,
	Control_Set_Color_Enabled_Border_Pull,
	Control_Set_Color_Enabled_Border_Push,
	Control_Set_Color_Enabled_BackGround,

	Control_Set_Color_Disabled_Ink_Pull,
	Control_Set_Color_Disabled_Ink_Push,
	Control_Set_Color_Disabled_Buton_Pull,
	Control_Set_Color_Disabled_Buton_Push,
	Control_Set_Color_Disabled_Border,
	Control_Set_Color_Disabled_BackGround,
	Control_Set_Color_Disabled_ScalerOnDisabled,
	Control_Set_Color_Screen,
	Control_Set_Enabled,
	Control_Set_Visible,
	Control_Set_StateChangedOn,
	Control_Set_Value,
	Control_Set_MinimValue,
	Control_Set_MaximValue,

	Control_Set_CallBackOnDownData,
	Control_Set_CallBackOnDownFunction,
	Control_Set_CallBackOnUpData,
	Control_Set_CallBackOnUpFunction,
	Control_Set_CallBackOnMoveData,
	Control_Set_CallBackOnMoveFunction,
	Control_Set_CallBackOnValueChangedData,
	Control_Set_CallBackOnValueChangedFunction,
	Control_Set_CallBackOnCheckedData,
	Control_Set_CallBackOnCheckedFunction,
	Control_Set_CallBackOnUnCheckedData,
	Control_Set_CallBackOnUnCheckedFunction
}ControlCommands;
/*#####################################################*/
typedef enum
{
	Align_Left,
	Align_Center,
	Align_Right
}Align;
/*#####################################################*/
typedef enum
{
	Cursor_NoAction,
	Cursor_Up,
	Cursor_Down,
	Cursor_Move
}CursorState;
/*#####################################################*/
typedef struct
{
	signed int X;
	signed int Y;
	unsigned int Data;
	void* Settings;
	bool CursorCoordonateUsed;
	bool WindowRefresh;
	ControlCommands Comand;
	CursorState Cursor;
}tControlCommandData;
/*#####################################################*/
typedef struct
{
	struct
	{
		unsigned int Ink;
		unsigned int Buton;
		unsigned int Border;
		unsigned int BackGround;
		double ScalerOnDisabled;
	}Disabled;
	struct
	{
		struct
		{
			unsigned int Push;
			unsigned int Pull;
		}Ink;
		struct
		{
			unsigned int Push;
			unsigned int Pull;
		}Buton;
		struct
		{
			unsigned int Push;
			unsigned int Pull;
		}Border;
		unsigned int BackGround;
		double ScalerOnDisabled;
	}Enabled;
	unsigned int Scren;
}controls_color_struct_t;
/*#####################################################*/
typedef struct
{
	Align TextAlign;
	String_t *Text;
	bool WordWrap;
	tFont* Font;
}controls_caption_t;
/*#####################################################*/
/*#####################################################*/
/*#####################################################*/
/*#####################################################*/
extern controlscolor controls_color;
/*#####################################################*/
typedef struct
{
	struct
	{
		signed int X;
		signed int Y;
	}Position;
	struct
	{
		signed int X;
		signed int Y;
	}Size;
	struct
	{
		unsigned int Background;
		unsigned int Scren;
	}Color;
	bool Visible;
	bool Enabled;
	struct
	{
		unsigned int ItemsNumber;
		void** List;
	}Children;
	struct
	{
		struct
		{
			bool Initiated;
		}Control;
	}Internals;
}tPanel;
/*#####################################################*/
#define RGB_TO_UINT(R,G,B) ((R)<<16) | ((G)<<8) | ((B))
/*#####################################################*/
#ifdef FLASH_DEVICE
const unsigned char CharTable6x8[] PROGMEM =
#else
static const unsigned char CharTable6x8[] =
#endif
{
	6                          ,0          ,6          ,8          ,32            ,128,
/*  OffsetOfBeginingCharTable  ,0=Y-X|1=X-X,X-Dimension,Y-Dimension,BeginAsciiChar,EndAsciiChar*/
	0x00,0x00,0x00,0x00,0x00,0x00,
	0x5F,0x00,0x00,0x00,0x00,0x00,//   !		32,33
	0x07,0x00,0x07,0x00,0x00,0x00,
	0x14,0x7F,0x14,0x7F,0x14,0x00,// " #		34,35
	0x24,0x2A,0x7F,0x2A,0x12,0x00,
	0x23,0x13,0x08,0x64,0x62,0x00,// 0x %		36,37
	0x36,0x49,0x55,0x22,0x50,0x00,
	0x05,0x03,0x00,0x00,0x00,0x00,// & '		38,39
	0x1C,0x22,0x41,0x00,0x00,0x00,
	0x41,0x22,0x1C,0x00,0x00,0x00,// ( )		40,41
	0x08,0x2A,0x1C,0x2A,0x08,0x00,
	0x08,0x08,0x3E,0x08,0x08,0x00,// * +		42,43
	0x50,0x30,0x00,0x00,0x00,0x00,
	0x08,0x08,0x08,0x00,0x00,0x00,// , -		44,45
	0x30,0x30,0x00,0x00,0x00,0x00,
	0x20,0x10,0x08,0x04,0x02,0x00,// . /		46,47
	0x3E,0x51,0x49,0x45,0x3E,0x00,
	0x42,0x7F,0x40,0x00,0x00,0x00,// 0 1		48,49
	0x42,0x61,0x51,0x49,0x46,0x00,
	0x21,0x41,0x45,0x4B,0x31,0x00,// 2 3		50,51
	0x18,0x14,0x12,0x7F,0x10,0x00,
	0x27,0x45,0x45,0x45,0x39,0x00,// 4 5		52,53
	0x3C,0x4A,0x49,0x49,0x30,0x00,
	0x01,0x71,0x09,0x05,0x03,0x00,// 6 7		54,55
	0x36,0x49,0x49,0x49,0x36,0x00,
	0x06,0x49,0x49,0x29,0x1E,0x00,// 8 9		56,57
	0x36,0x00,0x00,0x00,0x00,0x00,
	0x56,0x36,0x00,0x00,0x00,0x00,// : ;		58,59
	0x08,0x14,0x22,0x41,0x00,0x00,
	0x14,0x14,0x14,0x00,0x00,0x00,// < =		60,61
	0x41,0x22,0x14,0x08,0x00,0x00,
	0x02,0x01,0x51,0x09,0x06,0x00,// > ?		62,63
	0x32,0x49,0x79,0x41,0x3E,0x00,
	0x7E,0x11,0x11,0x7E,0x00,0x00,// @ A		64,65
	0x7F,0x49,0x49,0x36,0x00,0x00,
	0x3E,0x41,0x41,0x22,0x00,0x00,// B C		66,67
	0x7F,0x41,0x22,0x1C,0x00,0x00,
	0x7F,0x49,0x49,0x41,0x00,0x00,// D E		68,69
	0x7F,0x09,0x09,0x01,0x00,0x00,
	0x3E,0x41,0x51,0x32,0x00,0x00,// F G		70,71
	0x7F,0x08,0x08,0x7F,0x00,0x00,
	0x41,0x7F,0x41,0x00,0x00,0x00,// H I		72,73
	0x20,0x40,0x41,0x3F,0x01,0x00,
	0x7F,0x08,0x14,0x22,0x41,0x00,// J K		74,75
	0x7F,0x40,0x40,0x00,0x00,0x00,
	0x7F,0x02,0x04,0x02,0x7F,0x00,// L M		76,77
	0x7F,0x04,0x08,0x10,0x7F,0x00,
	0x3E,0x41,0x41,0x3E,0x00,0x00,// N O		78,79
	0x7F,0x09,0x09,0x06,0x00,0x00,
	0x3E,0x41,0x51,0x21,0x5E,0x00,// P Q		80,81
	0x7F,0x19,0x29,0x46,0x00,0x00,
	0x46,0x49,0x49,0x31,0x00,0x00,// R S		82,83
	0x01,0x7F,0x01,0x00,0x00,0x00,
	0x3F,0x40,0x40,0x3F,0x00,0x00,// T U		84,85
	0x1F,0x20,0x40,0x20,0x1F,0x00,
	0x7F,0x20,0x18,0x20,0x7F,0x00,// V W		86,87
	0x63,0x14,0x08,0x14,0x63,0x00,
	0x03,0x04,0x78,0x04,0x03,0x00,// X Y		88,89
	0x61,0x51,0x49,0x45,0x43,0x00,
	0x7F,0x41,0x41,0x00,0x00,0x00,// Z [		90,91
	0x02,0x04,0x08,0x10,0x20,0x00,
	0x41,0x41,0x7F,0x00,0x00,0x00,// \ ]		92,93
	0x04,0x02,0x01,0x02,0x04,0x00,
	0x40,0x40,0x40,0x00,0x00,0x00,// ^ _		94,95
	0x01,0x02,0x04,0x00,0x00,0x00,
	0x20,0x54,0x54,0x78,0x00,0x00,// ` a		96,97
	0x7F,0x48,0x44,0x38,0x00,0x00,
	0x38,0x44,0x44,0x00,0x00,0x00,// b c		98,99
	0x38,0x44,0x48,0x7F,0x00,0x00,
	0x38,0x54,0x54,0x18,0x00,0x00,// d e		100,101
	0x08,0x7E,0x09,0x01,0x00,0x00,
	0x08,0x14,0x54,0x3C,0x00,0x00,// f g		102,103
	0x7F,0x08,0x04,0x78,0x00,0x00,
	0x44,0x7D,0x40,0x00,0x00,0x00,// h i		104,105
	0x20,0x40,0x44,0x3D,0x00,0x00,
	0x7F,0x10,0x28,0x44,0x00,0x00,// j k		106,107
	0x41,0x7F,0x40,0x00,0x00,0x00,
	0x7C,0x04,0x18,0x04,0x78,0x00,// l m		108,109
	0x7C,0x08,0x04,0x78,0x00,0x00,
	0x38,0x44,0x44,0x38,0x00,0x00,// n o		110,111
	0x7C,0x14,0x14,0x08,0x00,0x00,
	0x08,0x14,0x18,0x7C,0x00,0x00,// p q		112,113
	0x7C,0x08,0x04,0x08,0x00,0x00,
	0x48,0x54,0x54,0x20,0x00,0x00,// r s		114,115
	0x04,0x3F,0x44,0x40,0x00,0x00,
	0x3C,0x40,0x20,0x7C,0x00,0x00,// t u		116,117
	0x1C,0x20,0x40,0x20,0x1C,0x00,
	0x3C,0x40,0x30,0x40,0x3C,0x00,// v w		118,119
	0x44,0x28,0x10,0x28,0x44,0x00,
	0x0C,0x50,0x50,0x3C,0x00,0x00,// x y		120,121
	0x44,0x64,0x54,0x4C,0x44,0x00,
	0x08,0x36,0x41,0x00,0x00,0x00,// z {		122,123
	0x7F,0x00,0x00,0x00,0x00,0x00,
	0x41,0x36,0x08,0x00,0x00,0x00,// | }		124,125
	0x08,0x2A,0x1C,0x08,0x00,0x00,
	0x08,0x1C,0x2A,0x08,0x00,0x00,// -> <-	126,127
	0x14,0x36,0x77,0x36,0x14,0x00 };//			128								
/*#####################################################*/
unsigned int controlls_change_color(unsigned int color, double value);
bool screen_copy(void *pDisplayTo, void *pDisplayFrom, bool put_cursor, signed int X, signed int Y, unsigned int color);
void clip_limit(tRectangle *limited, tRectangle *limits);
void TouchPaintPoint(void *pDisplay, signed int X, signed int Y, unsigned int color);
bool check_if_inside_box(signed int x_start, signed int y_start, signed int x_len, signed int y_len, signed int x_point, signed int y_point);
void put_circle(void *pDisplay, signed int x, signed int y, signed int _radius, unsigned char fill, unsigned int color);
void put_line(void *pDisplay, signed int X1, signed int Y1, signed int X2, signed int Y2, unsigned char width, unsigned int color);
void put_elipse(void *pDisplay, signed int xc,signed int yc,signed int _rx,signed int _ry, unsigned char Fill, unsigned int color);
void put_triangle(void *pDisplay, signed int  Ax,signed int  Ay,signed int  Bx,signed int  By,signed int  Cx,signed int  Cy, unsigned char Fill, unsigned int color);


//void put_horizontal_line(tDisplay *pDisplay, signed int X1, signed int X2, signed int Y, unsigned char width, unsigned int color);
//void put_vertical_line(tDisplay *pDisplay, signed int Y1, signed int Y2, signed int X, unsigned char width, unsigned int color);
void string_select_get(void *pDisplay, tFont *pFont, char *pcString, bool WordWrap, signed int lLength, signed int *Start, signed int *SelStartReturn, signed int *SelLenReturn, signed int _XPush, signed int _YPush, signed int _XPull, signed int _YPull, signed int lX, signed int lY, unsigned int *return_command);
StringProperties_t string_properties_get(void *pDisplay, tFont *pFont, char *pcString, bool WordWrap, signed int lLength);
unsigned char char_height_get(tFont *pFont);
signed int string_width_get(void *pDisplay, tFont *pFont, char *pcString, signed int lLength);
signed int string_rows_get(char *pcString, signed int lLength);
signed int put_string(print_string_properties *properties);
signed int put_string_tiny_chr(print_string_properties *properties);
signed int string_width_get_tiny_chr(void *pDisplay, tFont *pFont, char *pcString, signed int lLength);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "controls_definition.c"
#endif
/*#####################################################*/
#endif /* CONTROLS_DEFINITION_H_ */
/*#####################################################*/
