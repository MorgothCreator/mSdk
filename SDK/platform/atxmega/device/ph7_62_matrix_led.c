/*
 * ph7_62_matrix_led.c
 *
 * Created: 2/15/2013 11:29:03 PM
 *  Author: XxXx
 */ 
//#####################################################
#include <stdbool.h>
#include "ph7_62_matrix_led.h"
#include "driver/timer.h"
#include "api/lcd_api.h"
#include "api/lcd_def.h"
//#####################################################
extern unsigned long CoreFreq;
//#####################################################
void ph7_62_int1_service_function(void *Data)
{
	tDisplay *pDisplay = (tDisplay *)Data;
	ph7_62_t *UserData = (ph7_62_t *)pDisplay->UserData;
	
	
	ph7_62_RGB_t *VideoRamPointerUp  = UserData->DisplayData + ((UserData->V_Cnt + 1) * pDisplay->Width);
	ph7_62_RGB_t *VideoRamPointerDn  = VideoRamPointerUp + (UserData->V_Cnt_Limit * pDisplay->Width);
	//unsigned char *VideoRamPointerUp  = (unsigned char *)_VideoRamPointerUp;
	//unsigned char *VideoRamPointerDn  = (unsigned char *)_VideoRamPointerDn;
	unsigned char H_Cnt = 0;
	unsigned char ColorCnt = UserData->ColorCnt;
	unsigned char Tmp = 0;
	//VideoRamPointerUp-=3;
	//VideoRamPointerDn-=3;
	UserData->Oe_Port->OUTSET = UserData->Oe_PinMask;
	UserData->ABC_Port->OUT = (UserData->ABC_Port->OUT & ~(7 << ph7_62_ABC_PinPosition/*UserData->ABC_PinPosition*/)) | ((~UserData->V_Cnt & 7)  << ph7_62_ABC_PinPosition/*UserData->ABC_PinPosition*/);
	for (;H_Cnt < pDisplay->Width; H_Cnt++)
	{
		VideoRamPointerUp--;
		VideoRamPointerDn--;

		Tmp = UserData->RGB_Port->OUT;
		//Tmp &= ~UserData->RGB_Mask;
		if(VideoRamPointerUp->R > ColorCnt) Tmp |= ph7_62_R1_PinMask;//UserData->R1_PinMask;
		else  Tmp &= ~ph7_62_R1_PinMask;//UserData->R1_PinMask;
		if(VideoRamPointerDn->R > ColorCnt) Tmp |= ph7_62_R2_PinMask;//UserData->R2_PinMask;
		else  Tmp &= ~ph7_62_R2_PinMask;//UserData->R2_PinMask;
							
		if(VideoRamPointerUp->G > ColorCnt) Tmp |= ph7_62_G1_PinMask;//UserData->G1_PinMask;
		else  Tmp &= ~ph7_62_G1_PinMask;//UserData->G1_PinMask;
		if(VideoRamPointerDn->G > ColorCnt) Tmp |= ph7_62_G2_PinMask;//UserData->G2_PinMask;
		else  Tmp &= ~ph7_62_G2_PinMask;//UserData->G2_PinMask;
							
		if(VideoRamPointerUp->B > ColorCnt) Tmp |= ph7_62_B1_PinMask;//UserData->B1_PinMask;
		else  Tmp &= ~ph7_62_B1_PinMask;//UserData->B1_PinMask;
		if(VideoRamPointerDn->B > ColorCnt) Tmp |= ph7_62_B2_PinMask;//UserData->B2_PinMask;
		else  Tmp &= ~ph7_62_B2_PinMask;//UserData->B2_PinMask;
							
		//VideoRamPointerUp-=6;
		//VideoRamPointerDn-=6;
		UserData->RGB_Port->OUT = Tmp;
		UserData->Sclk_Port->OUTSET = UserData->Sclk_PinMask;
		UserData->Sclk_Port->OUTCLR = UserData->Sclk_PinMask;
	}
	UserData->Latch_Port->OUTSET = UserData->Latch_PinMask;
	UserData->Latch_Port->OUTCLR = UserData->Latch_PinMask;
	UserData->Oe_Port->OUTCLR = UserData->Oe_PinMask;
	UserData->ColorCnt++;
	if(UserData->ColorCnt >= UserData->ColorNr)
	{
		UserData->ColorCnt = 0;
		UserData->V_Cnt++;
		if(UserData->V_Cnt >= UserData->V_Cnt_Limit) UserData->V_Cnt = 0;
	}
}
//#####################################################
bool ph7_62_init(tDisplay *pDisplay)
{
	if(!pDisplay) return false;
	new_timer *TimerStruct = new_(new_timer);
	if(!TimerStruct) return false;

	ph7_62_t *UserData = (ph7_62_t *)pDisplay->UserData;
	//UserData->TimerBaseAddr = tc_get_set_base_address(TimerStruct, UserData->UsedTimerNr);
	
	UserData->DisplayData = (ph7_62_RGB_t *)calloc(1, sizeof(ph7_62_RGB_t) * pDisplay->Height * pDisplay->Width);
	
	if(!UserData->DisplayData)
	{
		if(UserData) free(UserData);
		return false;
	}
	
	UserData->ABC_Port->DIRSET = 7 << UserData->ABC_PinPosition;
	UserData->RGB_Port->DIRSET = UserData->R1_PinMask;
	UserData->RGB_Port->DIRSET = UserData->R2_PinMask;
	UserData->RGB_Port->DIRSET = UserData->G1_PinMask;
	UserData->RGB_Port->DIRSET = UserData->G2_PinMask;
	UserData->RGB_Port->DIRSET = UserData->B1_PinMask;
	UserData->RGB_Port->DIRSET = UserData->B2_PinMask;
	UserData->Latch_Port->DIRSET = UserData->Latch_PinMask;
	UserData->Oe_Port->DIRSET = UserData->Oe_PinMask;
	UserData->Sclk_Port->DIRSET = UserData->Sclk_PinMask;
	UserData->V_Cnt_Limit = pDisplay->Height / UserData->PanelsNr;
	
	//unsigned char H_Cnt = 0;
	//unsigned char V_Cnt = 0;
	//unsigned char ColorCnt = 0;
	//unsigned char HalfWidth = pDisplay->Width >> 1;
	UserData->RGB_Mask = UserData->R1_PinMask | UserData->R2_PinMask | UserData->G1_PinMask | UserData->G2_PinMask | UserData->B1_PinMask | UserData->B2_PinMask;
	//ph7_62_RGB_t *VideoRam = UserData->DisplayData;

	TimerStruct->Timer_PeriodUpdate = CoreFreq / (pDisplay->Height / UserData->PanelsNr)/ UserData->ColorNr/ UserData->FpsNr;
	TimerStruct->Timer_CompCapUpdateA = 0;
	TimerStruct->Timer_CompCapUpdateB = 0;
	TimerStruct->Timer_CompCapUpdateC = 0;
	TimerStruct->Timer_CompCapUpdateD = 0;
	TimerStruct->Timer_Number = 1;
	TimerStruct->Timer_ClockSel = _Timer_ClkSel_Div1;
	TimerStruct->Timer_WaveformGenMode = _Timer_WaveformGenMode_Normal;
	TimerStruct->Timer_EvAction = _Timer_EventAction_Off;
	TimerStruct->Timer_EvDelay = 0;
	TimerStruct->Timer_EvSourceSelect = _Timer_EventSource_Off;
	TimerStruct->Timer_ByteModeSel = _Timer_ByteMode_16Bit;
	TimerStruct->Timer_ErrIntLevel = _Timer_Int_Disabled;
	TimerStruct->Timer_OvfIntLevel = _Timer_Int_Med_Level;
	TimerStruct->Timer_Com = _Timer_Command_None;
	TimerStruct->Timer_LockUpdate = false;
	TimerStruct->Timer_Dir = _Timer_Increment;
	TimerStruct->Timer_PerBuffValid = false;
	TimerStruct->Timer_CompCaptureEnA = false;
	TimerStruct->Timer_CompCaptureEnB = false;
	TimerStruct->Timer_CompCaptureEnC = false;
	TimerStruct->Timer_CompCaptureEnD = false;
	TimerStruct->Timer_CompOutValueA = false;
	TimerStruct->Timer_CompOutValueB = false;
	TimerStruct->Timer_CompOutValueC = false;
	TimerStruct->Timer_CompOutValueD = false;
	TimerStruct->Timer_CompCapIntLevelA = _Timer_Int_Disabled;
	TimerStruct->Timer_CompCapIntLevelB = _Timer_Int_Disabled;
	TimerStruct->Timer_CompCapIntLevelC = _Timer_Int_Disabled;
	TimerStruct->Timer_CompCapIntLevelD = _Timer_Int_Disabled;
	TimerStruct->Timer_CompCapBuffValidA = false;
	TimerStruct->Timer_CompCapBuffValidB = false;
	TimerStruct->Timer_CompCapBuffValidC = false;
	TimerStruct->Timer_CompCapBuffValidD = false;
	TimerStruct->CallBackOvf = ph7_62_int1_service_function;
	TimerStruct->CallBackOvfData = (void*)pDisplay;
	tc_init(TimerStruct,1);



/*	while (1)
	{

		for (ColorCnt = 0; ColorCnt < UserData->ColorNr; ColorCnt++)
		{
			for (V_Cnt = 0;V_Cnt < pDisplay->Width / UserData->PanelsNr; V_Cnt++)
			{
				UserData->ABC_Port->OUT = (UserData->ABC_Port->OUT & ~(7 << UserData->ABC_PinPosition)) | (V_Cnt  << UserData->ABC_PinPosition);
				ph7_62_RGB_t *VideoRamPointerUp  = (VideoRam + ((V_Cnt + 1) * pDisplay->Width));
				ph7_62_RGB_t *VideoRamPointerDn  = (VideoRam + ((V_Cnt + 1) * pDisplay->Width) + HalfWidth);
				_delay_us(100);
				UserData->Oe_Port->OUTSET = UserData->Oe_PinMask;
				for (H_Cnt = 0;H_Cnt < pDisplay->Width; H_Cnt++)
				{
					VideoRamPointerUp--;
					VideoRamPointerDn--;

					unsigned char Tmp = UserData->RGB_Port->OUT;
					Tmp &= ~RGB_Mask;
					if(VideoRamPointerUp[0].R > ColorCnt) Tmp |= UserData->R1_PinMask;
					//else  Tmp &= ~UserData->R1_PinMask;
					if(VideoRamPointerDn[0].R > ColorCnt) Tmp |= UserData->R2_PinMask;
					//else  Tmp &= ~UserData->R2_PinMask;
					
					if(VideoRamPointerUp[0].G > ColorCnt) Tmp |= UserData->G1_PinMask;
					//else  Tmp &= ~UserData->G1_PinMask;
					if(VideoRamPointerDn[0].G > ColorCnt) Tmp |= UserData->G2_PinMask;
					//else  Tmp &= ~UserData->G2_PinMask;
					
					if(VideoRamPointerUp[0].B > ColorCnt) Tmp |= UserData->B1_PinMask;
					//else  Tmp &= ~UserData->B1_PinMask;
					if(VideoRamPointerDn[0].B > ColorCnt) Tmp |= UserData->B2_PinMask;
					//else  Tmp &= ~UserData->B2_PinMask;
					
					UserData->RGB_Port->OUT = Tmp;
					UserData->Sclk_Port->OUTSET = UserData->Sclk_PinMask;
					UserData->Sclk_Port->OUTCLR = UserData->Sclk_PinMask;
				}
				UserData->Latch_Port->OUTSET = UserData->Latch_PinMask;
				UserData->Latch_Port->OUTCLR = UserData->Latch_PinMask;
				UserData->Oe_Port->OUTCLR = UserData->Oe_PinMask;
			}
		}
	}*/
	
	
	return true;
}
//#####################################################
