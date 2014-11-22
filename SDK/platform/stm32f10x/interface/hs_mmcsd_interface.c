/*
 * hs_mmcsd_interface.c
 *
 * Created: 2/11/2013 3:10:04 PM
 *  Author: XxXx
 */
//#######################################################################################
#include <stdbool.h>
#include "sys/plat_properties.h"
#include "hs_mmcsd_interface.h"
#include "api/gpio_def.h"
#include "api/gpio_api.h"
#include "api/uart_def.h"
#include "api/uart_api.h"
#include "lib/fs/fat.h"
//#######################################################################################
void _mmcsd_ioctl(void *SdCtrlStruct, unsigned int  command,  unsigned int *buffer)
{
	//mmcsdCtrlInfo* _SdCtrlStruct = (mmcsdCtrlInfo*)SdCtrlStruct;
/*    switch(command)
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

    }*/
}
unsigned int MMCSDWriteCmdSend(void *_ctrl, void *ptr, unsigned long block, unsigned int nblks)
{
	return 0;
}
unsigned int MMCSDReadCmdSend(void *_ctrl, void *ptr, unsigned long block, unsigned int nblks)
{
	return 0;
}
bool _mmcsd_init(void *SdStruct, signed int CardDetectPortNr, signed int CardDetectPinNr, new_gpio* StatusLed)
{
	return false;
}
void _mmcsd_idle(void *SdStruct)
{

}
//#######################################################################################
