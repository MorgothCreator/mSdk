//#######################################################################################
// AC Driver
//
// File Name            :"IO_AC.c"
// Location             :"Drivers/IO_AC.c"
// Title                :AC IO Driver
// Date                 :10.07.2011
// Version              :1.0
// Target MCU           :All ATXmega
// AUTHOR				:Iulian Gheorghiu
//			Romania
//			morgoth.creator@gmail.com
//			http://digitalelectronicsandprograming.blogspot.com/
//			http://morgothatxmegaprograming.blogspot.com/
//			http://devboardshop.servehttp.com
//
// DESCRIPTION:
//  This is a Hardware analog comparator driver
//	
//
//#######################################################################################
//#######################################################################################
//#######################################################################################
//#include "Driver/IO_AC.h"
#include <avr/io.h>
#include "ac.h"
//#####################################################
AcServiceAddresses Ac_ServiceAddresses;
//#####################################################
static AC_t *Ac_GetSetBaseAddress(Ac_t *AcStruct,unsigned char AcModule)
{
	AC_t *Temp = 0;
	switch(AcModule)
	{
		case(0):
			Ac_ServiceAddresses.Ac0RamServiceAddr = AcStruct;
			Temp = &ACA;
			break; 
#ifdef ACB
		case(1):
			Ac_ServiceAddresses.Ac1RamServiceAddr = AcStruct;
			Temp = &ACB;
			break; 
#endif
	}
	AcStruct->Ac_BaseAddr = (void*)Temp;
	return Temp;
}
//#####################################################
void ac_init(Ac_t *AcStruct)
{
	AC_t *ModuleAddress = Ac_GetSetBaseAddress(AcStruct, AcStruct->Ac_ModuleNr);
	unsigned char Temp = ModuleAddress->AC0CTRL;
	Temp = (Temp&(~AC_INTMODE_gm))|(AcStruct->Ac_Ch0IntMode<<AC_INTMODE_gp);
	Temp = (Temp&(~AC_INTLVL_gm))|(AcStruct->Ac_Ch0IntLevel<<AC_INTLVL_gp);
	Temp = (Temp&(~AC_HSMODE_bm))|(AcStruct->Ac_Ch0HighSpeedEn<<AC_HSMODE_bp);
	Temp = (Temp&(~AC_HYSMODE_gm))|(AcStruct->Ac_Ch0HisterezisSel<<AC_HYSMODE_gp);
	Temp = (Temp&(~AC_ENABLE_bm))|(AcStruct->Ac_Ch0En<<AC_ENABLE_bp);
	ModuleAddress->AC0CTRL = Temp;
	Temp = ModuleAddress->AC1CTRL;
	Temp = (Temp&(~AC_INTMODE_gm))|(AcStruct->Ac_Ch1IntMode<<AC_INTMODE_gp);
	Temp = (Temp&(~AC_INTLVL_gm))|(AcStruct->Ac_Ch1IntLevel<<AC_INTLVL_gp);
	Temp = (Temp&(~AC_HSMODE_bm))|(AcStruct->Ac_Ch1HighSpeedEn<<AC_HSMODE_bp);
	Temp = (Temp&(~AC_HYSMODE_gm))|(AcStruct->Ac_Ch1HisterezisSel<<AC_HYSMODE_gp);
	Temp = (Temp&(~AC_ENABLE_bm))|(AcStruct->Ac_Ch1En<<AC_ENABLE_bp);
	ModuleAddress->AC1CTRL = Temp;
	Temp = ModuleAddress->AC0MUXCTRL;
	Temp = (Temp&(~AC_MUXPOS_gm))|(AcStruct->Ac_Ch0PosMux<<AC_MUXPOS_gp);
	Temp = (Temp&(~AC_MUXNEG_gm))|(AcStruct->Ac_Ch0NegMux<<AC_MUXNEG_gp);
	ModuleAddress->AC0MUXCTRL = Temp;
	Temp = ModuleAddress->AC1MUXCTRL;
	Temp = (Temp&(~AC_MUXPOS_gm))|(AcStruct->Ac_Ch1PosMux<<AC_MUXPOS_gp);
	Temp = (Temp&(~AC_MUXNEG_gm))|(AcStruct->Ac_Ch1NegMux<<AC_MUXNEG_gp);
	ModuleAddress->AC1MUXCTRL = Temp;
	ModuleAddress->CTRLA = (ModuleAddress->CTRLA & (~AC_AC0OUT_bm))  | (AcStruct->Ac_Ac0OutEn << AC_AC0OUT_bp);
	ModuleAddress->CTRLB = (ModuleAddress->CTRLB & (~AC_SCALEFAC_gm)) | (AcStruct->Ac_ScaleFactor<<AC_SCALEFAC_gp);
	Temp = ModuleAddress->WINCTRL;
	Temp = (Temp&(~AC_WEN_bm))|(AcStruct->Ac_WindowEn<<AC_WEN_bp);
	Temp = (Temp&(~AC_WINTMODE_gm))|(AcStruct->Ac_WindowIntMode<<AC_WINTMODE_gp);
	Temp = (Temp&(~AC_WINTLVL_gm))|(AcStruct->Ac_WindowIntLevel<<AC_WINTLVL_gp);
	ModuleAddress->WINCTRL = Temp;
}
//#####################################################
//#####################################################
#ifdef use_ac0_ch0_int
ISR(ACA_AC0_vect)
{
	Ac_t *StructAddr = Ac_ServiceAddresses.Ac0RamServiceAddr;
	StructAddr->CallBackCh0(StructAddr->CallBackCh0Data);
}
#endif

#ifdef use_ac0_ch1_int
ISR(ACA_AC1_vect)
{
	Ac_t *StructAddr = Ac_ServiceAddresses.Ac0RamServiceAddr;
	StructAddr->CallBackCh1(StructAddr->CallBackCh1Data);
}
#endif

#ifdef ACB
#ifdef use_ac1_ch0_int
ISR(ACB_AC0_vect)
{
	Ac_t *StructAddr = Ac_ServiceAddresses.Ac1RamServiceAddr;
	StructAddr->CallBackCh0(StructAddr->CallBackCh0Data);
}
#endif

#ifdef use_ac1_ch1_int
ISR(ACB_AC1_vect)
{
	Ac_t *StructAddr = Ac_ServiceAddresses.Ac1RamServiceAddr;
	StructAddr->CallBackCh1(StructAddr->CallBackCh1Data);
}
#endif
#endif


