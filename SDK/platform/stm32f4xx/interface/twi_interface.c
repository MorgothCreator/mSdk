/*
 * twi_interface.c
 *
 * Created: 2/8/2013 5:00:27 PM
 *  Iulian Gheorghiu <morgoth.creator@gmail.com>
 */
/*#####################################################*/
#include "stdlib.h"
#include "stm32f4xx_conf.h"
#include "include/stm32f4xx.h"
#include "twi_interface.h"
#include "driver/stm32f4xx_i2c.h"
#include "api/timer_api.h"
#include "api/twi_def.h"
#include "gpio_interface.h"

#define USE_I2C_TX_DMA
//#####################################################
I2C_TypeDef *sEE_I2C[3] = {I2C1, I2C2, I2C3};
#define I2Cn                             3

I2C_TypeDef* I2C[I2Cn] = { I2C1, I2C2, I2C3};

const uint32_t I2C_CLK[I2Cn] = {RCC_APB1Periph_I2C1, RCC_APB1Periph_I2C2, RCC_APB1Periph_I2C3};


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

  /* sEE_I2C Peripheral Disable */
  I2C_Cmd(sEE_I2C[TwiStruct->TwiNr], DISABLE);

  /* sEE_I2C DeInit */
  I2C_DeInit(sEE_I2C[TwiStruct->TwiNr]);

  /*!< sEE_I2C Periph clock disable */
  RCC_APB1PeriphClockCmd(I2C_CLK[TwiStruct->TwiNr], DISABLE);

  /*!< GPIO configuration */
  /*!< Configure sEE_I2C pins: SCL */
  GPIO_InitStructure.GPIO_Pin = 1 << TwiStruct->SclPin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init(GET_GPIO_PORT_ADDR[TwiStruct->SclPort], &GPIO_InitStructure);

  /*!< Configure sEE_I2C pins: SDA */
  GPIO_InitStructure.GPIO_Pin = 1 << TwiStruct->SdaPin;
  GPIO_Init(GET_GPIO_PORT_ADDR[TwiStruct->SdaPort], &GPIO_InitStructure);

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

  /*!< sEE_I2C_SCL_GPIO_CLK and sEE_I2C_SDA_GPIO_CLK Periph clock enable */
  RCC_APB2PeriphClockCmd(GET_PORT_CLK_ADDR[TwiStruct->SclPort] | GET_PORT_CLK_ADDR[TwiStruct->SdaPort], ENABLE);

  /*!< sEE_I2C Periph clock enable */
  RCC_APB1PeriphClockCmd(I2C_CLK[TwiStruct->TwiNr], ENABLE);

  /*!< GPIO configuration */
  /*!< Configure sEE_I2C pins: SCL */
  GPIO_InitStructure.GPIO_Pin = 1 << TwiStruct->SclPin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GET_GPIO_PORT_ADDR[TwiStruct->SclPort], &GPIO_InitStructure);

  /*!< Configure sEE_I2C pins: SDA */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_Pin = 1 << TwiStruct->SdaPin;
  GPIO_Init(GET_GPIO_PORT_ADDR[TwiStruct->SdaPort], &GPIO_InitStructure);

  GPIO_PinAFConfig(GET_GPIO_PORT_ADDR[TwiStruct->SclPort], TwiStruct->SclPin, GPIO_AF_I2C1);
  GPIO_PinAFConfig(GET_GPIO_PORT_ADDR[TwiStruct->SdaPort], TwiStruct->SdaPin, GPIO_AF_I2C1);

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
	I2C_TypeDef *I2Cx = sEE_I2C[TwiStruct->TwiNr];
	/* If bus is freeze we will reset the unit and restore the settings. */
	if(I2C_GetFlagStatus(sEE_I2C[TwiStruct->TwiNr], I2C_FLAG_AF | I2C_FLAG_ARLO) || (I2Cx->CR1 & (1 << 8))) {
		  /* sEE_I2C Peripheral Enable */
		I2C_TypeDef I2CxBack;
		I2CxBack.CR1 = I2Cx->CR1;
		I2CxBack.CR2 = I2Cx->CR2;
		I2CxBack.OAR1 = I2Cx->OAR1;
		I2CxBack.OAR2 = I2Cx->OAR2;
		I2CxBack.CCR = I2Cx->CCR;
		I2CxBack.TRISE = I2Cx->TRISE;
		I2C_SoftwareResetCmd(sEE_I2C[TwiStruct->TwiNr], ENABLE);
		I2C_SoftwareResetCmd(sEE_I2C[TwiStruct->TwiNr], DISABLE);
		I2Cx->TRISE = I2CxBack.TRISE;
		I2Cx->CCR = I2CxBack.CCR;
		I2Cx->OAR2 = I2CxBack.OAR2;
		I2Cx->OAR1 = I2CxBack.OAR1;
		I2Cx->CR2 = I2CxBack.CR2;
		I2Cx->CR1 = I2CxBack.CR1;
	}
  /*!< While the bus is busy */
	timer(TimerBusyTimeout);
	  timer_interval(&TimerBusyTimeout, TwiStruct->BusyTimeOut);
	  timer_enable(&TimerBusyTimeout);
	  while(I2C_GetFlagStatus(sEE_I2C[TwiStruct->TwiNr], I2C_FLAG_BUSY))
	  {
		  if(timer_tick(&TimerBusyTimeout)) {
				I2C_TypeDef I2CxBack;
				I2CxBack.CR1 = I2Cx->CR1;
				I2CxBack.CR2 = I2Cx->CR2;
				I2CxBack.OAR1 = I2Cx->OAR1;
				I2CxBack.OAR2 = I2Cx->OAR2;
				I2CxBack.CCR = I2Cx->CCR;
				I2CxBack.TRISE = I2Cx->TRISE;
				I2C_SoftwareResetCmd(sEE_I2C[TwiStruct->TwiNr], ENABLE);
				I2C_SoftwareResetCmd(sEE_I2C[TwiStruct->TwiNr], DISABLE);
				I2Cx->TRISE = I2CxBack.TRISE;
				I2Cx->CCR = I2CxBack.CCR;
				I2Cx->OAR2 = I2CxBack.OAR2;
				I2Cx->OAR1 = I2CxBack.OAR1;
				I2Cx->CR2 = I2CxBack.CR2;
				I2Cx->CR1 = I2CxBack.CR1;
				break;
			}
	  }
  /*!< Send START condition */
  I2C_GenerateSTART(sEE_I2C[TwiStruct->TwiNr], ENABLE);

  /*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
  sEETimeout = sEE_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(sEE_I2C[TwiStruct->TwiNr], I2C_EVENT_MASTER_MODE_SELECT))
  {
		if((sEETimeout--) == 0) {
			return false;
		}
  }

  /*!< Send EEPROM address for write */
  I2C_Send7bitAddress(sEE_I2C[TwiStruct->TwiNr], TwiStruct->MasterSlaveAddr << 1, I2C_Direction_Transmitter);

  /*!< Test on EV6 and clear it */
  sEETimeout = sEE_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(sEE_I2C[TwiStruct->TwiNr], I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
		if((sEETimeout--) == 0) {
			return false;
		}
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
			if((sEETimeout--) == 0) {
				return false;
			}
	  }
  }

  if(ReceiveBytes) {
	  /* If number of data to be read is 1, then DMA couldn't be used */
	  /* One Byte Master Reception procedure (POLLING) ---------------------------*/
	  if (NumByteToRead < 2)
	  {
		  /*!< Send STRAT condition a second time */
		  I2C_GenerateSTART(sEE_I2C[TwiStruct->TwiNr], ENABLE);

		  /*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
		  sEETimeout = sEE_FLAG_TIMEOUT;
		  while(!I2C_CheckEvent(sEE_I2C[TwiStruct->TwiNr], I2C_EVENT_MASTER_MODE_SELECT))
		  {
				if((sEETimeout--) == 0) {
					return false;
				}
		  }

		  /*!< Send EEPROM address for read */
		  I2C_Send7bitAddress(sEE_I2C[TwiStruct->TwiNr], TwiStruct->MasterSlaveAddr << 1, I2C_Direction_Receiver);

		/* Wait on ADDR flag to be set (ADDR is still not cleared at this level */
		sEETimeout = sEE_FLAG_TIMEOUT;
		while(I2C_GetFlagStatus(sEE_I2C[TwiStruct->TwiNr], I2C_FLAG_ADDR) == RESET)
		{
			if((sEETimeout--) == 0) {
				return false;
			}
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
			if((sEETimeout--) == 0) {
				return false;
			}
		}

		/*!< Read the byte received from the EEPROM */
		*TwiStruct->RxBuff = I2C_ReceiveData(sEE_I2C[TwiStruct->TwiNr]);

		/*!< Decrement the read bytes counter */
		NumByteToRead--;

		/* Wait to make sure that STOP control bit has been cleared */
		sEETimeout = sEE_FLAG_TIMEOUT;
		while(sEE_I2C[TwiStruct->TwiNr]->CR1 & I2C_CR1_STOP)
		{
			if((sEETimeout--) == 0) {
				return false;
			}
		}

		/*!< Re-Enable Acknowledgement to be ready for another reception */
		I2C_AcknowledgeConfig(sEE_I2C[TwiStruct->TwiNr], ENABLE);
	  }
	  else/* More than one Byte Master Reception procedure (DMA) -----------------*/
	  {
		  /*!< Send STRAT condition a second time */
		  I2C_GenerateSTART(sEE_I2C[TwiStruct->TwiNr], ENABLE);

		  /*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
		  sEETimeout = sEE_FLAG_TIMEOUT;
		  while(!I2C_CheckEvent(sEE_I2C[TwiStruct->TwiNr], I2C_EVENT_MASTER_MODE_SELECT))
		  {
				if((sEETimeout--) == 0) {
					return false;
				}
		  }

		  /*!< Send EEPROM address for read */
		  I2C_Send7bitAddress(sEE_I2C[TwiStruct->TwiNr], TwiStruct->MasterSlaveAddr << 1, I2C_Direction_Receiver);

		  /* If number of data to be read is 1, then DMA couldn't be used */
		  /* One Byte Master Reception procedure (POLLING) ---------------------------*/
		  //if (NumByteToRead < 2)
		  //{
			/* Wait on ADDR flag to be set (ADDR is still not cleared at this level */
			sEETimeout = sEE_FLAG_TIMEOUT;
			while(I2C_GetFlagStatus(sEE_I2C[TwiStruct->TwiNr], I2C_FLAG_ADDR) == RESET)
			{
				if((sEETimeout--) == 0) {
					return false;
				}
			}
			(void)sEE_I2C[TwiStruct->TwiNr]->SR2;

			  cnt = 0;
			  for(; cnt < ReceiveBytes; cnt++)
			  {
				if(cnt == ReceiveBytes - 1)
				{
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
				}

				/* Wait for the byte to be received */
				sEETimeout = sEE_FLAG_TIMEOUT;
				while(I2C_GetFlagStatus(sEE_I2C[TwiStruct->TwiNr], I2C_FLAG_RXNE) == RESET)
				{
					if((sEETimeout--) == 0) {
						return false;
					}
				}

				/*!< Read the byte received from the EEPROM */
				TwiStruct->RxBuff[cnt] = I2C_ReceiveData(sEE_I2C[TwiStruct->TwiNr]);
			  }

			/* Wait to make sure that STOP control bit has been cleared */
			sEETimeout = sEE_FLAG_TIMEOUT;
			while(sEE_I2C[TwiStruct->TwiNr]->CR1 & I2C_CR1_STOP)
			{
				if((sEETimeout--) == 0) {
					return false;
				}
			}

			/*!< Re-Enable Acknowledgement to be ready for another reception */
			I2C_AcknowledgeConfig(sEE_I2C[TwiStruct->TwiNr], ENABLE);
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
			if((sEETimeout--) == 0) {
				return true;
			}
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
