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
//#define thirdpartyfatfs

#include "sys/plat_properties.h"
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
#include "lib/fat_fs/inc/ff.h"
#include "api/gpio_api.h"
#include "board_init.h"
#ifdef MMCSD_PERF
#include "perf.h"
#endif

new_gpio *CardDetectPinMmcSd[3] = {(new_gpio *)-1, (new_gpio *)-1, (new_gpio *)-1};
new_gpio *LedStatusMmcSd[3] = {NULL, NULL, NULL};
extern new_uart* DebugCom;
extern FileInfo_t *FILE1;


//*****************************************************************************
//
// Current FAT fs state.
//
//*****************************************************************************
FATFS g_s_mmcFatFs[3];
DIR g_sDirObject;

//*****************************************************************************
//
// This buffer holds the full path to the current working directory.  Initially
// it is root ("/").
//
//*****************************************************************************
#define PATH_BUF_SIZE   4
static char g_cCwdBuf[PATH_BUF_SIZE] = "0:/";

/******************************************************************************
**                      INTERNAL MACRO DEFINITIONS
*******************************************************************************/

/* Frequency */
#define HSMMCSD_IN_FREQ                96000000 /* 96MHz */
#define HSMMCSD_INIT_FREQ              48000000   /* 400kHz */

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
//mmcsdCardInfo sdCard;

/* SD Controller info structure */
mmcsdCtrlInfo *ctrlInfo[3];

/* EDMA callback function array */
extern void (*cb_Fxn[EDMA3_NUM_TCC]) (unsigned int tcc, unsigned int status);

/******************************************************************************
**                      VARIABLE DEFINITIONS
*******************************************************************************/
/* Global flags for interrupt handling */
volatile unsigned int sdBlkSize = HSMMCSD_BLK_SIZE;
volatile unsigned int callbackOccured[3] = {0,0,0};
volatile unsigned int xferCompFlag[3] = {0,0,0};
volatile unsigned int dataTimeout[3] = {0,0,0};
volatile unsigned int cmdCompFlag[3] = {0,0,0};
volatile unsigned int cmdTimeout[3] = {0,0,0};
volatile unsigned int errFlag[3] = {0,0,0};
volatile unsigned int initFlg[3] = {1,1,1};

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

/*
 * Check command status
 */

static unsigned int HSMMCSDCmdStatusGet(mmcsdCtrlInfo *ctrl)
{
    unsigned int status = 0;

    /*do
    {
    	if(CardDetectPinMmcSd[ctrl->SdNr] == -1 || gpio_in(CardDetectPinMmcSd[ctrl->SdNr]))
    	{
    		cmdTimeout[ctrl->SdNr] = 0;
    		break;
    	}
    }*/
    while ((cmdCompFlag[ctrl->SdNr] == 0) && (cmdTimeout[ctrl->SdNr] == 0));

    if (cmdCompFlag[ctrl->SdNr])
    {
        status = 1;
        cmdCompFlag[ctrl->SdNr] = 0;
    }

    if (cmdTimeout[ctrl->SdNr])
    {
        status = 0;
        cmdTimeout[ctrl->SdNr] = 0;
    }

    return status;
}

static unsigned int HSMMCSDXferStatusGet(mmcsdCtrlInfo *ctrl)
{
    unsigned int status = 0;
    volatile unsigned int timeOut = 0xFFFF;

    /*do
    {
    	if(CardDetectPinMmcSd[ctrl->SdNr] == -1 || gpio_in(CardDetectPinMmcSd[ctrl->SdNr]))
    	{
    		cmdTimeout[ctrl->SdNr] = 0;
    		break;
    	}
    }*/
    while ((xferCompFlag[ctrl->SdNr] == 0) && (dataTimeout[ctrl->SdNr] == 0));

    if (xferCompFlag[ctrl->SdNr])
    {
        status = 1;
        xferCompFlag[ctrl->SdNr] = 0;
    }

    if (dataTimeout[ctrl->SdNr])
    {
        status = 0;
        dataTimeout[ctrl->SdNr] = 0;
    }

    /* Also, poll for the callback */
    if (HWREG(ctrl->memBase + MMCHS_CMD) & MMCHS_CMD_DP)
    {
        while(callbackOccured[ctrl->SdNr] == 0 && ((timeOut--) != 0));
        callbackOccured[ctrl->SdNr] = 0;

        if(timeOut == 0)
        {
            status = 0;
        }
    }

    ctrl->dmaEnable = 0;

    return status;
}

void HSMMCSDRxDmaConfig(mmcsdCtrlInfo *ctrl, void *ptr, unsigned int blkSize, unsigned int nblks)
{
    EDMA3CCPaRAMEntry paramSet;

    paramSet.srcAddr    = ctrl->memBase + MMCHS_DATA;
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

void HSMMCSDTxDmaConfig(mmcsdCtrlInfo *ctrl, void *ptr, unsigned int blkSize, unsigned int blks)
{
    EDMA3CCPaRAMEntry paramSet;

    paramSet.srcAddr    = (unsigned int)ptr;
    paramSet.destAddr   = ctrl->memBase + MMCHS_DATA;
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
    callbackOccured[ctrl->SdNr] = 0;
    xferCompFlag[ctrl->SdNr] = 0;
    CacheDataCleanInvalidateBuff((unsigned int) ptr, (512 * nBlks));
    if (rwFlag == 1)
    {
        HSMMCSDRxDmaConfig(ctrl, ptr, blkSize, nBlks);
    }
    else
    {
        HSMMCSDTxDmaConfig(ctrl, ptr, blkSize, nBlks);
    }

    ctrl->dmaEnable = 1;
    HSMMCSDBlkLenSet(ctrl->memBase, blkSize);
}



void HSMMCSDIsrGen(mmcsdCtrlInfo* SdCtrlStruct)
{
    volatile unsigned int status = 0;

    status = HSMMCSDIntrStatusGet(SdCtrlStruct->memBase, 0xFFFFFFFF);
    
    HSMMCSDIntrStatusClear(SdCtrlStruct->memBase, status);

    if (status & HS_MMCSD_STAT_CMDCOMP)
    {
        cmdCompFlag[SdCtrlStruct->SdNr] = 1;
    }

    if (status & HS_MMCSD_STAT_ERR)
    {
        errFlag[SdCtrlStruct->SdNr] = status & 0xFFFF0000;

        if (status & HS_MMCSD_STAT_CMDTIMEOUT)
        {
            cmdTimeout[SdCtrlStruct->SdNr] = 1;
        }

        if (status & HS_MMCSD_STAT_DATATIMEOUT)
        {
            dataTimeout[SdCtrlStruct->SdNr] = 1;
        }
    }

    if (status & HS_MMCSD_STAT_TRNFCOMP)
    {
        xferCompFlag[SdCtrlStruct->SdNr] = 1;
    }
}

void HSMMCSD0Isr(void)
{
	HSMMCSDIsrGen(ctrlInfo[0]);
}
void HSMMCSD1Isr(void)
{
	HSMMCSDIsrGen(ctrlInfo[1]);
}
void HSMMCSD2Isr(void)
{
	HSMMCSDIsrGen(ctrlInfo[2]);
}
/*
** Initialize the MMCSD controller structure for use
*/
static void HSMMCSDControllerSetup(mmcsdCtrlInfo* SdCtrlStruct, signed int CardDetectPortNr, signed int CardDetectPinNr)
{
	ctrlInfo[SdCtrlStruct->SdNr] = SdCtrlStruct;
	switch(SdCtrlStruct->SdNr)
	{
	case (0):
		SdCtrlStruct->memBase = SOC_MMCHS_0_REGS;
		break;
	case (1):
		SdCtrlStruct->memBase = SOC_MMCHS_1_REGS;
		break;
	case (2):
		SdCtrlStruct->memBase = SOC_MMCHS_2_REGS;
		break;
	}
	SdCtrlStruct->ctrlInit = HSMMCSDControllerInit;
	SdCtrlStruct->xferSetup = HSMMCSDXferSetup;
	SdCtrlStruct->cmdStatusGet = HSMMCSDCmdStatusGet;
	SdCtrlStruct->xferStatusGet = HSMMCSDXferStatusGet;
    /* Use the funciton HSMMCSDCDPinStatusGet() to use the card presence
       using the controller.
    */
	SdCtrlStruct->cardPresent = HSMMCSDCardPresent;
	SdCtrlStruct->cmdSend = HSMMCSDCmdSend;
	SdCtrlStruct->busWidthConfig = HSMMCSDBusWidthConfig;
	SdCtrlStruct->busFreqConfig = HSMMCSDBusFreqConfig;
	SdCtrlStruct->intrMask = (HS_MMCSD_INTR_CMDCOMP | HS_MMCSD_INTR_CMDTIMEOUT |
                            HS_MMCSD_INTR_DATATIMEOUT | HS_MMCSD_INTR_TRNFCOMP);
	SdCtrlStruct->intrEnable = HSMMCSDIntEnable;
	SdCtrlStruct->busWidth = (SD_BUS_WIDTH_1BIT | SD_BUS_WIDTH_4BIT);
	SdCtrlStruct->highspeed = 1;
	SdCtrlStruct->ocr = (SD_OCR_VDD_3P0_3P1 | SD_OCR_VDD_3P1_3P2);
	SdCtrlStruct->card = (mmcsdCardInfo*)malloc(sizeof(mmcsdCardInfo));//&sdCard;
	SdCtrlStruct->ipClk = HSMMCSD_IN_FREQ;
	SdCtrlStruct->opClk = HSMMCSD_INIT_FREQ;
    //sdCard.ctrl = &ctrlInfo;
	SdCtrlStruct->card->ctrl = (mmcsdCtrlInfo*)(void*)&ctrlInfo;

	if(CardDetectPortNr == -1 || CardDetectPinNr == -1)
	{
		SdCtrlStruct->cdPinNum = -1;
		//CardDetectPinMmcSd[0] = new_(new_gpio);
		CardDetectPinMmcSd[SdCtrlStruct->SdNr] = (new_gpio *)-1;
	}
	else
	{
		SdCtrlStruct->cdPinNum = (CardDetectPortNr<<5) + CardDetectPinNr;
		//CardDetectPinMmcSd[0] = new_(new_gpio);
		CardDetectPinMmcSd[SdCtrlStruct->SdNr] = gpio_assign(CardDetectPortNr, CardDetectPinNr, GPIO_DIR_INPUT, false);
	}

    callbackOccured[SdCtrlStruct->SdNr] = 0;
    xferCompFlag[SdCtrlStruct->SdNr] = 0;
    dataTimeout[SdCtrlStruct->SdNr] = 0;
    cmdCompFlag[SdCtrlStruct->SdNr] = 0;
    cmdTimeout[SdCtrlStruct->SdNr] = 0;
}


void _mmcsd_init(void *SdCtrlStruct, signed int CardDetectPortNr, signed int CardDetectPinNr, new_gpio* StatusLed)
{

	LedStatusMmcSd[((mmcsdCtrlInfo*)SdCtrlStruct)->SdNr] = StatusLed;
	/* Configure EDMA to service the HSMMCSD events. */
    HSMMCSDEdmaInit(((mmcsdCtrlInfo*)SdCtrlStruct)->SdNr);

    /* Perform pin-mux for HSMMCSD pins. */
    //HSMMCSDPinMuxSetup();
    pin_mux_mmcsd_beaglebone(((mmcsdCtrlInfo*)SdCtrlStruct)->SdNr);

    /* Enable module clock for HSMMCSD. */
    HSMMCSDModuleClkConfig(((mmcsdCtrlInfo*)SdCtrlStruct)->SdNr);

    /* Basic controller initializations */
    HSMMCSDControllerSetup((mmcsdCtrlInfo*)SdCtrlStruct, CardDetectPortNr, CardDetectPinNr);

    /* Initialize the MMCSD controller */
    MMCSDCtrlInit((mmcsdCtrlInfo*)SdCtrlStruct);

    MMCSDIntEnable((mmcsdCtrlInfo*)SdCtrlStruct);
}

void _mmcsd_idle(void *SdCtrlStruct)
{
    if(!gpio_in(CardDetectPinMmcSd[((mmcsdCtrlInfo*)SdCtrlStruct)->SdNr])/*(HSMMCSDCardPresent(&ctrlInfo)) == 1*/ || (int)(CardDetectPinMmcSd[((mmcsdCtrlInfo*)SdCtrlStruct)->SdNr]) == -1)
    {
        if(initFlg[((mmcsdCtrlInfo*)SdCtrlStruct)->SdNr])
        {
            initFlg[((mmcsdCtrlInfo*)SdCtrlStruct)->SdNr] = 0;
        	if(MMCSDCardInit((mmcsdCtrlInfo*)SdCtrlStruct))
        	{
#ifndef thirdpartyfatfs
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
#else
                g_s_mmcFatFs[((mmcsdCtrlInfo*)SdCtrlStruct)->SdNr].drv_rw_func.DriveStruct = SdCtrlStruct;
                g_s_mmcFatFs[((mmcsdCtrlInfo*)SdCtrlStruct)->SdNr].drv_rw_func.drv_r_func = MMCSDReadCmdSend;
                g_s_mmcFatFs[((mmcsdCtrlInfo*)SdCtrlStruct)->SdNr].drv_rw_func.drv_w_func = MMCSDWriteCmdSend;
                if(!f_mount(0, &g_s_mmcFatFs[((mmcsdCtrlInfo*)SdCtrlStruct)->SdNr]))
                {
                    if(f_opendir(&g_sDirObject, g_cCwdBuf) == FR_OK)
                    {
						if(DebugCom)
						{
																				UARTPuts(DebugCom,   "MMCSD0 drive 0 mounted\n\r" , -1);
																				UARTPuts(DebugCom,   "MMCSD0 Fat fs detected\n\r" , -1);
																				UARTprintf(DebugCom, "MMCSD0 Fs type:                 ");
							if(g_s_mmcFatFs[((mmcsdCtrlInfo*)SdCtrlStruct)->SdNr].fs_type == FS_FAT12)	{ 				UARTprintf(DebugCom, "Fat12");}
							else if(g_s_mmcFatFs[((mmcsdCtrlInfo*)SdCtrlStruct)->SdNr].fs_type == FS_FAT16){ 				UARTprintf(DebugCom, "Fat16");}
							else if(g_s_mmcFatFs[((mmcsdCtrlInfo*)SdCtrlStruct)->SdNr].fs_type == FS_FAT32){ 				UARTprintf(DebugCom, "Fat32");}
							else								{ 				UARTprintf(DebugCom, "None");}
																				UARTprintf(DebugCom, "\n\r");
																				//UARTprintf(DebugCom, "MMCSD0 BootSectorAddress:       %u \n\r",(unsigned int)g_sFatFs.);
																				UARTprintf(DebugCom, "MMCSD0 BytesPerSector:          %d \n\r",/*(int)g_sFatFs.s_size*/512);
																				UARTprintf(DebugCom, "MMCSD0 SectorsPerCluster:       %d \n\r",(int)g_s_mmcFatFs[((mmcsdCtrlInfo*)SdCtrlStruct)->SdNr].csize);
																				//UARTprintf(DebugCom, "MMCSD0 AllocTable1Begin:        %u \n\r",(unsigned int)g_sFatFs.fatbase);
																				UARTprintf(DebugCom, "MMCSD0 NumberOfFats:            %d \n\r",(int)g_s_mmcFatFs[((mmcsdCtrlInfo*)SdCtrlStruct)->SdNr].n_fats);
																				//UARTprintf(DebugCom, "MMCSD0 MediaType:               %d \n\r",Drives_Table[0]->DiskInfo_MediaType);
																				//UARTprintf(DebugCom, "MMCSD0 AllocTableSize:          %u \n\r",Drives_Table[0]->DiskInfo_AllocTableSize);
																				UARTprintf(DebugCom, "MMCSD0 DataSectionBegin:        %d \n\r",(int)g_s_mmcFatFs[((mmcsdCtrlInfo*)SdCtrlStruct)->SdNr].fatbase);
																				UARTprintf(DebugCom, "MMCSD0 uSD DiskCapacity:        %uMB\n\r",(unsigned long)((unsigned long long)((unsigned long long)g_s_mmcFatFs[((mmcsdCtrlInfo*)SdCtrlStruct)->SdNr].max_clust * (unsigned long long)g_s_mmcFatFs[((mmcsdCtrlInfo*)SdCtrlStruct)->SdNr].csize * (unsigned long long)/*g_sFatFs.s_size*/512) / 1000000));
						}
                    } else  if(DebugCom)										UARTPuts(DebugCom,   "MMCSD0 ERROR oppening path\n\r" , -1);
#endif
                }
                else if(DebugCom)												UARTPuts(DebugCom,   "MMCSD0 ERROR mounting disk\n\r" , -1);
        	}
        }
    }
    else
    {
        Sysdelay(1);
        if(initFlg[((mmcsdCtrlInfo*)SdCtrlStruct)->SdNr] != 1)
        {
            initFlg[((mmcsdCtrlInfo*)SdCtrlStruct)->SdNr] = 1;
#ifndef thirdpartyfatfs
        	if(_FatData_CloseSesion(FILE1) == 1 && DebugCom != NULL)						UARTPuts(DebugCom, "MMCSD0 Session closed\n\r" , -1);
        	FILE1 = NULL;
        	if(_Fat_Unmount(0) == 1 && DebugCom != NULL)									UARTPuts(DebugCom, "MMCSD0 unmount\n\r" , -1);
#endif
        	/* Reinitialize all the state variables */
            callbackOccured[((mmcsdCtrlInfo*)SdCtrlStruct)->SdNr] = 0;
            xferCompFlag[((mmcsdCtrlInfo*)SdCtrlStruct)->SdNr] = 0;
            dataTimeout[((mmcsdCtrlInfo*)SdCtrlStruct)->SdNr] = 0;
            cmdCompFlag[((mmcsdCtrlInfo*)SdCtrlStruct)->SdNr] = 0;
            cmdTimeout[((mmcsdCtrlInfo*)SdCtrlStruct)->SdNr] = 0;
            /* Initialize the MMCSD controller */
            MMCSDCtrlInit((mmcsdCtrlInfo*)SdCtrlStruct);
            MMCSDIntEnable((mmcsdCtrlInfo*)SdCtrlStruct);
        }
    }
}

