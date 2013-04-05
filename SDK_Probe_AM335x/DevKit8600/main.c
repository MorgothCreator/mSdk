/***************************************************************************************/
/*    AM335x Multiplatform SDK test application.                                       */
/*    Copyright (C) 2013  Iulian Gheorghiu.                                            */
/*                                                                                     */
/*    This program is free software: you can redistribute it and/or modify             */
/*    it under the terms of the GNU General Public License as published by             */
/*    the Free Software Foundation, either version 3 of the License, or                */
/*    (at your option) any later version.                                              */
/*                                                                                     */
/*    This program is distributed in the hope that it will be useful,                  */
/*    but WITHOUT ANY WARRANTY; without even the implied warranty of                   */
/*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                    */
/*    GNU General Public License for more details.                                     */
/*                                                                                     */
/*    You should have received a copy of the GNU General Public License                */
/*    along with this program.  If not, see <http://www.gnu.org/licenses/>             */
/***************************************************************************************/
#include <math.h>
#include "main.h"

#include "board_properties.h"
#include "board_init.h"
#include "interface/lcd_interface.h"
#include "api/usb_msc_host_api.h"
#include "api/mmcsd_api.h"
#include "interface/rtc_interface.h"

#include "lib/gfx/controls_definition.h"
#include "lib/gfx/buton.h"
#include "lib/gfx/checkbox.h"
#include "lib/gfx/progressbar.h"
#include "lib/gfx/scrollbar.h"
#include "lib/gfx/textbox.h"
#include "lib/gfx/listbox.h"
#include "lib/gfx/window.h"
#include "lib/gfx/files/stl.h"

#include "lib/fs/fat.h"
#include "device/mpu60x0.h"
#include "device/mhc5883.h"
#include "device/ms5611.h"
#include "device/ft5x06.h"

#include "protocols/nmea.h"
//#include "lib/gfx/bmp/bmp.h"
//#include "lib/gfx/jpg/jpeg_decoder.h"
//#include "lib/gfx/png/png.h"
//#include "lib/gfx/mpeg/config.h"
//#include "lib/gfx/mpeg/Mpeg_api.h"

//#include "net/http_simple_server.h"

#ifdef UseMpeg12
#include "lib/gfx/mpeg12/mpeg12_api.h"
#endif

tWindow *MainWindow = NULL;

#ifdef UseMpeg12
mpeg_struct_t *MpegStruct;

#define MpegState_Play	1
#define MpegState_Stop	2

#define MpegOnFullScreen
int Fps = 0;
int MpegPlayerState = MpegState_Stop;
static volatile unsigned int CntDisplayRTC = 0;
static volatile unsigned int PlayerCommands = MpegState_Play;
#endif

bool stl_file_oppened = false;

double X_Axiz = 0;
double Y_Axiz = 0;
double Z_Axiz = 0;

double X_Compass_Calibration = 0;
double Y_Compass_Calibration = 0;
double Z_Compass_Calibration = 0;

signed int CompasField = 768;
signed int CompasFieldCalibration = 0;

static volatile unsigned int CntDisplayRTC = 0;

tDisplay *BackScreen = NULL;

/*#####################################################*/
timer(TimerScanTouch);
timer(TimerStlPaint);
/*#####################################################*/
void* btn_next_file(void* data)
{
#ifdef UseMpeg12
	if(FILE1)
	{
		mpeg_free(MpegStruct);
		_FatData_CloseFile(FILE1);
		if(MpegStruct) free(MpegStruct);
		MpegPlayerState = MpegState_Stop;
		if(FILE1)
		{
			if(_FatData_Dn(FILE1) == TRUE && PlayerCommands == MpegState_Play);
			else
			{
				_FatData_GoToRoot(FILE1);
			}
		}
		PlayerCommands = MpegState_Play;
		if(MpegPlayerState == MpegState_Stop && PlayerCommands == MpegState_Play)
		{
			if(!memcmp(FILE1->FileInfo_PointedFileInDirectoryExtension, "MPG", 3))
			{
				//delay_ms(1000);
				if(_FatData_OpenFile(FILE1))
					{
					//sample2(ScreenBuff, FILE1);
//					picturebox_clear_color(PICTUREBOX1, ClrBlack);
					MpegPlayerState = MpegState_Play;
					MpegStruct = calloc(1, sizeof(mpeg_struct_t));
					MpegStruct->EnableFrameLimit = true;
#ifdef MpegOnFullScreen
					MpegStruct->CallbackDisplayFrameVariable = (unsigned int)ScreenBuff;
					MpegStruct->CallbackDisplayFrame = screen_put_rgb_array_32;
					MpegStruct->mpeg_convert = mpeg2convert_rgb32;
#else
					MpegStruct->CallbackDisplayFrameVariable = (unsigned int)PICTUREBOX1;
					MpegStruct->CallbackDisplayFrame = picturebox_put_rgb_array_32;
#endif
//					PROGRESSVIDEO->Maximum = FILE1->FileInfo_CurrentOppenedFile_FileSize;
					mpeg_instance_init(MpegStruct, FILE1);
				}
				else PlayerCommands = MpegState_Stop;
			}
			else PlayerCommands = MpegState_Stop;
		}
	}
#endif
	return NULL;
//	PROGRESSVIDEO->Value = FILE1->Offset;
}
/*#####################################################*/
int main(void) {
	board_init();
	RtcInit();
/*******************************************************/
    timer_interval(&TimerScanTouch, 20);
    timer_interval(&TimerStlPaint, 1000);
/*******************************************************/
    //http_simple_init();
/*******************************************************/
    BackScreen = new_(new_screen);
    memcpy((void *)BackScreen, (void *)ScreenBuff, sizeof(new_screen));
    //BackScreen->Height = ScreenBuff->Height;
    //BackScreen->Width = ScreenBuff->Width;
    BackScreen->DisplayData = memalign(sizeof(BackScreen->DisplayData[0]) << 3, (BackScreen->Width * BackScreen->Height * sizeof(BackScreen->DisplayData[0])) + 32);
    MainWindow = new_window(BackScreen);
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

	tControlCommandData control_comand;
	control_comand.Comand = Control_Nop;
	control_comand.CursorCoordonateUsed = true;
/*******************************************************/
	volatile unsigned char ScreenReRefreshCnt = 0;
/*******************************************************/
	while(1)
	{
#ifdef USE_WDR
	WDR();
#endif
		if(timer_tick(&TimerScanTouch))
		{
			if(BackScreen)
			{
				if(TouchScreen->TouchScreen_Type == TouchScreen_Type_Int) TouchIdle(TouchScreen);
				else if(TouchScreen->TouchScreen_Type == TouchScreen_Type_FT5x06) ft5x06_TouchIdle(TouchScreen);
				//control_comand.CursorCoordonateUsed = false;
				//control_comand.Comand = Control_Nop;
				memset(&control_comand, 0, sizeof(tControlCommandData));
				control_comand.X = TouchScreen->TouchResponse.x1;
				control_comand.Y = TouchScreen->TouchResponse.y1;
				control_comand.Cursor = (CursorState)TouchScreen->TouchResponse.touch_event1;
				MainWindow->idle(MainWindow, &control_comand);
				if(control_comand.CursorCoordonateUsed) ScreenReRefreshCnt = 4;
				if(control_comand.CursorCoordonateUsed || ScreenReRefreshCnt != 0)
				{
					ScreenReRefreshCnt--;
					screen_copy(ScreenBuff, BackScreen);
				}

				#ifdef UseMpeg12
				if(FILE1)
				{
					if(MpegPlayerState == MpegState_Stop && PlayerCommands == MpegState_Play)
					{
						if(!memcmp(FILE1->FileInfo_PointedFileInDirectoryExtension, "MPG", 3))
						{
							//delay_ms(1000);
							if(_FatData_OpenFile(FILE1))
							{
								//sample2(ScreenBuff, FILE1);
								MpegPlayerState = MpegState_Play;
								MpegStruct = calloc(1, sizeof(mpeg_struct_t));
								MpegStruct->EnableFrameLimit = true;
#ifdef MpegOnFullScreen
								MpegStruct->CallbackDisplayFrameVariable = (unsigned int)ScreenBuff;
								MpegStruct->CallbackDisplayFrame = screen_put_rgb_array_32;
								MpegStruct->mpeg_convert = mpeg2convert_rgb32;
#else
								MpegStruct->CallbackDisplayFrameVariable = (unsigned int)PICTUREBOX1;
								MpegStruct->CallbackDisplayFrame = picturebox_put_rgb_array_32;
#endif
								//PROGRESSVIDEO->Maximum = FILE1->FileInfo_CurrentOppenedFile_FileSize;
								mpeg_instance_init(MpegStruct, FILE1);
							}
							else PlayerCommands = MpegState_Stop;
						}
						else PlayerCommands = MpegState_Stop;
					}
				}
#endif
			}
			mmcsd_idle(NULL);
			usb_host_idle(0);
			usb_host_idle(1);
			if(rtcSecUpdate != CntDisplayRTC)
			{
				CntDisplayRTC = rtcSecUpdate;
			}
#ifdef UseMpeg12
			if(MpegPlayerState == MpegState_Play)
			{
				if((mpeg_idle(MpegStruct, ScreenBuff, FILE1) == 0 && FILE1 != 0) || PlayerCommands == MpegState_Stop)
				{
					mpeg_free(MpegStruct);
					_FatData_CloseFile(FILE1);
					if(MpegStruct) free(MpegStruct);
					MpegPlayerState = MpegState_Stop;
					if(FILE1 && PlayerCommands == MpegState_Play)
					{
						if(_FatData_Dn(FILE1) == TRUE);
						else
						{
							_FatData_GoToRoot(FILE1);
						}
					}
				}
			}
#endif
		}
#ifdef board_type_devkit8600
		signed char CharUart2 = -1;
		do
		{
			CharUart2 = UARTGetcNoBlocking(Uart2);
			if(CharUart2 >= 0)
			{
				UARTPutc(DebugCom, (unsigned char)CharUart2);
			}
		}while(CharUart2 >= 0);
		signed char CharUartDebug = -1;
		do
		{
			CharUartDebug = UARTGetcNoBlocking(DebugCom);
			if(CharUartDebug >= 0)
			{
				UARTPutc(Uart2, (unsigned char)CharUartDebug);
			}
		}while(CharUartDebug >= 0);
#endif
	}
/*#####################################################*/
}
