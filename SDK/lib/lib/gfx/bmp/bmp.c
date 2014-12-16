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
#include "ffconf.h"		/* FatFs configuration options */
#include "lib/fat_fs/inc/ff.h"

#define	BYTE_PER_PIXEL  3

static bool fill_bitmap(tDisplay *pDisplay, unsigned int *dest_buff, FIL *g_sFilObject, bm_info_header_t *bm_info_header, unsigned char* data, signed int X, signed int Y, bool use_transparency, unsigned long bm_size, unsigned int X_len, unsigned int Y_len)
{
	unsigned int k = 0,l = 0;
	unsigned char  *bm = data;
	signed short X_Coordinate = 0;
	signed short Y_Coordinate = 0;
	volatile unsigned short BitCount = bm_info_header->biBitCount;
	unsigned int Temp;
	unsigned char  *mask;
	unsigned int BitColourUsed;
	unsigned int color;
	int	i, j, pos = 0;
	int	row = bm_info_header->biHeight;
	int	col = bm_info_header->biWidth;
	unsigned char			blue, red, green;
	unsigned short int		rowMultiplier;
	unsigned char*			pData;
    FRESULT fresult;
    unsigned long bytes_read = 0;
    bool return_status = true;
    unsigned char *file;
    unsigned int *destination_buffer = dest_buff;
	switch(BitCount) {
		case(32):
		case(24):

			pos = bm_info_header->biSizeImage;
			rowMultiplier = col * 3;
			if(BitCount == 32)
				rowMultiplier = col * 4;
			else
				rowMultiplier = col * 3;
			if(rowMultiplier%4)				/*If not a multilier of 4 then pad rowMultiplier with zeros */
			{
				int extraZeros = 4 - ((col*BYTE_PER_PIXEL)%4);
				rowMultiplier += extraZeros;
			}
			if(!g_sFilObject) {
			pData = data;
			} else {
				pData = malloc(rowMultiplier);
			}
			for(i = (row - 1); i >= 0; i--) {
				pos = 0;
				fresult = f_read(g_sFilObject, pData, rowMultiplier, &bytes_read);
				if(fresult) return false;
				for(j = 0; j < col; j++) {
					blue = pData[pos++];
					green = pData[pos++];
					red = pData[pos++];
					color = BGR_TO_UINT(blue, green, red);
					if(!dest_buff)
						put_pixel(pDisplay, X + j, Y + i, color << 8);
					else
						*destination_buffer++ = color;
					if(BitCount == 32)
						pos++;
				}
				if(!g_sFilObject)
					pData += rowMultiplier;
			}
			if(g_sFilObject)
				if(pData) free(pData);
			break;
		case(8):
			Y_Coordinate = Y + (row - 1);
			if(g_sFilObject) {
				file = bm = malloc(bm_size - (sizeof(bm_info_header_t) + sizeof(bm_file_header_t)));
				fresult = f_read(g_sFilObject, bm, bm_size - (sizeof(bm_info_header_t) + sizeof(bm_file_header_t)), &bytes_read);
				bm += (unsigned int)data - 54;
			}
			BitColourUsed = bm_info_header->biClrUsed;
			if ((unsigned char)bm_info_header->biClrUsed == 0){
				BitColourUsed = 256;
			}
			Temp = 0;
			mask = (unsigned char*)(bm - (BitColourUsed * 4));
			for (l = 0; l < row; l++) {
				X_Coordinate = X;
				for (k = 0; k < col; k++) {
					Temp = *bm;
					Temp = Temp<<2;
					color = BGR_TO_UINT(*(mask + Temp), *(mask + Temp + 1), *(mask + Temp + 2));
					if(!dest_buff)
						put_pixel(pDisplay, X_Coordinate, Y_Coordinate, color << 8);
					else
						*destination_buffer++ = color;
					X_Coordinate++;
					bm++;
				}
				Y_Coordinate--;
				bm += 4 - (col%4);				/*If not a multilier of 4 then pad rowMultiplier with zeros */
			}
			if(g_sFilObject)
				if(file) free(file);
			break;
		default:
			return_status = false;
	}
	return return_status;
}
//-----------------------------------------------------
void bitmap_swap_endian_blk(unsigned char *pblock, int size)
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
bool put_bitmap(tDisplay *pDisplay, unsigned char *file, signed int X, signed int Y, bool use_transparency)
{
	bm_file_header_t bm_file_header;
	bm_info_header_t bm_info_header;
	unsigned char* data_start;
	/* get file header and header information */
	memcpy(&bm_info_header, file + 14, sizeof(bm_info_header_t));
	memcpy(&bm_file_header, file, sizeof(bm_file_header_t));
	//bitmap_swap_endian_blk((unsigned char *) &bm_file_header.bfType, 2);
	//bitmap_swap_endian_blk((unsigned char *) &bm_file_header.bfSize, 4);
	//bitmap_swap_endian_blk((unsigned char *) &bm_file_header.bfOffBits, 4);
	//bitmap_swap_endian_blk((unsigned char *) &bm_info_header.biWidth, 4);
	//bitmap_swap_endian_blk((unsigned char *) &bm_info_header.biHeight, 4);
	//bitmap_swap_endian_blk((unsigned char *) &bm_info_header.biBitCount, 2);
	//bitmap_swap_endian_blk((unsigned char *) &bm_info_header.biCompression, 4);
	data_start = file + bm_file_header.bfOffBits;
	return fill_bitmap(pDisplay, NULL, NULL, &bm_info_header, data_start, X, Y, use_transparency, bm_file_header.bfSize, 0, 0);
}
//-----------------------------------------------------
bool put_bbitmap(unsigned int *dest_buff, unsigned int dest_buff_x_len, unsigned int dest_buff_y_len, unsigned char *file, signed int X, signed int Y, bool use_transparency)
{
	bm_file_header_t bm_file_header;
	bm_info_header_t bm_info_header;
	unsigned char* data_start;
	/* get file header and header information */
	memcpy(&bm_info_header, file + 14, sizeof(bm_info_header_t));
	memcpy(&bm_file_header, file, sizeof(bm_file_header_t));
	//bitmap_swap_endian_blk((unsigned char *) &bm_file_header.bfType, 2);
	//bitmap_swap_endian_blk((unsigned char *) &bm_file_header.bfSize, 4);
	//bitmap_swap_endian_blk((unsigned char *) &bm_file_header.bfOffBits, 4);
	//bitmap_swap_endian_blk((unsigned char *) &bm_info_header.biWidth, 4);
	//bitmap_swap_endian_blk((unsigned char *) &bm_info_header.biHeight, 4);
	//bitmap_swap_endian_blk((unsigned char *) &bm_info_header.biBitCount, 2);
	//bitmap_swap_endian_blk((unsigned char *) &bm_info_header.biCompression, 4);
	data_start = file + bm_file_header.bfOffBits;
	return fill_bitmap(NULL, dest_buff, NULL, &bm_info_header, data_start, X, Y, use_transparency, bm_file_header.bfSize, dest_buff_x_len, dest_buff_y_len);
}
//-----------------------------------------------------
bm_info_header_t *bitmap_properties_get(unsigned char *file)
{
	bm_info_header_t *return_data = malloc(sizeof(bm_info_header_t));
	memcpy(&return_data, file + 14, sizeof(bm_info_header_t));
	return return_data;
}
//-----------------------------------------------------
FRESULT put_fbitmap(tDisplay *pDisplay, unsigned char *path, signed int X, signed int Y, bool use_transparency)
{
    FRESULT fresult;
    FIL g_sFilObject;
	fresult = f_open(&g_sFilObject, (char *)path, FA_READ);
	bm_file_header_t bm_file_header;
	bm_info_header_t bm_info_header;
	/* get file header and header information */
    unsigned long bytes_read = 0;
	fresult = f_read(&g_sFilObject, &bm_file_header, sizeof(bm_file_header_t), &bytes_read);
	if(fresult) return fresult;
	fresult = f_read(&g_sFilObject, &bm_info_header, sizeof(bm_info_header_t), &bytes_read);
	if(fresult) return fresult;
	return fill_bitmap(pDisplay, NULL, &g_sFilObject, &bm_info_header, (unsigned char *)bm_file_header.bfOffBits, X, Y, use_transparency, bm_file_header.bfSize, 0, 0);
}
//-----------------------------------------------------
FRESULT put_fbbitmap(unsigned int *dest_buff, unsigned int dest_buff_x_len, unsigned int dest_buff_y_len, unsigned char *path, signed int X, signed int Y, bool use_transparency)
{
    FRESULT fresult;
    FIL g_sFilObject;
	fresult = f_open(&g_sFilObject, (char *)path, FA_READ);
	bm_file_header_t bm_file_header;
	bm_info_header_t bm_info_header;
	/* get file header and header information */
    unsigned long bytes_read = 0;
	fresult = f_read(&g_sFilObject, &bm_file_header, sizeof(bm_file_header_t), &bytes_read);
	if(fresult) return fresult;
	fresult = f_read(&g_sFilObject, &bm_info_header, sizeof(bm_info_header_t), &bytes_read);
	if(fresult) return fresult;
	return fill_bitmap(NULL, dest_buff, &g_sFilObject, &bm_info_header, (unsigned char *)bm_file_header.bfOffBits, X, Y, use_transparency, bm_file_header.bfSize, dest_buff_x_len, dest_buff_y_len);
}
//-----------------------------------------------------
bm_info_header_t *fbitmap_properties_get(unsigned char *file)
{
	bm_info_header_t *return_data = malloc(sizeof(bm_info_header_t));
	memcpy(&return_data, file + 14, sizeof(bm_info_header_t));
	return return_data;
}
//#####################################################
