/*
 * twi_interface.c
 *
 * Created: 2/8/2013 5:00:27 PM
 *  Author: XxXx
 */
/*#####################################################*/
#include "stdlib.h"
#include "stm32f10x_conf.h"
#include "include/stm32f10x.h"
#include "twi_interface.h"
#include "driver/stm32f10x_i2c.h"
//#####################################################
I2C_TypeDef *sEE_I2C[2] = {I2C1, I2C2};
#define I2Cn                             2

I2C_TypeDef* I2C[I2Cn] = { I2C1, I2C2};

GPIO_TypeDef* I2C_SCL_PORT[I2Cn] = {GPIOB,  GPIOA};

GPIO_TypeDef* I2C_SDA_PORT[I2Cn] = {GPIOB,  GPIOA};

const uint32_t I2C_CLK[I2Cn] = {RCC_APB1Periph_I2C1, RCC_APB1Periph_I2C2};

const uint32_t I2C_SCL_PORT_CLK[I2Cn] = {RCC_APB2Periph_GPIOB, RCC_APB2Periph_GPIOA};

const uint32_t I2C_SDA_PORT_CLK[I2Cn] = {RCC_APB2Periph_GPIOB, RCC_APB2Periph_GPIOA};

const uint16_t I2C_SCL_PIN[I2Cn] = {GPIO_Pin_6, GPIO_Pin_2};

const uint16_t I2C_SDA_PIN[I2Cn] = {GPIO_Pin_7, GPIO_Pin_3};

#define sEE_I2C_DMA                      DMA1
DMA_Channel_TypeDef *sEE_I2C_DMA_CHANNEL_TX[I2Cn] =          {DMA1_Channel6, DMA1_Channel4};
DMA_Channel_TypeDef *sEE_I2C_DMA_CHANNEL_RX[I2Cn] =          {DMA1_Channel7, DMA1_Channel5};
#define sEE_I2C_DMA_FLAG_TX_TC           DMA1_IT_TC6
#define sEE_I2C_DMA_FLAG_TX_GL           DMA1_IT_GL6
#define sEE_I2C_DMA_FLAG_RX_TC           DMA1_IT_TC7
#define sEE_I2C_DMA_FLAG_RX_GL           DMA1_IT_GL7
uint32_t sEE_I2C_DMA_CLK[I2Cn] =                  {RCC_AHBPeriph_DMA1, RCC_AHBPeriph_DMA1};
uint32_t sEE_I2C_DR_Address[I2Cn] =                {((uint32_t)0x40005410) , ((uint32_t)0x40005410)};
#define sEE_USE_DMA

uint8_t sEE_I2C_DMA_TX_IRQn[I2Cn] =               {DMA1_Channel6_IRQn, DMA1_Channel4_IRQn};
uint8_t sEE_I2C_DMA_RX_IRQn[I2Cn] =               {DMA1_Channel7_IRQn, DMA1_Channel5_IRQn};
#define sEE_I2C1_DMA_TX_IRQHandler        DMA1_Channel6_IRQHandler
#define sEE_I2C1_DMA_RX_IRQHandler        DMA1_Channel7_IRQHandler
#define sEE_I2C_DMA_PREPRIO              0
#define sEE_I2C_DMA_SUBPRIO              0

DMA_InitTypeDef   sEEDMA_InitStructure[I2Cn];
/**
  * @brief  Start critical section: these callbacks should be typically used
  *         to disable interrupts when entering a critical section of I2C communication
  *         You may use default callbacks provided into this driver by uncommenting the
  *         define USE_DEFAULT_CRITICAL_CALLBACK.
  *         Or you can comment that line and implement these callbacks into your
  *         application.
  * @param  None.
  * @retval None.
  */
void sEE_EnterCriticalSection_UserCallback(void)
{
  __disable_irq();
}

/**
  * @brief  Start and End of critical section: these callbacks should be typically used
  *         to re-enable interrupts when exiting a critical section of I2C communication
  *         You may use default callbacks provided into this driver by uncommenting the
  *         define USE_DEFAULT_CRITICAL_CALLBACK.
  *         Or you can comment that line and implement these callbacks into your
  *         application.
  * @param  None.
  * @retval None.
  */
void sEE_ExitCriticalSection_UserCallback(void)
{
  __enable_irq();
}
/**
  * @brief  DeInitializes peripherals used by the I2C EEPROM driver.
  * @param  None
  * @retval None
  */
void sEE_LowLevel_DeInit(new_twi* TwiStruct)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /* sEE_I2C Peripheral Disable */
  I2C_Cmd(sEE_I2C[TwiStruct->TwiNr], DISABLE);

  /* sEE_I2C DeInit */
  I2C_DeInit(sEE_I2C[TwiStruct->TwiNr]);

  /*!< sEE_I2C Periph clock disable */
  RCC_APB1PeriphClockCmd(I2C_CLK[TwiStruct->TwiNr], DISABLE);

  /*!< GPIO configuration */
  /*!< Configure sEE_I2C pins: SCL */
  GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN[TwiStruct->TwiNr];
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(I2C_SCL_PORT[TwiStruct->TwiNr], &GPIO_InitStructure);

  /*!< Configure sEE_I2C pins: SDA */
  GPIO_InitStructure.GPIO_Pin = I2C_SDA_PIN[TwiStruct->TwiNr];
  GPIO_Init(I2C_SDA_PORT[TwiStruct->TwiNr], &GPIO_InitStructure);

  /* Configure and enable I2C DMA TX Channel interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = sEE_I2C_DMA_TX_IRQn[TwiStruct->TwiNr];
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = sEE_I2C_DMA_PREPRIO;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = sEE_I2C_DMA_SUBPRIO;
  NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Configure and enable I2C DMA RX Channel interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = sEE_I2C_DMA_RX_IRQn[TwiStruct->TwiNr];
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = sEE_I2C_DMA_PREPRIO;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = sEE_I2C_DMA_SUBPRIO;
  NVIC_Init(&NVIC_InitStructure);

  /* Disable and Deinitialize the DMA channels */
  DMA_Cmd(sEE_I2C_DMA_CHANNEL_TX[TwiStruct->TwiNr], DISABLE);
  DMA_Cmd(sEE_I2C_DMA_CHANNEL_RX[TwiStruct->TwiNr], DISABLE);
  DMA_DeInit(sEE_I2C_DMA_CHANNEL_TX[TwiStruct->TwiNr]);
  DMA_DeInit(sEE_I2C_DMA_CHANNEL_RX[TwiStruct->TwiNr]);
}
//#####################################################
/**
  * @brief  Initializes peripherals used by the I2C EEPROM driver.
  * @param  None
  * @retval None
  */
void sEE_LowLevel_Init(new_twi* TwiStruct)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /*!< sEE_I2C_SCL_GPIO_CLK and sEE_I2C_SDA_GPIO_CLK Periph clock enable */
  RCC_APB2PeriphClockCmd(I2C_SCL_PORT_CLK[TwiStruct->TwiNr] | I2C_SDA_PORT_CLK[TwiStruct->TwiNr], ENABLE);

  /*!< sEE_I2C Periph clock enable */
  RCC_APB1PeriphClockCmd(I2C_CLK[TwiStruct->TwiNr], ENABLE);

  /*!< GPIO configuration */
  /*!< Configure sEE_I2C pins: SCL */
  GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN[TwiStruct->TwiNr];
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_Init(I2C_SCL_PORT[TwiStruct->TwiNr], &GPIO_InitStructure);

  /*!< Configure sEE_I2C pins: SDA */
  GPIO_InitStructure.GPIO_Pin = I2C_SDA_PIN[TwiStruct->TwiNr];
  GPIO_Init(I2C_SDA_PORT[TwiStruct->TwiNr], &GPIO_InitStructure);

  /* Configure and enable I2C DMA TX Channel interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = sEE_I2C_DMA_TX_IRQn[TwiStruct->TwiNr];
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = sEE_I2C_DMA_PREPRIO;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = sEE_I2C_DMA_SUBPRIO;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Configure and enable I2C DMA RX Channel interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = sEE_I2C_DMA_RX_IRQn[TwiStruct->TwiNr];
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = sEE_I2C_DMA_PREPRIO;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = sEE_I2C_DMA_SUBPRIO;
  NVIC_Init(&NVIC_InitStructure);

  /*!< I2C DMA TX and RX channels configuration */
  /* Enable the DMA clock */
  RCC_AHBPeriphClockCmd(sEE_I2C_DMA_CLK[TwiStruct->TwiNr], ENABLE);

  /* I2C TX DMA Channel configuration */
  DMA_DeInit(sEE_I2C_DMA_CHANNEL_TX[TwiStruct->TwiNr]);
  sEEDMA_InitStructure[TwiStruct->TwiNr].DMA_PeripheralBaseAddr = (uint32_t)sEE_I2C_DR_Address[TwiStruct->TwiNr];
  sEEDMA_InitStructure[TwiStruct->TwiNr].DMA_MemoryBaseAddr = (uint32_t)0;   /* This parameter will be configured durig communication */
  sEEDMA_InitStructure[TwiStruct->TwiNr].DMA_DIR = DMA_DIR_PeripheralDST;    /* This parameter will be configured durig communication */
  sEEDMA_InitStructure[TwiStruct->TwiNr].DMA_BufferSize = 0xFFFF;            /* This parameter will be configured durig communication */
  sEEDMA_InitStructure[TwiStruct->TwiNr].DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  sEEDMA_InitStructure[TwiStruct->TwiNr].DMA_MemoryInc = DMA_MemoryInc_Enable;
  sEEDMA_InitStructure[TwiStruct->TwiNr].DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte;
  sEEDMA_InitStructure[TwiStruct->TwiNr].DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  sEEDMA_InitStructure[TwiStruct->TwiNr].DMA_Mode = DMA_Mode_Normal;
  sEEDMA_InitStructure[TwiStruct->TwiNr].DMA_Priority = DMA_Priority_VeryHigh;
  sEEDMA_InitStructure[TwiStruct->TwiNr].DMA_M2M = DMA_M2M_Disable;
  DMA_Init(sEE_I2C_DMA_CHANNEL_TX[TwiStruct->TwiNr], &sEEDMA_InitStructure[TwiStruct->TwiNr]);

  /* I2C RX DMA Channel configuration */
  DMA_DeInit(sEE_I2C_DMA_CHANNEL_RX[TwiStruct->TwiNr]);
  DMA_Init(sEE_I2C_DMA_CHANNEL_RX[TwiStruct->TwiNr], &sEEDMA_InitStructure[TwiStruct->TwiNr]);

  /* Enable the DMA Channels Interrupts */
  DMA_ITConfig(sEE_I2C_DMA_CHANNEL_TX[TwiStruct->TwiNr], DMA_IT_TC, ENABLE);
  DMA_ITConfig(sEE_I2C_DMA_CHANNEL_RX[TwiStruct->TwiNr], DMA_IT_TC, ENABLE);
}
//#####################################################
/**
  * @brief  Initializes DMA channel used by the I2C EEPROM driver.
  * @param  None
  * @retval None
  */
void sEE_LowLevel_DMAConfig(new_twi* TwiStruct, uint32_t pBuffer, uint32_t BufferSize, uint32_t Direction)
{
  /* Initialize the DMA with the new parameters */
  if (Direction == sEE_DIRECTION_TX)
  {
    /* Configure the DMA Tx Channel with the buffer address and the buffer size */
    sEEDMA_InitStructure[TwiStruct->TwiNr].DMA_MemoryBaseAddr = (uint32_t)pBuffer;
    sEEDMA_InitStructure[TwiStruct->TwiNr].DMA_DIR = DMA_DIR_PeripheralDST;
    sEEDMA_InitStructure[TwiStruct->TwiNr].DMA_BufferSize = (uint32_t)BufferSize;
    DMA_Init(sEE_I2C_DMA_CHANNEL_TX[TwiStruct->TwiNr], &sEEDMA_InitStructure[TwiStruct->TwiNr]);
  }
  else
  {
    /* Configure the DMA Rx Channel with the buffer address and the buffer size */
    sEEDMA_InitStructure[TwiStruct->TwiNr].DMA_MemoryBaseAddr = (uint32_t)pBuffer;
    sEEDMA_InitStructure[TwiStruct->TwiNr].DMA_DIR = DMA_DIR_PeripheralSRC;
    sEEDMA_InitStructure[TwiStruct->TwiNr].DMA_BufferSize = (uint32_t)BufferSize;
    DMA_Init(sEE_I2C_DMA_CHANNEL_RX[TwiStruct->TwiNr], &sEEDMA_InitStructure[TwiStruct->TwiNr]);
  }
}
//#####################################################
/**
  * @brief  This function handles the DMA Tx Channel interrupt Handler.
  * @param  None
  * @retval None
  */
void sEE_I2C1_DMA_TX_IRQHandler(void)
{
	uint32_t sEETimeout;
  /* Check if the DMA transfer is complete */
  if(DMA_GetFlagStatus(sEE_I2C_DMA_FLAG_TX_TC) != RESET)
  {
    /* Disable the DMA Tx Channel and Clear all its Flags */
    DMA_Cmd(sEE_I2C_DMA_CHANNEL_TX[0], DISABLE);
    DMA_ClearFlag(sEE_I2C_DMA_FLAG_TX_GL);

    /*!< Wait till all data have been physically transferred on the bus */
    sEETimeout = sEE_LONG_TIMEOUT;
    while(!I2C_GetFlagStatus(sEE_I2C[0], I2C_FLAG_BTF))
    {
      if((sEETimeout--) == 0) return;
    }

    /*!< Send STOP condition */
    I2C_GenerateSTOP(sEE_I2C[0], ENABLE);

    /* Perform a read on SR1 and SR2 register to clear eventualaly pending flags */
    (void)sEE_I2C[0]->SR1;
    (void)sEE_I2C[0]->SR2;

    /* Reset the variable holding the number of data to be written */
    //*sEEDataWritePointer = 0;
  }
}
//#####################################################
/**
  * @brief  This function handles the DMA Rx Channel interrupt Handler.
  * @param  None
  * @retval None
  */
void sEE_I2C1_DMA_RX_IRQHandler(void)
{
  /* Check if the DMA transfer is complete */
  if(DMA_GetFlagStatus(sEE_I2C_DMA_FLAG_RX_TC) != RESET)
  {
    /*!< Send STOP Condition */
    I2C_GenerateSTOP(sEE_I2C[0], ENABLE);

    /* Disable the DMA Rx Channel and Clear all its Flags */
    DMA_Cmd(sEE_I2C_DMA_CHANNEL_RX[0], DISABLE);
    DMA_ClearFlag(sEE_I2C_DMA_FLAG_RX_GL);

    /* Reset the variable holding the number of data to be read */
    //*sEEDataReadPointer = 0;
  }
}
//#####################################################
//#include "int/int_twi.h"
/**
  * @brief  Reads a block of data from the EEPROM.
  * @param  pBuffer : pointer to the buffer that receives the data read from
  *         the EEPROM.
  * @param  ReadAddr : EEPROM's internal address to start reading from.
  * @param  NumByteToRead : pointer to the variable holding number of bytes to
  *         be read from the EEPROM.
  *
  *        @note The variable pointed by NumByteToRead is reset to 0 when all the
  *              data are read from the EEPROM. Application should monitor this
  *              variable in order know when the transfer is complete.
  *
  * @note When number of data to be read is higher than 1, this function just
  *       configures the communication and enable the DMA channel to transfer data.
  *       Meanwhile, the user application may perform other tasks.
  *       When number of data to be read is 1, then the DMA is not used. The byte
  *       is read in polling mode.
  *
  * @retval sEE_OK (0) if operation is correctly performed, else return value
  *         different from sEE_OK (0) or the timeout user callback.
  */
uint32_t TWI_MasterWriteRead(new_twi* TwiStruct, unsigned int TransmitBytes, unsigned int ReceiveBytes)
{
	uint32_t sEETimeout;
	/* Set the pointer to the Number of data to be read. This pointer will be used
      by the DMA Transfer Completer interrupt Handler in order to reset the
      variable to 0. User should check on this variable in order to know if the
      DMA transfer has been complete or not. */
	uint16_t NumByteToRead = ReceiveBytes;

  /*!< While the bus is busy */
  sEETimeout = sEE_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(sEE_I2C[TwiStruct->TwiNr], I2C_FLAG_BUSY))
  {
    if((sEETimeout--) == 0)
    	return false;
  }

  /*!< Send START condition */
  I2C_GenerateSTART(sEE_I2C[TwiStruct->TwiNr], ENABLE);

  /*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
  sEETimeout = sEE_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(sEE_I2C[TwiStruct->TwiNr], I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((sEETimeout--) == 0)
    	return false;
  }

  /*!< Send EEPROM address for write */
  I2C_Send7bitAddress(sEE_I2C[TwiStruct->TwiNr], TwiStruct->MasterSlaveAddr << 1, I2C_Direction_Transmitter);

  /*!< Test on EV6 and clear it */
  sEETimeout = sEE_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(sEE_I2C[TwiStruct->TwiNr], I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    if((sEETimeout--) == 0)
    	return false;
  }


  unsigned int cnt = 0;
  for(; cnt < TransmitBytes; cnt++)
  {
	  /*!< Send the EEPROM's internal address to read from: MSB of the address first */
	  I2C_SendData(sEE_I2C[TwiStruct->TwiNr], TwiStruct->TxBuff[cnt]);

	  /*!< Test on EV8 and clear it */
	  sEETimeout = sEE_FLAG_TIMEOUT;
	  while(!I2C_CheckEvent(sEE_I2C[TwiStruct->TwiNr], I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	  {
		if((sEETimeout--) == 0)
			return false;
	  }
  }

  if(ReceiveBytes) {
	  /*!< Send STRAT condition a second time */
	  I2C_GenerateSTART(sEE_I2C[TwiStruct->TwiNr], ENABLE);

	  /*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
	  sEETimeout = sEE_FLAG_TIMEOUT;
	  while(!I2C_CheckEvent(sEE_I2C[TwiStruct->TwiNr], I2C_EVENT_MASTER_MODE_SELECT))
	  {
		if((sEETimeout--) == 0)
			return false;
	  }

	  /*!< Send EEPROM address for read */
	  I2C_Send7bitAddress(sEE_I2C[TwiStruct->TwiNr], TwiStruct->MasterSlaveAddr << 1, I2C_Direction_Receiver);

	  /* If number of data to be read is 1, then DMA couldn't be used */
	  /* One Byte Master Reception procedure (POLLING) ---------------------------*/
	  if (NumByteToRead < 2)
	  {
		/* Wait on ADDR flag to be set (ADDR is still not cleared at this level */
		sEETimeout = sEE_FLAG_TIMEOUT;
		while(I2C_GetFlagStatus(sEE_I2C[TwiStruct->TwiNr], I2C_FLAG_ADDR) == RESET)
		{
		  if((sEETimeout--) == 0)
			  return false;
		}

		/*!< Disable Acknowledgement */
		I2C_AcknowledgeConfig(sEE_I2C[TwiStruct->TwiNr], DISABLE);

		/* Call User callback for critical section start (should typically disable interrupts) */
		sEE_EnterCriticalSection_UserCallback();

		/* Clear ADDR register by reading SR1 then SR2 register (SR1 has already been read) */
		(void)sEE_I2C[TwiStruct->TwiNr]->SR2;

		/*!< Send STOP Condition */
		I2C_GenerateSTOP(sEE_I2C[TwiStruct->TwiNr], ENABLE);

		/* Call User callback for critical section end (should typically re-enable interrupts) */
		sEE_ExitCriticalSection_UserCallback();

		/* Wait for the byte to be received */
		sEETimeout = sEE_FLAG_TIMEOUT;
		while(I2C_GetFlagStatus(sEE_I2C[TwiStruct->TwiNr], I2C_FLAG_RXNE) == RESET)
		{
		  if((sEETimeout--) == 0)
			  return false;
		}

		/*!< Read the byte received from the EEPROM */
		*TwiStruct->RxBuff = I2C_ReceiveData(sEE_I2C[TwiStruct->TwiNr]);

		/*!< Decrement the read bytes counter */
		NumByteToRead--;

		/* Wait to make sure that STOP control bit has been cleared */
		sEETimeout = sEE_FLAG_TIMEOUT;
		while(sEE_I2C[TwiStruct->TwiNr]->CR1 & I2C_CR1_STOP)
		{
		  if((sEETimeout--) == 0)
			  return false;
		}

		/*!< Re-Enable Acknowledgement to be ready for another reception */
		I2C_AcknowledgeConfig(sEE_I2C[TwiStruct->TwiNr], ENABLE);
	  }
	  else/* More than one Byte Master Reception procedure (DMA) -----------------*/
	  {
		/*!< Test on EV6 and clear it */
		sEETimeout = sEE_FLAG_TIMEOUT;
		while(!I2C_CheckEvent(sEE_I2C[TwiStruct->TwiNr], I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
		{
		  if((sEETimeout--) == 0)
			  return false;
		}

		/* Configure the DMA Rx Channel with the buffer address and the buffer size */
		sEE_LowLevel_DMAConfig(TwiStruct, (uint32_t)TwiStruct->RxBuff, NumByteToRead, sEE_DIRECTION_RX);

		/* Inform the DMA that the next End Of Transfer Signal will be the last one */
		I2C_DMALastTransferCmd(sEE_I2C[TwiStruct->TwiNr], ENABLE);

		/* Enable the DMA Rx Channel */
		DMA_Cmd(sEE_I2C_DMA_CHANNEL_RX[TwiStruct->TwiNr], ENABLE);
	  }
  }
  else {
		/* Call User callback for critical section start (should typically disable interrupts) */
		sEE_EnterCriticalSection_UserCallback();

		/* Clear ADDR register by reading SR1 then SR2 register (SR1 has already been read) */
		(void)sEE_I2C[TwiStruct->TwiNr]->SR2;

		/*!< Send STOP Condition */
		I2C_GenerateSTOP(sEE_I2C[TwiStruct->TwiNr], ENABLE);

		/* Call User callback for critical section end (should typically re-enable interrupts) */
		sEE_ExitCriticalSection_UserCallback();

		/* Wait for the byte to be received */
		sEETimeout = sEE_FLAG_TIMEOUT;
		while(I2C_GetFlagStatus(sEE_I2C[TwiStruct->TwiNr], I2C_FLAG_RXNE) == RESET)
		{
		  if((sEETimeout--) == 0)
			  return true;
		}

  }
  /* If all operations OK, return sEE_OK (0) */
  return true;
}
//#####################################################
/**
  * @brief  Initializes peripherals used by the I2C EEPROM driver.
  * @param  None
  * @retval None
  */
bool TWI_open(new_twi* TwiStruct)
{
  I2C_InitTypeDef  I2C_InitStructure;

  sEE_LowLevel_Init(TwiStruct);

  /*!< I2C configuration */
  /* sEE_I2C configuration */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = TwiStruct->MasterSlaveAddr;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = TwiStruct->BaudRate;

  /* sEE_I2C Peripheral Enable */
  I2C_Cmd(sEE_I2C[TwiStruct->TwiNr], ENABLE);
  /* Apply sEE_I2C configuration after enabling it */
  I2C_Init(sEE_I2C[TwiStruct->TwiNr], &I2C_InitStructure);

  /* Enable the sEE_I2C peripheral DMA requests */
  I2C_DMACmd(sEE_I2C[TwiStruct->TwiNr], ENABLE);
  return true;
}
/*#####################################################*/
bool _SetupI2CTransmit(new_twi* TwiStruct, unsigned int TransmitBytes)
{
	return TWI_MasterWriteRead(TwiStruct, TransmitBytes, 0);
}
/*#####################################################*/
bool _SetupI2CReception(new_twi* TwiStruct, unsigned int TransmitBytes, unsigned int ReceiveBytes)
{
	return TWI_MasterWriteRead(TwiStruct, TransmitBytes, ReceiveBytes);
}
/*#####################################################*/
bool _twi_open(new_twi* TwiStruct)
{
	if(TwiStruct->RxBuffSize) {
		TwiStruct->RxBuff = calloc(1, TwiStruct->RxBuffSize);
		if(!TwiStruct->RxBuff) {
			_twi_close(TwiStruct);
			return false;
		}
	}
	if(TwiStruct->TxBuffSize) {
		TwiStruct->TxBuff = calloc(1, TwiStruct->TxBuffSize);
		if(!TwiStruct->TxBuff) {
			_twi_close(TwiStruct);
			return false;
		}
	}
	return TWI_open(TwiStruct);
}
/*#####################################################*/
void _twi_close(new_twi* TwiStruct)
{
	sEE_LowLevel_DeInit(TwiStruct);
	if(TwiStruct->RxBuff) free(TwiStruct->RxBuff);
	if(TwiStruct->TxBuff) free(TwiStruct->TxBuff);
}
/*#####################################################*/
