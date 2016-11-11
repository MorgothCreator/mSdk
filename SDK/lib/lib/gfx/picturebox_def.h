/*
 * picturebox_def.h
 *
 *  Created on: Nov 13, 2015
 *      Author: John Smith
 */

#ifndef LIB_LIB_GFX_PICTUREBOX_DEF_H_
#define LIB_LIB_GFX_PICTUREBOX_DEF_H_

#include "3d.h"
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
		}PositionOffset;
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
		//CursorState OldStateCursor;
		STimer_t ContinuouslyPushTimer;
		tDisplay *pDisplay;
		tRectangle PictureWindowLimits;
		void *ParentWindow;
		bool ParentWindowStateEnabled;
	}Internals;
	bool PaintBackground;
	unsigned int BackgroundColor;
	bool Visible;
	bool Enabled;
	tControlCommandData*(*Idle)(void*, tControlCommandData*);
	bool (*free)(struct PictureBox_s* settings);
	void (*clear)(struct PictureBox_s* settings);
	//void picturebox_copy_rectangle)(tPictureBox* settings, unsigned int *src_buff, signed int src_x_buff_size, signed int src_y_buff_size, signed int src_x_offset, signed int src_y_offset);
	void (*copy_rectangle)(struct PictureBox_s* settings, unsigned int *src_buff, unsigned int src_buff_data_offset, tRectangle *_dest_rectangle, tRectangle *_src_rectangle, signed int src_width, signed int src_height);
	void (*put_pixel)(struct PictureBox_s* settings, signed int X, signed int Y, unsigned int color);
	void (*put_horizontal_line)(struct PictureBox_s* settings, signed int X1, signed int X2, signed int Y, unsigned char width, unsigned int color);
	void (*put_vertical_line)(struct PictureBox_s* settings, signed int Y1, signed int Y2, signed int X, unsigned char width, unsigned int color);
	void (*put_circle)(struct PictureBox_s* settings, signed int x, signed int y, signed int radius, unsigned char fill, unsigned int color);
	void (*put_line)(struct PictureBox_s* settings, signed int X1, signed int Y1, signed int X2, signed int Y2, unsigned char width, unsigned int color);
	void (*put_elipse)(struct PictureBox_s* settings, signed int xc,signed int yc,signed int rx,signed int ry, unsigned char fill, unsigned int color);
	void (*put_triangle)(struct PictureBox_s* settings, signed int  Ax,signed int  Ay,signed int  Bx,signed int  By,signed int  Cx,signed int  Cy, unsigned char fill, unsigned int color);
	void (*put_string)(struct PictureBox_s* settings, print_string_properties *properties);
	void (*put_3d_triangle)(struct PictureBox_s* settings, _3d_points *Points, signed int X_offset, signed int Y_offset, double X_Angle, double Y_Angle, double Z_Angle, unsigned int Color);
	void (*put_3d_rectangle)(struct PictureBox_s* settings, _3d_points *Points, signed int X_offset, signed int Y_offset, double X_Angle, double Y_Angle, double Z_Angle, unsigned int Color);
	#if _USE_BITMAP_LIBRARY == 1
	bool (*put_bitmap)(struct PictureBox_s* settings, unsigned char *file, signed int X, signed int Y, bool use_transparency, bool scale);
	bool (*put_fbitmap)(struct PictureBox_s* settings, char *path, signed int X, signed int Y, bool use_transparency, bool scale);
	#endif
}tPictureBox;
//#######################################################################################
#define _new_picturebox(name) tPictureBox *name = NULL
//#define free_picturebox(address) free(address);



#endif /* LIB_LIB_GFX_PICTUREBOX_DEF_H_ */
