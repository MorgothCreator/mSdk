//#####################################################
#ifndef ADC_H_
#define ADC_H_
//#####################################################
#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <stdbool.h>
#include "general/unions.h"
//#####################################################
/*Definitions*/
#define _Adc_Dma_Off						(0)
#define _Adc_Dma_Ch01						(1)
#define _Adc_Dma_Ch012						(2)
#define _Adc_Dma_Ch0123						(3)
//-----------------------------------------------------
#define _Adc_Start_Ch0						(1<<0)
#define _Adc_Start_Ch1						(1<<1)
#define _Adc_Start_Ch2						(1<<2)
#define _Adc_Start_Ch3						(1<<3)
//-----------------------------------------------------
#define _Adc_Flush_False					(0)
#define _Adc_Flush_True						(1)
//-----------------------------------------------------
#define _Adc_Enable_False					(0)
#define _Adc_Enable_True					(1)
//-------------------------------------------------------
#define _Adc_ConvMode_Unsigned				(0)
#define _Adc_ConvMode_Signed				(1)
//-----------------------------------------------------
#define _Adc_FreeRun_False					(0)
#define _Adc_FreeRun_True					(1)
//-----------------------------------------------------
#define _Adc_ConvRes_12RightAdj				(0)
#define _Adc_ConvRes_8RightAdj				(2)
#define _Adc_ConvRes_12LeftAdj				(3)
//-------------------------------------------------------
#define _Adc_RefSel_Int1V					(0)
#define _Adc_RefSel_IntVcc					(1)
#define _Adc_RefSel_ExtA					(2)
#define _Adc_RefSel_ExtB					(3)
//-----------------------------------------------------
#define _Adc_BandgapEn_False				(0)
#define _Adc_BandgapEn_True					(1)
//-----------------------------------------------------
#define _Adc_TemperatureEn_False			(0)
#define _Adc_TemperatureEn_True				(1)
//-----------------------------------------------------
#define _Adc_ChSweep_0						(0)
#define _Adc_ChSweep_01						(1)
#define _Adc_ChSweep_012					(2)
#define _Adc_ChSweep_0123					(3)
//-----------------------------------------------------
#define _Adc_EventLine_0123					(0)
#define _Adc_EventLine_1234					(1)
#define _Adc_EventLine_2345					(2)
#define _Adc_EventLine_3456					(3)
#define _Adc_EventLine_4567					(4)
#define _Adc_EventLine_567					(5)
#define _Adc_EventLine_67					(6)
#define _Adc_EventLine_7					(7)
//-----------------------------------------------------
#define _Adc_EventMode_None					(0)
#define _Adc_EventMode_Ch0					(1)
#define _Adc_EventMode_Ch01					(2)
#define _Adc_EventMode_Ch012				(3)
#define _Adc_EventMode_Ch0123				(4)
#define _Adc_EventMode_ChSweep				(5)
#define _Adc_EventMode_ChSyncSweep			(6)
//-----------------------------------------------------
#define _Adc_Prescaller_4					(0)
#define _Adc_Prescaller_8					(1)
#define _Adc_Prescaller_16					(2)
#define _Adc_Prescaller_32					(3)
#define _Adc_Prescaller_64					(4)
#define _Adc_Prescaller_128					(5)
#define _Adc_Prescaller_256					(6)
#define _Adc_Prescaller_512					(7)
//-----------------------------------------------------
#define _Adc_ChStart_False					(0)
#define _Adc_ChStart_True					(1)
//-----------------------------------------------------
#define _Adc_ChGain_1						(0)
#define _Adc_ChGain_2						(1)
#define _Adc_ChGain_4						(2)
#define _Adc_ChGain_8						(3)
#define _Adc_ChGain_16						(4)
#define _Adc_ChGain_32						(5)
#define _Adc_ChGain_64						(6)
//-----------------------------------------------------
#define _Adc_ChInMode_Internal				(0)
#define _Adc_ChInMode_SingleEnded			(1)
#define _Adc_ChInMode_Diff					(2)
#define _Adc_ChInMode_DiffWGain				(3)
//-----------------------------------------------------
#define _Adc_ChInSourcePos_Temp				(0)
#define _Adc_ChInSourcePos_Bandgap			(1)
#define _Adc_ChInSourcePos_ScaledVcc		(2)
#define _Adc_ChInSourcePos_Dac				(3)
//-------------------------
#define _Adc_ChInSrcPos_0					(0)
#define _Adc_ChInSrcPos_1					(1)
#define _Adc_ChInSrcPos_2					(2)
#define _Adc_ChInSrcPos_3					(3)
#define _Adc_ChInSrcPos_4					(4)
#define _Adc_ChInSrcPos_5					(5)
#define _Adc_ChInSrcPos_6					(6)
#define _Adc_ChInSrcPos_7					(7)
//-----------------------------------------------------
#define _Adc_ChInSrcNeg_0					(0)
#define _Adc_ChInSrcNeg_1					(1)
#define _Adc_ChInSrcNeg_2					(2)
#define _Adc_ChInSrcNeg_3					(3)
#define _Adc_ChInSrcNeg_4					(0)
#define _Adc_ChInSrcNeg_5					(1)
#define _Adc_ChInSrcNeg_6					(2)
#define _Adc_ChInSrcNeg_7					(3)
//-----------------------------------------------------
#define _Adc_ChIntMode_Complete				(0)
#define _Adc_ChIntMode_Below				(1)
#define _Adc_ChIntMode_Above				(3)
//-----------------------------------------------------
#define _Adc_ChIntLevel_Disabled			(0)
#define _Adc_ChIntLevel_Low					(1)
#define _Adc_ChIntLevel_Med					(2)
#define _Adc_ChIntLevel_High				(3)

#ifdef ADC_CH_GAIN_gm
#define ADC_CH_GAINFAC_gm	ADC_CH_GAIN_gm
#endif

#ifdef ADC_CH_GAIN_gp
#define ADC_CH_GAINFAC_gp	ADC_CH_GAIN_gp
#endif
//#####################################################
typedef struct
{
	ADC_CH_t* Adc_ChBaseAddr;
	unsigned char Adc_ChGain;
	unsigned char Adc_ChInMode;
	unsigned char Adc_ChInSrcPos;
	unsigned char Adc_ChInSrcNeg;
	unsigned char Adc_ChIntMode;
	unsigned char Adc_ChIntLevel;
	void* CallbackData;
	void(*ChannelCallback)(void*);
}Adc_Ch_t;
//-----------------------------------------------------
typedef struct
{
	ADC_t* Adc_GenBaseAddr;
	Adc_Ch_t* Adc_GenStructAddrCh0;
	Adc_Ch_t* Adc_GenStructAddrCh1;
	Adc_Ch_t* Adc_GenStructAddrCh2;
	Adc_Ch_t* Adc_GenStructAddrCh3;
	uint16_t Adc_GenCompVal;
	unsigned char Adc_GenDmaSel;
	unsigned char Adc_GenConvMode;
	unsigned char Adc_GenConvRes;
	unsigned char Adc_GenFreeRun;
	unsigned char Adc_GenRefSel;
	unsigned char Adc_GenBandgapEn;
	unsigned char Adc_GenTempEn;
	unsigned char Adc_GenChSweep;
	unsigned char Adc_GenEventLine;
	unsigned char Adc_GenEventMode;
	unsigned char Adc_GenPrescaller;
}Adc_t;
//#####################################################
typedef struct
{
	Adc_t* AdcAGeneralRamServiceAddr;
	Adc_t* AdcBGeneralRamServiceAddr;
} AdcServiceAddresses;
//#####################################################
#define new_adc Adc_t
#ifndef new_
#define new_(structure) (structure*)calloc(1,sizeof(structure));
#endif
#define free_adc(address) free(address);
//#####################################################
#define new_adc_ch Adc_Ch_t
#ifndef new_
#define new_(structure) (structure*)calloc(1,sizeof(structure));
#endif
#define free_adc_ch(address) free(address);
//#####################################################
bool adc_gen_en(Adc_t *AdcGenStruct, unsigned char GenUnit, bool Action);
bool adc_ch_reg(Adc_t *AdcGenStruct,Adc_Ch_t *AdcChStruct,unsigned char ChNr);
Adc_Ch_t* adc_get_ch_struct_addr(Adc_t *AdcGenStruct,unsigned char ChNr);
void adc_ch_start(Adc_t *AdcGenStruct,unsigned char ChNr);
ADC_CH_t* adc_base_addr_read(Adc_t* AdcGenStruct,unsigned char ChNr);
unsigned short adc_get_value(Adc_t *AdcGenStruct, unsigned char ChNr, bool WaitConversion);
//#####################################################
#ifdef HEADER_INCLUDE_C_FILES
#include "adc.c"
#endif
//#####################################################
#endif