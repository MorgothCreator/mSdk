/*
 * twi_interface.c
 *
 * Created: 2/8/2013 5:00:27 PM
 *  Iulian Gheorghiu <morgoth.creator@gmail.com>
 */
/*#####################################################*/
#include "stdlib.h"
//#include "stm32f7xx_conf.h"
#include "include/stm32f7xx.h"
#include "main.h"
#include "twi_interface.h"
#include "include/stm32f7xx.h"
#include "driver/stm32f7xx_hal.h"
#include "driver/stm32f7xx_hal_i2c.h"
#include "driver/stm32f7xx_hal_rcc.h"
#include "api/timer_api.h"
#include "api/twi_def.h"
#include "twi_interface_def.h"
#include "gpio_interface.h"

#define TIMING_CLEAR_MASK   ((uint32_t)0xF0FFFFFFU)  /*!<  I2C TIMING clear register Mask */
#define I2C_TIMEOUT_ADDR    ((uint32_t)10000U)       /*!< 10 s  */
#define I2C_TIMEOUT_BUSY    ((uint32_t)25U)          /*!< 25 ms */
#define I2C_TIMEOUT_DIR     ((uint32_t)25U)          /*!< 25 ms */
#define I2C_TIMEOUT_RXNE    ((uint32_t)25U)          /*!< 25 ms */
#define I2C_TIMEOUT_STOPF   ((uint32_t)25U)          /*!< 25 ms */
#define I2C_TIMEOUT_TC      ((uint32_t)25U)          /*!< 25 ms */
#define I2C_TIMEOUT_TCR     ((uint32_t)25U)          /*!< 25 ms */
#define I2C_TIMEOUT_TXIS    ((uint32_t)25U)          /*!< 25 ms */
#define I2C_TIMEOUT_FLAG    ((uint32_t)25U)          /*!< 25 ms */

#define MAX_NBYTE_SIZE      255U
#define SlaveAddr_SHIFT     7U
#define SlaveAddr_MSK       0x06U

#define USE_I2C_TX_DMA
/* I2C TIMING Register define when I2C clock source is APB1 (SYSCLK/4) */
/* I2C TIMING is calculated in case of the I2C Clock source is the APB1CLK = 50 MHz */
/* This example use TIMING to 0x40912732 to reach 100 kHz speed (Rise time = 700 ns, Fall time = 100 ns) */
#define I2C_TIMING      0x40912732
//#####################################################
#if (USE_DRIVER_SEMAPHORE == true)
volatile bool twi_semaphore[TWI_INTERFACE_COUNT];
#endif

I2C_TypeDef *sEE_I2C[4] = {
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

  if(TwiStruct->TwiNr == 3 && TwiStruct->SclPort == IOB && TwiStruct->SclPin == 8)
	  GPIO_InitStructure.Alternate = GPIO_AF1_I2C4;

  GPIO_InitStructure.Pin = 1 << TwiStruct->SclPin;
  HAL_GPIO_Init(GET_GPIO_PORT_ADDR[TwiStruct->SclPort], &GPIO_InitStructure);

  GPIO_InitStructure.Alternate = GPIO_AF4_I2C1;
  if(TwiStruct->TwiNr == 3 && TwiStruct->SdaPort == IOB && TwiStruct->SdaPin == 7)
	  GPIO_InitStructure.Alternate = GPIO_AF11_I2C4;
  else if(TwiStruct->TwiNr == 3 && TwiStruct->SdaPort == IOB && TwiStruct->SdaPin == 9)
	  GPIO_InitStructure.Alternate = GPIO_AF1_I2C4;
  /*!< Configure sEE_I2C pins: SDA */
  //GPIO_InitStructure.Mode = GPIO_OType_OD;
  GPIO_InitStructure.Pin = 1 << TwiStruct->SdaPin;
  HAL_GPIO_Init(GET_GPIO_PORT_ADDR[TwiStruct->SdaPort], &GPIO_InitStructure);
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
void I2C_SoftwareResetCmd(I2C_HandleTypeDef *hi2c)
{
	I2C_TypeDef* I2Cx = hi2c->Instance;
	/* Check the parameters */
  assert_param(IS_I2C_ALL_PERIPH(I2Cx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  I2C_TypeDef _I2Cx;
  _I2Cx.TIMINGR = I2Cx->TIMINGR;
  _I2Cx.OAR2 = I2Cx->OAR2;
  _I2Cx.OAR1 = I2Cx->OAR1;
  _I2Cx.ICR = I2Cx->ICR;
  _I2Cx.CR2 = I2Cx->CR2;
  _I2Cx.CR1 = I2Cx->CR1;

  /* Enable the selected I2C peripheral */
  __HAL_I2C_DISABLE(hi2c);
  /* Enable the selected I2C peripheral */
  __HAL_I2C_ENABLE(hi2c);

  I2Cx->CR1 = _I2Cx.CR1;
  I2Cx->CR2 = _I2Cx.CR2;
  I2Cx->ICR = _I2Cx.ICR;
  I2Cx->OAR1 = _I2Cx.OAR1;
  I2Cx->OAR2 = _I2Cx.OAR2;
  I2Cx->TIMINGR = _I2Cx.TIMINGR;

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
/*unsigned long TWI_MasterWriteRead(new_twi* TwiStruct, unsigned int TransmitBytes, unsigned int _Size)
{
 	 I2C_HandleTypeDef *hi2c = (I2C_HandleTypeDef *)TwiStruct->udata;

}*/
//#####################################################
/**
  * @brief  Initializes peripherals used by the I2C EEPROM driver.
  * @param  None
  * @retval None
  */
bool TWI_open(new_twi* TwiStruct)
{
	if(!TwiStruct)
		return false;
	  I2C_HandleTypeDef *I2cHandle = calloc(1, sizeof(I2C_HandleTypeDef));
	  if(!I2cHandle)
		  return false;
	   //I2C_InitTypeDef  I2C_InitStructure;
	  TwiStruct->udata = (void *)I2cHandle;
	  sEE_LowLevel_Init(TwiStruct);
/*##-1- Configure the I2C peripheral #######################################*/
	  I2cHandle->Instance             = sEE_I2C[TwiStruct->TwiNr];

	  I2cHandle->Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
	  I2cHandle->Init.Timing      		= I2C_TIMING;
	  I2cHandle->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	  //I2cHandle->Init.DutyCycle       = I2C_DUTYCYCLE_2;
	  I2cHandle->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	  I2cHandle->Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
	  I2cHandle->Init.OwnAddress1     = TwiStruct->MasterSlaveAddr;
	  //I2cHandle.Init.OwnAddress2     = 0xFF;

	  if(HAL_I2C_Init(I2cHandle) != HAL_OK)
	  {
		  /* Initialization Error */
		  return false;
	  }
	  return true;
}
/**
  * @brief  I2C Tx data register flush process.
  * @param  hi2c: I2C handle.
  * @retval None
  */
static void I2C_Flush_TXDR(I2C_HandleTypeDef *hi2c)
{
  /* If a pending TXIS flag is set */
  /* Write a dummy data in TXDR to clear it */
  if(__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_TXIS) != RESET)
  {
     hi2c->Instance->TXDR = 0x00;
  }

  /* Flush TX register if not empty */
  if(__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_TXE) == RESET)
  {
    __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_TXE);
  }
}
/**
  * @brief  This function handles Acknowledge failed detection during an I2C Communication.
  * @param  hi2c: Pointer to a I2C_HandleTypeDef structure that contains
  *               the configuration information for the specified I2C.
  * @param  Timeout: Timeout duration
  * @param  Tickstart: Tick start value
  * @retval HAL status
  */
static HAL_StatusTypeDef I2C_IsAcknowledgeFailed(I2C_HandleTypeDef *hi2c, uint32_t Timeout, uint32_t Tickstart)
{
  if(__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_AF) == SET)
  {
    /* Wait until STOP Flag is reset */
    /* AutoEnd should be initiate after AF */
    while(__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_STOPF) == RESET)
    {
      /* Check for the Timeout */
      if(Timeout != HAL_MAX_DELAY)
      {
      if((Timeout == 0)||((HAL_GetTick() - Tickstart) > Timeout))
        {
          hi2c->State= HAL_I2C_STATE_READY;
          hi2c->Mode = HAL_I2C_MODE_NONE;

          /* Process Unlocked */
          __HAL_UNLOCK(hi2c);
          return HAL_TIMEOUT;
        }
      }
    }

    /* Clear NACKF Flag */
    __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_AF);

    /* Clear STOP Flag */
    __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_STOPF);

    /* Flush TX register */
    I2C_Flush_TXDR(hi2c);

    /* Clear Configuration Register 2 */
    I2C_RESET_CR2(hi2c);

    hi2c->ErrorCode = HAL_I2C_ERROR_AF;
    hi2c->State= HAL_I2C_STATE_READY;
    hi2c->Mode = HAL_I2C_MODE_NONE;

    /* Process Unlocked */
    __HAL_UNLOCK(hi2c);

    return HAL_ERROR;
  }
  return HAL_OK;
}
/**
  * @brief  This function handles I2C Communication Timeout for specific usage of STOP flag.
  * @param  hi2c: Pointer to a I2C_HandleTypeDef structure that contains
  *               the configuration information for the specified I2C.
  * @param  Timeout: Timeout duration
  * @param  Tickstart: Tick start value
  * @retval HAL status
  */
static HAL_StatusTypeDef I2C_WaitOnSTOPFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Timeout, uint32_t Tickstart)
{
  while(__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_STOPF) == RESET)
  {
    /* Check if a NACK is detected */
    if(I2C_IsAcknowledgeFailed(hi2c, Timeout, Tickstart) != HAL_OK)
    {
      return HAL_ERROR;
    }

    /* Check for the Timeout */
    if((Timeout == 0)||((HAL_GetTick() - Tickstart) > Timeout))
    {
      hi2c->ErrorCode |= HAL_I2C_ERROR_TIMEOUT;
      hi2c->State= HAL_I2C_STATE_READY;
      hi2c->Mode = HAL_I2C_MODE_NONE;

      /* Process Unlocked */
      __HAL_UNLOCK(hi2c);

      return HAL_TIMEOUT;
    }
  }
  return HAL_OK;
}
/**
  * @brief  Handles I2Cx communication when starting transfer or during transfer (TC or TCR flag are set).
  * @param  hi2c: I2C handle.
  * @param  DevAddress: Specifies the slave address to be programmed.
  * @param  Size: Specifies the number of bytes to be programmed.
  *   This parameter must be a value between 0 and 255.
  * @param  Mode: New state of the I2C START condition generation.
  *   This parameter can be a value of @ref I2C_RELOAD_END_MODE.
  * @param  Request: New state of the I2C START condition generation.
  *   This parameter can be a value of I2C_START_STOP_MODE.
  * @retval None
  */
static void I2C_TransferConfig(I2C_HandleTypeDef *hi2c,  uint16_t DevAddress, uint8_t Size, uint32_t Mode, uint32_t Request)
{
  uint32_t tmpreg = 0;

  /* Check the parameters */
  assert_param(IS_I2C_ALL_INSTANCE(hi2c->Instance));
  assert_param(IS_TRANSFER_MODE(Mode));
  assert_param(IS_TRANSFER_REQUEST(Request));

  /* Get the CR2 register value */
  tmpreg = hi2c->Instance->CR2;

  /* clear tmpreg specific bits */
  tmpreg &= (uint32_t)~((uint32_t)(I2C_CR2_SADD | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_AUTOEND | I2C_CR2_RD_WRN | I2C_CR2_START | I2C_CR2_STOP));

  /* update tmpreg */
  tmpreg |= (uint32_t)(((uint32_t)DevAddress & I2C_CR2_SADD) | (((uint32_t)Size << 16 ) & I2C_CR2_NBYTES) | \
            (uint32_t)Mode | (uint32_t)Request);

  /* update CR2 register */
  hi2c->Instance->CR2 = tmpreg;
}
/**
  * @brief  This function handles I2C Communication Timeout.
  * @param  hi2c: Pointer to a I2C_HandleTypeDef structure that contains
  *               the configuration information for the specified I2C.
  * @param  Flag: Specifies the I2C flag to check.
  * @param  Status: The new Flag status (SET or RESET).
  * @param  Timeout: Timeout duration
  * @param  Tickstart: Tick start value
  * @retval HAL status
  */
static HAL_StatusTypeDef I2C_WaitOnFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Flag, FlagStatus Status, uint32_t Timeout, uint32_t Tickstart)
{
  while((__HAL_I2C_GET_FLAG(hi2c, Flag) ? SET : RESET) == Status)
  {
    /* Check for the Timeout */
    if(Timeout != HAL_MAX_DELAY)
    {
      if((Timeout == 0)||((HAL_GetTick() - Tickstart ) > Timeout))
      {
        hi2c->State= HAL_I2C_STATE_READY;
        hi2c->Mode = HAL_I2C_MODE_NONE;

        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);
        return HAL_TIMEOUT;
      }
    }
  }
  return HAL_OK;
}
/**
  * @brief  This function handles I2C Communication Timeout for specific usage of TXIS flag.
  * @param  hi2c: Pointer to a I2C_HandleTypeDef structure that contains
  *               the configuration information for the specified I2C.
  * @param  Timeout: Timeout duration
  * @param  Tickstart: Tick start value
  * @retval HAL status
  */
static HAL_StatusTypeDef I2C_WaitOnTXISFlagUntilTimeout(I2C_HandleTypeDef *hi2c, uint32_t Timeout, uint32_t Tickstart)
{
  while(__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_TXIS) == RESET)
  {
    /* Check if a NACK is detected */
    if(I2C_IsAcknowledgeFailed(hi2c, Timeout, Tickstart) != HAL_OK)
    {
      return HAL_ERROR;
    }

    /* Check for the Timeout */
    if(Timeout != HAL_MAX_DELAY)
    {
      if((Timeout == 0)||((HAL_GetTick() - Tickstart) > Timeout))
      {
        hi2c->ErrorCode |= HAL_I2C_ERROR_TIMEOUT;
        hi2c->State= HAL_I2C_STATE_READY;
        hi2c->Mode = HAL_I2C_MODE_NONE;

        /* Process Unlocked */
        __HAL_UNLOCK(hi2c);

        return HAL_TIMEOUT;
      }
    }
  }
  return HAL_OK;
}
/*#####################################################*/
#define Timeout 10

unsigned long TWI_MasterWriteRead(new_twi* TwiStruct, unsigned int TransmitBytes, unsigned int ReceiveBytes)
{
	uint32_t tickstart = 0;
	I2C_HandleTypeDef *hi2c = (I2C_HandleTypeDef *)TwiStruct->udata;
    /* Init tickstart for timeout management*/
    tickstart = HAL_GetTick();

    if(I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_BUSY, SET, I2C_TIMEOUT_BUSY, tickstart) != HAL_OK)
    {
    	I2C_SoftwareResetCmd(hi2c);
      return HAL_TIMEOUT;
    }

    hi2c->State     = HAL_I2C_STATE_BUSY_TX;
    hi2c->Mode      = HAL_I2C_MODE_MASTER;
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;

    /* Prepare transfer parameters */
    hi2c->pBuffPtr  = TwiStruct->TxBuff;
    hi2c->XferCount = TransmitBytes;
    hi2c->XferISR   = NULL;

    /* Set NBYTES to write and reload if hi2c->XferCount > MAX_NBYTE_SIZE */
    if(hi2c->XferCount > MAX_NBYTE_SIZE)
    {
    	hi2c->XferSize = MAX_NBYTE_SIZE;
    	I2C_TransferConfig(hi2c, TwiStruct->MasterSlaveAddr, hi2c->XferSize, I2C_RELOAD_MODE, I2C_GENERATE_START_WRITE);
    }
    else
    {
		if(ReceiveBytes != 0)
		{
			hi2c->XferSize = hi2c->XferCount;
			I2C_TransferConfig(hi2c, TwiStruct->MasterSlaveAddr, hi2c->XferSize, I2C_SOFTEND_MODE, I2C_GENERATE_START_WRITE);
		}
		else
		{
			hi2c->XferSize = hi2c->XferCount;
			I2C_TransferConfig(hi2c, TwiStruct->MasterSlaveAddr, hi2c->XferSize, I2C_AUTOEND_MODE, I2C_GENERATE_START_WRITE);
		}
    }

	do
	{
	    tickstart = HAL_GetTick();
	      /* Wait until TXIS flag is set */
	      if(I2C_WaitOnTXISFlagUntilTimeout(hi2c, Timeout, tickstart) != HAL_OK)
	      {
	        if(hi2c->ErrorCode == HAL_I2C_ERROR_AF)
	        {
	          return HAL_ERROR;
	        }
	        else
	        {
	          return HAL_TIMEOUT;
	        }
	      }

		/* Read data from RXDR */
	    hi2c->Instance->TXDR = (*hi2c->pBuffPtr++);
	    hi2c->XferCount--;
	    hi2c->XferSize--;

		if((hi2c->XferSize == 0) && (hi2c->XferCount != 0))
		{
			/* Wait until TCR flag is set */
			if(I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_TCR, RESET, Timeout, tickstart) != HAL_OK)
			{
				return HAL_TIMEOUT;
			}

			if(hi2c->XferCount > MAX_NBYTE_SIZE)
			{
				hi2c->XferSize = MAX_NBYTE_SIZE;
				I2C_TransferConfig(hi2c, TwiStruct->MasterSlaveAddr, hi2c->XferSize, I2C_RELOAD_MODE, I2C_NO_STARTSTOP);
			}
			else
			{
				if(ReceiveBytes != 0)
				{
					hi2c->XferSize = hi2c->XferCount;
					I2C_TransferConfig(hi2c, TwiStruct->MasterSlaveAddr, hi2c->XferSize, I2C_SOFTEND_MODE, I2C_NO_STARTSTOP);
				}
				else
				{
					hi2c->XferSize = hi2c->XferCount;
					I2C_TransferConfig(hi2c, TwiStruct->MasterSlaveAddr, hi2c->XferSize, I2C_AUTOEND_MODE, I2C_NO_STARTSTOP);
				}
			}
		}
	}while(hi2c->XferCount > 0);
	if(ReceiveBytes == 0)
	{
		/* No need to Check TC flag, with AUTOEND mode the stop is automatically generated */
		/* Wait until STOPF flag is reset */
		if(I2C_WaitOnSTOPFlagUntilTimeout(hi2c, Timeout, tickstart) != HAL_OK)
		{
			if(hi2c->ErrorCode == HAL_I2C_ERROR_AF)
			{
				return HAL_ERROR;
			}
			else
			{
				return HAL_TIMEOUT;
			}
		}
		/* Clear STOP Flag */
		__HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_STOPF);
		/* Clear Configuration Register 2 */
		I2C_RESET_CR2(hi2c);
		hi2c->State = HAL_I2C_STATE_READY;
		hi2c->Mode  = HAL_I2C_MODE_NONE;
		/* Process Unlocked */
		__HAL_UNLOCK(hi2c);
		return HAL_OK;
	}

    tickstart = HAL_GetTick();

    /* Wait until TCR flag is set */
    if(I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_TC, RESET, Timeout, tickstart) != HAL_OK)
    {
      return HAL_TIMEOUT;
    }
    hi2c->State     = HAL_I2C_STATE_BUSY_RX;
    hi2c->Mode      = HAL_I2C_MODE_MASTER;
    hi2c->ErrorCode = HAL_I2C_ERROR_NONE;

    /* Prepare transfer parameters */
    hi2c->pBuffPtr  = TwiStruct->RxBuff;
    hi2c->XferCount = ReceiveBytes;
    hi2c->XferISR   = NULL;

    /* Set NBYTES to write and reload if hi2c->XferCount > MAX_NBYTE_SIZE */
    if(hi2c->XferCount > MAX_NBYTE_SIZE)
    {
    	hi2c->XferSize = MAX_NBYTE_SIZE;
    	I2C_TransferConfig(hi2c, TwiStruct->MasterSlaveAddr | 1, hi2c->XferSize, I2C_RELOAD_MODE, I2C_GENERATE_START_READ);
    }
    else
    {
    	hi2c->XferSize = hi2c->XferCount;
    	I2C_TransferConfig(hi2c, TwiStruct->MasterSlaveAddr | 1, hi2c->XferSize, I2C_AUTOEND_MODE, I2C_GENERATE_START_READ);
    }

	do
	{
	    tickstart = HAL_GetTick();
		/* Wait until RXNE flag is set */
		if(I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_RXNE, RESET, Timeout, tickstart) != HAL_OK)
		{
			return HAL_TIMEOUT;
		}

		/* Read data from RXDR */
	    (*hi2c->pBuffPtr++) = hi2c->Instance->RXDR;
	    hi2c->XferSize--;
	    hi2c->XferCount--;

		if((hi2c->XferSize == 0) && (hi2c->XferCount != 0))
		{
			/* Wait until TCR flag is set */
			if(I2C_WaitOnFlagUntilTimeout(hi2c, I2C_FLAG_TCR, RESET, Timeout, tickstart) != HAL_OK)
			{
				return HAL_TIMEOUT;
			}

			if(hi2c->XferCount > MAX_NBYTE_SIZE)
			{
				hi2c->XferSize = MAX_NBYTE_SIZE;
				I2C_TransferConfig(hi2c, TwiStruct->MasterSlaveAddr | 1, hi2c->XferSize, I2C_RELOAD_MODE, I2C_NO_STARTSTOP);
			}
			else
			{
				hi2c->XferSize = hi2c->XferCount;
				I2C_TransferConfig(hi2c, TwiStruct->MasterSlaveAddr | 1, hi2c->XferSize, I2C_AUTOEND_MODE, I2C_NO_STARTSTOP);
			}
		}
	}while(hi2c->XferCount > 0);
	/* No need to Check TC flag, with AUTOEND mode the stop is automatically generated */
	/* Wait until STOPF flag is reset */
	if(I2C_WaitOnSTOPFlagUntilTimeout(hi2c, Timeout, tickstart) != HAL_OK)
	{
		if(hi2c->ErrorCode == HAL_I2C_ERROR_AF)
		{
			return HAL_ERROR;
		}
		else
		{
			return HAL_TIMEOUT;
		}
	}

	/* Clear STOP Flag */
	__HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_STOPF);

	/* Clear Configuration Register 2 */
	I2C_RESET_CR2(hi2c);

	hi2c->State = HAL_I2C_STATE_READY;
	hi2c->Mode  = HAL_I2C_MODE_NONE;

	/* Process Unlocked */
	__HAL_UNLOCK(hi2c);

	return HAL_OK;
}


bool _I2C_trx(struct Twi_s* param, unsigned char addr, unsigned char *buff_send, unsigned int bytes_send, unsigned char *buff_receive, unsigned int bytes_receive)
{
	if(!param)
		return false;
#if (USE_DRIVER_SEMAPHORE == true)
	while(twi_semaphore[param->TwiNr]);
	twi_semaphore[param->TwiNr] = true;
#endif
	param->MasterSlaveAddr = addr << 1;
	param->TxBuff = buff_send;
	param->RxBuff = buff_receive;
	bool result = TWI_MasterWriteRead(param, bytes_send, bytes_receive);

#if (USE_DRIVER_SEMAPHORE == true)
	twi_semaphore[param->TwiNr] = false;
#endif
	if(result == HAL_OK)
		return true;
	else
		return false;
}
/*#####################################################*/
bool _I2C_tx(struct Twi_s* param, unsigned char addr, unsigned char *buff_send, unsigned int bytes_send)
{
	if(!param)
		return false;
#if (USE_DRIVER_SEMAPHORE == true)
	while(twi_semaphore[param->TwiNr]);
	twi_semaphore[param->TwiNr] = true;
#endif
	param->MasterSlaveAddr = addr << 1;
	param->TxBuff = buff_send;
	bool result = TWI_MasterWriteRead(param, bytes_send, 0);
#if (USE_DRIVER_SEMAPHORE == true)
	twi_semaphore[param->TwiNr] = false;
#endif
	if(result == HAL_OK)
		return true;
	else
		return false;
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
