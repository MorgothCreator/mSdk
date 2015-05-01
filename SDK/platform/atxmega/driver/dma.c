/*
 * dma.c
 *
 * Created: 2/10/2013 6:18:04 PM
 *  Author: XxXx
 */ 
//#####################################################
#include <avr/io.h>
#include <stdbool.h>
#include "dma.h"
//#####################################################
DmaServiceAddresses Dma_ServiceAddresses;
//#####################################################
DMA_CH_t *Dma_GetSetBaseAddress(Dma_t *DmaStruct,unsigned char DmaNumber)
{
	DMA_CH_t *Temp = 0;
	switch(DmaNumber)
	{
		case(0):
		Dma_ServiceAddresses.DmaCh0RamServiceAddr = DmaStruct;
		Temp = (DMA_CH_t *)&DMA.CH0;
		break;
		case(1):
		Dma_ServiceAddresses.DmaCh1RamServiceAddr = DmaStruct;
		Temp = (DMA_CH_t *)&DMA.CH1;
		break;
		case(2):
		Dma_ServiceAddresses.DmaCh2RamServiceAddr = DmaStruct;
		Temp = (DMA_CH_t *)&DMA.CH2;
		break;
		case(3):
		Dma_ServiceAddresses.DmaCh3RamServiceAddr = DmaStruct;
		Temp = (DMA_CH_t *)&DMA.CH3;
		break;
	}
	DmaStruct->Dma_BaseAddr = Temp;
	return Temp;
}
//#####################################################
bool dma_enable(Dma_Gen_t *DmaStruct)
{
	if((DMA.CTRL & DMA_ENABLE_bm) == 0)
	{
		Dma_ServiceAddresses.DmaGeneralStructAddress = DmaStruct;
		unsigned char Temp = DMA.CTRL;
		Temp = (Temp&(~DMA_DBUFMODE_gm))|(DmaStruct->Dma_DoubleBuffMode<<DMA_DBUFMODE_gp);
		Temp = (Temp&(~DMA_PRIMODE_gm))|(DmaStruct->Dma_ChanelPriority<<DMA_PRIMODE_gp);
		Temp = (Temp&(~DMA_ENABLE_bm))|(1<<DMA_ENABLE_bp);
		DMA.CTRL = Temp;
		return true;
	} else 	return false;
}
//#####################################################
bool dma_disable(Dma_Gen_t *DmaStruct)
{
	DMA.CTRL = (DMA.CTRL&(~DMA_ENABLE_bp));
	return true;
}
//#####################################################
bool dma_enabled(void)
{
	if(DMA.CTRL & DMA_ENABLE_bm) return true;
	else return false;
}
//#####################################################
unsigned char dma_search_and_use(Dma_t *DmaStruct, unsigned char ChNr)
{
	unsigned char Ch_Count = 3;
	if(ChNr > 3)
	{
		DMA_CH_t *TempPtr;
		if(ChNr > 8)
		{
			do
			{
				Ch_Count++;
				if(Ch_Count >= 4) Ch_Count = 0;
				TempPtr = Dma_GetSetBaseAddress(DmaStruct,Ch_Count);
			}
			while(TempPtr->CTRLA & DMA_CH_ENABLE_bm);
		}
		else
		{
			TempPtr = Dma_GetSetBaseAddress(DmaStruct,ChNr & 0x03);
			Ch_Count = ChNr & 0x03;
		}
		//DmaStruct->Dma_Number = Ch_Count;
		unsigned char Temp = TempPtr->CTRLA;
		Temp = (Temp&(~DMA_CH_SINGLE_bm))|(DmaStruct->Dma_SingleShot<<DMA_CH_SINGLE_bp);
		Temp = (Temp&(~DMA_CH_BURSTLEN_gm))|(DmaStruct->Dma_BurstMode<<DMA_CH_BURSTLEN_gp);
		Temp = (Temp&(~DMA_CH_REPEAT_bm))|(DmaStruct->Dma_RepeatMode<<DMA_CH_REPEAT_bp);
		TempPtr->CTRLA = Temp;
		Temp = TempPtr->CTRLB;
		Temp = (Temp&(~DMA_CH_ERRINTLVL_gm))|(DmaStruct->Dma_ErrIntLevel<<DMA_CH_ERRINTLVL_gp);
		Temp = (Temp&(~DMA_CH_TRNINTLVL_gm))|(DmaStruct->Dma_CompleteIntLevel<<DMA_CH_TRNINTLVL_gp);
		TempPtr->CTRLB = Temp;
		Temp = TempPtr->ADDRCTRL;
		Temp = (Temp&(~DMA_CH_SRCRELOAD_gm))|(DmaStruct->Dma_SrcAddrReload<<DMA_CH_SRCRELOAD_gp);
		Temp = (Temp&(~DMA_CH_SRCDIR_gm))|(DmaStruct->Dma_SrcAddrMode<<DMA_CH_SRCDIR_gp);
		Temp = (Temp&(~DMA_CH_DESTRELOAD_gm))|(DmaStruct->Dma_DestAddrReload<<DMA_CH_DESTRELOAD_gp);
		Temp = (Temp&(~DMA_CH_DESTDIR_gm))|(DmaStruct->Dma_DestAddrMode<<DMA_CH_DESTDIR_gp);
		TempPtr->ADDRCTRL = Temp;
		TempPtr->TRIGSRC = DmaStruct->Dma_Trigers;
		unsigned short Tmp = DmaStruct->Dma_BlockLength;
		TempPtr->TRFCNTL = Tmp;
		TempPtr->TRFCNTH = Tmp>>8;
		TempPtr->REPCNT = DmaStruct->Dma_RepeatNr;
		unsigned long Tmp1 = DmaStruct->Dma_SrcAddr;
		TempPtr->SRCADDR0 = Tmp1;
		TempPtr->SRCADDR1 = Tmp1>>8;
		TempPtr->SRCADDR2 = Tmp1>>16;
		Tmp1 = DmaStruct->Dma_DestAddr;
		TempPtr->DESTADDR0 = Tmp1;
		TempPtr->DESTADDR1 = Tmp1>>8;
		TempPtr->DESTADDR2 = Tmp1>>16;
		TempPtr->CTRLA |= DMA_CH_ENABLE_bm;
		//dma_transfer_request(DmaStruct);
		return Ch_Count;
	}
	else
	{
		DMA_CH_t *TempPtr = Dma_GetSetBaseAddress(DmaStruct, ChNr & 0x03);
		TempPtr->CTRLA |= DMA_CH_ENABLE_bm;
		return true;
	}
}
//#####################################################
void dma_ch_restore_settings(Dma_t *DmaStruct)
{
	DMA_CH_t *TempPtr = DmaStruct->Dma_BaseAddr;
	unsigned char Temp = TempPtr->CTRLA;
	Temp = (Temp&(~DMA_CH_SINGLE_bm))|(DmaStruct->Dma_SingleShot<<DMA_CH_SINGLE_bp);
	Temp = (Temp&(~DMA_CH_BURSTLEN_gm))|(DmaStruct->Dma_BurstMode<<DMA_CH_BURSTLEN_gp);
	Temp = (Temp&(~DMA_CH_REPEAT_bm))|(DmaStruct->Dma_RepeatMode<<DMA_CH_REPEAT_bp);
	TempPtr->CTRLA = Temp;
	Temp = TempPtr->CTRLB;
	Temp = (Temp&(~DMA_CH_ERRINTLVL_gm))|(DmaStruct->Dma_ErrIntLevel<<DMA_CH_ERRINTLVL_gp);
	Temp = (Temp&(~DMA_CH_TRNINTLVL_gm))|(DmaStruct->Dma_CompleteIntLevel<<DMA_CH_TRNINTLVL_gp);
	TempPtr->CTRLB = Temp;
	Temp = TempPtr->ADDRCTRL;
	Temp = (Temp&(~DMA_CH_SRCRELOAD_gm))|(DmaStruct->Dma_SrcAddrReload<<DMA_CH_SRCRELOAD_gp);
	Temp = (Temp&(~DMA_CH_SRCDIR_gm))|(DmaStruct->Dma_SrcAddrMode<<DMA_CH_SRCDIR_gp);
	Temp = (Temp&(~DMA_CH_DESTRELOAD_gm))|(DmaStruct->Dma_DestAddrReload<<DMA_CH_DESTRELOAD_gp);
	Temp = (Temp&(~DMA_CH_DESTDIR_gm))|(DmaStruct->Dma_DestAddrMode<<DMA_CH_DESTDIR_gp);
	TempPtr->ADDRCTRL = Temp;
	TempPtr->TRIGSRC = DmaStruct->Dma_Trigers;
	unsigned short Tmp = DmaStruct->Dma_BlockLength;
	TempPtr->TRFCNTL = Tmp;
	TempPtr->TRFCNTH = Tmp>>8;
	TempPtr->REPCNT = DmaStruct->Dma_RepeatNr;
	unsigned long Tmp1 = DmaStruct->Dma_SrcAddr;
	TempPtr->SRCADDR0 = Tmp1;
	TempPtr->SRCADDR1 = Tmp1>>8;
	TempPtr->SRCADDR2 = Tmp1>>16;
	Tmp1 = DmaStruct->Dma_DestAddr;
	TempPtr->DESTADDR0 = Tmp1;
	TempPtr->DESTADDR1 = Tmp1>>8;
	TempPtr->DESTADDR2 = Tmp1>>16;
	TempPtr->CTRLA |= DMA_CH_ENABLE_bm;
}
//#####################################################
void dma_ch_dma_enable(Dma_t *DmaStruct)
{
	DMA_CH_t *ModuleAddress = DmaStruct->Dma_BaseAddr;
	ModuleAddress->CTRLA |= DMA_CH_ENABLE_bm;
}
//#####################################################
void dma_src_addr_set(Dma_t *DmaStruct, unsigned long Addr)
{
	DMA_CH_t *ModuleAddress = DmaStruct->Dma_BaseAddr;
	ModuleAddress->SRCADDR0 = Addr;
	ModuleAddress->SRCADDR1 = Addr >> 8;
	ModuleAddress->SRCADDR2 = Addr >> 16;
}
//#####################################################
void dma_dest_addr_set(Dma_t *DmaStruct, unsigned long Addr)
{
	DMA_CH_t *ModuleAddress = DmaStruct->Dma_BaseAddr;
	ModuleAddress->DESTADDR0 = Addr;
	ModuleAddress->DESTADDR1 = Addr >> 8;
	ModuleAddress->DESTADDR2 = Addr >> 16;
}
//#####################################################
void dma_ch_repeat_enable(Dma_t *DmaStruct)
{
	DMA_CH_t *ModuleAddress = DmaStruct->Dma_BaseAddr;
	ModuleAddress->CTRLA |= DMA_CH_REPEAT_bm;
}
//#####################################################
void dma_repeat_nr_set(Dma_t *DmaStruct, unsigned char RepeatCnt)
{
	DMA_CH_t *ModuleAddress = DmaStruct->Dma_BaseAddr;
	ModuleAddress->REPCNT = RepeatCnt;
}
//#####################################################
void dma_blk_len_set(Dma_t *DmaStruct, unsigned short BlkLen)
{
	DMA_CH_t *ModuleAddress = DmaStruct->Dma_BaseAddr;
	ModuleAddress->TRFCNTL = BlkLen;
	ModuleAddress->TRFCNTH = BlkLen >> 8;
}
//#####################################################
unsigned char dma_ch_busy(Dma_t *DmaStruct)
{
	DMA_CH_t *ModuleAddress = DmaStruct->Dma_BaseAddr;
	return ModuleAddress->CTRLB & DMA_CH_CHBUSY_bm;
}
//#####################################################
//#####################################################
//#####################################################
//#####################################################
void dma_transfer_request(Dma_t *DmaStruct)
{
	DmaStruct->Dma_Status &= ~_Dma_Transfer_Complete_bm;
	DMA_CH_t *ModuleAddress = DmaStruct->Dma_BaseAddr;
	ModuleAddress->CTRLA |= DMA_CH_ENABLE_bm;
	ModuleAddress->CTRLA |= DMA_CH_TRFREQ_bm;
}
//#####################################################
void dma_complete_int(Dma_t *DmaStruct)
{
	DMA_CH_t *ModuleAddress = DmaStruct->Dma_BaseAddr;
	ModuleAddress->CTRLA |= DMA_RESET_bm;
	DmaStruct->Dma_Status |= _Dma_Transfer_Complete_bm;
}
//#####################################################
ISR(DMA_CH0_vect)
{
	//dma_complete_int(Dma_ServiceAddresses.DmaCh0RamServiceAddr);
	Dma_t *DmaChStruct = Dma_ServiceAddresses.DmaCh0RamServiceAddr;
	if(DmaChStruct->CallBackTransferEnd) DmaChStruct->CallBackTransferEnd(DmaChStruct->CallBackTransferEndData);
}
ISR(DMA_CH1_vect)
{
	//dma_complete_int(Dma_ServiceAddresses.DmaCh1RamServiceAddr);
	Dma_t *DmaChStruct = Dma_ServiceAddresses.DmaCh1RamServiceAddr;
	if(DmaChStruct->CallBackTransferEnd) DmaChStruct->CallBackTransferEnd(DmaChStruct->CallBackTransferEndData);
}
ISR(DMA_CH2_vect)
{
	//dma_complete_int(Dma_ServiceAddresses.DmaCh2RamServiceAddr);
	Dma_t *DmaChStruct = Dma_ServiceAddresses.DmaCh2RamServiceAddr;
	if(DmaChStruct->CallBackTransferEnd) DmaChStruct->CallBackTransferEnd(DmaChStruct->CallBackTransferEndData);
}
ISR(DMA_CH3_vect)
{
	//dma_complete_int(Dma_ServiceAddresses.DmaCh3RamServiceAddr);
	Dma_t *DmaChStruct = Dma_ServiceAddresses.DmaCh3RamServiceAddr;
	if(DmaChStruct->CallBackTransferEnd) DmaChStruct->CallBackTransferEnd(DmaChStruct->CallBackTransferEndData);
}
