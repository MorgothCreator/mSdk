/**
  ******************************************************************************
  * @file    LibJPEG/LibJPEG_Encoding/Src/decode.c 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    09-October-2015
  * @brief   This file contain the decompress method.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "decode.h"
#include "lib/fat_fs/inc/ff.h"
#include "include/jpeglib.h"

/* Private typedef -----------------------------------------------------------*/
  /* This struct contains the JPEG decompression parameters */
  struct jpeg_decompress_struct cinfo;
  /* This struct represents a JPEG error handler */
  struct jpeg_error_mgr jerr;
  
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Jpeg Decode
  * @param  callback: line decoding callback
  * @param  file1:    pointer to the jpg file
  * @param  width:    image width
  * @param  buff:     pointer to the image line    
  * @retval None
  */
void jpeg_decode(FIL *file, unsigned long width, unsigned char * buff, unsigned char (*callback)(unsigned char*, unsigned long))
{ 
    
  /* Decode JPEG Image */
  JSAMPROW buffer[2] = {0}; /* Output row buffer */
  unsigned long row_stride = 0; /* physical row width in image buffer */
  
  buffer[0] = buff;

  /* Step 1: allocate and initialize JPEG decompression object */
  cinfo.err = jpeg_std_error(&jerr);

  /* Initialize the JPEG decompression object */  
  jpeg_create_decompress(&cinfo);
  
  jpeg_stdio_src (&cinfo, file);

  /* Step 3: read image parameters with jpeg_read_header() */
  jpeg_read_header(&cinfo, TRUE);

  /* TBC */
  /* Step 4: set parameters for decompression */
  cinfo.dct_method = JDCT_FLOAT;

  /* Step 5: start decompressor */
  jpeg_start_decompress(&cinfo);

  row_stride = width * 3;
  while (cinfo.output_scanline < cinfo.output_height)
  {
    (void) jpeg_read_scanlines(&cinfo, buffer, 1);
    
    /* TBC */
    if (callback(buffer[0], row_stride) != 0)
    {
      break;
    }
  }

  /* Step 6: Finish decompression */
  jpeg_finish_decompress(&cinfo);

  /* Step 7: Release JPEG decompression object */
  jpeg_destroy_decompress(&cinfo);
    
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
/**
  * @brief  Copy decompressed data to display buffer.
  * @param  Row: Output row buffer
  * @param  DataLength: Row width in output buffer
  * @retval None
  */
unsigned char Jpeg_CallbackFunction(unsigned char* Row, unsigned long DataLength)
{
  /*unsigned long i = 0;
  RGB_matrix =  (RGB_typedef*)Row;
  unsigned long  ARGB8888Buffer[IMAGE_WIDTH];

#ifdef SWAP_RB
  for(i = 0; i < IMAGE_WIDTH; i++)
  {
	  ARGB8888Buffer[i]  = (unsigned long)
      (
        0xFF000000                                       |
       (((unsigned long)(RGB_matrix[i].B) & 0x000000FF) >> 0) |
       (((unsigned long)(RGB_matrix[i].G) & 0x000000FF) << 8) |
       (((unsigned long)(RGB_matrix[i].R) & 0x000000FF) << 16)
      );

    BSP_LCD_DrawPixel((i + Xpos), (counter + Ypos), ARGB8888Buffer[i]);
  }
#else
  for(i = 0; i < IMAGE_WIDTH; i++)
  {
	  ARGB8888Buffer[i]  = (unsigned long)
      (
        0xFF000000                                       |
       (((unsigned long)(RGB_matrix[i].R) & 0x000000FF) >> 0) |
       (((unsigned long)(RGB_matrix[i].G) & 0x000000FF) << 8) |
       (((unsigned long)(RGB_matrix[i].B) & 0x000000FF) << 16)
      );

    BSP_LCD_DrawPixel((i + Xpos), (counter + Ypos), ARGB8888Buffer[i]);
  }
#endif

  counter++;*/
  return 0;
}
