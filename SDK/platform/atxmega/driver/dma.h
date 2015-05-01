/*
 * dma.h
 *
 * Created: 2/10/2013 6:18:13 PM
 *  Author: XxXx
 */ 


#ifndef DMA_H_
#define DMA_H_

#include <avr/io.h>
#include <avr/interrupt.h>
//#####################################################
#define _Dma_DoubleBuffMode_Disabled			(0)
#define _Dma_DoubleBuffMode_Ch01				(1)
#define _Dma_DoubleBuffMode_Ch23				(2)
#define _Dma_DoubleBuffMode_Ch01Ch23			(3)
//-----------------------------------------------------
#define _Dma_ChanelPriority_RoundRobin0123		(0)
#define _Dma_ChanelPriority_Ch0RoundRobin123	(1)
#define _Dma_ChanelPriority_Ch01RoundRobin23	(2)
#define _Dma_ChanelPriority_Ch0123				(3)
//#####################################################
#define _Dma_Transfer_Complete_bm				(1<<0)
#define _Dma_Transfer_Complete_bp				(0)


#define _Dma_Int_Disabled						(0)
#define _Dma_Int_Low_Level						(1)
#define _Dma_Int_Med_Level						(2)
#define _Dma_Int_High_Level						(3)
//-----------------------------------------------------
#define _Dma_BurstMode_1						(0)
#define _Dma_BurstMode_2						(1)
#define _Dma_BurstMode_4						(2)
#define _Dma_BurstMode_8						(3)
//-----------------------------------------------------
#define _Dma_SourceAddressReload_None			(0)
#define _Dma_SourceAddressReload_Block			(1)
#define _Dma_SourceAddressReload_Burst			(2)
#define _Dma_SourceAddressReload_Transaction	(3)
//-----------------------------------------------------
#define _Dma_SourceAddressMode_Fixed			(0)
#define _Dma_SourceAddressMode_Inc				(1)
#define _Dma_SourceAddressMode_Dec				(2)
//-----------------------------------------------------
#define _Dma_DestAddressReload_None				(0)
#define _Dma_DestAddressReload_Block			(1)
#define _Dma_DestAddressReload_Burst			(2)
#define _Dma_DestAddressReload_Transaction		(3)
//-----------------------------------------------------
#define _Dma_DestAddressMode_Fixed				(0)
#define _Dma_DestAddressMode_Inc				(1)
#define _Dma_DestAddressMode_Dec				(2)
//-----------------------------------------------------
//Triggers for dma request
#define _Dma_Triger_Off							(0)
//----------------------------
#define _Dma_Triger_EventSystem_Ch0				(0x01 + 0x00)
#define _Dma_Triger_EventSystem_Ch1				(0x01 + 0x01)
#define _Dma_Triger_EventSystem_Ch2				(0x01 + 0x02)
//----------------------------
#define _Dma_Triger_Adca_Ch0					(0x10 + 0x00)
#define _Dma_Triger_Adca_Ch1					(0x10 + 0x01)
#define _Dma_Triger_Adca_Ch2					(0x10 + 0x02)
#define _Dma_Triger_Adca_Ch3					(0x10 + 0x03)
#define _Dma_Triger_Adca_Ch4					(0x10 + 0x04)
//----------------------------
#define _Dma_Triger_Daca_Ch0					(0x15 + 0x00)
#define _Dma_Triger_Daca_Ch1					(0x15 + 0x01)
#define _Dma_Triger_Daca_Ch2					(0x15 + 0x02)
#define _Dma_Triger_Daca_Ch3					(0x15 + 0x03)
#define _Dma_Triger_Daca_Ch4					(0x15 + 0x04)
//----------------------------
//----------------------------
#define _Dma_Triger_Adcb_Ch0					(0x20 + 0x00)
#define _Dma_Triger_Adcb_Ch1					(0x20 + 0x01)
#define _Dma_Triger_Adcb_Ch2					(0x20 + 0x02)
#define _Dma_Triger_Adcb_Ch3					(0x20 + 0x03)
#define _Dma_Triger_Adcb_Ch4					(0x20 + 0x04)
//----------------------------
#define _Dma_Triger_Dacb_Ch0					(0x25 + 0x00)
#define _Dma_Triger_Dacb_Ch1					(0x25 + 0x01)
#define _Dma_Triger_Dacb_Ch2					(0x25 + 0x02)
#define _Dma_Triger_Dacb_Ch3					(0x25 + 0x03)
#define _Dma_Triger_Dacb_Ch4					(0x25 + 0x04)
//----------------------------
#define _Dma_Triger_Tcc0_Ovf					(0x40 + 0x00)
#define _Dma_Triger_Tcc0_Err					(0x40 + 0x01)
#define _Dma_Triger_Tcc0_Cca					(0x40 + 0x02)
#define _Dma_Triger_Tcc0_Ccb					(0x40 + 0x03)
#define _Dma_Triger_Tcc0_Ccc					(0x40 + 0x04)
#define _Dma_Triger_Tcc0_Ccd					(0x40 + 0x05)
//----------------------------
#define _Dma_Triger_Tcc1_Ovf					(0x46 + 0x00)
#define _Dma_Triger_Tcc1_Err					(0x46 + 0x01)
#define _Dma_Triger_Tcc1_Cca					(0x46 + 0x02)
#define _Dma_Triger_Tcc1_Ccb					(0x46 + 0x03)
#define _Dma_Triger_Tcc1_Ccc					(0x46 + 0x04)
#define _Dma_Triger_Tcc1_Ccd					(0x46 + 0x05)
//----------------------------
#define _Dma_Triger_Spic						(0x4A + 0x00)
//----------------------------
#define _Dma_Triger_Usartc0_Rxc					(0x4B + 0x00)
#define _Dma_Triger_Usartc0_Dre					(0x4B + 0x01)
//----------------------------
#define _Dma_Triger_Usartc1_Rxc					(0x4E + 0x00)
#define _Dma_Triger_Usartc1_Dre					(0x4E + 0x01)
//----------------------------
#define _Dma_Triger_Tcd0_Ovf					(0x60 + 0x00)
#define _Dma_Triger_Tcd0_Err					(0x60 + 0x01)
#define _Dma_Triger_Tcd0_Cca					(0x60 + 0x02)
#define _Dma_Triger_Tcd0_Ccb					(0x60 + 0x03)
#define _Dma_Triger_Tcd0_Ccc					(0x60 + 0x04)
#define _Dma_Triger_Tcd0_Ccd					(0x60 + 0x05)
//----------------------------
#define _Dma_Triger_Tcd1_Ovf					(0x66 + 0x00)
#define _Dma_Triger_Tcd1_Err					(0x66 + 0x01)
#define _Dma_Triger_Tcd1_Cca					(0x66 + 0x02)
#define _Dma_Triger_Tcd1_Ccb					(0x66 + 0x03)
#define _Dma_Triger_Tcd1_Ccc					(0x66 + 0x04)
#define _Dma_Triger_Tcd1_Ccd					(0x66 + 0x05)
//----------------------------
#define _Dma_Triger_Spid						(0x6A + 0x00)
//----------------------------
#define _Dma_Triger_Usartd0_Rxc					(0x6B + 0x00)
#define _Dma_Triger_Usartd0_Dre					(0x6B + 0x01)
//----------------------------
#define _Dma_Triger_Usartd1_Rxc					(0x6E + 0x00)
#define _Dma_Triger_Usartd1_Dre					(0x6E + 0x01)
//----------------------------
#define _Dma_Triger_Tce0_Ovf					(0x80 + 0x00)
#define _Dma_Triger_Tce0_Err					(0x80 + 0x01)
#define _Dma_Triger_Tce0_Cca					(0x80 + 0x02)
#define _Dma_Triger_Tce0_Ccb					(0x80 + 0x03)
#define _Dma_Triger_Tce0_Ccc					(0x80 + 0x04)
#define _Dma_Triger_Tce0_Ccd					(0x80 + 0x05)
//----------------------------
#define _Dma_Triger_Tce1_Ovf					(0x86 + 0x00)
#define _Dma_Triger_Tce1_Err					(0x86 + 0x01)
#define _Dma_Triger_Tce1_Cca					(0x86 + 0x02)
#define _Dma_Triger_Tce1_Ccb					(0x86 + 0x03)
#define _Dma_Triger_Tce1_Ccc					(0x86 + 0x04)
#define _Dma_Triger_Tce1_Ccd					(0x86 + 0x05)
//----------------------------
#define _Dma_Triger_Spie						(0x8A + 0x00)
//----------------------------
#define _Dma_Triger_Usarte0_Rxc					(0x8B + 0x00)
#define _Dma_Triger_Usarte0_Dre					(0x8B + 0x01)
//----------------------------
#define _Dma_Triger_Usarte1_Rxc					(0x8E + 0x00)
#define _Dma_Triger_Usarte1_Dre					(0x8E + 0x01)
//----------------------------
#define _Dma_Triger_Tcf0_Ovf					(0xA0 + 0x00)
#define _Dma_Triger_Tcf0_Err					(0xA0 + 0x01)
#define _Dma_Triger_Tcf0_Cca					(0xA0 + 0x02)
#define _Dma_Triger_Tcf0_Ccb					(0xA0 + 0x03)
#define _Dma_Triger_Tcf0_Ccc					(0xA0 + 0x04)
#define _Dma_Triger_Tcf0_Ccd					(0xA0 + 0x05)
//----------------------------
#define _Dma_Triger_Tcf1_Ovf					(0xA6 + 0x00)
#define _Dma_Triger_Tcf1_Err					(0xA6 + 0x01)
#define _Dma_Triger_Tcf1_Cca					(0xA6 + 0x02)
#define _Dma_Triger_Tcf1_Ccb					(0xA6 + 0x03)
#define _Dma_Triger_Tcf1_Ccc					(0xA6 + 0x04)
#define _Dma_Triger_Tcf1_Ccd					(0xA6 + 0x05)
//----------------------------
#define _Dma_Triger_Spif						(0xAA + 0x00)
//----------------------------
#define _Dma_Triger_Usartf0_Rxc					(0xAB + 0x00)
#define _Dma_Triger_Usartf0_Dre					(0xAB + 0x01)
//----------------------------
#define _Dma_Triger_Usartf1_Rxc					(0xAE + 0x00)
#define _Dma_Triger_Usartf1_Dre					(0xAE + 0x01)
//#####################################################
typedef struct
{
	unsigned long Dma_SrcAddr;						//Here, user will store the source address from where DMA channel will get the data
	unsigned long Dma_DestAddr;						//Here, user will store the destination address where to store data the DMA channel
	DMA_CH_t *Dma_BaseAddr;						//Here will be stored the DMA channel Base address by the "dma_search_and_use" function
	unsigned short Dma_BlockLength;					//0..65535
	//	unsigned char  Dma_Number;						//0..3//Here "dma_search_and_use" function will store the DMA channe number that will be used for current transaction
	unsigned char  Dma_RepeatNr;						//0..255
	unsigned char  Dma_RepeatMode;					//True, False
	unsigned char  Dma_SingleShot;					//True, False
	unsigned char  Dma_BurstMode;						//_Dma_BurstMode_1, _Dma_BurstMode_2, _Dma_BurstMode_4, _Dma_BurstMode_8
	unsigned char  Dma_ErrIntLevel;					//_Dma_Int_Disabled, _Dma_Int_Low_Level, _Dma_Int_Med_Level, _Dma_Int_High_Level
	unsigned char  Dma_CompleteIntLevel;				//_Dma_Int_Disabled, _Dma_Int_Low_Level, _Dma_Int_Med_Level, _Dma_Int_High_Level
	unsigned char  Dma_SrcAddrReload;					//_Dma_SourceAddressReload_None, _Dma_SourceAddressReload_Block, _Dma_SourceAddressReload_Burst, _Dma_SourceAddressReload_Transaction
	unsigned char  Dma_SrcAddrMode;					//_Dma_SourceAddressMode_Fixed, _Dma_SourceAddressMode_Inc, _Dma_SourceAddressMode_Dec
	unsigned char  Dma_DestAddrReload;				//_Dma_DestAddressReload_None, _Dma_DestAddressReload_Block, _Dma_DestAddressReload_Burst, _Dma_DestAddressReload_Transaction
	unsigned char  Dma_DestAddrMode;					//_Dma_DestAddressMode_Fixed, _Dma_DestAddressMode_Inc, _Dma_DestAddressMode_Dec
	unsigned char  Dma_Trigers;
	unsigned char  Dma_Status;						//Here, user can read if transfer is complete
	void* CallBackTransferEndData;
	void(*CallBackTransferEnd)(void*);
}Dma_t;
//-----------------------------------------------------
typedef struct
{
	unsigned char Dma_Status;
	unsigned char Dma_DoubleBuffMode;		//
	unsigned char Dma_ChanelPriority;		//
}Dma_Gen_t;
//#####################################################
typedef struct
{
	Dma_Gen_t *DmaGeneralStructAddress;
	Dma_t *DmaCh0RamServiceAddr;
	Dma_t *DmaCh1RamServiceAddr;
	Dma_t *DmaCh2RamServiceAddr;
	Dma_t *DmaCh3RamServiceAddr;
} DmaServiceAddresses;
//#####################################################
#define new_dma_gen Dma_Gen_t
#ifndef new_
#define new_(structure) (structure*)calloc(1,sizeof(structure));
#endif
#define free_dma_gen(address) free(address);
//#####################################################
#define new_dma_ch Dma_t
#ifndef new_
#define new_(structure) (structure*)calloc(1,sizeof(structure));
#endif
#define free_dma_ch(address) free(address);
//#######################################################
bool dma_enable(Dma_Gen_t *DmaStruct);
bool dma_disable(Dma_Gen_t *DmaStruct);
bool dma_enabled(void);
unsigned char dma_search_and_use(Dma_t *DmaStruct, unsigned char ChNr);
void dma_ch_dma_enable(Dma_t *DmaStruct);
void dma_src_addr_set(Dma_t *DmaStruct, unsigned long Addr);
void dma_dest_addr_set(Dma_t *DmaStruct, unsigned long Addr);
void dma_ch_repeat_enable(Dma_t *DmaStruct);
void dma_repeat_nr_set(Dma_t *DmaStruct, unsigned char RepeatCnt);
void dma_blk_len_set(Dma_t *DmaStruct, unsigned short BlkLen);
unsigned char dma_ch_busy(Dma_t *DmaStruct);
void dma_transfer_request(Dma_t *DmaStruct);
void dma_complete_int(Dma_t *DmaStruct);
//#######################################################
extern DmaServiceAddresses Dma_ServiceAddresses;
//#######################################################
#ifdef HEADER_INCLUDE_C_FILES
#include "dma.c"
#endif

#endif /* DMA_H_ */