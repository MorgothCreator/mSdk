/*
 *  lib/api/lcd_def.h
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

#ifndef LCD_DEF_H_
#define LCD_DEF_H_
/**********************************************/
#include <stdbool.h>
#include "twi_def.h"
#include "gpio_def.h"
#include "sys/plat_properties.h"
/**********************************************/
#ifdef FLASH_DEVICE
#define read_data_byte(addr) pgm_read_byte(&addr)
#define read_data_word(addr) pgm_read_word(&addr)
#else
#define read_data_byte(addr) addr
#define read_data_word(addr) addr
#endif
/**********************************************/
/**********************************************/
#define S035Q01 0
#define AT070TN92 1
#define TFT43AB_OMAP35x 2
#define VGA 3
#define LVDS 4
#define MI0283 5
#define HD 6
#define FHD 7
//*****************************************************************************

//*****************************************************************************
typedef struct
{
	unsigned int X;
	unsigned int Y;
	unsigned long ref_clk;
	unsigned int pll_m;
	unsigned int pll_n;
	unsigned int pll_m2;
	unsigned long raster_clk;
	unsigned int palete_len;
	unsigned int bus_size;
	unsigned int hsw;
	unsigned int hfp;
	unsigned int hbp;
	unsigned int vsw;
	unsigned int vfp;
	unsigned int vbp;
}RASTER_TIMINGS;
//*****************************************************************************
RASTER_TIMINGS lcd_S035Q01_beaglebone_exp;
RASTER_TIMINGS lcd_TFT43AB_OMAP35x_beaglebone_exp;
RASTER_TIMINGS lcd_AT070TN92_beaglebone_exp;
RASTER_TIMINGS lcd_720p_480_60hz_beaglebone_exp;
RASTER_TIMINGS lcd_720p_50hz_beaglebone_exp;
RASTER_TIMINGS lcd_1080p_24hz_beaglebone_exp;
//*****************************************************************************
//
//! This structure defines the extents of a rectangle.  All points greater than
//! or equal to the minimum and less than or equal to the maximum are part of
//! the rectangle.
//
//*****************************************************************************
typedef struct
{
    //
    //! The minimum X coordinate of the rectangle.
    //
	signed int sXMin;

    //
    //! The minimum Y coordinate of the rectangle.
    //
	signed int sYMin;

    //
    //! The maximum X coordinate of the rectangle.
    //
	signed int sXMax;

    //
    //! The maximum Y coordinate of the rectangle.
    //
	signed int sYMax;
}
tRectangle;
//*****************************************************************************
//
//! This structure describes a font used for drawing text onto the screen.
//
//*****************************************************************************
typedef struct
{
    //
    //! The format of the font.  Can be one of FONT_FMT_UNCOMPRESSED or
    //! FONT_FMT_PIXEL_RLE.
    //
    unsigned char ucFormat;

    //
    //! The maximum width of a character; this is the width of the widest
    //! character in the font, though any individual character may be narrower
    //! than this width.
    //
    signed char ucMaxWidth;

    //
    //! The height of the character cell; this may be taller than the font data
    //! for the characters (to provide inter-line spacing).
    //
    signed char ucHeight;

    //
    //! The offset between the top of the character cell and the baseline of
    //! the glyph.  The baseline is the bottom row of a capital letter, below
    //! which only the descenders of the lower case letters occur.
    //
    unsigned char ucBaseline;

    //
    //! The offset within pucData to the data for each character in the font.
    //
    unsigned short pusOffset[96];

    //
    //! A pointer to the data for the font.
    //
    const unsigned char *pucData;
}
tFont;
/**********************************************/
typedef struct sDisplay
{
	unsigned short Orientation;
	//signed int Width;
	//signed int Height;
	//unsigned int LcdType;
	bool touch_invert_x;
	bool touch_invert_y;
	bool pmic_back_light;
	bool invert_backlight;
	volatile unsigned int* DisplayData;
	unsigned int BackLightPort;
	unsigned int BackLightPin;
	unsigned int BackLightLevel;
	unsigned int BackColor;
	unsigned int InkColor;
	signed int Buff_Offset;
	volatile bool TransferComplte;
    tRectangle sClipRegion;
	Gpio_t* BackLight;
	Twi_t* PmicTwiModuleStruct;
	RASTER_TIMINGS *raster_timings;
	RASTER_TIMINGS *old_raster_timings;
	void *UserData;
}tDisplay;
//*****************************************************************************
//
//! This structure defines a drawing context to be used to draw onto the
//! screen.  Multiple drawing contexts may exist at any time.
//
//*****************************************************************************
typedef struct
{
    //
    //! The size of this structure.
    //
    //int lSize;

    //
    //! The screen onto which drawing operations are performed.
    //
    tDisplay *pDisplay;

    //
    //! The clipping region to be used when drawing onto the screen.
    //
    tRectangle sClipRegion;



    void *pChildrenContext;

    //
    //! The color used to draw primitives onto the screen.
    //
    unsigned int ulForeground;

    //
    //! The background color used to draw primitives onto the screen.
    //
    unsigned int ulBackground;

    //
    //! The background color used to draw primitives onto the screen.
    //
    unsigned char ulOpaque;

    //
    //! The background color used to draw primitives onto the screen.
    //
    unsigned char ulWordWrap;

    //
    //! The background color used to draw primitives onto the screen.
    //
    unsigned char ulVisible;

    //
    //! The font used to render text onto the screen.
    //
    tFont *pFont;
}
tContext;
/**********************************************/
typedef struct StringProperties_s
{
	signed int FirstRowLength_Pixels;
	signed int FirstRowLength_Chars;
	signed int StringLengthWithSpecialChars_Chars;
	signed int StringLengthWithOutSpecialChars_Chars;
	signed int StringRowsMaxLength_Pixels;
	signed int StringColsHeight_Pixels;
	signed int StringColsHeight_Rows;
	signed int CharHeight_Pixels;
}StringProperties_t;
/**********************************************/
#define new_screen tDisplay
#ifndef new_
#define new_(structure) (structure*)calloc(1,sizeof(structure));
#endif
#define free_screen(address) free(address);
/**********************************************/
#define new_screen_context tContext
#ifndef new_
#define new_(structure) (structure*)calloc(1,sizeof(structure));
#endif
#define free_screen_context(address) free(address);
//*****************************************************************************
//
// A set of color definitions.  This set is the subset of the X11 colors (from
// rgb.txt) that are supported by typical web browsers.
//
//*****************************************************************************
#define ClrAliceBlue            0x00F0F8FF
#define ClrAntiqueWhite         0x00FAEBD7
#define ClrAqua                 0x0000FFFF
#define ClrAquamarine           0x007FFFD4
#define ClrAzure                0x00F0FFFF
#define ClrBeige                0x00F5F5DC
#define ClrBisque               0x00FFE4C4
#define ClrBlack                0x00000000
#define ClrBlanchedAlmond       0x00FFEBCD
#define ClrBlue                 0x000000FF
#define ClrBlueViolet           0x008A2BE2
#define ClrBrown                0x00A52A2A
#define ClrBurlyWood            0x00DEB887
#define ClrCadetBlue            0x005F9EA0
#define ClrChartreuse           0x007FFF00
#define ClrChocolate            0x00D2691E
#define ClrCoral                0x00FF7F50
#define ClrCornflowerBlue       0x006495ED
#define ClrCornsilk             0x00FFF8DC
#define ClrCrimson              0x00DC143C
#define ClrCyan                 0x0000FFFF
#define ClrDarkBlue             0x0000008B
#define ClrDarkCyan             0x00008B8B
#define ClrDarkGoldenrod        0x00B8860B
#define ClrDarkGray             0x00A9A9A9
#define ClrDarkGreen            0x00006400
#define ClrDarkKhaki            0x00BDB76B
#define ClrDarkMagenta          0x008B008B
#define ClrDarkOliveGreen       0x00556B2F
#define ClrDarkOrange           0x00FF8C00
#define ClrDarkOrchid           0x009932CC
#define ClrDarkRed              0x008B0000
#define ClrDarkSalmon           0x00E9967A
#define ClrDarkSeaGreen         0x008FBC8F
#define ClrDarkSlateBlue        0x00483D8B
#define ClrDarkSlateGray        0x002F4F4F
#define ClrDarkTurquoise        0x0000CED1
#define ClrDarkViolet           0x009400D3
#define ClrDeepPink             0x00FF1493
#define ClrDeepSkyBlue          0x0000BFFF
#define ClrDimGray              0x00696969
#define ClrDodgerBlue           0x001E90FF
#define ClrFireBrick            0x00B22222
#define ClrFloralWhite          0x00FFFAF0
#define ClrForestGreen          0x00228B22
#define ClrFuchsia              0x00FF00FF
#define ClrGainsboro            0x00DCDCDC
#define ClrGhostWhite           0x00F8F8FF
#define ClrGold                 0x00FFD700
#define ClrGoldenrod            0x00DAA520
#define ClrGray                 0x00808080
#define ClrGreen                0x00008000
#define ClrGreenYellow          0x00ADFF2F
#define ClrHoneydew             0x00F0FFF0
#define ClrHotPink              0x00FF69B4
#define ClrIndianRed            0x00CD5C5C
#define ClrIndigo               0x004B0082
#define ClrIvory                0x00FFFFF0
#define ClrKhaki                0x00F0E68C
#define ClrLavender             0x00E6E6FA
#define ClrLavenderBlush        0x00FFF0F5
#define ClrLawnGreen            0x007CFC00
#define ClrLemonChiffon         0x00FFFACD
#define ClrLightBlue            0x00ADD8E6
#define ClrLightCoral           0x00F08080
#define ClrLightCyan            0x00E0FFFF
#define ClrLightGoldenrodYellow 0x00FAFAD2
#define ClrLightGreen           0x0090EE90
#define ClrLightGrey            0x00D3D3D3
#define ClrLightPink            0x00FFB6C1
#define ClrLightSalmon          0x00FFA07A
#define ClrLightSeaGreen        0x0020B2AA
#define ClrLightSkyBlue         0x0087CEFA
#define ClrLightSlateGray       0x00778899
#define ClrLightSteelBlue       0x00B0C4DE
#define ClrLightYellow          0x00FFFFE0
#define ClrLime                 0x0000FF00
#define ClrLimeGreen            0x0032CD32
#define ClrLinen                0x00FAF0E6
#define ClrMagenta              0x00FF00FF
#define ClrMaroon               0x00800000
#define ClrMediumAquamarine     0x0066CDAA
#define ClrMediumBlue           0x000000CD
#define ClrMediumOrchid         0x00BA55D3
#define ClrMediumPurple         0x009370DB
#define ClrMediumSeaGreen       0x003CB371
#define ClrMediumSlateBlue      0x007B68EE
#define ClrMediumSpringGreen    0x0000FA9A
#define ClrMediumTurquoise      0x0048D1CC
#define ClrMediumVioletRed      0x00C71585
#define ClrMidnightBlue         0x00191970
#define ClrMintCream            0x00F5FFFA
#define ClrMistyRose            0x00FFE4E1
#define ClrMoccasin             0x00FFE4B5
#define ClrNavajoWhite          0x00FFDEAD
#define ClrNavy                 0x00000080
#define ClrOldLace              0x00FDF5E6
#define ClrOlive                0x00808000
#define ClrOliveDrab            0x006B8E23
#define ClrOrange               0x00FFA500
#define ClrOrangeRed            0x00FF4500
#define ClrOrchid               0x00DA70D6
#define ClrPaleGoldenrod        0x00EEE8AA
#define ClrPaleGreen            0x0098FB98
#define ClrPaleTurquoise        0x00AFEEEE
#define ClrPaleVioletRed        0x00DB7093
#define ClrPapayaWhip           0x00FFEFD5
#define ClrPeachPuff            0x00FFDAB9
#define ClrPeru                 0x00CD853F
#define ClrPink                 0x00FFC0CB
#define ClrPlum                 0x00DDA0DD
#define ClrPowderBlue           0x00B0E0E6
#define ClrPurple               0x00800080
#define ClrRed                  0x00FF0000
#define ClrRosyBrown            0x00BC8F8F
#define ClrRoyalBlue            0x004169E1
#define ClrSaddleBrown          0x008B4513
#define ClrSalmon               0x00FA8072
#define ClrSandyBrown           0x00F4A460
#define ClrSeaGreen             0x002E8B57
#define ClrSeashell             0x00FFF5EE
#define ClrSienna               0x00A0522D
#define ClrSilver               0x00C0C0C0
#define ClrSkyBlue              0x0087CEEB
#define ClrSlateBlue            0x006A5ACD
#define ClrSlateGray            0x00708090
#define ClrSnow                 0x00FFFAFA
#define ClrSpringGreen          0x0000FF7F
#define ClrSteelBlue            0x004682B4
#define ClrTan                  0x00D2B48C
#define ClrTeal                 0x00008080
#define ClrThistle              0x00D8BFD8
#define ClrTomato               0x00FF6347
#define ClrTurquoise            0x0040E0D0
#define ClrViolet               0x00EE82EE
#define ClrWheat                0x00F5DEB3
#define ClrWhite                0x00FFFFFF
#define ClrWhiteSmoke           0x00F5F5F5
#define ClrYellow               0x00FFFF00
#define ClrYellowGreen          0x009ACD32

//*****************************************************************************
//
//! Indicates that the font data is stored in an uncompressed format.
//
//*****************************************************************************
#define FONT_FMT_UNCOMPRESSED   0x00

//*****************************************************************************
//
//! Indicates that the font data is stored using a pixel-based RLE format.
//
//*****************************************************************************
#define FONT_FMT_PIXEL_RLE      0x01

//*****************************************************************************
//
//! Indicates that the image data is not compressed and represents each pixel
//! with a single bit.
//
//*****************************************************************************
#define IMAGE_FMT_1BPP_UNCOMP   0x01

//*****************************************************************************
//
//! Indicates that the image data is not compressed and represents each pixel
//! with four bits.
//
//*****************************************************************************
#define IMAGE_FMT_4BPP_UNCOMP   0x04

//*****************************************************************************
//
//! Indicates that the image data is not compressed and represents each pixel
//! with eight bits.
//
//*****************************************************************************
#define IMAGE_FMT_8BPP_UNCOMP   0x08

//*****************************************************************************
//
//! Indicates that the image data is compressed and represents each pixel with
//! a single bit.
//
//*****************************************************************************
#define IMAGE_FMT_1BPP_COMP     0x81

//*****************************************************************************
//
//! Indicates that the image data is compressed and represents each pixel with
//! four bits.
//
//*****************************************************************************
#define IMAGE_FMT_4BPP_COMP     0x84

//*****************************************************************************
//
//! Indicates that the image data is compressed and represents each pixel with
//! eight bits.
//
//*****************************************************************************
#define IMAGE_FMT_8BPP_COMP     0x88

#define IMAGE_FMT_16BPP_UNCOMP   0x10
#define IMAGE_FMT_16BPP_COMP     0x90

#define IMAGE_FMT_24BPP_UNCOMP   0x20
#define IMAGE_FMT_24BPP_COMP     0xA0

//*****************************************************************************
//
// Prototypes for the predefined fonts in the graphics library.  ..Cm.. is the
// computer modern font, which is a serif font.  ..Cmsc.. is the computer
// modern small-caps font, which is also a serif font.  ..Cmss.. is the
// computer modern sans-serif font.
//
//*****************************************************************************
extern const tFont g_sFontCm12;
extern const tFont g_sFontCm12b;
extern const tFont g_sFontCm12i;
extern const tFont g_sFontCm14;
extern const tFont g_sFontCm14b;
extern const tFont g_sFontCm14i;
extern const tFont g_sFontCm16;
extern const tFont g_sFontCm16b;
extern const tFont g_sFontCm16i;
extern const tFont g_sFontCm18;
extern const tFont g_sFontCm18b;
extern const tFont g_sFontCm18i;
extern const tFont g_sFontCm20;
extern const tFont g_sFontCm20b;
extern const tFont g_sFontCm20i;
extern const tFont g_sFontCm22;
extern const tFont g_sFontCm22b;
extern const tFont g_sFontCm22i;
extern const tFont g_sFontCm24;
extern const tFont g_sFontCm24b;
extern const tFont g_sFontCm24i;
extern const tFont g_sFontCm26;
extern const tFont g_sFontCm26b;
extern const tFont g_sFontCm26i;
extern const tFont g_sFontCm28;
extern const tFont g_sFontCm28b;
extern const tFont g_sFontCm28i;
extern const tFont g_sFontCm30;
extern const tFont g_sFontCm30b;
extern const tFont g_sFontCm30i;
extern const tFont g_sFontCm32;
extern const tFont g_sFontCm32b;
extern const tFont g_sFontCm32i;
extern const tFont g_sFontCm34;
extern const tFont g_sFontCm34b;
extern const tFont g_sFontCm34i;
extern const tFont g_sFontCm36;
extern const tFont g_sFontCm36b;
extern const tFont g_sFontCm36i;
extern const tFont g_sFontCm38;
extern const tFont g_sFontCm38b;
extern const tFont g_sFontCm38i;
extern const tFont g_sFontCm40;
extern const tFont g_sFontCm40b;
extern const tFont g_sFontCm40i;
extern const tFont g_sFontCm42;
extern const tFont g_sFontCm42b;
extern const tFont g_sFontCm42i;
extern const tFont g_sFontCm44;
extern const tFont g_sFontCm44b;
extern const tFont g_sFontCm44i;
extern const tFont g_sFontCm46;
extern const tFont g_sFontCm46b;
extern const tFont g_sFontCm46i;
extern const tFont g_sFontCm48;
extern const tFont g_sFontCm48b;
extern const tFont g_sFontCm48i;
extern const tFont g_sFontCmsc12;
extern const tFont g_sFontCmsc14;
extern const tFont g_sFontCmsc16;
extern const tFont g_sFontCmsc18;
extern const tFont g_sFontCmsc20;
extern const tFont g_sFontCmsc22;
extern const tFont g_sFontCmsc24;
extern const tFont g_sFontCmsc26;
extern const tFont g_sFontCmsc28;
extern const tFont g_sFontCmsc30;
extern const tFont g_sFontCmsc32;
extern const tFont g_sFontCmsc34;
extern const tFont g_sFontCmsc36;
extern const tFont g_sFontCmsc38;
extern const tFont g_sFontCmsc40;
extern const tFont g_sFontCmsc42;
extern const tFont g_sFontCmsc44;
extern const tFont g_sFontCmsc46;
extern const tFont g_sFontCmsc48;
extern const tFont g_sFontCmss12;
extern const tFont g_sFontCmss12b;
extern const tFont g_sFontCmss12i;
extern const tFont g_sFontCmss14;
extern const tFont g_sFontCmss14b;
extern const tFont g_sFontCmss14i;
extern const tFont g_sFontCmss16;
extern const tFont g_sFontCmss16b;
extern const tFont g_sFontCmss16i;
extern const tFont g_sFontCmss18;
extern const tFont g_sFontCmss18b;
extern const tFont g_sFontCmss18i;
extern const tFont g_sFontCmss20;
extern const tFont g_sFontCmss20b;
extern const tFont g_sFontCmss20i;
extern const tFont g_sFontCmss22;
extern const tFont g_sFontCmss22b;
extern const tFont g_sFontCmss22i;
extern const tFont g_sFontCmss24;
extern const tFont g_sFontCmss24b;
extern const tFont g_sFontCmss24i;
extern const tFont g_sFontCmss26;
extern const tFont g_sFontCmss26b;
extern const tFont g_sFontCmss26i;
extern const tFont g_sFontCmss28;
extern const tFont g_sFontCmss28b;
extern const tFont g_sFontCmss28i;
extern const tFont g_sFontCmss30;
extern const tFont g_sFontCmss30b;
extern const tFont g_sFontCmss30i;
extern const tFont g_sFontCmss32;
extern const tFont g_sFontCmss32b;
extern const tFont g_sFontCmss32i;
extern const tFont g_sFontCmss34;
extern const tFont g_sFontCmss34b;
extern const tFont g_sFontCmss34i;
extern const tFont g_sFontCmss36;
extern const tFont g_sFontCmss36b;
extern const tFont g_sFontCmss36i;
extern const tFont g_sFontCmss38;
extern const tFont g_sFontCmss38b;
extern const tFont g_sFontCmss38i;
extern const tFont g_sFontCmss40;
extern const tFont g_sFontCmss40b;
extern const tFont g_sFontCmss40i;
extern const tFont g_sFontCmss42;
extern const tFont g_sFontCmss42b;
extern const tFont g_sFontCmss42i;
extern const tFont g_sFontCmss44;
extern const tFont g_sFontCmss44b;
extern const tFont g_sFontCmss44i;
extern const tFont g_sFontCmss46;
extern const tFont g_sFontCmss46b;
extern const tFont g_sFontCmss46i;
extern const tFont g_sFontCmss48;
extern const tFont g_sFontCmss48b;
extern const tFont g_sFontCmss48i;
extern const tFont g_sFontCmtt12;
extern const tFont g_sFontCmtt14;
extern const tFont g_sFontCmtt16;
extern const tFont g_sFontCmtt18;
extern const tFont g_sFontCmtt20;
extern const tFont g_sFontCmtt22;
extern const tFont g_sFontCmtt24;
extern const tFont g_sFontCmtt26;
extern const tFont g_sFontCmtt28;
extern const tFont g_sFontCmtt30;
extern const tFont g_sFontCmtt32;
extern const tFont g_sFontCmtt34;
extern const tFont g_sFontCmtt36;
extern const tFont g_sFontCmtt38;
extern const tFont g_sFontCmtt40;
extern const tFont g_sFontCmtt42;
extern const tFont g_sFontCmtt44;
extern const tFont g_sFontCmtt46;
extern const tFont g_sFontCmtt48;
extern const tFont g_sFontFixed6x8;
/**********************************************/
#ifdef HEADER_INCLUDE_C_FILES
#include "lib/gfx/resource/fonts.c"
#endif
//*****************************************************************************
//
// Masks and shifts to aid in color format translation by drivers.
//
//*****************************************************************************
#define ClrRedMask              0x00FF0000
#define ClrRedShift             16
#define ClrGreenMask            0x0000FF00
#define ClrGreenShift           8
#define ClrBlueMask             0x000000FF
#define ClrBlueShift            0

/**********************************************/
#endif /* LCD_DEF_H_ */
