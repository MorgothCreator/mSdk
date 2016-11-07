/*
 * lcd_interface.c
 *
 * Created: 2/8/2013 10:26:43 PM
 *  Author: XxXx
 */

#include "main.h"
#include <stdbool.h>
#include "device/otm8009a.h"
#include "board_properties.h"
#include "lcd_interface.h"
#include "sys/cache.h"
#include "sys/sysdelay.h"
#include "lib/gfx/controls_definition.h"
#include "api/gpio_api.h"

#include "include/stm32f4xx.h"
#include "driver/stm32f4xx_hal_dsi.h"
#include "driver/stm32f4xx_hal_dma2d.h"
#include "driver/stm32f4xx_hal_ltdc_ex.h"
#include "driver/stm32f4xx_hal_ltdc.h"
#include "driver/stm32f4xx_hal_gpio.h"
#include "driver/stm32f4xx_hal_rcc.h"
#include "driver/stm32f4xx_hal.h"
#include "driver/stm32469i_eval_sdram.h"

/**
  * @brief  OTM8009A delay
  * @param  Delay: Delay in ms
  */
void OTM8009A_IO_Delay(unsigned long Delay)
{
  HAL_Delay(Delay);
}

#ifdef LCD_TYPE_DSI

static DSI_VidCfgTypeDef hdsivideo_handle;
//DMA2D_HandleTypeDef hdma2d_eval;
LTDC_HandleTypeDef  hltdc_eval;
DSI_HandleTypeDef hdsi_eval;
DMA2D_HandleTypeDef Dma2dHandle;
uint32_t lcd_x_size = OTM8009A_800X480_WIDTH;
uint32_t lcd_y_size = OTM8009A_800X480_HEIGHT;

/**
  * @brief  LCD FB_StartAddress
  */
#define LCD_FB_START_ADDRESS       ((uint32_t)0xC0000000)
/** @brief Maximum number of LTDC layers
 */
#define LTDC_MAX_LAYER_NUMBER             ((uint32_t) 2)
/** @brief LTDC Background layer index
 */
#define LTDC_ACTIVE_LAYER_BACKGROUND      ((uint32_t) 0)

/** @brief LTDC Foreground layer index
 */
#define LTDC_ACTIVE_LAYER_FOREGROUND      ((uint32_t) 1)

//static uint32_t  ActiveLayer = LTDC_ACTIVE_LAYER_BACKGROUND;
/**
  * @brief  Current Drawing Layer properties variable
  */
//static LCD_DrawPropTypeDef DrawProp[LTDC_MAX_LAYER_NUMBER];
/**
  * @brief  DCS or Generic short/long write command
  * @param  NbParams: Number of parameters. It indicates the write command mode:
  *                 If inferior to 2, a long write command is performed else short.
  * @param  pParams: Pointer to parameter values table.
  * @retval HAL status
  */
static void DMA2D_Config(void);















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
void BSP_LCD_DrawBitmap(uint32_t Xpos, uint32_t Ypos, uint8_t *pbmp)
{
  uint32_t index = 0, width = 0, height = 0, bit_pixel = 0;
  uint32_t Address;
  uint32_t InputColorMode = 0;

  /* Get bitmap data address offset */
  index = *(__IO uint16_t *) (pbmp + 10);
  index |= (*(__IO uint16_t *) (pbmp + 12)) << 16;

  /* Read bitmap width */
  width = *(uint16_t *) (pbmp + 18);
  width |= (*(uint16_t *) (pbmp + 20)) << 16;

  /* Read bitmap height */
  height = *(uint16_t *) (pbmp + 22);
  height |= (*(uint16_t *) (pbmp + 24)) << 16;

  /* Read bit/pixel */
  bit_pixel = *(uint16_t *) (pbmp + 28);

  /* Set the address */
  Address = hltdc_eval.LayerCfg[0].FBStartAdress + (((lcd_x_size*Ypos) + Xpos)*(4));

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
    Address+=  (lcd_x_size*4);
    pbmp -= width*(bit_pixel/8);
  }
}




























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
/*******************************************************************************
                       LTDC, DMA2D and DSI BSP Routines
*******************************************************************************/
/**
  * @brief  Handles DMA2D interrupt request.
  * @note : Can be surcharged by application code implementation of the function.
  */
__weak void BSP_LCD_DMA2D_IRQHandler(void)
{
  HAL_DMA2D_IRQHandler(&Dma2dHandle);
}

/**
  * @brief  Handles DSI interrupt request.
  * @note : Can be surcharged by application code implementation of the function.
  */
__weak void BSP_LCD_DSI_IRQHandler(void)
{
  HAL_DSI_IRQHandler(&(hdsi_eval));
}


/**
  * @brief  Handles LTDC interrupt request.
  * @note : Can be surcharged by application code implementation of the function.
  */
__weak void BSP_LCD_LTDC_IRQHandler(void)
{
  HAL_LTDC_IRQHandler(&(hltdc_eval));
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
  * @brief  This function handles LTDC Error interrupt Handler.
  * @note : Can be surcharged by application code implementation of the function.
  */

__weak void BSP_LCD_LTDC_ER_IRQHandler(void)
{
  HAL_LTDC_IRQHandler(&(hltdc_eval));
}

/**
  * @brief  BSP LCD Reset
  *         Hw reset the LCD DSI activating its XRES signal (active low for some time)
  *         and desactivating it later.
  *         This signal is only cabled on Discovery Rev B and beyond.
  */
void BSP_LCD_Reset(void)
{
#if !defined(USE_STM32469I_DISCO_REVA)
/* EVAL Rev B and beyond : reset the LCD by activation of XRES (active low) connected to PH7 */
  GPIO_InitTypeDef  gpio_init_structure;

  __HAL_RCC_GPIOH_CLK_ENABLE();

    /* Configure the GPIO on PH7 */
    gpio_init_structure.Pin   = GPIO_PIN_7;
    gpio_init_structure.Mode  = GPIO_MODE_OUTPUT_OD;
    gpio_init_structure.Pull  = GPIO_NOPULL;
    gpio_init_structure.Speed = GPIO_SPEED_HIGH;

    HAL_GPIO_Init(GPIOH, &gpio_init_structure);

    /* Activate XRES active low */
    HAL_GPIO_WritePin(GPIOH, GPIO_PIN_7, GPIO_PIN_RESET);

    HAL_Delay(20); /* wait 20 ms */

    /* Desactivate XRES */
    HAL_GPIO_WritePin(GPIOH, GPIO_PIN_7, GPIO_PIN_SET);

    /* Wait for 10ms after releasing XRES before sending commands */
    HAL_Delay(10);
#else

#endif /* USE_STM32469I_DISCO_REVA == 0 */
}


/**
  * @brief  Initializes the LCD layers.
  * @param  LayerIndex: Layer foreground or background
  * @param  FB_Address: Layer frame buffer
  * @retval None
  */
void BSP_LCD_LayerDefaultInit(uint16_t LayerIndex, uint32_t FB_Address)
{
    LCD_LayerCfgTypeDef  Layercfg;

  /* Layer Init */
  Layercfg.WindowX0 = 0;
  Layercfg.WindowX1 = lcd_x_size;
  Layercfg.WindowY0 = 0;
  Layercfg.WindowY1 = lcd_y_size;
  Layercfg.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;
  Layercfg.FBStartAdress = FB_Address;
  Layercfg.Alpha = 255;
  Layercfg.Alpha0 = 0;
  Layercfg.Backcolor.Blue = 0;
  Layercfg.Backcolor.Green = 0;
  Layercfg.Backcolor.Red = 0;
  Layercfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
  Layercfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
  Layercfg.ImageWidth = lcd_x_size;
  Layercfg.ImageHeight = lcd_y_size;

  HAL_LTDC_ConfigLayer(&hltdc_eval, &Layercfg, LayerIndex);
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
  static RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;
  uint32_t LcdClock  = 27429; /*!< LcdClk = 27429 kHz */
  uint32_t Clockratio  = 0;
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

#if !defined(USE_STM32469I_DISCO_REVA)
  dsiPllInit.PLLNDIV  = 125;
  dsiPllInit.PLLIDF   = DSI_PLL_IN_DIV2;
  dsiPllInit.PLLODF   = DSI_PLL_OUT_DIV1;
#else
  dsiPllInit.PLLNDIV  = 100;
  dsiPllInit.PLLIDF   = DSI_PLL_IN_DIV5;
  dsiPllInit.PLLODF   = DSI_PLL_OUT_DIV1;
#endif
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
  VSA  = pDisplay->raster_timings->vsw;        /* 12 */
  VBP  = pDisplay->raster_timings->vbp;          /* 12 */
  VFP  = pDisplay->raster_timings->vfp;          /* 12 */
  HSA  = pDisplay->raster_timings->hsw;        /* 120 */
  HBP  = pDisplay->raster_timings->hbp;          /* 120 */
  HFP  = pDisplay->raster_timings->hfp;          /* 120 */
  if(pDisplay->Orientation == 90 || pDisplay->Orientation == 270)
  {
	    lcd_x_size = pDisplay->raster_timings->X;  /* 480 */
	    lcd_y_size = pDisplay->raster_timings->Y; /* 800 */
  }
  else
  {
    /* lcd_orientation == LCD_ORIENTATION_LANDSCAPE */
	    lcd_x_size = pDisplay->raster_timings->Y;  /* 800 */
	    lcd_y_size = pDisplay->raster_timings->X; /* 480 */
  }

  HACT = lcd_x_size;
  VACT = lcd_y_size;


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
  hdsivideo_handle.LPVACTLargestPacketSize = 64;


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
  hltdc_eval.Init.HorizontalSync = (HSA - 1);
  hltdc_eval.Init.AccumulatedHBP = (HSA + HBP - 1);
  hltdc_eval.Init.AccumulatedActiveW = (lcd_x_size + HSA + HBP - 1);
  hltdc_eval.Init.TotalWidth = (lcd_x_size + HSA + HBP + HFP - 1);

  /* Initialize the LCD pixel width and pixel height */
  hltdc_eval.LayerCfg->ImageWidth  = lcd_x_size;
  hltdc_eval.LayerCfg->ImageHeight = lcd_y_size;


  /* LCD clock configuration */
  /* PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz */
  /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN = 384 Mhz */
  /* PLLLCDCLK = PLLSAI_VCO Output/PLLSAIR = 384 MHz / 7 = 54.857 MHz */
  /* LTDC clock frequency = PLLLCDCLK / LTDC_PLLSAI_DIVR_2 = 54.857 MHz / 2 = 27.429 MHz */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 384;
  PeriphClkInitStruct.PLLSAI.PLLSAIR = 7;
  PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_2;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

  /* Background value */
  hltdc_eval.Init.Backcolor.Blue = 0;
  hltdc_eval.Init.Backcolor.Green = 0;
  hltdc_eval.Init.Backcolor.Red = 0;
  hltdc_eval.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  hltdc_eval.Instance = LTDC;

  /* Get LTDC Configuration from DSI Configuration */
  HAL_LTDC_StructInitFromVideoConfig(&(hltdc_eval), &(hdsivideo_handle));

  /* Initialize the LTDC */
  HAL_LTDC_Init(&hltdc_eval);

  /* Initialize the font */
  DMA2D_Config();
  BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS);
  //BSP_LCD_SetFont(&LCD_DEFAULT_FONT);

/************************End LTDC Initialization*******************************/


/***********************OTM8009A Initialization********************************/

  /* Initialize the OTM8009A LCD Display IC Driver (KoD LCD IC Driver)
  *  depending on configuration set in 'hdsivideo_handle'.
  */
  OTM8009A_Init(hdsivideo_handle.ColorCoding, pDisplay->Orientation);

/***********************End OTM8009A Initialization****************************/
  pDisplay->sClipRegion.sXMax = pDisplay->raster_timings->X;
  pDisplay->sClipRegion.sYMax = pDisplay->raster_timings->Y;
  return LCD_OK;
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

/**
  * @brief  Converts a line to an ARGB8888 pixel format.
  * @param  pSrc: Pointer to source buffer
  * @param  pDst: Output color
  * @param  xSize: Buffer width
  * @param  ColorMode: Input color mode
  * @retval None
  */
static void CopyBuffer(uint32_t *pSrc, uint32_t *pDst, uint16_t x, uint16_t y, uint16_t xsize, uint16_t ysize)
{

  uint32_t destination = (uint32_t)pDst + (y * lcd_x_size + x) * 4;
  uint32_t source      = (uint32_t)pSrc;

  /*##-1- Configure the DMA2D Mode, Color Mode and output offset #############*/
  Dma2dHandle.Init.Mode         = DMA2D_M2M;
  Dma2dHandle.Init.ColorMode    = DMA2D_ARGB8888;
  Dma2dHandle.Init.OutputOffset = lcd_x_size - xsize;

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
      if (HAL_DMA2D_Start(&Dma2dHandle, source, destination, xsize, ysize) == HAL_OK)
      {
        /* Polling For DMA transfer */
        HAL_DMA2D_PollForTransfer(&Dma2dHandle, 100);
      }
    }
  }
}

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

#if defined(STM32F469xx) || defined(STM32F479xx)

bool SetUpLCD(void* LcdStruct)
{
	//screen_open(LcdStruct);
#ifdef LCD_TYPE_DSI
	if(BSP_LCD_InitEx(LcdStruct) == LCD_OK)
	{
		_lcd_enable();
		return true;
	}
	else
		return false;
#endif
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
	if(pDisplay->invert_backlight) gpio_out(pDisplay->BackLight, 0);
			else gpio_out(pDisplay->BackLight, 1);
}
//#######################################################################################
void _screen_backlight_off(void *_pDisplay)
{
	tDisplay *pDisplay = (tDisplay *)_pDisplay;
	if(pDisplay->invert_backlight) gpio_out(pDisplay->BackLight, 1);
			else gpio_out(pDisplay->BackLight, 0);
}
//#######################################################################################
bool _screen_copy(void *_pDisplayTo, void *_pDisplayFrom, bool put_cursor, signed int X, signed int Y, unsigned int color)
{
	tDisplay *pDisplayTo = (tDisplay *)_pDisplayTo;
	tDisplay *pDisplayFrom = (tDisplay *)_pDisplayFrom;
	if(pDisplayTo->raster_timings->X != pDisplayFrom->raster_timings->X || pDisplayTo->raster_timings->Y != pDisplayFrom->raster_timings->Y)
		return false;
	CacheDataCleanBuff((unsigned int)pDisplayFrom->DisplayData, (pDisplayFrom->raster_timings->X * pDisplayFrom->raster_timings->Y * sizeof(pDisplayFrom->DisplayData[0])) + (pDisplayFrom->raster_timings->palete_len * sizeof(pDisplayFrom->DisplayData[0])));
	signed int LineCnt = 0;
	volatile unsigned int* ScreenBuff = pDisplayTo->DisplayData + pDisplayTo->raster_timings->palete_len;
	volatile unsigned int* _ScreenBuff = pDisplayFrom->DisplayData + pDisplayTo->raster_timings->palete_len;

	for(; LineCnt < pDisplayTo->raster_timings->Y; LineCnt ++)
	{
		memcpy((void *)(ScreenBuff + (pDisplayFrom->raster_timings->X * LineCnt)), (void *)(_ScreenBuff + (pDisplayFrom->raster_timings->X * LineCnt)), (sizeof(ScreenBuff[0]) * pDisplayTo->raster_timings->X));
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
		Xaddress = (hltdc_eval.LayerCfg[0].FBStartAdress) + 4*(pDisplay->raster_timings->X*LineCnt + _x_start);
		/* Fill the rectangle */
		LL_FillBuffer(0, (uint32_t *)Xaddress, _x_end - _x_start, y_end - LineCnt, pDisplay->raster_timings->X - (_x_end - _x_start), _color);
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
		Xaddress = (hltdc_eval.LayerCfg[0].FBStartAdress) + 4*(pDisplay->raster_timings->X*Y + X);
		/* Fill the rectangle */
		LL_FillBuffer(0, (uint32_t *)Xaddress, 1, 1, pDisplay->raster_timings->X - 1, color | 0xFF000000);
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

	signed int x_end = x1 + width;
	signed int y_end = y1 + height;
	if(x1 >= pDisplay->sClipRegion.sXMax || y1 >= pDisplay->sClipRegion.sYMax || x_end < pDisplay->sClipRegion.sXMin || y_end < pDisplay->sClipRegion.sYMin)
		return;
	register signed int LineCnt = y1;
	if(y1 < pDisplay->sClipRegion.sYMin)
		y1 = pDisplay->sClipRegion.sYMin;
	signed int _y_end = y_end;
	if(_y_end > pDisplay->sClipRegion.sYMax)
		_y_end = pDisplay->sClipRegion.sYMax;
	signed int x_start = x1;
	if(x_start < pDisplay->sClipRegion.sXMin)
		x_start = pDisplay->sClipRegion.sXMin;
	signed int _x_end = x_end;
	if(_x_end > pDisplay->sClipRegion.sXMax)
		_x_end = pDisplay->sClipRegion.sXMax;

	if(_y_end > pDisplay->sClipRegion.sYMax)
		_y_end = pDisplay->sClipRegion.sYMax;
	if(_x_end > pDisplay->sClipRegion.sXMax)
		_x_end = pDisplay->sClipRegion.sXMax;

	signed int src_x_offset = 0;
	signed int src_y_offset = 0;
	if(x1 < pDisplay->sClipRegion.sXMin)
		src_x_offset = 0 - x1;
	if(y1 < pDisplay->sClipRegion.sYMin)
		src_y_offset = 0 - y1;
	signed int src_x_max = width - src_x_offset;
	signed int src_y_max = height - src_y_offset;
	if(x1 > width - src_x_offset)
		src_x_max = width - src_x_offset;
	if(y1 > height - src_y_offset)
		src_y_max = height - src_y_offset;

	uint32_t destination = (uint32_t)((volatile unsigned int *)LCD_FB_START_ADDRESS) + (y1 * lcd_x_size + x_start) * 4;
	uint32_t source      = (uint32_t)rgb_buffer + (width * src_y_offset + src_x_offset);

	/*##-1- Configure the DMA2D Mode, Color Mode and output offset #############*/
	Dma2dHandle.Init.Mode         = DMA2D_M2M;
	Dma2dHandle.Init.ColorMode    = DMA2D_ARGB8888;
	Dma2dHandle.Init.OutputOffset = lcd_x_size - (_x_end - x_start);

	/*##-2- DMA2D Callbacks Configuration ######################################*/
	Dma2dHandle.XferCpltCallback  = NULL;

	/*##-3- Foreground Configuration ###########################################*/
	Dma2dHandle.LayerCfg[0].AlphaMode = DMA2D_NO_MODIF_ALPHA;
	Dma2dHandle.LayerCfg[0].InputAlpha = 0xFF;
	Dma2dHandle.LayerCfg[0].InputColorMode = CM_ARGB8888;
	Dma2dHandle.LayerCfg[0].InputOffset = width - (src_x_max - src_x_offset);

	Dma2dHandle.Instance          = DMA2D;

	/* DMA2D Initialization */
	if(HAL_DMA2D_Init(&Dma2dHandle) == HAL_OK)
	{
		if(HAL_DMA2D_ConfigLayer(&Dma2dHandle, 0) == HAL_OK)
		{
			if (HAL_DMA2D_Start(&Dma2dHandle, source, destination, width - src_x_offset, height - src_y_offset) == HAL_OK)
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
	Xaddress = (hltdc_eval.LayerCfg[0].FBStartAdress) + 4*(pDisplay->raster_timings->X* Y + X1_Tmp);
	/* Fill the rectangle */
	LL_FillBuffer(0, (uint32_t *)Xaddress, X2_Tmp - X1_Tmp, width, pDisplay->raster_timings->X - (X2_Tmp - X1_Tmp), _color);

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
	Xaddress = (hltdc_eval.LayerCfg[0].FBStartAdress) + 4*(pDisplay->raster_timings->X*Y1_Tmp + X);
	/* Fill the rectangle */
	LL_FillBuffer(0, (uint32_t *)Xaddress, width, Y2_Tmp - Y1_Tmp, pDisplay->raster_timings->X - (width), _color);

}
//#######################################################################################
void _screen_clear(void *_pDisplay, unsigned int color)
{
	tDisplay *pDisplay = (tDisplay *)_pDisplay;
	_put_rectangle(_pDisplay, 0, 0, pDisplay->raster_timings->X, pDisplay->raster_timings->Y, true, color);
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
