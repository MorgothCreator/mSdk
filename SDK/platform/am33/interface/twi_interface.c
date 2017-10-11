/*
 * twi_interface.c
 *
 *  Created on: Dec 3, 2012
 *      Author: XxXx
 */
/*#####################################################*/
#include "stdbool.h"
#include "stdlib.h"
#include "string.h"
#include "twi_interface.h"
#include "api/twi_def.h"
#include "api/timer_api.h"
#include "aintc/aintc_twi.h"
#include "sys/twi_definition.h"
#include "pinmux/pin_mux_twi.h"
#include "clk/clk_twi.h"
#include "include/hsi2c.h"
/*#####################################################*/
/*
** Transmits data over I2C bus
*/
bool _SetupI2CTransmit(new_twi* TwiStruct, unsigned int TransmitBytes)
{
	if(!TwiStruct) return false;
	/* Set i2c slave address */
	timer(TimerBusyTimeout);
	timer_interval(&TimerBusyTimeout, TwiStruct->BusyTimeOut);
	timer_enable(&TimerBusyTimeout);
    while(I2CMasterBusy(TwiStruct->BaseAddr)) {
    	  if(timer_tick(&TimerBusyTimeout)) {
    			break;
    		}
      }
	I2CMasterSlaveAddrSet(TwiStruct->BaseAddr, TwiStruct->MasterSlaveAddr);

    TwiStruct->tCount = 0;

    if(TwiStruct->UseInterrupt)
    {
        /* Data Count specifies the number of bytes to be transmitted */
        I2CSetDataCount(TwiStruct->BaseAddr, TransmitBytes);

        TwiStruct->numOfBytes = I2CDataCountGet(TwiStruct->BaseAddr);

        /* Clear status of all interrupts */
        CleanUpInterrupts(TwiStruct);

        /* Configure I2C controller in Master Transmitter mode */
        I2CMasterControl(TwiStruct->BaseAddr, I2C_CFG_MST_TX | I2C_CFG_STOP);

        /* Transmit interrupt is enabled */
        I2CMasterIntEnableEx(TwiStruct->BaseAddr, I2C_INT_TRANSMIT_READY | I2C_INT_STOP_CONDITION | I2C_INT_NO_ACK);

        TwiStruct->error_flag = 0;

        /* Generate Start Condition over I2C bus */
        I2CMasterStart(TwiStruct->BaseAddr);

	    timer_enable(&TimerBusyTimeout);
        while(I2CMasterBusBusy(TwiStruct->BaseAddr) == 0) {
      	  if(timer_tick(&TimerBusyTimeout)) {
      			return false;
      		}
        }

	    timer_enable(&TimerBusyTimeout);
        while(TwiStruct->flag) {
        	  if(timer_tick(&TimerBusyTimeout)) {
        			return false;
        		}
        }

        if(TwiStruct->error_flag) return false;

	    timer_enable(&TimerBusyTimeout);
        while(I2CMasterBusy(TwiStruct->BaseAddr)) {
        	  if(timer_tick(&TimerBusyTimeout)) {
        			return false;
        		}
        }

        /* Wait untill I2C registers are ready to access */
	    timer_enable(&TimerBusyTimeout);
        while(0 == (I2CMasterIntRawStatus(TwiStruct->BaseAddr) & I2C_INT_ADRR_READY_ACESS)) {
        	  if(timer_tick(&TimerBusyTimeout)) {
        			return false;
        		}
        }

        TwiStruct->flag = 1;
		return true;
    }
    else
    {
		I2CSetDataCount(TwiStruct->BaseAddr, TransmitBytes);

		CleanUpInterrupts(TwiStruct);

		I2CMasterControl(TwiStruct->BaseAddr, I2C_CFG_MST_TX);

        I2CMasterStart(TwiStruct->BaseAddr);

	    timer_enable(&TimerBusyTimeout);
		while(I2CMasterBusBusy(TwiStruct->BaseAddr) == 0) {
	      	  if(timer_tick(&TimerBusyTimeout)) {
	      			return false;
	      		}
		}

	    timer_enable(&TimerBusyTimeout);
		while((I2C_INT_TRANSMIT_READY == (I2CMasterIntRawStatus(TwiStruct->BaseAddr) & I2C_INT_TRANSMIT_READY))
			   && TransmitBytes--)
		{
	        I2CMasterDataPut(TwiStruct->BaseAddr, TwiStruct->TxBuff[TwiStruct->tCount++]);

			I2CMasterIntClearEx(TwiStruct->BaseAddr, I2C_INT_TRANSMIT_READY);
	      	  if(timer_tick(&TimerBusyTimeout)) {
	      			return false;
	      		}
		}

		I2CMasterStop(TwiStruct->BaseAddr);

	    timer_enable(&TimerBusyTimeout);
		while(0 == (I2CMasterIntRawStatus(TwiStruct->BaseAddr) & I2C_INT_STOP_CONDITION)) {
	      	  if(timer_tick(&TimerBusyTimeout)) {
	      			return false;
	      		}

		}

		I2CMasterIntClearEx(TwiStruct->BaseAddr, I2C_INT_STOP_CONDITION);
		return true;
    }
}
/*#####################################################*/
/*
** Receives data over I2C bus
*/
bool _SetupI2CReception(new_twi* TwiStruct, unsigned int TransmitBytes, unsigned int ReceiveBytes)
{
    if(!TwiStruct) return false;
	/* Set i2c slave address */

    timer(TimerBusyTimeout);
    timer_interval(&TimerBusyTimeout, TwiStruct->BusyTimeOut);
    timer_enable(&TimerBusyTimeout);
    while(I2CMasterBusy(TwiStruct->BaseAddr))
      {
    	  if(timer_tick(&TimerBusyTimeout)) {
    			/*
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
    			*/
    			break;
    		}
      }
    I2CMasterSlaveAddrSet(TwiStruct->BaseAddr, TwiStruct->MasterSlaveAddr);
    TwiStruct->tCount = 0;
    TwiStruct->rCount = 0;

    if(TwiStruct->UseInterrupt)
    {
		TwiStruct->error_flag = 0;
		/* Data Count specifies the number of bytes to be transmitted */
		I2CSetDataCount(TwiStruct->BaseAddr, TransmitBytes);

		TwiStruct->numOfBytes = I2CDataCountGet(TwiStruct->BaseAddr);

		/* Clear status of all interrupts */
		CleanUpInterrupts(TwiStruct);

		/* Configure I2C controller in Master Transmitter mode */
		I2CMasterControl(TwiStruct->BaseAddr, I2C_CFG_MST_TX);

		/* Transmit interrupt is enabled */
		I2CMasterIntEnableEx(TwiStruct->BaseAddr, I2C_INT_TRANSMIT_READY | I2C_INT_NO_ACK);


		/* Generate Start Condition over I2C bus */
		I2CMasterStart(TwiStruct->BaseAddr);

		//do
		//{
		//	if(TwiStruct->error_flag) return false;
		//}
	    timer_enable(&TimerBusyTimeout);
		while(I2CMasterBusBusy(TwiStruct->BaseAddr) == 0) {
	      	  if(timer_tick(&TimerBusyTimeout)) {
	      			return false;
	      		}
		}

	    timer_enable(&TimerBusyTimeout);
		while(TwiStruct->tCount != TwiStruct->numOfBytes) {
	      	  if(timer_tick(&TimerBusyTimeout)) {
	      			return false;
	      		}
		}


		TwiStruct->flag = 1;

		/* Wait untill I2C registers are ready to access */
	    timer_enable(&TimerBusyTimeout);
		while(!(I2CMasterIntRawStatus(TwiStruct->BaseAddr) & (I2C_INT_ADRR_READY_ACESS))) {
	      	  if(timer_tick(&TimerBusyTimeout)) {
	      			return false;
	      	  }
		}

		/* Data Count specifies the number of bytes to be received */
		I2CSetDataCount(TwiStruct->BaseAddr, ReceiveBytes);

		TwiStruct->numOfBytes = I2CDataCountGet(TwiStruct->BaseAddr);

		CleanUpInterrupts(TwiStruct);

		/* Configure I2C controller in Master Receiver mode */
		I2CMasterControl(TwiStruct->BaseAddr, I2C_CFG_MST_RX);

		/* Receive and Stop Condition Interrupts are enabled */
		I2CMasterIntEnableEx(TwiStruct->BaseAddr,  I2C_INT_RECV_READY |
											  I2C_INT_STOP_CONDITION | I2C_INT_NO_ACK);

		/* Generate Start Condition over I2C bus */
		I2CMasterStart(TwiStruct->BaseAddr);

	    timer_enable(&TimerBusyTimeout);
		do
		{
	      	  if(timer_tick(&TimerBusyTimeout)) {
	      			return false;
	      	  }
			if(TwiStruct->error_flag) return false;
		}
		while(I2CMasterBusBusy(TwiStruct->BaseAddr) == 0);

	    timer_enable(&TimerBusyTimeout);
		do
		{
	      	  if(timer_tick(&TimerBusyTimeout)) {
	      			return false;
	      	  }
			if(TwiStruct->error_flag) return false;
		}
		while(TwiStruct->flag);

		TwiStruct->flag = 1;

		return true;
    }
    else
    {
        I2CSetDataCount(TwiStruct->BaseAddr, TransmitBytes);

        CleanUpInterrupts(TwiStruct);

        I2CMasterControl(TwiStruct->BaseAddr, I2C_CFG_MST_TX);

		I2CMasterStart(TwiStruct->BaseAddr);

	    timer_enable(&TimerBusyTimeout);
        while(I2CMasterBusBusy(TwiStruct->BaseAddr) == 0) {
	      	  if(timer_tick(&TimerBusyTimeout)) {
	      			return false;
	      	  }
        }

        timer_enable(&TimerBusyTimeout);
        while((I2C_INT_TRANSMIT_READY == (I2CMasterIntRawStatus(TwiStruct->BaseAddr) & I2C_INT_TRANSMIT_READY))
               && TransmitBytes--)
        {
        	I2CMasterDataPut(TwiStruct->BaseAddr, TwiStruct->TxBuff[TwiStruct->tCount++]);

            I2CMasterIntClearEx(TwiStruct->BaseAddr, I2C_INT_TRANSMIT_READY);
	      	  if(timer_tick(&TimerBusyTimeout)) {
	      			return false;
	      	  }
        }
        timer_enable(&TimerBusyTimeout);
        while(0 == (I2CMasterIntRawStatus(TwiStruct->BaseAddr) & I2C_INT_ADRR_READY_ACESS)) {
	      	  if(timer_tick(&TimerBusyTimeout)) {
	      			return false;
	      	  }
        }

        I2CSetDataCount(TwiStruct->BaseAddr, ReceiveBytes);

        CleanUpInterrupts(TwiStruct);

        I2CMasterControl(TwiStruct->BaseAddr, I2C_CFG_MST_RX);

		I2CMasterStart(TwiStruct->BaseAddr);

	    timer_enable(&TimerBusyTimeout);
        while(I2CMasterBusBusy(TwiStruct->BaseAddr) == 0) {
	      	  if(timer_tick(&TimerBusyTimeout)) {
	      			return false;
	      	  }
        }

        while((ReceiveBytes--))
        {
            timer_enable(&TimerBusyTimeout);
            while(0 == (I2CMasterIntRawStatus(TwiStruct->BaseAddr) & I2C_INT_RECV_READY)) {
  	      	  if(timer_tick(&TimerBusyTimeout)) {
  	      			return false;
  	      	  }
            }

            TwiStruct->RxBuff[TwiStruct->rCount++] = I2CMasterDataGet(TwiStruct->BaseAddr);

            I2CMasterIntClearEx(TwiStruct->BaseAddr, I2C_INT_RECV_READY);
        }

        I2CMasterStop(TwiStruct->BaseAddr);

        timer_enable(&TimerBusyTimeout);
		while(0 == (I2CMasterIntRawStatus(TwiStruct->BaseAddr) & I2C_INT_STOP_CONDITION)) {
	      	  if(timer_tick(&TimerBusyTimeout)) {
	      			return false;
	      	  }
		}

        I2CMasterIntClearEx(TwiStruct->BaseAddr, I2C_INT_STOP_CONDITION);

		return true;
    }
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
    bool result = _SetupI2CReception(param, bytes_send, bytes_receive);

#if (USE_DRIVER_SEMAPHORE == true)
    twi_semaphore[param->TwiNr] = false;
#endif
     return result;
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
    bool result = _SetupI2CReception(param, bytes_send, 0);
#if (USE_DRIVER_SEMAPHORE == true)
    twi_semaphore[param->TwiNr] = false;
#endif
    return result;
}


/*#####################################################*/
bool _twi_open(new_twi* TwiStruct)
{
	unsigned char *RxBuff;
	unsigned char *TxBuff;
	if(TwiStruct->RxBuffSize)
	{
		RxBuff = (unsigned char *)calloc(1, TwiStruct->RxBuffSize);
		if(RxBuff == NULL) return 0;
		TwiStruct->RxBuff = RxBuff;
	}
	if(TwiStruct->TxBuffSize)
	{
		TxBuff = (unsigned char *)calloc(1, TwiStruct->TxBuffSize);
		if(TxBuff == NULL ) return 0;
		TwiStruct->TxBuff = TxBuff;
	}
    //new_twi* TwiStruct = new_(new_twi);
	TWIModuleClkConfig(TwiStruct);
    //if(TwiStruct == NULL) return NULL;
    if(TwiStruct->UseInterrupt)I2CAINTCConfigure(TwiStruct);
    switch(TwiStruct->TwiNr)
    {
    case 0:
		TwiStruct->BaseAddr = I2C0_addr;
		Twi_Int_Service.Twi0 = TwiStruct;
		pin_mux_twi0_sda(TwiStruct->SdaPin);
		pin_mux_twi0_scl(TwiStruct->SclPin);
    	break;
    case 1:
		TwiStruct->BaseAddr = I2C1_addr;
		Twi_Int_Service.Twi1 = TwiStruct;
		pin_mux_twi1_sda(TwiStruct->SdaPin);
		pin_mux_twi1_scl(TwiStruct->SclPin);
    	break;
    case 2:
		TwiStruct->BaseAddr = I2C2_addr;
		Twi_Int_Service.Twi2 = TwiStruct;
		pin_mux_twi2_sda(TwiStruct->SdaPin);
		pin_mux_twi2_scl(TwiStruct->SclPin);
    	break;
    }
    //I2CPinMuxSetup(TwiStruct->TwiNr);
    /* Put i2c in reset/disabled state */
    I2CMasterDisable(TwiStruct->BaseAddr);

    if(TwiStruct->UseInterrupt) I2CSoftReset(TwiStruct->BaseAddr);
    /* Disable auto Idle functionality */
    I2CAutoIdleDisable(TwiStruct->BaseAddr);

    /* Configure i2c bus speed to 100khz */
    I2CMasterInitExpClk(TwiStruct->BaseAddr, 48000000, 12000000, TwiStruct->BaudRate);

    /* Bring I2C out of reset */
    I2CMasterEnable(TwiStruct->BaseAddr);
    while(!I2CSystemStatusGet(TwiStruct->BaseAddr));
    return true;
}
/*#####################################################*/
void _twi_close(new_twi* TwiStruct)
{
    I2CMasterDisable(TwiStruct->BaseAddr);
    TWIModuleClkUnConfig(TwiStruct);
    I2CAINTCUnConfigure(TwiStruct);
    switch(TwiStruct->TwiNr)
    {
    case 0:
		Twi_Int_Service.Twi0 = NULL;
    	break;
    case 1:
		Twi_Int_Service.Twi1 = NULL;
    	break;
    case 2:
		Twi_Int_Service.Twi2 = NULL;
    	break;
    }
    if(TwiStruct->RxBuffSize) free((void*)TwiStruct->RxBuffSize);
    if(TwiStruct->TxBuffSize) free((void*)TwiStruct->TxBuffSize);
    if(TwiStruct) free(TwiStruct);
}
/*#####################################################*/


