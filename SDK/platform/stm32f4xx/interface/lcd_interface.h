/*
 * lcd_interface.h
 *
 * Created: 2/8/2013 10:26:56 PM
 *  Author: XxXx
 */ 
/*#####################################################*/
#ifndef LCD_INTERFACE_H_
#define LCD_INTERFACE_H_
/*#####################################################*/
#include <stdbool.h>
#include "api/lcd_def.h"
/*#####################################################*/
#ifdef SSD2119

/** @defgroup STM32F4_DISCOVERY_LCD_Exported_Types
  * @{
  */
typedef struct
{
  unsigned short X;
  unsigned short Y;
} Point, * pPoint;
/**
  * @}
  */

/*#####################################################*/
/**
  * @brief Various internal SD2119 registers name labels
  */
#define SSD2119_DEVICE_CODE_READ_REG  0x00
#define SSD2119_OSC_START_REG         0x00
#define SSD2119_OUTPUT_CTRL_REG       0x01
#define SSD2119_LCD_DRIVE_AC_CTRL_REG 0x02
#define SSD2119_PWR_CTRL_1_REG        0x03
#define SSD2119_DISPLAY_CTRL_REG      0x07
#define SSD2119_FRAME_CYCLE_CTRL_REG  0x0B
#define SSD2119_PWR_CTRL_2_REG        0x0C
#define SSD2119_PWR_CTRL_3_REG        0x0D
#define SSD2119_PWR_CTRL_4_REG        0x0E
#define SSD2119_GATE_SCAN_START_REG   0x0F
#define SSD2119_SLEEP_MODE_1_REG      0x10
#define SSD2119_ENTRY_MODE_REG        0x11
#define SSD2119_SLEEP_MODE_2_REG      0x12
#define SSD2119_GEN_IF_CTRL_REG       0x15
#define SSD2119_PWR_CTRL_5_REG        0x1E
#define SSD2119_RAM_DATA_REG          0x22
#define SSD2119_FRAME_FREQ_REG        0x25
#define SSD2119_ANALOG_SET_REG        0x26
#define SSD2119_VCOM_OTP_1_REG        0x28
#define SSD2119_VCOM_OTP_2_REG        0x29
#define SSD2119_GAMMA_CTRL_1_REG      0x30
#define SSD2119_GAMMA_CTRL_2_REG      0x31
#define SSD2119_GAMMA_CTRL_3_REG      0x32
#define SSD2119_GAMMA_CTRL_4_REG      0x33
#define SSD2119_GAMMA_CTRL_5_REG      0x34
#define SSD2119_GAMMA_CTRL_6_REG      0x35
#define SSD2119_GAMMA_CTRL_7_REG      0x36
#define SSD2119_GAMMA_CTRL_8_REG      0x37
#define SSD2119_GAMMA_CTRL_9_REG      0x3A
#define SSD2119_GAMMA_CTRL_10_REG     0x3B
#define SSD2119_V_RAM_POS_REG         0x44
#define SSD2119_H_RAM_START_REG       0x45
#define SSD2119_H_RAM_END_REG         0x46
#define SSD2119_X_RAM_ADDR_REG        0x4E
#define SSD2119_Y_RAM_ADDR_REG        0x4F

#define ENTRY_MODE_DEFAULT 0x6830
#define ENTRY_MODE_BMP 	   0x6810
#define MAKE_ENTRY_MODE(x) ((ENTRY_MODE_DEFAULT & 0xFF00) | (x))

/**
  * @brief The dimensions of the LCD panel.
  */
#define LCD_VERTICAL_MAX   240
#define LCD_HORIZONTAL_MAX 320

/**
  * @brief  LCD Size (Width and Height)
  */
#define LCD_PIXEL_WIDTH          320
#define LCD_PIXEL_HEIGHT         240
#endif








/**
  * @brief  LCD Control pin
  */
#define LCD_NCS_PIN                             GPIO_PIN_2
#define LCD_NCS_GPIO_PORT                       GPIOC
#define LCD_NCS_GPIO_CLK_ENABLE()               __GPIOC_CLK_ENABLE()
#define LCD_NCS_GPIO_CLK_DISABLE()              __GPIOC_CLK_DISABLE()
/**
  * @}
  */
/**
  * @brief  LCD Command/data pin
  */
#define LCD_WRX_PIN                             GPIO_PIN_13
#define LCD_WRX_GPIO_PORT                       GPIOD
#define LCD_WRX_GPIO_CLK_ENABLE()               __GPIOD_CLK_ENABLE()
#define LCD_WRX_GPIO_CLK_DISABLE()              __GPIOD_CLK_DISABLE()

#define LCD_RDX_PIN                             GPIO_PIN_12
#define LCD_RDX_GPIO_PORT                       GPIOD
#define LCD_RDX_GPIO_CLK_ENABLE()               __GPIOD_CLK_ENABLE()
#define LCD_RDX_GPIO_CLK_DISABLE()              __GPIOD_CLK_DISABLE()


/** @defgroup STM32F429I_DISCOVERY_LCD_Exported_Types
  * @{
  */
typedef enum
{
  LCD_OK = 0,
  LCD_ERROR = 1,
  LCD_TIMEOUT = 2
}LCD_StatusTypeDef;

typedef struct
{
  unsigned long  TextColor;
  unsigned long  BackColor;
  //sFONT     *pFont;
}LCD_DrawPropTypeDef;

typedef struct
{
  short X;
  short Y;
} Point, * pPoint;

/**
  * @brief  Line mode structures definition
  */
typedef enum
{
  CENTER_MODE             = 0x01,    /* center mode */
  RIGHT_MODE              = 0x02,    /* right mode  */
  LEFT_MODE               = 0x03,    /* left mode   */
}Text_AlignModeTypdef;
/**
  * @}
  */

/** @defgroup STM32F429I_DISCOVERY_LCD_Exported_Constants
  * @{
  */
#define LCD_LayerCfgTypeDef    LTDC_LayerCfgTypeDef

/**
  * @brief  LCD status structure definition
  */
#define MAX_LAYER_NUMBER       2
#define LCD_FRAME_BUFFER       ((unsigned long)0xD0000000)
#define BUFFER_OFFSET          ((unsigned long)0x50000)

/**
  * @brief  LCD color
  */
#define LCD_COLOR_BLUE          0xFF0000FF
#define LCD_COLOR_GREEN         0xFF00FF00
#define LCD_COLOR_RED           0xFFFF0000
#define LCD_COLOR_CYAN          0xFF00FFFF
#define LCD_COLOR_MAGENTA       0xFFFF00FF
#define LCD_COLOR_YELLOW        0xFFFFFF00
#define LCD_COLOR_LIGHTBLUE     0xFF8080FF
#define LCD_COLOR_LIGHTGREEN    0xFF80FF80
#define LCD_COLOR_LIGHTRED      0xFFFF8080
#define LCD_COLOR_LIGHTCYAN     0xFF80FFFF
#define LCD_COLOR_LIGHTMAGENTA  0xFFFF80FF
#define LCD_COLOR_LIGHTYELLOW   0xFFFFFF80
#define LCD_COLOR_DARKBLUE      0xFF000080
#define LCD_COLOR_DARKGREEN     0xFF008000
#define LCD_COLOR_DARKRED       0xFF800000
#define LCD_COLOR_DARKCYAN      0xFF008080
#define LCD_COLOR_DARKMAGENTA   0xFF800080
#define LCD_COLOR_DARKYELLOW    0xFF808000
#define LCD_COLOR_WHITE         0xFFFFFFFF
#define LCD_COLOR_LIGHTGRAY     0xFFD3D3D3
#define LCD_COLOR_GRAY          0xFF808080
#define LCD_COLOR_DARKGRAY      0xFF404040
#define LCD_COLOR_BLACK         0xFF000000
#define LCD_COLOR_BROWN         0xFFA52A2A
#define LCD_COLOR_ORANGE        0xFFFFA500
#define LCD_COLOR_TRANSPARENT   0xFF000000
/**
  * @brief LCD default font
  */
#define LCD_DEFAULT_FONT         Font24

/**
  * @brief  LCD Layer
  */
#define LCD_BACKGROUND_LAYER     0x0000
#define LCD_FOREGROUND_LAYER     0x0001

/**
  * @}
  */

/** @defgroup STM32F429I_DISCOVERY_LCD_Exported_Macros
  * @{
  */
/**
  * @brief LCD Pixel format
  */
#define LCD_PIXEL_FORMAT_ARGB8888         LTDC_PIXEL_FORMAT_ARGB8888
#define LCD_PIXEL_FORMAT_RGB888           LTDC_PIXEL_FORMAT_RGB888
#define LCD_PIXEL_FORMAT_RGB565           LTDC_PIXEL_FORMAT_RGB565
#define LCD_PIXEL_FORMAT_ARGB1555         LTDC_PIXEL_FORMAT_ARGB1555
#define LCD_PIXEL_FORMAT_ARGB4444         LTDC_PIXEL_FORMAT_ARGB4444
#define LCD_PIXEL_FORMAT_L8               LTDC_PIXEL_FORMAT_L8
#define LCD_PIXEL_FORMAT_AL44             LTDC_PIXEL_FORMAT_AL44
#define LCD_PIXEL_FORMAT_AL88             LTDC_PIXEL_FORMAT_AL88
/**
  * @}
  */

/** @defgroup STM32F429I_DISCOVERY_LCD_Exported_Functions
  * @{
  */
unsigned char  BSP_LCD_Init(void);









/*#####################################################*/
bool SetUpLCD(tDisplay* LcdStruct);
void _lcd_enable();
void _lcd_disable();
void _screen_backlight_on(tDisplay *pDisplay);
void _screen_backlight_off(tDisplay *pDisplay);
bool _screen_copy(tDisplay *pDisplayTo, tDisplay *pDisplayFrom, bool put_cursor, signed int X, signed int Y, unsigned int color);
void _box_cache_clean(tDisplay *pDisplay, signed int x_start, signed int y_start, signed int x_len, signed int y_len);
void _put_rectangle(tDisplay *pDisplay, signed int x_start, signed int y_start, signed int x_len, signed int y_len, bool fill, unsigned int color);
void _put_pixel(tDisplay *pDisplay, signed int X, signed int Y, unsigned int color);
void _screen_put_rgb_array_16(void *_pDisplay, unsigned short *rgb_buffer, unsigned int x1, unsigned int y1,unsigned int width, unsigned int height);
void _screen_put_rgb_array_24(void *_pDisplay, unsigned char *rgb_buffer, unsigned long x1, unsigned long y1,unsigned long width, unsigned long height);
void _screen_put_rgb_array_32(void *_pDisplay, unsigned char *rgb_buffer, unsigned int x1, unsigned int y1,unsigned int width, unsigned int height);
void _screen_put_horizontal_line(tDisplay *pDisplay, signed int X1, signed int X2, signed int Y, unsigned char width, unsigned int color);
void _screen_put_vertical_line(tDisplay *pDisplay, signed int Y1, signed int Y2, signed int X, unsigned char width, unsigned int color);
void _screen_clear(tDisplay *pDisplay, unsigned int color);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "lcd_interface.c"
#endif
/*#####################################################*/
#endif /* LCD_INTERFACE_H_ */
/*#####################################################*/
