/*
 * timer.c
 *
 * Created: 2/15/2013 11:32:19 PM
 *  Author: XxXx
 */ 

#include <stdbool.h>
#include "timer.h"
#include "general/unions.h"
//#####################################################
TimerServiceAddresses Timer_ServiceAddresses;

#ifdef TC0_BYTEM_bp
#define TC0_BYTEM_gp	TC0_BYTEM_bp
#endif
#ifdef TC0_BYTEM_bm
#define TC0_BYTEM_gm	TC0_BYTEM_bm
#endif
//#####################################################
inline TC0_t* tc_get_set_base_address(Tc_t *TimerStruct,unsigned char TimerNr)
{
	TC0_t *Temp = NULL;
	switch(TimerNr)
	{
		case(0):
			Timer_ServiceAddresses.Timer0RamServiceAddr = TimerStruct;
			Temp = (TC0_t *)&TCC0;
			break; 
		case(1):
			Timer_ServiceAddresses.Timer1RamServiceAddr = TimerStruct;
			Temp = (TC0_t *)&TCC1;
			break; 
		case(2):
			Timer_ServiceAddresses.Timer2RamServiceAddr = TimerStruct;
			Temp = (TC0_t *)&TCD0;
			break; 
		case(3):
			Timer_ServiceAddresses.Timer3RamServiceAddr = TimerStruct;
			Temp = (TC0_t *)&TCD1;
			break; 
		case(4):
			Timer_ServiceAddresses.Timer4RamServiceAddr = TimerStruct;
			Temp = (TC0_t *)&TCE0;
			break; 
#ifdef TCE1
		case(5):
			Timer_ServiceAddresses.Timer5RamServiceAddr = TimerStruct;
			Temp = (TC0_t *)&TCE1;
			break; 
#endif
#ifdef TCF0
		case(6):
			Timer_ServiceAddresses.Timer6RamServiceAddr = TimerStruct;
			Temp = (TC0_t *)&TCF0;
			break; 
#endif
#ifdef TCF1
		case(7):
			Timer_ServiceAddresses.Timer7RamServiceAddr = TimerStruct;
			Temp = (TC0_t *)&TCF1;
			break; 
#endif
	}
	TimerStruct->Timer_BaseAddr = Temp;
	return Temp;
}
//#####################################################
void tc_init(Tc_t *TimerStruct, unsigned char TimerNr)
{
	TC0_t* TempPtr = tc_get_set_base_address(TimerStruct, TimerNr);
	TempPtr->CTRLA = (TempPtr->CTRLA&(255-TC0_CLKSEL_gm))|(TimerStruct->Timer_ClockSel<<TC0_CLKSEL_gp);//CTRLA
	
	unsigned char Temp;
	Temp = TempPtr->CTRLB;//CTRLB
	Temp = (Temp&(~TC0_CCDEN_bm))|(TimerStruct->Timer_CompCaptureEnD<<TC0_CCDEN_bp);
	Temp = (Temp&(~TC0_CCCEN_bm))|(TimerStruct->Timer_CompCaptureEnC<<TC0_CCCEN_bp);
	Temp = (Temp&(~TC0_CCBEN_bm))|(TimerStruct->Timer_CompCaptureEnB<<TC0_CCBEN_bp);
	Temp = (Temp&(~TC0_CCAEN_bm))|(TimerStruct->Timer_CompCaptureEnA<<TC0_CCAEN_bp);
	Temp = (Temp&(~TC0_WGMODE_gm))|(TimerStruct->Timer_WaveformGenMode<<TC0_WGMODE_gp);
	TempPtr->CTRLB = Temp;
	
	Temp = TempPtr->CTRLC;//CTRLC
	Temp = (Temp&(~TC0_CMPD_bm))|(TimerStruct->Timer_CompOutValueD<<TC0_CMPD_bp);
	Temp = (Temp&(~TC0_CMPC_bm))|(TimerStruct->Timer_CompOutValueC<<TC0_CMPC_bp);
	Temp = (Temp&(~TC0_CMPB_bm))|(TimerStruct->Timer_CompOutValueB<<TC0_CMPB_bp);
	Temp = (Temp&(~TC0_CMPA_bm))|(TimerStruct->Timer_CompOutValueA<<TC0_CMPA_bp);
	TempPtr->CTRLC = Temp;
	
	Temp = TempPtr->CTRLD;//CTRLD
	Temp = (Temp&(~TC0_EVACT_gm))|(TimerStruct->Timer_EvAction<<TC0_EVACT_gp);
	Temp = (Temp&(~TC0_EVDLY_bm))|(TimerStruct->Timer_EvDelay<<TC0_EVDLY_bp);
	Temp = (Temp&(~TC0_EVSEL_gm))|(TimerStruct->Timer_EvSourceSelect<<TC0_EVSEL_gp);
	TempPtr->CTRLD = Temp;
	
	TempPtr->CTRLE = (TempPtr->CTRLE&(~TC0_BYTEM_gm))|(TimerStruct->Timer_ByteModeSel<<TC0_BYTEM_gp);//CTRLE
	
	Temp = TempPtr->INTCTRLA;//INTCTRLA
	Temp = (Temp&(~TC0_ERRINTLVL_gm))|(TimerStruct->Timer_ErrIntLevel<<TC0_ERRINTLVL_gp);
	Temp = (Temp&(~TC0_OVFINTLVL_gm))|(TimerStruct->Timer_OvfIntLevel<<TC0_OVFINTLVL_gp);
	TempPtr->INTCTRLA = Temp;

	Temp = TempPtr->INTCTRLB;//INTCTRLB
	Temp = (Temp&(~TC0_CCDINTLVL_gm))|(TimerStruct->Timer_CompCapIntLevelD<<TC0_CCDINTLVL_gp);
	Temp = (Temp&(~TC0_CCCINTLVL_gm))|(TimerStruct->Timer_CompCapIntLevelC<<TC0_CCCINTLVL_gp);
	Temp = (Temp&(~TC0_CCBINTLVL_gm))|(TimerStruct->Timer_CompCapIntLevelB<<TC0_CCBINTLVL_gp);
	Temp = (Temp&(~TC0_CCAINTLVL_gm))|(TimerStruct->Timer_CompCapIntLevelA<<TC0_CCAINTLVL_gp);
	TempPtr->INTCTRLB = Temp;

	Temp = TempPtr->CTRLFCLR;//CTRLFCLR
	Temp = (Temp&(~TC0_CMD_gm))|(((~TimerStruct->Timer_Com)&TC0_CMD_gm)<<TC0_CMD_gp);
	Temp = (Temp&(~TC0_LUPD_bm))|(((~TimerStruct->Timer_LockUpdate)&TC0_LUPD_bm)<<TC0_LUPD_bp);
	Temp = (Temp&(~TC0_DIR_bm))|(((~TimerStruct->Timer_Dir)&TC0_DIR_bm)<<TC0_DIR_bp);
	TempPtr->CTRLFCLR = Temp;

	Temp = TempPtr->CTRLFSET;//CTRLFSET
	Temp = (Temp&(~TC0_CMD_gm))|(TimerStruct->Timer_Com<<TC0_CMD_gp);
	Temp = (Temp&(~TC0_LUPD_bm))|(TimerStruct->Timer_LockUpdate<<TC0_LUPD_bp);
	Temp = (Temp&(~TC0_DIR_bm))|(TimerStruct->Timer_Dir<<TC0_DIR_bp);
	TempPtr->CTRLFSET = Temp;

	/*Temp = TempPtr[10];//CTRLGCLR
	Temp = (Temp&(255-TC0_CCDBV_bm))|(TimerStruct->Timer_CompCapBuffValidD<<TC0_CCDBV_bp);
	Temp = (Temp&(255-TC0_CCCBV_bm))|(TimerStruct->Timer_CompCapBuffValidC<<TC0_CCCBV_bp);
	Temp = (Temp&(255-TC0_CCBBV_bm))|(TimerStruct->Timer_CompCapBuffValidB<<TC0_CCBBV_bp);
	Temp = (Temp&(255-TC0_CCABV_bm))|(TimerStruct->Timer_CompCapBuffValidA<<TC0_CCABV_bp);
	Temp = (Temp&(255-TC0_PERBV_bm))|(TimerStruct->Timer_PerBuffValid<<TC0_PERBV_bp);
	TempPtr[10] = Temp;

	Temp = TempPtr[11];//CTRLGSET
	Temp = (Temp&(255-TC0_CCDBV_bm))|(TimerStruct->Timer_CompCapBuffValidD<<TC0_CCDBV_bp);
	Temp = (Temp&(255-TC0_CCCBV_bm))|(TimerStruct->Timer_CompCapBuffValidC<<TC0_CCCBV_bp);
	Temp = (Temp&(255-TC0_CCBBV_bm))|(TimerStruct->Timer_CompCapBuffValidB<<TC0_CCBBV_bp);
	Temp = (Temp&(255-TC0_CCABV_bm))|(TimerStruct->Timer_CompCapBuffValidA<<TC0_CCABV_bp);
	Temp = (Temp&(255-TC0_PERBV_bm))|(TimerStruct->Timer_PerBuffValid<<TC0_PERBV_bp);
	TempPtr[11] = Temp;*/
	
	convert16to8 ByteToWrite_;
	ByteToWrite_.i16 = TimerStruct->Timer_PeriodUpdate;
	TempPtr->PERL = ByteToWrite_.Byte0;
	TempPtr->PERH = ByteToWrite_.Byte1;

	ByteToWrite_.i16 = TimerStruct->Timer_CompCapUpdateA;
	TempPtr->CCAL = ByteToWrite_.Byte0;
	TempPtr->CCAH = ByteToWrite_.Byte1;

	ByteToWrite_.i16 = TimerStruct->Timer_CompCapUpdateB;
	TempPtr->CCBL = ByteToWrite_.Byte0;
	TempPtr->CCBH = ByteToWrite_.Byte1;

	ByteToWrite_.i16 = TimerStruct->Timer_CompCapUpdateC;
	TempPtr->CCCL = ByteToWrite_.Byte0;
	TempPtr->CCCH = ByteToWrite_.Byte1;

	ByteToWrite_.i16 = TimerStruct->Timer_CompCapUpdateD;
	TempPtr->CCDL = ByteToWrite_.Byte0;
	TempPtr->CCDH = ByteToWrite_.Byte1;
}
//#####################################################
void tc_clk_sel(Tc_t *TimerStruct,unsigned char Value)
{
	TC0_t* TempPtr = TimerStruct->Timer_BaseAddr;
	TempPtr->CTRLA = (TempPtr->CTRLA&(~TC0_CLKSEL_gm))|(Value<<TC0_CLKSEL_gp);//CTRLA
}
//#####################################################
void tc_wave_gen_mede(Tc_t *TimerStruct,unsigned char Value)
{
	TC0_t* TempPtr = TimerStruct->Timer_BaseAddr;
	TempPtr->CTRLB = (TempPtr->CTRLB&(~TC0_WGMODE_gm))|(Value<<TC0_WGMODE_gp);//CTRLA
}
//#####################################################
void tc_event_action(Tc_t *TimerStruct,unsigned char Value)
{
	TC0_t* TempPtr = TimerStruct->Timer_BaseAddr;
	TempPtr->CTRLD = (TempPtr->CTRLD&(~TC0_EVACT_gm))|(Value<<TC0_EVACT_gp);//CTRLA
}
//#####################################################
void tc_event_delay(Tc_t *TimerStruct,unsigned char Value)
{
	TC0_t* TempPtr = TimerStruct->Timer_BaseAddr;
	TempPtr->CTRLD = (TempPtr->CTRLD&(~TC0_EVDLY_bm))|(Value<<TC0_EVDLY_bp);//CTRLA
}
//#####################################################
void tc_source_select(Tc_t *TimerStruct,unsigned char Value)
{
	TC0_t* TempPtr = TimerStruct->Timer_BaseAddr;
	TempPtr->CTRLD = (TempPtr->CTRLD&(~TC0_EVSEL_gm))|(Value<<TC0_EVSEL_gp);//CTRLA
}
//#####################################################
void tc_byte_mode(Tc_t *TimerStruct,unsigned char Action)
{
	TC0_t* TempPtr = TimerStruct->Timer_BaseAddr;
	TempPtr->CTRLE = (TempPtr->CTRLE&(~TC0_BYTEM_gm))|(Action<<TC0_BYTEM_gp);//CTRLA
}
//#####################################################
void tc_err_int_lvl(Tc_t *TimerStruct,unsigned char Value)
{
	TC0_t* TempPtr = TimerStruct->Timer_BaseAddr;
	TempPtr->INTCTRLA = (TempPtr->INTCTRLA&(~TC0_ERRINTLVL_gm))|(Value<<TC0_ERRINTLVL_gp);//CTRLA
}
//#####################################################
void tc_ovf_int_lvl(Tc_t *TimerStruct,unsigned char Value)
{
	TC0_t* TempPtr = TimerStruct->Timer_BaseAddr;
	TempPtr->INTCTRLA = (TempPtr->INTCTRLA&(~TC0_OVFINTLVL_gm))|(Value<<TC0_OVFINTLVL_gp);//CTRLA
}
//#####################################################
void tc_command_set(Tc_t *TimerStruct,unsigned char Value)
{
	TC0_t* TempPtr = TimerStruct->Timer_BaseAddr;
	TempPtr->CTRLFSET = (TempPtr->CTRLFSET&(~TC0_CMD_gm))|(Value<<TC0_CMD_gp);//CTRLA
}
//#####################################################
void tc_command_clr(Tc_t *TimerStruct,unsigned char Value)
{
	TC0_t* TempPtr = TimerStruct->Timer_BaseAddr;
	TempPtr->CTRLFCLR = (TempPtr->CTRLFCLR&(~TC0_CMD_gm))|(((~Value) & TC0_CMD_gm)<<TC0_CMD_gp);//CTRLA
}
//#####################################################
void tc_direction_set(Tc_t *TimerStruct,unsigned char Action)
{
	TC0_t* TempPtr = TimerStruct->Timer_BaseAddr;
	if(Action == _Timer_Increment) TempPtr->CTRLFCLR = TempPtr->CTRLFCLR|(1<<TC0_LUPD_bp);//CTRLA
	else TempPtr->CTRLFSET = TempPtr->CTRLFSET|(1<<TC0_LUPD_bp);//CTRLA
}
//#####################################################
void tc_lock_update(Tc_t *TimerStruct,unsigned char Action)
{
	TC0_t* TempPtr = TimerStruct->Timer_BaseAddr;
	if(Action == true) TempPtr->CTRLFSET = TempPtr->CTRLFSET|(1<<TC0_LUPD_bp);//CTRLA
	else  TempPtr->CTRLFCLR = TempPtr->CTRLFCLR|(1<<TC0_LUPD_bp);//CTRLA
}
//#####################################################
void tc_chanel_int_lvl(Tc_t *TimerStruct,unsigned char Chanel,unsigned char Action)
{
	TC0_t* TempPtr = TimerStruct->Timer_BaseAddr;
	unsigned char Temp = TempPtr->INTCTRLB;
	switch (Chanel)
	{
		case(0):
			Temp = (Temp&(~TC0_CCAINTLVL_gm))|(Action<<TC0_CCAINTLVL_gp);//CTRLA
			break;
		case(1):
			Temp = (Temp&(~TC0_CCBINTLVL_gm))|(Action<<TC0_CCBINTLVL_gp);//CTRLB
			break;
		case(2):
			Temp = (Temp&(~TC0_CCCINTLVL_gm))|(Action<<TC0_CCCINTLVL_gp);//CTRLC
			break;
		case(3):
			Temp = (Temp&(~TC0_CCDINTLVL_gm))|(Action<<TC0_CCDINTLVL_gp);//CTRLD
			break;
	}	
	TempPtr->INTCTRLB = Temp;
}
//#####################################################
void tc_comp_out_en(Tc_t *TimerStruct,unsigned char Chanel,unsigned char Action)
{
	TC0_t* TempPtr = TimerStruct->Timer_BaseAddr;
	unsigned char Temp = TempPtr->CTRLC;
	switch (Chanel)
	{
		case(0):
			Temp = (Temp&(~TC0_CMPA_bm))|(Action<<TC0_CMPA_bp);//CTRLB
			break;
		case(1):
			Temp = (Temp&(~TC0_CMPB_bm))|(Action<<TC0_CMPB_bp);//CTRLB
			break;
		case(2):
			Temp = (Temp&(~TC0_CMPC_bm))|(Action<<TC0_CMPC_bp);//CTRLB
			break;
		case(3):
			Temp = (Temp&(~TC0_CMPD_bm))|(Action<<TC0_CMPD_bp);//CTRLB
			break;
	}	
	TempPtr->CTRLC = Temp;
}
//#####################################################
void tc_comp_cap_en(Tc_t *TimerStruct,unsigned char Chanel,unsigned char Action)
{
	TC0_t* TempPtr = TimerStruct->Timer_BaseAddr;
	unsigned char Temp = TempPtr->CTRLB;
	switch (Chanel)
	{
		case(0):
			Temp = (Temp&(~TC0_CCAEN_bm))|(Action<<TC0_CCAEN_bp);//CTRLB
			break;
		case(1):
			Temp = (Temp&(~TC0_CCBEN_bm))|(Action<<TC0_CCBEN_bp);//CTRLB
			break;
		case(2):
			Temp = (Temp&(~TC0_CCCEN_bm))|(Action<<TC0_CCCEN_bp);//CTRLB
			break;
		case(3):
			Temp = (Temp&(~TC0_CCDEN_bm))|(Action<<TC0_CCDEN_bp);//CTRLB
			break;
	}	
	TempPtr->CTRLB = Temp;
}
//#####################################################
void tc_cnt_write(Tc_t *TimerStruct,unsigned short Value)
{
	TC0_t* TempPtr = TimerStruct->Timer_BaseAddr;
	convert16to8 ByteToWrite_;
	ByteToWrite_.i16 = Value;
	TempPtr->CNTL = ByteToWrite_.Byte0;
	TempPtr->CNTH = ByteToWrite_.Byte1;
}	
//#####################################################
void tc_per_write(Tc_t *TimerStruct,unsigned short Value)
{
	TC0_t* TempPtr = TimerStruct->Timer_BaseAddr;
	convert16to8 ByteToWrite_;
	ByteToWrite_.i16 = Value;
	TempPtr->PERL = ByteToWrite_.Byte0;
	TempPtr->PERH = ByteToWrite_.Byte1;
}	
//#####################################################
void tc_comp_cap_write(Tc_t *TimerStruct, unsigned char Chanel, unsigned short Value)
{
	TC0_t* TempPtr = TimerStruct->Timer_BaseAddr;
	convert16to8 ByteToWrite_;
	ByteToWrite_.i16 = Value;
	switch (Chanel)
	{
		case(0):
			TempPtr->CCAL = ByteToWrite_.Byte0;
			TempPtr->CCAH = ByteToWrite_.Byte1;
			break;
		case(1):
			TempPtr->CCBL = ByteToWrite_.Byte0;
			TempPtr->CCBH = ByteToWrite_.Byte1;
			break;
		case(2):
			TempPtr->CCCL = ByteToWrite_.Byte0;
			TempPtr->CCCH = ByteToWrite_.Byte1;
			break;
		case(3):
			TempPtr->CCDL = ByteToWrite_.Byte0;
			TempPtr->CCDH = ByteToWrite_.Byte1;
			break;
	}	
}
//#####################################################
void tc_per_buff_write(Tc_t *TimerStruct,unsigned short Value)
{
	TC0_t* TempPtr = TimerStruct->Timer_BaseAddr;
	convert16to8 ByteToWrite_;
	ByteToWrite_.i16 = Value;
	TempPtr->PERBUFL = ByteToWrite_.Byte0;
	TempPtr->PERBUFH = ByteToWrite_.Byte1;
}	
//#####################################################
void tc_comp_cap_buff_write(Tc_t *TimerStruct,unsigned char Chanel,unsigned short Value)
{
	TC0_t* TempPtr = TimerStruct->Timer_BaseAddr;
	convert16to8 ByteToWrite_;
	ByteToWrite_.i16 = Value;
	switch (Chanel)
	{
		case(0):
			TempPtr->CCABUFL = ByteToWrite_.Byte0;
			TempPtr->CCABUFH = ByteToWrite_.Byte1;
			break;
		case(1):
			TempPtr->CCBBUFL = ByteToWrite_.Byte0;
			TempPtr->CCBBUFH = ByteToWrite_.Byte1;
			break;
		case(2):
			TempPtr->CCCBUFL = ByteToWrite_.Byte0;
			TempPtr->CCCBUFH = ByteToWrite_.Byte1;
			break;
		case(3):
			TempPtr->CCDBUFL = ByteToWrite_.Byte0;
			TempPtr->CCDBUFH = ByteToWrite_.Byte1;
			break;
	}	
}
//#####################################################
unsigned short tc_cnt_read(Tc_t *TimerStruct)
{
	TC0_t* TempPtr = TimerStruct->Timer_BaseAddr;
	convert8to16 LoongToRead_;
	LoongToRead_.Byte0 = TempPtr->CNTL;
	LoongToRead_.Byte1 = TempPtr->CNTH;
	return LoongToRead_.ShortReturn;
}	
//#####################################################
//#####################################################
#ifdef use_tc0_ovf_int
ISR(TCC0_OVF_vect)
{
	Tc_t *TimerStruct = Timer_ServiceAddresses.Timer0RamServiceAddr;
	if(TimerStruct->CallBackOvf)TimerStruct->CallBackOvf(TimerStruct->CallBackOvfData);
}
#endif

#ifdef use_tc0_cca_int
ISR(TCC0_CCA_vect)
{
	Tc_t *TimerStruct = Timer_ServiceAddresses.Timer0RamServiceAddr;
	if(TimerStruct->CallBackCompCaptureA)TimerStruct->CallBackCompCaptureA(TimerStruct->CallBackCompCaptureDataA);
}
#endif

#ifdef use_tc0_ccb_int
ISR(TCC0_CCB_vect)
{
	Tc_t *TimerStruct = Timer_ServiceAddresses.Timer0RamServiceAddr;
	if(TimerStruct->CallBackCompCaptureB)TimerStruct->CallBackCompCaptureB(TimerStruct->CallBackCompCaptureDataB);
}
#endif

#ifdef use_tc0_ccc_int
ISR(TCC0_CCC_vect)
{
	Tc_t *TimerStruct = Timer_ServiceAddresses.Timer0RamServiceAddr;
	if(TimerStruct->CallBackCompCaptureC)TimerStruct->CallBackCompCaptureC(TimerStruct->CallBackCompCaptureDataC);
}
#endif

#ifdef use_tc0_ccd_int
ISR(TCC0_CCD_vect)
{
	Tc_t *TimerStruct = Timer_ServiceAddresses.Timer0RamServiceAddr;
	if(TimerStruct->CallBackCompCaptureD)TimerStruct->CallBackCompCaptureD(TimerStruct->CallBackCompCaptureDataD);
}
#endif
//#####################################################
#ifdef use_tc1_ovf_int
ISR(TCC1_OVF_vect)
{
	Tc_t *TimerStruct = Timer_ServiceAddresses.Timer1RamServiceAddr;
	if(TimerStruct->CallBackOvf)TimerStruct->CallBackOvf(TimerStruct->CallBackOvfData);
}
#endif

#ifdef use_tc1_cca_int
ISR(TCC1_CCA_vect)
{
	Tc_t *TimerStruct = Timer_ServiceAddresses.Timer1RamServiceAddr;
	if(TimerStruct->CallBackCompCaptureA)TimerStruct->CallBackCompCaptureA(TimerStruct->CallBackCompCaptureDataA);
}
#endif

#ifdef use_tc1_ccb_int
ISR(TCC1_CCB_vect)
{
	Tc_t *TimerStruct = Timer_ServiceAddresses.Timer1RamServiceAddr;
	if(TimerStruct->CallBackCompCaptureB)TimerStruct->CallBackCompCaptureB(TimerStruct->CallBackCompCaptureDataB);
}
#endif
//#####################################################
//#####################################################
#ifdef use_tc2_ovf_int
ISR(TCD0_OVF_vect)
{
	Tc_t *TimerStruct = Timer_ServiceAddresses.Timer2RamServiceAddr;
	if(TimerStruct->CallBackOvf)TimerStruct->CallBackOvf(TimerStruct->CallBackOvfData);
}
#endif

#ifdef use_tc2_cca_int
ISR(TCD0_CCA_vect)
{
	Tc_t *TimerStruct = Timer_ServiceAddresses.Timer2RamServiceAddr;
	if(TimerStruct->CallBackCompCaptureA)TimerStruct->CallBackCompCaptureA(TimerStruct->CallBackCompCaptureDataA);
}
#endif

#ifdef use_tc2_ccb_int
ISR(TCD0_CCB_vect)
{
	Tc_t *TimerStruct = Timer_ServiceAddresses.Timer2RamServiceAddr;
	if(TimerStruct->CallBackCompCaptureB)TimerStruct->CallBackCompCaptureB(TimerStruct->CallBackCompCaptureDataB);
}
#endif

#ifdef use_tc2_ccc_int
ISR(TCD0_CCC_vect)
{
	Tc_t *TimerStruct = Timer_ServiceAddresses.Timer2RamServiceAddr;
	if(TimerStruct->CallBackCompCaptureC)TimerStruct->CallBackCompCaptureC(TimerStruct->CallBackCompCaptureDataC);
}
#endif

#ifdef use_tc2_ccd_int
ISR(TCD0_CCD_vect)
{
	Tc_t *TimerStruct = Timer_ServiceAddresses.Timer2RamServiceAddr;
	if(TimerStruct->CallBackCompCaptureD)TimerStruct->CallBackCompCaptureD(TimerStruct->CallBackCompCaptureDataD);
}
#endif
//#####################################################
#ifdef use_tc3_ovf_int
ISR(TCD1_OVF_vect)
{
	Tc_t *TimerStruct = Timer_ServiceAddresses.Timer3RamServiceAddr;
	if(TimerStruct->CallBackOvf)TimerStruct->CallBackOvf(TimerStruct->CallBackOvfData);
}
#endif

#ifdef use_tc3_cca_int
ISR(TCD1_CCA_vect)
{
	Tc_t *TimerStruct = Timer_ServiceAddresses.Timer3RamServiceAddr;
	if(TimerStruct->CallBackCompCaptureA)TimerStruct->CallBackCompCaptureA(TimerStruct->CallBackCompCaptureDataA);
}
#endif

#ifdef use_tc3_ccb_int
ISR(TCD1_CCB_vect)
{
	Tc_t *TimerStruct = Timer_ServiceAddresses.Timer3RamServiceAddr;
	if(TimerStruct->CallBackCompCaptureB)TimerStruct->CallBackCompCaptureB(TimerStruct->CallBackCompCaptureDataB);
}
#endif
//#####################################################
//#####################################################
#ifdef use_tc4_ovf_int
ISR(TCE0_OVF_vect)
{
	Tc_t *TimerStruct = Timer_ServiceAddresses.Timer4RamServiceAddr;
	if(TimerStruct->CallBackOvf)TimerStruct->CallBackOvf(TimerStruct->CallBackOvfData);
}
#endif

#ifdef use_tc4_cca_int
ISR(TCE0_CCA_vect)
{
	Tc_t *TimerStruct = Timer_ServiceAddresses.Timer4RamServiceAddr;
	if(TimerStruct->CallBackCompCaptureA)TimerStruct->CallBackCompCaptureA(TimerStruct->CallBackCompCaptureDataA);
}
#endif

#ifdef use_tc4_ccb_int
ISR(TCE0_CCB_vect)
{
	Tc_t *TimerStruct = Timer_ServiceAddresses.Timer4RamServiceAddr;
	if(TimerStruct->CallBackCompCaptureB)TimerStruct->CallBackCompCaptureB(TimerStruct->CallBackCompCaptureDataB);
}
#endif

#ifdef use_tc4_ccc_int
ISR(TCE0_CCC_vect)
{
	Tc_t *TimerStruct = Timer_ServiceAddresses.Timer4RamServiceAddr;
	if(TimerStruct->CallBackCompCaptureC)TimerStruct->CallBackCompCaptureC(TimerStruct->CallBackCompCaptureDataC);
}
#endif

#ifdef use_tc4_ccd_int
ISR(TCE0_CCD_vect)
{
	Tc_t *TimerStruct = Timer_ServiceAddresses.Timer4RamServiceAddr;
	if(TimerStruct->CallBackCompCaptureD)TimerStruct->CallBackCompCaptureD(TimerStruct->CallBackCompCaptureDataD);
}
#endif
//#####################################################
#ifdef use_tc5_ovf_int
ISR(TCE1_OVF_vect)
{
	Tc_t *TimerStruct = Timer_ServiceAddresses.Timer5RamServiceAddr;
	if(TimerStruct->CallBackOvf)TimerStruct->CallBackOvf(TimerStruct->CallBackOvfData);
}
#endif

#ifdef use_tc5_cca_int
ISR(TCE1_CCA_vect)
{
	Tc_t *TimerStruct = Timer_ServiceAddresses.Timer5RamServiceAddr;
	if(TimerStruct->CallBackCompCaptureA)TimerStruct->CallBackCompCaptureA(TimerStruct->CallBackCompCaptureDataA);
}
#endif

#ifdef use_tc5_ccb_int
ISR(TCE1_CCB_vect)
{
	Tc_t *TimerStruct = Timer_ServiceAddresses.Timer5RamServiceAddr;
	if(TimerStruct->CallBackCompCaptureB)TimerStruct->CallBackCompCaptureB(TimerStruct->CallBackCompCaptureDataB);
}
#endif
//#####################################################
//#####################################################
#ifdef use_tc6_ovf_int
ISR(TCF0_OVF_vect)
{
	Tc_t *TimerStruct = Timer_ServiceAddresses.Timer6RamServiceAddr;
	if(TimerStruct->CallBackOvf)TimerStruct->CallBackOvf(TimerStruct->CallBackOvfData);
}
#endif

#ifdef use_tc6_cca_int
ISR(TCF0_CCA_vect)
{
	Tc_t *TimerStruct = Timer_ServiceAddresses.Timer6RamServiceAddr;
	if(TimerStruct->CallBackCompCaptureA)TimerStruct->CallBackCompCaptureA(TimerStruct->CallBackCompCaptureDataA);
}
#endif

#ifdef use_tc6_ccb_int
ISR(TCF0_CCB_vect)
{
	Tc_t *TimerStruct = Timer_ServiceAddresses.Timer6RamServiceAddr;
	if(TimerStruct->CallBackCompCaptureB)TimerStruct->CallBackCompCaptureB(TimerStruct->CallBackCompCaptureDataB);
}
#endif

#ifdef use_tc6_ccc_int
ISR(TCF0_CCC_vect)
{
	Tc_t *TimerStruct = Timer_ServiceAddresses.Timer6RamServiceAddr;
	if(TimerStruct->CallBackCompCaptureC)TimerStruct->CallBackCompCaptureC(TimerStruct->CallBackCompCaptureDataC);
}
#endif

#ifdef use_tc6_ccd_int
ISR(TCF0_CCD_vect)
{
	Tc_t *TimerStruct = Timer_ServiceAddresses.Timer6RamServiceAddr;
	if(TimerStruct->CallBackCompCaptureD)TimerStruct->CallBackCompCaptureD(TimerStruct->CallBackCompCaptureDataD);
}
#endif
//#####################################################
#ifdef use_tc7_ovf_int
ISR(TCF1_OVF_vect)
{
	Tc_t *TimerStruct = Timer_ServiceAddresses.Timer7RamServiceAddr;
	if(TimerStruct->CallBackOvf)TimerStruct->CallBackOvf(TimerStruct->CallBackOvfData);
}
#endif

#ifdef use_tc7_cca_int
ISR(TCF1_CCA_vect)
{
	Tc_t *TimerStruct = Timer_ServiceAddresses.Timer7RamServiceAddr;
	if(TimerStruct->CallBackCompCaptureA)TimerStruct->CallBackCompCaptureA(TimerStruct->CallBackCompCaptureDataA);
}
#endif

#ifdef use_tc7_ccb_int
ISR(TCF1_CCB_vect)
{
	Tc_t *TimerStruct = Timer_ServiceAddresses.Timer7RamServiceAddr;
	if(TimerStruct->CallBackCompCaptureB)TimerStruct->CallBackCompCaptureB(TimerStruct->CallBackCompCaptureDataB);
}
#endif
//#####################################################

