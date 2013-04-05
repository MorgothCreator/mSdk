/*
 * SDK_Probe_ATxmega.c
 *
 * Created: 2/8/2013 12:14:37 AM
 *  Author: XxXx
 */ 
/*#####################################################*/
#define F_CPU	44000000

#include <avr/io.h>
#include <stdlib.h>
#include "sys/avr_compiler.h"
#include "board_properties.h"
#include "board_init.h"
#include "sys/core_init.h"
#include "lib/gfx/buton.h"
#include "lib/gfx/checkbox.h"
#include "lib/gfx/progressbar.h"
#include "lib/gfx/scrollbar.h"
#include "lib/gfx/textbox.h"
#include "lib/gfx/listbox.h"
#include "lib/gfx/window.h"
//#include "device/24c.h"
#include "device/ar1020.h"
//#include "device/ph7_62_matrix_led.h"

//#include "lib/fft.h"
/*#####################################################*/
timer(TimerScanTouch);
timer(TimerInitTouchCalibration);
/*#####################################################*/
/*tButton *btn = NULL;
tButton *next_file = NULL;
tCheckBox *check = NULL;
tProgressBar *progress = NULL;
tProgressBar *progress1 = NULL;
tScrollBar *Vscroll = NULL;
tScrollBar *Hscroll = NULL;
tTextBox *text = NULL;
tListBox *list = NULL;*/
tWindow *MainWindow = NULL;
/*-----------------------------------------------------*/
FileInfo_t *BrowserDisk1 = NULL;
/*-----------------------------------------------------*/
//new_screen* ph_7_62_ScreenBuff = NULL;
//unsigned long FFT_Cnt = 0;
/*#####################################################*/
int main(void)
{
	malloc_set_start(0x3000);
	malloc_set_end(0xFFFF);
    board_init();
    timer_interval(&TimerScanTouch, 25);
	timer_interval(&TimerInitTouchCalibration, 10000);
	//unsigned char TmpBuff[10];
	//if (E2promRead(TWI3, 0x50, 0, TmpBuff, 10)) UARTPuts(DebugCom, "24C eeprom device detected\n\r" , -1);
	//else UARTPuts(DebugCom, "24C eeprom device not detected\n\r" , -1);

    MainWindow = new_window(ScreenBuff);
    window_new_buton(MainWindow, Btn1);
    window_new_checkbox(MainWindow, CB1);
    window_new_listbox(MainWindow, ListBox1);
    window_new_progressbar(MainWindow, PBar1);
    window_new_scrollbar(MainWindow, ScrollBar1);
    window_new_textbox(MainWindow, TextBox1);

    char TmpStr[30];
    unsigned int CntItems = 0;
    for(CntItems = 0; CntItems < 100; CntItems++)
    {
	    sprintf(TmpStr, "%d", CntItems);
	    listbox_item_add(ListBox1, TmpStr);
    }

    /*btn = new_button(ScreenBuff);
    next_file = new_button(ScreenBuff);
    check = new_checkbox(ScreenBuff);
    progress = new_progressbar(ScreenBuff);
    progress1 = new_progressbar(ScreenBuff);
    Vscroll = new_scrollbar(ScreenBuff);
    Hscroll = new_scrollbar(ScreenBuff);
    text = new_textbox(ScreenBuff);
    list = new_listbox(ScreenBuff);*/


	/*if(ScreenBuff)
	{
		Vscroll->Maximum = 80;
		Vscroll->Position.X = 10;
		Vscroll->Position.Y = 152;

		Hscroll->Maximum = 80;
		Hscroll->Position.X = 40;
		Hscroll->Position.Y = 152;
		Hscroll->Size.X = 60;
		Hscroll->Size.Y = 20;

		btn->Caption.Text = "Btn1";

		next_file->Caption.Text = "Next file";
		next_file->Position.X = 300;
		next_file->Position.Y = 10;
		next_file->Size.X = 100;
		next_file->Size.Y = 100;

		check->Position.X = 80;
		check->Size.X = 100;

		progress->Position.X = 200;
		progress->Size.X = 100;
		progress->Size.Y = 20;

		progress1->Position.X = 200;
		progress1->Position.Y = 40;
		progress1->Size.X = 100;
		progress1->Size.Y = 20;

		char TmpStr[30];
		unsigned int CntItems = 0;
		for(CntItems = 0; CntItems < 100; CntItems++)
		{
			sprintf(TmpStr, "%d", CntItems);
			listbox_item_add(list, TmpStr);
		}
		list->Internals.ItemStartOnBox = 1;
		list->Size.ScrollSize = 20;
		list->Position.X = 120;
		list->Size.X = 150;
		list->Size.Y = 150;
	}*/
	//btn->Events.OnUp.CallbackData = (void*)btn;
	//btn->Events.OnUp.CallBack = btn_change_location;

	/*tControlCommandData control_comand;
	control_comand.Comand = Control_Nop;
	
	if(Drives_Table[0]->DiskInfo_FatType != NoFs) 
	{
		unsigned char Status = FileSearch_Status_Directory_not_exist;
		BrowserDisk1 = _FatData_SearchFileDirectory(0, ":/Dir1/Dir2/Dir3/File1.txt", &Status);
		if(Status == FileSearch_Status_File_open_ok)
		{
			UARTprintf(DebugCom, "Opened file: %s.\n\r" , str_to_upercase(BrowserDisk1->FileInfo_PointedFileInDirectoryLongFileName));
			_FatData_CloseFile(BrowserDisk1);
		}
		else if(Status == FileSearch_Status_FileEmpty) UARTPuts(DebugCom, "The file is empty.\n\r", -1);
		else UARTPuts(DebugCom, "The file can't be opened or not exist.\n\r", -1);
	}	*/
	
	
	/*new_ph7_62 *ph_7_62_settings = new_(new_ph7_62);
	ph_7_62_settings->RGB_Port = &ph7_62_RGB_Port;
	ph_7_62_settings->B1_PinMask = ph7_62_B1_PinMask;
	ph_7_62_settings->B2_PinMask = ph7_62_B2_PinMask;
	ph_7_62_settings->G1_PinMask = ph7_62_G1_PinMask;
	ph_7_62_settings->G2_PinMask = ph7_62_G2_PinMask;
	ph_7_62_settings->R1_PinMask = ph7_62_R1_PinMask;
	ph_7_62_settings->R2_PinMask = ph7_62_R2_PinMask;
	ph_7_62_settings->Sclk_PinMask = ph7_62_Sclk_PinMask;
	ph_7_62_settings->Sclk_Port = &ph7_62_Sclk_Port;
	ph_7_62_settings->Latch_PinMask = ph7_62_Latch_PinMask;
	ph_7_62_settings->Latch_Port = &ph7_62_Latch_Port;
	ph_7_62_settings->Oe_PinMask = ph7_62_Oe_PinMask;
	ph_7_62_settings->Oe_Port = &ph7_62_Oe_Port;
	ph_7_62_settings->ABC_PinPosition = ph7_62_ABC_PinPosition;
	ph_7_62_settings->ABC_Port = &ph7_62_ABC_Port;
	ph_7_62_settings->UsedTimerChanel = 0;
	ph_7_62_settings->UsedTimerNr = 1;
	ph_7_62_settings->PanelsNr = ph7_62_PanelsNr;
	ph_7_62_settings->ColorNr = 8;
	ph_7_62_settings->FpsNr = 50;

	ph_7_62_ScreenBuff = new_(new_screen);
	ph_7_62_ScreenBuff->Height = ph7_62_Height;
	ph_7_62_ScreenBuff->Width = ph7_62_Width;
	ph_7_62_ScreenBuff->UserData = ph_7_62_settings;
	ph7_62_init(ph_7_62_ScreenBuff);
	
	memset(ph_7_62_settings->DisplayData, 1, sizeof(ph7_62_RGB_t) * ph_7_62_ScreenBuff->Height * ph_7_62_ScreenBuff->Width);
	for(unsigned int Tmp1 = 0; Tmp1 < ph_7_62_ScreenBuff->Height * ph_7_62_ScreenBuff->Width; Tmp1++) ph_7_62_settings->DisplayData[Tmp1].R = (Tmp1) & (ph_7_62_settings->ColorNr - 1);
	for(unsigned int Tmp1 = 0; Tmp1 < ph_7_62_ScreenBuff->Height * ph_7_62_ScreenBuff->Width; Tmp1++) ph_7_62_settings->DisplayData[Tmp1].G = (Tmp1) & (ph_7_62_settings->ColorNr - 1);
	for(unsigned int Tmp1 = 0; Tmp1 < ph_7_62_ScreenBuff->Height * ph_7_62_ScreenBuff->Width; Tmp1++) ph_7_62_settings->DisplayData[Tmp1].B = (Tmp1) & (ph_7_62_settings->ColorNr - 1);
	//ph_7_62_settings->DisplayData[1 + ( 0 * ph_7_62_ScreenBuff->Width)].R = ph_7_62_settings->ColorNr;
	//ph_7_62_settings->DisplayData[1 + ( 8 * ph_7_62_ScreenBuff->Width)].R = ph_7_62_settings->ColorNr;

	//ph_7_62_settings->DisplayData[2 + ( 0 * ph_7_62_ScreenBuff->Width)].G = ph_7_62_settings->ColorNr;
	//ph_7_62_settings->DisplayData[2 + ( 8 * ph_7_62_ScreenBuff->Width)].G = ph_7_62_settings->ColorNr;

	//ph_7_62_settings->DisplayData[3 + ( 0 * ph_7_62_ScreenBuff->Width)].B = ph_7_62_settings->ColorNr;
	//ph_7_62_settings->DisplayData[3 + ( 8 * ph_7_62_ScreenBuff->Width)].B = ph_7_62_settings->ColorNr;*/

	
	/*sound_sample *input_buff = (sound_sample *)malloc((FFT_BUFFER_SIZE * 2) * sizeof(sound_sample));
	USE_FFT_BUFFERS *output_buff = (USE_FFT_BUFFERS *)malloc(((FFT_BUFFER_SIZE / 2) + 1) * sizeof(double));
	fft_state *state = fft_init();
	double fft_increment = 0.0001;*/
	
	tControlCommandData control_comand;
	control_comand.Comand = Control_Nop;
	while(1)
    {
		asm("SEI");
		if(timer_tick(&TimerScanTouch))
		{
			//UARTprintf(DebugCom, "%u\n\r", FFT_Cnt);
			//FFT_Cnt = 0;
			control_comand.Comand = Control_Nop;
			if (gpio_in(HARDBTN1))
			{
				timer_enable(&TimerInitTouchCalibration);
			}
			else
			{
				if(timer_tick(&TimerInitTouchCalibration))
				{
					UARTPuts(DebugCom, "Init calibration of LCD resistive touch screen....." , -1);
					put_rectangle(ScreenBuff, 0, 0, ScreenBuff->Width, ScreenBuff->Height, true, controls_color.Scren);
					ar1020_calibration_start(TouchScreen, ScreenBuff);
					UARTPuts(DebugCom, "OK.\n\r" , -1);
					control_comand.Comand = Control_Entire_Repaint;
					timer_enable(&TimerInitTouchCalibration);
				}
			}
			ar1020_read_coordonate(TouchScreen);
			memset(&control_comand, 0, sizeof(tControlCommandData));
			control_comand.X = TouchScreen->TouchResponse.x1;
			control_comand.Y = TouchScreen->TouchResponse.y1;
			control_comand.Cursor = (CursorState)TouchScreen->TouchResponse.touch_event1;
			MainWindow->idle(MainWindow, &control_comand);
			/*control_comand.CursorCoordonateUsed = false;
			control_comand.X = TouchScreen->TouchResponse.x1;
			control_comand.Y = TouchScreen->TouchResponse.y1;
			control_comand.Cursor = (CursorState)TouchScreen->TouchResponse.touch_event1;*/
			/*progressbar(progress, &control_comand);
			if(progress->Events.ValueChanged)
			{
				progress->Events.ValueChanged = false;
				btn->Position.Y = progress->Value;
				check->Position.Y = progress->Value;
				Hscroll->Position.Y = progress->Value + 152;
				Vscroll->Position.Y = progress->Value + 152;
				list->Position.Y = progress->Value + 75;
				text->Position.Y = progress->Value + 50;
			}
			progressbar(progress1, &control_comand);
			if(progress1->Events.ValueChanged)
			{
				progress1->Events.ValueChanged = false;
				btn->Position.X = progress1->Value + 10;
				check->Position.X = progress1->Value + 80;
				Hscroll->Position.X = progress1->Value + 40;
				Vscroll->Position.X = progress1->Value + 10;
				list->Position.X = progress1->Value + 120;
				text->Position.X = progress1->Value + 10;
			}
			button(btn, &control_comand);
			button(next_file, &control_comand);
			checkbox(check, &control_comand);
			scrollbar(Hscroll, &control_comand);
			scrollbar(Vscroll, &control_comand);
			textbox(text, &control_comand);
			listbox(list, &control_comand);*/
		}			
    }
}
/*#####################################################*/
