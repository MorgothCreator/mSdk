/*
 *  lib/gfx/controls_definition.c
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

#ifdef FLASH_DEVICE
#include <avr/pgmspace.h>
#endif
#include "sys/plat_properties.h"
#include "controls_definition.h"
#include "api/lcd_api.h"
#include "api/lcd_def.h"
//#include "api/pmic_api.h"
#include "sys/cache.h"
#include "resource/fonts.h"
//#######################################################################################
#ifndef USE_16_BIT_COLOR_DEPTH
controlscolor controls_color =
{
	.Control_Color_Enabled_Ink_Pull = (0x39<<16)|(0x39<<8)|(0x39),
	.Control_Color_Enabled_Ink_Push = (0x00<<16)|(0x00<<8)|(0x00),
	.Control_Color_Enabled_Buton_Pull = (0xF0<<16)|(0xF0<<8)|(0xF0),
	.Control_Color_Enabled_Buton_Push = (0xA0<<16)|(0xA0<<8)|(0xA0),
	.Control_Color_Enabled_Border_Pull = (0x80<<16)|(0x80<<8)|(0x80),
	.Control_Color_Enabled_Border_Push = (0x80<<16)|(0x80<<8)|(0x80),
	.Control_Color_Enabled_BackGround = (0x37<<16)|(0x37<<8)|(0x37),
	.Control_Color_Enabled_WindowBorder = (0x80<<16)|(0x80<<8)|(0x80),
	.Control_Color_Enabled_WindowHeader = (0x7F<<16)|(0x7F<<8)|(0x7F),

	.Control_Color_Disabled_Ink_Pull = ((0x39<<16)  )|((0x39<<8)  )|((0x39)  ),
	//((0x39<<16)/2)|((0x39<<8)/2)|((0x39)/2),
	.Control_Color_Disabled_Buton_Pull = 0x989CB6,//((0x37<<16)  )|((0x37<<8)  )|((0x37)  ),
	//((0x91<<16)/2)|((0x91<<8)/2)|((0x91)/2),
	.Control_Color_Disabled_Border_Pull = 0x808080,//((0x37<<16)  )|((0x37<<8)  )|((0x37)  ),
	//((0xFF<<16)/2)|((0xFF<<8)/2)|((0xFF)/2),
	.Control_Color_Disabled_BackGround = ((0x37<<16)  )|((0x37<<8)  )|((0x37)  ),
	.Control_Color_Disabled_WindowBorder = 0x989CB6,//((0x37<<16)  )|((0x37<<8)  )|((0x37)  ),
	.Control_Color_Disabled_WindowHeader = 0x989CB6,//((0x37<<16)  )|((0x37<<8)  )|((0x37)  ),
	.Scren = 0x989CB6,
#ifdef DEFAULT_FONT
	.DefaultFont = (tFont*)&DEFAULT_FONT
#else
	.DefaultFont = (tFont*)NULL
#endif
};
#else
#define COLOR_24_to_16(R, G, B) ((unsigned)(((R)>>3)&0b11111) | (((G)<<3)&0b11111100000) | (((B)<<8)&0b1111100000000000))
controlscolor controls_color =
{
		.Control_Color_Enabled_Ink_Pull = COLOR_24_to_16(0x39,0x39,0x39),
		.Control_Color_Enabled_Ink_Push = COLOR_24_to_16(0x00,0x00,0x00),
		.Control_Color_Enabled_Buton_Pull = COLOR_24_to_16(0xFF,0xFF,0xFF),
		.Control_Color_Enabled_Buton_Push = COLOR_24_to_16(0x91,0x91,0x91),
		.Control_Color_Enabled_Border_Pull = COLOR_24_to_16(0xFF,0xFF,0xFF),
		.Control_Color_Enabled_Border_PushCOLOR_24_to_16(0xFF,0xFF,0xFF),
		.Control_Color_Enabled_BackGround = COLOR_24_to_16(0x37,0x37,0x37),
		.Control_Color_Enabled_WindowBorder = COLOR_24_to_16(0x55,0x55,0x55),
		.Control_Color_Enabled_WindowHeader = COLOR_24_to_16(0x7F,0x7F,0x7F),

		.Control_Color_Disabled_Ink_Pull = COLOR_24_to_16((0x00)/2,(0x00)/2,(0x00)/2),
	//COLOR_24_to_16((0x39)/2,(0x39)/2,(0x39)/2),
		.Control_Color_Disabled_Buton_Pull = COLOR_24_to_16((0xFF)/2,(0xFF)/2,(0xFF)/2),
	//COLOR_24_to_16((0x91)/2,(0x91)/2,(0x91)/2),
		.Control_Color_Disabled_Border_Pull = COLOR_24_to_16((0xFF)/2,(0xFF)/2,(0xFF)/2),
	//COLOR_24_to_16((0xFF)/2,(0xFF)/2,(0xFF)/2),
		.Control_Color_Disabled_BackGround = COLOR_24_to_16((0x37)/2,(0x37)/2,(0x37)/2),
		.Control_Color_Disabled_WindowBorder = COLOR_24_to_16(0x98, 0x9C, 0xB6),
		.Control_Color_Disabled_WindowHeader = COLOR_24_to_16(0x98, 0x9C, 0xB6),
	COLOR_24_to_16(0x98, 0x9C, 0xB6),
#ifdef USE_FONT_CMSS14B
	.DefaultFont = (tFont*)&g_sFontCmss14b
#else
	.DefaultFont = (tFont*)NULL
#endif
};
#endif
//#######################################################################################
#ifndef USE_16_BIT_COLOR_DEPTH
unsigned int controlls_change_color(unsigned int color, double value)
{
	if(value>(double)0)
	{
		double Red   = (double)((color)    &0xFF);
		double Green = (double)((color>>8) &0xFF);
		double Blue  = (double)((color>>16)&0xFF);
		Red         *=value;
		Green       *=value;
		Blue        *=value;
		if(Red>  (double)255)Red  =(double)255;
		if(Green>(double)255)Green=(double)255;
		if(Blue> (double)255)Blue =(double)255;
		return (unsigned int)((unsigned int)Red|(((unsigned int)Green)<<8)|(((unsigned int)Blue)<<16));
	}
	else if(value<(double)0)
	{
		double Red   = (double)((color)    &0xFF);
		double Green = (double)((color>>8) &0xFF);
		double Blue  = (double)((color>>16)&0xFF);
		Red         /= 0 - value;
		Green       /= 0 - value;
		Blue        /= 0 - value;
		return (unsigned int)((unsigned int)Red|(((unsigned int)Green)<<8)|(((unsigned int)Blue)<<16));
	}
	return color;
}
#else
unsigned int controlls_change_color(unsigned int color, double value)
{
	if(value>(double)0)
	{
		double Red   = (double)((color)    &0x1F);
		double Green = (double)((color>>5) &0x3F);
		double Blue  = (double)((color>>11)&0x1F);
		Red         *=value;
		Green       *=value;
		Blue        *=value;
		if(Red>  (double)31)Red  =(double)31;
		if(Green>(double)63)Green=(double)63;
		if(Blue> (double)31)Blue =(double)31;
		return (unsigned int)((unsigned int)Red|(((unsigned int)Green)<<5)|(((unsigned int)Blue)<<11));
	}
	else if(value<(double)0)
	{
		double Red   = (double)((color)    &0x1F);
		double Green = (double)((color>>5) &0x3F);
		double Blue  = (double)((color>>11)&0x1F);
		Red         /= 0 - value;
		Green       /= 0 - value;
		Blue        /= 0 - value;
		return (unsigned int)((unsigned int)Red|(((unsigned int)Green)<<5)|(((unsigned int)Blue)<<11));
	}
	return color;
}
#endif
//#######################################################################################
bool screen_copy(tDisplay *pDisplayTo, tDisplay *pDisplayFrom, bool put_cursor, signed int X, signed int Y, unsigned int color)
{
	/*if(pDisplayTo->raster_timings->X != pDisplayFrom->raster_timings->X || pDisplayTo->raster_timings->Y != pDisplayFrom->raster_timings->Y) return false;
	//memcpy((void *)pDisplayTo->DisplayData, (void *)pDisplayFrom->DisplayData, (sizeof(unsigned int) * pDisplayFrom->Height * pDisplayFrom->Width) + 32);
	//box_cache_clean(pDisplayTo, 0, 0, pDisplayFrom->Width, pDisplayFrom->Height);
	signed int LineCnt = 0;
	volatile unsigned int* ScreenBuff = pDisplayTo->DisplayData + pDisplayTo->raster_timings->palete_len;
	volatile unsigned int* _ScreenBuff = pDisplayFrom->DisplayData + pDisplayTo->raster_timings->palete_len;
	for(; LineCnt < pDisplayTo->raster_timings->Y; LineCnt ++)
	{
		memcpy((void *)(ScreenBuff + (pDisplayFrom->raster_timings->X * LineCnt)), (void *)(_ScreenBuff + (pDisplayFrom->raster_timings->X * LineCnt)), (sizeof(ScreenBuff[0]) * pDisplayTo->raster_timings->X));
		if(put_cursor == true && LineCnt >= Y && LineCnt <= Y + 2)
		{
			unsigned int cnt_x = X;
			for(;cnt_x < X + 2; cnt_x++) put_pixel(pDisplayTo, cnt_x, LineCnt, color);
		}
		CacheDataCleanInvalidateBuff((unsigned int)((unsigned int*)(ScreenBuff + (pDisplayFrom->raster_timings->X * LineCnt))), (sizeof(ScreenBuff[0]) * pDisplayTo->raster_timings->X) + 64);
	}
	return true;*/
	return _screen_copy(pDisplayTo, pDisplayFrom, put_cursor, X, Y, color);
}
//#######################################################################################
void clip_limit(tRectangle *limited, tRectangle *limits)
{
	if(limited->sXMin < limits->sXMin) limited->sXMin = limits->sXMin;
	if(limited->sYMin < limits->sYMin) limited->sYMin = limits->sYMin;
	if(limited->sXMax >= limits->sXMax) limited->sXMax = limits->sXMax;
	if(limited->sYMax >= limits->sYMax) limited->sYMax = limits->sYMax;
}
//#######################################################################################
void TouchPaintPoint(tDisplay *pDisplay, signed int X, signed int Y, unsigned int color)
{
	put_horizontal_line(pDisplay, X- 7, 6, Y, 1, color);
	put_horizontal_line(pDisplay, X+ 2, 6, Y, 1, color);
	put_vertical_line(pDisplay, Y- 7, 6, X, 1, color);
	put_vertical_line(pDisplay, Y+ 2, 6, X, 1, color);
}
//#######################################################################################
bool check_if_inside_box(signed int x_start, signed int y_start, signed int x_len, signed int y_len, signed int x_point, signed int y_point)
{
	signed int x_end = x_start + x_len ,y_end = y_start + y_len;
	if(x_point >= x_start && y_point >= y_start && x_point < x_end && y_point < y_end) return true;
	else return false;
}
//#######################################################################################
void put_pixel(tDisplay *pDisplay, signed int X, signed int Y, unsigned int color);
void put_circle(tDisplay *pDisplay, signed int x, signed int y, signed int _radius, unsigned char fill, unsigned int color)
{
	signed int radius = _radius;
	if(radius < 0) radius = ~radius;
	signed int a, b, P;

   a = 0;
   b = radius;
   P = 1 - radius;
   signed int Tmp1; 
   signed int Tmp2; 
   signed int Tmp3; 
   signed int Tmp4; 
   signed int Tmp5; 
   signed int Tmp6; 
   signed int Tmp7; 
   signed int Tmp8;
   signed int _Tmp5 = 5, _Tmp7 = 0;

   do
   {
      Tmp1 = x+a;
      Tmp2 = x-a;
      Tmp3 = x+b;
      Tmp4 = x-b;
      Tmp5 = y+a;
      Tmp6 = y-a;
      Tmp7 = y+b;
      Tmp8 = y-b;
	  if(fill)
      {

		 if(_Tmp7 != Tmp7)
		 {
			put_horizontal_line(pDisplay,Tmp2, Tmp1, Tmp7, 1, color);
			put_horizontal_line(pDisplay,Tmp2, Tmp1, Tmp8, 1, color);
		 }
         if(_Tmp5 != Tmp5)
		 {
			put_horizontal_line(pDisplay,Tmp4, Tmp3, Tmp5, 1, color);
			put_horizontal_line(pDisplay,Tmp4, Tmp3, Tmp6, 1, color);
		 }
      _Tmp5 = Tmp5;
      _Tmp7 = Tmp7;
      }
      else
      {
		put_pixel(pDisplay,Tmp1, Tmp7, color);
		put_pixel(pDisplay,Tmp3, Tmp5, color);
		put_pixel(pDisplay,Tmp2, Tmp7, color);
		put_pixel(pDisplay,Tmp4, Tmp5, color);
		put_pixel(pDisplay,Tmp3, Tmp6, color);
		put_pixel(pDisplay,Tmp1, Tmp8, color);
		put_pixel(pDisplay,Tmp2, Tmp8, color);
		put_pixel(pDisplay,Tmp4, Tmp6, color);
      }

      if(P < 0)
         P += 3 + 2 * a++;
      else
         P += 5 + 2 * (a++ - b--);
    } while(a <= b);
}
//#######################################################################################
void put_line(tDisplay *pDisplay, signed int X1, signed int Y1, signed int X2, signed int Y2, unsigned char width, unsigned int color)
{
	/*if(X1 > X2)
	{
		int Tmp = X1;
		X1 = X2;
		X2 = Tmp;
	}
	if(Y1 > Y2)
	{
		int Tmp = Y1;
		Y1 = Y2;
		Y2 = Tmp;
	}*/
	if(width == 1)
	{
		signed int CurrentX, CurrentY, Xinc, Yinc,
		Dx, Dy, TwoDx, TwoDy,
		TwoDxAccumulatedError, TwoDyAccumulatedError;
		Dx = (X2-X1);
		Dy = (Y2-Y1);
		TwoDx = Dx + Dx;
		TwoDy = Dy + Dy;
		CurrentX = X1;
		CurrentY = Y1;
		Xinc = 1;
		Yinc = 1;
		if(Dx < 0)
		{
			Xinc = -1;
			Dx = -Dx;
			TwoDx = -TwoDx;
		}
		if (Dy < 0)
		{
			Yinc = -1;
			Dy = -Dy;
			TwoDy = -TwoDy;
		}
		put_pixel(pDisplay,X1,Y1, color);
		if ((Dx != 0) || (Dy != 0))
		{
			if (Dy <= Dx)
			{
				TwoDxAccumulatedError = 0;
				do
				{
					CurrentX += Xinc;
					TwoDxAccumulatedError += TwoDy;
					if(TwoDxAccumulatedError > Dx)
					{
						CurrentY += Yinc;
						TwoDxAccumulatedError -= TwoDx;
					}
					put_pixel(pDisplay,CurrentX,CurrentY, color);
				}
				while (CurrentX != X2);
			}
			else
			{
				TwoDyAccumulatedError = 0;
				do
				{
					CurrentY += Yinc;
					TwoDyAccumulatedError += TwoDx;
					if(TwoDyAccumulatedError>Dy)
					{
						CurrentX += Xinc;
						TwoDyAccumulatedError -= TwoDy;
					}
					put_pixel(pDisplay,CurrentX,CurrentY, color);
				}
				while (CurrentY != Y2);
			}
		}
	}
	else
	{
		signed int half_width;
	   signed int dy, dx;
	   signed int  addx=1, addy=1, j;
	   signed int P, diff;
	   signed int c1, c2;

	   int i=0;
	   dx = X2 - X1;
	   dy = Y2 - Y1;

	   half_width = width/2;
	   c1 = -(dx*X1 + dy*Y1);
	   c2 = -(dx*X2 + dy*Y2);

	   if(X1 > X2)
	   {
		   signed int temp;
		  temp = c1;
		  c1 = c2;
		  c2 = temp;
		  addx = -1;
	   }
	   if(Y1 > Y2)
	   {
		   signed int temp;
		  temp = c1;
		  c1 = c2;
		  c2 = temp;
		  addy = -1;
	   }

	   if(dx >= dy)
	   {
		  P = 2*dy - dx;
		  diff = P - dx;

		  for(i=0; i<=dx; ++i)
		  {
#ifdef Use_FastDrawBar
			 screen_draw_vertical_line(pContext,Y1+ (-half_width), Y1+ (half_width+width%2), X1, 1);
#else
			 for(j=-half_width; j<half_width+width%2; ++j)
			 {
				 signed int temp;

				temp = dx*X1+dy*(Y1+j);    // Use more RAM to increase speed
				if(temp+c1 >= 0 && temp+c2 <=0) put_pixel(pDisplay,X1, Y1+j, color);
			 }
#endif
			 if(P < 0)
			 {
				P  += 2*dy;
				X1 += addx;
			 }
			 else
			 {
				P  += diff;
				X1 += addx;
				Y1 += addy;
			 }
		  }
	   }
	   else
	   {
		  P = 2*dx - dy;
		  diff = P - dy;

		  for(i=0; i<=dy; ++i)
		  {
			 if(P < 0)
			 {
				P  += 2*dx;
				Y1 += addy;
			 }
			 else
			 {
				P  += diff;
				X1 += addx;
				Y1 += addy;
			 }
#ifdef Use_FastDrawBar
			 put_horizontal_line(pContext,(X1+(-half_width)), (X1+(half_width+width%2)), Y1, 1);
#else
			 for(j=-half_width; j<half_width+width%2; ++j)
			 {
				 signed int temp;

				temp = dx*X1+dy*(Y1+j);    // Use more RAM to increase speed
				if(temp+c1 >= 0 && temp+c2 <= 0) put_pixel(pDisplay,X1+j, Y1, color);
			 }
#endif
		  }
	   }
	}
}
//#######################################################################################
void elipseplot(tDisplay *pDisplay, signed int xc,signed int yc,signed int x,signed int y, unsigned char Fill, unsigned int color)
{
	int Tmp1 = xc +x;
	int Tmp2 = xc -x;
	int Tmp3 = yc +y;
	int Tmp4 = yc -y;

	if(Fill)
	{
		put_horizontal_line(pDisplay,Tmp2, Tmp1, Tmp3, 1, color);
		put_horizontal_line(pDisplay,Tmp2, Tmp1, Tmp4, 1, color);
	}
	else
	{
		put_pixel(pDisplay,(unsigned int)(Tmp1),(unsigned int)(Tmp3), color);
		put_pixel(pDisplay,(unsigned int)(Tmp2),(unsigned int)(Tmp3), color);
		put_pixel(pDisplay,(unsigned int)(Tmp1),(unsigned int)(Tmp4), color);
		put_pixel(pDisplay,(unsigned int)(Tmp2),(unsigned int)(Tmp4), color);
	}
}
//----------------------------------------------------------------------------------------
void put_elipse(tDisplay *pDisplay, signed int xc,signed int yc,signed int _rx,signed int _ry, unsigned char Fill, unsigned int color)
{
	signed int rx = _rx;
	if(rx < 0) rx = 0xFFFFFFFF - rx;
	signed int ry = _ry;
	if(ry < 0) ry = 0xFFFFFFFF - ry;
	int rx2=rx*rx;
	int ry2=ry*ry;
	int tory2=2*ry2;
	int torx2=2*rx2;
	int p;
	int x=0;
	int y=ry;
	int py=torx2*y;
	int px=0;
	void elipseplot(tDisplay *,signed int,signed int,signed int,signed int,unsigned char, unsigned int);
	elipseplot(pDisplay,xc,yc,x,y,Fill, color);
	p=/*round(*/ry2-(rx2*ry)+(0.25*rx2)/*)*/;
	while(px<py)
	{
		x++;
		px+=tory2;
		if(p<0) p+=ry2+px;
		else
		{
			y--;
			py-=torx2;
			p+=ry2+px-py;
		}
		elipseplot(pDisplay,xc,yc,x,y,Fill, color);
	}
	p=/*round(*/ry2*(x+0.5)*(x+0.5)+rx2*(y-1)*(y-1)-rx2*ry2/*)*/;
	while(y>0)
	{
		y--;
		py-=torx2;
		if(p>0) p+=rx2-py;
		else
		{
			x++;
			px+=tory2;
			p+=rx2-py+px;
		}
		elipseplot(pDisplay,xc,yc,x,y,Fill, color);
	}
}
//#######################################################################################
/*
    *  the coordinates of vertices are (A.x,A.y), (B.x,B.y), (C.x,C.y); we assume that A.y<=B.y<=C.y (you should sort them first)
    * dx1,dx2,dx3 are deltas used in interpolation
    * horizline draws horizontal segment with coordinates (S.x,Y), (E.x,Y)
    * S.x, E.x are left and right x-coordinates of the segment we have to draw
    * S=A means that S.x=A.x; S.y=A.y;
*/

void triangle_swap_nibble(signed int* a,signed int *b)
{
	signed int t=*a;
	*a=*b;
	*b=t;
}

void put_triangle(tDisplay *pDisplay, signed int  Ax,signed int  Ay,signed int  Bx,signed int  By,signed int  Cx,signed int  Cy, unsigned char Fill, unsigned int color)
{
	signed long dx1,dx2,dx3;
	signed long Sx,Ex;
	unsigned int Sy,Ey;

	if (Ay>By) { triangle_swap_nibble(&Ay,&By); triangle_swap_nibble(&Ax,&Bx); }
	if (Ay>Cy) { triangle_swap_nibble(&Ay,&Cy); triangle_swap_nibble(&Ax,&Cx); }
	if (By>Cy) { triangle_swap_nibble(&By,&Cy); triangle_swap_nibble(&Bx,&Cx); }
	if (By-Ay > 0) dx1=((signed long)(Bx-Ax)<<16)/(By-Ay); else dx1=0;
	if (Cy-Ay > 0) dx2=((signed long)(Cx-Ax)<<16)/(Cy-Ay); else dx2=0;
	if (Cy-By > 0) dx3=((signed long)(Cx-Bx)<<16)/(Cy-By); else dx3=0;

	Ex=Sx=(signed long)Ax<<16; Ey=Sy=Ay;

	if(dx1 > dx2)
	{
		while(Sy<=By)
		{
			put_line(pDisplay, Sx>>16,Sy++,Ex>>16,Ey++,1,color);
			Sx+=dx2;
			Ex+=dx1;
		}
		Ex=(signed long)Bx<<16; Ey=By;
		while(Sy<=Cy)
		{
			put_line(pDisplay, Sx>>16,Sy++,Ex>>16,Ey++,1,color);
			Sx+=dx2;
			Ex+=dx3;
		}
	}
	else
	{
		while(Sy<=By)
		{
			put_line(pDisplay, Sx>>16,Sy++,Ex>>16,Ey++,1,color);
			Sx+=dx1;
			Ex+=dx2;
		}
		Sx=(signed long)Bx<<16; Sy=By;
		while(Sy<=Cy)
		{
			put_line(pDisplay, Sx>>16,Sy++,Ex>>16,Ey++,1,color);
			Sx+=dx3;
			Ex+=dx2;
		}
	}
}
//#######################################################################################
//#######################################################################################
static unsigned char _NumLeadingZeros(unsigned int x)
{
	register unsigned char count = 0;//sizeof(x)*8;

	while(x)
	{
		if(!(x & 0xFF000000))
		{
			count = count+8;
			x = x << 8;
		}
		else
		{
			while(!(x & 0x80000000))
			{
				x = x << 1;
				count++;
			}
			break;
		}
	}

	return count;
}
//#######################################################################################
void string_select_get(tDisplay *pDisplay, tFont *pFont, char *pcString, bool WordWrap, signed int lLength, signed int *Start, signed int *SelStartReturn, signed int *SelLenReturn, signed int _XPush, signed int _YPush, signed int _XPull, signed int _YPull, signed int lX, signed int lY, unsigned int *return_command)
{

	if(pDisplay->sClipRegion.sXMin > _XPull)
	{
		*return_command = ReturnCommand_GoLeft;
		unsigned int tmp = pDisplay->sClipRegion.sXMin - _XPull;
		if(tmp > ReturnCommand_MaxValue) tmp = ReturnCommand_MaxValue;
		*return_command |= tmp;
	}
	if(pDisplay->sClipRegion.sXMax < _XPull)
	{
		*return_command = ReturnCommand_GoRight;
		unsigned int tmp = _XPull - pDisplay->sClipRegion.sXMax;
		if(tmp > ReturnCommand_MaxValue) tmp = ReturnCommand_MaxValue;
		*return_command |= tmp;
	}
	if(pDisplay->sClipRegion.sYMin > _YPull)
	{
		*return_command = ReturnCommand_GoUp;
		unsigned int tmp = pDisplay->sClipRegion.sYMin - _YPull;
		if(tmp > ReturnCommand_MaxValue) tmp = ReturnCommand_MaxValue;
		*return_command |= tmp;
	}
	if(pDisplay->sClipRegion.sYMax < _YPull)
	{
		*return_command = ReturnCommand_GoDn;
		unsigned int tmp = _YPull - pDisplay->sClipRegion.sYMax;
		if(tmp > ReturnCommand_MaxValue) tmp = ReturnCommand_MaxValue;
		*return_command |= tmp;
	}
	if(*return_command) return;

	signed int StringColsHeight_Pixels = lY;
	//signed int CharHeight_Pixels = 0;


	const unsigned short *pusOffset;
    const unsigned char *pucData;
    //signed int lRows;
    volatile signed int StringLengthOfEveryRow = lX;


    //
    // Get some pointers to relevant information in the font to make things
    // easier, and give the compiler a hint about extraneous loads that it can
    // avoid.
    //
#ifdef FLASH_DEVICE
	pusOffset = (const unsigned short *)pFont + 2;
    pucData = (const unsigned char *)&pFont->pucData + 2;
#else
	pusOffset = pFont->pusOffset;
	pucData = pFont->pucData;
#endif

	signed int XPush = _XPush;
	signed int YPush = _YPush;
	signed int XPull = _XPull;
	signed int YPull = _YPull;

	/*if(XPush > XPull)
	{
		signed int Tmp = XPull;
		XPull = XPush;
		XPush = Tmp;
	}
	if(YPush > YPull)
	{
		signed int Tmp = YPull;
		YPull = YPush;
		YPush = Tmp;
	}*/

	signed int CharCount = 0;

	bool StartSelected = false;
	bool LenSelected = false;

    //
    // Loop through the characters in the string.
    //
    for(;*pcString && lLength; pcString++, lLength--)
    {
    	if(*Start < 0 && XPush < StringLengthOfEveryRow + (read_data_byte(pucData[read_data_word(pusOffset[*pcString - ' ']) + 1]) >> 1) && YPush < StringColsHeight_Pixels + read_data_byte(pFont->ucHeight) && StartSelected == false)
    	{
    		*SelStartReturn = CharCount;
    		*Start = CharCount;
    		StartSelected = true;
    	}
    	if(*Start >= 0 && XPull < StringLengthOfEveryRow + (read_data_byte(pucData[read_data_word(pusOffset[*pcString - ' ']) + 1]) >> 1) && YPull < StringColsHeight_Pixels + read_data_byte(pFont->ucHeight) && LenSelected == false)
    	{
    		*SelLenReturn = CharCount - *SelStartReturn;
    		LenSelected = true;
    		return;
    	}


    		CharCount += 1;
    	//
        // Get a pointer to the font data for the next character from the
        // string.  If there is not a glyph for the next character, replace it
        // with a ".".
        //
        if(((StringLengthOfEveryRow - pDisplay->sClipRegion.sXMin) + read_data_byte(pucData[read_data_word(pusOffset[*pcString - ' ']) + 1]) >= (pDisplay->sClipRegion.sXMax - pDisplay->sClipRegion.sXMin) && WordWrap == true))
        {
            //pucData = 0;

            StringLengthOfEveryRow = lX;
            StringColsHeight_Pixels += read_data_byte(pFont->ucHeight);
        }
        if(*pcString == '\r')
        {
         	//pucData = 0;

        	StringLengthOfEveryRow = lX;
        	StringColsHeight_Pixels += read_data_byte(pFont->ucHeight);
        }
        else if(*pcString == '\n')
        {
        	//pucData = 0;
			StringLengthOfEveryRow += read_data_byte(pucData[read_data_word(pusOffset[ABSENT_CHAR_REPLACEMENT - ' ']) + 1]);
        }
        else
        {
			if((*pcString >= ' ') && (*pcString) <= '~')
			{
				//
				// Add the width of this character as drawn with the given font.
				//
				StringLengthOfEveryRow += read_data_byte(pucData[read_data_word(pusOffset[*pcString - ' ']) + 1]);
			}
			else
			{
				//
				// This character does not exist in the font so replace it with
				// a '.' instead.  This matches the approach taken in Grput_string
				// and ensures that the width returned here represents the
				// rendered dimension of the string.
				//
				StringLengthOfEveryRow += read_data_byte(pucData[read_data_word(pusOffset[ABSENT_CHAR_REPLACEMENT - ' ']) + 1]);
			}
        }
    }

	if(*Start >= 0 && *pcString == 0 && YPull < StringColsHeight_Pixels + read_data_byte(pFont->ucHeight) && LenSelected == false)
	{
		*SelLenReturn = CharCount - *SelStartReturn;
		LenSelected = true;
	}

}
//#######################################################################################
StringProperties_t string_properties_get(tDisplay *pDisplay, tFont *pFont, char *pcString, bool WordWrap, signed int lLength)
{
	StringProperties_t StringReturnProperties;

	volatile signed int FirstRowLength_Pixels = 0;
	volatile signed int FirstRowLength_Chars = 0;
	volatile signed int StringLengthWithSpecialChars_Chars = 0;
	volatile signed int StringLengthWithOutSpecialChars_Chars = 0;
	volatile signed int StringRowsMaxLength_Pixels = 0;
	//signed int StringColsHeight_Pixels = 0;
	volatile signed int StringColsHeight_Rows = 0;
	//signed int CharHeight_Pixels = 0;

	volatile unsigned char EndOfFirstRow = false;

	//unsigned short *pusOffset;
    //unsigned char *pucData;
    //signed int lRows;
    volatile signed int StringLengthOfEveryRow = 0;

    //
    // Check the arguments.
    //
    //if(pDisplay) return;
    memset(&StringReturnProperties, 0, sizeof(StringReturnProperties));

    //
    // Get some pointers to relevant information in the font to make things
    // easier, and give the compiler a hint about extraneous loads that it can
    // avoid.
    //
#ifdef FLASH_DEVICE
    const unsigned short *pusOffset = (const unsigned short *)pFont + 2;
    const unsigned char *pucData = (const unsigned char *)&pFont->pucData + 2;
#else
	const unsigned short *pusOffset = pFont->pusOffset;
    const unsigned char *pucData = pFont->pucData;
#endif

	StringReturnProperties.FirstRowLength_Pixels = 0;
	StringReturnProperties.FirstRowLength_Chars = 0;
    //
    // Loop through the characters in the string.
    //
    for(;*pcString && lLength; pcString++, lLength--)
    {
        //
        // Get a pointer to the font data for the next character from the
        // string.  If there is not a glyph for the next character, replace it
        // with a ".".
        //
        if((StringLengthOfEveryRow + read_data_byte(pucData[read_data_word(pusOffset[*pcString - ' ']) + 1]) >= (pDisplay->sClipRegion.sXMax - pDisplay->sClipRegion.sXMin) && WordWrap == true))
        {
        	//pucData = 0;

        	if(EndOfFirstRow == false)
        	{
        		StringReturnProperties.FirstRowLength_Pixels = FirstRowLength_Pixels;
        		StringReturnProperties.FirstRowLength_Chars = FirstRowLength_Chars;
        	}
        	EndOfFirstRow = true;
        	StringColsHeight_Rows++;
			//tringLengthWithOutSpecialChars_Chars--;
        	//StringLengthWithSpecialChars_Chars++;
        	if(StringLengthOfEveryRow > StringRowsMaxLength_Pixels)
        	{
        		StringRowsMaxLength_Pixels = StringLengthOfEveryRow;
        	}
        	StringLengthOfEveryRow = 0;
			//FirstRowLength_Chars--;
			//FirstRowLength_Pixels -= pucData[pusOffset[ABSENT_CHAR_REPLACEMENT - ' '] + 1];
        }
        if(*pcString == '\r')
        {
        	//pucData = 0;

        	if(EndOfFirstRow == false)
        	{
        		StringReturnProperties.FirstRowLength_Pixels = FirstRowLength_Pixels;
        		StringReturnProperties.FirstRowLength_Chars = FirstRowLength_Chars;
        	}
        	EndOfFirstRow = true;
        	StringColsHeight_Rows++;
			//StringLengthWithOutSpecialChars_Chars--;
        	StringLengthWithSpecialChars_Chars++;
        	if(StringLengthOfEveryRow > StringRowsMaxLength_Pixels)
        	{
        		StringRowsMaxLength_Pixels = StringLengthOfEveryRow;
        	}
        	StringLengthOfEveryRow = 0;
			//FirstRowLength_Chars--;
			//FirstRowLength_Pixels -= pucData[pusOffset[ABSENT_CHAR_REPLACEMENT - ' '] + 1];
        }
        else if(*pcString == '\n')
        {
        	//pucData = 0;
			unsigned char CharLen = read_data_byte(pucData[read_data_word(pusOffset[ABSENT_CHAR_REPLACEMENT - ' ']) + 1]);
			FirstRowLength_Pixels += CharLen;
			StringLengthOfEveryRow += CharLen;
			FirstRowLength_Chars++;
			StringLengthWithOutSpecialChars_Chars++;
            StringLengthWithSpecialChars_Chars++;
        }
        else
        {
			if((*pcString >= ' ') && (*pcString <= '~'))
			{
				//
				// Add the width of this character as drawn with the given font.
				//
				unsigned char CharLen = read_data_byte(pucData[read_data_word(pusOffset[*pcString - ' ']) + 1]);
				FirstRowLength_Pixels += CharLen;
				StringLengthOfEveryRow += CharLen;
				FirstRowLength_Chars++;
				StringLengthWithOutSpecialChars_Chars++;
				StringLengthWithSpecialChars_Chars++;
			}
			else
			{
				//
				// This character does not exist in the font so replace it with
				// a '.' instead.  This matches the approach taken in Grput_string
				// and ensures that the width returned here represents the
				// rendered dimension of the string.
				//
				unsigned char CharLen = read_data_byte(pucData[read_data_word(pusOffset[ABSENT_CHAR_REPLACEMENT - ' ']) + 1]);
				FirstRowLength_Pixels += CharLen;
				StringLengthOfEveryRow += CharLen;
				FirstRowLength_Chars ++;
				StringLengthWithOutSpecialChars_Chars++;
				StringLengthWithSpecialChars_Chars++;
			}
        }
    }

    if(StringLengthWithSpecialChars_Chars) StringColsHeight_Rows++;

	if(StringLengthOfEveryRow > StringRowsMaxLength_Pixels) StringRowsMaxLength_Pixels = StringLengthOfEveryRow;
    StringReturnProperties.CharHeight_Pixels = read_data_byte(pFont->ucHeight);
    StringReturnProperties.StringColsHeight_Pixels = StringColsHeight_Rows * read_data_byte(pFont->ucHeight);
    StringReturnProperties.StringColsHeight_Rows = StringColsHeight_Rows;
    StringReturnProperties.StringLengthWithOutSpecialChars_Chars = StringLengthWithOutSpecialChars_Chars;
    StringReturnProperties.StringLengthWithSpecialChars_Chars = StringLengthWithSpecialChars_Chars;
    StringReturnProperties.StringRowsMaxLength_Pixels = StringRowsMaxLength_Pixels;
    //
    // Return the number of Rows of the string.
    //
    return(StringReturnProperties);
}
//#######################################################################################
unsigned char char_height_get(tFont *pFont)
{
	return read_data_byte(pFont->ucHeight);
}
//#######################################################################################
signed int string_width_get(tDisplay *pDisplay, tFont *pFont, char *pcString, signed int lLength)
{
    const unsigned short *pusOffset;
    const unsigned char *pucData;
    signed int lWidth;

    //
    // Check the arguments.
    //
    if(!pFont) return 0;
    if(!pcString) return 0;

    //
    // Get some pointers to relevant information in the font to make things
    // easier, and give the compiler a hint about extraneous loads that it can
    // avoid.
    //
#ifdef FLASH_DEVICE
	pusOffset = (const unsigned short *)pFont + 2;
    pucData = (const unsigned char *)&pFont->pucData + 2;
#else
	pusOffset = pFont->pusOffset;
	pucData = pFont->pucData;
#endif

    //
    // Loop through the characters in the string.
    //
    for(lWidth = 0; *pcString && lLength; pcString++, lLength--)
    {
        //
        // Get a pointer to the font data for the next character from the
        // string.  If there is not a glyph for the next character, replace it
        // with a ".".
        //
        if(*pcString == '\n')
        {
        	//pucData = 0;
        	lWidth += read_data_byte(pucData[read_data_word(pusOffset[ABSENT_CHAR_REPLACEMENT - ' ']) + 1]);
        }
        if((*pcString >= ' ') && (*pcString <= '~'))
        {
            //
            // Add the width of this character as drawn with the given font.
            //
            lWidth += read_data_byte(pucData[read_data_word(pusOffset[*pcString - ' ']) + 1]);
        }
        else
        {
            //
            // This character does not exist in the font so replace it with
            // a '.' instead.  This matches the approach taken in Grput_string
            // and ensures that the width returned here represents the
            // rendered dimension of the string.
            //
            lWidth += read_data_byte(pucData[read_data_word(pusOffset[ABSENT_CHAR_REPLACEMENT - ' ']) + 1]);
        }
    }

    //
    // Return the width of the string.
    //
    return(lWidth);
}
//#######################################################################################
signed int string_rows_get(char *pcString, signed int lLength)
{
    //const unsigned short *pusOffset;
    //const unsigned char *pucData;
    signed int lRows;

    //
    // Check the arguments.
    //
    if(!pcString) return 0;

    //
    // Get some pointers to relevant information in the font to make things
    // easier, and give the compiler a hint about extraneous loads that it can
    // avoid.
    //
    //
    // Loop through the characters in the string.
    //
    for(; *pcString && lLength; pcString++, lLength--)
    {
        //
        // Get a pointer to the font data for the next character from the
        // string.  If there is not a glyph for the next character, replace it
        // with a ".".
        //
        if(*pcString == '\r')
        {
            lRows++;
            //pcString++;
        }
    }
    //
    // Return the number of Rows of the string.
    //
    return(lRows);
}
//#######################################################################################
signed int put_string(print_string_properties *properties)
{
	tDisplay *pDisplay = properties->pDisplay;
	tFont *pFont = properties->pFont;
	char *pcString = properties->pcString;
	signed int lLength = properties->lLength;
	unsigned int foreground_color = properties->foreground_color;
	unsigned int background_color = properties->background_color;
	bool ulOpaque = properties->ulOpaque;
	bool ulVisible = properties->ulVisible;
	bool WordWrap = properties->WordWrap;
	signed int lX = properties->lX;
	signed int lY = properties->lY;
	signed int _SelStart = properties->_SelStart;
	signed int _SelLen = properties->_SelLen;

	signed int lIdx, lX0, lY0, lCount, lOff, lOn, lBit;
	signed int lXBackup = lX;
    const unsigned short *pusOffset;
    const unsigned char *pucData;

    bool _ulOpaque = ulOpaque;

    signed int SelStart = _SelStart;
    signed int SelLen = _SelLen;

	if(SelLen < 0)
	{
		SelStart -= 0 - SelLen;
		SelLen = 0 - SelLen;
	}


#ifdef FLASH_DEVICE
	pusOffset = (const unsigned short *)pFont + 2;
    pucData = (const unsigned char *)&pFont->pucData + 2;
#else
	pusOffset = pFont->pusOffset;
	pucData = pFont->pucData;
#endif

    //
    // Check the arguments.
    //
    if(!pDisplay) return 0;
    if(!pFont) return 0;
    if(!pcString) return 0;

    unsigned int CharCount = 0;
	unsigned int ChCount = 0;

    //
    // Copy the drawing context into a local structure that can be modified.
    //
    //
    // Loop through the characters in the string.
    //
    while(*pcString && lLength--)
    {
        ChCount++;
		if(SelLen)
        {
			if(CharCount >= SelStart && CharCount < SelStart + SelLen) _ulOpaque = true;
			else _ulOpaque = false;
			CharCount++;
        }

    	//
        // Stop drawing the string if the right edge of the clipping region has
        // been exceeded.
        //
        /*if(lX > sCon.sClipRegion.sXMax)
        {
            break;
        }*/

        //
        // Get a pointer to the font data for the next character from the
        // string.  If there is not a glyph for the next character, replace it
        // with a ".".
        //
        if(*pcString == '\r')
        {
        	lX = lXBackup;
        	pucData = 0;
            pcString++;
        }
        else if(*pcString == '\n')
        {
        	lY += read_data_byte(pFont->ucHeight);
        	pucData = 0;
            pcString++;
        }
        else
        {
			if((*pcString >= ' ') && (*pcString <= '~'))
			{
#ifdef FLASH_DEVICE
				pucData = ((const unsigned char *)&pFont->pucData + 2 + read_data_word(pusOffset[*pcString++ - ' ']));
#else
				pucData = (pFont->pucData + read_data_word(pusOffset[*pcString++ - ' ']));
#endif
			}
			else
			{
#ifdef FLASH_DEVICE
				pucData = ((const unsigned char *)&pFont->pucData + 2 + read_data_word(pusOffset[ABSENT_CHAR_REPLACEMENT - ' ']));
#else
				pucData = (pFont->pucData + read_data_word(pusOffset[ABSENT_CHAR_REPLACEMENT - ' ']));
#endif
				pcString++;
			}

	        if(lX + read_data_byte(pucData[1]) >= pDisplay->sClipRegion.sXMax && WordWrap == true)
	    	{
	        	lX = lXBackup;
	        	lY += read_data_byte(pFont->ucHeight);
	    	}
	        else
			//
			// See if the entire character is to the left of the clipping region.
			//
			if((lX + read_data_byte(pucData[1])) < pDisplay->sClipRegion.sXMin)
			{
				//
				// Increment the X coordinate by the width of the character.
				//
				lX += read_data_byte(pucData[1]);

				//
				// Go to the next character in the string.
				//
				continue;
			}

			//
			// Loop through the bytes in the encoded data for this glyph.
			//
			for(lIdx = 2, lX0 = 0, lBit = 0, lY0 = 0; lIdx < read_data_byte(pucData[0]); )
			{
				//
				// See if the bottom of the clipping region has been exceeded.
				//
				if((lY + lY0) > pDisplay->sClipRegion.sYMax)
				{
					//
					// Stop drawing this character.
					//
					break;
				}

				//
				// See if the font is uncompressed.
				//
				if(read_data_byte(pFont->ucFormat) == FONT_FMT_UNCOMPRESSED)
				{
					//
					// Count the number of off pixels from this position in the
					// glyph image.
					//
					for(lOff = 0; lIdx < read_data_byte(pucData[0]); )
					{
						//
						// Get the number of zero pixels at this position.
						//
						lCount = _NumLeadingZeros(read_data_byte(pucData[lIdx]) << (24 + lBit));

						//
						// If there were more than 8, then it is a "false" result
						// since it counted beyond the end of the current byte.
						// Therefore, simply limit it to the number of pixels
						// remaining in this byte.
						//
						if(lCount > 8)
						{
							lCount = 8 - lBit;
						}

						//
						// Increment the number of off pixels.
						//
						lOff += lCount;

						//
						// Increment the bit position within the byte.
						//
						lBit += lCount;

						//
						// See if the end of the byte has been reached.
						//
						if(lBit == 8)
						{
							//
							// Advance to the next byte and continue counting off
							// pixels.
							//
							lBit = 0;
							lIdx++;
						}
						else
						{
							//
							// Since the end of the byte was not reached, there
							// must be an on pixel.  Therefore, stop counting off
							// pixels.
							//
							break;
						}
					}

					//
					// Count the number of on pixels from this position in the
					// glyph image.
					//
					for(lOn = 0; lIdx < read_data_byte(pucData[0]); )
					{
						//
						// Get the number of one pixels at this location (by
						// inverting the data and counting the number of zeros).
						//
						lCount = _NumLeadingZeros(~(read_data_byte(pucData[lIdx]) << (24 + lBit)));

						//
						// If there were more than 8, then it is a "false" result
						// since it counted beyond the end of the current byte.
						// Therefore, simply limit it to the number of pixels
						// remaining in this byte.
						//
						if(lCount > 8)
						{
							lCount = 8 - lBit;
						}

						//
						// Increment the number of on pixels.
						//
						lOn += lCount;

						//
						// Increment the bit position within the byte.
						//
						lBit += lCount;

						//
						// See if the end of the byte has been reached.
						//
						if(lBit == 8)
						{
							//
							// Advance to the next byte and continue counting on
							// pixels.
							//
							lBit = 0;
							lIdx++;
						}
						else
						{
							//
							// Since the end of the byte was not reached, there
							// must be an off pixel.  Therefore, stop counting on
							// pixels.
							//
							break;
						}
					}
				}

				//
				// Otherwise, the font is compressed with a pixel RLE scheme.
				//
				else
				{
					//
					// See if this is a byte that encodes some on and off pixels.
					//
					if(read_data_byte(pucData[lIdx]))
					{
						//
						// Extract the number of off pixels.
						//
						lOff = (read_data_byte(pucData[lIdx]) >> 4) & 15;

						//
						// Extract the number of on pixels.
						//
						lOn = read_data_byte(pucData[lIdx]) & 15;

						//
						// Skip past this encoded byte.
						//
						lIdx++;
					}

					//
					// Otherwise, see if this is a repeated on pixel byte.
					//
					else if(read_data_byte(pucData[lIdx + 1]) & 0x80)
					{
						//
						// There are no off pixels in this encoding.
						//
						lOff = 0;

						//
						// Extract the number of on pixels.
						//
						lOn = (read_data_byte(pucData[lIdx + 1]) & 0x7f) * 8;

						//
						// Skip past these two encoded bytes.
						//
						lIdx += 2;
					}

					//
					// Otherwise, this is a repeated off pixel byte.
					//
					else
					{
						//
						// Extract the number of off pixels.
						//
						lOff = read_data_byte(pucData[lIdx + 1]) * 8;

						//
						// There are no on pixels in this encoding.
						//
						lOn = 0;

						//
						// Skip past these two encoded bytes.
						//
						lIdx += 2;
					}
				}

				//
				// Loop while there are any off pixels.
				//
				while(lOff)
				{
					//
					// See if the bottom of the clipping region has been exceeded.
					//
					if((lY + lY0) > pDisplay->sClipRegion.sYMax)
					{
						//
						// Ignore the remainder of the on pixels.
						//
						break;
					}

					//
					// See if there is more than one on pixel that will fit onto
					// the current row.
					//
					if((lOff > 1) && ((lX0 + 1) < read_data_byte(pucData[1])))
					{
						//
						// Determine the number of on pixels that will fit on this
						// row.
						//
						lCount = (((lX0 + lOff) > read_data_byte(pucData[1])) ? read_data_byte(pucData[1]) - lX0 :
								  lOff);

						//
						// If this row is within the clipping region, draw a
						// horizontal line that corresponds to the sequence of on
						// pixels.
						//
						if(((lY + lY0) >= pDisplay->sClipRegion.sYMin) && _ulOpaque)
						{
							if(ulVisible == true) put_horizontal_line(pDisplay, lX + lX0, lCount, lY + lY0, 1, background_color);
						}

						//
						// Decrement the count of on pixels by the number on this
						// row.
						//
						lOff -= lCount;

						//
						// Increment the X offset by the number of on pixels.
						//
						lX0 += lCount;
					}

					//
					// Otherwise, there is only a single on pixel that can be
					// drawn.
					//
					else
					{
						//
						// If this pixel is within the clipping region, then draw
						// it.
						//
						if(((lX + lX0) >= pDisplay->sClipRegion.sXMin) &&
						   ((lX + lX0) <= pDisplay->sClipRegion.sXMax) &&
						   ((lY + lY0) >= pDisplay->sClipRegion.sYMin) && _ulOpaque == true)
						{
#ifdef USE_16_BIT_COLOR_DEPTH
							if(ulVisible == true) put_pixel(pDisplay, lX + lX0, lY + lY0, background_color);
#else
							if(ulVisible == true) put_pixel(pDisplay, lX + lX0, lY + lY0, background_color<<8);
#endif
						}

						//
						// Decrement the count of on pixels.
						//
						lOff--;

						//
						// Increment the X offset.
						//
						lX0++;
					}

					//
					// See if the X offset has reached the right side of the
					// character glyph.
					//
					if(lX0 == read_data_byte(pucData[1]))
					{
						//
						// Increment the Y offset.
						//
						lY0++;

						//
						// Reset the X offset to the left side of the character
						// glyph.
						//
						lX0 = 0;
					}
				}

				//
				// Loop while there are any on pixels.
				//
				while(lOn)
				{
					//
					// See if the bottom of the clipping region has been exceeded.
					//
					if((lY + lY0) > pDisplay->sClipRegion.sYMax)
					{
						//
						// Ignore the remainder of the on pixels.
						//
						break;
					}

					//
					// See if there is more than one on pixel that will fit onto
					// the current row.
					//
					if((lOn > 1) && ((lX0 + 1) < read_data_byte(pucData[1])))
					{
						//
						// Determine the number of on pixels that will fit on this
						// row.
						//
						lCount = (((lX0 + lOn) > read_data_byte(pucData[1])) ? read_data_byte(pucData[1]) - lX0 :
								  lOn);

						//
						// If this row is within the clipping region, draw a
						// horizontal line that corresponds to the sequence of on
						// pixels.
						//
						if((lY + lY0) >= pDisplay->sClipRegion.sYMin)
						{
							if(ulVisible == true) put_horizontal_line(pDisplay, lX + lX0, lCount, lY + lY0, 1, foreground_color);
						}

						//
						// Decrement the count of on pixels by the number on this
						// row.
						//
						lOn -= lCount;

						//
						// Increment the X offset by the number of on pixels.
						//
						lX0 += lCount;
					}

					//
					// Otherwise, there is only a single on pixel that can be
					// drawn.
					//
					else
					{
						//
						// If this pixel is within the clipping region, then draw
						// it.
						//
						if(((lX + lX0) >= pDisplay->sClipRegion.sXMin) &&
						   ((lX + lX0) <= pDisplay->sClipRegion.sXMax) &&
						   ((lY + lY0) >= pDisplay->sClipRegion.sYMin))
						{
#ifdef USE_16_BIT_COLOR_DEPTH
							if(ulVisible == true) put_pixel(pDisplay, lX + lX0, lY + lY0, foreground_color);
#else
							if(ulVisible == true) put_pixel(pDisplay, lX + lX0, lY + lY0, foreground_color<<8);
#endif
						}

						//
						// Decrement the count of on pixels.
						//
						lOn--;

						//
						// Increment the X offset.
						//
						lX0++;
					}

					//
					// See if the X offset has reached the right side of the
					// character glyph.
					//
					if(lX0 == read_data_byte(pucData[1]))
					{
						//
						// Increment the Y offset.
						//
						lY0++;

						//
						// Reset the X offset to the left side of the character
						// glyph.
						//
						lX0 = 0;
					}
				}
			}

			//
			// Increment the X coordinate by the width of the character.
			//
			lX += read_data_byte(pucData[1]);
        }
    }
	return ChCount;
}
//#######################################################################################
signed int put_string_tiny_chr(print_string_properties *properties)
{
	tDisplay *pDisplay = properties->pDisplay;
	//tFont *pFont = properties->pFont;
	char *pcString = properties->pcString;
	signed int lLength = properties->lLength;
	unsigned int foreground_color = properties->foreground_color;
	unsigned int background_color = properties->background_color;
	bool ulOpaque = properties->ulOpaque;
	bool ulVisible = properties->ulVisible;
	bool WordWrap = properties->WordWrap;
	signed int lX = properties->lX;
	signed int lY = properties->lY;
	//signed int _SelStart = properties->_SelStart;
	//signed int _SelLen = properties->_SelLen;

	//struct Display_Struct* ScreenStruct = (struct Display_Struct*)ScreenFile->udata;
	signed char chWidth = 0;
	//signed char chWidth_Tmp = 0;
	signed char chHeight = 0;
	signed int CharPtr;
	signed char Tmp = 0;
	bool CompactWriting = true;
	signed int Cursor_X = lX;
	signed int Cursor_Y = lY;
	unsigned int CharCnt = 0;
#ifdef FLASH_DEVICE
	chWidth = pgm_read_byte(&CharTable6x8[2]);
	chHeight = pgm_read_byte(&CharTable6x8[3]);
#else
	chWidth = CharTable6x8[2];
	chHeight = CharTable6x8[3];
#endif
	do 
	{
		unsigned char Char = *pcString;
		if(lLength >= 0 && CharCnt > lLength) return CharCnt - 1;
		if(Char == 0) return CharCnt - 1;
#ifdef FLASH_DEVICE
		CharPtr = ((Char - pgm_read_byte(&CharTable6x8[4])) * chWidth) + pgm_read_byte(&CharTable6x8[0]);
		if(Char < pgm_read_byte(&CharTable6x8[4]) || Char > pgm_read_byte(&CharTable6x8[5]))
#else
		CharPtr = ((Char - CharTable6x8[4]) * chWidth) + CharTable6x8[0];
		if(Char < CharTable6x8[4] || Char > CharTable6x8[5])
#endif
		{
			//chWidth_Tmp = chWidth;
			chWidth = 0;
		}
		else
		{
			unsigned char Temp;
			if(CompactWriting)
			{
				for (Tmp = 1;Tmp < chWidth;Tmp++)
				{
#ifdef FLASH_DEVICE
					Temp = pgm_read_byte(&CharTable6x8[Tmp + CharPtr]);
#else
					Temp = CharTable6x8[Tmp + CharPtr];
#endif
					if(Temp == 0) break;
				}
				Tmp++;
			}
			else
			{
				Tmp = chWidth;
			}
			if(Cursor_X + Tmp >= pDisplay->sClipRegion.sXMin && Cursor_X < pDisplay->sClipRegion.sXMax + Tmp && Cursor_Y + chHeight >= pDisplay->sClipRegion.sYMin && Cursor_Y < pDisplay->sClipRegion.sYMax + chHeight)
			{
				if(ulVisible)
				{
					signed int XX = 0;
					signed int YY = 0;
					for (XX = 0; XX < Tmp; XX++)
					{
#ifdef FLASH_DEVICE
						Temp = pgm_read_byte(&CharTable6x8[XX + CharPtr]);
#else
						Temp = CharTable6x8[XX + CharPtr];
#endif
						for (YY = 0; YY < chHeight; YY++)
						{
							if (Temp & 0x1)
							{
								put_pixel(pDisplay, XX + Cursor_X, YY + Cursor_Y, foreground_color);
							}
							else
							{
								if(ulOpaque) put_pixel(pDisplay, XX + Cursor_X, YY + Cursor_Y, background_color);
							}
							Temp = Temp >>1;
						}
					}
				}
			}
		}
		//if(Tmp < chWidth) Tmp++;
		switch(Char)
		{
			case '\r':
				Cursor_X = lX;
				pcString++;
				break;
			case '\n':
				Cursor_Y += chHeight;
				pcString++;
				break;
			//case 9:
			//case 11:
				////signed short TabCursor = (ScreenStruct->Cursor_X/((chWidth_Tmp>>1)*4)*(chWidth_Tmp>>1))*4;
				//Tmp = (((Cursor_X/(chWidth_Tmp<<2))*(chWidth_Tmp<<2))-Cursor_X) + (chWidth_Tmp<<2);
			default:
				Cursor_X += Tmp;
				/*if((ScreenStruct->Cursor_X + chWidth > ScreenStruct->Width) && ScreenStruct->WorldWrap == True)
				{
					ScreenStruct->CharWidth = Tmp;
					ScreenStruct->CharHeight = chHeight;
					return EOF;
				}*/
				if((Cursor_X + chWidth > pDisplay->sClipRegion.sXMax) && WordWrap == true)
				{
					Cursor_Y += chHeight;
					Cursor_X = lX;
				}
				pcString++;
		}
		CharCnt++;
	}while(1);
}

//#######################################################################################
signed int string_width_get_tiny_chr(tDisplay *pDisplay, tFont *pFont, char *pcString, signed int lLength)
{
	signed char chWidth = 0;
	//signed char chWidth_Tmp = 0;
	signed int CharPtr;
	bool CompactWriting = true;
	signed int Cursor_X = 0;
	unsigned int CharCnt = 0;
#ifdef FLASH_DEVICE
	chWidth = pgm_read_byte(&CharTable6x8[2]);
#else
	chWidth = CharTable6x8[2];
#endif
	do 
	{
		signed char Tmp = 0;
		unsigned char Char = *pcString;
		if(lLength >= 0 && CharCnt > lLength) return Cursor_X;
		if(Char == 0) return Cursor_X;
#ifdef FLASH_DEVICE
		CharPtr = ((Char - pgm_read_byte(&CharTable6x8[4])) * chWidth) + pgm_read_byte(&CharTable6x8[0]);
		if(Char < pgm_read_byte(&CharTable6x8[4]) || Char > pgm_read_byte(&CharTable6x8[5]))
#else
		CharPtr = ((Char - CharTable6x8[4]) * chWidth) + CharTable6x8[0];
		if(Char < CharTable6x8[4] || Char > CharTable6x8[5])
#endif
		{
			//chWidth_Tmp = chWidth;
			chWidth = 0;
		}
		else
		{
			unsigned char Temp;
			if(CompactWriting)
			{
				for (Tmp = 1;Tmp < chWidth;Tmp++)
				{
#ifdef FLASH_DEVICE
					Temp = pgm_read_byte(&CharTable6x8[Tmp + CharPtr]);
#else
					Temp = CharTable6x8[Tmp + CharPtr];
#endif
					if(Temp == 0) break;
				}
				Tmp++;
			}
			else
			{
				Tmp = chWidth;
			}
		}
		Cursor_X += Tmp;
		pcString++;
		CharCnt++;
	}while(1);
}

