/**
 * \file   hs_mmcsd_rw.c
 *
 * \brief  Sample application for HS MMCSD
 *
*/

/*
* Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/ 
*/
/* 
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*
*    Redistributions of source code must retain the above copyright 
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the 
*    documentation and/or other materials provided with the   
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/

#include "board_properties.h"
#include "hs_mmcsd_interface.h"
#include "edma3_interface.h"
#include "api/edma_api.h"
#include "mmcsdlib/mmcsd_proto.h"
#include "mmcsdlib/hs_mmcsdlib.h"
//#include "include/armv7a/am335x/evmAM335x.h"
#include "../include/armv7a/am335x/edma_event.h"
#include "../include/hw/soc_AM335x.h"
#include "../include/interrupt.h"
#include "../include/hs_mmcsd.h"
#include <string.h>
#include "../sys/sysdelay.h"
#include "../sys/cache.h"
#include "../include/edma.h"
#include "../sys/mmu.h"
#include "../clk/clk_edma.h"
#include "../clk/clk_mmcsd.h"
#include "../pinmux/pin_mux_mmcsd.h"
#include "lib/fs/fat.h"
#include "api/gpio_api.h"
#include "board_init.h"
#ifdef MMCSD_PERF
#include "perf.h"
#endif

new_gpio *CardDetectPinMmcSd0 = NULL;
new_gpio *LedStatusMmcSd0 = NULL;
extern new_uart* DebugCom;
extern FileInfo_t *FILE1;

/******************************************************************************
**                      INTERNAL MACRO DEFINITIONS
*******************************************************************************/

/* Frequency */
#define HSMMCSD_IN_FREQ                96000000 /* 96MHz */
#define HSMMCSD_INIT_FREQ              400000   /* 400kHz */

#define HSMMCSD_CARD_DETECT_PINNUM     6

/* EDMA3 Event queue number. */
#define EVT_QUEUE_NUM                  (0)
 
/* EDMA3 Region Number. */
#define REGION_NUMBER                  (0)

/* Block size config */
#define HSMMCSD_BLK_SIZE               512
#define HSMMCSD_RW_BLK                 1

/* Global data pointers */
#define HSMMCSD_DATA_SIZE              512

/* GPIO instance related macros. */
#define GPIO_INST_BASE                 (SOC_GPIO_0_REGS)

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


/* SD card info structure */
mmcsdCardInfo sdCard;

/* SD Controller info structure */
mmcsdCtrlInfo  ctrlInfo;

/* EDMA callback function array */
extern void (*cb_Fxn[EDMA3_NUM_TCC]) (unsigned int tcc, unsigned int status);

/******************************************************************************
**                      VARIABLE DEFINITIONS
*******************************************************************************/
/* Global flags for interrupt handling */
volatile unsigned int sdBlkSize = HSMMCSD_BLK_SIZE;
volatile unsigned int callbackOccured = 0;
volatile unsigned int xferCompFlag = 0; 
volatile unsigned int dataTimeout = 0;
volatile unsigned int cmdCompFlag = 0;
volatile unsigned int cmdTimeout = 0; 
volatile unsigned int errFlag = 0;
volatile unsigned int initFlg = 1;

#ifdef __IAR_SYSTEMS_ICC__
#pragma data_alignment=SOC_CACHELINE_SIZE
unsigned char data[HSMMCSD_DATA_SIZE];

#elif defined(__TMS470__)
#pragma DATA_ALIGN(data, SOC_CACHELINE_SIZE);
unsigned char data[HSMMCSD_DATA_SIZE];

#elif defined(gcc)
unsigned char data[HSMMCSD_DATA_SIZE] 
                    __attribute__ ((aligned (SOC_CACHELINE_SIZE)))= {0};

#else
#error "Unsupported Compiler. \r\n"

#endif

/* page tables start must be aligned in 16K boundary */                  //
#ifdef __TMS470__
#pragma DATA_ALIGN(pageTable, MMU_PAGETABLE_ALIGN_SIZE);
static volatile unsigned int pageTable[MMU_PAGETABLE_NUM_ENTRY];
#elif defined(__IAR_SYSTEMS_ICC__)
#pragma data_alignment=MMU_PAGETABLE_ALIGN_SIZE
static volatile unsigned int pageTable[MMU_PAGETABLE_NUM_ENTRY];
#elif defined(gcc)
static volatile unsigned int pageTable[MMU_PAGETABLE_NUM_ENTRY] 
            __attribute__((aligned(MMU_PAGETABLE_ALIGN_SIZE)));
#else
#error "Unsupported Compiler. \r\n"
#endif

/*
 * Check command status
 */

static unsigned int HSMMCSDCmdStatusGet(mmcsdCtrlInfo *ctrl)
{
    unsigned int status = 0;

    do
    {
    	if(gpio_in(CardDetectPinMmcSd0))
    	{
    		cmdTimeout = 0;
    		break;
    	}
    }
    while ((cmdCompFlag == 0) && (cmdTimeout == 0));

    if (cmdCompFlag)
    {
        status = 1;
        cmdCompFlag = 0;
    }

    if (cmdTimeout)
    {
        status = 0;
        cmdTimeout = 0;
    }

    return status;
}

static unsigned int HSMMCSDXferStatusGet(mmcsdCtrlInfo *ctrl)
{
    unsigned int status = 0;
    volatile unsigned int timeOut = 0xFFFF;

    do
    {
    	if(gpio_in(CardDetectPinMmcSd0))
    	{
    		cmdTimeout = 0;
    		break;
    	}
    }
    while ((xferCompFlag == 0) && (dataTimeout == 0));

    if (xferCompFlag)
    {
        status = 1;
        xferCompFlag = 0;
    }

    if (dataTimeout)
    {
        status = 0;
        dataTimeout = 0;
    }

    /* Also, poll for the callback */
    if (HWREG(ctrl->memBase + MMCHS_CMD) & MMCHS_CMD_DP)
    {
        while(callbackOccured == 0 && ((timeOut--) != 0));
        callbackOccured = 0;

        if(timeOut == 0)
        {
            status = 0;
        }
    }

    ctrlInfo.dmaEnable = 0;

    return status;
}

void HSMMCSDRxDmaConfig(void *ptr, unsigned int blkSize, unsigned int nblks)
{
    EDMA3CCPaRAMEntry paramSet;

    paramSet.srcAddr    = ctrlInfo.memBase + MMCHS_DATA;
    paramSet.destAddr   = (unsigned int)ptr;
    paramSet.srcBIdx    = 0;
    paramSet.srcCIdx    = 0;
    paramSet.destBIdx   = 4;
    paramSet.destCIdx   = (unsigned short)blkSize;
    paramSet.aCnt       = 0x4;
    paramSet.bCnt       = (unsigned short)blkSize/4;
    paramSet.cCnt       = (unsigned short)nblks;
    paramSet.bCntReload = 0x0;
    paramSet.linkAddr   = 0xffff;
    paramSet.opt        = 0;

    /* Set OPT */
    paramSet.opt |= ((MMCSD_RX_EDMA_CHAN << EDMA3CC_OPT_TCC_SHIFT) & EDMA3CC_OPT_TCC);

    /* 1. Transmission complition interrupt enable */
    paramSet.opt |= (1 << EDMA3CC_OPT_TCINTEN_SHIFT);

    /* 2. Read FIFO : SRC Constant addr mode */
    paramSet.opt |= (1 << 0);

    /* 3. SRC FIFO width is 32 bit */
    paramSet.opt |= (2 << 8);

    /* 4.  AB-Sync mode */
    paramSet.opt |= (1 << 2);

    /* configure PaRAM Set */
    EDMA3SetPaRAM(EDMA_INST_BASE, MMCSD_RX_EDMA_CHAN, &paramSet);

    /* Enable the transfer */
    EDMA3EnableTransfer(EDMA_INST_BASE, MMCSD_RX_EDMA_CHAN, EDMA3_TRIG_MODE_EVENT);
}

void HSMMCSDTxDmaConfig(void *ptr, unsigned int blkSize, unsigned int blks)
{
    EDMA3CCPaRAMEntry paramSet;

    paramSet.srcAddr    = (unsigned int)ptr;
    paramSet.destAddr   = ctrlInfo.memBase + MMCHS_DATA;
    paramSet.srcBIdx    = 4;
    paramSet.srcCIdx    = blkSize;
    paramSet.destBIdx   = 0;
    paramSet.destCIdx   = 0;
    paramSet.aCnt       = 0x4;
    paramSet.bCnt       = (unsigned short)blkSize/4;
    paramSet.cCnt       = (unsigned short)blks;
    paramSet.bCntReload = 0x0;
    paramSet.linkAddr   = 0xffff;
    paramSet.opt        = 0;

    /* Set OPT */
    paramSet.opt |= ((MMCSD_TX_EDMA_CHAN << EDMA3CC_OPT_TCC_SHIFT) & EDMA3CC_OPT_TCC);

    /* 1. Transmission complition interrupt enable */
    paramSet.opt |= (1 << EDMA3CC_OPT_TCINTEN_SHIFT);

    /* 2. Read FIFO : DST Constant addr mode */
    paramSet.opt |= (1 << 1);

    /* 3. DST FIFO width is 32 bit */
    paramSet.opt |= (2 << 8);

    /* 4.  AB-Sync mode */
    paramSet.opt |= (1 << 2);

    /* configure PaRAM Set */
    EDMA3SetPaRAM(EDMA_INST_BASE, MMCSD_TX_EDMA_CHAN, &paramSet);

    /* Enable the transfer */
    EDMA3EnableTransfer(EDMA_INST_BASE, MMCSD_TX_EDMA_CHAN, EDMA3_TRIG_MODE_EVENT);
}

static void HSMMCSDXferSetup(mmcsdCtrlInfo *ctrl, unsigned char rwFlag, void *ptr,
                             unsigned int blkSize, unsigned int nBlks)
{
    callbackOccured = 0;
    xferCompFlag = 0;
    CacheDataCleanInvalidateBuff((unsigned int) ptr, (512 * nBlks));
    if (rwFlag == 1)
    {
        HSMMCSDRxDmaConfig(ptr, blkSize, nBlks);
    }
    else
    {
        HSMMCSDTxDmaConfig(ptr, blkSize, nBlks);
    }

    ctrl->dmaEnable = 1;
    HSMMCSDBlkLenSet(ctrl->memBase, blkSize);
}



void HSMMCSDIsr(void)
{
    volatile unsigned int status = 0;

    status = HSMMCSDIntrStatusGet(ctrlInfo.memBase, 0xFFFFFFFF);
    
    HSMMCSDIntrStatusClear(ctrlInfo.memBase, status);

    if (status & HS_MMCSD_STAT_CMDCOMP)
    {
        cmdCompFlag = 1;
    }

    if (status & HS_MMCSD_STAT_ERR)
    {
        errFlag = status & 0xFFFF0000;

        if (status & HS_MMCSD_STAT_CMDTIMEOUT)
        {
            cmdTimeout = 1;
        }

        if (status & HS_MMCSD_STAT_DATATIMEOUT)
        {
            dataTimeout = 1;
        }
    }

    if (status & HS_MMCSD_STAT_TRNFCOMP)
    {
        xferCompFlag = 1;
    }
}

/*
** Initialize the MMCSD controller structure for use
*/
static void HSMMCSDControllerSetup(signed int CardDetectPortNr, signed int CardDetectPinNr)
{
    ctrlInfo.memBase = MMCSD_INST_BASE;
    ctrlInfo.ctrlInit = HSMMCSDControllerInit;
    ctrlInfo.xferSetup = HSMMCSDXferSetup;
    ctrlInfo.cmdStatusGet = HSMMCSDCmdStatusGet;
    ctrlInfo.xferStatusGet = HSMMCSDXferStatusGet;
    /* Use the funciton HSMMCSDCDPinStatusGet() to use the card presence
       using the controller.
    */
    ctrlInfo.cardPresent = HSMMCSDCardPresent;
    ctrlInfo.cmdSend = HSMMCSDCmdSend;
    ctrlInfo.busWidthConfig = HSMMCSDBusWidthConfig;
    ctrlInfo.busFreqConfig = HSMMCSDBusFreqConfig;
    ctrlInfo.intrMask = (HS_MMCSD_INTR_CMDCOMP | HS_MMCSD_INTR_CMDTIMEOUT |
                            HS_MMCSD_INTR_DATATIMEOUT | HS_MMCSD_INTR_TRNFCOMP);
    ctrlInfo.intrEnable = HSMMCSDIntEnable;
    ctrlInfo.busWidth = (SD_BUS_WIDTH_1BIT | SD_BUS_WIDTH_4BIT);
    ctrlInfo.highspeed = 1;
    ctrlInfo.ocr = (SD_OCR_VDD_3P0_3P1 | SD_OCR_VDD_3P1_3P2);
    ctrlInfo.card = &sdCard;
    ctrlInfo.ipClk = HSMMCSD_IN_FREQ;
    ctrlInfo.opClk = HSMMCSD_INIT_FREQ;
    ctrlInfo.cdPinNum = (CardDetectPortNr<<5) + CardDetectPinNr;
    sdCard.ctrl = &ctrlInfo;

    CardDetectPinMmcSd0 = new_(new_gpio);
    CardDetectPinMmcSd0 = gpio_assign(CardDetectPortNr, CardDetectPinNr, GPIO_DIR_INPUT);

    callbackOccured = 0;
    xferCompFlag = 0;
    dataTimeout = 0;
    cmdCompFlag = 0;
    cmdTimeout = 0;
}


inline void _mmcsd_init(void *SdStruct, signed int CardDetectPortNr, signed int CardDetectPinNr, new_gpio* StatusLed)
{

	LedStatusMmcSd0 = StatusLed;
	/* Configure EDMA to service the HSMMCSD events. */
    HSMMCSDEdmaInit();

    /* Perform pin-mux for HSMMCSD pins. */
    //HSMMCSDPinMuxSetup();
    pin_mux_mmcsd_beaglebone(0);

    /* Enable module clock for HSMMCSD. */
    HSMMCSDModuleClkConfig();

    /* Basic controller initializations */
    HSMMCSDControllerSetup(CardDetectPortNr, CardDetectPinNr);

    /* Initialize the MMCSD controller */
    MMCSDCtrlInit((mmcsdCtrlInfo*)&ctrlInfo);

    MMCSDIntEnable((mmcsdCtrlInfo*)&ctrlInfo);
}

inline void _mmcsd_idle(void *SdStruct)
{
    if(!gpio_in(CardDetectPinMmcSd0)/*(HSMMCSDCardPresent(&ctrlInfo)) == 1*/)
    {
        if(initFlg)
        {
        	if(MMCSDCardInit((mmcsdCtrlInfo*)&ctrlInfo))
        	{
                Drives_Table[0] = new_(new_fat_disk);
                Drives_Table[0]->DiskInfo_SdDriverStructAddr = &ctrlInfo;
                //Drives_Table[0]->drive_init = MMCSD_CardInit;
                Drives_Table[0]->drive_read_page = MMCSDReadCmdSend;
                Drives_Table[0]->drive_write_page = MMCSDWriteCmdSend;
                if(_Fat_Mount(0))
                {
                	if(DebugCom)
                	{
																			UARTPuts(DebugCom,   "MMCSD0 drive 0 mounted\n\r" , -1);
																			UARTPuts(DebugCom,   "MMCSD0 Fat fs detected\n\r" , -1);
						if(Drives_Table[0]->DiskInfo_FatType == NoFs ){ 	UARTprintf(DebugCom, "MMCSD0 Fs type:                 None\n\r");}
						if(Drives_Table[0]->DiskInfo_FatType == Fat12){ 	UARTprintf(DebugCom, "MMCSD0 Fs type:                 Fat12\n\r");}
						if(Drives_Table[0]->DiskInfo_FatType == Fat16){ 	UARTprintf(DebugCom, "MMCSD0 Fs type:                 Fat16\n\r");}
						if(Drives_Table[0]->DiskInfo_FatType == Fat32){ 	UARTprintf(DebugCom, "MMCSD0 Fs type:                 Fat32\n\r");}
																			UARTprintf(DebugCom, "MMCSD0 BootSectorAddress:       %u \n\r",Drives_Table[0]->DiskInfo_BootSectorAddress);
																			UARTprintf(DebugCom, "MMCSD0 BytesPerSector:          %d \n\r",Drives_Table[0]->DiskInfo_BytesPerSector);
																			UARTprintf(DebugCom, "MMCSD0 SectorsPerCluster:       %d \n\r",Drives_Table[0]->DiskInfo_SectorsPerCluster);
																			UARTprintf(DebugCom, "MMCSD0 AllocTable1Begin:        %u \n\r",Drives_Table[0]->DiskInfo_AllocTable1Begin);
																			UARTprintf(DebugCom, "MMCSD0 NumberOfFats:            %d \n\r",Drives_Table[0]->DiskInfo_NumberOfFats);
																			UARTprintf(DebugCom, "MMCSD0 MediaType:               %d \n\r",Drives_Table[0]->DiskInfo_MediaType);
																			UARTprintf(DebugCom, "MMCSD0 AllocTableSize:          %u \n\r",Drives_Table[0]->DiskInfo_AllocTableSize);
																			UARTprintf(DebugCom, "MMCSD0 DataSectionBegin:        %d \n\r",Drives_Table[0]->DiskInfo_DataSectionBegin);
						if(Drives_Table[0]->DiskInfo_FatType == Fat32)		UARTprintf(DebugCom, "MMCSD0 uSD DiskCapacity:        %uMB\n\r",(unsigned long long)((unsigned long long)Drives_Table[0]->DiskInfo_AllocTableSize * 128 * (unsigned long long)Drives_Table[0]->DiskInfo_SectorsPerCluster * (unsigned long long)Drives_Table[0]->DiskInfo_BytesPerSector) / 1000000);
						else if(Drives_Table[0]->DiskInfo_FatType == Fat16)	UARTprintf(DebugCom, "MMCSD0 uSD DiskCapacity:        %uMB\n\r",(unsigned long long)((unsigned long long)Drives_Table[0]->DiskInfo_AllocTableSize * 256 * (unsigned long long)Drives_Table[0]->DiskInfo_SectorsPerCluster * (unsigned long long)Drives_Table[0]->DiskInfo_BytesPerSector) / 1000000);
                	}
                }
                else if(DebugCom)											UARTPuts(DebugCom,   "MMCSD0 Fat not detected\n\r" , -1);
        	}
            else if(DebugCom)												UARTPuts(DebugCom,   "MMCSD0 card not detected\n\r" , -1);
        }
        initFlg = 0;
    }
    else
    {
        Sysdelay(1);
        if(initFlg != 1)
        {
        	if(_FatData_CloseSesion(FILE1) == 1 && DebugCom != NULL)						UARTPuts(DebugCom, "MMCSD0 Session closed\n\r" , -1);
        	FILE1 = NULL;
        	if(_Fat_Unmount(0) == 1 && DebugCom != NULL)									UARTPuts(DebugCom, "MMCSD0 unmount\n\r" , -1);
        	/* Reinitialize all the state variables */
            callbackOccured = 0;
            xferCompFlag = 0;
            dataTimeout = 0;
            cmdCompFlag = 0;
            cmdTimeout = 0;
            /* Initialize the MMCSD controller */
            MMCSDCtrlInit((mmcsdCtrlInfo*)&ctrlInfo);
            MMCSDIntEnable((mmcsdCtrlInfo*)&ctrlInfo);
        }
        initFlg = 1;
    }
}

