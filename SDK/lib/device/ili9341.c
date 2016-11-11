/**
  ******************************************************************************
  * @file    ili9341.c
  * @author  MCD Application Team
  * @version V1.0.2
  * @date    02-December-2014
  * @brief   This file includes the LCD driver for ILI9341 LCD.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "ili9341.h"
#include "api/lcd_def.h"
#include "api/timer_api.h"
#include "api/mcspi_api.h"
#include "api/gpio_api.h"

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup Components
  * @{
  */ 
  
/** @addtogroup ILI9341
  * @brief This file provides a set of functions needed to drive the 
  *        ILI9341 LCD.
  * @{
  */

/** @defgroup ILI9341_Private_TypesDefinitions
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup ILI9341_Private_Defines
  * @{
  */
/**
  * @}
  */ 
  
/** @defgroup ILI9341_Private_Macros
  * @{
  */
/**
  * @}
  */  

/**
  * @}
  */ 
  
/** @defgroup ILI9341_Private_FunctionPrototypes
  * @{
  */

/**
  * @}
  */ 
  
/** @defgroup ILI9341_Private_Functions
  * @{
  */   

/**
  * @brief  Reads 4 bytes from device.
  * @param  ReadSize: Number of bytes to read (max 4 bytes)
  * @retval Value read on the SPI
  */
static unsigned long ILI9341_SPIx_Read(tDisplay* pDisplay, unsigned char ReadSize)
{
	ILI9341_t *structure = (ILI9341_t *)pDisplay->UserData;
	unsigned long readvalue = 0x000000FF;
	if(ReadSize == 2)
		readvalue = 0x0000FFFF;
	if(ReadSize == 3)
		readvalue = 0x00FFFFFF;
	if(ReadSize == 4)
		readvalue = 0xFFFFFFFF;
	structure->SPI->Buff = (unsigned char *)&readvalue;
	structure->SPI->CsSelect = structure->spi_cs_instance;
	mcspi_transfer(structure->SPI, 0, ReadSize);
	return readvalue;
}

/**
  * @brief  Writes a byte to device.
  * @param  Value: value to be written
  * @retval None
  */
static void ILI9341_SPIx_Write(tDisplay* pDisplay, unsigned short Value)
{
	ILI9341_t *structure = (ILI9341_t *)pDisplay->UserData;
	structure->SPI->Buff = (unsigned char *)&Value;
	structure->SPI->CsSelect = structure->spi_cs_instance;
	mcspi_transfer(structure->SPI, 1, 0);
}

/**
  * @brief  Sends a Byte through the SPI interface and return the Byte received
  *         from the SPI bus.
  * @param  Byte: Byte send.
  * @retval The received byte value
  */
static unsigned char ILI9341_SPIx_WriteRead(tDisplay* pDisplay, unsigned char Byte)
{
	unsigned short receivedbyte = Byte;

	ILI9341_t *structure = (ILI9341_t *)pDisplay->UserData;
	structure->SPI->Buff = (unsigned char *)&receivedbyte;
	structure->SPI->CsSelect = structure->spi_cs_instance;
	mcspi_transfer(structure->SPI, 1, 1);
	return receivedbyte >> 8;
}

/**
  * @brief  Writes register value.
  * @param  None
  * @retval None
  */
void ili9341_WriteData(tDisplay* pDisplay, unsigned short RegValue)
{
	ILI9341_t *structure = (ILI9341_t *)pDisplay->UserData;
  /* Set WRX to send data */
	gpio_out(structure->RW, 1);

  /* Reset LCD control line(/CS) and Send data */
	gpio_out(structure->CS, 0);
	ILI9341_SPIx_Write(pDisplay, RegValue);

  /* Deselect: Chip Select high */
	gpio_out(structure->CS, 1);
}

/**
  * @brief  Writes register address.
  * @param  None
  * @retval None
  */
void ili9341_WriteReg(tDisplay* pDisplay, unsigned char Reg)
{
	ILI9341_t *structure = (ILI9341_t *)pDisplay->UserData;
  /* Reset WRX to send command */
	gpio_out(structure->RW, 0);

  /* Reset LCD control line(/CS) and Send command */
	gpio_out(structure->CS, 0);
	ILI9341_SPIx_Write(pDisplay, Reg);

  /* Deselect: Chip Select high */
	gpio_out(structure->CS, 1);
}

/**
  * @brief  Reads register value.
  * @param  RegValue Address of the register to read
  * @param  ReadSize Number of bytes to read
  * @retval Content of the register value
  */
unsigned long ili9341_ReadData(tDisplay* pDisplay, unsigned short RegValue, unsigned char ReadSize)
{
	ILI9341_t *structure = (ILI9341_t *)pDisplay->UserData;
	unsigned long readvalue = 0;

  /* Select: Chip Select low */
	gpio_out(structure->CS, 0);

  /* Reset WRX to send command */
	gpio_out(structure->RW, 0);

	ILI9341_SPIx_Write(pDisplay, RegValue);

	readvalue = ILI9341_SPIx_Read(pDisplay, ReadSize);

  /* Set WRX to send data */
	gpio_out(structure->RW, 1);

  /* Deselect: Chip Select high */
	gpio_out(structure->CS, 1);

	return readvalue;
}

/**
  * @brief  Power on the LCD.
  * @param  None
  * @retval None
  */
bool ili9341_Init(void* pDisplay)
{
	tDisplay* _pDisplay = (tDisplay *) pDisplay;
	ILI9341_t *structure = (ILI9341_t *)_pDisplay->UserData;
	gpio_out(structure->CS, 0);
	gpio_out(structure->CS, 1);
	gpio_out(structure->RW, 1);
	gpio_out(structure->RD, 1);
	ili9341_WriteReg(_pDisplay, 0xCA);
	ili9341_WriteData(_pDisplay, 0xC3);
	ili9341_WriteData(_pDisplay, 0x08);
	ili9341_WriteData(_pDisplay, 0x50);
	ili9341_WriteReg(_pDisplay, LCD_POWERB);
	ili9341_WriteData(_pDisplay, 0x00);
	ili9341_WriteData(_pDisplay, 0xC1);
	ili9341_WriteData(_pDisplay, 0x30);
	ili9341_WriteReg(_pDisplay, LCD_POWER_SEQ);
	ili9341_WriteData(_pDisplay, 0x64);
	ili9341_WriteData(_pDisplay, 0x03);
	ili9341_WriteData(_pDisplay, 0x12);
	ili9341_WriteData(_pDisplay, 0x81);
	ili9341_WriteReg(_pDisplay, LCD_DTCA);
	ili9341_WriteData(_pDisplay, 0x85);
	ili9341_WriteData(_pDisplay, 0x00);
	ili9341_WriteData(_pDisplay, 0x78);
	ili9341_WriteReg(_pDisplay, LCD_POWERA);
	ili9341_WriteData(_pDisplay, 0x39);
	ili9341_WriteData(_pDisplay, 0x2C);
	ili9341_WriteData(_pDisplay, 0x00);
	ili9341_WriteData(_pDisplay, 0x34);
	ili9341_WriteData(_pDisplay, 0x02);
	ili9341_WriteReg(_pDisplay, LCD_PRC);
	ili9341_WriteData(_pDisplay, 0x20);
	ili9341_WriteReg(_pDisplay, LCD_DTCB);
	ili9341_WriteData(_pDisplay, 0x00);
	ili9341_WriteData(_pDisplay, 0x00);
	ili9341_WriteReg(_pDisplay, LCD_FRMCTR1);
	ili9341_WriteData(_pDisplay, 0x00);
	ili9341_WriteData(_pDisplay, 0x1B);
	ili9341_WriteReg(_pDisplay, LCD_DFC);
	ili9341_WriteData(_pDisplay, 0x0A);
	ili9341_WriteData(_pDisplay, 0xA2);
	ili9341_WriteReg(_pDisplay, LCD_POWER1);
	ili9341_WriteData(_pDisplay, 0x10);
	ili9341_WriteReg(_pDisplay, LCD_POWER2);
	ili9341_WriteData(_pDisplay, 0x10);
	ili9341_WriteReg(_pDisplay, LCD_VCOM1);
	ili9341_WriteData(_pDisplay, 0x45);
	ili9341_WriteData(_pDisplay, 0x15);
	ili9341_WriteReg(_pDisplay, LCD_VCOM2);
	ili9341_WriteData(_pDisplay, 0x90);
	ili9341_WriteReg(_pDisplay, LCD_MAC);
	ili9341_WriteData(_pDisplay, 0xC8);
	ili9341_WriteReg(_pDisplay, LCD_3GAMMA_EN);
	ili9341_WriteData(_pDisplay, 0x00);
	ili9341_WriteReg(_pDisplay, LCD_RGB_INTERFACE);
	ili9341_WriteData(_pDisplay, 0xC2);
	ili9341_WriteReg(_pDisplay, LCD_DFC);
	ili9341_WriteData(_pDisplay, 0x0A);
	ili9341_WriteData(_pDisplay, 0xA7);
	ili9341_WriteData(_pDisplay, 0x27);
	ili9341_WriteData(_pDisplay, 0x04);

	/* Colomn address set */
	ili9341_WriteReg(_pDisplay, LCD_COLUMN_ADDR);
	ili9341_WriteData(_pDisplay, 0x00);
	ili9341_WriteData(_pDisplay, 0x00);
	ili9341_WriteData(_pDisplay, 0x00);
	ili9341_WriteData(_pDisplay, 0xEF);
	/* Page address set */
	ili9341_WriteReg(_pDisplay, LCD_PAGE_ADDR);
	ili9341_WriteData(_pDisplay, 0x00);
	ili9341_WriteData(_pDisplay, 0x00);
	ili9341_WriteData(_pDisplay, 0x01);
	ili9341_WriteData(_pDisplay, 0x3F);
	ili9341_WriteReg(_pDisplay, LCD_INTERFACE);
	ili9341_WriteData(_pDisplay, 0x01);
	ili9341_WriteData(_pDisplay, 0x00);
	ili9341_WriteData(_pDisplay, 0x06);

	ili9341_WriteReg(_pDisplay, LCD_GRAM);
	sys_delay(200);

	ili9341_WriteReg(_pDisplay, LCD_GAMMA);
	ili9341_WriteData(_pDisplay, 0x01);

	ili9341_WriteReg(_pDisplay, LCD_PGAMMA);
	ili9341_WriteData(_pDisplay, 0x0F);
	ili9341_WriteData(_pDisplay, 0x29);
	ili9341_WriteData(_pDisplay, 0x24);
	ili9341_WriteData(_pDisplay, 0x0C);
	ili9341_WriteData(_pDisplay, 0x0E);
	ili9341_WriteData(_pDisplay, 0x09);
	ili9341_WriteData(_pDisplay, 0x4E);
	ili9341_WriteData(_pDisplay, 0x78);
	ili9341_WriteData(_pDisplay, 0x3C);
	ili9341_WriteData(_pDisplay, 0x09);
	ili9341_WriteData(_pDisplay, 0x13);
	ili9341_WriteData(_pDisplay, 0x05);
	ili9341_WriteData(_pDisplay, 0x17);
	ili9341_WriteData(_pDisplay, 0x11);
	ili9341_WriteData(_pDisplay, 0x00);
	ili9341_WriteReg(_pDisplay, LCD_NGAMMA);
	ili9341_WriteData(_pDisplay, 0x00);
	ili9341_WriteData(_pDisplay, 0x16);
	ili9341_WriteData(_pDisplay, 0x1B);
	ili9341_WriteData(_pDisplay, 0x04);
	ili9341_WriteData(_pDisplay, 0x11);
	ili9341_WriteData(_pDisplay, 0x07);
	ili9341_WriteData(_pDisplay, 0x31);
	ili9341_WriteData(_pDisplay, 0x33);
	ili9341_WriteData(_pDisplay, 0x42);
	ili9341_WriteData(_pDisplay, 0x05);
	ili9341_WriteData(_pDisplay, 0x0C);
	ili9341_WriteData(_pDisplay, 0x0A);
	ili9341_WriteData(_pDisplay, 0x28);
	ili9341_WriteData(_pDisplay, 0x2F);
	ili9341_WriteData(_pDisplay, 0x0F);

	ili9341_WriteReg(_pDisplay, LCD_SLEEP_OUT);
	sys_delay(200);
	ili9341_WriteReg(_pDisplay, LCD_DISPLAY_ON);
	/* GRAM start writing */
	ili9341_WriteReg(_pDisplay, LCD_GRAM);
	return true;
}

void ili9341_driver_register(void *pDisplay)
{
	tDisplay* _pDisplay = (tDisplay *) pDisplay;
	_pDisplay->lcd_func.init = ili9341_Init;
	_pDisplay->lcd_func.enable = ili9341_DisplayOn;
	_pDisplay->lcd_func.disable = ili9341_DisplayOff;
}
/**
  * @brief  Disables the Display.
  * @param  None
  * @retval LCD Register Value.
  */
unsigned short ili9341_ReadID(void* pDisplay)
{
	tDisplay* _pDisplay = (tDisplay *) pDisplay;
	return ((unsigned short)ili9341_ReadData(_pDisplay, LCD_READ_ID4, LCD_READ_ID4_SIZE));
}

/**
  * @brief  Enables the Display.
  * @param  None
  * @retval None
  */
void ili9341_DisplayOn(void* pDisplay)
{
  /* Display On */
	tDisplay* _pDisplay = (tDisplay *) pDisplay;
	ili9341_WriteReg(_pDisplay, LCD_DISPLAY_ON);
}

/**
  * @brief  Disables the Display.
  * @param  None
  * @retval None
  */
void ili9341_DisplayOff(void* pDisplay)
{
  /* Display Off */
	tDisplay* _pDisplay = (tDisplay *) pDisplay;
	ili9341_WriteReg(_pDisplay, LCD_DISPLAY_OFF);
}

/**
  * @}
  */ 

/**
  * @}
  */ 
  
/**
  * @}
  */ 

/**
  * @}
  */
  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
