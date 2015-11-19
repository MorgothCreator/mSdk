/*
 *  lib/bmp/bmp.h
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

#ifndef BMP_C_
#define BMP_C_
#define __Fat__
#include <stdbool.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include "../gfx_util.h"
#include "../controls_definition.h"
#include "lib/fat_fs/inc/ff.h"

//#####################################################
typedef struct bm_info_header_s {
	/*! \brief Size of the information header */
 	unsigned long biSize;
 	/*! \brief Width of the bitmap in pixels */
 	unsigned long biWidth;
	/*! \brief Height of the bitmap in pixels
	 * 	If this value is positiv -> "bottom-up"-bitmap (picture data starts with the bottom
	 * line and ends with the top line)
     * If this value is negativ ->"top-down"-Bitmap (picture data starts with the top line
     * and ends with the bottom line)
	 */
 	unsigned long biHeight;
	/*! \brief Number of color planes being used. Not often used. */
	unsigned short 	biPlanes;
	/*! \brief Number of bits per pixel, which is the color depth of the image.
	 *  Typical values are 1, 4, 8, 16, 24 and 32.
	 */
	unsigned short biBitCount;
	/*! \brief Define the compression method being used
	 *  0 (BI_RGB): No compression
	 *  1 (BI_RLE8):
	 *  2 (BI_RLE4):
	 *  3 (BI_BITFIELDS):
	 */
	unsigned long 	biCompression;
	/*! \brief Image size. This is the size of the raw bitmap data. */
	unsigned long 	biSizeImage;
	/*! \brief Horizontale resolution */
	unsigned long 	biXPelsPerMeter;
	/*! \brief Vertical resolution */
	unsigned long 	biYPelsPerMeter;
 	unsigned long 	biClrUsed;
	unsigned long 	biClrImportant;
}__attribute__((__packed__))bm_info_header_t;
//#####################################################
typedef struct bm_file_header_s {
	unsigned short bfType;
	/*! \brief Size of the bitmap file */
	unsigned long bfSize;
	/*! \brief Reserved. May be used by some software */
	unsigned long bfReserved;
	/*! \brief Offset in bytes where the bitmap data can be found */
	unsigned long bfOffBits;
}__attribute__((__packed__))bm_file_header_t;
//#####################################################
bool put_bitmap(tDisplay *pDisplay, unsigned char *file, signed int X, signed int Y, bool use_transparency);
bool put_bbitmap(unsigned int *dest_buff, unsigned int dest_buff_x_len, unsigned int dest_buff_y_len, unsigned char *file, signed int X, signed int Y, bool use_transparency);
bm_info_header_t *bitmap_properties_get(unsigned char *file);
FRESULT put_fbitmap(tDisplay *pDisplay, char *path, signed int X, signed int Y, bool use_transparency);
FRESULT put_fbbitmap(unsigned int *dest_buff, unsigned int dest_buff_x_len, unsigned int dest_buff_y_len, char *path, signed int X, signed int Y, bool use_transparency);
bm_info_header_t *fbitmap_properties_get(unsigned char *file);
//#####################################################
#ifdef HEADER_INCLUDE_C_FILES
#include "bmp.c"
#endif
//#####################################################
#endif /* BMP_C_ */
