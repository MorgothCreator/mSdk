/*
 * aintc_twi.c
 *
 *  Created on: Dec 3, 2012
 *      Author: XxXx
 */
/*#####################################################*/
#include <stdbool.h>
#include "aintc_twi.h"
#include "api/twi_def.h"
#include "../include/hsi2c.h"
#include "../include/interrupt.h"
/*#####################################################*/
Twi_Int_Service_t Twi_Int_Service;

/*#####################################################*/
/*
** I2C Interrupt Service Routine. This function will read and write
** data through I2C bus.
*/
void I2CIsr(new_twi* TwiStruct)
{
    unsigned int status = 0;

    /* Get only Enabled interrupt status */
    status = I2CMasterIntStatus(TwiStruct->BaseAddr);

    /*
    ** Clear all enabled interrupt status except receive ready and
    ** transmit ready interrupt status
    */
    I2CMasterIntClearEx(TwiStruct->BaseAddr,
	                    (status & ~(I2C_INT_RECV_READY | I2C_INT_TRANSMIT_READY | I2C_INT_NO_ACK)));

    if(status & I2C_INT_RECV_READY)
    {
         /* Receive data from data receive register */
    	TwiStruct->RxBuff[TwiStruct->rCount++] = I2CMasterDataGet(TwiStruct->BaseAddr);

	/* Clear receive ready interrupt status */
        I2CMasterIntClearEx(TwiStruct->BaseAddr,  I2C_INT_RECV_READY);

         if(TwiStruct->rCount == TwiStruct->numOfBytes)
         {
              /* Disable the receive ready interrupt */
              I2CMasterIntDisableEx(TwiStruct->BaseAddr, I2C_INT_RECV_READY);
              /* Generate a STOP */
              I2CMasterStop(TwiStruct->BaseAddr);

         }
    }

    if (status & I2C_INT_TRANSMIT_READY)
    {
        /* Put data to data transmit register of i2c */
        I2CMasterDataPut(TwiStruct->BaseAddr, TwiStruct->TxBuff[TwiStruct->tCount++]);

        /* Clear Transmit interrupt status */
	I2CMasterIntClearEx(TwiStruct->BaseAddr, I2C_INT_TRANSMIT_READY);

         if(TwiStruct->tCount == TwiStruct->numOfBytes)
         {
              /* Disable the transmit ready interrupt */
              I2CMasterIntDisableEx(TwiStruct->BaseAddr, I2C_INT_TRANSMIT_READY);
         }

    }

    if (status & I2C_INT_STOP_CONDITION)
    {
      	 /* Disable transmit data ready and receive data read interupt */
         I2CMasterIntDisableEx(TwiStruct->BaseAddr, I2C_INT_TRANSMIT_READY |
                                               I2C_INT_RECV_READY     |
					       I2C_INT_STOP_CONDITION);
         TwiStruct->flag = 0;
    }

    if(status & I2C_INT_NO_ACK)
    {
         I2CMasterIntDisableEx(TwiStruct->BaseAddr, I2C_INT_TRANSMIT_READY  |
                                               I2C_INT_RECV_READY      |
                                               I2C_INT_NO_ACK          |
                                               I2C_INT_STOP_CONDITION);
         /* Generate a STOP */
         I2CMasterStop(TwiStruct->BaseAddr);

         TwiStruct->flag = 0;
         TwiStruct->error_flag = 1;
    }

    //I2CEndOfInterrupt(TwiStruct->BaseAddr, 0);
}
/*#####################################################*/
void I2C0Isr(void)
{
	I2CIsr(Twi_Int_Service.Twi0);
}
/*#####################################################*/
void I2C1Isr(void)
{
	I2CIsr(Twi_Int_Service.Twi1);
}
/*#####################################################*/
void I2C2Isr(void)
{
	I2CIsr(Twi_Int_Service.Twi2);
}
/*#####################################################*/
/* Configures AINTC to generate interrupt */
void I2CAINTCConfigure(new_twi* TwiStruct)
{
    /* Intialize the ARM Interrupt Controller(AINTC) */
    //IntAINTCInit();

    switch (TwiStruct->TwiNr)
    {
    case 0:
    	IntProtectionDisable();
        /* Registering the Interrupt Service Routine(ISR). */
        IntRegister(SYS_INT_I2C0INT, I2C0Isr);

        /* Setting the priority for the system interrupt in AINTC. */
        IntPrioritySet(SYS_INT_I2C0INT, TwiStruct->Priority, AINTC_HOSTINT_ROUTE_IRQ );

        /* Enabling the system interrupt in AINTC. */
        IntSystemEnable(SYS_INT_I2C0INT);
        IntProtectionEnable();
    	break;
    case 1:
    	IntProtectionDisable();
        /* Registering the Interrupt Service Routine(ISR). */
        IntRegister(SYS_INT_I2C1INT, I2C1Isr);

        /* Setting the priority for the system interrupt in AINTC. */
        IntPrioritySet(SYS_INT_I2C1INT, TwiStruct->Priority, AINTC_HOSTINT_ROUTE_IRQ );

        /* Enabling the system interrupt in AINTC. */
        IntSystemEnable(SYS_INT_I2C1INT);
        IntProtectionEnable();
    	break;
    case 2:
    	IntProtectionDisable();
        /* Registering the Interrupt Service Routine(ISR). */
        IntRegister(SYS_INT_I2C2INT, I2C2Isr);

        /* Setting the priority for the system interrupt in AINTC. */
        IntPrioritySet(SYS_INT_I2C2INT, TwiStruct->Priority, AINTC_HOSTINT_ROUTE_IRQ );

        /* Enabling the system interrupt in AINTC. */
        IntSystemEnable(SYS_INT_I2C2INT);
        IntProtectionEnable();
    	break;
    }
}
/*#####################################################*/
/* Configures AINTC to generate interrupt */
void I2CAINTCUnConfigure(new_twi* TwiStruct)
{
    /* Intialize the ARM Interrupt Controller(AINTC) */
    //IntAINTCInit();

    switch (TwiStruct->TwiNr)
    {
    case 0:
    	IntProtectionDisable();
        /* Enabling the system interrupt in AINTC. */
        IntSystemDisable(SYS_INT_I2C0INT);

        /* Registering the Interrupt Service Routine(ISR). */
        IntUnRegister(SYS_INT_I2C0INT);
        IntProtectionEnable();
    	break;
    case 1:
    	IntProtectionDisable();
        /* Enabling the system interrupt in AINTC. */
    	IntSystemDisable(SYS_INT_I2C1INT);

    	/* Registering the Interrupt Service Routine(ISR). */
    	IntUnRegister(SYS_INT_I2C1INT);
        IntProtectionEnable();
    	break;
    case 2:
    	IntProtectionDisable();
        /* Enabling the system interrupt in AINTC. */
    	IntSystemDisable(SYS_INT_I2C2INT);

    	/* Registering the Interrupt Service Routine(ISR). */
    	IntUnRegister(SYS_INT_I2C2INT);
        IntProtectionEnable();
    	break;
    }
}
/*#####################################################*/
/* Clear status of all interrupts */
void CleanUpInterrupts(new_twi* TwiStruct)
{
    if(TwiStruct->WithInterrupt)I2CMasterIntEnableEx(TwiStruct->BaseAddr, 0x7FF);
    I2CMasterIntClearEx(TwiStruct->BaseAddr,  0x7FF);
    if(TwiStruct->WithInterrupt)I2CMasterIntDisableEx(TwiStruct->BaseAddr, 0x7FF);
}
/*#####################################################*/

