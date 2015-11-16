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
}tPictureBox;
//#######################################################################################
#define _new_picturebox(name) tPictureBox *name = NULL
//#define free_picturebox(address) free(address);



#endif /* LIB_LIB_GFX_PICTUREBOX_DEF_H_ */
