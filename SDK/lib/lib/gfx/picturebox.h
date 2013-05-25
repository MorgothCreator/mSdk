#ifndef PICTUREBOX_H_
#define PICTUREBOX_H_
//#######################################################################################
#include <stdbool.h>
#include "api/timer_api.h"
#include "controls_definition.h"
//#######################################################################################
typedef struct PictureBox_s
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
	controls_color_struct_t Color;
	struct
	{
		struct
		{
			void* CallbackReturnData;
			void*(*CallBack)(struct PictureBox_s *, tControlCommandData *);
		}OnMove;
		struct
		{
			void* CallbackReturnData;
			void*(*CallBack)(struct PictureBox_s *, tControlCommandData *);
		}OnUp;
		struct
		{
			void* CallbackReturnData;
			void*(*CallBack)(struct PictureBox_s *, tControlCommandData *);
		}OnDown;
		struct
		{
			void* CallbackReturnData;
			void*(*CallBack)(struct PictureBox_s *, tControlCommandData *);
		}Refresh;
		bool CursorUp;
		bool CursorDown;
		bool CursorMove;
	}Events;
	struct
	{
		struct
		{
			bool Initiated;
		}Control;
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
		bool OldStateVisible;
		bool OldStateEnabled;
		bool NeedEntireRefresh;
		bool NeedEntireRepaint;
		bool CursorDownInsideBox;
		bool IsChildren;
		bool NoPaintBackGround;
		CursorState OldStateCursor;
		STimer_t ContinuouslyPushTimer;
		tDisplay *pDisplay;
		tRectangle PictureWindowLimits;
	}Internals;
	bool PaintBackground;
	unsigned int BackgroundColor;
	bool Visible;
	bool Enabled;
	tControlCommandData*(*Idle)(void*, tControlCommandData*);
}tPictureBox;
//#######################################################################################
void picturebox(tPictureBox *settings, tControlCommandData* control_comand);
tPictureBox *new_picturebox(tDisplay *ScreenDisplay);
bool free_picturebox(tPictureBox* settings);
void picturebox_clear(tPictureBox* settings);
void picturebox_put_pixel(tPictureBox* settings, signed int X, signed int Y, unsigned int color);
void picturebox_put_horizontal_line(tPictureBox* settings, signed int X1, signed int X2, signed int Y, unsigned char width, unsigned int color);
void picturebox_put_vertical_line(tPictureBox* settings, signed int Y1, signed int Y2, signed int X, unsigned char width, unsigned int color);
void picturebox_put_circle(tPictureBox* settings, signed int x, signed int y, signed int radius, unsigned char fill, unsigned int color);
void picturebox_put_line(tPictureBox* settings, signed int X1, signed int Y1, signed int X2, signed int Y2, unsigned char width, unsigned int color);
void picturebox_put_elipse(tPictureBox* settings, signed int xc,signed int yc,signed int rx,signed int ry, unsigned char fill, unsigned int color);
void picturebox_put_triangle(tPictureBox* settings, signed int  Ax,signed int  Ay,signed int  Bx,signed int  By,signed int  Cx,signed int  Cy, unsigned char fill, unsigned int color);
void picturebox_put_string(tPictureBox* settings, tFont *pFont, char *pcString, signed int lLength, unsigned int foreground_color, unsigned int background_color, bool ulOpaque, bool ulVisible, bool WordWrap, signed int lX, signed int lY, signed int _SelStart, signed int _SelLen);
//#######################################################################################
#ifdef HEADER_INCLUDE_C_FILES
#include "buton.c"
#endif
//#######################################################################################
#endif /* BUTON_H_ */
