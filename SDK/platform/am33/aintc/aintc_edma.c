/*
 * aintc_edma.c
 *
 *  Created on: Dec 2, 2012
 *      Author: XxXx
 */
/*#####################################################*/
#include "../include/edma.h"
#include "../include/interrupt.h"
#include "../include/hw/hw_control_AM335x.h"
#include "../include/hw/soc_AM335x.h"
#include "../include/hw/hw_cm_wkup.h"
#include "../include/hw/hw_cm_per.h"
#include "../include/hw/hw_types.h"
/*#####################################################*/
/* EDMA3 Region Number. */
#define REGION_NUMBER             (0)
/*#####################################################*/
/* EDMA callback function array */
void (*cb_Fxn[EDMA3_NUM_TCC]) (unsigned int tcc, unsigned int status);
/*#####################################################*/
static void Edma3CompletionIsr(void)
{
    volatile unsigned int pendingIrqs;
    volatile unsigned int isIPR = 0;

    unsigned int indexl;
    unsigned int Cnt = 0;

    indexl = 1;

    isIPR = HWREG(SOC_EDMA30CC_0_REGS + EDMA3CC_S_IPR(REGION_NUMBER));
    if(isIPR)
    {
        while ((Cnt < EDMA3CC_COMPL_HANDLER_RETRY_COUNT)&& (indexl != 0u))
        {
            indexl = 0u;
            pendingIrqs = HWREG(SOC_EDMA30CC_0_REGS + EDMA3CC_S_IPR(REGION_NUMBER));
            while (pendingIrqs)
            {
                if((pendingIrqs & 1u) == TRUE)
                {
                    /**
                    * If the user has not given any callback function
                    * while requesting the TCC, its TCC specific bit
                    * in the IPR register will NOT be cleared.
                    */
                    /* here write to ICR to clear the corresponding IPR bits */
                    HWREG(SOC_EDMA30CC_0_REGS + EDMA3CC_S_ICR(REGION_NUMBER)) = (1u << indexl);

                    if (cb_Fxn[indexl] != NULL)
                    {
                        (*cb_Fxn[indexl])(indexl, EDMA3_XFER_COMPLETE);
                    }
                }
                ++indexl;
                pendingIrqs >>= 1u;
            }
            Cnt++;
        }
    }
}
/*#####################################################*/
static void Edma3CCErrorIsr(void)
{
    volatile unsigned int pendingIrqs = 0;
    unsigned int evtqueNum = 0;
    unsigned int index = 1;
    unsigned int Cnt = 0;

    if((HWREG(SOC_EDMA30CC_0_REGS + EDMA3CC_EMR) != 0 ) || \
       (HWREG(SOC_EDMA30CC_0_REGS + EDMA3CC_QEMR) != 0) || \
       (HWREG(SOC_EDMA30CC_0_REGS + EDMA3CC_CCERR) != 0))
    {
        /* Loop for EDMA3CC_ERR_HANDLER_RETRY_COUNT number of time, breaks
           when no pending interrupt is found */
        while ((Cnt < EDMA3CC_ERR_HANDLER_RETRY_COUNT) && (index != 0u))
        {
            index = 0u;
            pendingIrqs = HWREG(SOC_EDMA30CC_0_REGS + EDMA3CC_EMR);
            while (pendingIrqs)
            {
                /*Process all the pending interrupts*/
                if((pendingIrqs & 1u)==TRUE)
                {
                    /* Write to EMCR to clear the corresponding EMR bits.*/
                    HWREG(SOC_EDMA30CC_0_REGS + EDMA3CC_EMCR) = (1u<<index);
                    /*Clear any SER*/
                    HWREG(SOC_EDMA30CC_0_REGS + EDMA3CC_S_SECR(REGION_NUMBER)) = (1u << index);
                }
                ++index;
                pendingIrqs >>= 1u;
            }
            index = 0u;
            pendingIrqs = HWREG(SOC_EDMA30CC_0_REGS + EDMA3CC_QEMR);
            while (pendingIrqs)
            {
                /*Process all the pending interrupts*/
                if((pendingIrqs & 1u)==TRUE)
                {
                    /* Here write to QEMCR to clear the corresponding QEMR bits*/
                    HWREG(SOC_EDMA30CC_0_REGS + EDMA3CC_QEMCR) = (1u<<index);
                    /*Clear any QSER*/
                    HWREG(SOC_EDMA30CC_0_REGS + EDMA3CC_S_QSECR(0)) = (1u<<index);
                }
                ++index;
                pendingIrqs >>= 1u;
            }
            index = 0u;
            pendingIrqs = HWREG(SOC_EDMA30CC_0_REGS + EDMA3CC_CCERR);
            if (pendingIrqs != 0u)
            {
                /* Process all the pending CC error interrupts. */
                  /* Queue threshold error for different event queues.*/
                for (evtqueNum = 0u; evtqueNum < EDMA3_0_NUM_EVTQUE; evtqueNum++)
                {
                    if((pendingIrqs & (1u << evtqueNum)) != 0u)
                    {
                        /* Clear the error interrupt. */
                        HWREG(SOC_EDMA30CC_0_REGS + EDMA3CC_CCERRCLR) = (1u << evtqueNum);
                    }
                 }

                 /* Transfer completion code error. */
                 if ((pendingIrqs & (1 << EDMA3CC_CCERR_TCCERR_SHIFT)) != 0u)
                 {
                    HWREG(SOC_EDMA30CC_0_REGS + EDMA3CC_CCERRCLR) = \
                    (0x01u << EDMA3CC_CCERR_TCCERR_SHIFT);
                 }
                 ++index;
            }

            Cnt++;
        }
    }
}
/*#####################################################*/
void EDMA3AINTCConfigure(void)
{
	IntProtectionDisable();
    /* Initializing the ARM Interrupt Controller. */
    //IntAINTCInit();

    /* Registering EDMA3 Channel Controller 0 transfer completion interrupt.  */
    IntRegister(SYS_INT_EDMACOMPINT, Edma3CompletionIsr);

    /* Setting the priority for EDMA3CC0 completion interrupt in AINTC. */
    IntPrioritySet(SYS_INT_EDMACOMPINT, 0, AINTC_HOSTINT_ROUTE_IRQ);

    /* Registering EDMA3 Channel Controller 0 Error Interrupt. */
    IntRegister(SYS_INT_EDMAERRINT, Edma3CCErrorIsr);

    /* Setting the priority for EDMA3CC0 Error interrupt in AINTC. */
    IntPrioritySet(SYS_INT_EDMAERRINT, 0, AINTC_HOSTINT_ROUTE_IRQ);

    /* Enabling the EDMA3CC0 completion interrupt in AINTC. */
    IntSystemEnable(SYS_INT_EDMACOMPINT);

    /* Enabling the EDMA3CC0 Error interrupt in AINTC. */
    IntSystemEnable(SYS_INT_EDMAERRINT);

    /* Registering HSMMC 0 Interrupt handler */
    //IntRegister(SYS_INT_HSMMC_0, HSMMCSDIsr);

    /* Setting the priority for EDMA3CC0 completion interrupt in AINTC. */
    //IntPrioritySet(SYS_INT_HSMMC_0, 0, AINTC_HOSTINT_ROUTE_IRQ);

    /* Enabling the HSMMC 0 interrupt in AINTC. */
    //IntSystemEnable(SYS_INT_HSMMC_0);

    /* Enabling IRQ in CPSR of ARM processor. */
    //IntMasterIRQEnable();
    IntProtectionEnable();
}

/*#####################################################*/


