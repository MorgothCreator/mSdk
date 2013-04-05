/*
 * Psp_Display.h
 *
 *  Created on: Feb 2, 2011
 *      Author: Nimeni
 */

#include "api/lcd_def.h"

#ifndef PSP_DISPLAY_H_
#define PSP_DISPLAY_H_


//#include <Dx.h>

typedef  struct  bsp_lcdc_hdr {
	unsigned char 		dblbuf;
	unsigned long		 *active_buffer;
	unsigned long		 *background_buffer;
	unsigned long      DMABaseAddr1;                                       /* DMA Base Address 1                                       */
	unsigned long      DMABaseAddr2;                                       /* DMA Base Address 2                                       */
	unsigned char      DMABurstLen;                                        /* DMA Burst Length                                         */
    unsigned short      ResX;                                               /* Number of columns in pixels on the display               */
    unsigned short      ResY;                                               /* Number of rows in pixels on the display                  */
    unsigned char     En2DMode;                                           /* Enable 2D addressing mode                                */
    unsigned short      VirtualResX;                                        /* Virtual horizontal size in pixels on 2D addressing mode  */
    unsigned short      VirtualResY;                                        /* Virtual vertical size in pixels on 2D addressing mode    */
    unsigned short      FrameRate;                                          /* Display frame rate                                       */
    unsigned char      PixelSize;                                          /* Bits per pixel                                           */
    unsigned char      MemFormat;                                          /* LCD memory ordering format                               */
    unsigned long      LCDCClk;                                            /* LCD controller clock                                     */
    unsigned short      GuardTime;                                          /* Delay between LCD control signals and power control      */
    unsigned char      IFWidth;                                            /* Interface width                                          */
    unsigned char     EnDualScanMode;                                     /* Enable Dual Scan mode                                    */
    unsigned char      DispType;                                           /* LCD Display Type                                         */
    unsigned char     InvPolarityData;                                    /* Invert polarity of data signal                           */
    unsigned char     InvPolarityVSync;                                   /* Invert polarity of vsync signal                          */
    unsigned char     InvPolarityHSync;                                   /* Invert polarity of hsync signal                          */
    unsigned char     InvPolarityPixelClk;                                /* Invert polarity of pixel clock signal                    */
    unsigned char     InvPolarityDataValid;                               /* Invert polarity of valid data signal                     */
    unsigned char     EnPixelClkAlwaysActive;                             /* Pixel clock remains always active                        */
    unsigned char      ContrastVal;                                        /* Contrast PWM value                                       */
    unsigned char      ContrastPreScaler;                                  /* Contrast PWM prescaler                                   */
    unsigned char     InvPolarityContrast;                                /* Invert polarity of contrast PWM signal                   */
    unsigned char     EnContrast;                                         /* Enable contrast PWM                                      */
    unsigned char     EnModeToggleRate;                                   /* Enable selection of MODE toggle rate                     */
    unsigned char      ModeToggleRate;                                     /* MODE toggle rate value                                   */
    unsigned char      HPW;                                                /* Horizontal synchronization pulse width                   */
    unsigned char      HBP;                                                /* Horizontal back porch                                    */
    unsigned short      HFP;                                                /* Horizontal front porch                                   */
    unsigned char      VPW;                                                /* Vertical synchronization pulse width                     */
    unsigned char      VBP;                                                /* Vertical back porch                                      */
    unsigned char      VFP;                                                /* Vertical front porch                                     */
    unsigned char      VHDly;                                              /* Vertical to horizontal delay                             */
} BSP_LCDC_HDR;


//volatile  avr32_lcdc_t  *lcdc = &AVR32_LCDC;

#define LCD_BPP BPP_24

#define DisplayXResolution	(800)
#define DisplayYResolution	(480)

#define DISP_ON  (1<<31)				// pin for display on
#define DISP_OFF (1<<31)
#define BL_ON    (1<<28)				// pin for backlight
#define BL_OFF   (1<<28)
#define LCD_ON ((BL_ON) | (DISP_ON))
#define LCD_OFF ((BL_ON) | (DISP_ON))

// Bits per pixel ?
#define BPP_1 1
#define BPP_2 2
#define BPP_4 4
#define BPP_8 8
#define BPP_16 16
#define BPP_24 24
#define BPP_32 32

#define NORMAL 0
#define INVERTED 1

#define ENABLED 1
#define DISABLED 0

#define PRE_NONE 0
#define PRE_HALF 1
#define PRE_FOURTH 2
#define PRE_EIGTH 3

#define PARTLY_ACTIVE 0
#define ALWAYS_ACTIVE 1

#define EACH_FRAME 0
#define MVAL_DEFINED 1





#define CONSOLE_COLS		(800 / VIDEO_FONT_WIDTH)
#define CONSOLE_ROW_SIZE	(VIDEO_FONT_HEIGHT * lcd_line_length)
#define CONSOLE_ROW_FIRST	(lcd_console_address)
#define CONSOLE_ROW_SECOND	(lcd_console_address + CONSOLE_ROW_SIZE)
#define CONSOLE_ROW_LAST	(lcd_console_address + CONSOLE_SIZE - CONSOLE_ROW_SIZE)
#define CONSOLE_SIZE		(CONSOLE_ROW_SIZE * CONSOLE_ROWS)
#define CONSOLE_SCROLL_SIZE	(CONSOLE_SIZE - CONSOLE_ROW_SIZE)
# define CONSOLE_ROWS		(480 / VIDEO_FONT_HEIGHT)

#define LCD_MONOCHROME	0
#define LCD_COLOR2	1
#define LCD_COLOR4	2
#define LCD_COLOR8	3
#define LCD_COLOR16	4

#define NBITS(bit_code)		(1 << (bit_code))
#define NCOLORS(bit_code)	(1 << NBITS(bit_code))



typedef  enum {
    LCD_BIG_ENDIAN = 0,
    LCD_LITTLE_ENDIAN,
    LCD_WINCE
} LCD_MEMFORMAT;

typedef  enum {
    LCD_IFWIDTH_4 = 0,
    LCD_IFWIDTH_8,
    LCD_IFWIDTH_16
} LCD_IFWIDTH;

typedef  enum {
    LCD_STN_MONO = 0,
    LCD_STN_COLOR,
    LCD_TFT
} LCD_DISPTYPE;

#define     AVR32_INTC_ICR_reg            ((volatile unsigned long *)(AVR32_INTC_ADDRESS + 0x200))
#define     AVR32_PM_HSBMASK_reg         (*(volatile unsigned long *)(AVR32_PM_ADDRESS   + 0x00C))
#define     AVR32_PM_PBBMASK_reg         (*(volatile unsigned long *)(AVR32_PM_ADDRESS   + 0x014))

struct  bsp_lcdc_hdr lcd;
unsigned long lcd_line_length;
unsigned long fb_size;

void lcd_init(tDisplay* LcdStruct);

#include "lcd.c"

#endif /* PSP_DISPLAY_H_ */
