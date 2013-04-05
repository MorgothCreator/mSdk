/*
 * aintc_mcspi.c
 *
 *  Created on: Dec 12, 2012
 *      Author: XxXx
 */
/*#####################################################*/
#include "api/mcspi_def.h"
#include "aintc_mcspi.h"
#include "../include/mcspi.h"
#include "../include/interrupt.h"
#include "../include/hw/hw_control_AM335x.h"
#include "../include/hw/soc_AM335x.h"
#include "../include/hw/hw_cm_wkup.h"
#include "../include/hw/hw_cm_per.h"
#include "../include/hw/hw_types.h"
/*#####################################################*/
Mcspi_Int_Service_t Mcspi_Int_Service;
/*#####################################################*/
/*
** McSPI Interrupt Service Routine. This function will clear the status of the
** Tx/Rx interrupts when generated. Will write the Tx data on transmit data
** register and also will put the received data from receive data register to
** a location in memory.
*/
static void McSPIIsr(Mcspi_t *McspiStruct)
{
    unsigned int intCode = 0;

    intCode = McSPIIntStatusGet(McspiStruct->BaseAddr);

    while(intCode)
    {
        if(MCSPI_INT_TX_EMPTY(McspiStruct->Channel) == (intCode & MCSPI_INT_TX_EMPTY(McspiStruct->Channel)))
        {
            McSPIIntStatusClear(McspiStruct->BaseAddr, MCSPI_INT_TX_EMPTY(McspiStruct->Channel));

            McspiStruct->numOfBytes--;

            McSPITransmitData(McspiStruct->BaseAddr,(unsigned int)(*McspiStruct->BuffTmp++), McspiStruct->Channel);

            if(!McspiStruct->numOfBytes)
            {
                McSPIIntDisable(McspiStruct->BaseAddr, MCSPI_INT_TX_EMPTY(McspiStruct->Channel));

                McSPIIntStatusClear(McspiStruct->BaseAddr, MCSPI_INT_TX_EMPTY(McspiStruct->Channel));
            }
        }

        if(MCSPI_INT_RX_FULL(McspiStruct->Channel) == (intCode & MCSPI_INT_RX_FULL(McspiStruct->Channel)))
        {
            McSPIIntStatusClear(McspiStruct->BaseAddr, MCSPI_INT_RX_FULL(McspiStruct->Channel));

            *McspiStruct->BuffTmp++ = (unsigned char) McSPIReceiveData(McspiStruct->BaseAddr, McspiStruct->Channel);

            if(!(McspiStruct->numOfBytes))
            {
                McSPIIntDisable(McspiStruct->BaseAddr, MCSPI_INT_RX_FULL(McspiStruct->Channel));

                McspiStruct->flag = 0;
            }
        }

        intCode = McSPIIntStatusGet(McspiStruct->BaseAddr);
    }
}
/*#####################################################*/
static void McSPI0Isr(void)
{
	McSPIIsr(Mcspi_Int_Service.Mcspi0);
}
/*#####################################################*/
static void McSPI1Isr(void)
{
	McSPIIsr(Mcspi_Int_Service.Mcspi1);
}
/*#####################################################*/
/* Interrupt mapping to AINTC and registering McSPI ISR */
void McSPIAintcConfigure(unsigned char instance)
{
	switch(instance)
	{
		case 0:
	    	IntProtectionDisable();
			/* Register McSPIIsr interrupt handler */
			IntRegister(SYS_INT_SPI0INT, McSPI0Isr);

			/* Set Interrupt Priority */
			IntPrioritySet(SYS_INT_SPI0INT, 0, AINTC_HOSTINT_ROUTE_IRQ);

			/* Enable system interrupt in AINTC */
			IntSystemEnable(SYS_INT_SPI0INT);
	        IntProtectionEnable();
			break;
		case 1:
	    	IntProtectionDisable();
			/* Register McSPIIsr interrupt handler */
			IntRegister(SYS_INT_SPI1INT, McSPI1Isr);

			/* Set Interrupt Priority */
			IntPrioritySet(SYS_INT_SPI1INT, 0, AINTC_HOSTINT_ROUTE_IRQ);

			/* Enable system interrupt in AINTC */
			IntSystemEnable(SYS_INT_SPI1INT);
	        IntProtectionEnable();
			break;
	}
}
/*#####################################################*/
/*#####################################################*/
/* Interrupt mapping to AINTC and registering McSPI ISR */
void McSPIAintcUnConfigure(unsigned char instance)
{
	switch(instance)
	{
		case 0:
	    	IntProtectionDisable();
			/* Enable system interrupt in AINTC */
			IntSystemDisable(SYS_INT_SPI0INT);

			/* Register McSPIIsr interrupt handler */
			IntUnRegister(SYS_INT_SPI0INT);
	        IntProtectionEnable();
	        break;
		case 1:
	    	IntProtectionDisable();
			/* Enable system interrupt in AINTC */
			IntSystemDisable(SYS_INT_SPI1INT);

			/* Register McSPIIsr interrupt handler */
			IntUnRegister(SYS_INT_SPI1INT);
	        IntProtectionEnable();
	        break;
	}
}
/*#####################################################*/


