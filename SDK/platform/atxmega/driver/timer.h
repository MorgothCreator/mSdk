/*
 * timer.h
 *
 * Created: 2/15/2013 11:32:08 PM
 *  Author: XxXx
 */ 
//#####################################################
#ifndef TIMER_H_
#define TIMER_H_
//#####################################################
#include <avr/io.h>
#include <stdlib.h>
//#####################################################
#define _Timer_Int_Disabled						(0)
#define _Timer_Int_Low_Level					(1)
#define _Timer_Int_Med_Level					(2)
#define _Timer_Int_High_Level					(3)
//-----------------------------------------------------
#define _Timer_ClkSel_Off						(0)
#define _Timer_ClkSel_Div1						(1)
#define _Timer_ClkSel_Div2						(2)
#define _Timer_ClkSel_Div4						(3)
#define _Timer_ClkSel_Div8						(4)
#define _Timer_ClkSel_Div64						(5)
#define _Timer_ClkSel_Div256					(6)
#define _Timer_ClkSel_Div1024					(7)
#define _Timer_ClkSel_EventCh0					(8)
#define _Timer_ClkSel_EventCh1					(9)
#define _Timer_ClkSel_EventCh2					(10)
#define _Timer_ClkSel_EventCh3					(11)
#define _Timer_ClkSel_EventCh4					(12)
#define _Timer_ClkSel_EventCh5					(13)
#define _Timer_ClkSel_EventCh6					(14)
#define _Timer_ClkSel_EventCh7					(15)
//-----------------------------------------------------
#define _Timer_WaveformGenMode_Normal			(0)
#define _Timer_WaveformGenMode_FRQ				(1)
#define _Timer_WaveformGenMode_SsPwm			(3)
#define _Timer_WaveformGenMode_DsTPwm			(5)
#define _Timer_WaveformGenMode_DsTbPwm			(6)
#define _Timer_WaveformGenMode_DsBPwm			(7)
//-----------------------------------------------------
#define _Timer_EventAction_Off					(0)
#define _Timer_EventAction_InputCapture			(1)
#define _Timer_EventAction_UpDownCount			(2)
#define _Timer_EventAction_QDecode				(3)
#define _Timer_EventAction_Restart				(4)
#define _Timer_EventAction_FreqCapture			(5)
#define _Timer_EventAction_PulseWidthCapture	(6)
//-----------------------------------------------------
#define _Timer_EventSource_Off					(0)
#define _Timer_EventSource_Chanel0				(8)
#define _Timer_EventSource_Chanel1				(9)
#define _Timer_EventSource_Chanel2				(10)
#define _Timer_EventSource_Chanel3				(11)
#define _Timer_EventSource_Chanel4				(12)
#define _Timer_EventSource_Chanel5				(13)
#define _Timer_EventSource_Chanel6				(14)
#define _Timer_EventSource_Chanel7				(15)
//-----------------------------------------------------
#define _Timer_ByteMode_16Bit					(0)
#define _Timer_ByteMode_8Bit					(1)
//-----------------------------------------------------
#define _Timer_Command_None						(0)
#define _Timer_Command_Update					(1)
#define _Timer_Command_Restart					(2)
#define _Timer_Command_Reset					(3)
//-----------------------------------------------------
#define _Timer_Increment						(0)
#define _Timer_Decrement						(1)
//#####################################################
typedef struct
{
	TC0_t* Timer_BaseAddr;						//Here will be stored the Timer  Base address.
	unsigned short Timer_CntValue;
	unsigned short Timer_PeriodUpdate;
	unsigned short Timer_CompCapUpdateA;
	unsigned short Timer_CompCapUpdateB;
	unsigned short Timer_CompCapUpdateC;
	unsigned short Timer_CompCapUpdateD;
	unsigned char Timer_Number;
	unsigned char Timer_ClockSel;
	unsigned char Timer_WaveformGenMode;
	unsigned char Timer_EvAction;
	unsigned char Timer_EvDelay;
	unsigned char Timer_EvSourceSelect;
	unsigned char Timer_ByteModeSel;
	unsigned char Timer_ErrIntLevel;
	unsigned char Timer_OvfIntLevel;
	unsigned char Timer_Com;
	unsigned char Timer_LockUpdate;
	unsigned char Timer_Dir;
	unsigned char Timer_PerBuffValid;
	unsigned char Timer_CompCaptureEnA;
	unsigned char Timer_CompCaptureEnB;
	unsigned char Timer_CompCaptureEnC;
	unsigned char Timer_CompCaptureEnD;
	unsigned char Timer_CompOutValueA;
	unsigned char Timer_CompOutValueB;
	unsigned char Timer_CompOutValueC;
	unsigned char Timer_CompOutValueD;
	unsigned char Timer_CompCapIntLevelA;
	unsigned char Timer_CompCapIntLevelB;
	unsigned char Timer_CompCapIntLevelC;
	unsigned char Timer_CompCapIntLevelD;
	unsigned char Timer_CompCapBuffValidA;
	unsigned char Timer_CompCapBuffValidB;
	unsigned char Timer_CompCapBuffValidC;
	unsigned char Timer_CompCapBuffValidD;
	void* CallBackCompCaptureDataA;
	void* CallBackCompCaptureDataB;
	void* CallBackCompCaptureDataC;
	void* CallBackCompCaptureDataD;
	void* CallBackOvfData;
	void(*CallBackCompCaptureA)(void*);
	void(*CallBackCompCaptureB)(void*);
	void(*CallBackCompCaptureC)(void*);
	void(*CallBackCompCaptureD)(void*);
	void(*CallBackOvf)(void*);
}Tc_t;
//#####################################################
typedef struct
{
	Tc_t* Timer0RamServiceAddr;
	Tc_t* Timer1RamServiceAddr;
	Tc_t* Timer2RamServiceAddr;
	Tc_t* Timer3RamServiceAddr;
	Tc_t* Timer4RamServiceAddr;
	Tc_t* Timer5RamServiceAddr;
	Tc_t* Timer6RamServiceAddr;
	Tc_t* Timer7RamServiceAddr;
} TimerServiceAddresses;
extern TimerServiceAddresses Timer_ServiceAddresses;
//#####################################################
#define new_timer Tc_t
#ifndef new_
#define new_(structure) (structure*)calloc(1,sizeof(structure));
#endif
#define free_timer(address) free(address);
//#####################################################
TC0_t* tc_get_set_base_address(Tc_t *TimerStruct,unsigned char TimerNr);
void tc_init(Tc_t *TimerStruct, unsigned char TimerNr);
void tc_clk_sel(Tc_t *TimerStruct,unsigned char Value);
void tc_wave_gen_mede(Tc_t *TimerStruct,unsigned char Value);
void tc_event_action(Tc_t *TimerStruct,unsigned char Value);
void tc_event_delay(Tc_t *TimerStruct,unsigned char Value);
void tc_source_select(Tc_t *TimerStruct,unsigned char Value);
void tc_byte_mode(Tc_t *TimerStruct,unsigned char Action);
void tc_err_int_lvl(Tc_t *TimerStruct,unsigned char Value);
void tc_ovf_int_lvl(Tc_t *TimerStruct,unsigned char Value);
void tc_command_set(Tc_t *TimerStruct,unsigned char Value);
void tc_command_clr(Tc_t *TimerStruct,unsigned char Value);
void tc_direction_set(Tc_t *TimerStruct,unsigned char Action);
void tc_lock_update(Tc_t *TimerStruct,unsigned char Action);
void tc_chanel_int_lvl(Tc_t *TimerStruct,unsigned char Chanel,unsigned char Action);
void tc_comp_out_en(Tc_t *TimerStruct,unsigned char Chanel,unsigned char Action);
void tc_comp_cap_en(Tc_t *TimerStruct,unsigned char Chanel,unsigned char Action);
void tc_cnt_write(Tc_t *TimerStruct,unsigned short Value);
void tc_per_write(Tc_t *TimerStruct,unsigned short Value);
void tc_comp_cap_write(Tc_t *TimerStruct, unsigned char Chanel, unsigned short Value);
void tc_per_buff_write(Tc_t *TimerStruct,unsigned short Value);
void tc_comp_cap_buff_write(Tc_t *TimerStruct,unsigned char Chanel,unsigned short Value);
unsigned short tc_cnt_read(Tc_t *TimerStruct);
//#####################################################
#ifdef HEADER_INCLUDE_C_FILES
#include "timer.c"
#endif
//#####################################################
#endif /* TIMER_H_ */
//#####################################################
