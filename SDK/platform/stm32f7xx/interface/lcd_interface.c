/*
 * lcd_interface.c
 *
 * Created: 2/8/2013 10:26:43 PM
 *  Author: XxXx
 */

#include "main.h"
#include <stdbool.h>
#include "device/otm8009a.h"
//#include "board_properties.h"
#include "lcd_interface.h"
#include "sys/cache.h"
#include "sys/sysdelay.h"
#include "lib/gfx/controls_definition.h"
#include "api/gpio_api.h"

#include "include/stm32f7xx.h"
#include "driver/stm32f7xx_hal_dsi.h"
#include "driver/stm32f7xx_hal_dma2d.h"
#include "driver/stm32f7xx_hal_ltdc_ex.h"
#include "driver/stm32f7xx_hal_ltdc.h"
#include "driver/stm32f7xx_hal_gpio.h"
#include "driver/stm32f7xx_hal_rcc.h"
#include "driver/stm32f7xx_hal.h"
//#include "driver/stm32469i_eval_sdram.h"

LTDC_HandleTypeDef  LtdcHandler;
DMA2D_HandleTypeDef Dma2dHandle;
/**
  * @brief  LCD FB_StartAddress
  */
#define LCD_FB_START_ADDRESS       ((uint32_t)0xC0000000)

/**
  * @brief  Initializes the LCD layers.
  * @param  LayerIndex: Layer foreground or background
  * @param  FB_Address: Layer frame buffer
  * @retval None
  */
void BSP_LCD_LayerDefaultInit(void *_pDisplay, uint16_t LayerIndex, uint32_t FB_Address)
{
	tDisplay *pDisplay = (tDisplay *)_pDisplay;
    LCD_LayerCfgTypeDef  Layercfg;

  /* Layer Init */
  Layercfg.WindowX0 = 0;
  Layercfg.WindowX1 = pDisplay->LcdTimings->X;
  Layercfg.WindowY0 = 0;
  Layercfg.WindowY1 = pDisplay->LcdTimings->Y;
  Layercfg.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;
  Layercfg.FBStartAdress = FB_Address;
  Layercfg.Alpha = 255;
  Layercfg.Alpha0 = 0;
  Layercfg.Backcolor.Blue = 0;
  Layercfg.Backcolor.Green = 0;
  Layercfg.Backcolor.Red = 0;
  Layercfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
  Layercfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
  Layercfg.ImageWidth = pDisplay->LcdTimings->X;
  Layercfg.ImageHeight = pDisplay->LcdTimings->Y;
  pDisplay->DisplayData = (volatile unsigned int*)FB_Address;

  HAL_LTDC_ConfigLayer(&LtdcHandler, &Layercfg, LayerIndex);
}

/**
  * @brief  BSP LCD Reset
  *         Hw reset the LCD DSI activating its XRES signal (active low for some time)
  *         and desactivating it later.
  */
void BSP_LCD_Reset(void)
{
  GPIO_InitTypeDef  gpio_init_structure;

  __HAL_RCC_GPIOJ_CLK_ENABLE();

    /* Configure the GPIO on PJ15 */
    gpio_init_structure.Pin   = GPIO_PIN_15;
    gpio_init_structure.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio_init_structure.Pull  = GPIO_PULLUP;
    gpio_init_structure.Speed = GPIO_SPEED_HIGH;

    HAL_GPIO_Init(GPIOJ, &gpio_init_structure);

    /* Activate XRES active low */
    HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_15, GPIO_PIN_RESET);

    HAL_Delay(20); /* wait 20 ms */

    /* Desactivate XRES */
    HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_15, GPIO_PIN_SET);

    /* Wait for 10ms after releasing XRES before sending commands */
    HAL_Delay(10);
}

/**
  * @brief  DMA2D Transfer completed callback
  * @param  hdma2d: DMA2D handle.
  * @note   This example shows a simple way to report end of DMA2D transfer, and
  *         you can add your own implementation.
  * @retval None
  */
void TransferComplete(DMA2D_HandleTypeDef *hdma2d)
{
  /* Turn LED1 On */
  //BSP_LED_On(LED1);
  /* The Blended image is now ready for display */
  //blended_image_ready = 1;
}
/**
  * @brief  DMA2D error callbacks
  * @param  hdma2d: DMA2D handle
  * @note   This example shows a simple way to report DMA2D transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
static void TransferError(DMA2D_HandleTypeDef *hdma2d)
{
  /* Turn LED2 On */
  //BSP_LED_On(LED2);
}
/**
  * @brief  On Error Handler on condition TRUE.
  * @param  condition : Can be TRUE or FALSE
  * @retval None
  */
void OnError_Handler(uint32_t condition)
{
  if(condition)
  {
    //BSP_LED_On(LED3);
    while(1) { ; } /* Blocking on error */
  }
}
/**
  * @brief DMA2D configuration.
  * @note  This function Configure the DMA2D peripheral :
  *        1) Configure the Transfer mode as memory to memory with blending.
  *        2) Configure the output color mode as RGB565 pixel format.
  *        3) Configure the Foreground
  *          - Foreground image is loaded from FLASH memory (RGB565_240x130_2[])
  *          - constant alpha value (decreased to see the background)
  *          - color mode as RGB565 pixel format
  *        4) Configure the Background
  *          - Background image loaded from FLASH memory (RGB565_240x130_1[])
  *          - color mode as RGB565 pixel format
  * @retval
  *  None
  */
static void DMA2D_Config(void)
{
  HAL_StatusTypeDef hal_status = HAL_OK;

  /* Configure the DMA2D Mode, Color Mode and output offset */
  Dma2dHandle.Init.Mode         = DMA2D_M2M_BLEND; /* DMA2D mode Memory to Memory with Blending */
  Dma2dHandle.Init.ColorMode    = DMA2D_RGB565; /* output format of DMA2D */
  Dma2dHandle.Init.OutputOffset = 0x0;  /* No offset in output */

  /* DMA2D Callbacks Configuration */
  Dma2dHandle.XferCpltCallback  = TransferComplete;
  Dma2dHandle.XferErrorCallback = TransferError;

  /* Foreground layer Configuration */
  Dma2dHandle.LayerCfg[1].AlphaMode = DMA2D_REPLACE_ALPHA;
  Dma2dHandle.LayerCfg[1].InputAlpha = 0x7F; /* 127 : semi-transparent */
  Dma2dHandle.LayerCfg[1].InputColorMode = CM_RGB565;
  Dma2dHandle.LayerCfg[1].InputOffset = 0x0; /* No offset in input */

  /* Background layer Configuration */
  Dma2dHandle.LayerCfg[0].AlphaMode = DMA2D_REPLACE_ALPHA;
  Dma2dHandle.LayerCfg[0].InputAlpha = 0x7F; /* 127 : semi-transparent */
  Dma2dHandle.LayerCfg[0].InputColorMode = CM_RGB565;
  Dma2dHandle.LayerCfg[0].InputOffset = 0x0; /* No offset in input */

  Dma2dHandle.Instance = DMA2D;

  /* DMA2D Initialization */
  hal_status = HAL_DMA2D_Init(&Dma2dHandle);
  OnError_Handler(hal_status != HAL_OK);

  /* Apply DMA2D Foreground configuration */
  HAL_DMA2D_ConfigLayer(&Dma2dHandle, 1);

  /* Apply DMA2D Background configuration */
  HAL_DMA2D_ConfigLayer(&Dma2dHandle, 0);
}

#if defined(STM32F429xx) || defined(STM32F439xx)|| defined(STM32F469xx) || defined(STM32F479xx)
/** @brief Maximum number of LTDC layers
 */
#define LTDC_MAX_LAYER_NUMBER             ((uint32_t) 2)
/** @brief LTDC Background layer index
 */
#define LTDC_ACTIVE_LAYER_BACKGROUND      ((uint32_t) 0)

/** @brief LTDC Foreground layer index
 */
#define LTDC_ACTIVE_LAYER_FOREGROUND      ((uint32_t) 1)


static void DMA2D_Config(void);

/**
  * @brief  This function handles LTDC Error interrupt Handler.
  * @note : Can be surcharged by application code implementation of the function.
  */

/*__weak void BSP_LCD_LTDC_ER_IRQHandler(void)
{
  HAL_LTDC_IRQHandler(&(LtdcHandler));
}*/


/**
  * @brief  Handles DMA2D interrupt request.
  * @note : Can be surcharged by application code implementation of the function.
  */
/*__weak void BSP_LCD_DMA2D_IRQHandler(void)
{
  HAL_DMA2D_IRQHandler(&Dma2dHandle);
}*/

/**
  * @brief  Handles LTDC interrupt request.
  * @note : Can be surcharged by application code implementation of the function.
  */
/*__weak void BSP_LCD_LTDC_IRQHandler(void)
{
  HAL_LTDC_IRQHandler(&(LtdcHandler));
}*/


/**
  * @brief  Converts a line to an ARGB8888 pixel format.
  * @param  pSrc: Pointer to source buffer
  * @param  pDst: Output color
  * @param  xSize: Buffer width
  * @param  ColorMode: Input color mode
  */
static void LL_ConvertLineToARGB8888(void *pSrc, void *pDst, uint32_t xSize, uint32_t ColorMode)
{
  /* Configure the DMA2D Mode, Color Mode and output offset */
	Dma2dHandle.Init.Mode         = DMA2D_M2M_PFC;
	Dma2dHandle.Init.ColorMode    = DMA2D_ARGB8888;
	Dma2dHandle.Init.OutputOffset = 0;

  /* Foreground Configuration */
	Dma2dHandle.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
	Dma2dHandle.LayerCfg[1].InputAlpha = 0xFF;
	Dma2dHandle.LayerCfg[1].InputColorMode = ColorMode;
	Dma2dHandle.LayerCfg[1].InputOffset = 0;

	Dma2dHandle.Instance = DMA2D;

  /* DMA2D Initialization */
  if(HAL_DMA2D_Init(&Dma2dHandle) == HAL_OK)
  {
    if(HAL_DMA2D_ConfigLayer(&Dma2dHandle, 1) == HAL_OK)
    {
      if (HAL_DMA2D_Start(&Dma2dHandle, (uint32_t)pSrc, (uint32_t)pDst, xSize, 1) == HAL_OK)
      {
        /* Polling For DMA transfer */
        HAL_DMA2D_PollForTransfer(&Dma2dHandle, 10);
      }
    }
  }
}

/**
  * @brief  Draws a bitmap picture loaded in the internal Flash (32 bpp) in currently active layer.
  * @param  Xpos: Bmp X position in the LCD
  * @param  Ypos: Bmp Y position in the LCD
  * @param  pbmp: Pointer to Bmp picture address in the internal Flash
  */
void BSP_LCD_DrawBitmap(void *_pDisplay, uint32_t Xpos, uint32_t Ypos, uint8_t *pbmp)
{
	tDisplay *pDisplay = (tDisplay *)_pDisplay;
  uint32_t index = 0, width = 0, height = 0, bit_pixel = 0;
  uint32_t Address;
  uint32_t InputColorMode = 0;

  /* Get bitmap data address offset */
  index = *(volatile uint16_t *) (pbmp + 10);
  index |= (*(volatile uint16_t *) (pbmp + 12)) << 16;

  /* Read bitmap width */
  width = *(uint16_t *) (pbmp + 18);
  width |= (*(uint16_t *) (pbmp + 20)) << 16;

  /* Read bitmap height */
  height = *(uint16_t *) (pbmp + 22);
  height |= (*(uint16_t *) (pbmp + 24)) << 16;

  /* Read bit/pixel */
  bit_pixel = *(uint16_t *) (pbmp + 28);

  /* Set the address */
  Address = LtdcHandler.LayerCfg[0].FBStartAdress + (((pDisplay->LcdTimings->X*Ypos) + Xpos)*(4));

  /* Get the layer pixel format */
  if ((bit_pixel/8) == 4)
  {
    InputColorMode = CM_ARGB8888;
  }
  else if ((bit_pixel/8) == 2)
  {
    InputColorMode = CM_RGB565;
  }
  else
  {
    InputColorMode = CM_RGB888;
  }

  /* Bypass the bitmap header */
  pbmp += (index + (width * (height - 1) * (bit_pixel/8)));

  /* Convert picture to ARGB8888 pixel format */
  for(index=0; index < height; index++)
  {
    /* Pixel format conversion */
    LL_ConvertLineToARGB8888((uint32_t *)pbmp, (uint32_t *)Address, width, InputColorMode);

    /* Increment the source and destination buffers */
    Address+=  (pDisplay->LcdTimings->X*4);
    pbmp -= width*(bit_pixel/8);
  }
}


/**
  * @brief  De-Initializes the BSP LCD Msp
  * Application can surcharge if needed this function implementation.
  */
__weak void BSP_LCD_MspDeInit(void)
{
  /** @brief Disable IRQ of LTDC IP */
  HAL_NVIC_DisableIRQ(LTDC_IRQn);

  /** @brief Disable IRQ of DMA2D IP */
  HAL_NVIC_DisableIRQ(DMA2D_IRQn);

  /** @brief Disable IRQ of DSI IP */
  HAL_NVIC_DisableIRQ(DSI_IRQn);

  /** @brief Force and let in reset state LTDC, DMA2D and DSI Host + Wrapper IPs */
  __HAL_RCC_LTDC_FORCE_RESET();
  __HAL_RCC_DMA2D_FORCE_RESET();
  __HAL_RCC_DSI_FORCE_RESET();

  /** @brief Disable the LTDC, DMA2D and DSI Host and Wrapper clocks */
  __HAL_RCC_LTDC_CLK_DISABLE();
  __HAL_RCC_DMA2D_CLK_DISABLE();
  __HAL_RCC_DSI_CLK_DISABLE();
}

/**
  * @brief  Initialize the BSP LCD Msp.
  * Application can surcharge if needed this function implementation
  */
__weak void BSP_LCD_MspInit(void)
{
  /** @brief Enable the LTDC clock */
  __HAL_RCC_LTDC_CLK_ENABLE();

  /** @brief Toggle Sw reset of LTDC IP */
  __HAL_RCC_LTDC_FORCE_RESET();
  __HAL_RCC_LTDC_RELEASE_RESET();

  /** @brief Enable the DMA2D clock */
  __HAL_RCC_DMA2D_CLK_ENABLE();

  /** @brief Toggle Sw reset of DMA2D IP */
  __HAL_RCC_DMA2D_FORCE_RESET();
  __HAL_RCC_DMA2D_RELEASE_RESET();

  /** @brief Enable DSI Host and wrapper clocks */
  __HAL_RCC_DSI_CLK_ENABLE();

  /** @brief Soft Reset the DSI Host and wrapper */
  __HAL_RCC_DSI_FORCE_RESET();
  __HAL_RCC_DSI_RELEASE_RESET();

  /** @brief NVIC configuration for LTDC interrupt that is now enabled */
  HAL_NVIC_SetPriority(LTDC_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(LTDC_IRQn);

  /** @brief NVIC configuration for DMA2D interrupt that is now enabled */
  HAL_NVIC_SetPriority(DMA2D_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(DMA2D_IRQn);

  /** @brief NVIC configuration for DSI interrupt that is now enabled */
  HAL_NVIC_SetPriority(DSI_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(DSI_IRQn);
}




/**
  * @brief  Initializes the LCD.
  * @param  None
  * @retval LCD state
  */
uint8_t BSP_LCD_Init(void *_pDisplay)
{
	tDisplay *pDisplay = (tDisplay *)_pDisplay;
  /* On STM32F429I-DISCO, it is not possible to read ILI9341 ID because */
  /* PIN EXTC is not connected to VDD and then LCD_READ_ID4 is not accessible. */
  /* In this case, ReadID function is bypassed.*/
  /*if(ili9341_drv.ReadID() == ILI9341_ID)*/

    /* LTDC Configuration ----------------------------------------------------*/
    LtdcHandler.Instance = LTDC;

    /* Timing configuration  (Typical configuration from ILI9341 datasheet)
          HSYNC=10 (9+1)
          HBP=20 (29-10+1)
          ActiveW=240 (269-20-10+1)
          HFP=10 (279-240-20-10+1)

          VSYNC=2 (1+1)
          VBP=2 (3-2+1)
          ActiveH=320 (323-2-2+1)
          VFP=4 (327-320-2-2+1)
      */

    /* Configure horizontal synchronization width */
    LtdcHandler.Init.HorizontalSync = pDisplay->LcdTimings->hsw;
    /* Configure vertical synchronization height */
    LtdcHandler.Init.VerticalSync = pDisplay->LcdTimings->vsw;
    /* Configure accumulated horizontal back porch */
    LtdcHandler.Init.AccumulatedHBP = pDisplay->LcdTimings->hbp;
    /* Configure accumulated vertical back porch */
    LtdcHandler.Init.AccumulatedVBP = pDisplay->LcdTimings->vbp;
    /* Configure accumulated active width */
    LtdcHandler.Init.AccumulatedActiveW = pDisplay->LcdTimings->X + pDisplay->LcdTimings->hbp;
    /* Configure accumulated active height */
    LtdcHandler.Init.AccumulatedActiveH = pDisplay->LcdTimings->Y + pDisplay->LcdTimings->vbp;
    /* Configure total width */
    LtdcHandler.Init.TotalWidth = pDisplay->LcdTimings->X + pDisplay->LcdTimings->hbp + pDisplay->LcdTimings->hfp;
    /* Configure total height */
    LtdcHandler.Init.TotalHeigh = pDisplay->LcdTimings->Y + pDisplay->LcdTimings->vbp + pDisplay->LcdTimings->vfp;

    /* Configure R,G,B component values for LCD background color */
    LtdcHandler.Init.Backcolor.Red= 0;
    LtdcHandler.Init.Backcolor.Blue= 0;
    LtdcHandler.Init.Backcolor.Green= 0;


    /* Polarity */
    LtdcHandler.Init.HSPolarity = LTDC_HSPOLARITY_AL;
    LtdcHandler.Init.VSPolarity = LTDC_VSPOLARITY_AL;
    LtdcHandler.Init.DEPolarity = LTDC_DEPOLARITY_AL;
    LtdcHandler.Init.PCPolarity = LTDC_PCPOLARITY_IPC;

    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable the LTDC and DMA2D Clock */
    __LTDC_CLK_ENABLE();
    __DMA2D_CLK_ENABLE();
    /* Enable GPIOs clock */
    __GPIOA_CLK_ENABLE();
    __GPIOB_CLK_ENABLE();
    __GPIOC_CLK_ENABLE();
    __GPIOD_CLK_ENABLE();
    __GPIOF_CLK_ENABLE();
    __GPIOG_CLK_ENABLE();

    /* GPIOs Configuration */
    /*
     +------------------------+-----------------------+----------------------------+
     +                       LCD pins assignment                                   +
     +------------------------+-----------------------+----------------------------+
     |  LCD_TFT R2 <-> PC.10  |  LCD_TFT G2 <-> PA.06 |  LCD_TFT B2 <-> PD.06      |
     |  LCD_TFT R3 <-> PB.00  |  LCD_TFT G3 <-> PG.10 |  LCD_TFT B3 <-> PG.11      |
     |  LCD_TFT R4 <-> PA.11  |  LCD_TFT G4 <-> PB.10 |  LCD_TFT B4 <-> PG.12      |
     |  LCD_TFT R5 <-> PA.12  |  LCD_TFT G5 <-> PB.11 |  LCD_TFT B5 <-> PA.03      |
     |  LCD_TFT R6 <-> PB.01  |  LCD_TFT G6 <-> PC.07 |  LCD_TFT B6 <-> PB.08      |
     |  LCD_TFT R7 <-> PG.06  |  LCD_TFT G7 <-> PD.03 |  LCD_TFT B7 <-> PB.09      |
     -------------------------------------------------------------------------------
              |  LCD_TFT HSYNC <-> PC.06  | LCDTFT VSYNC <->  PA.04 |
              |  LCD_TFT CLK   <-> PG.07  | LCD_TFT DE   <->  PF.10 |
               -----------------------------------------------------
    */

    /* GPIOA configuration */
    GPIO_InitStructure.Pin = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_6 |
                             GPIO_PIN_11 | GPIO_PIN_12;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
    GPIO_InitStructure.Alternate= GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

   /* GPIOB configuration */
    GPIO_InitStructure.Pin = GPIO_PIN_8 | \
                             GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

   /* GPIOC configuration */
    GPIO_InitStructure.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_10;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

   /* GPIOD configuration */
    GPIO_InitStructure.Pin = GPIO_PIN_3 | GPIO_PIN_6;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);

   /* GPIOF configuration */
    GPIO_InitStructure.Pin = GPIO_PIN_10;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStructure);

   /* GPIOG configuration */
    GPIO_InitStructure.Pin = GPIO_PIN_6 | GPIO_PIN_7 | \
                             GPIO_PIN_11;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);

    /* GPIOB configuration */
    GPIO_InitStructure.Pin = GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_InitStructure.Alternate= GPIO_AF9_LTDC;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* GPIOG configuration */
    GPIO_InitStructure.Pin = GPIO_PIN_10 | GPIO_PIN_12;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);

    /* Initialize the font */
    DMA2D_Config();
    HAL_LTDC_Init(&LtdcHandler);
    /* Initialize the font */
    BSP_LCD_LayerDefaultInit(_pDisplay, 0, LCD_FB_START_ADDRESS);

    pDisplay->sClipRegion.sXMax = pDisplay->LcdTimings->X;
    pDisplay->sClipRegion.sYMax = pDisplay->LcdTimings->Y;
  return LCD_OK;
}



#endif

#if defined(USE_LCD_TYPE_DSI) && defined(STM32F769xx) || defined(STM32F779xx)

static DSI_VidCfgTypeDef hdsivideo_handle;
DSI_HandleTypeDef hdsi_eval;


void DSI_IO_WriteCmd(uint32_t NbrParams, uint8_t *pParams)
{
  if(NbrParams <= 1)
  {
   HAL_DSI_ShortWrite(&hdsi_eval, LCD_OTM8009A_ID, DSI_DCS_SHORT_PKT_WRITE_P1, pParams[0], pParams[1]);
  }
  else
  {
   HAL_DSI_LongWrite(&hdsi_eval,  LCD_OTM8009A_ID, DSI_DCS_LONG_PKT_WRITE, NbrParams, pParams[NbrParams], pParams);
  }
}

/**
  * @brief  De-Initializes the BSP LCD Msp
  * Application can surcharge if needed this function implementation.
  */
__weak void BSP_LCD_MspDeInit(void)
{
  /** @brief Disable IRQ of LTDC IP */
  HAL_NVIC_DisableIRQ(LTDC_IRQn);

  /** @brief Disable IRQ of DMA2D IP */
  HAL_NVIC_DisableIRQ(DMA2D_IRQn);

  /** @brief Disable IRQ of DSI IP */
  HAL_NVIC_DisableIRQ(DSI_IRQn);

  /** @brief Force and let in reset state LTDC, DMA2D and DSI Host + Wrapper IPs */
  __HAL_RCC_LTDC_FORCE_RESET();
  __HAL_RCC_DMA2D_FORCE_RESET();
  __HAL_RCC_DSI_FORCE_RESET();

  /** @brief Disable the LTDC, DMA2D and DSI Host and Wrapper clocks */
  __HAL_RCC_LTDC_CLK_DISABLE();
  __HAL_RCC_DMA2D_CLK_DISABLE();
  __HAL_RCC_DSI_CLK_DISABLE();
}

/**
  * @brief  Initialize the BSP LCD Msp.
  * Application can surcharge if needed this function implementation
  */
__weak void BSP_LCD_MspInit(void)
{
	  /** @brief Enable the LTDC clock */
	  __HAL_RCC_LTDC_CLK_ENABLE();

	  /** @brief Toggle Sw reset of LTDC IP */
	  __HAL_RCC_LTDC_FORCE_RESET();
	  __HAL_RCC_LTDC_RELEASE_RESET();

	  /** @brief Enable the DMA2D clock */
	  __HAL_RCC_DMA2D_CLK_ENABLE();

	  /** @brief Toggle Sw reset of DMA2D IP */
	  __HAL_RCC_DMA2D_FORCE_RESET();
	  __HAL_RCC_DMA2D_RELEASE_RESET();

	  /** @brief Enable DSI Host and wrapper clocks */
	  __HAL_RCC_DSI_CLK_ENABLE();

	  /** @brief Soft Reset the DSI Host and wrapper */
	  __HAL_RCC_DSI_FORCE_RESET();
	  __HAL_RCC_DSI_RELEASE_RESET();

	  /** @brief NVIC configuration for LTDC interrupt that is now enabled */
	  HAL_NVIC_SetPriority(LTDC_IRQn, 3, 0);
	  HAL_NVIC_EnableIRQ(LTDC_IRQn);

	  /** @brief NVIC configuration for DMA2D interrupt that is now enabled */
	  HAL_NVIC_SetPriority(DMA2D_IRQn, 3, 0);
	  HAL_NVIC_EnableIRQ(DMA2D_IRQn);

	  /** @brief NVIC configuration for DSI interrupt that is now enabled */
	  HAL_NVIC_SetPriority(DSI_IRQn, 3, 0);
	  HAL_NVIC_EnableIRQ(DSI_IRQn);
}
/*******************************************************************************
                       LTDC, DMA2D and DSI BSP Routines
*******************************************************************************/

/**
  * @brief  Handles DSI interrupt request.
  * @note : Can be surcharged by application code implementation of the function.
  */
/*__weak void BSP_LCD_DSI_IRQHandler(void)
{
  HAL_DSI_IRQHandler(&(hdsi_eval));
}*/



#if defined(STM32F769xx) || defined(STM32F779xx)
#define LCD_DSI_ID              0x11
/**
  * @brief  Returns the ID of connected screen by checking the HDMI
  *        (adv7533 component) ID or LCD DSI (via TS ID) ID.
  * @param  None
  * @retval LCD ID
  */
static uint16_t LCD_IO_GetID(void)
{
#if defined(USE_LCD_HDMI)
  HDMI_IO_Init();

  HDMI_IO_Delay(60);

  if(ADV7533_ID == adv7533_drv.ReadID(ADV7533_CEC_DSI_I2C_ADDR))
  {
    return ADV7533_ID;
  }
  else if(HDMI_IO_Read(LCD_DSI_ADDRESS, LCD_DSI_ID_REG) == LCD_DSI_ID)
  {
    return LCD_DSI_ID;
  }
  else
  {
    return 0;
  }
#else
  return LCD_DSI_ID;
#endif /* USE_LCD_HDMI */
}

/**
  * @brief  Initializes the DSI LCD.
  * The ititialization is done as below:
  *     - DSI PLL ititialization
  *     - DSI ititialization
  *     - LTDC ititialization
  *     - OTM8009A LCD Display IC Driver ititialization
  * @param  None
  * @retval LCD state
  */
uint8_t BSP_LCD_InitEx(void *_pDisplay)
{
	tDisplay *pDisplay = (tDisplay *)_pDisplay;
  DSI_PLLInitTypeDef dsiPllInit;
  uint32_t LcdClock  = 27429; /*!< LcdClk = 27429 kHz */
  uint32_t Clockratio  = 0;
  uint16_t read_id = 0;

  uint32_t laneByteClk_kHz = 0;
  uint32_t                   VSA; /*!< Vertical start active time in units of lines */
  uint32_t                   VBP; /*!< Vertical Back Porch time in units of lines */
  uint32_t                   VFP; /*!< Vertical Front Porch time in units of lines */
  uint32_t                   VACT; /*!< Vertical Active time in units of lines = imageSize Y in pixels to display */
  uint32_t                   HSA; /*!< Horizontal start active time in units of lcdClk */
  uint32_t                   HBP; /*!< Horizontal Back Porch time in units of lcdClk */
  uint32_t                   HFP; /*!< Horizontal Front Porch time in units of lcdClk */
  uint32_t                   HACT; /*!< Horizontal Active time in units of lcdClk = imageSize X in pixels to display */


  /* Toggle Hardware Reset of the DSI LCD using
  * its XRES signal (active low) */
  BSP_LCD_Reset();

  /* Check the connected monitor */
  read_id = LCD_IO_GetID();

#if defined(USE_LCD_HDMI)
  if(read_id == ADV7533_ID)
  {
    return BSP_LCD_HDMIInitEx(HDMI_FORMAT_720_576);
  }
  else if(read_id != LCD_DSI_ID)
  {
    return LCD_ERROR;
  }
#else
  if(read_id != LCD_DSI_ID)
  {
    return LCD_ERROR;
  }
#endif /* USE_LCD_HDMI */

  /* Call first MSP Initialize only in case of first initialization
  * This will set IP blocks LTDC, DSI and DMA2D
  * - out of reset
  * - clocked
  * - NVIC IRQ related to IP blocks enabled
  */
  BSP_LCD_MspInit();

/*************************DSI Initialization***********************************/

  /* Base address of DSI Host/Wrapper registers to be set before calling De-Init */
  hdsi_eval.Instance = DSI;

  HAL_DSI_DeInit(&(hdsi_eval));

  dsiPllInit.PLLNDIV  = 100;
  dsiPllInit.PLLIDF   = DSI_PLL_IN_DIV5;
  dsiPllInit.PLLODF   = DSI_PLL_OUT_DIV1;
  laneByteClk_kHz = 62500; /* 500 MHz / 8 = 62.5 MHz = 62500 kHz */

  /* Set number of Lanes */
  hdsi_eval.Init.NumberOfLanes = DSI_TWO_DATA_LANES;

  /* TXEscapeCkdiv = f(LaneByteClk)/15.62 = 4 */
  hdsi_eval.Init.TXEscapeCkdiv = laneByteClk_kHz/15620;

  HAL_DSI_Init(&(hdsi_eval), &(dsiPllInit));
  Clockratio = laneByteClk_kHz/LcdClock;
  /* Timing parameters for all Video modes
  * Set Timing parameters of LTDC depending on its chosen orientation
  */
  VSA  = pDisplay->LcdTimings->vsw;        /* 12 */
  VBP  = pDisplay->LcdTimings->vbp;          /* 12 */
  VFP  = pDisplay->LcdTimings->vfp;          /* 12 */
  HSA  = pDisplay->LcdTimings->hsw;        /* 120 */
  HBP  = pDisplay->LcdTimings->hbp;          /* 120 */
  HFP  = pDisplay->LcdTimings->hfp;          /* 120 */
  HACT = pDisplay->LcdTimings->X;
  VACT = pDisplay->LcdTimings->Y;


  hdsivideo_handle.VirtualChannelID = LCD_OTM8009A_ID;
  hdsivideo_handle.ColorCoding = LCD_DSI_PIXEL_DATA_FMT_RBG888;
  hdsivideo_handle.VSPolarity = DSI_VSYNC_ACTIVE_HIGH;
  hdsivideo_handle.HSPolarity = DSI_HSYNC_ACTIVE_HIGH;
  hdsivideo_handle.DEPolarity = DSI_DATA_ENABLE_ACTIVE_HIGH;
  hdsivideo_handle.Mode = DSI_VID_MODE_BURST; /* Mode Video burst ie : one LgP per line */
  hdsivideo_handle.NullPacketSize = 0xFFF;
  hdsivideo_handle.NumberOfChunks = 0;
  hdsivideo_handle.PacketSize                = HACT; /* Value depending on display orientation choice portrait/landscape */
  hdsivideo_handle.HorizontalSyncActive      = HSA*Clockratio;
  hdsivideo_handle.HorizontalBackPorch       = HBP*Clockratio;
  hdsivideo_handle.HorizontalLine            = (HACT + HSA + HBP + HFP)*Clockratio; /* Value depending on display orientation choice portrait/landscape */
  hdsivideo_handle.VerticalSyncActive        = VSA;
  hdsivideo_handle.VerticalBackPorch         = VBP;
  hdsivideo_handle.VerticalFrontPorch        = VFP;
  hdsivideo_handle.VerticalActive            = VACT; /* Value depending on display orientation choice portrait/landscape */

  /* Enable or disable sending LP command while streaming is active in video mode */
  hdsivideo_handle.LPCommandEnable = DSI_LP_COMMAND_ENABLE; /* Enable sending commands in mode LP (Low Power) */

  /* Largest packet size possible to transmit in LP mode in VSA, VBP, VFP regions */
  /* Only useful when sending LP packets is allowed while streaming is active in video mode */
  hdsivideo_handle.LPLargestPacketSize = 64;

  /* Largest packet size possible to transmit in LP mode in HFP region during VACT period */
  /* Only useful when sending LP packets is allowed while streaming is active in video mode */
  hdsivideo_handle.LPVACTLargestPacketSize = 0;


  /* Specify for each region of the video frame, if the transmission of command in LP mode is allowed in this region */
  /* while streaming is active in video mode                                                                         */
  hdsivideo_handle.LPHorizontalFrontPorchEnable = DSI_LP_HFP_ENABLE;   /* Allow sending LP commands during HFP period */
  hdsivideo_handle.LPHorizontalBackPorchEnable  = DSI_LP_HBP_ENABLE;   /* Allow sending LP commands during HBP period */
  hdsivideo_handle.LPVerticalActiveEnable = DSI_LP_VACT_ENABLE;  /* Allow sending LP commands during VACT period */
  hdsivideo_handle.LPVerticalFrontPorchEnable = DSI_LP_VFP_ENABLE;   /* Allow sending LP commands during VFP period */
  hdsivideo_handle.LPVerticalBackPorchEnable = DSI_LP_VBP_ENABLE;   /* Allow sending LP commands during VBP period */
  hdsivideo_handle.LPVerticalSyncActiveEnable = DSI_LP_VSYNC_ENABLE; /* Allow sending LP commands during VSync = VSA period */

  /* Configure DSI Video mode timings with settings set above */
  HAL_DSI_ConfigVideoMode(&(hdsi_eval), &(hdsivideo_handle));

  /* Enable the DSI host and wrapper : but LTDC is not started yet at this stage */
  HAL_DSI_Start(&(hdsi_eval));
/*************************End DSI Initialization*******************************/


/************************LTDC Initialization***********************************/

  /* Timing Configuration */
  LtdcHandler.Init.HorizontalSync = (HSA - 1);
  LtdcHandler.Init.AccumulatedHBP = (HSA + HBP - 1);
  LtdcHandler.Init.AccumulatedActiveW = (pDisplay->LcdTimings->X + HSA + HBP - 1);
  LtdcHandler.Init.TotalWidth = (pDisplay->LcdTimings->X + HSA + HBP + HFP - 1);

  /* Initialize the LCD pixel width and pixel height */
  LtdcHandler.LayerCfg->ImageWidth  = pDisplay->LcdTimings->X;
  LtdcHandler.LayerCfg->ImageHeight = pDisplay->LcdTimings->Y;



  /* Background value */
  LtdcHandler.Init.Backcolor.Blue = 0;
  LtdcHandler.Init.Backcolor.Green = 0;
  LtdcHandler.Init.Backcolor.Red = 0;
  LtdcHandler.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  LtdcHandler.Instance = LTDC;

  /* Get LTDC Configuration from DSI Configuration */
  HAL_LTDC_StructInitFromVideoConfig(&(LtdcHandler), &(hdsivideo_handle));

  /* Initialize the LTDC */
  HAL_LTDC_Init(&LtdcHandler);

  /* Initialize the font */
  DMA2D_Config();
  BSP_LCD_LayerDefaultInit(_pDisplay, 0, LCD_FB_START_ADDRESS);
  //BSP_LCD_SetFont(&LCD_DEFAULT_FONT);

/************************End LTDC Initialization*******************************/


/***********************OTM8009A Initialization********************************/

  /* Initialize the OTM8009A LCD Display IC Driver (KoD LCD IC Driver)
  *  depending on configuration set in 'hdsivideo_handle'.
  */
  OTM8009A_Init(_pDisplay, hdsivideo_handle.ColorCoding);

/***********************End OTM8009A Initialization****************************/
  pDisplay->sClipRegion.sXMax = pDisplay->LcdTimings->X;
  pDisplay->sClipRegion.sYMax = pDisplay->LcdTimings->Y;
  return LCD_OK;
}
#endif
/**
  * @brief  Fills a buffer.
  * @param  LayerIndex: Layer index
  * @param  pDst: Pointer to destination buffer
  * @param  xSize: Buffer width
  * @param  ySize: Buffer height
  * @param  OffLine: Offset
  * @param  ColorIndex: Color index
  */
static void LL_FillBuffer(uint32_t LayerIndex, void *pDst, uint32_t xSize, uint32_t ySize, uint32_t OffLine, uint32_t ColorIndex)
{
	  /*##-1- Configure the DMA2D Mode, Color Mode and output offset #############*/
	  Dma2dHandle.Init.Mode         = DMA2D_R2M;
	  Dma2dHandle.Init.ColorMode    = DMA2D_ARGB8888;
	  Dma2dHandle.Init.OutputOffset = OffLine;

  /* DMA2D Initialization */
  if(HAL_DMA2D_Init(&Dma2dHandle) == HAL_OK)
  {
    if(HAL_DMA2D_ConfigLayer(&Dma2dHandle, LayerIndex) == HAL_OK)
    {
      if (HAL_DMA2D_Start(&Dma2dHandle, ColorIndex | 0xFF000000, (uint32_t)pDst, xSize, ySize) == HAL_OK)
      {
        /* Polling For DMA transfer */
        HAL_DMA2D_PollForTransfer(&Dma2dHandle, 100);
      }
    }
  }
}


#endif

#if defined(STM32F769xx) || defined(STM32F779xx)

bool SetUpLCD(void* LcdStruct)
{
	//screen_open(LcdStruct);
	if(BSP_LCD_InitEx(LcdStruct) == LCD_OK)
	{
		return true;
	}
	else
		return false;
#ifdef SSD2119
	STM32f4_Discovery_LCD_Init();
#endif
	return true;
}

/**********************************************/
void _lcd_enable()
{
	  /* Send Display on DCS command to display */
	  HAL_DSI_ShortWrite(&(hdsi_eval),
	                     hdsivideo_handle.VirtualChannelID,
	                     DSI_DCS_SHORT_PKT_WRITE_P1,
	                     OTM8009A_CMD_DISPON,
	                     0x00);
}
/**********************************************/
void _lcd_disable()
{
	  /* Send Display off DCS Command to display */
	  HAL_DSI_ShortWrite(&(hdsi_eval),
	                     hdsivideo_handle.VirtualChannelID,
	                     DSI_DCS_SHORT_PKT_WRITE_P1,
	                     OTM8009A_CMD_DISPOFF,
	                     0x00);
}
//#######################################################################################
void _screen_backlight_on(void *_pDisplay)
{
	tDisplay *pDisplay = (tDisplay *)_pDisplay;
	gpio.out(pDisplay->BackLight, 1);
}
//#######################################################################################
void _screen_backlight_off(void *_pDisplay)
{
	tDisplay *pDisplay = (tDisplay *)_pDisplay;
	gpio.out(pDisplay->BackLight, 0);
}
//#######################################################################################
bool _screen_copy(void *_pDisplayTo, void *_pDisplayFrom, bool put_cursor, signed int X, signed int Y, unsigned int color)
{
	tDisplay *pDisplayTo = (tDisplay *)_pDisplayTo;
	tDisplay *pDisplayFrom = (tDisplay *)_pDisplayFrom;
	if(pDisplayTo->LcdTimings->X != pDisplayFrom->LcdTimings->X || pDisplayTo->LcdTimings->Y != pDisplayFrom->LcdTimings->Y)
		return false;
	CacheDataCleanBuff((unsigned int)pDisplayFrom->DisplayData, (pDisplayFrom->LcdTimings->X * pDisplayFrom->LcdTimings->Y * sizeof(pDisplayFrom->DisplayData[0])) + (pDisplayFrom->LcdTimings->palete_len * sizeof(pDisplayFrom->DisplayData[0])));
	signed int LineCnt = 0;
	volatile unsigned int* ScreenBuff = pDisplayTo->DisplayData + pDisplayTo->LcdTimings->palete_len;
	volatile unsigned int* _ScreenBuff = pDisplayFrom->DisplayData + pDisplayTo->LcdTimings->palete_len;

	for(; LineCnt < pDisplayTo->LcdTimings->Y; LineCnt ++)
	{
		memcpy((void *)(ScreenBuff + (pDisplayFrom->LcdTimings->X * LineCnt)), (void *)(_ScreenBuff + (pDisplayFrom->LcdTimings->X * LineCnt)), (sizeof(ScreenBuff[0]) * pDisplayTo->LcdTimings->X));
		if(put_cursor == true && LineCnt >= Y && LineCnt <= Y + 2)
		{
			unsigned int cnt_x = X;
			for(;cnt_x < X + 2; cnt_x++)
				_put_pixel(pDisplayTo, cnt_x, LineCnt, color);
		}
	}
	return true;
}
//#######################################################################################
void _box_cache_clean(void *_pDisplay, signed int x_start, signed int y_start, signed int x_len, signed int y_len)
{
	tDisplay *pDisplay = (tDisplay *)_pDisplay;

}
//#######################################################################################
void _put_rectangle(void *_pDisplay, signed int x_start, signed int y_start, signed int x_len, signed int y_len, bool fill, unsigned int color)
{
	tDisplay *pDisplay = (tDisplay *)_pDisplay;
	signed int x_end = x_start + x_len ,y_end = y_start + y_len;
	if(x_start >= pDisplay->sClipRegion.sXMax || y_start >= pDisplay->sClipRegion.sYMax || x_end < pDisplay->sClipRegion.sXMin || y_end < pDisplay->sClipRegion.sYMin)
		return;
	register signed int LineCnt = y_start;
	//volatile unsigned int* ScreenBuff = (volatile unsigned int *)LCD_FB_START_ADDRESS;
	unsigned int _color = color | 0xFF000000;
	if(fill)
	{
		if(LineCnt < pDisplay->sClipRegion.sYMin) LineCnt = pDisplay->sClipRegion.sYMin;
		signed int _x_start = x_start;
		if(_x_start < pDisplay->sClipRegion.sXMin) _x_start = pDisplay->sClipRegion.sXMin;
		register signed int _x_end = x_end;
		if(_x_end > pDisplay->sClipRegion.sXMax) _x_end = pDisplay->sClipRegion.sXMax;
		uint32_t  Xaddress = 0;
		Xaddress = (LtdcHandler.LayerCfg[0].FBStartAdress) + 4*(pDisplay->LcdTimings->X*LineCnt + _x_start);
		/* Fill the rectangle */
		LL_FillBuffer(0, (uint32_t *)Xaddress, _x_end - _x_start, y_end - LineCnt, pDisplay->LcdTimings->X - (_x_end - _x_start), _color);
		return;
	}

	_screen_put_horizontal_line(pDisplay, x_start, y_start, x_len - 1, 1, color);
	_screen_put_horizontal_line(pDisplay, x_start, (y_start + y_len) - 1, x_len, 1, color);
	_screen_put_vertical_line(pDisplay, x_start, y_start, y_len - 1, 1, color);
	_screen_put_vertical_line(pDisplay, (x_start + x_len) - 1, y_start, x_len, 1, color);
	return;
}
//#######################################################################################
void _put_pixel(void *_pDisplay, signed int X, signed int Y, unsigned int color)
{
	tDisplay *pDisplay = (tDisplay *)_pDisplay;
	if(X >= pDisplay->sClipRegion.sXMin && Y >= pDisplay->sClipRegion.sYMin && X < pDisplay->sClipRegion.sXMax && Y < pDisplay->sClipRegion.sYMax)
	{
		uint32_t  Xaddress = 0;
		Xaddress = (LtdcHandler.LayerCfg[0].FBStartAdress) + 4*(pDisplay->LcdTimings->X*Y + X);
		/* Fill the rectangle */
		LL_FillBuffer(0, (uint32_t *)Xaddress, 1, 1, pDisplay->LcdTimings->X - 1, color | 0xFF000000);
	}
}
//#######################################################################################
void _screen_put_rgb_array_16(void *_pDisplay, unsigned short *rgb_buffer, unsigned int x1, unsigned int y1,unsigned int width, unsigned int height)
{
	tDisplay *pDisplay = (tDisplay *)_pDisplay;
}
//#######################################################################################
void _screen_put_rgb_array_24(void *_pDisplay, unsigned char *rgb_buffer, unsigned long x1, unsigned long y1,unsigned long width, unsigned long height)
{
	tDisplay *pDisplay = (tDisplay *)_pDisplay;
}
//#######################################################################################
void _screen_put_rgb_array_32(void *_pDisplay, unsigned char *rgb_buffer, unsigned int x1, unsigned int y1,unsigned int width, unsigned int height)
{
	tDisplay *pDisplay = (tDisplay *)_pDisplay;

	/*//tWindow *ParentWindow = (tWindow*)settings->Internals.ParentWindow;
	//tDisplay *pDisplay = settings->Internals.pDisplay;
	//tRectangle back_up_clip = pDisplay->sClipRegion;
	//pDisplay->sClipRegion = settings->Internals.PictureWindowLimits;
	////put_pixel(pDisplay, X + settings->Position.X + 2, Y + settings->Position.Y + 2, color);
	tRectangle rectangle;
	rectangle.sXMin = 0;
	rectangle.sXMax = width;
	rectangle.sYMin = 0;
	rectangle.sYMax = height;
	tRectangle *dest_rectangle = &rectangle;
	tRectangle *src_rectangle = &rectangle;

	signed int dest_X_StartBox = x1;
	signed int dest_Y_StartBox = y1;
	//signed int dest_X_EndBox = settings->Internals.Position.X + settings->Internals.Size.X;
	//signed int dest_Y_EndBox = settings->Internals.Position.Y + settings->Internals.Size.Y;

	//Calculate the destination locations on the screen.
	dest_rectangle->sXMin += dest_X_StartBox;
	dest_rectangle->sXMax += dest_X_StartBox;
	dest_rectangle->sYMin += dest_Y_StartBox;
	dest_rectangle->sYMax += dest_Y_StartBox;
	if(dest_rectangle->sXMax < pDisplay->sClipRegion.sXMin ||
			dest_rectangle->sYMax < pDisplay->sClipRegion.sYMin ||
				dest_rectangle->sXMin >= pDisplay->sClipRegion.sXMax ||
					dest_rectangle->sYMin >= pDisplay->sClipRegion.sYMax)
		return;

	//Limit the destination area to the picture box size.
	clip_limit(dest_rectangle, &pDisplay->sClipRegion);

	signed int x_line_len = dest_rectangle->sXMax - dest_rectangle->sXMin;

	if(x_line_len <= 0)
		return;
	if(dest_X_StartBox <= dest_rectangle->sXMin)
		dest_X_StartBox = dest_rectangle->sXMin;

	signed int Y_cnt = dest_Y_StartBox;
	if(Y_cnt <= dest_rectangle->sYMin)
		Y_cnt = dest_rectangle->sYMin;

	signed int X_Start_Src_Buff = pDisplay->sClipRegion.sXMin;
	if(X_Start_Src_Buff > 0)
		X_Start_Src_Buff = 0;
	else
		X_Start_Src_Buff = (~X_Start_Src_Buff) + 1;
	signed int Y_Start_Src_Buff = pDisplay->sClipRegion.sYMin;
	if(Y_Start_Src_Buff > 0)
		Y_Start_Src_Buff = 0;
	else
		Y_Start_Src_Buff = (~Y_Start_Src_Buff) + 1;*/












	uint32_t destination = (uint32_t)((volatile unsigned int *)LCD_FB_START_ADDRESS) + ((y1 * pDisplay->LcdTimings->X + x1) * 4);
	uint32_t source      = (uint32_t)rgb_buffer;

	/*##-1- Configure the DMA2D Mode, Color Mode and output offset #############*/
	Dma2dHandle.Init.Mode         = DMA2D_M2M_PFC;
	Dma2dHandle.Init.ColorMode    = DMA2D_ARGB8888;
	Dma2dHandle.Init.OutputOffset = pDisplay->LcdTimings->X - width;

	/*##-2- DMA2D Callbacks Configuration ######################################*/
	Dma2dHandle.XferCpltCallback  = NULL;

	/*##-3- Foreground Configuration ###########################################*/
	Dma2dHandle.LayerCfg[0].AlphaMode = DMA2D_NO_MODIF_ALPHA;
	Dma2dHandle.LayerCfg[0].InputAlpha = 0xFF;
	Dma2dHandle.LayerCfg[0].InputColorMode = CM_ARGB8888;
	Dma2dHandle.LayerCfg[0].InputOffset = 0;

	Dma2dHandle.Instance          = DMA2D;

	/* DMA2D Initialization */
	if(HAL_DMA2D_Init(&Dma2dHandle) == HAL_OK)
	{
		if(HAL_DMA2D_ConfigLayer(&Dma2dHandle, 0) == HAL_OK)
		{
			if (HAL_DMA2D_Start(&Dma2dHandle, source, destination, width, height) == HAL_OK)
			{
				/* Polling For DMA transfer */
				HAL_DMA2D_PollForTransfer(&Dma2dHandle, 100);
			}
		}
	}
}
//#######################################################################################
void _screen_put_horizontal_line(void *_pDisplay, signed int X1, signed int X2, signed int Y, unsigned char width, unsigned int color)
{
	tDisplay *pDisplay = (tDisplay *)_pDisplay;
	if(width == 1 && (Y < pDisplay->sClipRegion.sYMin || Y >= pDisplay->sClipRegion.sYMax))
		return;
	register int X1_Tmp = X1, X2_Tmp = X1 + X2;
	if(X1_Tmp <= (int)pDisplay->sClipRegion.sXMin) X1_Tmp = (int)pDisplay->sClipRegion.sXMin;
	if(X1_Tmp >= (int)pDisplay->sClipRegion.sXMax) X1_Tmp = (int)pDisplay->sClipRegion.sXMax;
	if(X2_Tmp <= (int)pDisplay->sClipRegion.sXMin) X2_Tmp = (int)pDisplay->sClipRegion.sXMin;
	if(X2_Tmp >= (int)pDisplay->sClipRegion.sXMax) X2_Tmp = (int)pDisplay->sClipRegion.sXMax;
	Y -= (width >> 1);
	if(Y < (int)pDisplay->sClipRegion.sYMin) Y = (int)pDisplay->sClipRegion.sYMin;
	if(Y >= (int)pDisplay->sClipRegion.sYMax) Y = (int)pDisplay->sClipRegion.sYMax;
#ifdef USE_16_BIT_COLOR_DEPTH
	unsigned int _color = color;
#else
	unsigned int _color = color | 0xFF000000;
#endif
	uint32_t  Xaddress = 0;
	Xaddress = (LtdcHandler.LayerCfg[0].FBStartAdress) + 4*(pDisplay->LcdTimings->X* Y + X1_Tmp);
	/* Fill the rectangle */
	LL_FillBuffer(0, (uint32_t *)Xaddress, X2_Tmp - X1_Tmp, width, pDisplay->LcdTimings->X - (X2_Tmp - X1_Tmp), _color);

}
//#######################################################################################
void _screen_put_vertical_line(void *_pDisplay, signed int Y1, signed int Y2, signed int X, unsigned char width, unsigned int color)
{
	tDisplay *pDisplay = (tDisplay *)_pDisplay;
	if(width == 1 && (X < pDisplay->sClipRegion.sXMin || X >= pDisplay->sClipRegion.sXMax))
		return;
	register int Y1_Tmp = Y1, Y2_Tmp = Y1 + Y2;
	X -= (width >> 1);
	if(X <= (int)pDisplay->sClipRegion.sXMin) X = (int)pDisplay->sClipRegion.sXMin;
	if(X >= (int)pDisplay->sClipRegion.sXMax) X = (int)pDisplay->sClipRegion.sXMax;
	if(Y1_Tmp <= (int)pDisplay->sClipRegion.sYMin) Y1_Tmp = (int)pDisplay->sClipRegion.sYMin;
	if(Y1_Tmp >= (int)pDisplay->sClipRegion.sYMax) Y1_Tmp = (int)pDisplay->sClipRegion.sYMax;
	if(Y2_Tmp <= (int)pDisplay->sClipRegion.sYMin) Y2_Tmp = (int)pDisplay->sClipRegion.sYMin;
	if(Y2_Tmp >= (int)pDisplay->sClipRegion.sYMax) Y2_Tmp = (int)pDisplay->sClipRegion.sYMax;
#ifdef USE_16_BIT_COLOR_DEPTH
	unsigned int _color = color;
#else
	unsigned int _color = color | 0xFF000000;
#endif
	uint32_t  Xaddress = 0;
	Xaddress = (LtdcHandler.LayerCfg[0].FBStartAdress) + 4*(pDisplay->LcdTimings->X*Y1_Tmp + X);
	/* Fill the rectangle */
	LL_FillBuffer(0, (uint32_t *)Xaddress, width, Y2_Tmp - Y1_Tmp, pDisplay->LcdTimings->X - (width), _color);

}
//#######################################################################################
void _screen_clear(void *_pDisplay, unsigned int color)
{
	tDisplay *pDisplay = (tDisplay *)_pDisplay;
	_put_rectangle(_pDisplay, 0, 0, pDisplay->LcdTimings->X, pDisplay->LcdTimings->Y, true, color);
}
#else

bool SetUpLCD(void* LcdStruct)
{
	return false;
}

/**********************************************/
void _lcd_enable()
{
}
/**********************************************/
void _lcd_disable()
{

}
//#######################################################################################
void _screen_backlight_on(void *pDisplay)
{

}
//#######################################################################################
void _screen_backlight_off(void *pDisplay)
{

}
//#######################################################################################
bool _screen_copy(void *pDisplayTo, void *pDisplayFrom, bool put_cursor, signed int X, signed int Y, unsigned int color)
{

	return true;
}
//#######################################################################################
void _box_cache_clean(void *pDisplay, signed int x_start, signed int y_start, signed int x_len, signed int y_len)
{

}
//#######################################################################################
void _put_rectangle(void *pDisplay, signed int x_start, signed int y_start, signed int x_len, signed int y_len, bool fill, unsigned int color)
{

}
//#######################################################################################
void _put_pixel(void *pDisplay, signed int X, signed int Y, unsigned int color)
{

}
//#######################################################################################
void _screen_put_rgb_array_16(void *pDisplay, unsigned short *rgb_buffer, unsigned int x1, unsigned int y1,unsigned int width, unsigned int height)
{

}
//#######################################################################################
void _screen_put_rgb_array_24(void *pDisplay, unsigned char *rgb_buffer, unsigned long x1, unsigned long y1,unsigned long width, unsigned long height)
{

}
//#######################################################################################
void _screen_put_rgb_array_32(void *pDisplay, unsigned char *rgb_buffer, unsigned int x1, unsigned int y1,unsigned int width, unsigned int height)
{

}
//#######################################################################################
void _screen_put_horizontal_line(void *pDisplay, signed int X1, signed int X2, signed int Y, unsigned char width, unsigned int color)
{

}
//#######################################################################################
void _screen_put_vertical_line(void *pDisplay, signed int Y1, signed int Y2, signed int X, unsigned char width, unsigned int color)
{

}
//#######################################################################################
void _screen_clear(void *pDisplay, unsigned int color)
{

}

#endif
