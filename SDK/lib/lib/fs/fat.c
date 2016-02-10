/*
 *  lib/fs/fat.c
 *
 *  Copyright (C) 2013  Iulian Gheorghiu <morgoth.creator@gmail.com>
 *
 *  This file is part of Multiplatform SDK.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
//#include <malloc.h>
#include "fat.h"
#include "board_init.h"
#include "sys/plat_properties.h"
#include "api/uart_api.h"
#include "../string_lib.h"
#ifdef USE_UNIONS
#include "general/unions.h"
#endif
//***********************************************************
DiskInfo_t *Drives_Table[32];
//***********************************************************
extern new_uart* DebugCom;
//***********************************************************
/*unsigned long _Fat_IndexReadBeginAddressOfDataTable(FILE *stream)
{
	DiskInfo_t *fatinfo = stream->_cookie;
	return fatinfo->DiskInfo_DataSectionBegin;
}*/
//***********************************************************
/*unsigned long *_SelIndexTable(DiskInfo *fatinfo)
{
	if(fatinfo->DiskInfo_AllocTableSel == 0)
	{
		return (unsigned long*)fatinfo->DiskInfo_Buff1;
	}
	else if(fatinfo->DiskInfo_AllocTableSel == 1)
	{
		return (unsigned long*)fatinfo->DiskInfo_Buff2;
	}
	return 0;
}*/
//***********************************************************
//#include "../../api/uartStdio_api.h"
//extern new_uart* DebugCom;
IndexAllocResult _FatReadFsInfo(DiskInfo_t *fatinfo)
{
	if(!fatinfo) return Fat_NoDiskInserted;
	//DiskInfo_t *fatinfo = stream->_cookie;
	if(fatinfo->drive_read_page(fatinfo->DiskInfo_SdDriverStructAddr,fatinfo->DiskInfo_Buff1, 0, 1) == True);// || RES_WRPRT)
	{
		if((fatinfo->DiskInfo_Buff1[0x0036] == 'F' &&
			fatinfo->DiskInfo_Buff1[0x0036+1] == 'A' &&
			fatinfo->DiskInfo_Buff1[0x0036+2] == 'T') ||
			(fatinfo->DiskInfo_Buff1[0x0052] == 'F' &&
			fatinfo->DiskInfo_Buff1[0x0052+1] == 'A' &&
			fatinfo->DiskInfo_Buff1[0x0052+2] == 'T'))
			{
				fatinfo->DiskInfo_BootSectorAddress = 0;
			}
			else
			{
				unsigned long BootSectorAddress = fatinfo->DiskInfo_Buff1[454] + (fatinfo->DiskInfo_Buff1[454+1]*256) + (fatinfo->DiskInfo_Buff1[454+2]*65536) + (fatinfo->DiskInfo_Buff1[454+3]*16777216);
				fatinfo->DiskInfo_BootSectorAddress = BootSectorAddress;
				if(fatinfo->drive_read_page(fatinfo->DiskInfo_SdDriverStructAddr,fatinfo->DiskInfo_Buff1,BootSectorAddress,1)!= True)
				{
					return Fat_NoFatDetected;
				}
			}
		//UARTPuts(DebugCom, (char*)fatinfo->DiskInfo_Buff1, 512);
		fatinfo->DiskInfo_FatType = NoFs;
#ifdef Fs_Fat12_Support
		if(fatinfo->DiskInfo_Buff1[0x0036] == 'F' &&
			fatinfo->DiskInfo_Buff1[0x0036+1] == 'A' &&
			fatinfo->DiskInfo_Buff1[0x0036+2] == 'T' &&
			fatinfo->DiskInfo_Buff1[0x0036+3] == '1' &&
			fatinfo->DiskInfo_Buff1[0x0036+4] == '2')
				{
					fatinfo->DiskInfo_FatType = Fat12;
				} 
#endif
#if defined (Fs_Fat12_Support) && defined (Fs_Fat16_Support)
				else 
#endif
#ifdef Fs_Fat16_Support
				if(fatinfo->DiskInfo_Buff1[0x0036] == 'F' &&
						fatinfo->DiskInfo_Buff1[0x0036+1] == 'A' &&
						fatinfo->DiskInfo_Buff1[0x0036+2] == 'T' &&
						fatinfo->DiskInfo_Buff1[0x0036+3] == '1' &&
						fatinfo->DiskInfo_Buff1[0x0036+4] == '6')
						{
							fatinfo->DiskInfo_FatType = Fat16;
						} 
#endif
#if defined (Fs_Fat16_Support) && defined (Fs_Fat32_Support)
				else 
#endif
#ifdef Fs_Fat32_Support
						if(fatinfo->DiskInfo_Buff1[0x0052] == 'F' &&
								fatinfo->DiskInfo_Buff1[0x0052+1] == 'A' &&
								fatinfo->DiskInfo_Buff1[0x0052+2] == 'T' &&
								fatinfo->DiskInfo_Buff1[0x0052+3] == '3' &&
								fatinfo->DiskInfo_Buff1[0x0052+4] == '2')
								{
									fatinfo->DiskInfo_FatType = Fat32;
								}
#endif
		if(fatinfo->DiskInfo_FatType == NoFs) fatinfo->DiskInfo_BootSectorAddress = 0;
		else
		{
			fatinfo->DiskInfo_BytesPerSector = (unsigned short)fatinfo->DiskInfo_Buff1[BPB_BytsPerSec]|((unsigned short)fatinfo->DiskInfo_Buff1[BPB_BytsPerSec+1]<<8);
			fatinfo->DiskInfo_SectorsPerCluster = fatinfo->DiskInfo_Buff1[BPB_SecPerClus];
			fatinfo->DiskInfo_AllocTable1Begin = (unsigned long)(fatinfo->DiskInfo_Buff1[BPB_RsvdSecCnt]+(fatinfo->DiskInfo_Buff1[BPB_RsvdSecCnt+1]*256));
			fatinfo->DiskInfo_NumberOfFats = fatinfo->DiskInfo_Buff1[BPB_NumFATs];
			fatinfo->DiskInfo_MediaType = fatinfo->DiskInfo_Buff1[BPB_Media];

			switch(fatinfo->DiskInfo_SectorsPerCluster)
			{
				case 1:
					fatinfo->DiskInfo_SectorPerClusterMask = 0;
					break;
				case 2:
					fatinfo->DiskInfo_SectorPerClusterMask = 1;
					break;
				case 4:
					fatinfo->DiskInfo_SectorPerClusterMask = 2;
					break;
				case 8:
					fatinfo->DiskInfo_SectorPerClusterMask = 3;
					break;
				case 16:
					fatinfo->DiskInfo_SectorPerClusterMask = 4;
					break;
				case 32:
					fatinfo->DiskInfo_SectorPerClusterMask = 5;
					break;
				case 64:
					fatinfo->DiskInfo_SectorPerClusterMask = 6;
					break;
				case 128:
					fatinfo->DiskInfo_SectorPerClusterMask = 7;
					break;
				case 0:
					fatinfo->DiskInfo_SectorPerClusterMask = 8;
					break;
			}
			if(fatinfo->DiskInfo_FatType == Fat32)
			{
				fatinfo->DiskInfo_AllocTableSize = (unsigned long)(fatinfo->DiskInfo_Buff1[BPB_FAT32_FATSz32] + (fatinfo->DiskInfo_Buff1[BPB_FAT32_FATSz32+1]*256) + (fatinfo->DiskInfo_Buff1[BPB_FAT32_FATSz32+2]*65536) + (fatinfo->DiskInfo_Buff1[BPB_FAT32_FATSz32+3]*16777216));
			}
			else
			{
				fatinfo->DiskInfo_AllocTableSize = (unsigned long)(fatinfo->DiskInfo_Buff1[BPB_FATSz16] + (fatinfo->DiskInfo_Buff1[BPB_FATSz16+1]*256));
			}
			fatinfo->DiskInfo_DataSectionBegin = fatinfo->DiskInfo_BootSectorAddress + fatinfo->DiskInfo_AllocTable1Begin + (fatinfo->DiskInfo_AllocTableSize*fatinfo->DiskInfo_NumberOfFats);
			if(fatinfo->DiskInfo_FatType == NoFs) return Fat_NoFatDetected;
			else
			{
				if(fatinfo->DiskInfo_FatType == Fat16 || fatinfo->DiskInfo_FatType == Fat32)
				{
					fatinfo->DiskInfo_IndexNumber = (fatinfo->DiskInfo_AllocTableSize * fatinfo->DiskInfo_BytesPerSector)/(1<<(fatinfo->DiskInfo_FatType-1));
				}
				else
				{
					fatinfo->DiskInfo_IndexNumber = (fatinfo->DiskInfo_AllocTableSize * fatinfo->DiskInfo_BytesPerSector)/3;
				}
				return Fat_FatDetected;
			}
		}		
	}
	return Fat_NoDiskInserted;
}
//***********************************************************
//Ro:	Aceasta functie este folosita pentru a salva fortat bufferul in tabela de allocari
//En:	This function can be use to force save the buffer to allocation table.
unsigned char _Fat_IndexReadFirstBlock(DiskInfo_t *fatinfo)
{
	if(!fatinfo) return False;
	//DiskInfo_t *fatinfo = stream->_cookie;
	fatinfo->drive_read_page(fatinfo->DiskInfo_SdDriverStructAddr,fatinfo->DiskInfo_Buff1,fatinfo->DiskInfo_AllocTable1Begin + fatinfo->DiskInfo_BootSectorAddress, 1);
	fatinfo->DiskInfo_PageNrInBuff = 0;
	return True;
}
//***********************************************************
//Ro:	Aceasta functie este folosita pentru a salva fortat bufferul in tabela de allocari
//En:	This function can be use to force save the buffer to allocation table.
#ifdef Fs_RW
unsigned char _Fat_IndexSaveBuffer(DiskInfo_t *fatinfo)
{
	//DiskInfo_t *fatinfo = stream->udata;
	if((fatinfo->DiskInfo_Flags & (1<<Fat_Index_BufferModified)) == 1)
	{
		fatinfo->drive_write_page(fatinfo->DiskInfo_SdDriverStructAddr,fatinfo->DiskInfo_Buff1,fatinfo->DiskInfo_PageNrInBuff + fatinfo->DiskInfo_AllocTable1Begin + fatinfo->DiskInfo_BootSectorAddress, 1);
		fatinfo->DiskInfo_Flags = fatinfo->DiskInfo_Flags & !(1<<Fat_Index_BufferModified);
		return True;
	}
	return False;
}
#endif
//***********************************************************
//Ro:	Aceasta functie schimba pagina din tabela de alocari in buffer, tot o data si verifica
//daca bufferul este modificat pentru al salva in tabela de alocari
//En:	This function can change pages from Allocation Table in buffer, this function verify if
//the buffer is changed and if is changed this function can save the buffer to alloc table.
void _Fat_IndexChangePageInBuffer(DiskInfo_t *fatinfo, unsigned long PointedIndexPageNumber)
{
	if(!fatinfo) return;
	if(PointedIndexPageNumber != fatinfo->DiskInfo_PageNrInBuff)
	{
#ifdef Fs_RW
		if(fatinfo->DiskInfo_Flags & (1<<Fat_Index_BufferModified))
		{
			fatinfo->drive_write_page(fatinfo->DiskInfo_SdDriverStructAddr,fatinfo->DiskInfo_Buff1,fatinfo->DiskInfo_AllocTable1Begin + fatinfo->DiskInfo_BootSectorAddress + fatinfo->DiskInfo_PageNrInBuff, 1);
			fatinfo->DiskInfo_Flags &= ~(1<<Fat_Index_BufferModified);
		}
#endif
		fatinfo->drive_read_page(fatinfo->DiskInfo_SdDriverStructAddr,fatinfo->DiskInfo_Buff1,fatinfo->DiskInfo_AllocTable1Begin + PointedIndexPageNumber + fatinfo->DiskInfo_BootSectorAddress, 1);
		fatinfo->DiskInfo_PageNrInBuff = PointedIndexPageNumber;
	}
}
//***********************************************************
unsigned long _Fat_IndexReadWrite(DiskInfo_t *fatinfo,unsigned long Fat_IndexNumber,unsigned long Fat_IndexToWrite,unsigned char Fat_IndexRdWrAction)
{
	if(!fatinfo) return 0;
	//DiskInfo_t *fatinfo = stream->_cookie;
	unsigned long PointedIndexPageNumber = Fat_IndexNumber + 2;
	unsigned long PointedIndexInPage = PointedIndexPageNumber;
	unsigned char Fat_Type = fatinfo->DiskInfo_FatType;
#ifdef Fs_RW
	//convert32to8 IndexAddrWrite;
#endif
	switch (Fat_Type)
	{
#ifdef Fs_Fat12_Support
		case Fat12:
			PointedIndexPageNumber = PointedIndexPageNumber/192;
			PointedIndexInPage = PointedIndexInPage*3;
		break;
#endif
#ifdef Fs_Fat16_Support
		case Fat16:
			PointedIndexPageNumber = PointedIndexPageNumber>>8;
			PointedIndexInPage = PointedIndexInPage<<1;
		break;
#endif
#ifdef Fs_Fat32_Support
		case Fat32:
			PointedIndexPageNumber = PointedIndexPageNumber>>7;
			PointedIndexInPage = PointedIndexInPage<<2;
			break;
#endif
	}
	PointedIndexInPage = PointedIndexInPage & 0x1FF;
	_Fat_IndexChangePageInBuffer(fatinfo, PointedIndexPageNumber);
#ifdef Fs_RW
	switch (Fat_IndexRdWrAction)
	{
	case Fat_IndexRd:
#endif
		switch (Fat_Type)
		{
#ifdef Fs_Fat12_Support
			case Fat12:
				if(PointedIndexInPage&& 1 == 0)
				{
					return ((fatinfo->DiskInfo_Buff1[PointedIndexInPage] + (fatinfo->DiskInfo_Buff1[PointedIndexInPage+1]*256)) & 0x0FFF);
				}
				else
				{
					return (((fatinfo->DiskInfo_Buff1[PointedIndexInPage+1] + (fatinfo->DiskInfo_Buff1[PointedIndexInPage+2]*256))/16) & 0x0FFF);
				}
#endif
#ifdef Fs_Fat16_Support
			case Fat16:
				return (fatinfo->DiskInfo_Buff1[PointedIndexInPage]
						+ (fatinfo->DiskInfo_Buff1[PointedIndexInPage+1]<<8)) - 2;
#endif
#ifdef Fs_Fat32_Support
			case Fat32:
				return  ((unsigned long)fatinfo->DiskInfo_Buff1[PointedIndexInPage]
				+ ((unsigned long)fatinfo->DiskInfo_Buff1[PointedIndexInPage+1]<<8)
				+ ((unsigned long)fatinfo->DiskInfo_Buff1[PointedIndexInPage+2]<<16)
				+ ((unsigned long)fatinfo->DiskInfo_Buff1[PointedIndexInPage+3]<<24)) - 2;
#endif
		}
#ifdef Fs_RW
#if defined(Fs_Fat12_Support) || defined(Fs_Fat16_Support) || defined(Fs_Fat32_Support)
		break;
#endif
	case Fat_IndexWr:
		Fat_IndexToWrite += 2;
		switch (Fat_Type)
		{
#ifdef Fs_Fat12_Support
			case Fat12:
				if(PointedIndexInPage&& 1 == 0)
				{
					fatinfo->DiskInfo_Buff1[PointedIndexInPage] = (unsigned char)Fat_IndexToWrite;
					fatinfo->DiskInfo_Buff1[PointedIndexInPage+1] = (((unsigned char)Fat_IndexToWrite/256) & 0X0F) | (fatinfo->DiskInfo_Buff1[PointedIndexInPage+1] & 0xF0);
				}
				else
				{
					fatinfo->DiskInfo_Buff1[PointedIndexInPage+1] = (fatinfo->DiskInfo_Buff1[PointedIndexInPage+1] & 0x0F) || (((unsigned char)Fat_IndexToWrite * 16) & 0xF0);
					fatinfo->DiskInfo_Buff1[PointedIndexInPage+2] = ((unsigned short)Fat_IndexToWrite / 16);
				}
				return True;
#endif
#ifdef Fs_Fat16_Support
			case Fat16:
				fatinfo->DiskInfo_Buff1[PointedIndexInPage] = Fat_IndexToWrite;
				fatinfo->DiskInfo_Buff1[PointedIndexInPage+1] = Fat_IndexToWrite>>8;
				return True;
#endif
#ifdef Fs_Fat32_Support
			case Fat32:
				fatinfo->DiskInfo_Buff1[PointedIndexInPage] = Fat_IndexToWrite;
				fatinfo->DiskInfo_Buff1[PointedIndexInPage+1] = Fat_IndexToWrite>>8;
				fatinfo->DiskInfo_Buff1[PointedIndexInPage+2] = Fat_IndexToWrite>>16;
				fatinfo->DiskInfo_Buff1[PointedIndexInPage+3] = Fat_IndexToWrite>>24;
				return True;
#endif
		}
#if defined(Fs_Fat12_Support) || defined(Fs_Fat16_Support) || defined(Fs_Fat32_Support)
		break;
#endif
	}
#endif
	return False;
}
//***********************************************************
//Ro:	Aceasta functie foloseste functia "_Fat_IndexReadWrite" si este folosita pentru a scrie un index.
//En:	This routine use the "_Fat_IndexReadWrite" function to write one index.
#ifdef Fs_RW
unsigned char _Fat_IndexWr(DiskInfo_t *fatinfo, unsigned long Fat_IndexNumber,unsigned long Fat_IndexToWrite)
{
	return (unsigned char)_Fat_IndexReadWrite(fatinfo, Fat_IndexNumber, Fat_IndexToWrite, Fat_IndexWr);
}
#endif
//***********************************************************
//Ro:	Aceasta functie foloseste functia "_Fat_IndexReadWrite" si este folosita pentru a reda un index.
//En:	This routine use the "_Fat_IndexReadWrite" function to read one index.
unsigned long _Fat_IndexRd(DiskInfo_t *fatinfo,unsigned long Fat_IndexNumber)
{
	return _Fat_IndexReadWrite(fatinfo, Fat_IndexNumber, 0, Fat_IndexRd);
}
//***********************************************************
long _FatIndex_Rd(FILE *stream)
{
	return 0;
}
//***********************************************************
//extern FILE *fdevopen(long (*__put)(char, FILE*), long (*__get)(FILE*));
bool _Fat_Mount(unsigned long DriveNumber)
{
	DiskInfo_t *fatinfo = Drives_Table[DriveNumber];
	if(fatinfo == NULL)
	{
		return false;
	}
	fatinfo->DiskInfo_Buff1 = (unsigned char *)malloc(512);
	if(fatinfo->DiskInfo_Buff1 == NULL)
	{
		return false;
	}
	fatinfo->DiskInfo_PageNrInBuff = 0;
	fatinfo->DiskInfo_DriveType = SdMmc;
	unsigned char A = _FatReadFsInfo(fatinfo);
	if(A == Fat_FatDetected)
	{
		//fatinfo->DiskInfo_ReservedPagesInFatEntry = _Fat_IndexReadWrite(fatinfo, 0, 0, Fat_IndexRd);
		_Fat_IndexReadFirstBlock(fatinfo);
		fatinfo->DiskInfo_RequestForUnmount = False;
		fatinfo->DiskInfo_OppenedSesions = 0;
		return true;
	}
	else
	{
		if(fatinfo->DiskInfo_Buff1) free(fatinfo->DiskInfo_Buff1);
		fatinfo->DiskInfo_Buff1 = NULL;
		return false;
	}
}
//***********************************************************
bool _Fat_Unmount(unsigned long DriveNumber)
{
	DiskInfo_t *fatinfo = Drives_Table[DriveNumber];
	if(!fatinfo || !fatinfo->DiskInfo_Buff1) return False;
	if(fatinfo->DiskInfo_OppenedSesions) return false;
	else
	{
		if(fatinfo->DiskInfo_Buff1)free(fatinfo->DiskInfo_Buff1);
		if(fatinfo) free(fatinfo);
		return true;
	}
}
//***********************************************************
//*********************************************************** 
//***********************************************************
//***********************************************************
//***********************************************************
//***********************************************************
//***********************************************************
//***********************************************************
//***********************************************************
//***********************************************************
//***********************************************************
//Ro:	Aceasta functie schimba pagina din tabela de alocari in buffer, tot o data si verifica
//daca bufferul este modificat pentru al salva in tabela de alocari
//En:	This function can change pages from Allocation Table in buffer, this function verify if
//the buffer is changed and if is changed this function can save the buffer to alloc table.
void _FatData_ChangePageInBuffer(FileInfo_t *fileinfo,unsigned long PageNumber)
{
	if(!fileinfo) return;
	unsigned long Fat12_16_RootDisplacement = 0;
#if defined (Fs_Fat16_Support) || defined (Fs_Fat32_Support)
	if(fileinfo->FileInfo_DiskInfo->DiskInfo_FatType != FAT_TYPE_FAT32) Fat12_16_RootDisplacement = 32;
#endif
#ifdef Fs_RW
	if((fileinfo->FileInfo_Flags & (1<<FileInfo_BufferModified)))
	{
		fileinfo->FileInfo_DiskInfo->drive_write_page(fileinfo->FileInfo_DiskInfo->DiskInfo_SdDriverStructAddr, fileinfo->FileInfo_BufferAddr, fileinfo->FileInfo_DiskInfo->DiskInfo_DataSectionBegin + Fat12_16_RootDisplacement + (fileinfo->FileInfo_OldIndexOnDisk << fileinfo->FileInfo_DiskInfo->DiskInfo_SectorPerClusterMask), fileinfo->FileInfo_DiskInfo->DiskInfo_SectorsPerCluster);
		fileinfo->FileInfo_Flags &= ~(1<<FileInfo_BufferModified);
	}
#endif
	fileinfo->FileInfo_DiskInfo->drive_read_page(fileinfo->FileInfo_DiskInfo->DiskInfo_SdDriverStructAddr, fileinfo->FileInfo_BufferAddr, (fileinfo->FileInfo_DiskInfo->DiskInfo_DataSectionBegin + Fat12_16_RootDisplacement + (fileinfo->FileInfo_IndexOnDisk << fileinfo->FileInfo_DiskInfo->DiskInfo_SectorPerClusterMask)/* + fileinfo->FileInfo_CurrentOppenedFile_PageInSector*/), fileinfo->FileInfo_DiskInfo->DiskInfo_SectorsPerCluster);
			//fileinfo->FileInfo_PageNrInBuffer = PageNumber;
		//}
	//}	
}
//***********************************************************
//Ro:	Aceasta functie este folosita pentru a salva fortat bufferul in tabela de allocari
//En:	This function can be use to force save the buffer to allocation table.
#ifdef Fs_RW
static void _FatData_SaveBuffer(FileInfo_t *fileinfo)
{
	uint32_t Fat12_16_RootDisplacement = 0;
#if defined (Fs_Fat16_Support) || defined (Fs_Fat32_Support)
	if(fileinfo->FileInfo_DiskInfo->DiskInfo_FatType != FAT_TYPE_FAT32) Fat12_16_RootDisplacement = 32;
#endif
	if((fileinfo->FileInfo_Flags && (1<<FileInfo_BufferModified)))
	{
		fileinfo->FileInfo_DiskInfo->drive_write_page(fileinfo->FileInfo_DiskInfo->DiskInfo_SdDriverStructAddr, fileinfo->FileInfo_BufferAddr, (fileinfo->FileInfo_DiskInfo->DiskInfo_DataSectionBegin + Fat12_16_RootDisplacement + (fileinfo->FileInfo_IndexOnDisk << fileinfo->FileInfo_DiskInfo->DiskInfo_SectorPerClusterMask) + fileinfo->FileInfo_CurrentOppenedFile_PageInSector), fileinfo->FileInfo_DiskInfo->DiskInfo_SectorsPerCluster);
		fileinfo->FileInfo_Flags &= ~(1<<FileInfo_BufferModified);
	}
}
#endif
//***********************************************************
bool _FatData_GoToIndex(FileInfo_t *fileinfo, unsigned long DestinationByte)
{
	if(!fileinfo) return false;
#ifdef USE_WDR
	WDR();
#endif
	//struct FileInfo_Struct *fileinfo = stream->_cookie;
	//unsigned char SregTmp = SREG;
	//asm("CLI");
	DiskInfo_t *DiskInfo = fileinfo->FileInfo_DiskInfo;
	//FILE *_DiskInfo = fileinfo->FileInfo_DiskInfo;
	unsigned char FatType = DiskInfo->DiskInfo_FatType;
	unsigned long IndexOnFile = fileinfo->FileInfo_IndexOnFile;
	unsigned long DestinationIndex = DestinationByte >> (9 + fileinfo->FileInfo_DiskInfo->DiskInfo_SectorPerClusterMask);
	//unsigned char DestinationPageInSector = DestinationIndex;
	//DestinationPageInSector &= DiskInfo->DiskInfo_SectorsPerCluster - 1;
	//DestinationIndex = DestinationIndex >> DiskInfo->DiskInfo_SectorPerClusterMask;
	unsigned long IndexesCount = 0;
	if(IndexOnFile != DestinationIndex)
	{
		if(DestinationIndex < IndexOnFile)
		{
			unsigned long TmpIndex = fileinfo->FileInfo_CurrentOppenedFile_Address;// - 2;
			for(IndexesCount = 0; IndexesCount < DestinationIndex; IndexesCount++)
			{
#ifdef Fs_Fat32_Support
				if((TmpIndex == LastClusterInFile - 2) & (FatType == FAT_TYPE_FAT32))
				{
					//SREG = SregTmp;
					return false;
				}
#endif
#if defined (Fs_Fat16_Support) && defined (Fs_Fat32_Support)
				else 
#endif
#ifdef Fs_Fat16_Support
				if((TmpIndex == (LastClusterInFile>>16) - 2) & (FatType == FAT_TYPE_FAT16))
				{
					//SREG = SregTmp;
					return false;
				}
#endif
				TmpIndex = _Fat_IndexReadWrite(DiskInfo, TmpIndex, 0, Fat_IndexRd);
				if((unsigned char)(TmpIndex >> 24) == 0xF0) TmpIndex &= 0x00FFFFFF;
			}
			fileinfo->FileInfo_IndexOnDisk = TmpIndex;
		}
		else
		{
			unsigned long TmpIndex = fileinfo->FileInfo_IndexOnDisk;
			for(IndexesCount = IndexOnFile; IndexesCount < DestinationIndex; IndexesCount++)
			{
#ifdef Fs_Fat32_Support
				if((TmpIndex == LastClusterInFile - 2) & (FatType == FAT_TYPE_FAT32))
				{
					//SREG = SregTmp;
					return false;
				}
#endif
#if defined (Fs_Fat16_Support) && defined (Fs_Fat32_Support)
				else 
#endif
#ifdef Fs_Fat16_Support
				if((TmpIndex == (LastClusterInFile>>16) - 2) & (FatType == FAT_TYPE_FAT16))
				{
					//SREG = SregTmp;
					return false;
				}
#endif
				TmpIndex = _Fat_IndexReadWrite(DiskInfo, TmpIndex, 0, Fat_IndexRd);
				if((unsigned char)(TmpIndex >> 24) == 0xF0) TmpIndex &= 0x00FFFFFF;
			}
			fileinfo->FileInfo_IndexOnDisk = TmpIndex;
		}
	}
	if((IndexOnFile != DestinationIndex)/* || (DestinationPageInSector != fileinfo->FileInfo_OldPageInSector)*/)
	{
		//fileinfo->FileInfo_CurrentOppenedFile_PageInSector = DestinationPageInSector;
		_FatData_ChangePageInBuffer(fileinfo, DestinationIndex);
		//fileinfo->FileInfo_OldPageInSector = DestinationPageInSector;
		fileinfo->FileInfo_OldIndexOnDisk = fileinfo->FileInfo_IndexOnDisk;
	}		
	//if(fileinfo->FileInfo_IndexOnFile != DestinationIndex)
	//{
		fileinfo->FileInfo_IndexOnFile = DestinationIndex;
	//}	
	//SREG = SregTmp;
	return true;
}
//***********************************************************
//***********************************************************
//***********************************************************
void _FatData_GoToRoot(FileInfo_t *fileinfo)
{
	if(!fileinfo) return;
	//struct FileInfo_Struct *fileinfo = stream->_cookie;
	fileinfo->FileInfo_PointerInDirectory = 0;
	//_FatData_SesionInit(fileinfo);
	unsigned long Pointer = 0;
	do 
	{
		Pointer = fileinfo->FileInfo_PointerInDirectory<<5;
		if(_FatData_GoToIndex(fileinfo, Pointer))
		{
			unsigned char Attr = fileinfo->FileInfo_BufferAddr[(Pointer & (((unsigned long)fileinfo->FileInfo_DiskInfo->DiskInfo_SectorsPerCluster<<9)-1)) + 0x0B];
			if(Attr != FILE_ATTR_LFN_TEXT && Attr != 0) break;
			fileinfo->FileInfo_PointerInDirectory++;
		}		 
	} while (1);
	_FatData_ReadPointedFileDetails(fileinfo);
}
//***********************************************************
unsigned char _FatData_GetLongFileName(FileInfo_t *fileinfo)
{
	if(!fileinfo) return False;
	//struct FileInfo_Struct *fileinfo = stream->_cookie;
	unsigned char LongFileNamePointer = 0;
	signed long PointerInDirectory = fileinfo->FileInfo_PointerInDirectory;
	if(1)
	{
		do 
		{
			PointerInDirectory--;
			if(PointerInDirectory < 0) break;
			unsigned long Pointer = PointerInDirectory<<5;
			if(_FatData_GoToIndex(fileinfo, Pointer))
			{
				unsigned char Attr = fileinfo->FileInfo_BufferAddr[(Pointer & (((unsigned long)fileinfo->FileInfo_DiskInfo->DiskInfo_SectorsPerCluster<<9)-1)) + 0x0B];
				if(Attr == FILE_ATTR_LFN_TEXT)
				{
					fileinfo->FileInfo_PointedFileInDirectoryLongFileName[LongFileNamePointer] = fileinfo->FileInfo_BufferAddr[(Pointer & (((unsigned long)fileinfo->FileInfo_DiskInfo->DiskInfo_SectorsPerCluster<<9)-1)) + 0x01];
					LongFileNamePointer++;
					fileinfo->FileInfo_PointedFileInDirectoryLongFileName[LongFileNamePointer] = fileinfo->FileInfo_BufferAddr[(Pointer & (((unsigned long)fileinfo->FileInfo_DiskInfo->DiskInfo_SectorsPerCluster<<9)-1)) + 0x03];
					LongFileNamePointer++;
					fileinfo->FileInfo_PointedFileInDirectoryLongFileName[LongFileNamePointer] = fileinfo->FileInfo_BufferAddr[(Pointer & (((unsigned long)fileinfo->FileInfo_DiskInfo->DiskInfo_SectorsPerCluster<<9)-1)) + 0x05];
					LongFileNamePointer++;
					fileinfo->FileInfo_PointedFileInDirectoryLongFileName[LongFileNamePointer] = fileinfo->FileInfo_BufferAddr[(Pointer & (((unsigned long)fileinfo->FileInfo_DiskInfo->DiskInfo_SectorsPerCluster<<9)-1)) + 0x07];
					LongFileNamePointer++;
					fileinfo->FileInfo_PointedFileInDirectoryLongFileName[LongFileNamePointer] = fileinfo->FileInfo_BufferAddr[(Pointer & (((unsigned long)fileinfo->FileInfo_DiskInfo->DiskInfo_SectorsPerCluster<<9)-1)) + 0x09];

					LongFileNamePointer++;
					fileinfo->FileInfo_PointedFileInDirectoryLongFileName[LongFileNamePointer] = fileinfo->FileInfo_BufferAddr[(Pointer & (((unsigned long)fileinfo->FileInfo_DiskInfo->DiskInfo_SectorsPerCluster<<9)-1)) + 0x0E];
					LongFileNamePointer++;
					fileinfo->FileInfo_PointedFileInDirectoryLongFileName[LongFileNamePointer] = fileinfo->FileInfo_BufferAddr[(Pointer & (((unsigned long)fileinfo->FileInfo_DiskInfo->DiskInfo_SectorsPerCluster<<9)-1)) + 0x10];
					LongFileNamePointer++;
					fileinfo->FileInfo_PointedFileInDirectoryLongFileName[LongFileNamePointer] = fileinfo->FileInfo_BufferAddr[(Pointer & (((unsigned long)fileinfo->FileInfo_DiskInfo->DiskInfo_SectorsPerCluster<<9)-1)) + 0x12];
					LongFileNamePointer++;
					fileinfo->FileInfo_PointedFileInDirectoryLongFileName[LongFileNamePointer] = fileinfo->FileInfo_BufferAddr[(Pointer & (((unsigned long)fileinfo->FileInfo_DiskInfo->DiskInfo_SectorsPerCluster<<9)-1)) + 0x14];
					LongFileNamePointer++;
					fileinfo->FileInfo_PointedFileInDirectoryLongFileName[LongFileNamePointer] = fileinfo->FileInfo_BufferAddr[(Pointer & (((unsigned long)fileinfo->FileInfo_DiskInfo->DiskInfo_SectorsPerCluster<<9)-1)) + 0x16];
					LongFileNamePointer++;
					fileinfo->FileInfo_PointedFileInDirectoryLongFileName[LongFileNamePointer] = fileinfo->FileInfo_BufferAddr[(Pointer & (((unsigned long)fileinfo->FileInfo_DiskInfo->DiskInfo_SectorsPerCluster<<9)-1)) + 0x18];

					LongFileNamePointer++;
					fileinfo->FileInfo_PointedFileInDirectoryLongFileName[LongFileNamePointer] = fileinfo->FileInfo_BufferAddr[(Pointer & (((unsigned long)fileinfo->FileInfo_DiskInfo->DiskInfo_SectorsPerCluster<<9)-1)) + 0x1C];
					LongFileNamePointer++;
					fileinfo->FileInfo_PointedFileInDirectoryLongFileName[LongFileNamePointer] = fileinfo->FileInfo_BufferAddr[(Pointer & (((unsigned long)fileinfo->FileInfo_DiskInfo->DiskInfo_SectorsPerCluster<<9)-1)) + 0x1E];
					LongFileNamePointer++;
				}
				else break;
			}else break;
		} while (1);
	}
	fileinfo->FileInfo_PointedFileInDirectoryLongFileName[LongFileNamePointer] = 0;
	_FatData_GoToIndex(fileinfo, fileinfo->FileInfo_PointerInDirectory<<5);
	if(LongFileNamePointer != 0) return True;
	else return False;
}
//***********************************************************
void _FatData_ReadPointedFileDetails(FileInfo_t *fileinfo)
{
	if(!fileinfo) return;
	//struct FileInfo_Struct *fileinfo = stream->_cookie;
	unsigned long Pointer = fileinfo->FileInfo_PointerInDirectory<<5;
	unsigned char Attr = fileinfo->FileInfo_BufferAddr[(Pointer & (((unsigned long)fileinfo->FileInfo_DiskInfo->DiskInfo_SectorsPerCluster<<9)-1)) + 0x0B];
	fileinfo->FileInfo_PointedFileInDirectory_Attribute = Attr;
	if(_FatData_GetLongFileName(fileinfo));
	else 
	{
		memset(fileinfo->FileInfo_PointedFileInDirectoryLongFileName, 32, 12);
		char* TmpPtr = (char*)(fileinfo->FileInfo_BufferAddr + (Pointer & (((unsigned long)fileinfo->FileInfo_DiskInfo->DiskInfo_SectorsPerCluster<<9)-1)));
		volatile unsigned int TmpStrLen = (unsigned int)(strchr(TmpPtr, 32) - TmpPtr);
		if(TmpStrLen > 8) TmpStrLen = 8;
		memcpy(fileinfo->FileInfo_PointedFileInDirectoryLongFileName, TmpPtr, TmpStrLen);
		TmpPtr += TmpStrLen;
		//if(TmpStrLen == 8) TmpStrLen++;
		char *TmpExtensionPtr = (char *)(fileinfo->FileInfo_BufferAddr + (Pointer & (((unsigned long)fileinfo->FileInfo_DiskInfo->DiskInfo_SectorsPerCluster<<9)-1)) + 8);
		if(TmpExtensionPtr[0] != ' ' || TmpExtensionPtr[1] != ' ' || TmpExtensionPtr[2] != ' ')
		{
			fileinfo->FileInfo_PointedFileInDirectoryLongFileName[TmpStrLen] = '.';
			TmpStrLen++;
		}
		if(TmpExtensionPtr[0] != ' ')
		{
			fileinfo->FileInfo_PointedFileInDirectoryLongFileName[TmpStrLen] = TmpExtensionPtr[0];
			TmpStrLen++;
			if(TmpExtensionPtr[1] != ' ')
			{
				fileinfo->FileInfo_PointedFileInDirectoryLongFileName[TmpStrLen] = TmpExtensionPtr[1];
				TmpStrLen++;
				if(TmpExtensionPtr[2] != ' ')
				{
					fileinfo->FileInfo_PointedFileInDirectoryLongFileName[TmpStrLen] = TmpExtensionPtr[2];
					TmpStrLen++;
				}
			}
		}
		fileinfo->FileInfo_PointedFileInDirectoryLongFileName[TmpStrLen] = 0;
	}
	memcpy(fileinfo->FileInfo_PointedFileInDirectoryExtension, fileinfo->FileInfo_BufferAddr + ((Pointer & (((unsigned long)fileinfo->FileInfo_DiskInfo->DiskInfo_SectorsPerCluster<<9)-1)) + 0x08), 3);
	fileinfo->FileInfo_PointedFileInDirectoryExtension[3] = 0;
	fileinfo->FileInfo_PointedFileInDirectory_Address = (unsigned long)((unsigned long)(fileinfo->FileInfo_BufferAddr[(Pointer & (((unsigned long)fileinfo->FileInfo_DiskInfo->DiskInfo_SectorsPerCluster<<9)-1)) + 0x1A])
	                                                        | (unsigned long)((unsigned long)fileinfo->FileInfo_BufferAddr[(Pointer & (((unsigned long)fileinfo->FileInfo_DiskInfo->DiskInfo_SectorsPerCluster<<9)-1)) + 0x1B]<<8)
	                                                        | (unsigned long)((unsigned long)fileinfo->FileInfo_BufferAddr[(Pointer & (((unsigned long)fileinfo->FileInfo_DiskInfo->DiskInfo_SectorsPerCluster<<9)-1)) + 0x14]<<16)
	                                                        | (unsigned long)((unsigned long)fileinfo->FileInfo_BufferAddr[(Pointer & (((unsigned long)fileinfo->FileInfo_DiskInfo->DiskInfo_SectorsPerCluster<<9)-1)) + 0x15]<<24));
							
	fileinfo->FileInfo_PointedFileInDirectory_FileSize = (unsigned long)((unsigned long)(fileinfo->FileInfo_BufferAddr[(Pointer & (((unsigned long)fileinfo->FileInfo_DiskInfo->DiskInfo_SectorsPerCluster<<9)-1)) + 0x1C])
															| (unsigned long)((unsigned long)fileinfo->FileInfo_BufferAddr[(Pointer & (((unsigned long)fileinfo->FileInfo_DiskInfo->DiskInfo_SectorsPerCluster<<9)-1)) + 0x1D]<<8)
															| (unsigned long)((unsigned long)fileinfo->FileInfo_BufferAddr[(Pointer & (((unsigned long)fileinfo->FileInfo_DiskInfo->DiskInfo_SectorsPerCluster<<9)-1)) + 0x1E]<<16)
															| (unsigned long)((unsigned long)fileinfo->FileInfo_BufferAddr[(Pointer & (((unsigned long)fileinfo->FileInfo_DiskInfo->DiskInfo_SectorsPerCluster<<9)-1)) + 0x1F]<<24));
}
//***********************************************************
FileInfo_GetFilesLocation_t _FatData_GetFilesLocation(FileInfo_t *fileinfo)
{
	//struct FileInfo_Struct *fileinfo = stream->_cookie;
	struct FileInfo_GetFilesLocation_Struct Return;
	Return.NrOfFiles = 0;
	Return.ListOfFiles = 0;
	unsigned long FileCount = 1;
	unsigned char Status =False;
	unsigned long BackUpPointerInDirectory = fileinfo->FileInfo_PointerInDirectory;
	if((fileinfo->FileInfo_CurrentOppenedFile_Attributes & FILE_ATTR_DIRECTORY) || (fileinfo->FileInfo_CurrentOppenedFile_Attributes & FILE_ATTR_VOLUME_ID))
	{
		_FatData_GoToRoot(fileinfo);
		do 
		{
			Status = _FatData_Dn(fileinfo);
			if(Status == False) break;
			FileCount++;
		} while (1);
		Return.NrOfFiles = FileCount;
		unsigned long* Table = (unsigned long*)malloc(FileCount * sizeof(unsigned long));
		Return.ListOfFiles = Table;
		FileCount = 1;
		_FatData_GoToRoot(fileinfo);
		Table[0] = fileinfo->FileInfo_PointerInDirectory;
		do 
		{
			Status = _FatData_Dn(fileinfo);
			if(Status == False) break;
			Table[FileCount] = fileinfo->FileInfo_PointerInDirectory;
			FileCount++;
		} while (1);
	}
	else Return.NrOfFiles = 0;
	fileinfo->FileInfo_PointerInDirectory = BackUpPointerInDirectory;
	//if(_FatData_Dn(stream)) _FatData_Up(stream);
	//else if(_FatData_Up(stream)) _FatData_Dn(stream);
	_FatData_ReadPointedFileDetails(fileinfo);
	return Return;
}
//***********************************************************
bool _FatData_PointFile(FileInfo_t *fileinfo, unsigned long* Table, unsigned long FileToPoint)
{
	if(!fileinfo) return false;
	//struct FileInfo_Struct *fileinfo = stream->_cookie;
	fileinfo->FileInfo_PointerInDirectory = Table[FileToPoint];
	unsigned long Pointer = fileinfo->FileInfo_PointerInDirectory<<5;
	_FatData_GoToIndex(fileinfo, Pointer);
	_FatData_ReadPointedFileDetails(fileinfo);
	return true;
}
//***********************************************************
bool _FatData_Up(FileInfo_t *fileinfo)
{
	if(!fileinfo) return false;
	//struct FileInfo_Struct *fileinfo = stream->_cookie;
	if((fileinfo->FileInfo_CurrentOppenedFile_Attributes & FILE_ATTR_VOLUME_ID) | (fileinfo->FileInfo_CurrentOppenedFile_Attributes & FILE_ATTR_DIRECTORY))
	{
		if(fileinfo->FileInfo_PointerInDirectory != 0)
		{
			do 
			{
				fileinfo->FileInfo_PointerInDirectory--;
				unsigned long Pointer = fileinfo->FileInfo_PointerInDirectory<<5;
				if(_FatData_GoToIndex(fileinfo, Pointer))
				{
					unsigned char Attr = fileinfo->FileInfo_BufferAddr[(Pointer & (((unsigned long)fileinfo->FileInfo_DiskInfo->DiskInfo_SectorsPerCluster<<9)-1)) + 0x0B];
					if((Attr == FILE_ATTR_VOLUME_ID) || (Attr == (FILE_ATTR_DIRECTORY | FILE_ATTR_ARCHIVE)) || (Attr & FILE_ATTR_DIRECTORY) || (Attr & FILE_ATTR_ARCHIVE))
					{
						if((fileinfo->FileInfo_BufferAddr[(Pointer & (((unsigned long)fileinfo->FileInfo_DiskInfo->DiskInfo_SectorsPerCluster<<9)-1))] != FILE_HEADER_DELETED) && (Attr != FILE_ATTR_LFN_TEXT))
						{
							_FatData_ReadPointedFileDetails(fileinfo);
							return true;
						}					
					}	
				}	
				if(fileinfo->FileInfo_PointerInDirectory == 0) return False;
			}while(1);		
			//fileinfo->FileInfo_PointerInDirectory++;
		}	
	}	
	return false;
}
//***********************************************************
bool _FatData_Dn(FileInfo_t *fileinfo)
{
	if(!fileinfo) return false;
	//struct FileInfo_Struct *fileinfo = stream->_cookie;
	DiskInfo_t *IndexFile = fileinfo->FileInfo_DiskInfo;
	if((fileinfo->FileInfo_CurrentOppenedFile_Attributes & FILE_ATTR_VOLUME_ID) | (fileinfo->FileInfo_CurrentOppenedFile_Attributes & FILE_ATTR_DIRECTORY))
	{
		do 
		{
			fileinfo->FileInfo_PointerInDirectory++;
#if defined (Fs_Fat16_Support) || defined (Fs_Fat32_Support)
			if((IndexFile->DiskInfo_FatType != Fat32) & (fileinfo->FileInfo_PointerInDirectory > 15) & (fileinfo->FileInfo_CurrentOppenedFile_Address == 0)) break;
#endif
			unsigned long Pointer = fileinfo->FileInfo_PointerInDirectory<<5;
			if(_FatData_GoToIndex(fileinfo, Pointer))
			{
				unsigned char Attr = fileinfo->FileInfo_BufferAddr[(Pointer & (((unsigned long)fileinfo->FileInfo_DiskInfo->DiskInfo_SectorsPerCluster<<9)-1)) + 0x0B];
				if((Attr == FILE_ATTR_VOLUME_ID) || (Attr == (FILE_ATTR_DIRECTORY | FILE_ATTR_ARCHIVE)) || (Attr & FILE_ATTR_DIRECTORY) || (Attr & FILE_ATTR_ARCHIVE))
				{
					if((fileinfo->FileInfo_BufferAddr[(Pointer & (((unsigned long)fileinfo->FileInfo_DiskInfo->DiskInfo_SectorsPerCluster<<9)-1))] != FILE_HEADER_DELETED) && (Attr != FILE_ATTR_LFN_TEXT))
					{
						_FatData_ReadPointedFileDetails(fileinfo);
						return true;
					}					
				}
				else if(((!Attr) & FILE_ATTR_HIDDEN) || Attr != FILE_ATTR_LFN_TEXT) break;
			}	
		}while(1);	
		fileinfo->FileInfo_PointerInDirectory--;
	}	
	return false;
}
//***********************************************************
void _FatData_Fat16RootDirectory(FileInfo_t *fileinfo)
{
	if(!fileinfo) return;
	//struct FileInfo_Struct *fileinfo = stream->_cookie;
	fileinfo->FileInfo_PageNrInBuffer = 0;
	fileinfo->FileInfo_PointerInDirectory = 0;
	fileinfo->FileInfo_PointerInFile = 0;
	fileinfo->FileInfo_IndexOnDisk = 0;
	fileinfo->FileInfo_IndexOnFile = 0;
	fileinfo->FileInfo_OldPageInSector = 0;
	
	fileinfo->FileInfo_CurrentOppenedFile_Address = 0;
	fileinfo->FileInfo_CurrentOppenedFile_FileSize = 0;
	fileinfo->FileInfo_CurrentOppenedFile_Attributes = FILE_ATTR_DIRECTORY;
	fileinfo->FileInfo_CurrentOppenedFile_PageInSector = 0;

	fileinfo->FileInfo_LastOppenedFile_Address = fileinfo->FileInfo_CurrentOppenedFile_Address;
	fileinfo->FileInfo_LastOppenedFile_Attributes = fileinfo->FileInfo_CurrentOppenedFile_Attributes;
	fileinfo->FileInfo_LastOppenedFile_PageInSector = fileinfo->FileInfo_CurrentOppenedFile_PageInSector;

	fileinfo->FileInfo_PointedFileInDirectory_Address = 0;
	fileinfo->FileInfo_PointedFileInDirectory_FileSize = 0;
	fileinfo->FileInfo_PointedFileInDirectory_Attribute = 0;


	//DiskInfo_t *IndexFile = fileinfo->FileInfo_DiskInfo;
	fileinfo->FileInfo_DiskInfo->drive_read_page(fileinfo->FileInfo_DiskInfo->DiskInfo_SdDriverStructAddr ,fileinfo->FileInfo_BufferAddr, fileinfo->FileInfo_DiskInfo->DiskInfo_DataSectionBegin, fileinfo->FileInfo_DiskInfo->DiskInfo_SectorsPerCluster);
	_FatData_GoToRoot(fileinfo);
	_FatData_ReadPointedFileDetails(fileinfo);
}
//***********************************************************
bool _FatData_OpenFile(FileInfo_t *fileinfo)
{
	if(!fileinfo) return false;
	//struct FileInfo_Struct *fileinfo = stream->_cookie;
	//unsigned char SregTmp = SREG;
	//asm("CLI");
	if((fileinfo->FileInfo_CurrentOppenedFile_Attributes & FILE_ATTR_DIRECTORY) || (fileinfo->FileInfo_CurrentOppenedFile_Attributes & FILE_ATTR_VOLUME_ID))
	{
#if defined (Fs_Fat12_Support) || defined (Fs_Fat16_Support)
		if((fileinfo->FileInfo_CurrentOppenedFile_Address != 0) && (fileinfo->FileInfo_PointedFileInDirectory_Address == 0))
		{
			_FatData_Fat16RootDirectory(fileinfo);
		}
		else
		{
#endif	
			fileinfo->FileInfo_OldPageInSector = 0;
			fileinfo->FileInfo_LastOppenedFile_Address = fileinfo->FileInfo_CurrentOppenedFile_Address;
			fileinfo->FileInfo_LastOppenedFile_Attributes = fileinfo->FileInfo_CurrentOppenedFile_Attributes;
			fileinfo->FileInfo_LastOppenedFile_PageInSector = fileinfo->FileInfo_CurrentOppenedFile_PageInSector;
			fileinfo->FileInfo_CurrentOppenedFile_Attributes = fileinfo->FileInfo_PointedFileInDirectory_Attribute;
			fileinfo->FileInfo_CurrentOppenedFile_Address = fileinfo->FileInfo_PointedFileInDirectory_Address;
			fileinfo->FileInfo_CurrentOppenedFile_PageInSector = 0;
			if(fileinfo->FileInfo_CurrentOppenedFile_Address != 0) fileinfo->FileInfo_IndexOnDisk = fileinfo->FileInfo_CurrentOppenedFile_Address - 2;
			else fileinfo->FileInfo_IndexOnDisk = fileinfo->FileInfo_CurrentOppenedFile_Address;
			fileinfo->FileInfo_IndexOnFile = 0;
			unsigned long Fat12_16_RootDisplacement = 0;
			//DiskInfo_t *IndexFile = fileinfo->FileInfo_DiskInfo;
			//SD_Struct_t *SdDriverStruct = IndexFile->DiskInfo_SdDriverStructAddr;
			if(fileinfo->FileInfo_DiskInfo->DiskInfo_FatType != FAT_TYPE_FAT32) Fat12_16_RootDisplacement = 32;
			fileinfo->FileInfo_DiskInfo->drive_read_page(fileinfo->FileInfo_DiskInfo->DiskInfo_SdDriverStructAddr ,fileinfo->FileInfo_BufferAddr, fileinfo->FileInfo_DiskInfo->DiskInfo_DataSectionBegin + Fat12_16_RootDisplacement + (fileinfo->FileInfo_IndexOnDisk << fileinfo->FileInfo_DiskInfo->DiskInfo_SectorPerClusterMask), fileinfo->FileInfo_DiskInfo->DiskInfo_SectorsPerCluster);
		
			unsigned char OppenedFileAttr = fileinfo->FileInfo_PointedFileInDirectory_Attribute;
			if((OppenedFileAttr & FILE_ATTR_DIRECTORY) || (OppenedFileAttr & FILE_ATTR_VOLUME_ID))
			{
				
				_FatData_GoToRoot(fileinfo);
				_FatData_ReadPointedFileDetails(fileinfo);
			}
			else
			{
				fileinfo->FileInfo_CurrentOppenedFile_FileSize = fileinfo->FileInfo_PointedFileInDirectory_FileSize;
			}
#if defined (Fs_Fat12_Support) || defined (Fs_Fat16_Support)
		}	
#endif	
	//SREG = SregTmp;
	return true;
	}	
	//SREG = SregTmp;
	return false;
}
//***********************************************************
bool _FatData_CloseFile(FileInfo_t *fileinfo)
{
	if(!fileinfo) return false;
	//struct FileInfo_Struct *fileinfo = stream->_cookie;
	//unsigned char SregTmp = SREG;
	//asm("CLI");
	if(fileinfo->FileInfo_CurrentOppenedFile_Attributes & FILE_ATTR_ARCHIVE)
	{
#if defined (Fs_Fat12_Support) || defined (Fs_Fat16_Support)
		//DiskInfo_t *IndexFile = fileinfo->FileInfo_DiskInfo;
		if((fileinfo->FileInfo_LastOppenedFile_Address == 0) & (fileinfo->FileInfo_CurrentOppenedFile_Address != 0) & (fileinfo->FileInfo_DiskInfo->DiskInfo_FatType != Fat32))
		{
			_FatData_Fat16RootDirectory(fileinfo);
		}
		else
		{
#endif
			fileinfo->FileInfo_PageNrInBuffer = 0;
			fileinfo->FileInfo_IndexOnFile = 0;
			fileinfo->FileInfo_CurrentOppenedFile_Attributes = fileinfo->FileInfo_LastOppenedFile_Attributes;
			fileinfo->FileInfo_CurrentOppenedFile_Address = fileinfo->FileInfo_LastOppenedFile_Address;
			fileinfo->FileInfo_CurrentOppenedFile_PageInSector = fileinfo->FileInfo_LastOppenedFile_PageInSector;
			fileinfo->FileInfo_IndexOnDisk = fileinfo->FileInfo_CurrentOppenedFile_Address;
			if(fileinfo->FileInfo_IndexOnDisk != 0) fileinfo->FileInfo_IndexOnDisk -= 2;

			unsigned long Fat12_16_RootDisplacement = 0;
			//DiskInfo_t *IndexFile = fileinfo->FileInfo_DiskInfo;
			//SD_Struct_t *SdDriverStruct = IndexFile->DiskInfo_SdDriverStructAddr;
#if defined (Fs_Fat16_Support) || defined (Fs_Fat32_Support)
			if(fileinfo->FileInfo_DiskInfo->DiskInfo_FatType != FAT_TYPE_FAT32) Fat12_16_RootDisplacement = 32;
#endif
			fileinfo->FileInfo_DiskInfo->drive_read_page(fileinfo->FileInfo_DiskInfo->DiskInfo_SdDriverStructAddr,fileinfo->FileInfo_BufferAddr, fileinfo->FileInfo_DiskInfo->DiskInfo_DataSectionBegin + Fat12_16_RootDisplacement + (fileinfo->FileInfo_IndexOnDisk << fileinfo->FileInfo_DiskInfo->DiskInfo_SectorPerClusterMask)/* + fileinfo->FileInfo_CurrentOppenedFile_PageInSector*/, fileinfo->FileInfo_DiskInfo->DiskInfo_SectorsPerCluster);

			_FatData_ReadPointedFileDetails(fileinfo);
#if defined (Fs_Fat12_Support) || defined (Fs_Fat16_Support)
		}
#endif
		//SREG = SregTmp;
		return true;
	}	
	else 
	{
	//SREG = SregTmp;
	return false;
	}
}
//***********************************************************
/*unsigned char _FatData_Back(FileInfo_t *fileinfo)
{
	if(fileinfo->FileInfo_CurrentOppenedFile_Attributes & FILE_ATTR_DIRECTORY)
	{
		fileinfo->FileInfo_PointerInDirectory = 0;
		_FatData_Dn(fileinfo);
		_FatData_OpenFile(fileinfo);
		return True;
	}
	else return False;
}*/
//***********************************************************
unsigned char _FatData_ReadWriteByte(FileInfo_t *fileinfo, unsigned char ActionWrite, unsigned long Addr, unsigned char ByteToWrite)
{
	if(fileinfo == 0 || fileinfo->OppenedFileIsEmpty == true) return False;
	unsigned short ByteInBuffer = Addr & 0x1FF;
	//struct FileInfo_Struct *fileinfo = stream->_cookie;
#ifdef Fs_RW
	if(ActionWrite)
	{
		fileinfo->FileInfo_BufferAddr[ByteInBuffer] = ByteToWrite;
		_FatData_GoToIndex(fileinfo, Addr);
		fileinfo->FileInfo_Flags &= (1<<FileInfo_BufferModified);
	}
	else
	{
#endif
		/*if(Addr>>9 != fileinfo->FileInfo_CurrentOppenedFile_PageInSector)*/_FatData_GoToIndex(fileinfo, Addr);
		return fileinfo->FileInfo_BufferAddr[ByteInBuffer];
#ifdef Fs_RW
	}
	return True;
#endif
}
//***********************************************************
unsigned long _FatData_ReadSection(FileInfo_t *fileinfo, unsigned char *Buff, unsigned long Begin, unsigned long Size)
{
	if(fileinfo == 0  || Buff == 0 || fileinfo->OppenedFileIsEmpty == true) return 0;
	//struct FileInfo_Struct *fileinfo = stream->_cookie;
	unsigned long FileSize = fileinfo->FileInfo_CurrentOppenedFile_FileSize;
	unsigned char* PageBuff = fileinfo->FileInfo_BufferAddr;
	unsigned long BuffBytesCount = 0;
	unsigned long Address = Begin;
	unsigned long _Size = Size;
	if(Begin + Size > FileSize) _Size = FileSize - Begin - 1;
	if(_Size > FileSize) return 0;
	_FatData_GoToIndex(fileinfo, Address);
	do
	{
		if(BuffBytesCount >= _Size) break;
		if((Address & (((unsigned long)fileinfo->FileInfo_DiskInfo->DiskInfo_SectorsPerCluster<<9)-1)) == 0)
		{
			_FatData_GoToIndex(fileinfo, Address);
		}
		*Buff++ = PageBuff[Address & (((unsigned long)fileinfo->FileInfo_DiskInfo->DiskInfo_SectorsPerCluster<<9)-1)];
		BuffBytesCount++;
		Address++;
	}while(1);
	return BuffBytesCount;
}
//***********************************************************
unsigned char* _FatData_ReadFile(FileInfo_t *fileinfo)
{
	if(fileinfo == 0 || fileinfo->OppenedFileIsEmpty == true) return NULL;
	if(_FatData_OpenFile(fileinfo))
	{
		//struct FileInfo_Struct *fileinfo = fileinfo->_cookie;
		unsigned long FileSize = fileinfo->FileInfo_CurrentOppenedFile_FileSize;
		unsigned long FileSize_Pages = FileSize/(fileinfo->FileInfo_DiskInfo->DiskInfo_BytesPerSector * fileinfo->FileInfo_DiskInfo->DiskInfo_SectorsPerCluster);
		unsigned short FileSize_RestOfBytes = FileSize - (FileSize_Pages*(fileinfo->FileInfo_DiskInfo->DiskInfo_BytesPerSector * fileinfo->FileInfo_DiskInfo->DiskInfo_SectorsPerCluster));
		unsigned long PagesCount = 0;
#ifdef gcc
		unsigned char* Buff = (unsigned char*)malloc(FileSize);
#else
		unsigned char* Buff = (unsigned char*)memalign(32, FileSize);
#endif
		volatile unsigned char* BuffTmp = Buff;
		unsigned char* PageBuff = fileinfo->FileInfo_BufferAddr;
		unsigned char* backup_ptr = fileinfo->FileInfo_BufferAddr;
		if(Buff == 0)
		{
			_FatData_CloseFile(fileinfo);
			return (void*)0;
		}
		if(FileSize_Pages)
		{
			for(PagesCount = 0; PagesCount < FileSize_Pages; PagesCount++)
			{
				fileinfo->FileInfo_BufferAddr = (unsigned char*)(BuffTmp + (PagesCount<<(9 + fileinfo->FileInfo_DiskInfo->DiskInfo_SectorPerClusterMask)));
				if(_FatData_GoToIndex(fileinfo, PagesCount<<(9 + fileinfo->FileInfo_DiskInfo->DiskInfo_SectorPerClusterMask))) fileinfo->FileInfo_BufferAddr = backup_ptr;
				else
				{
					fileinfo->FileInfo_BufferAddr = backup_ptr;
					_FatData_CloseFile(fileinfo);
					return (void*)0;
				}
				//memcpy(Buff + (PagesCount<<(9 + fileinfo->FileInfo_DiskInfo->DiskInfo_SectorPerClusterMask)), PageBuff, (fileinfo->FileInfo_DiskInfo->DiskInfo_BytesPerSector * fileinfo->FileInfo_DiskInfo->DiskInfo_SectorsPerCluster));
			}
		}
		if(FileSize_RestOfBytes)
		{
			_FatData_GoToIndex(fileinfo, FileSize_Pages<<(9 + fileinfo->FileInfo_DiskInfo->DiskInfo_SectorPerClusterMask));
			memcpy(Buff + (FileSize_Pages<<(9 + fileinfo->FileInfo_DiskInfo->DiskInfo_SectorPerClusterMask)), PageBuff, FileSize_RestOfBytes);
		}
		_FatData_CloseFile(fileinfo);
		return Buff;
	} else return (void*)0;
}
//***********************************************************
long _FatData_ReadChar(FILE *stream)
{
	return 0;
}
//***********************************************************
long _FatData_PutChar(char Char, FILE *stream)
{
	return 0;
}
//***********************************************************
FileInfo_t *_FatData_OpenSesion(unsigned char DriveNr)
{
	//FILE *stream = calloc(1, sizeof(FILE));//fdevopen(_FatData_PutChar, _FatData_ReadChar);
	//if(stream == 0) return NULL;
	if(!Drives_Table[DriveNr]) return NULL;
	FileInfo_t *fileinfo = calloc(1, sizeof(FileInfo_t));
	if(fileinfo == 0)
	{
		//fclose(stream);
		return NULL;
	}
	fileinfo->FileInfo_DiskInfo = Drives_Table[DriveNr];
	if((fileinfo->FileInfo_BufferAddr = (unsigned char *)malloc((unsigned long)(fileinfo->FileInfo_DiskInfo->DiskInfo_BytesPerSector * fileinfo->FileInfo_DiskInfo->DiskInfo_SectorsPerCluster))) == 0)
	{
		free(fileinfo);
		//free(stream);
		return NULL;
	}
	//stream->_cookie = fileinfo;
	fileinfo->FileInfo_CurrentOppenedFile_Attributes = FILE_ATTR_VOLUME_ID;

	DiskInfo_t *IndexFile = fileinfo->FileInfo_DiskInfo;
	IndexFile->drive_read_page(IndexFile->DiskInfo_SdDriverStructAddr ,fileinfo->FileInfo_BufferAddr, IndexFile->DiskInfo_DataSectionBegin, fileinfo->FileInfo_DiskInfo->DiskInfo_SectorsPerCluster);
	_FatData_GoToRoot(fileinfo);
	_FatData_ReadPointedFileDetails(fileinfo);
	IndexFile->DiskInfo_OppenedSesions++;
	return fileinfo;
}
//***********************************************************
bool _FatData_CloseSesion(FileInfo_t *fileinfo)
{
	if(!fileinfo) return false;
	//struct FileInfo_Struct *fileinfo = stream->_cookie;
	DiskInfo_t *IndexFile = fileinfo->FileInfo_DiskInfo;
	if(!fileinfo->FileInfo_BufferAddr) return false;
	IndexFile->DiskInfo_OppenedSesions--;
	if(fileinfo->FileInfo_BufferAddr) free(fileinfo->FileInfo_BufferAddr);
	if(fileinfo) free(fileinfo);
	return true;
}
//***********************************************************
FileInfo_t *_FatData_SearchFileDirectory(unsigned int DriveNr, char *Str, unsigned char *Status)
{
	FileInfo_t *fileinfo = _FatData_OpenSesion(DriveNr);
	if(!fileinfo)
	{
		*Status = FileSearch_Status_Directory_not_exist;
		return NULL;
	}
	*Status = FileSearch_Status_Directory_not_exist;
	bool Response = true;
	char *Ptr1 = Str;
	char *Ptr2 = Str;
	Ptr1 = strstr(Ptr1, ":/") + 2;
	Ptr2 = strchr(Ptr1, '/');
	while (Ptr1 != Ptr2 && Ptr2 != NULL)
	{
		//UARTprintf(DebugCom, "Pointed file: %s\n\r" , str_to_upercase(fileinfo->FileInfo_PointedFileInDirectoryLongFileName));
		while(!memcmp(str_to_upercase(Ptr1), str_to_upercase(fileinfo->FileInfo_PointedFileInDirectoryLongFileName), Ptr2 - Ptr1) && Response)
		{
			Response = _FatData_Dn(fileinfo);
			//UARTprintf(DebugCom, "Pointed file: %s\n\r" , str_to_upercase(fileinfo->FileInfo_PointedFileInDirectoryLongFileName));
		}
		if(Response == false || _FatData_OpenFile(fileinfo) == false) 
		{
			_FatData_CloseSesion(fileinfo);
			*Status = FileSearch_Status_File_not_exist;
			return NULL;
		}			
		Ptr1 = Ptr2 + 1;
		Ptr2 = strchr(Ptr1, '/');
		if(!Ptr2) break;
	}			
	while (1)
	{
		if(!memcmp(str_to_upercase(Ptr1), str_to_upercase(fileinfo->FileInfo_PointedFileInDirectoryLongFileName), strlen(Ptr1)))
		{
			//UARTprintf(DebugCom, "File finded: %s\n\r" , str_to_upercase(fileinfo->FileInfo_PointedFileInDirectoryLongFileName));
			if(fileinfo->FileInfo_PointedFileInDirectory_Address == 0 || fileinfo->FileInfo_PointedFileInDirectory_FileSize == 0) 
			{
				*Status = FileSearch_Status_FileEmpty;
				_FatData_CloseSesion(fileinfo);
				return NULL;
			}				
			else 
			{
				fileinfo->OppenedFileIsEmpty = false;
				if(!_FatData_OpenFile(fileinfo))
				{
					_FatData_CloseSesion(fileinfo);
					*Status = FileSearch_Status_File_can_not_be_opened;
					return NULL;
				}
				else
				{
					//UARTprintf(DebugCom, "Opened file: %s\n\r" , str_to_upercase(fileinfo->FileInfo_PointedFileInDirectoryLongFileName));
					//_FatData_CloseFile(fileinfo);
					*Status = FileSearch_Status_File_open_ok;
					return fileinfo;
				}
			}				
		}
		else
		{
			if(!_FatData_Dn(fileinfo))
			{
				_FatData_CloseSesion(fileinfo);
				*Status = FileSearch_Status_File_not_exist;
				return NULL;
			}
		}		
	}	
	//return fileinfo;
}
//***********************************************************
bool FatData_CloseFile(FileInfo_t *fileinfo)
{
	if(!fileinfo) return false;
	_FatData_CloseSesion(fileinfo);
	return true;
}

//***********************************************************
//***********************************************************
//***********************************************************
//***********************************************************
//***********************************************************
#ifdef Fs_RW
//***********************************************************
static uint32_t _FatData_GetFreeIndexLocation(FileInfo_t *fileinfo, uint32_t BeginToSearchIndex)
{
	uint32_t IndexCount = BeginToSearchIndex;
	uint32_t TmpIndex = 0;
	do 
	{
		TmpIndex = _Fat_IndexReadWrite(fileinfo->FileInfo_DiskInfo, IndexCount, 0, Fat_IndexRd);
		if (!(TmpIndex+2)) break;
		IndexCount++;
	} 
	while (1);
	return IndexCount;
}
//***********************************************************
static void _FatData_ExtendFile(FileInfo_t *fileinfo,uint8_t FileType, uint32_t Indexes)
{
	unsigned long Index = fileinfo->FileInfo_IndexOnDisk;
	unsigned long _Index = 0;
	unsigned long FreeIndex = 0;
	//Check if is last index on file, also go to last index on file
	do 
	{
		_Index = _Fat_IndexRd(fileinfo->FileInfo_DiskInfo, Index);
		if(_Index+2 == LastClusterInFile) break;
		Index = _Index;
	} while (1);
	for(unsigned long ExtendFileCount = 0; ExtendFileCount < Indexes; ExtendFileCount++)
	{
		FreeIndex = _FatData_GetFreeIndexLocation(fileinfo, FreeIndex+1);
		_Fat_IndexWr(fileinfo->FileInfo_DiskInfo, Index, FreeIndex);
		unsigned long Fat12_16_RootDisplacement = 0;
#if defined (Fs_Fat16_Support) || defined (Fs_Fat32_Support)
		if(fileinfo->FileInfo_DiskInfo->DiskInfo_FatType != FAT_TYPE_FAT32) Fat12_16_RootDisplacement = 32;
#endif
		unsigned long ClusterToClear = fileinfo->FileInfo_DiskInfo->DiskInfo_DataSectionBegin + Fat12_16_RootDisplacement + (FreeIndex << fileinfo->FileInfo_DiskInfo->DiskInfo_SectorPerClusterMask) + fileinfo->FileInfo_OldPageInSector;
		if(FileType == FileType_Directory)
		{
			memset(fileinfo->FileInfo_BufferAddr, 0, fileinfo->FileInfo_DiskInfo->DiskInfo_BytesPerSector << fileinfo->FileInfo_DiskInfo->DiskInfo_SectorPerClusterMask);
			fileinfo->FileInfo_DiskInfo->drive_write_page(fileinfo->FileInfo_DiskInfo->DiskInfo_SdDriverStructAddr, fileinfo->FileInfo_BufferAddr, ClusterToClear, fileinfo->FileInfo_DiskInfo->DiskInfo_SectorsPerCluster);
			//for (uint8_t Tmp = 0; Tmp < fileinfo->FileInfo_DiskInfo->DiskInfo_SectorsPerCluster; Tmp++)
			//{
			//	fileinfo->FileInfo_DiskInfo->drive_write_page(SdDriverStruct, fileinfo->FileInfo_BufferAddr, ClusterToClear++, fileinfo->FileInfo_DiskInfo->DiskInfo_SectorsPerCluster);
			//}
		}		
		Index = FreeIndex;
	}		
	_Fat_IndexWr(fileinfo->FileInfo_DiskInfo, FreeIndex, LastClusterInFile-2);
	_Fat_IndexSaveBuffer(fileinfo->FileInfo_DiskInfo);
}
//***********************************************************
static unsigned long _FatData_SearchEmptyEntry(FileInfo_t *fileinfo)
{
	if((fileinfo->FileInfo_CurrentOppenedFile_Attributes & FILE_ATTR_DIRECTORY) || (fileinfo->FileInfo_CurrentOppenedFile_Attributes & FILE_ATTR_VOLUME_ID))
	{
		unsigned long EntryCount = 0;
		do 
		{
			if(_FatData_GoToIndex(fileinfo, EntryCount)); else _FatData_ExtendFile(fileinfo, FileType_Directory, 1);
			if(fileinfo->FileInfo_BufferAddr[(EntryCount & (((unsigned long)fileinfo->FileInfo_DiskInfo->DiskInfo_SectorsPerCluster<<9)-1)/*0x1FF*/) + 0x0B] == 0) return EntryCount>>5;
			EntryCount += 32;
		} while (1);
	}
	return EOF;
}
//***********************************************************
signed char _FatData_File_Create(FileInfo_t *fileinfo, char *Name, char *Extension, unsigned char Attr, unsigned long Size)
{
	//struct DiskInfo_Struct *IndexFile = fileinfo->FileInfo_DiskInfo->udata;
	if((fileinfo->FileInfo_CurrentOppenedFile_Attributes & FILE_ATTR_VOLUME_ID) || (fileinfo->FileInfo_CurrentOppenedFile_Attributes & FILE_ATTR_DIRECTORY))
	{
		unsigned long EntryOffsetAddr = _FatData_SearchEmptyEntry(fileinfo);
		if(EntryOffsetAddr == EOF) return EOF;
		else
		{
			unsigned long Mask = (((unsigned long)fileinfo->FileInfo_DiskInfo->DiskInfo_SectorsPerCluster<<9)-1)/*0x1FF*/;
			EntryOffsetAddr = EntryOffsetAddr<<5;
			unsigned long StringLength = strlen(Name);
			memcpy(fileinfo->FileInfo_BufferAddr + (EntryOffsetAddr & Mask), Name, StringLength);
			if(StringLength < 8) memset(fileinfo->FileInfo_BufferAddr + (EntryOffsetAddr & Mask) + StringLength, ' ', 8 - StringLength);
			if(Attr & FILE_ATTR_DIRECTORY) memset(fileinfo->FileInfo_BufferAddr + ((EntryOffsetAddr & Mask) + 0x08), ' ', 3);
			else memcpy(fileinfo->FileInfo_BufferAddr + ((EntryOffsetAddr & Mask) + 0x08), Extension, 3);
			if(Attr == FILE_ATTR_DIRECTORY) fileinfo->FileInfo_BufferAddr[(EntryOffsetAddr & Mask) + 0x0B] = Attr | FILE_ATTR_DIRECTORY;
			else fileinfo->FileInfo_BufferAddr[(EntryOffsetAddr & Mask) + 0x0B] = Attr;
			

			convert32to8 ConvertTmp;
			ConvertTmp.i32 = Size;
			fileinfo->FileInfo_BufferAddr[(EntryOffsetAddr & Mask) + 0x1C] = ConvertTmp.Byte0;
			fileinfo->FileInfo_BufferAddr[(EntryOffsetAddr & Mask) + 0x1D] = ConvertTmp.Byte1;
			fileinfo->FileInfo_BufferAddr[(EntryOffsetAddr & Mask) + 0x1E] = ConvertTmp.Byte2;
			fileinfo->FileInfo_BufferAddr[(EntryOffsetAddr & Mask) + 0x1F] = ConvertTmp.Byte3;
			
			if(Attr & FILE_ATTR_DIRECTORY)
			{
				unsigned long CurrentDirectoryAddr = fileinfo->FileInfo_CurrentOppenedFile_Address;
				unsigned long AddressOfNewFile = _FatData_GetFreeIndexLocation(fileinfo, 0);
				ConvertTmp.i32 = AddressOfNewFile + 2;
				fileinfo->FileInfo_BufferAddr[(EntryOffsetAddr & Mask) + 0x1A] = ConvertTmp.Byte0;
				fileinfo->FileInfo_BufferAddr[(EntryOffsetAddr & Mask) + 0x1B] = ConvertTmp.Byte1;
				fileinfo->FileInfo_BufferAddr[(EntryOffsetAddr & Mask) + 0x14] = ConvertTmp.Byte2;
				fileinfo->FileInfo_BufferAddr[(EntryOffsetAddr & Mask) + 0x15] = ConvertTmp.Byte3;
				fileinfo->FileInfo_Flags |= (1<<Fat_Index_BufferModified);
				_FatData_SaveBuffer(fileinfo);
				_Fat_IndexWr(fileinfo->FileInfo_DiskInfo, AddressOfNewFile, LastClusterInFile-2);
				_Fat_IndexSaveBuffer(fileinfo->FileInfo_DiskInfo);
				fileinfo->FileInfo_PointedFileInDirectory_Address = AddressOfNewFile + 2;
				fileinfo->FileInfo_CurrentOppenedFile_Attributes = FILE_ATTR_DIRECTORY;
				_FatData_OpenFile(fileinfo);
				
				unsigned long ClusterToClear = AddressOfNewFile + 2;
				memset(fileinfo->FileInfo_BufferAddr, 0, fileinfo->FileInfo_DiskInfo->DiskInfo_BytesPerSector << fileinfo->FileInfo_DiskInfo->DiskInfo_SectorPerClusterMask);
				fileinfo->FileInfo_DiskInfo->drive_write_page(fileinfo->FileInfo_DiskInfo->DiskInfo_SdDriverStructAddr, fileinfo->FileInfo_BufferAddr, ClusterToClear, fileinfo->FileInfo_DiskInfo->DiskInfo_SectorsPerCluster);
				//for (uint8_t Tmp = 0; Tmp < IndexFile->DiskInfo_SectorsPerCluster; Tmp++)
				//{
				//	sd_write_page(SdDriverStruct, fileinfo->FileInfo_BufferAddr, ClusterToClear++);
				//}
				
				
				EntryOffsetAddr = 0;
				
				memset(fileinfo->FileInfo_BufferAddr + 1, ' ', 10);
				fileinfo->FileInfo_BufferAddr[0] = '.';
				ConvertTmp.i32 = 0;
				fileinfo->FileInfo_BufferAddr[(EntryOffsetAddr & Mask) + 0x1C] = ConvertTmp.Byte0;
				fileinfo->FileInfo_BufferAddr[(EntryOffsetAddr & Mask) + 0x1D] = ConvertTmp.Byte1;
				fileinfo->FileInfo_BufferAddr[(EntryOffsetAddr & Mask) + 0x1E] = ConvertTmp.Byte2;
				fileinfo->FileInfo_BufferAddr[(EntryOffsetAddr & Mask) + 0x1F] = ConvertTmp.Byte3;
				ConvertTmp.i32 = AddressOfNewFile + 2;
				fileinfo->FileInfo_BufferAddr[(EntryOffsetAddr & Mask) + 0x1A] = ConvertTmp.Byte0;
				fileinfo->FileInfo_BufferAddr[(EntryOffsetAddr & Mask) + 0x1B] = ConvertTmp.Byte1;
				fileinfo->FileInfo_BufferAddr[(EntryOffsetAddr & Mask) + 0x14] = ConvertTmp.Byte2;
				fileinfo->FileInfo_BufferAddr[(EntryOffsetAddr & Mask) + 0x15] = ConvertTmp.Byte3;

				memset(fileinfo->FileInfo_BufferAddr + 34, ' ', 9);
				fileinfo->FileInfo_BufferAddr[32] = '.';
				fileinfo->FileInfo_BufferAddr[33] = '.';
				EntryOffsetAddr += 32;
				ConvertTmp.i32 = 0;
				fileinfo->FileInfo_BufferAddr[(EntryOffsetAddr & Mask) + 0x1C] = ConvertTmp.Byte0;
				fileinfo->FileInfo_BufferAddr[(EntryOffsetAddr & Mask) + 0x1D] = ConvertTmp.Byte1;
				fileinfo->FileInfo_BufferAddr[(EntryOffsetAddr & Mask) + 0x1E] = ConvertTmp.Byte2;
				fileinfo->FileInfo_BufferAddr[(EntryOffsetAddr & Mask) + 0x1F] = ConvertTmp.Byte3;
				ConvertTmp.i32 = CurrentDirectoryAddr + 2;
				fileinfo->FileInfo_BufferAddr[(EntryOffsetAddr & Mask) + 0x1A] = ConvertTmp.Byte0;
				fileinfo->FileInfo_BufferAddr[(EntryOffsetAddr & Mask) + 0x1B] = ConvertTmp.Byte1;
				fileinfo->FileInfo_BufferAddr[(EntryOffsetAddr & Mask) + 0x14] = ConvertTmp.Byte2;
				fileinfo->FileInfo_BufferAddr[(EntryOffsetAddr & Mask) + 0x15] = ConvertTmp.Byte3;
				fileinfo->FileInfo_Flags |= (1<<Fat_Index_BufferModified);
				_FatData_SaveBuffer(fileinfo);
				
				fileinfo->FileInfo_PointedFileInDirectory_Address = CurrentDirectoryAddr;
				fileinfo->FileInfo_CurrentOppenedFile_Attributes = FILE_ATTR_DIRECTORY;
				_FatData_OpenFile(fileinfo);
			}
			else
			{
				ConvertTmp.i32 = 0;
				fileinfo->FileInfo_BufferAddr[(EntryOffsetAddr & Mask) + 0x1A] = ConvertTmp.Byte0;
				fileinfo->FileInfo_BufferAddr[(EntryOffsetAddr & Mask) + 0x1B] = ConvertTmp.Byte1;
				fileinfo->FileInfo_BufferAddr[(EntryOffsetAddr & Mask) + 0x14] = ConvertTmp.Byte2;
				fileinfo->FileInfo_BufferAddr[(EntryOffsetAddr & Mask) + 0x15] = ConvertTmp.Byte3;
				fileinfo->FileInfo_Flags |= (1<<Fat_Index_BufferModified);
				_FatData_SaveBuffer(fileinfo);
			}
		}
	}
	return -1;
}
//***********************************************************
#endif
