/*
 * aintc_uart..c
 *
 *  Created on: Dec 1, 2012
 *      Author: XxXx
 */
/*#####################################################*/
#include "aintc_uart.h"
#include "api/uart_api.h"
#include "api/uart_def.h"
#include "../include/interrupt.h"
#include "../include/hw/hw_control_AM335x.h"
#include "../include/hw/soc_AM335x.h"
#include "../include/hw/hw_cm_wkup.h"
#include "../include/hw/hw_cm_per.h"
#include "../include/hw/hw_types.h"
/*#####################################################*/
Uart_Int_Service_t Uart_Int_Service;
/*#####################################################*/
void UARTIsr(Uart_t* UartStruct)
{
	//    unsigned int lcrRegValue = 0;
//    signed char retVal = 0;

//    /* Switching to Register Operational Mode of operation. */
//    lcrRegValue = UARTRegConfigModeEnable(UartStruct->BaseAddr, UART_REG_OPERATIONAL_MODE);
//	/*Transmit*/
//	if((UART_LSR_TX_SR_E | UART_LSR_TX_FIFO_E) !=
//          (HWREG(UartStruct->BaseAddr + UART_LSR) & (UART_LSR_TX_SR_E | UART_LSR_TX_FIFO_E)))
//	{
//		/*Tx Fifo is empty*/
//		if(UART_LSR_TX_FIFO_E == (HWREG(UartStruct->BaseAddr + UART_LSR) & UART_LSR_TX_FIFO_E))
//		{
//
//		}
//	}
//	/*Receive*/
//	if(0 == (HWREG(UartStruct->BaseAddr + UART_LSR) & UART_LSR_RX_FIFO_E))
//	{
//	    retVal = ((signed char)HWREG(UartStruct->BaseAddr + UART_RHR));
//	}
//    /* Restoring the value of LCR. */
//    HWREG(UartStruct->BaseAddr + UART_LCR) = lcrRegValue;
}
/*#####################################################*/
void UART0Isr(void)
{
	UARTIsr(Uart_Int_Service.Uart0);
}
/*#####################################################*/
void UART1Isr(void)
{
	UARTIsr(Uart_Int_Service.Uart1);
}
/*#####################################################*/
void UART2Isr(void)
{
	UARTIsr(Uart_Int_Service.Uart2);
}
/*#####################################################*/
void UART3Isr(void)
{
	UARTIsr(Uart_Int_Service.Uart3);
}
/*#####################################################*/
void UART4Isr(void)
{
	UARTIsr(Uart_Int_Service.Uart4);
}
/*#####################################################*/
void UART5Isr(void)
{
	UARTIsr(Uart_Int_Service.Uart5);
}
/*#####################################################*/
void UARTAINTCConfigure(Uart_t* UartStruct)
{
    /* Intialize the ARM Interrupt Controller(AINTC) */
    //IntAINTCInit();
    switch (UartStruct->UartNr)
    {
    case 0:
    	//IntProtectionDisable();
        /* Registering the Interrupt Service Routine(ISR). */
        IntRegister(SYS_INT_UART0INT, UART0Isr);

        /* Setting the priority for the system interrupt in AINTC. */
        IntPrioritySet(SYS_INT_UART0INT, UartStruct->Priority, AINTC_HOSTINT_ROUTE_IRQ );

        /* Enabling the system interrupt in AINTC. */
        IntSystemEnable(SYS_INT_UART0INT);
        //IntProtectionEnable();
    	break;
    case 1:
    	//IntProtectionDisable();
        /* Registering the Interrupt Service Routine(ISR). */
        IntRegister(SYS_INT_UART1INT, UART1Isr);

        /* Setting the priority for the system interrupt in AINTC. */
        IntPrioritySet(SYS_INT_UART1INT, UartStruct->Priority, AINTC_HOSTINT_ROUTE_IRQ );

        /* Enabling the system interrupt in AINTC. */
        IntSystemEnable(SYS_INT_UART1INT);
        //IntProtectionEnable();
    	break;
    case 2:
    	//IntProtectionDisable();
        /* Registering the Interrupt Service Routine(ISR). */
        IntRegister(SYS_INT_UART2INT, UART2Isr);

        /* Setting the priority for the system interrupt in AINTC. */
        IntPrioritySet(SYS_INT_UART2INT, UartStruct->Priority, AINTC_HOSTINT_ROUTE_IRQ );

        /* Enabling the system interrupt in AINTC. */
        IntSystemEnable(SYS_INT_UART2INT);
        //IntProtectionEnable();
    	break;
    case 3:
    	//IntProtectionDisable();
        /* Registering the Interrupt Service Routine(ISR). */
        IntRegister(SYS_INT_UART3INT, UART3Isr);

        /* Setting the priority for the system interrupt in AINTC. */
        IntPrioritySet(SYS_INT_UART3INT, UartStruct->Priority, AINTC_HOSTINT_ROUTE_IRQ );

        /* Enabling the system interrupt in AINTC. */
        IntSystemEnable(SYS_INT_UART3INT);
        //IntProtectionEnable();
    	break;
    case 4:
    	//IntProtectionDisable();
        /* Registering the Interrupt Service Routine(ISR). */
        IntRegister(SYS_INT_UART4INT, UART4Isr);

        /* Setting the priority for the system interrupt in AINTC. */
        IntPrioritySet(SYS_INT_UART4INT, UartStruct->Priority, AINTC_HOSTINT_ROUTE_IRQ );

        /* Enabling the system interrupt in AINTC. */
        IntSystemEnable(SYS_INT_UART4INT);
        //IntProtectionEnable();
    	break;
    case 5:
    	//IntProtectionDisable();
        /* Registering the Interrupt Service Routine(ISR). */
        IntRegister(SYS_INT_UART5INT, UART5Isr);

        /* Setting the priority for the system interrupt in AINTC. */
        IntPrioritySet(SYS_INT_UART5INT, UartStruct->Priority, AINTC_HOSTINT_ROUTE_IRQ );

        /* Enabling the system interrupt in AINTC. */
        IntSystemEnable(SYS_INT_UART5INT);
        //IntProtectionEnable();
    	break;
    }
}
/*#####################################################*/
void UARTAINTCUnConfigure(Uart_t* UartStruct)
{
    /* Intialize the ARM Interrupt Controller(AINTC) */
    //IntAINTCInit();
    switch (UartStruct->UartNr)
    {
    case 0:
    	//IntProtectionDisable();
        /* Enabling the system interrupt in AINTC. */
        IntSystemDisable(SYS_INT_UART0INT);

        /* Registering the Interrupt Service Routine(ISR). */
        IntUnRegister(SYS_INT_UART0INT);
        //IntProtectionEnable();
    	break;
    case 1:
    	//IntProtectionDisable();
        /* Enabling the system interrupt in AINTC. */
        IntSystemDisable(SYS_INT_UART1INT);

        /* Registering the Interrupt Service Routine(ISR). */
    	IntUnRegister(SYS_INT_UART1INT);
        //IntProtectionEnable();
    	break;
    case 2:
    	//IntProtectionDisable();
        /* Enabling the system interrupt in AINTC. */
        IntSystemDisable(SYS_INT_UART2INT);

        /* Registering the Interrupt Service Routine(ISR). */
    	IntUnRegister(SYS_INT_UART2INT);
        //IntProtectionEnable();
    	break;
    case 3:
    	//IntProtectionDisable();
        /* Enabling the system interrupt in AINTC. */
        IntSystemDisable(SYS_INT_UART3INT);

        /* Registering the Interrupt Service Routine(ISR). */
    	IntUnRegister(SYS_INT_UART3INT);

        //IntProtectionEnable();
    	break;
    case 4:
    	//IntProtectionDisable();
        /* Enabling the system interrupt in AINTC. */
        IntSystemDisable(SYS_INT_UART4INT);

        /* Registering the Interrupt Service Routine(ISR). */
    	IntUnRegister(SYS_INT_UART4INT);
        //IntProtectionEnable();
    	break;
    case 5:
    	//IntProtectionDisable();
        /* Enabling the system interrupt in AINTC. */
        IntSystemDisable(SYS_INT_UART5INT);

        /* Registering the Interrupt Service Routine(ISR). */
    	IntUnRegister(SYS_INT_UART5INT);
        //IntProtectionEnable();
    	break;
    }
}
/*#####################################################*/




