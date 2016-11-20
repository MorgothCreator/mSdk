/*
 * mcspi_interface.c
 *
 * Created: 3/7/2013 6:41:33 PM
 *  Author: XxXx
 */

#include <api/spi_def.h>
#include <stdlib.h>
#include "stm32f4xx_conf.h"
#include "include/stm32f4xx.h"
#include "main.h"
#include "mcspi_interface.h"
#include "api/gpio_def.h"
#include "api/gpio_api.h"
#include "driver/stm32f4xx_hal_conf.h"
#include "driver/stm32f4xx_hal_rcc_ex.h"
#include "driver/stm32f4xx_hal_rcc.h"
#include "driver/stm32f4xx_hal_spi.h"
#include "driver/stm32f4xx_hal_gpio.h"
#include "driver/stm32f4xx_hal_gpio_ex.h"
#include "gpio_interface.h"
//#include "driver/spi.h"

//extern unsigned long CoreFreq;
#define SPIn                             6

#if (USE_DRIVER_SEMAPHORE == true)
volatile bool spi_semaphore[SPI_INTERFACE_COUNT];
#endif

SPI_TypeDef* _SPI_[] = {
#ifdef SPI1
		SPI1,
#endif
#ifdef SPI2
		SPI2,
#endif
#ifdef SPI3
		SPI3,
#endif
#ifdef SPI4
		SPI4,
#endif
#ifdef SPI5
		SPI5,
#endif
#ifdef SPI6
		SPI6
#endif
};


/**
  * @brief SPI MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  *           - DMA configuration for transmission request by peripheral
  *           - NVIC configuration for DMA interrupt request enable
  * @param hspi: SPI handle pointer
  * @retval None
  */
bool _mcspi_open(new_mcspi *McspiStruct)
{
	if(!McspiStruct)
			return false;
	McspiStruct->UserData = calloc(1, sizeof(SPI_HandleTypeDef));
	if(!McspiStruct->UserData)
			return false;
	SPI_HandleTypeDef *SpiHandle = (SPI_HandleTypeDef *)McspiStruct->UserData;
	//SPI_HandleTypeDef *hspi = McspiStruct.

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  _gpio_init(McspiStruct->SckPort);
  _gpio_init(McspiStruct->MisoPort);
  _gpio_init(McspiStruct->MosiPort);
  //SPIx_SCK_GPIO_CLK_ENABLE();
  //SPIx_MISO_GPIO_CLK_ENABLE();
  //SPIx_MOSI_GPIO_CLK_ENABLE();
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
  /* Enable SPI3 clock */
  switch(McspiStruct->McspiNr)
  {
  case 0:
	  __HAL_RCC_SPI1_CLK_ENABLE();
	  GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
	  break;
  case 1:
	  __HAL_RCC_SPI2_CLK_ENABLE();
	  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
	  break;
  case 2:
	  __HAL_RCC_SPI3_CLK_ENABLE();
	  GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
	  break;
#if defined(STM32F427xx) || defined(STM32F429xx) || defined(STM32F437xx) || defined(STM32F439xx)
  case 3:
	  __HAL_RCC_SPI4_CLK_ENABLE();
	  break;
#endif
#ifdef __HAL_RCC_SPI5_CLK_ENABLE
  case 4:
	  __HAL_RCC_SPI5_CLK_ENABLE();
	  break;
#endif
#ifdef __HAL_RCC_SPI6_CLK_ENABLE
  case 5:
	  __HAL_RCC_SPI6_CLK_ENABLE();
	  break;
#endif
#ifdef __HAL_RCC_SPI7_CLK_ENABLE
  case 6:
	  __HAL_RCC_SPI7_CLK_ENABLE();
	  break;
#endif
#ifdef __HAL_RCC_SPI8_CLK_ENABLE
  case 7:
	  __HAL_RCC_SPI8_CLK_ENABLE();
	  break;
#endif
  default:

	  return false;
  }

  GPIO_InitStruct.Pin       = 1 << McspiStruct->SckPin;
  HAL_GPIO_Init(GET_GPIO_PORT_ADDR[McspiStruct->SckPort], &GPIO_InitStruct);

  /* SPI MISO GPIO pin configuration  */
  GPIO_InitStruct.Pin = 1 << McspiStruct->MisoPin;
  HAL_GPIO_Init(GET_GPIO_PORT_ADDR[McspiStruct->MisoPort], &GPIO_InitStruct);

  /* SPI MOSI GPIO pin configuration  */
  GPIO_InitStruct.Pin = 1 << McspiStruct->MosiPin;
  HAL_GPIO_Init(GET_GPIO_PORT_ADDR[McspiStruct->MosiPort], &GPIO_InitStruct);

  SpiHandle->Instance               = _SPI_[McspiStruct->McspiNr];
  SpiHandle->Init.BaudRatePrescaler = (McspiStruct->ClkDiv[McspiStruct->CsSelect] << 3) & SPI_BAUDRATEPRESCALER_256;
  SpiHandle->Init.Direction         = SPI_DIRECTION_2LINES;
  SpiHandle->Init.CLKPhase          = McspiStruct->Cpha & 0x01;
  SpiHandle->Init.CLKPolarity       = (McspiStruct->Cpol & 0x01) << 1;
  SpiHandle->Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
  SpiHandle->Init.CRCPolynomial     = 7;
  SpiHandle->Init.DataSize          = SPI_DATASIZE_8BIT;
  SpiHandle->Init.FirstBit          = SPI_FIRSTBIT_MSB;
  SpiHandle->Init.NSS               = SPI_NSS_SOFT;
  SpiHandle->Init.TIMode            = SPI_TIMODE_DISABLE;
  SpiHandle->Init.Mode 				= SPI_MODE_MASTER;
  if(HAL_SPI_Init(SpiHandle) != HAL_OK)
  	  return false;
  //__HAL_SPI_ENABLE(SpiHandle);
  /*##-2- Configure peripheral GPIO ##########################################*/
  /* SPI SCK GPIO pin configuration  */


  //GPIO_InitStructure.Alternate = GPIO_Mode_OUT;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
	if(McspiStruct->CsPort[0] || McspiStruct->CsPin[0])
	{
	    GPIO_InitStruct.Pin = 1 << McspiStruct->CsPin[0];
	    HAL_GPIO_Init(GET_GPIO_PORT_ADDR[McspiStruct->CsPort[0]], &GPIO_InitStruct);
	}
	if(McspiStruct->CsPort[1] || McspiStruct->CsPin[1])
	{
	    GPIO_InitStruct.Pin = 1 << McspiStruct->CsPin[1];
	    HAL_GPIO_Init(GET_GPIO_PORT_ADDR[McspiStruct->CsPort[1]], &GPIO_InitStruct);
	}
	if(McspiStruct->CsPort[2] || McspiStruct->CsPin[2])
	{
	    GPIO_InitStruct.Pin = 1 << McspiStruct->CsPin[2];
	    HAL_GPIO_Init(GET_GPIO_PORT_ADDR[McspiStruct->CsPort[2]], &GPIO_InitStruct);
	}
	if(McspiStruct->CsPort[3] || McspiStruct->CsPin[3])
	{
	    GPIO_InitStruct.Pin = 1 << McspiStruct->CsPin[3];
	    HAL_GPIO_Init(GET_GPIO_PORT_ADDR[McspiStruct->CsPort[3]], &GPIO_InitStruct);
	}
	McspiStruct->OldCsSelect = -1;
	  return true;
}

/**
  * @brief SPI MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO, DMA and NVIC configuration to their default state
  * @param hspi: SPI handle pointer
  * @retval None
  */
void _mcspi_close(new_mcspi *McspiStruct)
{
	if(!McspiStruct)
			return;
	SPI_HandleTypeDef *SpiHandle = (SPI_HandleTypeDef *)McspiStruct->UserData;


  /*##-1- Reset peripherals ##################################################*/
  //SPIx_FORCE_RESET();
  //SPIx_RELEASE_RESET();
  switch(McspiStruct->McspiNr)
  {
  case 0:
	  __HAL_RCC_SPI1_FORCE_RESET();
	  __HAL_RCC_SPI1_RELEASE_RESET();
	  break;
  case 1:
	  __HAL_RCC_SPI2_FORCE_RESET();
	  __HAL_RCC_SPI2_RELEASE_RESET();
	  break;
  case 2:
	  __HAL_RCC_SPI3_FORCE_RESET();
	  __HAL_RCC_SPI3_RELEASE_RESET();
	  break;
#if defined(STM32F427xx) || defined(STM32F429xx) || defined(STM32F437xx) || defined(STM32F439xx)
  case 3:
	  __HAL_RCC_SPI4_FORCE_RESET();
	  __HAL_RCC_SPI4_RELEASE_RESET();
	  break;
#endif
#ifdef __HAL_RCC_SPI5_FORCE_RESET
  case 4:
	  __HAL_RCC_SPI5_FORCE_RESET();
	  __HAL_RCC_SPI5_RELEASE_RESET();
	  break;
#endif
#ifdef __HAL_RCC_SPI6_FORCE_RESET
  case 5:
	  __HAL_RCC_SPI6_FORCE_RESET();
	  __HAL_RCC_SPI6_RELEASE_RESET();
	  break;
#endif
  }

  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* Configure SPI SCK as alternate function  */
  HAL_GPIO_DeInit(GET_GPIO_PORT_ADDR[McspiStruct->SckPort], 1 << McspiStruct->SckPin);
  /* Configure SPI MISO as alternate function  */
  HAL_GPIO_DeInit(GET_GPIO_PORT_ADDR[McspiStruct->MisoPort], 1 << McspiStruct->MisoPin);
  /* Configure SPI MOSI as alternate function  */
  HAL_GPIO_DeInit(GET_GPIO_PORT_ADDR[McspiStruct->MosiPort], 1 << McspiStruct->MosiPin);

  if(McspiStruct->CsPort[0] || McspiStruct->CsPin[0])
  	{
  		HAL_GPIO_DeInit(GET_GPIO_PORT_ADDR[McspiStruct->CsPort[0]], 1 << McspiStruct->CsPin[0]);
  	}
  	if(McspiStruct->CsPort[1] || McspiStruct->CsPin[1])
  	{
  		HAL_GPIO_DeInit(GET_GPIO_PORT_ADDR[McspiStruct->CsPort[1]], 1 << McspiStruct->CsPin[1]);
  	}
  	if(McspiStruct->CsPort[2] || McspiStruct->CsPin[2])
  	{
  		HAL_GPIO_DeInit(GET_GPIO_PORT_ADDR[McspiStruct->CsPort[2]], 1 << McspiStruct->CsPin[2]);
  	}
  	if(McspiStruct->CsPort[3] || McspiStruct->CsPin[3])
  	{
  		HAL_GPIO_DeInit(GET_GPIO_PORT_ADDR[McspiStruct->CsPort[3]], 1 << McspiStruct->CsPin[3]);
  	}
  	HAL_SPI_DeInit(SpiHandle);
}
/*#####################################################*/
void _mcspi_assert(Mcspi_t *McspiStruct)
{
#if (USE_DRIVER_SEMAPHORE == true)
	while(spi_semaphore[McspiStruct->McspiNr]);
#endif
	_mcspi_set_baud(McspiStruct, McspiStruct->ClkDiv[McspiStruct->CsSelect]);
#if (USE_DRIVER_SEMAPHORE == true)
	spi_semaphore[McspiStruct->McspiNr] = true;
#endif
	HAL_GPIO_WritePin(GET_GPIO_PORT_ADDR[McspiStruct->CsPort[McspiStruct->CsSelect]], 1 << McspiStruct->CsPin[McspiStruct->CsSelect], GPIO_PIN_RESET);
}
/*#####################################################*/
void _mcspi_deassert(Mcspi_t *McspiStruct)
{
	HAL_GPIO_WritePin(GET_GPIO_PORT_ADDR[McspiStruct->CsPort[McspiStruct->CsSelect]], 1 << McspiStruct->CsPin[McspiStruct->CsSelect], GPIO_PIN_SET);
#if (USE_DRIVER_SEMAPHORE == true)
	spi_semaphore[McspiStruct->McspiNr] = false;
#endif
}
/*#####################################################*/
bool _mcspi_transfer(Mcspi_t *McspiStruct, unsigned char *buff_send, unsigned char *buff_receive, unsigned int size)
{
#if (USE_DRIVER_SEMAPHORE == true)
	if(!spi_semaphore[McspiStruct->McspiNr])
		return false;
#endif
	bool status = true;
	SPI_HandleTypeDef *hspi = (SPI_HandleTypeDef *)McspiStruct->UserData;
	if(HAL_SPI_TransmitReceive(hspi, buff_send, buff_receive, size, 10) != HAL_OK)
		status = false;
#if (USE_DRIVER_SEMAPHORE == true)
	spi_semaphore[McspiStruct->McspiNr] = false;
#endif
	return status;
}
/*#####################################################*/
bool _mcspi_receive(Mcspi_t *McspiStruct, unsigned char *buff_receive, unsigned int bytes_receive)
{
#if (USE_DRIVER_SEMAPHORE == true)
	if(!spi_semaphore[McspiStruct->McspiNr])
		return false;
#endif
	bool status = true;
	SPI_HandleTypeDef *hspi = (SPI_HandleTypeDef *)McspiStruct->UserData;
	if(HAL_SPI_Receive(hspi, buff_receive, bytes_receive, 10) != HAL_OK)
		status = false;
#if (USE_DRIVER_SEMAPHORE == true)
	spi_semaphore[McspiStruct->McspiNr] = false;
#endif
	return status;
}
/*#####################################################*/
bool _mcspi_transmit(Mcspi_t *McspiStruct, unsigned char *buff_send, unsigned int bytes_send)
{
#if (USE_DRIVER_SEMAPHORE == true)
	if(!spi_semaphore[McspiStruct->McspiNr])
		return false;
#endif
	bool status = true;
	SPI_HandleTypeDef *hspi = (SPI_HandleTypeDef *)McspiStruct->UserData;
	if(HAL_SPI_Transmit(hspi, buff_send, bytes_send, 10) != HAL_OK)
		status = false;

	if(!McspiStruct->DisableCsHandle)
		HAL_GPIO_WritePin(GET_GPIO_PORT_ADDR[McspiStruct->CsPort[McspiStruct->CsSelect]], 1 << McspiStruct->CsPin[McspiStruct->CsSelect], GPIO_PIN_SET);
#if (USE_DRIVER_SEMAPHORE == true)
	spi_semaphore[McspiStruct->McspiNr] = false;
#endif
	return status;
}
/*#####################################################*/
bool _mcspi_set_baud(Mcspi_t *McspiStruct, unsigned long baud)
{
#if (USE_DRIVER_SEMAPHORE == true)
	while(spi_semaphore[McspiStruct->McspiNr]);
#endif
	SPI_HandleTypeDef *hspi = (SPI_HandleTypeDef *)McspiStruct->UserData;
	if((hspi->Instance->CR1 & SPI_BAUDRATEPRESCALER_256) != (SPI_BAUDRATEPRESCALER_256 & (baud << 3)))
	{
		hspi->Instance->CR1 &= ~SPI_BAUDRATEPRESCALER_256;
		hspi->Instance->CR1 |= SPI_BAUDRATEPRESCALER_256 & (baud << 3);
	}
	return false;
}
/*#####################################################*/
unsigned char _mcspi_SendByte(Mcspi_t *McspiStruct, unsigned char byte)
{
	SPI_HandleTypeDef *hspi = (SPI_HandleTypeDef *)McspiStruct->UserData;
	unsigned char tmp = byte;
	HAL_SPI_TransmitReceive(hspi, &byte, &tmp, 1, 10);
	return tmp;
}
/*#####################################################*/

