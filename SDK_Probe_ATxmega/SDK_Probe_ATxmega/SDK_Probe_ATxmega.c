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
		}			
    }
}
/*#####################################################*/
