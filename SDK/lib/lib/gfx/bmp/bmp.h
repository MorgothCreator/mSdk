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

//#####################################################
#ifndef False
#define False	0
#endif

#ifndef True
#define True	1
#endif

#ifndef Null
#define Null	-1
#endif
//#####################################################

typedef enum
{
	BMP_FromRamMemory = 0,
	BMP_FromFlashMemory,
	BMP_FromFileSystemMemory
}BMP_Source_MemoryType;


typedef struct bm_info_header_s {
	/*! \brief Size of the information header */
 	unsigned int biSize;
 	/*! \brief Width of the bitmap in pixels */
	signed int biWidth;
	/*! \brief Height of the bitmap in pixels
	 * 	If this value is positiv -> "bottom-up"-bitmap (picture data starts with the bottom
	 * line and ends with the top line)
     * If this value is negativ ->"top-down"-Bitmap (picture data starts with the top line
     * and ends with the bottom line)
	 */
	signed int biHeight;
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
	unsigned int 	biCompression;
	/*! \brief Image size. This is the size of the raw bitmap data. */
	unsigned int 	biSizeImage;
	/*! \brief Horizontale resolution */
	unsigned int 	biXPelsPerMeter;
	/*! \brief Vertical resolution */
	unsigned int 	biYPelsPerMeter;
 	unsigned int 	biClrUsed;
	unsigned int 	biClrImportant;
}/*__attribute__((__packed__))*/ bm_info_header_t;

typedef struct bm_file_header_s {
	unsigned short bfType;
	/*! \brief Size of the bitmap file */
	unsigned int bfSize;
	/*! \brief Reserved. May be used by some software */
	unsigned int bfReserved;
	/*! \brief Offset in bytes where the bitmap data can be found */
	unsigned int bfOffBits;
} /*__attribute__((__packed__))*/ bm_file_header_t ;
//-----------------------------------------------------
typedef struct bmp_return_struct
{
	unsigned short HeightBmp;
	unsigned short WidthBmp;
	unsigned char BitCount;
} bmp_return_t;
//-----------------------------------------------------
//unsigned char fill_frame_buffer(struct bm_info_header_s *bm_info_header, /*put_str_t *BmpLimits,*/ unsigned char* data_start, signed short X, signed short Y, unsigned char SourceMemory);
bmp_return_t paint_bitmap(tDisplay *pDisplay, unsigned char *file_start, /*struct put_str_t *BmpLimits,*/ signed short X, signed short Y, unsigned char Paint);

#ifdef HEADER_INCLUDE_C_FILES
#include "bmp.c"
#endif

#endif /* BMP_C_ */
