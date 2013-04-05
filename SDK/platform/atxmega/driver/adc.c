//#######################################################################################
// ADC Driver
//
// File Name            :"IO_Adc.c"
// Location             :"Drivers/IO_Adc.c"
// Title                :Internal ADC Driver
// Date                 :15.07.2011
// Version              :1.0
// Target MCU           :All ATXmega
// AUTHOR				:Gheorghiu Iulian
//			Romania
//			morgoth.creator@gmail.com
//			http://digitalelectronicsandprograming.blogspot.com/
//			http://morgothatxmegaprograming.blogspot.com/
//			http://devboardshop.servehttp.com
//
// DESCRIPTION:
//  This is a internal ADC driver
//	
//
//#######################################################################################
//						Example of how to initiate Adc module
//
//#######################################################################################
/*//	Initiate structure for selected Adc unit A or B = 0 or 1
	Adc_Gen_t AdcA_Gen_Struct;
	AdcA_Gen_Struct.Adc_GenCompVal						= 0;//set compare value to generate a compare interrupt if is case
	AdcA_Gen_Struct.Adc_GenDmaSel						= _Adc_Dma_Off;//Set DMA mode
	AdcA_Gen_Struct.Adc_GenConvMode						= _Adc_ConvMode_Unsigned;//Set conversion mode
	AdcA_Gen_Struct.Adc_GenConvRes						= _Adc_ConvRes_8RightAdj;//Set conversion resolution
	AdcA_Gen_Struct.Adc_GenFreeRun						= True;//Set if chanel work in free run mode, if is "True" will not be neccessary to call "adc_ch_start" routine
	AdcA_Gen_Struct.Adc_GenRefSel						= _Adc_RefSel_Int1V;//Set Reference voltage
	AdcA_Gen_Struct.Adc_GenBandgapEn					= _Adc_BandgapEn_True;//Set if bandgap is enabled or disabled
	AdcA_Gen_Struct.Adc_GenTempEn						= False;//Set if internal temperature sensor is enabled or disabled
	AdcA_Gen_Struct.Adc_GenChSweep						= _Adc_ChSweep_0123;//Set SWEEP mode
	AdcA_Gen_Struct.Adc_GenEventLine					= _Adc_EventLine_0123;//Set event line
	AdcA_Gen_Struct.Adc_GenEventMode					= _Adc_EventMode_None;//Set event mode
	AdcA_Gen_Struct.Adc_GenPrescaller					= _Adc_Prescaller_16;//Set prescaller, frequency run of the selected Adc unit
	//Before to call next function is absolutely necessarily to set the up structure of selected ADC unit
	adc_gen_en(&AdcA_Gen_Struct,True);*/
//#######################################################################################
//						Example of how to initiate one Adc channel
//
//#######################################################################################
/*	Adc_t AdcA_Ch0_Struct;
	AdcA_Ch0_Struct.Adc_ChGain							= _Adc_ChGain_16;//Set selected chanel gain
	AdcA_Ch0_Struct.Adc_ChInMode						= _Adc_ChInMode_DiffWGain;//Set selected chanel input mode
	AdcA_Ch0_Struct.Adc_ChInSrcNeg						= _Adc_ChInSourceNeg_4;//Set pin number as negative source of signal, only if is set in differential mode
	AdcA_Ch0_Struct.Adc_ChInSrcPos						= _Adc_ChInSourcePos_0;////Set pin number as positive source of signal
	AdcA_Ch0_Struct.Adc_ChIntLevel						= _Adc_ChIntLevel_Disabled;//Set selected chanel interupt level
	AdcA_Ch0_Struct.Adc_ChIntMode						= _Adc_ChIntMode_Complete;//Set selected chanel interupt mode
	before to call next function is absolutely necessarily to set the up structure of selected chanel
	adc_ch_reg(&AdcA_Gen_Struct,&AdcA_Ch0_Struct,0); '0' is channel number*/
//#######################################################################################
//#######################################################################################
//#######################################################################################
#include <avr/io.h>
#include <stdbool.h>
#include <stdio.h>
#include "adc.h"
//#####################################################
AdcServiceAddresses Adc_ServiceAddresses;
//#####################################################
ADC_t* Adc_GetSetGenBaseAddress(Adc_t *AdcGenStruct,unsigned char AdcNumber)
{
	ADC_t *Temp = NULL;
	switch(AdcNumber)
	{
		case(0):
			Temp = (ADC_t *)&ADCA;
			Adc_ServiceAddresses.AdcAGeneralRamServiceAddr = (void*)AdcGenStruct;
			break;
#ifdef ADCB
		case(1):
			Temp = (ADC_t *)&ADCB;
			Adc_ServiceAddresses.AdcBGeneralRamServiceAddr = (void*)AdcGenStruct;
			break;
#endif
		default:
			return NULL;
	}
	AdcGenStruct->Adc_GenBaseAddr = Temp;
	return Temp;
}
//#####################################################
bool adc_gen_en(Adc_t *AdcGenStruct, unsigned char GenUnit, bool enable)
{
	if(!AdcGenStruct) return false;
	if(enable == true)
	{
		ADC_t *ModuleAddress = Adc_GetSetGenBaseAddress(AdcGenStruct,GenUnit);
		//volatile (NVM)NVM_PROD_SIGNATURES_t ProductionSignatures;
		/*unsigned char Temp0 = 0;
		unsigned char Temp1 = 0;
		switch(AdcUnit)
		{
			case(0):
				Temp0 = NVM_PROD_SIGNATURES_t.ADCACAL0;
				Temp1 = NVM_PROD_SIGNATURES_t.ADCACAL1;
				break;
			case(1):
				Temp0 = NVM_PROD_SIGNATURES_t.ADCBCAL0;
				Temp1 = NVM_PROD_SIGNATURES_t.ADCBCAL1;
				break;
		}			
		ADC_t.CALL = Temp0;
		ADC_t.CALH = Temp1;*/
		convert16to8 CompareValue;
		CompareValue.i16 = AdcGenStruct->Adc_GenCompVal;
		ModuleAddress->CMPL = CompareValue.Byte0;
		ModuleAddress->CMPH = CompareValue.Byte1;
//-----------------------------------------------------
		unsigned char Temp = ModuleAddress->CTRLB;//ADC_CTRLB_offset
		Temp = (Temp&(~ADC_CONMODE_bm))|(AdcGenStruct->Adc_GenConvMode<<ADC_CONMODE_bp);
		Temp = (Temp&(~ADC_FREERUN_bm))|(AdcGenStruct->Adc_GenFreeRun<<ADC_FREERUN_bp);
		Temp = (Temp&(~ADC_RESOLUTION_gm))|(AdcGenStruct->Adc_GenConvRes<<ADC_RESOLUTION_gp);
		ModuleAddress->CTRLB = Temp;
//-----------------------------------------------------
		Temp = ModuleAddress->REFCTRL;//ADC_REFCTRL_offset
		Temp = (Temp&(~ADC_REFSEL_gm))|(AdcGenStruct->Adc_GenRefSel<<ADC_REFSEL_gp);
		Temp = (Temp&(~ADC_BANDGAP_bm))|(AdcGenStruct->Adc_GenBandgapEn<<ADC_BANDGAP_bp);
		Temp = (Temp&(~ADC_TEMPREF_bm))|(AdcGenStruct->Adc_GenTempEn<<ADC_TEMPREF_bp);
		ModuleAddress->REFCTRL = Temp;
//-----------------------------------------------------
		Temp = ModuleAddress->EVCTRL;//ADC_EVCTRL_offset
		Temp = (Temp&(~ADC_SWEEP_gm))|(AdcGenStruct->Adc_GenChSweep<<ADC_SWEEP_gp);
		Temp = (Temp&(~ADC_EVSEL_gm))|(AdcGenStruct->Adc_GenEventLine<<ADC_EVSEL_gp);
		Temp = (Temp&(~ADC_EVACT_gm))|(AdcGenStruct->Adc_GenEventMode<<ADC_EVACT_gp);
		ModuleAddress->EVCTRL = Temp;
//-----------------------------------------------------
		Temp = ModuleAddress->PRESCALER;//ADC_PRESCALER_offset
		Temp = (Temp&(~ADC_PRESCALER_gm))|(AdcGenStruct->Adc_GenPrescaller<<ADC_PRESCALER_gp);
		ModuleAddress->PRESCALER = Temp;
//-----------------------------------------------------
		Temp = ModuleAddress->CTRLA;//ADC_CTRLA_offset
		Temp = (Temp&(~ADC_DMASEL_gm))|(AdcGenStruct->Adc_GenDmaSel<<ADC_DMASEL_gp);
		Temp = (Temp&(~ADC_ENABLE_bm))|(1<<ADC_ENABLE_bp);
		ModuleAddress->CTRLA = Temp;
	}	
	else
	{
		ADC_t *ModuleAddress = AdcGenStruct->Adc_GenBaseAddr;
		unsigned char Temp = ModuleAddress->CTRLA;//ADC_CTRLA_offset
		Temp = Temp&(~ADC_ENABLE_bm);
		ModuleAddress->CTRLA = Temp;
	}
	return true;
}
//#####################################################
ADC_CH_t* Adc_GetSetAndRegisterChBaseAddress(Adc_t *AdcGenStruct, Adc_Ch_t *AdcChStruct,unsigned char ChNr)
{
	ADC_CH_t *Temp = NULL;
	switch(ChNr)
	{
		case(0):
			Temp = (ADC_CH_t *)(0x0020 + (uint16_t)AdcGenStruct->Adc_GenBaseAddr);
			AdcChStruct->Adc_ChBaseAddr = Temp;
			AdcGenStruct->Adc_GenStructAddrCh0 = AdcChStruct;
			break;
		case(1):
			Temp = (ADC_CH_t *)(0x0028 + (uint16_t)AdcGenStruct->Adc_GenBaseAddr);
			AdcChStruct->Adc_ChBaseAddr = Temp;
			AdcGenStruct->Adc_GenStructAddrCh1 = AdcChStruct;
			break;
		case(2):
			Temp = (ADC_CH_t *)(0x0030 + (uint16_t)AdcGenStruct->Adc_GenBaseAddr);
			AdcChStruct->Adc_ChBaseAddr = Temp;
			AdcGenStruct->Adc_GenStructAddrCh2 = AdcChStruct;
			break;
		case(3):
			Temp = (ADC_CH_t *)(0x0038 + (uint16_t)AdcGenStruct->Adc_GenBaseAddr);
			AdcChStruct->Adc_ChBaseAddr = Temp;
			AdcGenStruct->Adc_GenStructAddrCh3 = AdcChStruct;
			break;
	}
	return Temp;
}
//#####################################################
bool adc_ch_reg(Adc_t *AdcGenStruct, Adc_Ch_t *AdcChStruct,unsigned char ChNr)
{
	if(!AdcGenStruct || !AdcChStruct) return false;
	asm("cli");
	unsigned char *ChannelAddress = (unsigned char*)Adc_GetSetAndRegisterChBaseAddress(AdcGenStruct, AdcChStruct, ChNr);
	unsigned char Temp = ChannelAddress[1];//ADC_CH_MUXCTRL_offset
	Temp = (Temp&(~ADC_CH_MUXPOS_gm))|(AdcChStruct->Adc_ChInSrcPos<<ADC_CH_MUXPOS_gp);
	Temp = (Temp&(~ADC_CH_MUXNEG_gm))|(AdcChStruct->Adc_ChInSrcNeg<<ADC_CH_MUXNEG_gp);
	ChannelAddress[1] = Temp;

	Temp = ChannelAddress[2];//ADC_CH_INTCTRL_offset
	Temp = (Temp&(~ADC_CH_INTMODE_gm))|(AdcChStruct->Adc_ChIntMode<<ADC_CH_INTMODE_gp);
	Temp = (Temp&(~ADC_CH_INTLVL_gm))|(AdcChStruct->Adc_ChIntLevel<<ADC_CH_INTLVL_gp);
	ChannelAddress[2] = Temp;

	Temp = ChannelAddress[0];//ADC_CH_CTRL_offset
	//Temp = (Temp&(255-ADC_CH_START_bm))|(AdcChStruct->Adc_ChStart<<ADC_CH_START_bp);
	Temp = (Temp&(~ADC_CH_GAINFAC_gm))|(AdcChStruct->Adc_ChGain<<ADC_CH_GAINFAC_gp);
	Temp = (Temp&(~ADC_CH_INPUTMODE_gm))|(AdcChStruct->Adc_ChInMode<<ADC_CH_INPUTMODE_gp);
	ChannelAddress[0] = Temp;
	asm("sei");
	return true;
}
//#####################################################
Adc_Ch_t* adc_get_ch_struct_addr(Adc_t *AdcGenStruct,unsigned char ChNr)
{
	Adc_Ch_t* ChStructAddress = NULL;
	switch(ChNr)
	{
		case(0):
			ChStructAddress = (Adc_Ch_t*)AdcGenStruct->Adc_GenStructAddrCh0;
			break;
		case(1):
			ChStructAddress = (Adc_Ch_t*)AdcGenStruct->Adc_GenStructAddrCh1;
			break;
		case(2):
			ChStructAddress = (Adc_Ch_t*)AdcGenStruct->Adc_GenStructAddrCh2;
			break;
		case(3):
			ChStructAddress = (Adc_Ch_t*)AdcGenStruct->Adc_GenStructAddrCh3;
			break;
	}		
	return ChStructAddress;
}	
//#####################################################
void adc_ch_start(Adc_t *AdcGenStruct,unsigned char ChNr)
{
	if(!AdcGenStruct) return;
	Adc_Ch_t* ChStructAddress = adc_get_ch_struct_addr(AdcGenStruct,ChNr);
	ADC_CH_t *ChannelAddress = ChStructAddress->Adc_ChBaseAddr;
	ChannelAddress->CTRL |= ADC_CH_START_bm;//ADC_CH_CTRL_offset(start conversion)
}
//#####################################################
ADC_CH_t* adc_base_addr_read(Adc_t* AdcGenStruct,unsigned char ChNr)
{
	unsigned int Tmp = (unsigned int)AdcGenStruct->Adc_GenBaseAddr;
	ADC_CH_t *Temp = 0;
	switch(ChNr)
	{
		case(0):
			Temp = (ADC_CH_t *)(0x0020 + Tmp);
			break;
		case(1):
			Temp = (ADC_CH_t *)(0x0028 + Tmp);
			break;
		case(2):
			Temp = (ADC_CH_t *)(0x0030 + Tmp);
			break;
		case(3):
			Temp = (ADC_CH_t *)(0x0038 + Tmp);
			break;
	}
	return Temp;
}		
//#####################################################
//#####################################################
unsigned short adc_get_value(Adc_t *AdcGenStruct, unsigned char ChNr, bool WaitConversion)
{
	if(!AdcGenStruct) return 0;
	//Adc_t* ChStructAddress = get_ch_struct_addr(AdcGenStruct,ChNr);
	//unsigned char* ChBaseAddress = ChStructAddress->Adc_ChBaseAddr;
	//Temp = (unsigned char*)adc_base_addr_read(AdcGenStruct,ChNr);
	ADC_CH_t* Temp = adc_base_addr_read(AdcGenStruct, ChNr);
	if(WaitConversion == true) 
	{
		while (Temp->INTFLAGS == 0);
	}
	convert8to16 AdcReturnValue;
	AdcReturnValue.Byte0 = Temp->RESL;
	AdcReturnValue.Byte1 = Temp->RESH;
	return AdcReturnValue.ShortReturn;
}
//#####################################################
//#####################################################
//#####################################################
//#####################################################
//############ Interrupt vectors insertion ############
//#####################################################
//#####################################################
//#####################################################
//#####################################################
//#####################################################
//#####################################################
//#####################################################
//#####################################################
//inline void usart_adca_ch0_int(Adc_Gen_t *AdcGenStruct)
//{
//	
//}
//#####################################################
//inline void usart_adca_ch1_int(Adc_Gen_t *AdcGenStruct)
//{
//	
//}
//#####################################################
//inline void usart_adca_ch2_int(Adc_Gen_t *AdcGenStruct)
//{
//	
//}
//#####################################################
//inline void usart_adca_ch3_int(Adc_Gen_t *AdcGenStruct)
//{
//	
//}
//#####################################################
//inline void usart_adcb_ch0_int(Adc_Gen_t *AdcGenStruct)
//{
//	
//}
//#####################################################
//inline void usart_adcb_ch1_int(Adc_Gen_t *AdcGenStruct)
//{
//	
//}
//#####################################################
//inline void usart_adcb_ch2_int(Adc_Gen_t *AdcGenStruct)
//{
//	
//}
//#####################################################
//inline void usart_adcb_ch3_int(Adc_Gen_t *AdcGenStruct)
//{
//	
//}
//#####################################################
#ifdef use_adca_ch0
		ISR(ADCA_CH0_vect)
		{
			if(!Adc_ServiceAddresses.AdcAGeneralRamServiceAddr) return;
			Adc_t* Struct = Adc_ServiceAddresses.AdcAGeneralRamServiceAddr;
			if(!Struct) return;
			Adc_Ch_t* ChannelStruct = Struct->Adc_GenStructAddrCh0;
			if(!ChannelStruct) return;
			ChannelStruct->ChannelCallback(ChannelStruct->CallbackData);
		}
#endif
//-----------------------------------------------------
#ifdef use_adca_ch1
		ISR(ADCA_CH1_vect)
		{
			if(!Adc_ServiceAddresses.AdcAGeneralRamServiceAddr) return;
			Adc_t* Struct = Adc_ServiceAddresses.AdcAGeneralRamServiceAddr;
			if(!Struct) return;
			Adc_Ch_t* ChannelStruct = Struct->Adc_GenStructAddrCh1;
			if(!ChannelStruct) return;
			ChannelStruct->ChannelCallback(ChannelStruct->CallbackData);
		}
#endif
//-----------------------------------------------------
#ifdef use_adca_ch2
		ISR(ADCA_CH2_vect)
		{
			if(!Adc_ServiceAddresses.AdcAGeneralRamServiceAddr) return;
			Adc_t* Struct = Adc_ServiceAddresses.AdcAGeneralRamServiceAddr;
			if(!Struct) return;
			Adc_Ch_t* ChannelStruct = Struct->Adc_GenStructAddrCh2;
			if(!ChannelStruct) return;
			ChannelStruct->ChannelCallback(ChannelStruct->CallbackData);
		}
#endif
//-----------------------------------------------------
#ifdef use_adca_ch3
		ISR(ADCA_CH3_vect)
		{
			if(!Adc_ServiceAddresses.AdcAGeneralRamServiceAddr) return;
			Adc_t* Struct = Adc_ServiceAddresses.AdcAGeneralRamServiceAddr;
			if(!Struct) return;
			Adc_Ch_t* ChannelStruct = Struct->Adc_GenStructAddrCh3;
			if(!ChannelStruct) return;
			ChannelStruct->ChannelCallback(ChannelStruct->CallbackData);
		}
#endif
//-----------------------------------------------------
#ifdef use_adcb_ch0
		ISR(ADCB_CH0_vect)
		{
			if(!Adc_ServiceAddresses.AdcAGeneralRamServiceAddr) return;
			Adc_t* Struct = Adc_ServiceAddresses.AdcBGeneralRamServiceAddr;
			if(!Struct) return;
			Adc_Ch_t* ChannelStruct = Struct->Adc_GenStructAddrCh0;
			if(!ChannelStruct) return;
			ChannelStruct->ChannelCallback(ChannelStruct->CallbackData);
		}
#endif
//-----------------------------------------------------
#ifdef use_adcc_ch1
		ISR(ADCB_CH1_vect)
		{
			if(!Adc_ServiceAddresses.AdcAGeneralRamServiceAddr) return;
			Adc_t* Struct = Adc_ServiceAddresses.AdcBGeneralRamServiceAddr;
			if(!Struct) return;
			Adc_Ch_t* ChannelStruct = Struct->Adc_GenStructAddrCh1;
			if(!ChannelStruct) return;
			ChannelStruct->ChannelCallback(ChannelStruct->CallbackData);
		}
#endif
//-----------------------------------------------------
#ifdef use_adcd_ch2
		ISR(ADCB_CH2_vect)
		{
			if(!Adc_ServiceAddresses.AdcAGeneralRamServiceAddr) return;
			Adc_t* Struct = Adc_ServiceAddresses.AdcBGeneralRamServiceAddr;
			if(!Struct) return;
			Adc_Ch_t* ChannelStruct = Struct->Adc_GenStructAddrCh2;
			if(!ChannelStruct) return;
			ChannelStruct->ChannelCallback(ChannelStruct->CallbackData);
		}
#endif
//-----------------------------------------------------
#ifdef use_adce_ch3
		ISR(ADCB_CH3_vect)
		{
			if(!Adc_ServiceAddresses.AdcAGeneralRamServiceAddr) return;
			Adc_t* Struct = Adc_ServiceAddresses.AdcBGeneralRamServiceAddr;
			if(!Struct) return;
			Adc_Ch_t* ChannelStruct = Struct->Adc_GenStructAddrCh3;
			if(!ChannelStruct) return;
			ChannelStruct->ChannelCallback(ChannelStruct->CallbackData);
		}
#endif
//-----------------------------------------------------
