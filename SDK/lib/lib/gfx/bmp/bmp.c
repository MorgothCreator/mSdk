/*
 *  lib/bmp/bmp.c
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

#include <stdbool.h>
#include "../../../api/lcd_api.h"
#include "bmp.h"

static bool fill_frame_buffer(tDisplay *pDisplay, struct bm_info_header_s *bm_info_header, /*put_str_t *BmpLimits,*/ unsigned char* data_start, signed short X, signed short Y, unsigned char SourceMemory)
{

	unsigned int k = 0,l = 0;
	unsigned char  *bm = data_start;
//	unsigned char red = 0,green = 0,blue = 0;
	signed short X_Coordinate = 0;
	signed short Y_Coordinate = 0;
	unsigned int Tmp2 = bm_info_header->biWidth&0x3;
	unsigned int Tmp3 = (0 - ((unsigned char)bm_info_header->biWidth &0x3)) & 0x3;
	volatile unsigned short BitCount = bm_info_header->biBitCount;
	unsigned int Temp;
	unsigned int  *mask;
	unsigned int BitColourUsed;
	//struct rgb_8 color;
	unsigned int color;
	switch(BitCount)
	{
		case(32):
		case(24):
#ifdef LcdArea_Integrated
			lcd_area(X, Y, X+bm_info_header->biWidth-1, Y+bm_info_header->biHeight-1);
			lcd_drawstart();
#endif
			Y_Coordinate = Y;
			if(bm_info_header->biHeight > 0)
			{
				bm += ((BitCount * bm_info_header->biWidth * (bm_info_header->biHeight - 1)) / 8);//    (((bm_info_header->biWidth) * (Y+bm_info_header->biHeight-1)) * BitCount >> 3) - ((bm_info_header->biWidth-1) * BitCount >> 3) ;
				bm += Tmp2*(bm_info_header->biHeight - 1);
			}
			for (l = 0 ; l < bm_info_header->biHeight; l++)
			{
				X_Coordinate = X;
				for (k = 0; k < bm_info_header->biWidth; k++)
				{
#ifdef Dx_UseMarginLimits
#ifndef LcdArea_Integrated
					if(X_Coordinate >= BmpLimits->LeftEdgeLimit && X_Coordinate <= (BmpLimits->LeftEdgeLimit + BmpLimits->RightEdgeLimit) && Y_Coordinate >= BmpLimits->UpEdgeLimit && Y_Coordinate <= (BmpLimits->UpEdgeLimit + BmpLimits->DownEdgeLimit))
					{
#endif
#endif
						//color.red = (*bm);// >> 3;
						//color.green = (*(bm +1));// >> 2;
						//color.blue = (*(bm + 2));// >> 3;
						color = RGB_TO_UINT((*bm), (*(bm +1)), (*(bm + 2)));
#ifdef LcdArea_Integrated
						unsigned short TempColor = ((red>>3)&0b11111) | ((green<<3)&0b11111100000) | ((blue<<8)&0b1111100000000000);
						lcd_rw_data(TempColor);
#else
						//pContext->ulForeground = color;
						put_pixel(pDisplay, X_Coordinate,Y_Coordinate, color);
#endif
#ifdef Dx_UseMarginLimits
#ifndef LcdArea_Integrated
					}
#endif
#endif
					X_Coordinate++;
					bm += BitCount >> 3;
				}
				Y_Coordinate++;
				if(bm_info_header->biHeight < 0)
				{
					if(BitCount == 24)
					{
						bm += Tmp2;
					}
				}
				else
				{
					if(BitCount == 24)
					{
						bm -= Tmp2;
					}
					bm -= BitCount * 2 * bm_info_header->biWidth / 8;//bm_info_header->biBitCount *2* bm_info_header->biWidth / 8;
				}
			}
#ifdef LcdArea_Integrated
			lcd_drawstop();
#endif
			break;
		case(8):
			Y_Coordinate = Y;
			BitColourUsed = bm_info_header->biClrUsed;
			if ((unsigned char)bm_info_header->biClrUsed == 0)
			{
				BitColourUsed = 256;
			}
			Temp = 0;
			mask = (unsigned int*)(bm - (BitColourUsed * 4));
			if(bm_info_header->biHeight > 0)
			{
				bm += (bm_info_header->biWidth - 1) * (bm_info_header->biHeight - 1);//    (((bm_info_header->biWidth) * (Y+bm_info_header->biHeight-1)) * BitCount >> 3) - ((bm_info_header->biWidth-1) * BitCount >> 3) ;
				bm += ((Tmp3 & 0x3)+1) * (bm_info_header->biHeight - 1);
			}
			//if(SourceMemory == BMP_FromFileSystemMemory)
			//{
			//	_FatData_GoToIndex(fileinfo, (unsigned int)(unsigned short)bm);
			///	memcpy(IndexBuffer, fileinfo->FileInfo_BufferAddr, 512);
			//	memcpy(DataBuffer, fileinfo->FileInfo_BufferAddr, 512);
			//}
#ifdef LcdArea_Integrated
			lcd_area(X, Y, (X+bm_info_header->biWidth), (Y+bm_info_header->biHeight-1));
			lcd_drawstart();
#endif
			for (l = 0 ; l < bm_info_header->biHeight; l++)
			{
				X_Coordinate = X;
				for (k = 0; k <= bm_info_header->biWidth; k++)
				{
#ifdef Dx_UseMarginLimits
#ifdef Dx_UseMarginLimits
					if(X_Coordinate >= BmpLimits->LeftEdgeLimit && X_Coordinate <= (BmpLimits->LeftEdgeLimit + BmpLimits->RightEdgeLimit) && Y_Coordinate >= BmpLimits->UpEdgeLimit && Y_Coordinate <= (BmpLimits->UpEdgeLimit + BmpLimits->DownEdgeLimit))
					{
#endif
#endif
						Temp = *bm;
						Temp = Temp<<2;
						//color.red = *(mask + Temp);
						//color.green = *(mask + Temp + 1);
						//color.blue = *(mask + Temp + 2);
						color = RGB_TO_UINT(*(mask + Temp), *(mask + Temp + 1), *(mask + Temp + 2));
#ifdef LcdArea_Integrated
						unsigned short TempColor = ((blue>>3)&0b11111) | ((green<<3)&0b11111100000) | ((red<<8)&0b1111100000000000);
						lcd_rw_data(TempColor);
#else
						//pContext->ulForeground = color;
						put_pixel(pDisplay, X_Coordinate,Y_Coordinate, color);
#endif
#ifdef Dx_UseMarginLimits
#ifdef Dx_UseMarginLimits
					}
#endif
#endif
					X_Coordinate++;
					bm++;
				}
				Y_Coordinate++;
				if(bm_info_header->biHeight < 0) bm += Tmp3;
				else
				{
					bm -= Tmp3+1;
					bm -= 2 * bm_info_header->biWidth;
				}


				//bm += Tmp3;
			}
#ifdef LcdArea_Integrated
			lcd_drawstop();
#endif
			break;
		default:
			return false;
	}
	return true;
}
//-----------------------------------------------------
void swap_endian_blk(unsigned char *pblock, int size)
{
        unsigned char *pend = &pblock[size - 1];
        unsigned char temp;
        while(pblock < pend)
        {
                temp = *pblock;
                *pblock = *pend;
                *pend = temp;
                pblock++;
                pend--;
        }
}
//-----------------------------------------------------
bmp_return_t paint_bitmap(tDisplay *pDisplay, unsigned char *file_start, /*struct put_str_t *BmpLimits,*/ signed short X, signed short Y, unsigned char Paint)
{
	struct bm_file_header_s bm_file_header;
	struct bm_info_header_s bm_info_header;
	unsigned char* data_start;

	/* get file header and header information */
	memcpy(&bm_info_header, file_start + 14, sizeof(bm_info_header_t));
	if(Paint)
	{
		memcpy(&bm_file_header, file_start, sizeof(bm_file_header_t));
		swap_endian_blk((unsigned char *) &bm_file_header.bfType, 2);
		swap_endian_blk((unsigned char *) &bm_file_header.bfSize, 4);
		swap_endian_blk((unsigned char *) &bm_file_header.bfOffBits, 4);
		swap_endian_blk((unsigned char *) &bm_info_header.biWidth, 4);
		swap_endian_blk((unsigned char *) &bm_info_header.biHeight, 4);
		swap_endian_blk((unsigned char *) &bm_info_header.biBitCount, 2);
		swap_endian_blk((unsigned char *) &bm_info_header.biCompression, 4);
		//fill_frame_buffer_bm( &bm_info_header, data_start);
		data_start = file_start + bm_file_header.bfOffBits;
		fill_frame_buffer(pDisplay, &bm_info_header, /*BmpLimits,*/ data_start, X, Y, BMP_FromRamMemory);
	}
	struct bmp_return_struct BmpReturn;
	BmpReturn.HeightBmp = bm_info_header.biHeight & 0x7FFF;//0b0111111111111111;
	BmpReturn.WidthBmp = bm_info_header.biWidth & 0x7FFF;//0b0111111111111111;
	BmpReturn.BitCount = bm_info_header.biBitCount;
	if((unsigned char)bm_info_header.biBitCount == 8) BmpReturn.WidthBmp++;
	return BmpReturn;
}
//-----------------------------------------------------
//#####################################################
