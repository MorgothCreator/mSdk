/*
 * uart_interface.c
 *
 * Created: 2/8/2013 2:06:44 AM
 *  Author: XxXx
 */
/*#####################################################*/
#include "stm32f4xx_conf.h"
#include "uart_interface.h"
#include "driver/stm32f4xx_usart.h"
//#include "driver/uart.h"
//#include "int/int_uart.h"
/**
  * @brief  Configures the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void UART_nvic_config(Uart_t* UartSettings)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  switch(UartSettings->UartNr)
  {
	  case 0:
		  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		  break;
  	  case 1:
  		  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  		  break;
  	  case 2:
  		  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  		  break;
  	  case 3:
  		  NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
  		  break;
  	  case 4:
  		  NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
  		  break;
  }
  /* Enable the USARTx Interrupt */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = UartSettings->Priority;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
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

#define COMn                             6

USART_TypeDef* COM_USART[COMn] = { USART1, USART2, USART3, UART4, UART5, USART6};

GPIO_TypeDef* COM_TX_PORT[COMn] = {GPIOB,  GPIOD,  GPIOD,  GPIOC, GPIOD, GPIOC};

GPIO_TypeDef* COM_RX_PORT[COMn] = {GPIOB,  GPIOD,  GPIOD,  GPIOC, GPIOC, GPIOC};

const uint32_t COM_USART_CLK[COMn] = {RCC_APB2Periph_USART1, RCC_APB1Periph_USART2, RCC_APB1Periph_USART3, RCC_APB1Periph_UART4, RCC_APB1Periph_UART5, RCC_APB2Periph_USART6};

const uint32_t COM_TX_PORT_CLK[COMn] = {RCC_AHB1Periph_GPIOB, RCC_AHB1Periph_GPIOD, RCC_AHB1Periph_GPIOD, RCC_AHB1Periph_GPIOC, RCC_AHB1Periph_GPIOD, RCC_AHB1Periph_GPIOC};

const uint32_t COM_RX_PORT_CLK[COMn] = {RCC_AHB1Periph_GPIOB, RCC_AHB1Periph_GPIOD, RCC_AHB1Periph_GPIOD, RCC_AHB1Periph_GPIOC, RCC_AHB1Periph_GPIOC, RCC_AHB1Periph_GPIOC};

const uint16_t COM_TX_PIN[COMn] = {GPIO_Pin_6, GPIO_Pin_5, GPIO_Pin_8, GPIO_Pin_10, GPIO_Pin_2, GPIO_Pin_6};

const uint16_t COM_RX_PIN[COMn] = {GPIO_Pin_7, GPIO_Pin_6, GPIO_Pin_9, GPIO_Pin_11, GPIO_Pin_12, GPIO_Pin_7};

const uint16_t COM_TX_PIN_SOURCE[COMn] = {GPIO_PinSource6, GPIO_PinSource5, GPIO_PinSource8, GPIO_PinSource10, GPIO_PinSource2, GPIO_PinSource6};

const uint16_t COM_RX_PIN_SOURCE[COMn] = {GPIO_PinSource7, GPIO_PinSource6, GPIO_PinSource9, GPIO_PinSource11, GPIO_PinSource12, GPIO_PinSource7};

const uint16_t COM_TX_AF[COMn] = {GPIO_AF_USART1, GPIO_AF_USART2, GPIO_AF_USART3, GPIO_AF_UART4, GPIO_AF_UART5, GPIO_AF_USART6};

const uint16_t COM_RX_AF[COMn] = {GPIO_AF_USART1, GPIO_AF_USART2, GPIO_AF_USART3, GPIO_AF_UART4, GPIO_AF_UART5, GPIO_AF_USART6};

void STM_EVAL_COMInit(unsigned char COM, USART_InitTypeDef* USART_InitStruct)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIO clock */
  RCC_AHB1PeriphClockCmd(COM_TX_PORT_CLK[COM], ENABLE);
  RCC_AHB1PeriphClockCmd(COM_RX_PORT_CLK[COM], ENABLE);

  if (COM == 0 || COM == 5) RCC_APB2PeriphClockCmd(COM_USART_CLK[COM], ENABLE);
  else
  {
    /* Enable UART clock */
    RCC_APB1PeriphClockCmd(COM_USART_CLK[COM], ENABLE);
  }

  /* Connect PXx to USARTx_Tx*/
  GPIO_PinAFConfig(COM_TX_PORT[COM], COM_TX_PIN_SOURCE[COM], COM_TX_AF[COM]);

  /* Connect PXx to USARTx_Rx*/
  GPIO_PinAFConfig(COM_RX_PORT[COM], COM_RX_PIN_SOURCE[COM], COM_RX_AF[COM]);

  /* Configure USART Tx as alternate function  */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

  GPIO_InitStructure.GPIO_Pin = COM_TX_PIN[COM];
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(COM_TX_PORT[COM], &GPIO_InitStructure);

  /* Configure USART Rx as alternate function  */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = COM_RX_PIN[COM];
  GPIO_Init(COM_RX_PORT[COM], &GPIO_InitStructure);

  /* USART configuration */
  USART_Init(COM_USART[COM], USART_InitStruct);

  /* Enable USART */
  USART_Cmd(COM_USART[COM], ENABLE);
}
/*#####################################################*/
bool _uart_open(Uart_t* UartSettings)
{
	//USART_DeInit(COM_USART[UartSettings->UartNr]);
	//UART_nvic_config(UartSettings);
	UartSettings->BaseAddr = (unsigned int)COM_USART[UartSettings->UartNr];
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = UartSettings->BaudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	//UART_nvic_config(UartSettings);
	STM_EVAL_COMInit(UartSettings->UartNr, &USART_InitStructure);
	  //while(1);
	//USART_TypeDef* base_addr = (USART_TypeDef*)UartSettings->BaseAddr;
	//base_addr->CR1 |= USART_Mode_Rx | USART_Mode_Tx;
	return true;//UartOpen(UartSettings);
}
/*#####################################################*/
bool _uart_close(Uart_t *UartSettings)
{
	USART_DeInit((USART_TypeDef*)UartSettings->BaseAddr);
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
	  USART_SendData((USART_TypeDef*)BaseAddr, byteTx);
}
/*#####################################################*/
unsigned char _UARTCharGet(unsigned int BaseAddr)
{
	return (signed char)USART_ReceiveData((USART_TypeDef*)BaseAddr);//UARTCharGet(BaseAddr);
}
/*#####################################################*/
bool _UARTCharPutNonBlocking(unsigned int BaseAddr, unsigned char byteTx)
{
	return UARTCharPutNonBlocking((USART_TypeDef*)BaseAddr, byteTx);
}
/*#####################################################*/
signed short _UARTCharGetNonBlocking(unsigned int BaseAddr)
{
	return UARTCharGetNonBlocking((USART_TypeDef*)BaseAddr);
}
/*#####################################################*/
unsigned int _UARTRxErrorGet(unsigned int BaseAddr)
{
	return 0;//UARTRxErrorGet(BaseAddr);
}
/*#####################################################*/
