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

#include "main.h"
#include "sys/plat_properties.h"
#include "hs_mmcsd_interface.h"
#include "edma3_interface.h"
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
#include "api/edma_api.h"
#include "api/gpio_api.h"
#include "api/uart_api.h"
#include "api/uart_def.h"
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
static char g_cCwdBuf0[PATH_BUF_SIZE] = "0:/";
static char g_cCwdBuf1[PATH_BUF_SIZE] = "1:/";
char *g_cCwdBuf[2] = {g_cCwdBuf0, g_cCwdBuf1};

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
mmcsdCtrlInfo ctrlInfo[3];

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
    volatile unsigned int timeOut = 0xFFFF;

    /*do
    {
    	if(CardDetectPinMmcSd[ctrl->SdNr] == -1 || gpio_in(CardDetectPinMmcSd[ctrl->SdNr]))
    	{
    		cmdTimeout[ctrl->SdNr] = 0;
    		break;
    	}
    }*/
    while ((cmdCompFlag[ctrl->SdNr] == 0) && (cmdTimeout[ctrl->SdNr] == 0) && (timeOut--));

    if(timeOut == 0)
    {
        status = 0;
    }

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
    volatile unsigned int timeOut = 0xFFFFFF;

    /*do
    {
    	if(CardDetectPinMmcSd[ctrl->SdNr] == -1 || gpio_in(CardDetectPinMmcSd[ctrl->SdNr]))
    	{
    		cmdTimeout[ctrl->SdNr] = 0;
    		break;
    	}
    }*/
    if((int)(CardDetectPinMmcSd[ctrl->SdNr]) == -1)
    {
    	while ((xferCompFlag[ctrl->SdNr] == 0) && (dataTimeout[ctrl->SdNr] == 0) && (timeOut--));
    }
    else
    {
    	do
    	    {
    	    	if(gpio_in(CardDetectPinMmcSd[ctrl->SdNr]))
    	    	{
    	    		cmdTimeout[ctrl->SdNr] = 0;
    	    		break;
    	    	}
    	    }
    	while ((xferCompFlag[ctrl->SdNr] == 0) && (dataTimeout[ctrl->SdNr] == 0) && (timeOut--));
    }

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
    timeOut = 0xFFFF;
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
    int dma_channel = 0;
    switch(ctrl->SdNr)
    {
	case 0:
		dma_channel = EDMA3_CHA_MMCSD0_RX;
		break;
	case 1:
		dma_channel = EDMA3_CHA_MMCSD1_RX;
		break;
	case 2:
		dma_channel = EDMA3_CHA_MMCSD2_RX;
		break;
    }

    /* Set OPT */
    paramSet.opt |= ((dma_channel << EDMA3CC_OPT_TCC_SHIFT) & EDMA3CC_OPT_TCC);

    /* 1. Transmission complition interrupt enable */
    paramSet.opt |= (1 << EDMA3CC_OPT_TCINTEN_SHIFT);

    /* 2. Read FIFO : SRC Constant addr mode */
    paramSet.opt |= (1 << 0);

    /* 3. SRC FIFO width is 32 bit */
    paramSet.opt |= (2 << 8);

    /* 4.  AB-Sync mode */
    paramSet.opt |= (1 << 2);

    /* configure PaRAM Set */
    EDMA3SetPaRAM(EDMA_INST_BASE, dma_channel, &paramSet);

    /* Enable the transfer */
    EDMA3EnableTransfer(EDMA_INST_BASE, dma_channel, EDMA3_TRIG_MODE_EVENT);
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
    int dma_channel = 0;
    switch(ctrl->SdNr)
    {
	case 0:
		dma_channel = EDMA3_CHA_MMCSD0_TX;
		break;
	case 1:
		dma_channel = EDMA3_CHA_MMCSD1_TX;
		break;
	case 2:
		dma_channel = EDMA3_CHA_MMCSD2_TX;
		break;
    }
    /* Set OPT */
    paramSet.opt |= ((dma_channel << EDMA3CC_OPT_TCC_SHIFT) & EDMA3CC_OPT_TCC);

    /* 1. Transmission complition interrupt enable */
    paramSet.opt |= (1 << EDMA3CC_OPT_TCINTEN_SHIFT);

    /* 2. Read FIFO : DST Constant addr mode */
    paramSet.opt |= (1 << 1);

    /* 3. DST FIFO width is 32 bit */
    paramSet.opt |= (2 << 8);

    /* 4.  AB-Sync mode */
    paramSet.opt |= (1 << 2);

    /* configure PaRAM Set */
    EDMA3SetPaRAM(EDMA_INST_BASE, dma_channel, &paramSet);

    /* Enable the transfer */
    EDMA3EnableTransfer(EDMA_INST_BASE, dma_channel, EDMA3_TRIG_MODE_EVENT);
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
	HSMMCSDIsrGen(&ctrlInfo[0]);
}
void HSMMCSD1Isr(void)
{
	HSMMCSDIsrGen(&ctrlInfo[1]);
}
void HSMMCSD2Isr(void)
{
	HSMMCSDIsrGen(&ctrlInfo[2]);
}



/*
** Initialize the MMCSD controller structure for use
*/
static void HSMMCSDControllerSetup(mmcsdCtrlInfo* SdCtrlStruct, new_gpio* Cs)
{
	//ctrlInfo[SdCtrlStruct->SdNr] = SdCtrlStruct;
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
	SdCtrlStruct->opClk = 400000;
    //sdCard.ctrl = &ctrlInfo;
	SdCtrlStruct->card->ctrl = (mmcsdCtrlInfo*)(void*)&ctrlInfo[SdCtrlStruct->SdNr];

	CardDetectPinMmcSd[SdCtrlStruct->SdNr] = Cs;

    callbackOccured[SdCtrlStruct->SdNr] = 0;
    xferCompFlag[SdCtrlStruct->SdNr] = 0;
    dataTimeout[SdCtrlStruct->SdNr] = 0;
    cmdCompFlag[SdCtrlStruct->SdNr] = 0;
    cmdTimeout[SdCtrlStruct->SdNr] = 0;
}

extern MMC_extCsd extCsd;

void _mmcsd_ioctl(unsigned int unit_nr, unsigned int  command,  unsigned int *buffer)
{
	mmcsdCtrlInfo* _SdCtrlStruct = &ctrlInfo[unit_nr];
    switch(command)
    {

        case GET_SECTOR_COUNT:
        {
           if(_SdCtrlStruct->card->cardType == MMCSD_CARD_SD) *buffer = _SdCtrlStruct->card->nBlks;
           else if(_SdCtrlStruct->card->cardType == MMCSD_CARD_MMC) *buffer = extCsd.sec_count;
           else *buffer = 0;

            break;
        }
        case GET_SECTOR_SIZE:
        {
            *buffer = _SdCtrlStruct->card->blkLen;

            break;
        }
        default:
        {
            *buffer = 0;
            break;
        }

    }
}

void _mmcsd_init(unsigned int unit_nr, new_gpio* Cs, new_gpio* StatusLed)
{
	ctrlInfo[unit_nr].SdNr = unit_nr;
	LedStatusMmcSd[unit_nr] = StatusLed;
	/* Configure EDMA to service the HSMMCSD events. */
    HSMMCSDEdmaInit(unit_nr);

    /* Perform pin-mux for HSMMCSD pins. */
    //HSMMCSDPinMuxSetup();
    pin_mux_mmcsd_beaglebone(unit_nr);

    /* Enable module clock for HSMMCSD. */
    HSMMCSDModuleClkConfig(unit_nr);

    /* Basic controller initializations */
    HSMMCSDControllerSetup(&ctrlInfo[unit_nr], Cs);

    /* Initialize the MMCSD controller */
    MMCSDCtrlInit(&ctrlInfo[unit_nr]);

    MMCSDIntEnable(&ctrlInfo[unit_nr]);
}

void _mmcsd_idle(unsigned int unit_nr)
{
    if(!gpio_in(CardDetectPinMmcSd[unit_nr])/*(HSMMCSDCardPresent(&ctrlInfo)) == 1*/ || (int)(CardDetectPinMmcSd[unit_nr]) == -1)
    {
        if(initFlg[unit_nr])
        {
            initFlg[unit_nr] = 0;
        	if(MMCSDCardInit(&ctrlInfo[unit_nr]))
        	{
        		ctrlInfo[unit_nr].connected = true;
                g_s_mmcFatFs[unit_nr].drv_rw_func.DriveStruct = &ctrlInfo[unit_nr];
                g_s_mmcFatFs[unit_nr].drv_rw_func.drv_r_func = MMCSDReadCmdSend;
                g_s_mmcFatFs[unit_nr].drv_rw_func.drv_w_func = MMCSDWriteCmdSend;
                if(!f_mount(unit_nr, &g_s_mmcFatFs[unit_nr]))
                {
#ifdef MMCSD_DEBUG_EN
                    if(f_opendir(&g_sDirObject, g_cCwdBuf[unit_nr]) == FR_OK)
                    {
						if(DebugCom)
						{
																				UARTprintf(DebugCom,   "MMCSD%d drive %d mounted\n\r" , unit_nr , unit_nr);
																				UARTprintf(DebugCom,   "MMCSD%d Fat fs detected\n\r" , unit_nr);
																				UARTprintf(DebugCom, "MMCSD%d Fs type:                 " , unit_nr);
							if(g_s_mmcFatFs[unit_nr].fs_type == FS_FAT12)	{ 				UARTprintf(DebugCom, "Fat12");}
							else if(g_s_mmcFatFs[unit_nr].fs_type == FS_FAT16){ 				UARTprintf(DebugCom, "Fat16");}
							else if(g_s_mmcFatFs[unit_nr].fs_type == FS_FAT32){ 				UARTprintf(DebugCom, "Fat32");}
							else								{ 				UARTprintf(DebugCom, "None");}
																				UARTprintf(DebugCom, "\n\r");
																				//UARTprintf(DebugCom, "MMCSD0 BootSectorAddress:       %u \n\r",(unsigned int)g_sFatFs.);
																				UARTprintf(DebugCom, "MMCSD%d BytesPerSector:          %d \n\r",unit_nr, /*(int)g_sFatFs.s_size*/512);
																				UARTprintf(DebugCom, "MMCSD%d SectorsPerCluster:       %d \n\r",unit_nr, (int)g_s_mmcFatFs[unit_nr].csize);
																				//UARTprintf(DebugCom, "MMCSD0 AllocTable1Begin:        %u \n\r",(unsigned int)g_sFatFs.fatbase);
																				UARTprintf(DebugCom, "MMCSD%d NumberOfFats:            %d \n\r",unit_nr, (int)g_s_mmcFatFs[unit_nr].n_fats);
																				//UARTprintf(DebugCom, "MMCSD0 MediaType:               %d \n\r",Drives_Table[0]->DiskInfo_MediaType);
																				//UARTprintf(DebugCom, "MMCSD0 AllocTableSize:          %u \n\r",Drives_Table[0]->DiskInfo_AllocTableSize);
																				UARTprintf(DebugCom, "MMCSD%d DataSectionBegin:        %d \n\r",unit_nr, (int)g_s_mmcFatFs[unit_nr].fatbase);
																				UARTprintf(DebugCom, "MMCSD%d uSD DiskCapacity:        %uMB\n\r",unit_nr, (unsigned long)((unsigned long long)((unsigned long long)g_s_mmcFatFs[unit_nr].n_fatent * (unsigned long long)/*g_sFatFs.s_size*/512 *(unsigned long long)g_s_mmcFatFs[unit_nr].csize) / 1000000));
						}
                    } else  if(DebugCom)										UARTprintf(DebugCom,   "MMCSD%d ERROR oppening path\n\r" , unit_nr);
#endif
                }
                else if(DebugCom)												UARTprintf(DebugCom,   "MMCSD%d ERROR mounting disk\n\r" , unit_nr);
        	}
        }
    }
    else
    {
        Sysdelay(1);
        if(initFlg[unit_nr] != 1)
        {
        	ctrlInfo[unit_nr].connected = false;
        	initFlg[unit_nr] = 1;
#ifdef mmcsd_debug
#ifndef thirdpartyfatfs
        	if(_FatData_CloseSesion(FILE1) == 1 && DebugCom != NULL)						UARTPuts(DebugCom, "MMCSD0 Session closed\n\r" , -1);
        	FILE1 = NULL;
        	if(_Fat_Unmount(0) == 1 && DebugCom != NULL)									UARTPuts(DebugCom, "MMCSD0 unmount\n\r" , -1);

#else
        	UARTprintf(DebugCom,   "MMCSD%d Disconnected\n\r" , ((mmcsdCtrlInfo*)SdCtrlStruct)->SdNr);
#endif
#endif
        	/* Reinitialize all the state variables */
            callbackOccured[unit_nr] = 0;
            xferCompFlag[unit_nr] = 0;
            dataTimeout[unit_nr] = 0;
            cmdCompFlag[unit_nr] = 0;
            cmdTimeout[unit_nr] = 0;
            /* Initialize the MMCSD controller */
            MMCSDCtrlInit(&ctrlInfo[unit_nr]);
            MMCSDIntEnable(&ctrlInfo[unit_nr]);
        }
    }
}

