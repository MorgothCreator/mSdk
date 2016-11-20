/*
 * uart_interface.c
 *
 * Created: 2/8/2013 2:06:44 AM
 *  Author: XxXx
 */
/*#####################################################*/
#include <stdlib.h>
#include "stm32f4xx_conf.h"
#include "main.h"
#include "uart_interface_def.h"
#include "uart_interface.h"
#include "include/stm32f4xx.h"
#include "driver/stm32f4xx_hal_conf.h"
#include "driver/stm32f4xx_hal_uart.h"
#include "driver/stm32f4xx_hal_rcc.h"
#include "driver/misc.h"
#include "gpio_interface.h"
//#include "driver/uart.h"
//#include "int/int_uart.h"
/**
  * @brief  Configures COM port.
  * @param  COM: Specifies the COM port to be configured.
  *   This parameter can be one of following parameters:
  *     @arg COM1
  *     @arg COM2
  * @param  USART_InitStruct: pointer to a USART_InitTypeDef structure that
  *   contains the configuration information for the specified USART peripheral.
  * @retval None
  */

#define COMn                             8

#if (USE_DRIVER_SEMAPHORE == true)
volatile bool uart_semaphore[UART_INTERFACE_COUNT];
#endif

USART_TypeDef* COM_USART[COMn] = {
#ifdef USART1
		USART1,
#endif
#ifdef USART2
		USART2,
#endif
#ifdef USART3
		USART3,
#endif
#ifdef UART4
		UART4,
#endif
#ifdef UART5
		UART5,
#endif
#ifdef USART6
		USART6,
#endif
#ifdef UART7
		UART7,
#endif
#ifdef UART8
		UART8
#endif
};

/*#####################################################*/
bool _uart_open(Uart_t* UartSettings)
{
	  GPIO_InitTypeDef  GPIO_InitStruct;

	  /*##-1- Enable peripherals and GPIO Clocks #################################*/
	  /* Enable GPIO TX/RX clock */
	  _gpio_init(UartSettings->TxPort);
	  _gpio_init(UartSettings->RxPort);

	  //USARTx_TX_GPIO_CLK_ENABLE();
	  //USARTx_RX_GPIO_CLK_ENABLE();
	  /* Enable USART1 clock */
	  switch(UartSettings->UartNr)
	  {
#ifdef __HAL_RCC_USART1_CLK_ENABLE
	  case 0:
		  __HAL_RCC_USART1_CLK_ENABLE();
		  break;
#endif
#ifdef __HAL_RCC_USART2_CLK_ENABLE
	  case 1:
		  __HAL_RCC_USART2_CLK_ENABLE();
		  break;
#endif
#ifdef __HAL_RCC_USART3_CLK_ENABLE
	  case 2:
		  __HAL_RCC_USART3_CLK_ENABLE();
		  break;
#endif
#ifdef __HAL_RCC_UART4_CLK_ENABLE
	  case 3:
		  __HAL_RCC_UART4_CLK_ENABLE();
		  break;
#endif
#ifdef __HAL_RCC_UART5_CLK_ENABLE
	  case 4:
		  __HAL_RCC_UART5_CLK_ENABLE();
		  break;
#endif
#ifdef __HAL_RCC_USART6_CLK_ENABLE
	  case 5:
		  __HAL_RCC_USART6_CLK_ENABLE();
		  break;
#endif
#ifdef __HAL_RCC_UART7_CLK_ENABLE
	  case 6:
		  __HAL_RCC_UART7_CLK_ENABLE();
		  break;
#endif
#ifdef __HAL_RCC_UART8_CLK_ENABLE
	  case 7:
		  __HAL_RCC_UART8_CLK_ENABLE();
		  break;
#endif
	  }
	  //USARTx_CLK_ENABLE();

	  /*##-2- Configure peripheral GPIO ##########################################*/
	  /* UART TX GPIO pin configuration  */
	  GPIO_InitStruct.Pin       = 1 << UartSettings->TxPin;
	  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	  GPIO_InitStruct.Pull      = GPIO_NOPULL;
	  GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
	  switch(UartSettings->UartNr)
	  {
	  case 0:
	  case 1:
	  case 2:
		  GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
		  break;
	  case 3:
	  case 4:
	  case 5:
	  case 6:
	  case 7:
		  GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
		  break;
	  default:
		  return false;
	  }
	  //GPIO_InitStruct.Alternate = USARTx_TX_AF;

	  HAL_GPIO_Init(GET_GPIO_PORT_ADDR[UartSettings->TxPort], &GPIO_InitStruct);

	  /* UART RX GPIO pin configuration  */
	  GPIO_InitStruct.Pin = 1 << UartSettings->RxPin;
	  //GPIO_InitStruct.Alternate = USARTx_RX_AF;

	  HAL_GPIO_Init(GET_GPIO_PORT_ADDR[UartSettings->RxPort], &GPIO_InitStruct);

	  UartSettings->udata = calloc(1, sizeof(UART_HandleTypeDef));
	  if(!UartSettings->udata)
		  return false;
	  UART_HandleTypeDef *UartHandle = (UART_HandleTypeDef *)UartSettings->udata;
	//USART_DeInit(COM_USART[UartSettings->UartNr]);
	//UART_nvic_config(UartSettings);
	UartSettings->BaseAddr = (unsigned int)UartSettings;
	UartHandle->Instance = COM_USART[UartSettings->UartNr];
	UartHandle->Init.BaudRate = UartSettings->BaudRate;
	UartHandle->Init.WordLength = UART_WORDLENGTH_8B;
	UartHandle->Init.StopBits = UART_STOPBITS_1;
	UartHandle->Init.Parity = UART_PARITY_NONE;
	UartHandle->Init.HwFlowCtl = UART_HWCONTROL_NONE;
	UartHandle->Init.Mode = UART_MODE_TX_RX;
	UartHandle->Init.OverSampling = UART_OVERSAMPLING_16;

	HAL_UART_Init(UartHandle);
	return true;//UartOpen(UartSettings);
}
/*#####################################################*/
bool _uart_close(Uart_t *UartSettings)
{
	HAL_UART_DeInit((UART_HandleTypeDef*)UartSettings->udata);
	if(UartSettings->udata)
		free(UartSettings->udata);
	return true;
}
/*#####################################################*/
void _UARTBaudSetRate(unsigned int BaseAddr, unsigned long BaudRate)
{
	//uart_set_baud_rate(BaseAddr, BaudRate);
}
/*#####################################################*/
void _UARTCharPut(unsigned int BaseAddr, unsigned char byteTx)
{
	Uart_t* UartSettings = (Uart_t *)BaseAddr;
#if (USE_DRIVER_SEMAPHORE == true)
	while(uart_semaphore[UartSettings->UartNr]);
	uart_semaphore[UartSettings->UartNr] = true;
#endif
	HAL_UART_Transmit(UartSettings->udata, &byteTx, 1, 10);
#if (USE_DRIVER_SEMAPHORE == true)
	uart_semaphore[UartSettings->UartNr] = false;
#endif
}
/*#####################################################*/
unsigned char _UARTCharGet(unsigned int BaseAddr)
{
	unsigned char data = 0;
	Uart_t* UartSettings = (Uart_t *)BaseAddr;
#if (USE_DRIVER_SEMAPHORE == true)
	while(uart_semaphore[UartSettings->UartNr]);
	uart_semaphore[UartSettings->UartNr] = true;
#endif
	HAL_UART_Receive(UartSettings->udata, &data, 1, 10);
#if (USE_DRIVER_SEMAPHORE == true)
	uart_semaphore[UartSettings->UartNr] = false;
#endif
	return data;
}
/*#####################################################*/
bool _UARTCharPutNonBlocking(unsigned int BaseAddr, unsigned char byteTx)
{
	Uart_t* UartSettings = (Uart_t *)BaseAddr;
#if (USE_DRIVER_SEMAPHORE == true)
	while(uart_semaphore[UartSettings->UartNr]);
	uart_semaphore[UartSettings->UartNr] = true;
#endif
	HAL_StatusTypeDef status = HAL_UART_Transmit(UartSettings->udata, &byteTx, 1, 2);
	if(status == HAL_TIMEOUT || status == HAL_BUSY || status == HAL_ERROR)
	{
#if (USE_DRIVER_SEMAPHORE == true)
		uart_semaphore[UartSettings->UartNr] = false;
#endif
		return false;
	}
#if (USE_DRIVER_SEMAPHORE == true)
	uart_semaphore[UartSettings->UartNr] = false;
#endif
	return true;
}
/*#####################################################*/
signed short _UARTCharGetNonBlocking(unsigned int BaseAddr)
{
	signed short data = 0;
	Uart_t* UartSettings = (Uart_t *)BaseAddr;
#if (USE_DRIVER_SEMAPHORE == true)
	while(uart_semaphore[UartSettings->UartNr]);
	uart_semaphore[UartSettings->UartNr] = true;
#endif
	HAL_StatusTypeDef status = HAL_UART_Receive((UART_HandleTypeDef *)UartSettings->udata, (unsigned char *)&data, 1, 2);
	if(status == HAL_TIMEOUT || status == HAL_BUSY || status == HAL_ERROR)
	{
#if (USE_DRIVER_SEMAPHORE == true)
		uart_semaphore[UartSettings->UartNr] = false;
#endif
		return -1;//UARTCharPutNonBlocking((USART_TypeDef*)BaseAddr, byteTx);
	}
#if (USE_DRIVER_SEMAPHORE == true)
	uart_semaphore[UartSettings->UartNr] = false;
#endif
	return data;
}
/*#####################################################*/
unsigned int _UARTRxErrorGet(unsigned int BaseAddr)
{
	return 0;//UARTRxErrorGet(BaseAddr);
}
/*#####################################################*/
