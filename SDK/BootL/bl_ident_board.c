/*
 * bl_ident_board.c
 *
 *  Created on: Oct 28, 2014
 *      Author: Morgoth
 */
#ifdef __BL__
#include <string.h>
#include "hsi2c.h"
#include "soc_AM335x.h"
#include "hw_types.h"
#include "uartStdio.h"
#include "bl_platform.h"
#ifdef evmAM335x
    #include "hw_tps65910.h"
#elif  (defined beaglebone)
    #include "hw_tps65217.h"
#elif  (defined evmskAM335x)
    #include "hw_tps65910.h"
    #include "pin_mux.h"
#endif

extern void SetupI2CTransmit(unsigned int dcount);
extern void SetupReception(unsigned int dcount);
extern void CleanupInterrupts(void);

extern volatile unsigned char dataFromSlave[];
extern volatile unsigned char dataToSlave[];
extern volatile unsigned int tCount;
extern volatile unsigned int rCount;
//extern volatile unsigned int flag = 1;
//extern volatile unsigned int numOfBytes;


/*
 * \brief Receives data over I2C0 bus
 *
 * \param  dcount - Number of bytes to receive.
 *
 * \return none
 */

void EEP_SetupReception(unsigned int dcount)
{
    I2CSetDataCount(SOC_I2C_0_REGS, 2);

    CleanupInterrupts();

    I2CMasterControl(SOC_I2C_0_REGS, I2C_CFG_MST_TX);

    I2CMasterStart(SOC_I2C_0_REGS);

    while(I2CMasterBusBusy(SOC_I2C_0_REGS) == 0);

   int _dcount =2;

    while((I2C_INT_TRANSMIT_READY == (I2CMasterIntRawStatus(SOC_I2C_0_REGS)
                                     & I2C_INT_TRANSMIT_READY)) && _dcount--)
    {
        I2CMasterDataPut(SOC_I2C_0_REGS, dataToSlave[tCount++]);

        I2CMasterIntClearEx(SOC_I2C_0_REGS, I2C_INT_TRANSMIT_READY);
    }






   /* I2CMasterDataPut(SOC_I2C_0_REGS, dataToSlave[tCount]);

    I2CMasterIntClearEx(SOC_I2C_0_REGS, I2C_INT_TRANSMIT_READY);

    while(0 == (I2CMasterIntRawStatus(SOC_I2C_0_REGS) & I2C_INT_ADRR_READY_ACESS));

    I2CMasterDataPut(SOC_I2C_0_REGS, dataToSlave[tCount + 1]);

    I2CMasterIntClearEx(SOC_I2C_0_REGS, I2C_INT_TRANSMIT_READY);*/

    while(0 == (I2CMasterIntRawStatus(SOC_I2C_0_REGS) & I2C_INT_ADRR_READY_ACESS));


    I2CSetDataCount(SOC_I2C_0_REGS, dcount);

    CleanupInterrupts();

    I2CMasterControl(SOC_I2C_0_REGS, I2C_CFG_MST_RX);

    I2CMasterStart(SOC_I2C_0_REGS);

    /* Wait till the bus if free */
    while(I2CMasterBusBusy(SOC_I2C_0_REGS) == 0);

    /* Read the data from slave of dcount */
    while((dcount--))
    {
        while(0 == (I2CMasterIntRawStatus(SOC_I2C_0_REGS) & I2C_INT_RECV_READY));

        dataFromSlave[rCount++] = I2CMasterDataGet(SOC_I2C_0_REGS);

        I2CMasterIntClearEx(SOC_I2C_0_REGS, I2C_INT_RECV_READY);
    }

    I2CMasterStop(SOC_I2C_0_REGS);

    while(0 == (I2CMasterIntRawStatus(SOC_I2C_0_REGS) & I2C_INT_STOP_CONDITION));

    I2CMasterIntClearEx(SOC_I2C_0_REGS, I2C_INT_STOP_CONDITION);
}

int bb_get_version(void)
{
    I2CMasterSlaveAddrSet(SOC_I2C_0_REGS, 0x50);
    dataToSlave[0] = 0;
    dataToSlave[1] = 0;
    tCount = 0;
    rCount = 0;
    EEP_SetupReception(48);

	if(!memcmp((const void *)(dataFromSlave + 4), "A335BONE", 8))
	{
		char tmpstr[3];
		memcpy(tmpstr, (const void *)(dataFromSlave + 14), 2);
		tmpstr[2] = (int)NULL;
	    //UARTPuts("Board detected: BeagleBone rev ", -1);
	    //UARTPuts(tmpstr, -1);
	    //UARTPuts("\n\r", -1);
		return 0;
	}
	else if(!memcmp((const void *)(dataFromSlave + 4), "A335BNLT", 8))
	{
		char tmpstr[3];
		memcpy(tmpstr, (const void *)(dataFromSlave + 14), 2);
		tmpstr[2] = (int)NULL;
	    //UARTPuts("Board detected: BeagleBone Black rev ", -1);
	    //UARTPuts(tmpstr, -1);
	    //UARTPuts("\n\r", -1);
		return 1;
	}
	else
	{
	    //UARTPuts("Board detected: NONE", -1);
	    //UARTPuts("\n\r", -1);
		return -1;
	}
}
#endif
