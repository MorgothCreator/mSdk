/*
 * mmcsd_spi.c
 *
 *  Created on: Feb 9, 2016
 *      Author: John Smith
 */


#include <stdbool.h>
//#include "board_properties.h"
#include "mmcsd_spi.h"
#include "api/gpio_def.h"
#include "api/gpio_api.h"
#include "api/uart_def.h"
#include "api/uart_api.h"
#include "driver/uart.h"
//#include "driver/dma.h"
#include "lib/fs/fat.h"
#include "general/unions.h"
#include "sys/sysdelay.h"
//#######################################################################################
//new_dma_ch* DMA_MMCSD_TRANSMIT = NULL;
//new_dma_ch* DMA_MMCSD_RECEIVE = NULL;

#define PATH_BUF_SIZE   4
//static char g_cCwdBuf0[PATH_BUF_SIZE] = "0:/";
//static char g_cCwdBuf1[PATH_BUF_SIZE] = "1:/";
//char *g_cCwdBuf[2] = {g_cCwdBuf0, g_cCwdBuf1};
FATFS g_s_mmcFatFs[3];
DIR g_sDirObject;

/* MMC/SD command */
#define CMD0	(0)			/* GO_IDLE_STATE */
#define CMD1	(1)			/* SEND_OP_COND (MMC) */
#define	ACMD41	(0x80+41)	/* SEND_OP_COND (SDC) */
#define CMD8	(8)			/* SEND_IF_COND */
#define CMD9	(9)			/* SEND_CSD */
#define CMD10	(10)		/* SEND_CID */
#define CMD12	(12)		/* STOP_TRANSMISSION */
#define ACMD13	(0x80+13)	/* SD_STATUS (SDC) */
#define CMD16	(16)		/* SET_BLOCKLEN */
#define CMD17	(17)		/* READ_SINGLE_BLOCK */
#define CMD18	(18)		/* READ_MULTIPLE_BLOCK */
#define CMD23	(23)		/* SET_BLOCK_COUNT (MMC) */
#define	ACMD23	(0x80+23)	/* SET_WR_BLK_ERASE_COUNT (SDC) */
#define CMD24	(24)		/* WRITE_BLOCK */
#define CMD25	(25)		/* WRITE_MULTIPLE_BLOCK */
#define CMD32	(32)		/* ERASE_ER_BLK_START */
#define CMD33	(33)		/* ERASE_ER_BLK_END */
#define CMD38	(38)		/* ERASE */
#define CMD55	(55)		/* APP_CMD */
#define CMD58	(58)		/* READ_OCR */

/* Card type flags (CardType) */
#define CT_MMC		0x01		/* MMC ver 3 */
#define CT_SD1		0x02		/* SD ver 1 */
#define CT_SD2		0x04		/* SD ver 2 */
#define CT_SDC		(CT_SD1|CT_SD2)	/* SD */
#define CT_BLOCK	0x08		/* Block addressing */




new_uart* MMCSD = NULL;
extern new_uart* DebugCom;
extern new_mmcsd_spi *MMCSD_SPI[];
//#######################################################################################
uint8_t sd_io_data(SD_Struct_t *SD_Struct, uint8_t Value)
{
	return SD_Struct->HardUnitReadWriteCharFunc(SD_Struct->HardUnitStruct, Value);//usart_spi_data((Uart_t *)Struct, Value);
}
//#######################################################################################
/*static void sd_delay(uint16_t DelayMs)
{
	unsigned int cnt = 0;
	for(cnt = 0; cnt < 1000; cnt++);
	//Sysdelay(DelayMs);
}*/
//#######################################################################################
void sd_cs_assert(SD_Struct_t *SD_Struct)
{
	gpio_out(SD_Struct->CS_Port, 0);
}
//#######################################################################################
void sd_cs_deassert(SD_Struct_t *SD_Struct)
{
	gpio_out(SD_Struct->CS_Port, 1);
}
//#######################################################################################
uint8_t sd_cs_readstate(SD_Struct_t *SD_Struct)
{
	return gpio_in(SD_Struct->CS_Port);;
}
//#######################################################################################
/*static uint8_t sd_read_idle_bytes(SD_Struct_t *SD_Struct, uint16_t BytesNr)
{
	uint8_t Temp = 255;
	for (uint16_t IdleBytesCount = 0; IdleBytesCount < BytesNr;IdleBytesCount++)
	{
		Temp = sd_io_data(SD_Struct,255);
		if(Temp != 255)
			break;
	}
	 return Temp;
}
//#######################################################################################
static void sd_idle_bytes(SD_Struct_t *SD_Struct, uint16_t BytesNr)
{
	for (uint16_t IdleBytesCount = 0; IdleBytesCount < BytesNr;IdleBytesCount++)
	{
		sd_io_data(SD_Struct,255);
	}
}*/
//#######################################################################################
/*-----------------------------------------------------------------------*/
/* Wait for card ready                                                   */
/*-----------------------------------------------------------------------*/
int wait_ready (	/* 1:Ready, 0:Timeout */
	SD_Struct_t *SD_Struct,
	UINT wt			/* Timeout [ms] */
)
{
	BYTE d;

	unsigned int Timer2 = wt;
	do {
		d = sd_io_data(SD_Struct, 0xFF);
		/* This loop takes a time. Insert rot_rdq() here for multitask envilonment. */
		//sd_delay(1);
	} while (d != 0xFF && Timer2--);	/* Wait for card goes ready or timeout */

	return (d == 0xFF) ? 1 : 0;
}

/*-----------------------------------------------------------------------*/
/* Deselect card and release SPI                                         */
/*-----------------------------------------------------------------------*/

void deselect (SD_Struct_t *SD_Struct)
{
	sd_cs_deassert(SD_Struct);		/* CS = H */
	sd_io_data(SD_Struct, 0xFF);	/* Dummy clock (force DO hi-z for multiple slave SPI) */
}



/*-----------------------------------------------------------------------*/
/* Select card and wait for ready                                        */
/*-----------------------------------------------------------------------*/

int select (SD_Struct_t *SD_Struct)	/* 1:OK, 0:Timeout */
{
	sd_cs_assert(SD_Struct);
	sd_io_data(SD_Struct, 0xFF);	/* Dummy clock (force DO enabled) */

	if (wait_ready(SD_Struct, 50000))
		return 1;	/* OK */
	deselect(SD_Struct);
	return 0;	/* Timeout */
}
/*-----------------------------------------------------------------------*/
/* Send a command packet to the MMC                                      */
/*-----------------------------------------------------------------------*/

BYTE send_cmd (		/* Return value: R1 resp (bit7==1:Failed to send) */
	SD_Struct_t *SD_Struct,
	BYTE cmd,		/* Command index */
	DWORD arg		/* Argument */
)
{
	BYTE n, res;


	if (cmd & 0x80)
	{	/* Send a CMD55 prior to ACMD<n> */
		cmd &= 0x7F;
		res = send_cmd(SD_Struct, CMD55, 0);
		if (res > 1) return res;
	}

	/* Select card */
	deselect(SD_Struct);
	if (!select(SD_Struct))
		return 0xFF;

	/* Send command packet */
	sd_io_data(SD_Struct, 0x40 | cmd);				/* Start + command index */
	sd_io_data(SD_Struct, (BYTE)(arg >> 24));		/* Argument[31..24] */
	sd_io_data(SD_Struct, (BYTE)(arg >> 16));		/* Argument[23..16] */
	sd_io_data(SD_Struct, (BYTE)(arg >> 8));			/* Argument[15..8] */
	sd_io_data(SD_Struct, (BYTE)arg);				/* Argument[7..0] */
	n = 0x01;							/* Dummy CRC + Stop */
	if (cmd == CMD0)
		n = 0x95;			/* Valid CRC for CMD0(0) */
	if (cmd == CMD8)
		n = 0x87;			/* Valid CRC for CMD8(0x1AA) */
	sd_io_data(SD_Struct, n);

	/* Receive command resp */
	if (cmd == CMD12)
		sd_io_data(SD_Struct, 0xFF);	/* Diacard following one byte when CMD12 */
	n = 10;								/* Wait for response (10 bytes max) */
	do
		res = sd_io_data(SD_Struct, 0xFF);
	while ((res & 0x80) && --n);

	return res;							/* Return received response */
}

bool _mmcsd_spi_init(unsigned int unit_nr)
{
	SD_Struct_t *SD_Struct = MMCSD_SPI[unit_nr];
	SD_Struct->HardUnitSetBaudFunc((void *)SD_Struct->HardUnitStruct, 400000);			/* Set fast clock */
	sd_cs_deassert(SD_Struct);
	BYTE n, cmd, ty, ocr[4];
	SD_Struct->SD_Hc = IsSd;
	for (n = 10; n; n--)
		sd_io_data(SD_Struct, 0xFF);	/* Send 80 dummy clocks */

	ty = 0;
	if (send_cmd(SD_Struct, CMD0, 0) == 1)
	{			/* Put the card SPI/Idle state */
		unsigned int Timer1 = 100000;						/* Initialization timeout = 1 sec */
		if (send_cmd(SD_Struct, CMD8, 0x1AA) == 1)
		{	/* SDv2? */
			for (n = 0; n < 4; n++) ocr[n] = sd_io_data(SD_Struct, 0xFF);	/* Get 32 bit return value of R7 resp */
			if (ocr[2] == 0x01 && ocr[3] == 0xAA)
			{				/* Is the card supports vcc of 2.7-3.6V? */
				while (Timer1-- && send_cmd(SD_Struct, ACMD41, 1UL << 30)) ;	/* Wait for end of initialization with ACMD41(HCS) */
				if (Timer1 && send_cmd(SD_Struct, CMD58, 0) == 0)
				{		/* Check CCS bit in the OCR */
					for (n = 0; n < 4; n++)
						ocr[n] = sd_io_data(SD_Struct, 0xFF);
					ty = (ocr[0] & 0x40) ? CT_SD2 | CT_BLOCK : CT_SD2;	/* Card id SDv2 */
					if(ty & CT_BLOCK)
						SD_Struct->SD_Hc = IsSdhc;
				}
			}
		} else {	/* Not SDv2 card */
			if (send_cmd(SD_Struct, ACMD41, 0) <= 1)
			{	/* SDv1 or MMC? */
				ty = CT_SD1; cmd = ACMD41;	/* SDv1 (ACMD41(0)) */
			} else
			{
				ty = CT_MMC; cmd = CMD1;	/* MMCv3 (CMD1(0)) */
			}
			while (Timer1-- && send_cmd(SD_Struct, cmd, 0)) ;		/* Wait for end of initialization */
			if (!Timer1 || send_cmd(SD_Struct, CMD16, 512) != 0)	/* Set block length: 512 */
				ty = 0;
		}
	}
	deselect(SD_Struct);

	if (ty)
	{			/* OK */
		SD_Struct->HardUnitSetBaudFunc((void *)SD_Struct->HardUnitStruct, 10000000);			/* Set fast clock */
		SD_Struct->SD_Init_OK = true;
	} else
	{			/* Failed */
		//power_off();
		SD_Struct->SD_Init_OK = false;
	}

	//return Stat;

	//SD_Struct->HardUnitSetBaudFunc((void *)SD_Struct->HardUnitStruct, 4000000);
	sd_cs_deassert(SD_Struct);
	return SD_Struct->SD_Init_OK;
}

//#######################################################################################
static inline bool rcvr_datablock(void *_SD_Struct, unsigned char *buff, unsigned int bytes_to_read)
{
	SD_Struct_t *SD_Struct = (SD_Struct_t *)_SD_Struct;
	unsigned char token;
	//unsigned char Timer1 = 10;
	do
	{							/* Wait for data packet in timeout of 100ms */
		token = sd_io_data(SD_Struct,255);
	} while ((token == 0xFF)/* && Timer1--*/);

	if(token != MMC_DATA_TOKEN)
		return false;	/* If not valid data token, return with error */

	do
	{							/* Receive the data block into buffer */
		*buff++ = sd_io_data(SD_Struct,255);
	} while (--bytes_to_read);

	sd_io_data(SD_Struct,255);						/* Discard CRC */
	sd_io_data(SD_Struct,255);

	return true;					/* Return with success */
}
//#######################################################################################
unsigned int _sd_read_page(void *_SD_Struct, void* _Buffer, unsigned long block, unsigned int nblks)
{
	SD_Struct_t *SD_Struct = (SD_Struct_t *)_SD_Struct;
	if (/*drv || */!nblks) return false;		/* Check parameter */
	if (SD_Struct->SD_Init_OK == false) return false;	/* Check if drive is ready */

	if (SD_Struct->SD_Hc == IsSd)
		block *= 512;	/* LBA ot BA conversion (byte addressing cards) */

	if (nblks == 1)
	{	/* Single sector read */
		if ((send_cmd(SD_Struct, CMD17, block) == 0)	/* READ_SINGLE_BLOCK */
			&& rcvr_datablock(SD_Struct, _Buffer, 512))
			nblks = 0;
	}
	else
	{				/* Multiple sector read */
		if (send_cmd(SD_Struct, CMD18, block) == 0)
		{	/* READ_MULTIPLE_BLOCK */
			do
			{
				if (!rcvr_datablock(SD_Struct, _Buffer, 512)) break;
				_Buffer += 512;
			} while (--block);
			send_cmd(SD_Struct, CMD12, 0);				/* STOP_TRANSMISSION */
		}
	}
	deselect(SD_Struct);
	return nblks ? false : true;	/* Return result */
}
//#######################################################################################
unsigned int MMCSD_SPI_ReadCmdSend(void *_SD_Struct, void* _Buffer, unsigned long _block, unsigned int nblks)
{
	unsigned long block = _block;
	unsigned char* Buffer = (unsigned char*)_Buffer;
	//unsigned int TransferedBlocks = 0;
	do
	{
		if(!_sd_read_page(_SD_Struct, Buffer/* + (512*TransferedBlocks)*/, block++, 1)) return false;
		Buffer += 512;
		//TransferedBlocks++;
	} while (--nblks);
	return true;
}
//#######################################################################################
/* Send multiple byte */
static
void xmit_spi_multi (
	SD_Struct_t *SD_Struct,
	const BYTE *buff,	/* Pointer to the data */
	UINT btx			/* Number of bytes to send (512) */
)
{
	do {						/* Transmit data block */
		sd_io_data(SD_Struct, *buff++);
	} while (btx -= 1);
}
/*-----------------------------------------------------------------------*/
/* Send a data packet to the MMC                                         */
/*-----------------------------------------------------------------------*/

static
int xmit_datablock (	/* 1:OK, 0:Failed */
	SD_Struct_t *SD_Struct,
	const BYTE *buff,	/* Ponter to 512 byte data to be sent */
	BYTE token			/* Token */
)
{
	BYTE resp;


	if (!wait_ready(SD_Struct, 500)) return 0;		/* Wait for card ready */

	sd_io_data(SD_Struct, token);					/* Send token */
	if (token != 0xFD) {				/* Send data if token is other than StopTran */
		xmit_spi_multi(SD_Struct, buff, 512);		/* Data */
		sd_io_data(SD_Struct, 0xFF);
		sd_io_data(SD_Struct, 0xFF);	/* Dummy CRC */

		resp = sd_io_data(SD_Struct, 0xFF);				/* Receive data resp */
		if ((resp & 0x1F) != 0x05)		/* Function fails if the data packet was not accepted */
			return 0;
	}
	return 1;
}


unsigned int  _sd_write_page(void *_SD_Struct, void* _Buffer, unsigned long block, unsigned int nblks)
{
	SD_Struct_t *SD_Struct = (SD_Struct_t *)_SD_Struct;
	if (/*drv || */!nblks) return false;		/* Check parameter */
	if (SD_Struct->SD_Init_OK == false) return false;	/* Check if drive is ready */

	if (SD_Struct->SD_Hc == IsSd)
		block *= 512;	/* LBA ot BA conversion (byte addressing cards) */

	if (nblks == 1)
	{	/* Single sector write */
		if ((send_cmd(SD_Struct, CMD24, block) == 0)	/* WRITE_BLOCK */
			&& xmit_datablock(SD_Struct, _Buffer, 0xFE))
			nblks = 0;
	}
	else {				/* Multiple sector write */
		if (SD_Struct->SD_Hc == IsSd)
			send_cmd(SD_Struct, ACMD23, nblks);	/* Predefine number of sectors */
		if (send_cmd(SD_Struct, CMD25, block) == 0)
		{	/* WRITE_MULTIPLE_BLOCK */
			do
			{
				if (!xmit_datablock(SD_Struct, _Buffer, 0xFC)) break;
				_Buffer += 512;
			} while (--nblks);
			if (!xmit_datablock(SD_Struct, 0, 0xFD))	/* STOP_TRAN token */
				nblks = 1;
		}
	}
	deselect(SD_Struct);
	return nblks ? false : true;	/* Return result */
}
//#######################################################################################
unsigned int MMCSD_SPI_WriteCmdSend(void *_SD_Struct, void* _Buffer, unsigned long _block, unsigned int nblks)
{
	unsigned long block = _block;
	unsigned char* Buffer = (unsigned char*)_Buffer;
	//unsigned int TransferedBlocks = 0;
	do
	{
		if(!_sd_write_page(_SD_Struct, Buffer/* + (512*TransferedBlocks)*/, block++, 1)) return false;
		Buffer += 512;
		//TransferedBlocks++;
	} while (--nblks);
	return true;
}
//#######################################################################################
void mmcsd_spi_init(unsigned int unit_nr, new_gpio* Cs, new_gpio* StatusLed)
{
	SD_Struct_t *SD_Struct = MMCSD_SPI[unit_nr];
	SD_Struct->CS_Port = Cs;
	SD_Struct->initFlg = 1;
	_mmcsd_spi_init(unit_nr);
	return;
}
//#######################################################################################
void mmcsd_spi_idle(unsigned int unit_nr)
{
	SD_Struct_t *SD_StructDisk = MMCSD_SPI[unit_nr];
	/*//SD_Struct_t *SD_StructDisk = (SD_Struct_t *)SdStruct;
	Drives_Table[SD_StructDisk->DriveNr] = new_(new_fat_disk);
    Drives_Table[SD_StructDisk->DriveNr]->DiskInfo_SdDriverStructAddr = SD_StructDisk;
    //Drives_Table[0]->drive_init = MMCSD_CardInit;
    Drives_Table[SD_StructDisk->DriveNr]->drive_read_page = MMCSD_SPI_ReadCmdSend;
    Drives_Table[SD_StructDisk->DriveNr]->drive_write_page = MMCSD_SPI_WriteCmdSend;
    if(_Fat_Mount(SD_StructDisk->DriveNr))
    {
        if(DebugCom)
        {
																					UARTprintf(DebugCom, "MMCSD%d drive mounted\n\r" , (signed long)SD_StructDisk->DriveNr);
																					UARTprintf(DebugCom, "MMCSD%d Fat fs detected\n\r" , (signed long)SD_StructDisk->DriveNr);
			if(Drives_Table[SD_StructDisk->DriveNr]->DiskInfo_FatType == NoFs ){ 	UARTprintf(DebugCom, "MMCSD%d Fs type:                 None\n\r" , (signed long)SD_StructDisk->DriveNr);}
			if(Drives_Table[SD_StructDisk->DriveNr]->DiskInfo_FatType == Fat12){ 	UARTprintf(DebugCom, "MMCSD%d Fs type:                 Fat12\n\r" , (signed long)SD_StructDisk->DriveNr);}
			if(Drives_Table[SD_StructDisk->DriveNr]->DiskInfo_FatType == Fat16){ 	UARTprintf(DebugCom, "MMCSD%d Fs type:                 Fat16\n\r" , (signed long)SD_StructDisk->DriveNr);}
			if(Drives_Table[SD_StructDisk->DriveNr]->DiskInfo_FatType == Fat32){ 	UARTprintf(DebugCom, "MMCSD%d Fs type:                 Fat32\n\r" , (signed long)SD_StructDisk->DriveNr);}
																					UARTprintf(DebugCom, "MMCSD%d BootSectorAddress:       %u \n\r" , (signed long)SD_StructDisk->DriveNr, (unsigned long)Drives_Table[SD_StructDisk->DriveNr]->DiskInfo_BootSectorAddress);
																					UARTprintf(DebugCom, "MMCSD%d BytesPerSector:          %d \n\r" , (signed long)SD_StructDisk->DriveNr, (signed long)Drives_Table[SD_StructDisk->DriveNr]->DiskInfo_BytesPerSector);
																					UARTprintf(DebugCom, "MMCSD%d SectorsPerCluster:       %d \n\r" , (signed long)SD_StructDisk->DriveNr, (signed long)Drives_Table[SD_StructDisk->DriveNr]->DiskInfo_SectorsPerCluster);
																					UARTprintf(DebugCom, "MMCSD%d AllocTable1Begin:        %u \n\r" , (signed long)SD_StructDisk->DriveNr, (unsigned long)Drives_Table[SD_StructDisk->DriveNr]->DiskInfo_AllocTable1Begin);
																					UARTprintf(DebugCom, "MMCSD%d NumberOfFats:            %d \n\r" , (signed long)SD_StructDisk->DriveNr, (signed long)Drives_Table[SD_StructDisk->DriveNr]->DiskInfo_NumberOfFats);
																					UARTprintf(DebugCom, "MMCSD%d MediaType:               %d \n\r" , (signed long)SD_StructDisk->DriveNr, (signed long)Drives_Table[SD_StructDisk->DriveNr]->DiskInfo_MediaType);
																					UARTprintf(DebugCom, "MMCSD%d AllocTableSize:          %u \n\r" , (signed long)SD_StructDisk->DriveNr, (unsigned long)Drives_Table[SD_StructDisk->DriveNr]->DiskInfo_AllocTableSize);
																					UARTprintf(DebugCom, "MMCSD%d DataSectionBegin:        %u \n\r" , (signed long)SD_StructDisk->DriveNr, (unsigned long)Drives_Table[SD_StructDisk->DriveNr]->DiskInfo_DataSectionBegin);
			if(Drives_Table[0]->DiskInfo_FatType == Fat32)							UARTprintf(DebugCom, "MMCSD%d uSD DiskCapacity:        %uMB\n\r" , (signed long)SD_StructDisk->DriveNr, (unsigned long)((unsigned long long)((unsigned long long)Drives_Table[SD_StructDisk->DriveNr]->DiskInfo_AllocTableSize * 128 * (unsigned long long)Drives_Table[SD_StructDisk->DriveNr]->DiskInfo_SectorsPerCluster * (unsigned long long)Drives_Table[SD_StructDisk->DriveNr]->DiskInfo_BytesPerSector) / 1000000));
			else if(Drives_Table[0]->DiskInfo_FatType == Fat16)						UARTprintf(DebugCom, "MMCSD%d uSD DiskCapacity:        %uMB\n\r" , (signed long)SD_StructDisk->DriveNr, (unsigned long)((unsigned long long)((unsigned long long)Drives_Table[SD_StructDisk->DriveNr]->DiskInfo_AllocTableSize * 256 * (unsigned long long)Drives_Table[SD_StructDisk->DriveNr]->DiskInfo_SectorsPerCluster * (unsigned long long)Drives_Table[SD_StructDisk->DriveNr]->DiskInfo_BytesPerSector) / 1000000));
        }
    }
    else if(DebugCom)																UARTprintf(DebugCom,   "MMCSD%d Fat not detected\n\r" , (signed long)SD_StructDisk->DriveNr);
*/




    if(!gpio_in(SD_StructDisk->SD_Present)/*(HSMMCSDCardPresent(&ctrlInfo)) == 1*/ || (int)(SD_StructDisk->SD_Present) == 0 || (int)(SD_StructDisk->SD_Present) == -1)
    {
        if(SD_StructDisk->initFlg)
        {
        	SD_StructDisk->initFlg = 0;
        	if(_mmcsd_spi_init(unit_nr))
        	{
        		SD_StructDisk->connected = true;
        		SD_StructDisk->g_s_mmcFatFs.drv_rw_func.DriveStruct = SD_StructDisk;
        		SD_StructDisk->g_s_mmcFatFs.drv_rw_func.drv_r_func = MMCSD_SPI_ReadCmdSend;
        		SD_StructDisk->g_s_mmcFatFs.drv_rw_func.drv_w_func = MMCSD_SPI_WriteCmdSend;
                if(!f_mount(unit_nr, &SD_StructDisk->g_s_mmcFatFs))
                {
#ifdef MMCSD_DEBUG_EN
                    if(f_opendir(&g_sDirObject, "0:/") == FR_OK)
                    {
						if(DebugCom)
						{
																				UARTprintf(DebugCom,   "MMCSD%d drive %d mounted\n\r" , unit_nr , unit_nr);
																				UARTprintf(DebugCom,   "MMCSD%d Fat fs detected\n\r" , unit_nr);
																				UARTprintf(DebugCom, "MMCSD%d Fs type:                 " , unit_nr);
							if(SD_StructDisk->g_s_mmcFatFs.fs_type == FS_FAT12)	{ 				UARTprintf(DebugCom, "Fat12");}
							else if(SD_StructDisk->g_s_mmcFatFs.fs_type == FS_FAT16){ 				UARTprintf(DebugCom, "Fat16");}
							else if(SD_StructDisk->g_s_mmcFatFs.fs_type == FS_FAT32){ 				UARTprintf(DebugCom, "Fat32");}
							else								{ 				UARTprintf(DebugCom, "None");}
																				UARTprintf(DebugCom, "\n\r");
																				//UARTprintf(DebugCom, "MMCSD0 BootSectorAddress:       %u \n\r",(unsigned int)g_sFatFs.);
																				UARTprintf(DebugCom, "MMCSD%d BytesPerSector:          %d \n\r",unit_nr, /*(int)g_sFatFs.s_size*/512);
																				UARTprintf(DebugCom, "MMCSD%d SectorsPerCluster:       %d \n\r",unit_nr, (int)SD_StructDisk->g_s_mmcFatFs.csize);
																				//UARTprintf(DebugCom, "MMCSD0 AllocTable1Begin:        %u \n\r",(unsigned int)g_sFatFs.fatbase);
																				UARTprintf(DebugCom, "MMCSD%d NumberOfFats:            %d \n\r",unit_nr, (int)SD_StructDisk->g_s_mmcFatFs.n_fats);
																				//UARTprintf(DebugCom, "MMCSD0 MediaType:               %d \n\r",Drives_Table[0]->DiskInfo_MediaType);
																				//UARTprintf(DebugCom, "MMCSD0 AllocTableSize:          %u \n\r",Drives_Table[0]->DiskInfo_AllocTableSize);
																				UARTprintf(DebugCom, "MMCSD%d DataSectionBegin:        %d \n\r",unit_nr, (int)SD_StructDisk->g_s_mmcFatFs.fatbase);
																				unsigned long tmp = (unsigned long long)((unsigned long long)SD_StructDisk->g_s_mmcFatFs.n_fatent * (unsigned long long)/*g_sFatFs.s_size*/512 *(unsigned long long)SD_StructDisk->g_s_mmcFatFs.csize) >> 20/* / 1000000*/;
																				UARTprintf(DebugCom, "MMCSD%d uSD DiskCapacity:        %uMB\n\r",unit_nr, tmp);
						}
                    } else  if(DebugCom)										UARTprintf(DebugCom,   "MMCSD%d ERROR oppening path\n\r" , unit_nr);
#endif
                }
                else if(DebugCom)												UARTprintf(DebugCom,   "MMCSD%d ERROR mounting disk\n\r" , unit_nr);
        	}
        	else if(DebugCom)												UARTprintf(DebugCom,   "MMCSD%d not detected\n\r" , unit_nr);
        }
    }
    else
    {
        Sysdelay(1);
        if(SD_StructDisk->initFlg != 1)
        {
        	SD_StructDisk->connected = false;
        	SD_StructDisk->initFlg = 1;
#ifdef mmcsd_debug
#ifndef thirdpartyfatfs
        	if(_FatData_CloseSesion(FILE1) == 1 && DebugCom != NULL)						UARTPuts(DebugCom, "MMCSD0 Session closed\n\r" , -1);
        	FILE1 = NULL;
        	if(_Fat_Unmount(0) == 1 && DebugCom != NULL)									UARTPuts(DebugCom, "MMCSD0 unmount\n\r" , -1);

#else
        	UARTprintf(DebugCom,   "MMCSD%d Disconnected\n\r" , ((mmcsdCtrlInfo*)SdCtrlStruct)->SdNr);
#endif
#endif
        }
    }

}
//#######################################################################################
void mmcsd_spi_ioctl(unsigned int unit_nr, unsigned int  command,  unsigned int *buffer)
{
	*buffer = 0;
}


