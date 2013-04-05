/*
 *  lib/fs/fat.h
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

#ifndef __Fat__
#define __Fat__
//#####################################################
#define Fs_Fat16_Support
#define Fs_Fat32_Support
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "inttypes.h"
#include <stdlib.h>
#ifdef __UNIONS__
#include "general/unions.h"
#endif
//#include "../../sys/Wdt.h"
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
//***********************************************************
//***********************************************************
//#####################################################
#ifndef False
#define False	0
#endif

#ifndef True
#define True	1
#endif

#ifndef Null
#define Null	-1
#endif
//#####################################################
//#include "Driver/SD.c"
//#####################################################
//#####################################################
//#####################################################
//#####################################################
//#####################################################
//#####################################################
//#####################################################
/*
 * FatDefinitions
 *
 *  Created on: Apr 10, 2010
 *      Author: Morgoth
 */

#ifndef FATDEFINITIONS_H_
#define FATDEFINITIONS_H_

// Boot Sector
#define BS_jmpBoot				 					0	// Length = 3
#define BS_OEMName				 					3	// Length = 8
#define BPB_BytsPerSec			 					11	// Length = 2
#define BPB_SecPerClus			 					13	// Length = 1
#define BPB_RsvdSecCnt			 					14	// Length = 2;And begin of first alocation table
#define BPB_NumFATs			 						16	// Length = 1
#define BPB_RootEntCnt			 					17	// Length = 2
#define BPB_TotSec16			 					19	// Length = 2
#define BPB_Media				 					21	// Length = 1
#define BPB_FATSz16			 						22	// Length = 2
#define BPB_SecPerTrk			 					24	// Length = 2
#define BPB_NumHeads			 					26	// Length = 2
#define BPB_HiddSec			 						28	// Length = 4
#define BPB_TotSec32			 					32	// Length = 4

// FAT 12/16
#define BS_FAT_DrvNum			 					36	// Length = 1
#define BS_FAT_BootSig			 					38	// Length = 1
#define BS_FAT_VolID			 					39	// Length = 4
#define BS_FAT_VolLab			 					43	// Length = 11
#define BS_FAT_FilSysType		 					54	// Length = 8

// FAT 32
#define BPB_FAT32_FATSz32		 					36	// Length = 4;And size of alocation table
#define BPB_FAT32_ExtFlags		 					40	// Length = 2
#define BPB_FAT32_FSVer		 						42	// Length = 2
#define BPB_FAT32_RootClus		 					44	// Length = 4
#define BPB_FAT32_FSInfo		 					48	// Length = 2
#define BPB_FAT32_BkBootSec	 						50	// Length = 2
#define BS_FAT32_DrvNum		 						64	// Length = 1
#define BS_FAT32_BootSig		 					66	// Length = 1
#define BS_FAT32_VolID			 					67	// Length = 4
#define BS_FAT32_VolLab		 						71	// Length = 11
#define BS_FAT32_FilSysType	 						82	// Length = 8                    1005056

//-----------------------------------------------------------------------------
// FAT Types
//-----------------------------------------------------------------------------
#define FAT_TYPE_FAT12			 					1
#define FAT_TYPE_FAT16			 					2
#define FAT_TYPE_FAT32			 					3

//-----------------------------------------------------------------------------
// FAT32 Specific Statics
//-----------------------------------------------------------------------------
#define Signature_Position						  	510
#define Signature_Value							  	0xAA55
#define PARTITION1_TYPECODE_LOCATION	 		  	450
#define FAT32_TYPECODE1						 	  	0x0B
#define FAT32_TYPECODE2						 	  	0x0C
#define PARTITION1_LBA_BEGIN_LOCATION	 		  	454

//-----------------------------------------------------------------------------
// FAT32 File Attributes and Types
//-----------------------------------------------------------------------------
#define FILE_ATTR_READ_ONLY   	 					0x01
#define FILE_ATTR_HIDDEN 		 					0x02
#define FILE_ATTR_SYSTEM 		 					0x04
#define FILE_ATTR_SYSHID		 					0x06
#define FILE_ATTR_VOLUME_ID 	 					0x08
#define FILE_ATTR_DIRECTORY	 						0x10
#define FILE_ATTR_ARCHIVE  	 						0x20
#define FILE_ATTR_LFN_TEXT		 					0x0F
#define FILE_HEADER_BLANK		 					0x00
#define FILE_HEADER_DELETED	 						0xE5
#define FILE_TYPE_DIR			 					0x10
#define FILE_TYPE_FILE			 					0x20

#define FILE_ATTR_READ_ONLY_    					0x00
#define FILE_ATTR_HIDDEN_ 		 					0x01
#define FILE_ATTR_SYSTEM_ 		 					0x02
#define FILE_ATTR_VOLUME_ID_ 	 					0x03
#define FILE_ATTR_DIRECTORY_	 					0x04
#define FILE_ATTR_ARCHIVE_  	 					0x05

#endif /* FATDEFINITIONS_H_ */
//#####################################################
//#####################################################
//#####################################################
//#####################################################
//#####################################################
//#####################################################
//#####################################################
/*
 * Fat allocation table management
 *
 *  Created on: Apr 10, 2010
 *      Author: Morgoth
 */
#define		DefaultPagesPerBlock	(1)
#define 	Fat_IndexRd 			(1)
#define 	Fat_IndexWr 			(0)
#define		MaxDrivesNumber			(256)
#define		DefaultPagesPerBlock	(1)

#define		MaxNumOfOppenedFiles	256
//***********************************************************
#define		LastClusterInFile 		(0x0FFFFFFF)
#define		FreeCluster 			(0x00000000)
//***********************************************************
/*typedef struct {
	//unsigned char DriveType;
	//unsigned char FsType;
	unsigned long *FsInfo;
}DriveInfoTable;*/
//***********************************************************
typedef enum{
	Drive_UnknownError = 0,
	Drive_NotMount = 1,
	Drive_Mounted = 2,
	Drive_Busy = 3
}DriveMountResponse;
//***********************************************************
typedef enum{
	NoFs = 0,
	Fat12 = 1,
	Fat16 = 2,
	Fat32 = 3
}FsType;
//***********************************************************
typedef enum{
	Fat_NoDiskInserted = 0,
	Fat_FatDetected = 1,
	Fat_NoFatDetected = 2
}IndexAllocResult;
//***********************************************************
typedef enum{
	NoDrive = 0,
	SdMmc = 1,
	Hdd = 1
}StorageType;
//***********************************************************
typedef enum{
	Table1 = 0,
	Table2 = 1
}AllocTable;
//***********************************************************
typedef enum{
	Fat_Index_BufferModified = 0
}IndexFlags;
//***********************************************************
typedef struct {
	void*				DiskInfo_SdDriverStructAddr;
	unsigned long		DiskInfo_ReservedPagesInFatEntry;
	unsigned long 		DiskInfo_PageNrInBuff;/* long */
	unsigned long 		DiskInfo_TableSize;/* long */
	unsigned long 		DiskInfo_BeginToSearchFreeIndex;/* long */
	unsigned long 		DiskInfo_AllocTable1Begin;/* long */
	unsigned long 		DiskInfo_AllocTableSize;/* long */
	unsigned long 		DiskInfo_DataSectionBegin;/* long */
	unsigned long		DiskInfo_IndexNumber;/* long */
	unsigned long		DiskInfo_BootSectorAddress;/* long */
	unsigned short 		DiskInfo_BytesPerSector;/* short */
	unsigned char 		DiskInfo_Flags;/* char */
	unsigned char		DiskInfo_BufferModified;/* char */
	unsigned char 		DiskInfo_SectorsPerCluster;/* char */
	unsigned char 		DiskInfo_NumberOfFats;/* char */
	unsigned char 		DiskInfo_MediaType;/* char */
	unsigned char		DiskInfo_FatType;/* char */
	unsigned char		DiskInfo_DriveType;/* char */
	unsigned char		DiskInfo_DriveNumber;/* char */
	unsigned char		DiskInfo_AllocTableSel;/* char */
	unsigned char		DiskInfo_SectorPerClusterMask;/* char */
	unsigned char		DiskInfo_RequestForUnmount;/* char */
	unsigned char		DiskInfo_OppenedSesions;/* char */
	unsigned int		(*drive_read_page)(void*, void*, unsigned long, unsigned int);
	unsigned int		(*drive_write_page)(void*, void*, unsigned long, unsigned int);
	unsigned char* 		DiskInfo_Buff1;/* buff of char */
	unsigned char		DiskInfo_PartitionNr;
	char 				DiskInfo_PartitionName;
	//void 				*oppened_files[MaxNumOfOppenedFiles];
//long 			(*drive_init)(void);
}DiskInfo_t;
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
//***********************************************************
/*
 * Fat data management
 *
 *  Created on: Apr 10, 2010
 *      Author: Morgoth
 */
typedef enum{
	FileInfo_BufferModified = 0
}FileInfo_Flags;
//***********************************************************
typedef enum{
	FatData_NavigateUp = 0,
	FatData_NavigateDn
}FileInfo_Navigate_Directions;
//***********************************************************
typedef struct FileInfo_GetFilesLocation_Struct{
	unsigned long NrOfFiles;
	unsigned long* ListOfFiles;
}FileInfo_GetFilesLocation_t;
//***********************************************************
typedef enum{
	FileType_Directory = 0,
	FileType_Archive
}FileType;
//***********************************************************
typedef enum{
	FileSearch_Status_File_open_ok,
	FileSearch_Status_Directory_not_exist,
	FileSearch_Status_File_can_not_be_opened,
	FileSearch_Status_Directory_can_not_be_opened,
	FileSearch_Status_File_not_exist,
	FileSearch_Status_FileEmpty
}FileSearch_Status_enum;
//***********************************************************
typedef struct {
	unsigned char FileInfo_Flags;
	unsigned char* FileInfo_BufferAddr;
	
	unsigned long FileInfo_PageNrInBuffer;
	signed long FileInfo_PointerInDirectory;
	unsigned long FileInfo_PointerInFile;
	unsigned long FileInfo_IndexOnDisk;
	unsigned long FileInfo_IndexOnFile;
	unsigned char FileInfo_OldPageInSector;
	unsigned long FileInfo_OldIndexOnDisk;
	
	unsigned long FileInfo_LastOppenedFile_Address;
	unsigned char  FileInfo_LastOppenedFile_Attributes;
	unsigned char  FileInfo_LastOppenedFile_PageInSector;
	
	unsigned long FileInfo_CurrentOppenedFile_Address;
	unsigned long FileInfo_CurrentOppenedFile_FileSize;
	unsigned char  FileInfo_CurrentOppenedFile_Attributes;
	unsigned char  FileInfo_CurrentOppenedFile_PageInSector;

	unsigned long FileInfo_PointedFileInDirectory_Address;
	unsigned long FileInfo_PointedFileInDirectory_FileSize;
	unsigned char  FileInfo_PointedFileInDirectory_Attribute;
	unsigned long Offset;
	unsigned char SesionBusy;
	
	bool OppenedFileIsEmpty;
	
	char FileInfo_PointedFileInDirectoryExtension[4];
	char FileInfo_PointedFileInDirectoryLongFileName[128];

	DiskInfo_t* FileInfo_DiskInfo;
}FileInfo_t;
//***********************************************************
extern DiskInfo_t *Drives_Table[];
//***********************************************************
unsigned char _Fat_IndexReadFirstBlock(DiskInfo_t *fatinfo);
unsigned char _Fat_IndexReadHighClusterSize(DiskInfo_t *fatinfo);
unsigned long _Fat_IndexReadBeginAddressOfDataTable(DiskInfo_t *fatinfo);
IndexAllocResult _FatReadFsInfo(DiskInfo_t *fatinfo);
unsigned long _Fat_IndexReadWrite(DiskInfo_t *fatinfo,unsigned long Fat_IndexNumber,unsigned long Fat_IndexToWrite,unsigned char Fat_IndexRdWrAction);
#ifdef Fs_Read_And_Write
unsigned char _Fat_IndexWr(DiskInfo_t *fatinfo,unsigned long Fat_IndexNumber,unsigned long Fat_IndexToWrite);
#endif
unsigned long _Fat_IndexRd(DiskInfo_t *fatinfo,unsigned long Fat_IndexNumber);
bool _Fat_Mount(unsigned long DriveNumber);
bool _Fat_Unmount(unsigned long DriveNumber);
//#######################################################
#define new_fat_disk DiskInfo_t
#ifndef new_
#define new_(structure) (structure*)calloc(1,sizeof(structure));
#endif
#define free_fat_disk(address) free(address);
//***********************************************************
void _FatData_ChangePageInBuffer(FileInfo_t *fileinfo,unsigned long PageNumber);
bool _FatData_GoToIndex(FileInfo_t *fileinfo, unsigned long DestinationByte);
void _FatData_GoToRoot(FileInfo_t *fileinfo);
void _FatData_ReadPointedFileDetails(FileInfo_t *fileinfo);
bool _FatData_PointFile(FileInfo_t *fileinfo, unsigned long* Table, unsigned long FileToPoint);
bool _FatData_Up(FileInfo_t *fileinfo);
bool _FatData_Dn(FileInfo_t *fileinfo);
bool _FatData_OpenFile(FileInfo_t *fileinfo);
bool _FatData_CloseFile(FileInfo_t *fileinfo);
//unsigned char _FatData_Back(FileInfo_t *fileinfo);
unsigned char _FatData_ReadWriteByte(FileInfo_t *fileinfo, unsigned char ActionWrite, unsigned long Addr, unsigned char ByteToWrite);
FileInfo_GetFilesLocation_t _FatData_GetFilesLocation(FileInfo_t *fileinfo);
unsigned long _FatData_ReadSection(FileInfo_t *fileinfo, unsigned char *Buff, unsigned long Begin, unsigned long Size);
unsigned char* _FatData_ReadFile(FileInfo_t *fileinfo);
FileInfo_t *_FatData_OpenSesion(unsigned char DriveNr);
bool _FatData_CloseSesion(FileInfo_t *fileinfo);
FileInfo_t *_FatData_SearchFileDirectory(unsigned int FriveNr, char *Str, unsigned char *Status);
#ifdef Fs_RW
signed char _FatData_File_Create(FileInfo_t *fileinfo, char *Name, char *Extension, unsigned char Attr, unsigned long Size);
#endif
//#######################################################
#ifdef HEADER_INCLUDE_C_FILES
#include "fat.c"
#endif
//#######################################################
#endif
//#######################################################







