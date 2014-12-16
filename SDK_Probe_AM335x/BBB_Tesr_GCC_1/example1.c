/***************************************************************************************/
/*    AM335x Multiplatform SDK test application.                                       */
/*    Copyright (C) 2013  Iulian Gheorghiu.                                            */
/*                                                                                     */
/*    This program is free software; you can redistribute it and/or                    */
/*    modify it under the terms of the GNU General Public License                      */
/*    as published by the Free Software Foundation; either version 2                   */
/*    of the License, or (at your option) any later version.                           */
/*                                                                                     */
/*    This program is distributed in the hope that it will be useful,                  */
/*    but WITHOUT ANY WARRANTY; without even the implied warranty of                   */
/*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                    */
/*    GNU General Public License for more details.                                     */
/*                                                                                     */
/*    You should have received a copy of the GNU General Public License                */
/*    along with this program.  If not, see <http://www.gnu.org/licenses/>             */
/***************************************************************************************/
//#define USE_BACK_SCREEN


#include <math.h>
#include "main.h"
#include "example1.h"

#ifdef test1

#define USE_BACK_SCREEN

#include "sys/plat_properties.h"
#include "board_init.h"
#include "interface/lcd_interface.h"
#include "interface/usb_host_msc_interface.h"
#include "interface/usb_host_mouse_interface.h"
#include "api/timer_api.h"
#include "api/usb_api.h"
#include "device/ft5x06.h"

#include "lib/gfx/controls_definition.h"
#include "lib/gfx/button.h"
#include "lib/gfx/checkbox.h"
#include "lib/gfx/progressbar.h"
#include "lib/gfx/scrollbar.h"
#include "lib/gfx/textbox.h"
#include "lib/gfx/listbox.h"
#include "lib/gfx/window.h"
#include "lib/gfx/keyboard.h"

#include "lib/string_lib.h"

#include "lib/gfx/resource/fonts.h"

#include "app/console.h"

_new_window(HeaderWindow);
_new_window(MainWindow);
_new_window(KbdWindow);

#ifdef USE_BACK_SCREEN
tDisplay *BackScreen = NULL;
volatile unsigned char ScreenReRefreshCnt = 0;
#endif

signed int picture_old_x = 0;
signed int picture_old_y = 0;

unsigned int PictureBoxbackBuff[128*96];

//textbox_static_item(test_static_textbox, NULL, NULL);
/*#####################################################*/
timer(TimerScanTouch);
/*#####################################################*/
void *picture_box_clear_callback(void *data)
{
	tPictureBox* settings = (tPictureBox*)data;
	picturebox_clear(settings);
	unsigned int ClearCnt = 0;
	for(; ClearCnt < 128*96; ClearCnt++)
	{
		PictureBoxbackBuff[ClearCnt] = settings->BackgroundColor;
	}
	return NULL;
}
/*#####################################################*/
void *picture_box_callback_on_down(struct PictureBox_s *settings, tControlCommandData *control_comand)
{
	picture_old_x = control_comand->X;
	picture_old_y = control_comand->Y;
	return NULL;
}
/*#####################################################*/
#include "lib/gfx/bmp/test_bmp.h"
void *picture_box_refresh_callback(struct PictureBox_s *settings, tControlCommandData *control_comand)
{
	tRectangle dest_rectangle;
	dest_rectangle.sXMin = 0;
	dest_rectangle.sXMax = 128;
	dest_rectangle.sYMin = 0;
	dest_rectangle.sYMax = 96;

	tRectangle src_rectangle;
	src_rectangle.sXMin = 0;
	src_rectangle.sXMax = 128;
	src_rectangle.sYMin = 0;
	src_rectangle.sYMax = 96;

	picturebox_copy_rectangle(settings, PictureBoxbackBuff, 0, &dest_rectangle, &src_rectangle, 128, 96);
	//picturebox_put_bitmap(settings, (unsigned char *)ButtonOk, 0, 0, false, false);
	picturebox_put_fbitmap(settings, "0:test.bmp", 0, 0, false, false);
#ifdef USE_BACK_SCREEN
	ScreenReRefreshCnt = 2;
#endif
	return NULL;
}
/*#####################################################*/
void *picture_box_callback(struct PictureBox_s *settings, tControlCommandData *control_comand)
{
	PictureBoxbackBuff[control_comand->X + (control_comand->Y * 128)] = ClrBlack;
	picture_old_x = control_comand->X;
	picture_old_y = control_comand->Y;

	tRectangle dest_rectangle;
	dest_rectangle.sXMin = 0;
	dest_rectangle.sXMax = 128;
	dest_rectangle.sYMin = 0;
	dest_rectangle.sYMax = 96;

	tRectangle src_rectangle;
	src_rectangle.sXMin = 0;
	src_rectangle.sXMax = 128;
	src_rectangle.sYMin = 0;
	src_rectangle.sYMax = 96;

	picturebox_copy_rectangle(settings, PictureBoxbackBuff, 0, &dest_rectangle, &src_rectangle, 128, 96);
	//picturebox_put_bitmap(settings, (unsigned char *)ButtonOk, 0, 0, false, false);
	picturebox_put_fbitmap(settings, "0:test.bmp", 0, 0, false, false);
	return NULL;
}
/*#####################################################*/
int main(void) {
    board_init();
/*******************************************************/
    timer_interval(&TimerScanTouch, 10);
/*******************************************************/
#ifdef lcd
#ifdef USE_BACK_SCREEN
    BackScreen = new_(new_screen);
    memcpy((void *)BackScreen, (void *)ScreenBuff, sizeof(new_screen));
#ifdef gcc
    BackScreen->DisplayData = malloc((BackScreen->raster_timings->X * BackScreen->raster_timings->Y * sizeof(BackScreen->DisplayData[0])) + (BackScreen->raster_timings->palete_len * sizeof(BackScreen->DisplayData[0])));
#else
    BackScreen->DisplayData = memalign(sizeof(BackScreen->DisplayData[0]) << 3, (BackScreen->raster_timings->X * BackScreen->raster_timings->Y * sizeof(BackScreen->DisplayData[0])) + (BackScreen->raster_timings->palete_len * sizeof(BackScreen->DisplayData[0])));
#endif
    MainWindow = new_window(NULL, BackScreen);
#else
    MainWindow = new_window(NULL, ScreenBuff);
#endif
    window_new_button(MainWindow, Btn1);
    window_new_checkbox(MainWindow, CB1);
    window_new_listbox(MainWindow, ListBox1);
    window_new_progressbar(MainWindow, PBar1);
    window_new_scrollbar(MainWindow, ScrollBar1);
    window_new_textbox(MainWindow, TextBox1);
    window_new_picturebox(MainWindow, PictureBox1);
    window_new_window(MainWindow, Window1);

    MainWindow->WindowMoveLimits.sXMin = 0;
    MainWindow->WindowMoveLimits.sXMax = ScreenBuff->raster_timings->X;
    MainWindow->WindowMoveLimits.sYMin = 20;
    MainWindow->WindowMoveLimits.sYMax = ScreenBuff->raster_timings->Y - 100;

#ifdef USE_BACK_SCREEN
    KbdWindow = new_window(NULL, BackScreen);
    window_new_keyboard(KbdWindow, Kbd);
    HeaderWindow = new_window(NULL, BackScreen);
#else
    KbdWindow = new_window(NULL, ScreenBuff);
    window_new_keyboard(KbdWindow, Kbd);
    HeaderWindow = new_window(NULL, ScreenBuff);
#endif
    HeaderWindow->WindowMoveLimits.sXMin = 0;
    HeaderWindow->WindowMoveLimits.sXMax = ScreenBuff->raster_timings->X;
    HeaderWindow->WindowMoveLimits.sYMin = 0;
    HeaderWindow->WindowMoveLimits.sYMax = 20;
    HeaderWindow->HideHeader = true;
    HeaderWindow->HideHScroll = true;
    HeaderWindow->HideVScroll = true;

    KbdWindow->WindowMoveLimits.sXMin = 0;
    KbdWindow->WindowMoveLimits.sXMax = ScreenBuff->raster_timings->X;
    KbdWindow->WindowMoveLimits.sYMin = ScreenBuff->raster_timings->Y - 100;
    KbdWindow->WindowMoveLimits.sYMax = ScreenBuff->raster_timings->Y;
    KbdWindow->HideHeader = true;
    KbdWindow->HideHScroll = true;
    KbdWindow->HideVScroll = true;
    KbdWindow->Caption.Font = (tFont *)&g_sFontCmss18b;
/* Set location and size of virtual keyboard */
    Kbd->Position.X = 0;
    Kbd->Position.Y = 0;
    Kbd->Size.X = ScreenBuff->raster_timings->X - 6;
    Kbd->Size.Y = 98;

    Window1->Internals.FullScreen = false;
    Window1->Position.X = 400;
    Window1->Position.Y = 400;
    Window1->Size.X = 400;
    Window1->Size.Y = 300;

    window_new_button(Window1, Btn2);

    window_new_window(Window1, Window2);

    Window2->Internals.FullScreen = false;
    Window2->Position.X = 10;
    Window2->Position.Y = 50;
    Window2->Size.X = 300;
    Window2->Size.Y = 200;

    window_new_button(Window2, Btn3);

    window_new_tab_group(Window2, TabGroup1);
    //TabGroup1->Internals.FullScreen = false;
    TabGroup1->Position.X = 10;
    TabGroup1->Position.Y = 50;
    TabGroup1->Size.X = 200;
    TabGroup1->Size.Y = 100;

    tab_group_new_tab(TabGroup1, "Tab1");
    tab_group_new_tab(TabGroup1, "Tab2");
    tab_group_new_tab(TabGroup1, "Tab3");
    tab_group_new_tab(TabGroup1, "Tab4");
    tab_group_new_tab(TabGroup1, "Tab5");
    tab_group_new_tab(TabGroup1, "Tab6");
    tab_group_new_tab(TabGroup1, "Tab7");
    tab_group_new_tab(TabGroup1, "Tab8");

    tab_group_new_button(TabGroup1, Btn4, 0);
    tab_group_new_button(TabGroup1, Btn5, 1);
    tab_group_new_button(TabGroup1, Btn6, 2);
    tab_group_new_button(TabGroup1, Btn7, 3);
    tab_group_new_button(TabGroup1, Btn8, 4);
    tab_group_new_button(TabGroup1, Btn9, 5);
    tab_group_new_button(TabGroup1, Btn10, 6);
    tab_group_new_button(TabGroup1, Btn11, 7);

    Btn5->Position.X = 30;

    /* Enable clear background on refresh */
    PictureBox1->PaintBackground = true;
    /* Set callback's  for picture box*/
    PictureBox1->Events.OnMove.CallBack = picture_box_callback;
    PictureBox1->Events.OnDown.CallBack = picture_box_callback_on_down;
    /* Refresh is used to refresh the picture box when the window or picture box position , size is modified */
    PictureBox1->Events.Refresh.CallBack = picture_box_refresh_callback;

    /* Set callback's  for Btn1*/
    Btn1->Events.OnUp.CallbackData = PictureBox1;
    Btn1->Events.OnUp.CallBack = picture_box_clear_callback;

    /* Clear the picture box */
    picture_box_clear_callback(PictureBox1);

    char TmpStr[30];
    unsigned int CntItems = 0;
    for(CntItems = 0; CntItems < 100; CntItems++)
    {
        sprintf(TmpStr, "%d", CntItems);
        listbox_item_add(ListBox1, TmpStr);
    }
    listbox_item_insert(ListBox1, "Inserted Item", 1);
    listbox_item_remove(ListBox1, 3);


    string(TextBoxString, "Multiplatform SDK to create standalone applications\n\r1\n\r2\n\r3\n\r4\n\r5\n\r6\n\r7\n\r8\n\r9\n\r10\n\r11\n\r12\n\r13\n\r14\n\r15\n\r16\n\r17\n\r18");
    TextBox1->text(TextBox1, TextBoxString);
    TextBox1->text(TextBox1, TextBox1->to_uper(TextBox1));


    tControlCommandData control_comand;
    control_comand.Comand = Control_Nop;
    control_comand.CursorCoordonateUsed = true;
#endif
/*******************************************************/
/*arg's for console*/
    char *argv[2];
    argv[0] = NULL;
/*******************************************************/
	bool old_connected = false;
#ifdef BridgeUsbDev0ToMmcSd0
	if(sdCtrl[0].connected == false) usb_msc_dev_media_change_state(0, false);
#elif defined(BridgeUsbDev0ToMmcSd1)
	if(sdCtrl[1].connected == false) usb_msc_dev_media_change_state(0, false);
#endif
/*******************************************************/
    while(1)
    {
        if(timer_tick(&TimerScanTouch))
        {
    		//UARTprintf(DebugCom, "X= %d, Y= %d, But= %d, Whel= %d.\n\r" , MouseXPosition, MouseYPosition, g_ulButtons, MouseWheel);
#ifdef lcd
#ifdef USE_BACK_SCREEN
            if(BackScreen)
#else
            if(ScreenBuff)
#endif
            {
				memset(&control_comand, 0, sizeof(tControlCommandData));
#ifdef touch
				if(TouchScreen->TouchScreen_Type == TouchScreen_Type_Int) TouchIdle(TouchScreen);
				else if(TouchScreen->TouchScreen_Type == TouchScreen_Type_FT5x06) ft5x06_TouchIdle(TouchScreen);
				control_comand.X = TouchScreen->TouchResponse.x1;
				control_comand.Y = TouchScreen->TouchResponse.y1;
				control_comand.Cursor = (CursorState)TouchScreen->TouchResponse.touch_event1;
#endif
#ifdef usb_1_mouse
	            usb_mouse_host_idle(1, &control_comand);
#elif defined(usb_1_msc)
	            usb_msc_host_idle(1);
#endif
                HeaderWindow->idle(HeaderWindow, &control_comand);
                MainWindow->idle(MainWindow, &control_comand);
                KbdWindow->idle(KbdWindow, &control_comand);
#ifdef USE_BACK_SCREEN
                if(control_comand.WindowRefresh) ScreenReRefreshCnt = 2;
                if(ScreenReRefreshCnt)
                {
                    ScreenReRefreshCnt--;
                    screen_copy(ScreenBuff, BackScreen, true, control_comand.X, control_comand.Y, 0x00000000);
                    //put_rectangle(ScreenBuff, control_comand.X, control_comand.Y, 2, 2, true, 0x00000000);
                    //box_cache_clean(ScreenBuff, control_comand.X, control_comand.Y, 2, 2);
                }
#else
                put_rectangle(ScreenBuff, control_comand.X, control_comand.Y, 2, 2, true, 0x00000000);
                box_cache_clean(ScreenBuff, control_comand.X, control_comand.Y, 2, 2);
#endif
            }
#endif
#ifdef BridgeUsbDev0ToMmcSd0
        mmcsd_idle(&sdCtrl[0]);
        if(old_connected == false && sdCtrl[0].connected == true)
        {
        	old_connected = true;
        	usb_msc_dev_media_change_state(0, true);
        }
        else if(old_connected == true && sdCtrl[0].connected == false)
        {
        	old_connected = false;
        	usb_msc_dev_media_change_state(0, false);
        }

#elif defined(BridgeUsbDev0ToMmcSd1)
        mmcsd_idle(&sdCtrl[1]);
        if(old_connected == false && sdCtrl[1].connected == true)
        {
        	old_connected = true;
        	usb_msc_dev_media_change_state(1, true);
        }
        else if(old_connected == true && sdCtrl[1].connected == false)
        {
        	old_connected = false;
        	usb_msc_dev_media_change_state(1, false);
        }
#endif
        }
        signed int R_Chr = UARTGetcNoBlocking(DebugCom);
        argv[1] = (char *)R_Chr;
        console(2, argv);
    }
}
#endif


