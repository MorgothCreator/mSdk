/*
 * lcd_interface.c
 *
 * Created: 2/8/2013 10:26:43 PM
 *  Author: XxXx
 */

#include <stdbool.h>
#include "board_properties.h"
#include "lcd_interface.h"
#include "sys/cache.h"
#include "sys/sysdelay.h"
#include "lib/gfx/controls_definition.h"
#include "api/gpio_api.h"

#include "include/stm32f4xx.h"
//#include "driver/stm32f4xx_hal_fsmc.h"
#include "driver/stm32f4xx_hal_gpio.h"
#include "driver/stm32f4xx_hal_rcc.h"

//#include "device/ili9341.h"

#ifdef SSD2119
/** @defgroup stm32f4_discovery_LCD_Private define
  * @{
  */
#define LCD_RST_PIN                  (GPIO_Pin_3)
#define LCD_RST_PORT                 (GPIOD)

#define LCD_PWM_PIN                  (GPIO_Pin_13)
#define LCD_PWM_PORT                 (GPIOD)

/* Note: LCD /CS is NE1 - Bank 1 of NOR/SRAM Bank 1~4 */
#define  LCD_BASE_Data               ((u32)(0x60000000|0x00100000))
#define  LCD_BASE_Addr               ((u32)(0x60000000|0x00000000))
#define  LCD_CMD                     (*(vu16 *)LCD_BASE_Addr)
#define  LCD_Data                    (*(vu16 *)LCD_BASE_Data)

#define MAX_POLY_CORNERS             200
#define POLY_Y(Z)                    ((int32_t)((Points + Z)->X))
#define POLY_X(Z)                    ((int32_t)((Points + Z)->Y))

/**
  * @brief  Writes to the selected LCD register.
  * @param  LCD_Reg: address of the selected register.
  * @param  LCD_RegValue: value to write to the selected register.
  * @retval None
  */
void LCD_WriteReg(unsigned char LCD_Reg, unsigned short LCD_RegValue)
{
  /* Write 16-bit Index, then Write Reg */
  LCD_CMD = LCD_Reg;
  /* Write 16-bit Reg */
  LCD_Data = LCD_RegValue;
}

/**
  * @brief  Sets the cursor position.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @retval None
  */
void LCD_SetCursor(unsigned short Xpos, unsigned short Ypos)
{
  /* Set the X address of the display cursor.*/
  LCD_WriteReg(SSD2119_X_RAM_ADDR_REG, Xpos);

  /* Set the Y address of the display cursor.*/
  LCD_WriteReg(SSD2119_Y_RAM_ADDR_REG, Ypos);
}

/**
  * @brief  Reads the selected LCD Register.
  * @param  LCD_Reg: address of the selected register.
  * @retval LCD Register Value.
  */
unsigned short LCD_ReadReg(unsigned char LCD_Reg)
{
  /* Write 16-bit Index (then Read Reg) */
  LCD_CMD = LCD_Reg;
  /* Read 16-bit Reg */
  return (LCD_Data);
}

/**
  * @brief  Prepare to write to the LCD RAM.
  * @param  None
  * @retval None
  */
void LCD_WriteRAM_Prepare(void)
{
	LCD_CMD = SSD2119_RAM_DATA_REG;
}

/**
  * @brief  Writes to the LCD RAM.
  * @param  RGB_Code: the pixel color in RGB mode (5-6-5).
  * @retval None
  */
void LCD_WriteRAM(unsigned short RGB_Code)
{
  /* Write 16-bit GRAM Reg */
  LCD_Data = RGB_Code;
}

/**
  * @brief  Reads the LCD RAM.
  * @param  None
  * @retval LCD RAM Value.
  */
unsigned short LCD_ReadRAM(void)
{
  /* Write 16-bit Index (then Read Reg) */
//  LCD_CMD = SSD2119_RAM_DATA_REG; /* Select GRAM Reg */
  /* Read 16-bit Reg */
  return LCD_Data;
}

/**
LCD_DisplayOff
  */
void LCD_DisplayOff(void)
{

}
/**
  * @brief  LCD Default FSMC Init
  * @param  None
  * @retval None
  */
void LCD_DeInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /*!< LCD Display Off */
  LCD_DisplayOff();

  /* BANK 3 (of NOR/SRAM Bank 1~4) is disabled */
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM3, ENABLE);

  /*!< LCD_SPI DeInit */
  FSMC_NORSRAMDeInit(FSMC_Bank1_NORSRAM3);

/*-- GPIO Configuration ------------------------------------------------------*/
  /* SRAM Data lines configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 |
                                GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_MCO);


  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
                                GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 |
                                GPIO_Pin_15;

  GPIO_Init(GPIOE, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOE, GPIO_PinSource7 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource8 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource9 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource10 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource11 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource12 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource13 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource14 , GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource15 , GPIO_AF_MCO);

  /* SRAM Address lines configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 |
                                GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_12 | GPIO_Pin_13 |
                                GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource0, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource1, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource2, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource3, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource4, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource5, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource12, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource13, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource14, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource15, GPIO_AF_MCO);


  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 |
                                GPIO_Pin_4 | GPIO_Pin_5;

  GPIO_Init(GPIOG, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOG,GPIO_PinSource0, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOG,GPIO_PinSource1, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOG,GPIO_PinSource2, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOG,GPIO_PinSource3, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOG,GPIO_PinSource4, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOG,GPIO_PinSource5, GPIO_AF_MCO);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;

  GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOD,GPIO_PinSource11, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource12, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource13, GPIO_AF_MCO);

  /* NOE and NWE configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 |GPIO_Pin_5;

  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource4, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource5, GPIO_AF_MCO);

  /* NE3 configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;

  GPIO_Init(GPIOG, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource12, GPIO_AF_MCO);

  /* NBL0, NBL1 configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOE,GPIO_PinSource0, GPIO_AF_MCO);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource1, GPIO_AF_MCO);
}

/**
  * @brief  Configures LCD Control lines (FSMC Pins) in alternate function mode.
  * @param  None
  * @retval None
  */
void LCD_CtrlLinesConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIOB, GPIOD, GPIOE, GPIOF, GPIOG and AFIO clocks */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE |
                         RCC_AHB1Periph_GPIOF, ENABLE);

/*-- GPIO Configuration ------------------------------------------------------*/
  /* SRAM Data lines,  NOE and NWE configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 |
                                GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15 |
                                GPIO_Pin_4 |GPIO_Pin_5;;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FSMC);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
                                GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 |
                                GPIO_Pin_15;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOE, GPIO_PinSource7 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource8 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource9 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource10 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource11 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource12 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource13 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource14 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource15 , GPIO_AF_FSMC);

  /* SRAM Address lines configuration LCD-DC */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource3, GPIO_AF_FSMC);

  /* NE3 configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FSMC);

  /* LCD RST configuration */
  GPIO_InitStructure.GPIO_Pin = LCD_RST_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

  GPIO_Init(LCD_RST_PORT, &GPIO_InitStructure);

   /* LCD pwm configuration */
  /*GPIO_InitStructure.GPIO_Pin = LCD_PWM_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

  GPIO_Init(LCD_PWM_PORT, &GPIO_InitStructure);
  GPIO_SetBits(LCD_PWM_PORT, LCD_PWM_PIN);*/
}

/**
  * @brief  Configures the Parallel interface (FSMC) for LCD(Parallel mode)
  * @param  None
  * @retval None
  */
void LCD_FSMCConfig(void)
{
  FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef  p;

  /* Enable FSMC clock */
  RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);

/*-- FSMC Configuration ------------------------------------------------------*/
/*----------------------- SRAM Bank 1 ----------------------------------------*/
  /* FSMC_Bank1_NORSRAM4 configuration */
  p.FSMC_AddressSetupTime = 1;
  p.FSMC_AddressHoldTime = 0;
  p.FSMC_DataSetupTime = 9;
  p.FSMC_BusTurnAroundDuration = 0;
  p.FSMC_CLKDivision = 0;
  p.FSMC_DataLatency = 0;
  p.FSMC_AccessMode = FSMC_AccessMode_A;
  /* Color LCD configuration ------------------------------------
     LCD configured as follow:
        - Data/Address MUX = Disable
        - Memory Type = SRAM
        - Data Width = 16bit
        - Write Operation = Enable
        - Extended Mode = Enable
        - Asynchronous Wait = Disable */

  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
  FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;

  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);

  /* Enable FSMC NOR/SRAM Bank1 */
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
}

/**
  * @brief  LCD Init.
  * @retval None
  */
void STM32f4_Discovery_LCD_Init(void)
{

  unsigned long ulCount;

  /* Configure the LCD Control pins */
  LCD_CtrlLinesConfig();

  /* Configure the FSMC Parallel interface */
  LCD_FSMCConfig();

  Sysdelay(5);

  /* Reset LCD */
  GPIO_ResetBits(LCD_RST_PORT, LCD_RST_PIN);
  Sysdelay(10);
  GPIO_SetBits(LCD_RST_PORT, LCD_RST_PIN);

  /*
  SSD2119Init(void)
  */
  /* Enter sleep mode (if we are not already there).*/
  LCD_WriteReg(SSD2119_SLEEP_MODE_1_REG, 0x0001);

  /* Set initial power parameters. */
  LCD_WriteReg(SSD2119_PWR_CTRL_5_REG, 0x00B2);
  LCD_WriteReg(SSD2119_VCOM_OTP_1_REG, 0x0006);

  /* Start the oscillator.*/
  LCD_WriteReg(SSD2119_OSC_START_REG, 0x0001);

  /* Set pixel format and basic display orientation (scanning direction).*/
  LCD_WriteReg(SSD2119_OUTPUT_CTRL_REG, 0x30EF);
  LCD_WriteReg(SSD2119_LCD_DRIVE_AC_CTRL_REG, 0x0600);

  /* Exit sleep mode.*/
  LCD_WriteReg(SSD2119_SLEEP_MODE_1_REG, 0x0000);
  Sysdelay(5);

  /* Configure pixel color format and MCU interface parameters.*/
  LCD_WriteReg(SSD2119_ENTRY_MODE_REG, ENTRY_MODE_DEFAULT);

  /* Set analog parameters */
  LCD_WriteReg(SSD2119_SLEEP_MODE_2_REG, 0x0999);
  LCD_WriteReg(SSD2119_ANALOG_SET_REG, 0x3800);

  /* Enable the display */
  LCD_WriteReg(SSD2119_DISPLAY_CTRL_REG, 0x0033);

  /* Set VCIX2 voltage to 6.1V.*/
  LCD_WriteReg(SSD2119_PWR_CTRL_2_REG, 0x0005);

  /* Configure gamma correction.*/
  LCD_WriteReg(SSD2119_GAMMA_CTRL_1_REG, 0x0000);
  LCD_WriteReg(SSD2119_GAMMA_CTRL_2_REG, 0x0303);
  LCD_WriteReg(SSD2119_GAMMA_CTRL_3_REG, 0x0407);
  LCD_WriteReg(SSD2119_GAMMA_CTRL_4_REG, 0x0301);
  LCD_WriteReg(SSD2119_GAMMA_CTRL_5_REG, 0x0301);
  LCD_WriteReg(SSD2119_GAMMA_CTRL_6_REG, 0x0403);
  LCD_WriteReg(SSD2119_GAMMA_CTRL_7_REG, 0x0707);
  LCD_WriteReg(SSD2119_GAMMA_CTRL_8_REG, 0x0400);
  LCD_WriteReg(SSD2119_GAMMA_CTRL_9_REG, 0x0a00);
  LCD_WriteReg(SSD2119_GAMMA_CTRL_10_REG, 0x1000);

  /* Configure Vlcd63 and VCOMl */
  LCD_WriteReg(SSD2119_PWR_CTRL_3_REG, 0x000A);
  LCD_WriteReg(SSD2119_PWR_CTRL_4_REG, 0x2E00);

  /* Set the display size and ensure that the GRAM window is set to allow
     access to the full display buffer.*/
  LCD_WriteReg(SSD2119_V_RAM_POS_REG, (LCD_PIXEL_HEIGHT-1) << 8);
  LCD_WriteReg(SSD2119_H_RAM_START_REG, 0x0000);
  LCD_WriteReg(SSD2119_H_RAM_END_REG, LCD_PIXEL_WIDTH-1);

  LCD_WriteReg(SSD2119_X_RAM_ADDR_REG, 0x00);
  LCD_WriteReg(SSD2119_Y_RAM_ADDR_REG, 0x00);

  /* clear the lcd  */
  LCD_WriteReg(SSD2119_RAM_DATA_REG, 0x0000);
  for(ulCount = 0; ulCount < (LCD_PIXEL_WIDTH * LCD_PIXEL_HEIGHT); ulCount++)
  {
    LCD_WriteRAM(0xFFFF);
  }
  //LCD_SetFont(&LCD_DEFAULT_FONT);
}
#endif
#if defined(STM32F429xx) || defined(STM32F439xx) || defined(STM32F469xx) || defined(STM32F479xx)
#include "driver/stm32f4xx_hal_conf.h"
#include "driver/stm32f4xx_hal_ltdc.h"
#include "driver/stm32f4xx_hal_ltdc_ex.h"
#include "driver/stm32f4xx_hal_dma2d.h"
#include "driver/stm32f4xx_hal_rcc.h"
#include "driver/stm32f4xx_hal_rcc_ex.h"
#include "lib/device/ili9341.h"
#include "lcd_interface_def.h"
/** @defgroup STM32F429I_DISCOVERY_LCD_Private_Variables
  * @{
  */
static LTDC_HandleTypeDef  LtdcHandler;
static DMA2D_HandleTypeDef Dma2dHandler;
static RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

/* Default LCD configuration with LCD Layer 1 */
static uint32_t ActiveLayer = 0;
static LCD_DrawPropTypeDef DrawProp[MAX_LAYER_NUMBER];
LCD_DrvTypeDef  *LcdDrv;
/**
  * @brief  Sets the cursor position.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @retval None
  */
void LCD_SetCursor(unsigned short Xpos, unsigned short Ypos)
{
  /* Set the X address of the display cursor.*/
  //LCD_WriteReg(SSD2119_X_RAM_ADDR_REG, Xpos);

  /* Set the Y address of the display cursor.*/
  //LCD_WriteReg(SSD2119_Y_RAM_ADDR_REG, Ypos);
}
/**
  * @brief  Prepare to write to the LCD RAM.
  * @param  None
  * @retval None
  */
void LCD_WriteRAM_Prepare(void)
{
	//LCD_CMD = SSD2119_RAM_DATA_REG;
}

/**
  * @brief  Writes to the LCD RAM.
  * @param  RGB_Code: the pixel color in RGB mode (5-6-5).
  * @retval None
  */
void LCD_WriteRAM(unsigned short RGB_Code)
{
  /* Write 16-bit GRAM Reg */
  //LCD_Data = RGB_Code;
}

/**
  * @brief  Initializes the LCD.
  * @param  None
  * @retval LCD state
  */
uint8_t BSP_LCD_Init(void)
{
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
    LtdcHandler.Init.HorizontalSync = ILI9341_HSYNC;
    /* Configure vertical synchronization height */
    LtdcHandler.Init.VerticalSync = ILI9341_VSYNC;
    /* Configure accumulated horizontal back porch */
    LtdcHandler.Init.AccumulatedHBP = ILI9341_HBP;
    /* Configure accumulated vertical back porch */
    LtdcHandler.Init.AccumulatedVBP = ILI9341_VBP;
    /* Configure accumulated active width */
    LtdcHandler.Init.AccumulatedActiveW = 269;
    /* Configure accumulated active height */
    LtdcHandler.Init.AccumulatedActiveH = 323;
    /* Configure total width */
    LtdcHandler.Init.TotalWidth = 279;
    /* Configure total height */
    LtdcHandler.Init.TotalHeigh = 327;

    /* Configure R,G,B component values for LCD background color */
    LtdcHandler.Init.Backcolor.Red= 0;
    LtdcHandler.Init.Backcolor.Blue= 0;
    LtdcHandler.Init.Backcolor.Green= 0;

    /* LCD clock configuration */
    /* PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz */
    /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN = 192 Mhz */
    /* PLLLCDCLK = PLLSAI_VCO Output/PLLSAIR = 192/4 = 48 Mhz */
    /* LTDC clock frequency = PLLLCDCLK / LTDC_PLLSAI_DIVR_8 = 48/4 = 6Mhz */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
    PeriphClkInitStruct.PLLSAI.PLLSAIN = 192;
    PeriphClkInitStruct.PLLSAI.PLLSAIR = 4;
    PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_8;
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

    /* Polarity */
    LtdcHandler.Init.HSPolarity = LTDC_HSPOLARITY_AL;
    LtdcHandler.Init.VSPolarity = LTDC_VSPOLARITY_AL;
    LtdcHandler.Init.DEPolarity = LTDC_DEPOLARITY_AL;
    LtdcHandler.Init.PCPolarity = LTDC_PCPOLARITY_IPC;

    MspInit();
    HAL_LTDC_Init(&LtdcHandler);

    /* Select the device */
    LcdDrv = &ili9341_drv;

    /* LCD Init */
    LcdDrv->Init();

    /* Initialize the SDRAM */
    BSP_SDRAM_Init();

    /* Initialize the font */
    //BSP_LCD_SetFont(&LCD_DEFAULT_FONT);

  return LCD_OK;
}


bool SetUpLCD(tDisplay* LcdStruct)
{
	//screen_open(LcdStruct);

#ifdef SSD2119
	STM32f4_Discovery_LCD_Init();
#endif
	return true;
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
void _screen_backlight_on(tDisplay *pDisplay)
{
	if(pDisplay->invert_backlight) gpio_out(pDisplay->BackLight, 0);
			else gpio_out(pDisplay->BackLight, 1);
}
//#######################################################################################
void _screen_backlight_off(tDisplay *pDisplay)
{
	if(pDisplay->invert_backlight) gpio_out(pDisplay->BackLight, 1);
			else gpio_out(pDisplay->BackLight, 0);
}
//#######################################################################################
bool _screen_copy(tDisplay *pDisplayTo, tDisplay *pDisplayFrom, bool put_cursor, signed int X, signed int Y, unsigned int color)
{
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
void _box_cache_clean(tDisplay *pDisplay, signed int x_start, signed int y_start, signed int x_len, signed int y_len)
{

}
//#######################################################################################
void _put_rectangle(tDisplay *pDisplay, signed int x_start, signed int y_start, signed int x_len, signed int y_len, bool fill, unsigned int color)
{
	if(fill == true)
	{
		while(y_len--)
		{
			_screen_put_horizontal_line(pDisplay, x_start, y_start++, x_len, 1, color);
		}
	}
	else
	{
		_screen_put_horizontal_line(pDisplay, x_start, y_start, x_len - 1, 1, color);
		_screen_put_horizontal_line(pDisplay, x_start, (y_start + y_len) - 1, x_len, 1, color);
		_screen_put_vertical_line(pDisplay, x_start, y_start, y_len - 1, 1, color);
		_screen_put_vertical_line(pDisplay, (x_start + x_len) - 1, y_start, x_len, 1, color);
	}
	return;
}
//#######################################################################################
void _put_pixel(tDisplay *pDisplay, signed int X, signed int Y, unsigned int color)
{
	if(X < pDisplay->sClipRegion.sXMin || X >= pDisplay->sClipRegion.sXMax || Y < pDisplay->sClipRegion.sYMin || Y >= pDisplay->sClipRegion.sYMax)
	{
		return;
	}
	LCD_SetCursor(X, Y);
	LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
	LCD_WriteRAM(color);
}
//#######################################################################################
void _screen_put_rgb_array_16(void *_pDisplay, unsigned short *rgb_buffer, unsigned int x1, unsigned int y1,unsigned int width, unsigned int height)
{
	/*tDisplay *pDisplay = (tDisplay *)_pDisplay;
	///Write rgb array to video memory
	unsigned char *Buff = (unsigned char *)rgb_buffer;
	//unsigned long Length = (width*height)<<1;

	int   x, y;
	volatile unsigned char Tmp1;
	volatile unsigned char Tmp2;
	unsigned int color;
	for(y =y1; y<(height+y1);y++)
	{
		for(x = x1; x < width+x1; x++)
		{
			Tmp2 = *Buff++;
			Tmp1 = *Buff++;
			//color.blue = Tmp1 & 0b11111000;
			//color.green = ((Tmp1<<5) | (Tmp2>>3)) & 0b11111100;
			//color.red = Tmp2<<3;
			color = RGB_TO_UINT((Tmp2<<3) & 0xF8, ((Tmp1<<5) | (Tmp2>>3)) & 0xFC, Tmp1 & 0xF8);
			put_pixel(pDisplay, x, y, color);
		}
		CacheDataCleanBuff((unsigned int)&pDisplay->DisplayData + 8 + x1 + (pDisplay->Width * y), width * 4);
	}
	//lcd.dblbuf = dblbuf;*/
}
//#######################################################################################
void _screen_put_rgb_array_24(void *_pDisplay, unsigned char *rgb_buffer, unsigned long x1, unsigned long y1,unsigned long width, unsigned long height)
{
	/*tDisplay *pDisplay = (tDisplay *)_pDisplay;
	unsigned char *Buff = rgb_buffer;
	int   y;
	for(y =y1; y<(height+y1);y++)
	{
		if(y < pDisplay->sClipRegion.sYMin || y > pDisplay->sClipRegion.sYMax);
		else
		{
			unsigned int *DisplayStartLine = (unsigned int *)pDisplay->DisplayData + 8 + x1 + (pDisplay->Width * y);
			unsigned int *DisplayEndLine = DisplayStartLine + width;
			while(DisplayStartLine < DisplayEndLine)
			{
				*DisplayStartLine++= ((*Buff)<<24) | ((*(Buff+1))<<16) | ((*(Buff+2))<<8);
				Buff+=3;
			}
			CacheDataCleanBuff((unsigned int)&pDisplay->DisplayData + 8 + x1 + (pDisplay->Width * y * 4), width * 4);
		}
	}*/
}
//#######################################################################################
void _screen_put_rgb_array_32(void *_pDisplay, unsigned char *rgb_buffer, unsigned int x1, unsigned int y1,unsigned int width, unsigned int height)
{
	/*tDisplay *pDisplay = (tDisplay *)_pDisplay;
	int   y;
	int _y = 0;
	signed int _width = width;
	if(x1 + _width > pDisplay->sClipRegion.sXMax) _width = pDisplay->sClipRegion.sXMax - x1;
	_width *= 4;
	for(y =y1; y<(height+y1);y++)
	{
		if(y < pDisplay->sClipRegion.sYMin || y > pDisplay->sClipRegion.sYMax);
		else
		{
			unsigned int *DisplayStartLine = (unsigned int *)pDisplay->DisplayData + 8 + x1 + (pDisplay->Width * y);
			unsigned char *Buff = rgb_buffer + (width * _y * 4) - 1;
			_y++;
			memcpy((void*)(DisplayStartLine), (void*)(Buff), width * 4);
			CacheDataCleanBuff((unsigned int)((unsigned int*)(pDisplay->DisplayData + 8 + x1 + (pDisplay->Width * y))), _width);
		}
	}*/
}
//#######################################################################################
void _screen_put_horizontal_line(tDisplay *pDisplay, signed int X1, signed int X2, signed int Y, unsigned char width, unsigned int color)
{
	if(width == 1 && (Y < pDisplay->sClipRegion.sYMin || Y >= pDisplay->sClipRegion.sYMax))
		return;
	register int X1_Tmp = X1, X2_Tmp = X1 + X2;
	if(X1_Tmp <= (int)pDisplay->sClipRegion.sXMin) X1_Tmp = (int)pDisplay->sClipRegion.sXMin;
	if(X1_Tmp >= (int)pDisplay->sClipRegion.sXMax) X1_Tmp = (int)pDisplay->sClipRegion.sXMax;
	if(X2_Tmp <= (int)pDisplay->sClipRegion.sXMin) X2_Tmp = (int)pDisplay->sClipRegion.sXMin;
	if(X2_Tmp >= (int)pDisplay->sClipRegion.sXMax) X2_Tmp = (int)pDisplay->sClipRegion.sXMax;
	if(Y < (int)pDisplay->sClipRegion.sYMin) Y = (int)pDisplay->sClipRegion.sYMin;
	if(Y >= (int)pDisplay->sClipRegion.sYMax) Y = (int)pDisplay->sClipRegion.sYMax;
	int Half_width1 = (width>>1);
	int Half_width2 = width-Half_width1;
#ifdef USE_16_BIT_COLOR_DEPTH
	unsigned int _color = color;
#else
	unsigned int _color = color<<8;
#endif
	if(width == 1)
	{
		LCD_SetCursor(X1_Tmp, Y);
	    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
		for(;X1_Tmp < X2_Tmp; X1_Tmp++)
		{
			LCD_WriteRAM( _color);
		}
	}
	else
	{
		register int _Y_ = Y - Half_width1;
		for(; _Y_ < Y + Half_width2; _Y_++)
		{
			LCD_SetCursor(X1_Tmp, _Y_);
		    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
			register int _X_ = X1_Tmp;
			for(;_X_ < X2_Tmp; _X_++)
			{
				LCD_WriteRAM( _color);
			}
		}
	}
}
//#######################################################################################
void _screen_put_vertical_line(tDisplay *pDisplay, signed int Y1, signed int Y2, signed int X, unsigned char width, unsigned int color)
{
	if(width == 1 && (X < pDisplay->sClipRegion.sXMin || X >= pDisplay->sClipRegion.sXMax))
		return;
	register int Y1_Tmp = Y1, Y2_Tmp = Y1 + Y2;
	if(X <= (int)pDisplay->sClipRegion.sXMin) X = (int)pDisplay->sClipRegion.sXMin;
	if(X >= (int)pDisplay->sClipRegion.sXMax) X = (int)pDisplay->sClipRegion.sXMax;
	if(Y1_Tmp <= (int)pDisplay->sClipRegion.sYMin) Y1_Tmp = (int)pDisplay->sClipRegion.sYMin;
	if(Y1_Tmp >= (int)pDisplay->sClipRegion.sYMax) Y1_Tmp = (int)pDisplay->sClipRegion.sYMax;
	if(Y2_Tmp <= (int)pDisplay->sClipRegion.sYMin) Y2_Tmp = (int)pDisplay->sClipRegion.sYMin;
	if(Y2_Tmp >= (int)pDisplay->sClipRegion.sYMax) Y2_Tmp = (int)pDisplay->sClipRegion.sYMax;
	int Half_width1 = (width>>1);
	int Half_width2 = width-Half_width1;
#ifdef USE_16_BIT_COLOR_DEPTH
	unsigned int _color = color;
#else
	unsigned int _color = color<<8;
#endif
	if(width == 1)
	{
		for(;Y1_Tmp < Y2_Tmp; Y1_Tmp++)
		{
			LCD_SetCursor(X, Y1_Tmp);
		    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
			LCD_WriteRAM( _color);
		}
	}
	else
	{
		register int _X_ = X - Half_width1;
		for(; _X_ < X + Half_width2; _X_++)
		{
			LCD_SetCursor(_X_, Y1_Tmp);
		    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
			register int _Y_ = Y1_Tmp;
			for(;_Y_ < Y2_Tmp; _Y_++)
			{
				LCD_WriteRAM( _color);
			}
		}
	}
}
//#######################################################################################
void _screen_clear(tDisplay *pDisplay, unsigned int color)
{
	_put_rectangle(pDisplay, 0, 0, pDisplay->raster_timings->X, pDisplay->raster_timings->Y, true, color);
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
