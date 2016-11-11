/**
  ******************************************************************************
  * @file    USB_Device/CDC_Standalone/Src/usbd_cdc_interface.c
  * @author  MCD Application Team
  * @version V1.4.1
  * @date    09-October-2015
  * @brief   Source file for USBD CDC interface
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
#include "main.h"
#include "usbd_cdc_interface.h"
#include "driver/stm32f4xx_hal_tim.h"
#include "Inc/usbd_cdc.h"
#include "lib/buffers/ring_buff.h"

/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
USBD_CDC_LineCodingTypeDef LineCoding = {
    115200, /* baud rate */
    0x00,   /* stop bits-1 */
    0x00,   /* parity - none */
    0x08    /* nb. of bits 8 */
  };

uint8_t UserRxBuffer[APP_RX_DATA_SIZE];/* Received Data over USB are stored in this buffer */
uint8_t UserTxBuffer[APP_TX_DATA_SIZE];/* Received Data over UART (CDC interface) are stored in this buffer */
uint8_t UserTxBufferFiFo[APP_TX_DATA_SIZE];/* Received Data over UART (CDC interface) are stored in this buffer */


uint32_t UserTxBufPtrIn = 0;/* Increment this pointer or roll it back to
                               start address when data are received over USART */
uint32_t UserTxBufPtrOut = 0; /* Increment this pointer or roll it back to
                                 start address when data are sent over USB */
unsigned int UserRxBuffCnt = 0;
//extern int put_receive_char_ptr;

extern fifo_settings_t *usb_cdc_dev_rx_fifo;
extern fifo_settings_t *usb_cdc_dev_tx_fifo;

/* UART handler declaration */
UART_HandleTypeDef UartHandle;
/* TIM handler declaration */
TIM_HandleTypeDef  USBCDCTimHandle;
/* USB handler declaration */
extern USBD_HandleTypeDef  USBD_Device;

/* Private function prototypes -----------------------------------------------*/
static int8_t CDC_Itf_Init(void);
static int8_t CDC_Itf_DeInit(void);
static int8_t CDC_Itf_Control(uint8_t cmd, uint8_t* pbuf, uint16_t length);
static int8_t CDC_Itf_Receive(uint8_t* pbuf, uint32_t *Len);
static void Error_Handler(void);
static void ComPort_Config(void);
static void TIM_Config(void);

USBD_CDC_ItfTypeDef USBD_CDC_fops = 
{
  CDC_Itf_Init,
  CDC_Itf_DeInit,
  CDC_Itf_Control,
  CDC_Itf_Receive
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes the CDC media low layer      
  * @param  None
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Itf_Init(void)
{
  /*##-6- Enable TIM peripherals Clock #######################################*/
  USBCDCTIMx_CLK_ENABLE();

  /*##-7- Configure the NVIC for TIMx ########################################*/
  /* Set Interrupt Group Priority */
  HAL_NVIC_SetPriority(USBCDCTIMx_IRQn, 6, 0);

  /* Enable the TIMx global Interrupt */
  HAL_NVIC_EnableIRQ(USBCDCTIMx_IRQn);
  /*##-3- Configure the TIM Base generation  #################################*/
  TIM_Config();
  
  /*##-4- Start the TIM Base generation in interrupt mode ####################*/
  /* Start Channel1 */
  if(HAL_TIM_Base_Start_IT(&USBCDCTimHandle) != HAL_OK)
  {
    /* Starting Error */
    Error_Handler();
  }
  
  /*##-5- Set Application Buffers ############################################*/
  USBD_CDC_SetTxBuffer(&USBD_Device, UserTxBuffer, 0);
  USBD_CDC_SetRxBuffer(&USBD_Device, UserRxBuffer);
  
  return (USBD_OK);
}

/**
  * @brief  CDC_Itf_DeInit
  *         DeInitializes the CDC media low layer
  * @param  None
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Itf_DeInit(void)
{
  /* DeInitialize the UART peripheral */
  if(HAL_UART_DeInit(&UartHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  return (USBD_OK);
}

/**
  * @brief  CDC_Itf_Control
  *         Manage the CDC class requests
  * @param  Cmd: Command code            
  * @param  Buf: Buffer containing command data (request parameters)
  * @param  Len: Number of data to be sent (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Itf_Control (uint8_t cmd, uint8_t* pbuf, uint16_t length)
{ 
  switch (cmd)
  {
  case CDC_SEND_ENCAPSULATED_COMMAND:
    /* Add your code here */
    break;

  case CDC_GET_ENCAPSULATED_RESPONSE:
    /* Add your code here */
    break;

  case CDC_SET_COMM_FEATURE:
    /* Add your code here */
    break;

  case CDC_GET_COMM_FEATURE:
    /* Add your code here */
    break;

  case CDC_CLEAR_COMM_FEATURE:
    /* Add your code here */
    break;

  case CDC_SET_LINE_CODING:
    LineCoding.bitrate    = (uint32_t)(pbuf[0] | (pbuf[1] << 8) |\
                            (pbuf[2] << 16) | (pbuf[3] << 24));
    LineCoding.format     = pbuf[4];
    LineCoding.paritytype = pbuf[5];
    LineCoding.datatype   = pbuf[6];
    
    /* Set the new configuration */
    ComPort_Config();
    break;

  case CDC_GET_LINE_CODING:
    pbuf[0] = (uint8_t)(LineCoding.bitrate);
    pbuf[1] = (uint8_t)(LineCoding.bitrate >> 8);
    pbuf[2] = (uint8_t)(LineCoding.bitrate >> 16);
    pbuf[3] = (uint8_t)(LineCoding.bitrate >> 24);
    pbuf[4] = LineCoding.format;
    pbuf[5] = LineCoding.paritytype;
    pbuf[6] = LineCoding.datatype;     
    break;

  case CDC_SET_CONTROL_LINE_STATE:
    /* Add your code here */
    break;

  case CDC_SEND_BREAK:
     /* Add your code here */
    break;    
    
  default:
    break;
  }
  
  return (USBD_OK);
}

/**
  * @brief  TIM period elapsed callback
  * @param  htim: TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	//uint32_t buffptr;
	//uint32_t buffsize = 0;
  
	/*if(UserTxBufPtrOut != UserTxBufPtrIn)
  	  {
    	if(UserTxBufPtrOut > UserTxBufPtrIn) // Rollback
    	{
      	  buffsize = APP_RX_DATA_SIZE - UserTxBufPtrOut;
    	}
    	else
    	{
      	  buffsize = UserTxBufPtrIn - UserTxBufPtrOut;
    	}
    
    buffptr = UserTxBufPtrOut;*/
  unsigned int cnt = 0;
  for(; cnt < APP_RX_DATA_SIZE; cnt++)
    {
		fifo_pop_return_t data_return = fifo_pop(usb_cdc_dev_tx_fifo);
		if(data_return.status == false)
			break;
		UserTxBuffer[cnt] = data_return.character;
    }
  if(cnt == 0)
	  return;
    USBD_CDC_SetTxBuffer(&USBD_Device, UserTxBuffer, cnt);
    
    if(USBD_CDC_TransmitPacket(&USBD_Device) == USBD_OK)
    {
      /*UserTxBufPtrOut += buffsize;
      if (UserTxBufPtrOut == APP_RX_DATA_SIZE)
      {
        UserTxBufPtrOut = 0;
      }*/
    }
  //}
}

/**
  * @brief  CDC_Itf_DataRx
  *         Data received over USB OUT endpoint are sent over CDC interface 
  *         through this function.
  * @param  Buf: Buffer of data to be transmitted
  * @param  Len: Number of data received (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Itf_Receive(uint8_t* Buf, uint32_t *Len)
{
  //HAL_UART_Transmit_DMA(&UartHandle, Buf, *Len);
	//UserRxBuffer[UserRxBufPtrIn] =;
	/*if(*Len + UserRxBuffCnt >= APP_RX_DATA_SIZE)
	{
		memcpy(UserTxBufferFiFo + UserRxBuffCnt, Buf, APP_RX_DATA_SIZE - UserRxBuffCnt);
		memcpy(UserTxBufferFiFo, Buf + (APP_RX_DATA_SIZE - UserRxBuffCnt), (*Len - (APP_RX_DATA_SIZE - UserRxBuffCnt)));
		UserRxBuffCnt = *Len - (APP_RX_DATA_SIZE - UserRxBuffCnt);
	}
	else
	{
		memcpy(UserTxBufferFiFo + UserRxBuffCnt, Buf, *Len);
		UserRxBuffCnt += *Len;
	}*/
	unsigned int cnt = 0;
	for(; cnt < *Len; cnt++)
		fifo_push(usb_cdc_dev_rx_fifo, Buf[cnt]);
	USBD_CDC_ReceivePacket(&USBD_Device);
	return (USBD_OK);
}

/**
  * @brief  ComPort_Config
  *         Configure the COM Port with the parameters received from host.
  * @param  None.
  * @retval None
  * @note   When a configuration is not supported, a default value is used.
  */
static void ComPort_Config(void)
{
#if 0
  if(HAL_UART_DeInit(&UartHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /* set the Stop bit */
  switch (LineCoding.format)
  {
  case 0:
    UartHandle.Init.StopBits = UART_STOPBITS_1;
    break;
  case 2:
    UartHandle.Init.StopBits = UART_STOPBITS_2;
    break;
  default :
    UartHandle.Init.StopBits = UART_STOPBITS_1;
    break;
  }
  
  /* set the parity bit*/
  switch (LineCoding.paritytype)
  {
  case 0:
    UartHandle.Init.Parity = UART_PARITY_NONE;
    break;
  case 1:
    UartHandle.Init.Parity = UART_PARITY_ODD;
    break;
  case 2:
    UartHandle.Init.Parity = UART_PARITY_EVEN;
    break;
  default :
    UartHandle.Init.Parity = UART_PARITY_NONE;
    break;
  }
  
  /*set the data type : only 8bits and 9bits is supported */
  switch (LineCoding.datatype)
  {
  case 0x07:
    /* With this configuration a parity (Even or Odd) must be set */
    UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
    break;
  case 0x08:
    if(UartHandle.Init.Parity == UART_PARITY_NONE)
    {
      UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
    }
    else 
    {
      UartHandle.Init.WordLength = UART_WORDLENGTH_9B;
    }
    
    break;
  default :
    UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
    break;
  }
  
  UartHandle.Init.BaudRate     = LineCoding.bitrate;
  UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode         = UART_MODE_TX_RX;
  UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
  
  if(HAL_UART_Init(&UartHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Start reception: provide the buffer pointer with offset and the buffer size */
  HAL_UART_Receive_IT(&UartHandle, (uint8_t *)(UserTxBuffer + UserTxBufPtrIn), 1);
#endif
}

/**
  * @brief  TIM_Config: Configure TIMx timer
  * @param  None.
  * @retval None
  */
static void TIM_Config(void)
{  
  /* Set TIMx instance */
	USBCDCTimHandle.Instance = USBCDCTIMx;
  
  /* Initialize TIM3 peripheral as follows:
       + Period = (CDC_POLLING_INTERVAL * 10000) - 1
       + Prescaler = ((APB1 frequency / 1000000) - 1)
       + ClockDivision = 0
       + Counter direction = Up  */
	USBCDCTimHandle.Init.Period = (CDC_POLLING_INTERVAL*1000) - 1;
	USBCDCTimHandle.Init.Prescaler = 84-1;
	USBCDCTimHandle.Init.ClockDivision = 0;
	USBCDCTimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
  if(HAL_TIM_Base_Init(&USBCDCTimHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}

/**
  * @brief  UART error callbacks
  * @param  UartHandle: UART handle
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
  /* Transfer error occured in reception and/or transmission process */
  Error_Handler();
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Add your own code here */
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
