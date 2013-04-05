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

#include <string.h>
#include "board_properties.h"
#include "hs_mmcsd_interface.h"
#include "sys/sysdelay.h"
#include "sys/cache.h"
#include "lib/fs/fat.h"
#include "api/gpio_api.h"
#include "api/gpio_def.h"
#include "api/uart_api.h"
#include "driver/mmcsd.h"

new_gpio *CardDetectPinMmcSd0 = NULL;
new_gpio *LedStatusMmcSd0 = NULL;
extern new_uart* DebugCom;
extern FileInfo_t *FILE1;
volatile unsigned int initFlg = 1;

unsigned int MMCSDReadCmdSend(void *_ctrl, void *ptr, unsigned long block, unsigned int nblks)
{
	if(LedStatusMmcSd0) gpio_out(LedStatusMmcSd0, 1);
	return (unsigned int)sd_read_page(ptr, block, nblks);
	if(LedStatusMmcSd0) gpio_out(LedStatusMmcSd0, 0);
}

unsigned int MMCSDWriteCmdSend(void *_ctrl, void *ptr, unsigned long block, unsigned int nblks)
{
	if(LedStatusMmcSd0) gpio_out(LedStatusMmcSd0, 1);
	return (unsigned int)sd_write_page(block, nblks, ptr);
	if(LedStatusMmcSd0) gpio_out(LedStatusMmcSd0, 0);
}

void _mmcsd_init(void *SdStruct, signed int CardDetectPortNr, signed int CardDetectPinNr, new_gpio* StatusLed)
{

	LedStatusMmcSd0 = StatusLed;

    CardDetectPinMmcSd0 = new_(new_gpio);
    CardDetectPinMmcSd0 = gpio_assign(CardDetectPortNr, CardDetectPinNr, dir_in);

    mmc_controller_init();
}

void _mmcsd_idle(void *SdStruct)
{
    if(!gpio_in(CardDetectPinMmcSd0)/*(HSMMCSDCardPresent(&ctrlInfo)) == 1*/)
    {
        if(initFlg)
        {
        	if(mmc_card_init(0))
        	{
                Drives_Table[0] = new_(new_fat_disk);
                Drives_Table[0]->DiskInfo_SdDriverStructAddr = (void*)NULL;
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
						if(Drives_Table[0]->DiskInfo_FatType == Fat32)		UARTprintf(DebugCom, "MMCSD0 uSD DiskCapacity:        %uMB\n\r",(Drives_Table[0]->DiskInfo_AllocTableSize * 128 * Drives_Table[0]->DiskInfo_SectorsPerCluster * Drives_Table[0]->DiskInfo_BytesPerSector) / 1000000);
						else if(Drives_Table[0]->DiskInfo_FatType == Fat16)	UARTprintf(DebugCom, "MMCSD0 uSD DiskCapacity:        %uMB\n\r",(Drives_Table[0]->DiskInfo_AllocTableSize * 256 * Drives_Table[0]->DiskInfo_SectorsPerCluster * Drives_Table[0]->DiskInfo_BytesPerSector) / 1000000);
						FILE1 = _FatData_OpenSesion(0);
						if(FILE1) UARTPuts(DebugCom, "Fat File Sesion Init OK\n\r", -1);
						else UARTPuts(DebugCom, "Fat File Sesion Init ERROR\n\r", -1);
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
        //Sysdelay(1);
        if(initFlg != 1)
        {
        	if(_FatData_CloseSesion(FILE1) == 1 && DebugCom != NULL)						UARTPuts(DebugCom, "MMCSD0 Session closed\n\r" , -1);
        	FILE1 = NULL;
        	if(_Fat_Unmount(0) == 1 && DebugCom != NULL)									UARTPuts(DebugCom, "MMCSD0 unmount\n\r" , -1);
        	/* Reinitialize all the state variables */
        }
        initFlg = 1;
    }
}

