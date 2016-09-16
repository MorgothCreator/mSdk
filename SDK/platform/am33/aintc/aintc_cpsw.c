/*
 * aintc_cpsw.c
 *
 *  Created on: Jan 1, 2013
 *      Author: XxXx
 */

//#include "lwip_api.h"
#include "aintc_cpsw.h"
#include "../include/interrupt.h"

//#ifdef USE_LWIP

extern void CPSWCore0RxIsr(void);
extern void CPSWCore0TxIsr(void);
/*
** Set up the ARM Interrupt Controller for generating timer interrupt
*/
void AintcCPSWIntrSetUp(void)
{
	//IntProtectionDisable();
    /* Register the Receive ISR for Core 0 */
    //IntRegister(SYS_INT_3PGSWRXINT0, CPSWCore0RxIsr);

    /* Register the Transmit ISR for Core 0 */
    //IntRegister(SYS_INT_3PGSWTXINT0, CPSWCore0TxIsr);

    /* Set the priority */
    IntPrioritySet(SYS_INT_3PGSWTXINT0, 1, AINTC_HOSTINT_ROUTE_IRQ);
    IntPrioritySet(SYS_INT_3PGSWRXINT0, 1, AINTC_HOSTINT_ROUTE_IRQ);

    /* Enable the system interrupt */
    IntSystemEnable(SYS_INT_3PGSWTXINT0);
    IntSystemEnable(SYS_INT_3PGSWRXINT0);
    //IntProtectionEnable();
}

//#endif

