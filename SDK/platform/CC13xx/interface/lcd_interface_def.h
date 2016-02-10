/*
 * lcd_interface_def.h
 *
 *  Created on: Nov 14, 2015
 *      Author: John Smith
 */

#ifndef AM33_INTERFACE_LCD_INTERFACE_DEF_H_
#define AM33_INTERFACE_LCD_INTERFACE_DEF_H_


/** @defgroup LCD_Driver_structure  LCD Driver structure
  * @{
  */
typedef struct
{
  void     (*Init)(void);
  unsigned short (*ReadID)(void);
  void     (*DisplayOn)(void);
  void     (*DisplayOff)(void);
  void     (*SetCursor)(unsigned short, unsigned short);
  void     (*WritePixel)(unsigned short, unsigned short, unsigned short);
  unsigned short (*ReadPixel)(unsigned short, unsigned short);

   /* Optimized operation */
  void     (*SetDisplayWindow)(unsigned short, unsigned short, unsigned short, unsigned short);
  void     (*DrawHLine)(unsigned short, unsigned short, unsigned short, unsigned short);
  void     (*DrawVLine)(unsigned short, unsigned short, unsigned short, unsigned short);

  unsigned short (*GetLcdPixelWidth)(void);
  unsigned short (*GetLcdPixelHeight)(void);
  void     (*DrawBitmap)(unsigned short, unsigned short, unsigned char*);
  void     (*DrawRGBImage)(unsigned short, unsigned short, unsigned short, unsigned short, unsigned char*);
}LCD_DrvTypeDef;



#endif /* AM33_INTERFACE_LCD_INTERFACE_DEF_H_ */
