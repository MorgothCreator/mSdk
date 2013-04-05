/*
 * aintc_mcspi.c
 *
 *  Created on: Feb 2, 2013
 *      Author: XxXx
 */
#include "api/mcspi_def.h"
#include "aintc_mcspi.h"
#include "driver/mcspi.h"
#include "driver/interrupt.h"
//#include "include/hw/hw_control_AM335x.h"
//#include "include/hw/soc_AM335x.h"
//#include "include/hw/hw_cm_wkup.h"
//#include "include/hw/hw_cm_per.h"
#include "include/hw/hw_types.h"
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
static void McSPI2Isr(void)
{
	McSPIIsr(Mcspi_Int_Service.Mcspi2);
}
/*#####################################################*/
static void McSPI3Isr(void)
{
	McSPIIsr(Mcspi_Int_Service.Mcspi3);
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
			IntRegister(SYS_INT_MCSPI1_IRQ, McSPI0Isr);

			/* Set Interrupt Priority */
			IntPrioritySet(SYS_INT_MCSPI1_IRQ, 0, AINTC_HOSTINT_ROUTE_IRQ);

			/* Enable system interrupt in AINTC */
			IntSystemEnable(SYS_INT_MCSPI1_IRQ);
	        IntProtectionEnable();
			break;
		case 1:
	    	IntProtectionDisable();
			/* Register McSPIIsr interrupt handler */
			IntRegister(SYS_INT_MCSPI2_IRQ, McSPI1Isr);

			/* Set Interrupt Priority */
			IntPrioritySet(SYS_INT_MCSPI2_IRQ, 0, AINTC_HOSTINT_ROUTE_IRQ);

			/* Enable system interrupt in AINTC */
			IntSystemEnable(SYS_INT_MCSPI2_IRQ);
	        IntProtectionEnable();
			break;
		case 2:
	    	IntProtectionDisable();
			/* Register McSPIIsr interrupt handler */
			IntRegister(SYS_INT_MCSPI3_IRQ, McSPI2Isr);

			/* Set Interrupt Priority */
			IntPrioritySet(SYS_INT_MCSPI3_IRQ, 0, AINTC_HOSTINT_ROUTE_IRQ);

			/* Enable system interrupt in AINTC */
			IntSystemEnable(SYS_INT_MCSPI3_IRQ);
	        IntProtectionEnable();
			break;
		case 3:
	    	IntProtectionDisable();
			/* Register McSPIIsr interrupt handler */
			IntRegister(SYS_INT_MCSPI4_IRQ, McSPI3Isr);

			/* Set Interrupt Priority */
			IntPrioritySet(SYS_INT_MCSPI4_IRQ, 0, AINTC_HOSTINT_ROUTE_IRQ);

			/* Enable system interrupt in AINTC */
			IntSystemEnable(SYS_INT_MCSPI4_IRQ);
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
			IntSystemDisable(SYS_INT_MCSPI1_IRQ);

			/* Register McSPIIsr interrupt handler */
			IntUnRegister(SYS_INT_MCSPI1_IRQ);
	        IntProtectionEnable();
			break;
		case 1:
	    	IntProtectionDisable();
			/* Enable system interrupt in AINTC */
			IntSystemDisable(SYS_INT_MCSPI2_IRQ);

			/* Register McSPIIsr interrupt handler */
			IntUnRegister(SYS_INT_MCSPI2_IRQ);
	        IntProtectionEnable();
			break;
		case 2:
	    	IntProtectionDisable();
			/* Enable system interrupt in AINTC */
			IntSystemDisable(SYS_INT_MCSPI3_IRQ);

			/* Register McSPIIsr interrupt handler */
			IntUnRegister(SYS_INT_MCSPI3_IRQ);
	        IntProtectionEnable();
			break;
		case 3:
	    	IntProtectionDisable();
			/* Enable system interrupt in AINTC */
			IntSystemDisable(SYS_INT_MCSPI4_IRQ);

			/* Register McSPIIsr interrupt handler */
			IntUnRegister(SYS_INT_MCSPI4_IRQ);
	        IntProtectionEnable();
			break;
	}
}
/*#####################################################*/


