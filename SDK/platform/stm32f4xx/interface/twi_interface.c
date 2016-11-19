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
#include "include/stm32f4xx.h"
#include "driver/stm32f4xx_hal_i2c.h"
#include "driver/stm32f4xx_hal_rcc.h"
#include "api/timer_api.h"
#include "api/twi_def.h"
#include "gpio_interface.h"

#define USE_I2C_TX_DMA
//#####################################################
I2C_TypeDef *sEE_I2C[3] = {
#ifdef I2C1
		I2C1,
#endif
#ifdef I2C2
		I2C2,
#endif
#ifdef I2C3
		I2C3,
#endif
#ifdef I2C4
		I2C4,
#endif
#ifdef I2C5
		I2C5,
#endif
#ifdef I2C6
		I2C6
#endif
};
#define I2Cn                             3


//const unsigned long I2C_CLK[I2Cn] = {RCC_APB1Periph_I2C1, RCC_APB1Periph_I2C2, RCC_APB1Periph_I2C3};


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
  //I2C_Cmd(sEE_I2C[TwiStruct->TwiNr], DISABLE);

  /* sEE_I2C DeInit */
  //I2C_DeInit(sEE_I2C[TwiStruct->TwiNr]);

  /*!< sEE_I2C Periph clock disable */
  //RCC_APB1PeriphClockCmd(I2C_CLK[TwiStruct->TwiNr], DISABLE);
  switch(TwiStruct->TwiNr)
  {
#ifdef __HAL_RCC_I2C1_CLK_DISABLE
  case 0:
	  __HAL_RCC_I2C1_CLK_DISABLE();
	  break;
#endif
#ifdef __HAL_RCC_I2C2_CLK_DISABLE
  case 1:
	  __HAL_RCC_I2C2_CLK_DISABLE();
	  break;
#endif
#ifdef __HAL_RCC_I2C3_CLK_DISABLE
  case 2:
	  __HAL_RCC_I2C3_CLK_DISABLE();
	  break;
#endif
#ifdef __HAL_RCC_I2C4_CLK_DISABLE
  case 3:
	  __HAL_RCC_I2C4_CLK_DISABLE();
	  break;
#endif
#ifdef __HAL_RCC_I2C5_CLK_DISABLE
  case 4:
	  __HAL_RCC_I2C5_CLK_DISABLE();
	  break;
#endif
#ifdef __HAL_RCC_I2C6_CLK_DISABLE
  case 5:
	  __HAL_RCC_I2C6_CLK_DISABLE();
	  break;
#endif
  }
  /*!< GPIO configuration */
  /*!< Configure sEE_I2C pins: SCL */
  GPIO_InitStructure.Pin = 1 << TwiStruct->SclPin;
  GPIO_InitStructure.Alternate = GPIO_AF4_I2C1;
  GPIO_InitStructure.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStructure.Pull = GPIO_PULLUP;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GET_GPIO_PORT_ADDR[TwiStruct->SclPort], &GPIO_InitStructure);

  /*!< Configure sEE_I2C pins: SDA */
  GPIO_InitStructure.Pin = 1 << TwiStruct->SdaPin;
  HAL_GPIO_Init(GET_GPIO_PORT_ADDR[TwiStruct->SdaPort], &GPIO_InitStructure);

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
  //RCC_APB2PeriphClockCmd(GET_PORT_CLK_ADDR[TwiStruct->SclPort] | GET_PORT_CLK_ADDR[TwiStruct->SdaPort], ENABLE);
  _gpio_init(TwiStruct->SclPort);
  _gpio_init(TwiStruct->SdaPort);

  /*!< sEE_I2C Periph clock enable */
  //RCC_APB1PeriphClockCmd(I2C_CLK[TwiStruct->TwiNr], ENABLE);
  switch(TwiStruct->TwiNr)
  {
#ifdef __HAL_RCC_I2C1_CLK_ENABLE
  case 0:
	  __HAL_RCC_I2C1_CLK_ENABLE();
	  break;
#endif
#ifdef __HAL_RCC_I2C2_CLK_ENABLE
  case 1:
	  __HAL_RCC_I2C2_CLK_ENABLE();
	  break;
#endif
#ifdef __HAL_RCC_I2C3_CLK_ENABLE
  case 2:
	  __HAL_RCC_I2C3_CLK_ENABLE();
	  break;
#endif
#ifdef __HAL_RCC_I2C4_CLK_ENABLE
  case 3:
	  __HAL_RCC_I2C4_CLK_ENABLE();
	  break;
#endif
#ifdef __HAL_RCC_I2C5_CLK_ENABLE
  case 4:
	  __HAL_RCC_I2C5_CLK_ENABLE();
	  break;
#endif
#ifdef __HAL_RCC_I2C6_CLK_ENABLE
  case 5:
	  __HAL_RCC_I2C6_CLK_ENABLE();
	  break;
#endif
  }
  /*!< GPIO configuration */
  /*!< Configure sEE_I2C pins: SCL */
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
  GPIO_InitStructure.Alternate = GPIO_AF4_I2C1;
  GPIO_InitStructure.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStructure.Pull  = GPIO_PULLUP;

  GPIO_InitStructure.Pin = 1 << TwiStruct->SclPin;
  HAL_GPIO_Init(GET_GPIO_PORT_ADDR[TwiStruct->SclPort], &GPIO_InitStructure);

  /*!< Configure sEE_I2C pins: SDA */
  //GPIO_InitStructure.Mode = GPIO_OType_OD;
  GPIO_InitStructure.Pin = 1 << TwiStruct->SdaPin;
  HAL_GPIO_Init(GET_GPIO_PORT_ADDR[TwiStruct->SdaPort], &GPIO_InitStructure);

  //GPIO_PinAFConfig(GET_GPIO_PORT_ADDR[TwiStruct->SclPort], TwiStruct->SclPin, GPIO_AF_I2C1);
  //GPIO_PinAFConfig(GET_GPIO_PORT_ADDR[TwiStruct->SdaPort], TwiStruct->SdaPin, GPIO_AF_I2C1);

}
/**
  * @brief  Enables or disables the specified I2C software reset.
  * @note   When software reset is enabled, the I2C IOs are released (this can
  *         be useful to recover from bus errors).
  * @param  I2Cx: where x can be 1, 2 or 3 to select the I2C peripheral.
  * @param  NewState: new state of the I2C software reset.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void I2C_SoftwareResetCmd(I2C_TypeDef* I2Cx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* Peripheral under reset */
    I2Cx->CR1 |= I2C_CR1_SWRST;
  }
  else
  {
    /* Peripheral not under reset */
    I2Cx->CR1 &= (uint16_t)~((uint16_t)I2C_CR1_SWRST);
  }
}

void TWI_SendStop(I2C_HandleTypeDef *hi2c)
{

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
unsigned long TWI_MasterWriteRead(new_twi* TwiStruct, unsigned int TransmitBytes, unsigned int Size)
{
	//unsigned long sEETimeout;
	/* Set the pointer to the Number of data to be read. This pointer will be used
      by the DMA Transfer Completer interrupt Handler in order to reset the
      variable to 0. User should check on this variable in order to know if the
      DMA transfer has been complete or not. */
	I2C_TypeDef *I2Cx = sEE_I2C[TwiStruct->TwiNr];
	I2C_HandleTypeDef *hi2c = ((I2C_HandleTypeDef *)TwiStruct->udata);
	/* If bus is freeze we will reset the unit and restore the settings. */
	if(__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_AF | I2C_FLAG_ARLO) || (I2Cx->CR1 & (1 << 8))) {
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
	  while(__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BUSY))
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

	  unsigned int cnt = 0;
	  unsigned long Timeout = 10;
	    /* Disable Pos */
	    hi2c->Instance->CR1 &= ~I2C_CR1_POS;

	    hi2c->State = HAL_I2C_STATE_MEM_BUSY_RX;
	    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;

	    if(!TwiStruct->NoSendWriteOnRead)
	  {


		  /* Enable Acknowledge */
		  hi2c->Instance->CR1 |= I2C_CR1_ACK;

		  /* Generate Start */
		  hi2c->Instance->CR1 |= I2C_CR1_START;

		  /* Wait until SB flag is set */
		  if(I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_SB, RESET, Timeout) != HAL_OK)
		  {
        	  __HAL_UNLOCK(hi2c);
		    return false;
		  }

		  /* Send slave address */
		  hi2c->Instance->DR = I2C_7BIT_ADD_WRITE(TwiStruct->MasterSlaveAddr << 1);

		  /* Wait until ADDR flag is set */
		  if(I2C_WaitOnMasterAddressFlagUntilTimeout(hi2c, I2C_FLAG_ADDR, Timeout) != HAL_OK)
		  {
		    if(hi2c->ErrorCode == HAL_I2C_ERROR_AF)
		    {
	        	  __HAL_UNLOCK(hi2c);
		      return false;
		    }
		    else
		    {
	        	  __HAL_UNLOCK(hi2c);
		      return false;
		    }
		  }

		  /* Clear ADDR flag */
		  __HAL_I2C_CLEAR_ADDRFLAG(hi2c);

		  /* Wait until TXE flag is set */
		  if(I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_TXE, RESET, Timeout) != HAL_OK)
		  {
        	  __HAL_UNLOCK(hi2c);
		    return false;
		  }



		  for(; cnt < TransmitBytes; cnt++)
		  {

			/* Send MSB of Memory Address */
			hi2c->Instance->DR = TwiStruct->TxBuff[cnt];

			/* Wait until TXE flag is set */
			if(I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_TXE, RESET, Timeout) != HAL_OK)
			{
	        	  __HAL_UNLOCK(hi2c);
			return false;
			}
		  }
	  }

  if(!Size)
  {
	  /* Generate Stop */
	  hi2c->Instance->CR1 |= I2C_CR1_STOP;

	  hi2c->State = HAL_I2C_STATE_READY;

	  /* Process Unlocked */
	  __HAL_UNLOCK(hi2c);

  }else
  {

	  /* Generate Restart */
	  hi2c->Instance->CR1 |= I2C_CR1_START;

	  /* Wait until SB flag is set */
	  if(I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_SB, RESET, Timeout) != HAL_OK)
	  {
    	  __HAL_UNLOCK(hi2c);
	    return false;
	  }

	  /* Send slave address */
	  hi2c->Instance->DR = I2C_7BIT_ADD_READ(TwiStruct->MasterSlaveAddr << 1);

	  /* Wait until ADDR flag is set */
	  if(I2C_WaitOnMasterAddressFlagUntilTimeout(hi2c, I2C_FLAG_ADDR, Timeout) != HAL_OK)
	  {
    	  __HAL_UNLOCK(hi2c);
		  return false;
	  }
	  unsigned long data_cnt = 0;

	  if(Size == 1)
	      {
	        /* Disable Acknowledge */
	        hi2c->Instance->CR1 &= ~I2C_CR1_ACK;

	        /* Clear ADDR flag */
	        __HAL_I2C_CLEAR_ADDRFLAG(hi2c);

	        /* Generate Stop */
	        hi2c->Instance->CR1 |= I2C_CR1_STOP;
	      }
	      else if(Size == 2)
	      {
	        /* Disable Acknowledge */
	        hi2c->Instance->CR1 &= ~I2C_CR1_ACK;

	        /* Enable Pos */
	        hi2c->Instance->CR1 |= I2C_CR1_POS;

	        /* Clear ADDR flag */
	        __HAL_I2C_CLEAR_ADDRFLAG(hi2c);
	      }
	      else
	      {
	        /* Enable Acknowledge */
	        hi2c->Instance->CR1 |= I2C_CR1_ACK;

	        /* Clear ADDR flag */
	        __HAL_I2C_CLEAR_ADDRFLAG(hi2c);
	      }

	      while(Size > 0)
	      {
	        if(Size <= 3)
	        {
	          /* One byte */
	          if(Size == 1)
	          {
	            /* Wait until RXNE flag is set */
	            if(I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_RXNE, RESET, Timeout) != HAL_OK)
	            {
		        	  __HAL_UNLOCK(hi2c);
	              return false;
	            }

	            /* Read data from DR */
	            TwiStruct->RxBuff[data_cnt++] = hi2c->Instance->DR;
	            Size--;
	          }
	          /* Two bytes */
	          else if(Size == 2)
	          {
	            /* Wait until BTF flag is set */
	            if(I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_BTF, RESET, Timeout) != HAL_OK)
	            {
		        	  __HAL_UNLOCK(hi2c);
	              return false;
	            }

	            /* Generate Stop */
	            hi2c->Instance->CR1 |= I2C_CR1_STOP;

	            /* Read data from DR */
	            TwiStruct->RxBuff[data_cnt++] = hi2c->Instance->DR;
	            Size--;

	            /* Read data from DR */
	            TwiStruct->RxBuff[data_cnt++] = hi2c->Instance->DR;
	            Size--;
	          }
	          /* 3 Last bytes */
	          else
	          {
	            /* Wait until BTF flag is set */
	            if(I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_BTF, RESET, Timeout) != HAL_OK)
	            {
		        	  __HAL_UNLOCK(hi2c);
	              return false;
	            }

	            /* Disable Acknowledge */
	            hi2c->Instance->CR1 &= ~I2C_CR1_ACK;

	            /* Read data from DR */
	            TwiStruct->RxBuff[data_cnt++] = hi2c->Instance->DR;
	            Size--;

	            /* Wait until BTF flag is set */
	            if(I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_BTF, RESET, Timeout) != HAL_OK)
	            {
		        	  __HAL_UNLOCK(hi2c);
	              return false;
	            }

	            /* Generate Stop */
	            hi2c->Instance->CR1 |= I2C_CR1_STOP;

	            /* Read data from DR */
	            TwiStruct->RxBuff[data_cnt++] = hi2c->Instance->DR;
	            Size--;

	            /* Read data from DR */
	            TwiStruct->RxBuff[data_cnt++] = hi2c->Instance->DR;
	            Size--;
	          }
	        }
	        else
	        {
	          /* Wait until RXNE flag is set */
	          if(I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_RXNE, RESET, Timeout) != HAL_OK)
	          {
	        	  __HAL_UNLOCK(hi2c);
	            return false;
	          }

	          /* Read data from DR */
	          TwiStruct->RxBuff[data_cnt++] = hi2c->Instance->DR;
	          Size--;

	          if(__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BTF) == SET)
	          {
	            /* Read data from DR */
	        	  TwiStruct->RxBuff[data_cnt++] = hi2c->Instance->DR;
	            Size--;
	          }
	        }
	      }
	    }
  /* Process Unlocked */
  hi2c->State = HAL_I2C_STATE_READY;

  /* Process Unlocked */
 __HAL_UNLOCK(hi2c);
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
	  I2C_HandleTypeDef *I2cHandle = calloc(1, sizeof(I2C_HandleTypeDef));
	  if(!I2cHandle)
		  return false;
	   //I2C_InitTypeDef  I2C_InitStructure;
	  TwiStruct->udata = (void *)I2cHandle;
  sEE_LowLevel_Init(TwiStruct);
 /*##-1- Configure the I2C peripheral #######################################*/
  I2cHandle->Instance             = sEE_I2C[TwiStruct->TwiNr];

  I2cHandle->Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
  I2cHandle->Init.ClockSpeed      = TwiStruct->BaudRate;
  I2cHandle->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  I2cHandle->Init.DutyCycle       = I2C_DUTYCYCLE_2;
  I2cHandle->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  I2cHandle->Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
  I2cHandle->Init.OwnAddress1     = TwiStruct->MasterSlaveAddr;
  //I2cHandle.Init.OwnAddress2     = 0xFE;

  if(HAL_I2C_Init(I2cHandle) != HAL_OK)
  {
    /* Initialization Error */
    return false;
  }

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


bool _I2C_trx(struct Twi_s* param, unsigned char addr, unsigned char *buff_send, unsigned int bytes_send, unsigned char *buff_receive, unsigned int bytes_receive)
{
	param->MasterSlaveAddr = addr;
	param->TxBuff = buff_send;
	param->RxBuff = buff_receive;
	return TWI_MasterWriteRead(param, bytes_send, bytes_receive);
}

bool _I2C_tx(struct Twi_s* param, unsigned char addr, unsigned char *buff_send, unsigned int bytes_send)
{
	param->MasterSlaveAddr = addr;
	param->TxBuff = buff_send;
	return TWI_MasterWriteRead(param, bytes_send, 0);
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
	if(TwiStruct->udata) free(TwiStruct->udata);
	if(TwiStruct->RxBuff) free(TwiStruct->RxBuff);
	if(TwiStruct->TxBuff) free(TwiStruct->TxBuff);
}
/*#####################################################*/
