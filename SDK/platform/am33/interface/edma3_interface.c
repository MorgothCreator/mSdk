/*
 * edma3_interface.c
 *
 *  Created on: Dec 13, 2012
 *      Author: XxXx
 */
#include "edma3_interface.h"
#include "../include/hw/soc_AM335x.h"
#include "../include/armv7a/am335x/edma_event.h"
#include "../include/interrupt.h"
#include "mmcsdlib/hs_mmcsdlib.h"
#include "mmcsdlib/mmcsd_proto.h"
#include "api/edma_api.h"

extern void (*cb_Fxn[EDMA3_NUM_TCC]) (unsigned int tcc, unsigned int status);

/* MMCSD instance related macros. */
#define MMCSD_INST_BASE                (SOC_MMCHS_0_REGS)
#define MMCSD_INT_NUM                  (SYS_INT_MMCSD0INT)

/* EDMA instance related macros. */
#define EDMA_INST_BASE                 (SOC_EDMA30CC_0_REGS)
#define EDMA_COMPLTN_INT_NUM           (SYS_INT_EDMACOMPINT)
#define EDMA_ERROR_INT_NUM             (SYS_INT_EDMAERRINT)

/* EDMA Events */
#define MMCSD_TX_EDMA_CHAN             (EDMA3_CHA_MMCSD0_TX)
#define MMCSD_RX_EDMA_CHAN             (EDMA3_CHA_MMCSD0_RX)


extern volatile unsigned int callbackOccured;
/*
** This function is used as a callback from EDMA3 Completion Handler.
*/
static void callback(unsigned int tccNum, unsigned int status)
{
    callbackOccured = 1;
    EDMA3DisableTransfer(EDMA_INST_BASE, tccNum, EDMA3_TRIG_MODE_EVENT);
}
//#ifdef sayftewy
static void _Edma3CompletionIsr(void)
{
    volatile unsigned int pendingIrqs;
    volatile unsigned int isIPR = 0;

    unsigned int indexl;
    unsigned int Cnt = 0;

    indexl = 1;

    isIPR = EDMA3GetIntrStatus(EDMA_INST_BASE);

    if(isIPR)
    {
        while ((Cnt < EDMA3CC_COMPL_HANDLER_RETRY_COUNT)&& (indexl != 0u))
        {
            indexl = 0u;
            pendingIrqs = EDMA3GetIntrStatus(EDMA_INST_BASE);

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

                    EDMA3ClrIntr(EDMA_INST_BASE, indexl);

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

static void _Edma3CCErrorIsr(void)
{
    volatile unsigned int pendingIrqs;
    volatile unsigned int evtqueNum = 0;  /* Event Queue Num */
    volatile unsigned int isIPRH = 0;
    volatile unsigned int isIPR = 0;
    volatile unsigned int Cnt = 0u;
    volatile unsigned int index;

    pendingIrqs = 0u;
    index = 1u;

    isIPR  = EDMA3GetIntrStatus(EDMA_INST_BASE);
    isIPRH = EDMA3IntrStatusHighGet(EDMA_INST_BASE);

    if((isIPR | isIPRH ) || (EDMA3QdmaGetErrIntrStatus(EDMA_INST_BASE) != 0)
        || (EDMA3GetCCErrStatus(EDMA_INST_BASE) != 0))
    {
        /* Loop for EDMA3CC_ERR_HANDLER_RETRY_COUNT number of time,
         * breaks when no pending interrupt is found
         */
        while ((Cnt < EDMA3CC_ERR_HANDLER_RETRY_COUNT)
                    && (index != 0u))
        {
            index = 0u;

            if(isIPR)
            {
                   pendingIrqs = EDMA3GetErrIntrStatus(EDMA_INST_BASE);
            }
            else
            {
                   pendingIrqs = EDMA3ErrIntrHighStatusGet(EDMA_INST_BASE);
            }

            while (pendingIrqs)
            {
                   /*Process all the pending interrupts*/
                   if(TRUE == (pendingIrqs & 1u))
                   {
                      /* Write to EMCR to clear the corresponding EMR bits.
                       */
                        /*Clear any SER*/

                        if(isIPR)
                        {
                             EDMA3ClrMissEvt(EDMA_INST_BASE, index);
                        }
                        else
                        {
                             EDMA3ClrMissEvt(EDMA_INST_BASE, index + 32);
                        }
                   }
                   ++index;
                   pendingIrqs >>= 1u;
            }
            index = 0u;
            pendingIrqs = EDMA3QdmaGetErrIntrStatus(EDMA_INST_BASE);
            while (pendingIrqs)
            {
                /*Process all the pending interrupts*/
                if(TRUE == (pendingIrqs & 1u))
                {
                    /* Here write to QEMCR to clear the corresponding QEMR bits*/
                    /*Clear any QSER*/
                    EDMA3QdmaClrMissEvt(EDMA_INST_BASE, index);
                }
                ++index;
                pendingIrqs >>= 1u;
            }
            index = 0u;


            pendingIrqs = EDMA3GetCCErrStatus(EDMA_INST_BASE);
            if (pendingIrqs != 0u)
            {
            /* Process all the pending CC error interrupts. */
            /* Queue threshold error for different event queues.*/
            for (evtqueNum = 0u; evtqueNum < SOC_EDMA3_NUM_EVQUE; evtqueNum++)
                {
                if((pendingIrqs & (1u << evtqueNum)) != 0u)
                {
                        /* Clear the error interrupt. */
                        EDMA3ClrCCErr(EDMA_INST_BASE, (1u << evtqueNum));
                    }
                }

            /* Transfer completion code error. */
            if ((pendingIrqs & (1 << EDMA3CC_CCERR_TCCERR_SHIFT)) != 0u)
            {
                EDMA3ClrCCErr(EDMA_INST_BASE,
                                      (0x01u << EDMA3CC_CCERR_TCCERR_SHIFT));
            }
                ++index;
            }
            Cnt++;
        }
    }

}
extern void HSMMCSDIsr(void);
/*
** This function configures the AINTC to receive EDMA3 interrupts.
*/
static void _EDMA3AINTCConfigure(void)
{
    /* Initializing the ARM Interrupt Controller. */

    /* Registering EDMA3 Channel Controller transfer completion interrupt.  */
    IntRegister(EDMA_COMPLTN_INT_NUM, _Edma3CompletionIsr);

    /* Setting the priority for EDMA3CC completion interrupt in AINTC. */
    IntPrioritySet(EDMA_COMPLTN_INT_NUM, 0, AINTC_HOSTINT_ROUTE_IRQ);

    /* Registering EDMA3 Channel Controller Error Interrupt. */
    IntRegister(EDMA_ERROR_INT_NUM, _Edma3CCErrorIsr);

    /* Setting the priority for EDMA3CC Error interrupt in AINTC. */
    IntPrioritySet(EDMA_ERROR_INT_NUM, 0, AINTC_HOSTINT_ROUTE_IRQ);

    /* Enabling the EDMA3CC completion interrupt in AINTC. */
    IntSystemEnable(EDMA_COMPLTN_INT_NUM);

    /* Enabling the EDMA3CC Error interrupt in AINTC. */
    IntSystemEnable(EDMA_ERROR_INT_NUM);

    /* Registering HSMMC Interrupt handler */
    IntRegister(MMCSD_INT_NUM, HSMMCSDIsr);

    /* Setting the priority for EDMA3CC completion interrupt in AINTC. */
    IntPrioritySet(MMCSD_INT_NUM, 0, AINTC_HOSTINT_ROUTE_IRQ);

    /* Enabling the HSMMC interrupt in AINTC. */
    IntSystemEnable(MMCSD_INT_NUM);

    /* Enabling IRQ in CPSR of ARM processor. */
    IntMasterIRQEnable();
}

//#endif

void EDMA3Initialize(void)
{
	_EDMAModuleClkConfig();
    /* Initialization of EDMA3 */
    EDMA3Init(SOC_EDMA30CC_0_REGS, EVT_QUEUE_NUM);

    /* Configuring the AINTC to receive EDMA3 interrupts. */
    _EDMA3AINTCConfigure();
}

void HSMMCSDEdmaInit(void)
{
	EDMA3Initialize();

    /* Request DMA Channel and TCC for MMCSD Transmit*/
    EDMA3RequestChannel(EDMA_INST_BASE, EDMA3_CHANNEL_TYPE_DMA,
                        MMCSD_TX_EDMA_CHAN, MMCSD_TX_EDMA_CHAN,
                        EVT_QUEUE_NUM);

    /* Registering Callback Function for TX*/
    cb_Fxn[MMCSD_TX_EDMA_CHAN] = &callback;

    /* Request DMA Channel and TCC for UART Receive */
    EDMA3RequestChannel(EDMA_INST_BASE, EDMA3_CHANNEL_TYPE_DMA,
                        MMCSD_RX_EDMA_CHAN, MMCSD_RX_EDMA_CHAN,
                        EVT_QUEUE_NUM);

    /* Registering Callback Function for RX*/
    cb_Fxn[MMCSD_RX_EDMA_CHAN] = &callback;
}


