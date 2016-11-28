/*
 * main_window_template.c
 *
 *  Created on: Aug 13, 2016
 *      Author: John Smith
 */

#include "main_window_template.h"
#include "lib/string_lib.h"
#include "main.h"

void main_window_template_init(tWindow *MainWindow)
{
    /*MainWindow->AllowHScroll = false;
    MainWindow->AllowVScroll = false;
    MainWindow->HideHScroll = true;
    MainWindow->HideVScroll = true;
    MainWindow->MaxMinEnabled = false;
    MainWindow->MinimizeButonEnabled = false;
    MainWindow->CloseButonEnabled = false;
    //MainWindow->MaxMinVisible = false;
    //MainWindow->HideHeader = true;
    window_new_listbox(MainWindow, DevicesList);
    DevicesList->Position.X = 3;
    DevicesList->Position.Y = 3;
    DevicesList->Size.X = MainWindow->Internals.pDisplay->LcdTimings->X - 6;
    DevicesList->Size.Y = MainWindow->Internals.pDisplay->LcdTimings->Y - 135;
    DevicesList->Size.ScrollSize = 50;
    DevicesList->Size.ItemSizeY = 30;
    DevicesList->Size.MinScrollBtnSize = 30;
    unsigned int cnt = 0;
    char listbox_buff[32];
    for(; cnt < 256; cnt++)
    {
		strcpy((char *)listbox_buff, "Device ID ");
		char buff_tmp[10];
		utoa(cnt, buff_tmp, 10);
		strcat(listbox_buff, buff_tmp);
		strcat(listbox_buff, "\n\r");
    	DevicesList->Item.add(DevicesList, listbox_buff);
    }
    window_new_button(MainWindow, BtnStartTest);
    BtnStartTest->Position.Y = DevicesList->Position.Y + DevicesList->Size.Y + 5;
    BtnStartTest->Size.X = 90;
    BtnStartTest->Size.Y = 50;
    window_new_button(MainWindow, BtnStartDownload);
    BtnStartDownload->Position.X = 95;
    BtnStartDownload->Position.Y = DevicesList->Position.Y + DevicesList->Size.Y + 5;
    BtnStartDownload->Size.X = 130;
    BtnStartDownload->Size.Y = 50;
    BtnStartDownload->Caption.Text = String.Set(BtnStartDownload->Caption.Text, "Download");
    BtnStartDownload->Events.OnUp.CallBack = download_func;
    window_new_progressbar(MainWindow, ProgressDownload);
    ProgressDownload->Position.X = 5;
    ProgressDownload->Position.Y = DevicesList->Position.Y + DevicesList->Size.Y + 5 + BtnStartDownload->Size.Y + 5;
    ProgressDownload->Size.X = 230;
    ProgressDownload->Size.Y = 30;
    window_new_checkbox(MainWindow, CB1);
    CB1->Position.Y = BtnStartDownload->Position.Y + BtnStartDownload->Size.Y + 5;
    CB1->Size.X = 130;
    CB1->Size.Y = 50;*/
    //CB1->Enabled = false;*/


}
