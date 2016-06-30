/*
 * main.c
 */

#include <stdlib.h>
#include "main.h"
#include "board_init.h"
#include "api/timer_api.h"
#include "api/usb_api.h"
#include "device/mpu60x0_9150.h"
#include "device/ak8975.h"
#include "device/bmp180.h"
#include "device/sht11.h"
#include "device/srf02.h"
#include "device/mhc5883.h"
#include "device/ms5611.h"
#include "device/adxl345.h"
#include "device/hih6130.h"
#include "device/mpl3115a2.h"
#include "device/mpr121.h"
#include "device/lepton_flir.h"


#include "interface/hs_mmcsd_interface.h"


#include "lib/gfx/controls_definition.h"
#include "lib/gfx/button.h"
#include "lib/gfx/checkbox.h"
#include "lib/gfx/progressbar.h"
#include "lib/gfx/scrollbar.h"
#include "lib/gfx/textbox.h"
#include "lib/gfx/listbox.h"
#include "lib/gfx/window.h"
#include "lib/gfx/keyboard.h"
#include "lib/gfx/picturebox.h"

#include "lib/string_lib.h"

#include "lib/gfx/resource/fonts.h"

_new_window(HeaderWindow);
_new_window(MainWindow);
_new_window(KbdWindow);

#ifdef USE_BACK_SCREEN
tDisplay *BackScreen = NULL;
volatile unsigned char ScreenReRefreshCnt = 0;
#endif


//#include "interface/lwip/lwip_hardware_init.h"
//#include "lib/lwip/simple_fs_httpd.h"

timer(timer_led);
timer(TimerScanTouch);
char led_state = 0;

signed int picture_old_x = 0;
signed int picture_old_y = 0;

unsigned int PictureBoxbackBuff[128*96];

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
//#include "lib/gfx/bmp/test_bmp.h"
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
	//picturebox_put_fbitmap(settings, "0:test.bmp", 0, 0, false, false);
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
	//picturebox_put_fbitmap(settings, "0:test.bmp", 0, 0, false, false);
	return NULL;
}

void heart_beat_service(void) {
	if(timer_tick(&timer_led)) {
		switch(led_state) {
			case 0:
				led_state = 1;
				gpio_out(LED[0], 1);
				timer_interval(&timer_led, 10);
				break;
			case 1:
				led_state = 2;
				gpio_out(LED[0], 0);
				timer_interval(&timer_led, 200);
				break;
			case 2:
				led_state = 3;
				gpio_out(LED[0], 1);
				timer_interval(&timer_led, 10);
				break;
			case 3:
				led_state = 0;
				gpio_out(LED[0], 0);
				timer_interval(&timer_led, 780);
				break;
		}
	}
}
//..\..\emulation\boards\sk_am3358\gel\AM3358_StarterKit.gel
int main(void)
{
	board_init();
	timer(TimerReadSensors);
    timer_interval(&TimerReadSensors, 300);
    timer_interval(&TimerScanTouch, 10);





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
    //PictureBox1->Events.Refresh.CallBack = picture_box_refresh_callback;

    /* Set callback's  for Btn1*/
    Btn1->Events.OnUp.CallbackData = PictureBox1;
    Btn1->Events.OnUp.CallBack = picture_box_clear_callback;

    /* Clear the picture box */
    //picture_box_clear_callback(PictureBox1);

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
#if _USE_SHT11 == 1
	unsigned char sht11_status_reg = 0;
	bool sht11_read_mode = false;
#endif
#if _USE_LEPTON_FLIR == 1
	unsigned short flir_buff[((LEPTON_FLIR_LINE_SIZE / 2) - 2) * LEPTON_FLIR_LINES_NR];
#endif
    //lan_interface_init();
	//httpd_init();
/*******************************************************/
	bool old_connected = false;
#ifdef BridgeUsbDev0ToMmcSd0
	if(ctrlInfo[0].connected == false) usb_msc_dev_media_change_state(0, false);
#elif defined(BridgeUsbDev0ToMmcSd1)
	if(ctrlInfo[1].connected == false) usb_msc_dev_media_change_state(0, false);
#endif
/*******************************************************/
	unsigned int PwrLoadCount = 0;
	while(1)
	{
        if(timer_tick(&TimerScanTouch))
        {
			if(PwrLoadCount) PwrLoadCount--;
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
				USB_MOUSE_IDLE(1, &control_comand);
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
            } else PwrLoadCount++;

#endif
        }
#ifdef BridgeUsbDev0ToMmcSd0
        mmcsd_idle(0);
        if(old_connected == false && ctrlInfo[0].connected == true)
        {
        	old_connected = true;
        	usb_msc_dev_media_change_state(0, true);
        }
        else if(old_connected == true && ctrlInfo[0].connected == false)
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
		//lwip_idle();
		heart_beat_service();
#if _USE_SHT11 == 1
		if(sht11_read_mode) {
			/*
			 * Send start temperature measure
			 * The sht11_write function will send the command if internal flag 'busy' is false and return true.
			 * If flag busy is true will return true.
			 */
			if(sht11_write(SHT11, SHT11_START_MEASURE_TEMPERATURE, &sht11_status_reg)) {
				/*
				 * Pool SGT11 and read data if is ready
				 * If read is complete will return true, else will return false.
				 * When read is complete the busy flag will be set to false, this will allow
				 * sht11_write to send another command if is called.
				 */
				if(sht11_read(SHT11))
					/*
					 * Read complete.
					 * Now you can send another command.
					 */
					sht11_read_mode = false;
			}
		} else {
			/*
			 * Send start humidity measure
			 * The sht11_write function will send the command if internal flag 'busy' is false and return true.
			 * If flag busy is true will return true.
			 */
			if(sht11_write(SHT11, SHT11_START_MEASURE_HUMIDITY, &sht11_status_reg)) {
				/*
				 * Pool SGT11 and read data if is ready
				 * If read is complete will return true, else will return false.
				 * When read is complete the busy flag will be set to false, this will allow
				 * sht11_write to send another command if is called.
				 */
				if(sht11_read(SHT11))
					/*
					 * Read complete.
					 * Now you can send another command.
					 */
					sht11_read_mode = true;
			}
		}
#endif
#if _USE_SRF02 == 1
		/* Send start ranging */
		if(srf02_start(SRF02, SRF02_START_RANGING))
			/*
			 * Pool SRF02 and read data if is ready
			 * If data is not ready, this function will not freeze,
			 * If data ready will return true, if not wil return false.
			 */
			srf02_read(SRF02);
#endif
		if(timer_tick(&TimerReadSensors)) {
			//Time_Update();
#if _USE_MS5611 == 1
			ms5611_display_pressure_result(MS5611, MS5611_CONVERT_OSR_1024);
#endif
#if _USE_MHC5883 == 1
			mhc5883_display_result(MHC5883);
#endif
#if _USE_SHT11 == 1
			sht11_display_data(SHT11);
#endif
#if _USE_SRF02 == 1
			srf02_display_data(SRF02);
#endif
#if _USE_ADXL345 == 1
			signed int Xaccel = 0, Yaccel = 0, Zaccel = 0;
			if(adxl345_x_read(ADXL345, &Xaccel) &&
					adxl345_y_read(ADXL345, &Yaccel) &&
						adxl345_z_read(ADXL345, &Zaccel))
				UARTprintf(DebugCom, "ADXL345: X = %d, Y = %d, Z = %d\n\r", Xaccel, Yaccel, Zaccel);
#endif
#if _USE_BMP180 == 1
			//bmp180_display_result(BMP180, BMP180_CTRL_MEAS_OSS_1);
			float bmp180_temperature = 0.0;
			float bmp180_pressure = 0.0;
			float bmp180_altitude = 0.0;
			bool bmp180_stat = false;
			if(bmp180_get_temp(BMP180, &bmp180_temperature) &&
					bmp180_get_pressure(BMP180, &bmp180_pressure, BMP180_CTRL_MEAS_OSS_8) &&
						bmp180_get_altitude(BMP180, &bmp180_altitude, BMP180_CTRL_MEAS_OSS_8)) {
				bmp180_stat = true;
			}
#endif
#if _USE_MPU60x0_9150 == 1
			float mpu60x0_9150_temp = 0.0;
			bool mpu60x0_9150_temp_stat = mpu60x0_9150_temp_data_get(MPU60x0_9150, &mpu60x0_9150_temp);
			//mpu60x0_9150_temperature_display_result(MPU60x0_9150);
			signed short mpu60x0_9150_gyro_Xg = 0;
			signed short mpu60x0_9150_gyro_Yg = 0;
			signed short mpu60x0_9150_gyro_Zg = 0;
			bool mpu60x0_9150_gyro_stat = mpu60x0_9150_gyro_data_get(MPU60x0_9150, &mpu60x0_9150_gyro_Xg, &mpu60x0_9150_gyro_Yg, &mpu60x0_9150_gyro_Zg);
			//mpu60x0_9150_giroscope_display_result(MPU60x0_9150);
			signed short mpu60x0_9150_accel_Xa = 0;
			signed short mpu60x0_9150_accel_Ya = 0;
			signed short mpu60x0_9150_accel_Za = 0;
			bool mpu60x0_9150_accel_stat = mpu60x0_9150_accel_data_get(MPU60x0_9150, &mpu60x0_9150_accel_Xa, &mpu60x0_9150_accel_Ya, &mpu60x0_9150_accel_Za);
			//mpu60x0_9150_accelerometer_display_result(MPU60x0_9150);
#endif
#if _USE_AK8975 == 1
			//ak8975_display_result(AK8975);
			signed short AK8975_X_Axis = 0, AK8975_Y_Axis = 0, AK8975_Z_Axis = 0;
			bool ak8975_stat = ak8975_get_mag(AK8975, &AK8975_X_Axis, &AK8975_Y_Axis, &AK8975_Z_Axis);
#endif
#if _USE_HIH613x == 1
			unsigned char hih613x_status = 0;
			float hih613x_hum = 0, hih613x_temp = 0;
			if(!hih613x_get_hum_temp(HIH613x, &hih613x_status, &hih613x_hum, &hih613x_temp)) {
				hih613x_status = (unsigned char)-1;
			}
#endif
#if _USE_MPL3115A2 == 1
			float mpl3115a2_altitude = 0.0, mpl3115a2_temp = 0.0;
			bool mpl3115a2_status = mpl3115a2_get_alt_temp(MPL3115A2, &mpl3115a2_altitude, &mpl3115a2_temp);
#endif
#if _USE_LEPTON_FLIR == 1
			memset(flir_buff, 0, ((LEPTON_FLIR_LINE_SIZE - 4) * LEPTON_FLIR_LINES_NR));
			lepton_flir_get_image(LEPTON_FLIR, flir_buff);
#endif
/*
 * Display results.
 */
#if _USE_INT_ADC == 1
			UARTprintf(DebugCom, "ADC1: CH0 = %d, CH1 = %d, TempSensor = %2.2f\n\r\n\r", ADC[0]->ConvResult[0], ADC[0]->ConvResult[1], (float)(((float)1775 - (float)ADC[0]->ConvResult[2]) / 5.337) + (float)25);
#endif
#if _USE_BMP180 == 1
			if(bmp180_stat) {
				UARTprintf(DebugCom, "BMP180: T = %2.1f, P = %4.2f, Alt = %4.2f\n\r", bmp180_temperature, bmp180_pressure, bmp180_altitude);
			}
#endif
#if _USE_MPL3115A2 == 1
			if(mpl3115a2_status) {
				UARTprintf(DebugCom, "MPL3115A1: T = %3.2f, P = %3.2f, Alt = %4.2f\n\r", mpl3115a2_temp, 0.0, mpl3115a2_altitude);
			}
#endif
#if _USE_HIH613x == 1
			switch(hih613x_status)
			{
			case 0:
				UARTprintf(DebugCom, "HIH613x: T = %2.2f, H = %2.1f\n\r", hih613x_temp, hih613x_hum);
				break;
			case 1:
				UARTprintf(DebugCom, "Stale Data\n\r");
				break;
			case 2:
				UARTprintf(DebugCom, "In command mode\n\r");
				break;
			case 3:
				UARTprintf(DebugCom, "Diagnostic\n\r");
				break;
			}
#endif
#if _USE_MPU60x0_9150 == 1
			if(mpu60x0_9150_temp_stat) {
#ifndef _TINY_PRINT_
				UARTprintf(DebugCom, "MPU60x0: Temperature: %2.2f Gr Celsius\n\r", mpu60x0_9150_temp);
#else
				float GrCelsius = 0;
				float GrCelsiusMod = modff(mpu60x0_9150_temp, &GrCelsius);
				UARTprintf(DebugCom, "MPU60x0: Temperature: %d.%u Gr Celsius\n\r", (unsigned int)GrCelsius, (unsigned int)(GrCelsiusMod*1000));
#endif
			}
			if(mpu60x0_9150_gyro_stat) {
				UARTprintf(DebugCom, "MPU60x0: Giroscope: Xg = %d, Yg = %d, Zg = %d\n\r", mpu60x0_9150_gyro_Xg, mpu60x0_9150_gyro_Yg, mpu60x0_9150_gyro_Zg);
			}
			if(mpu60x0_9150_accel_stat) {
				UARTprintf(DebugCom, "MPU60x0: Accelerometer: Xa = %d, Ya = %d, Za = %d\n\r", mpu60x0_9150_accel_Xa, mpu60x0_9150_accel_Ya, mpu60x0_9150_accel_Za);
			}
#endif
#if _USE_AK8975 == 1
			if(ak8975_stat) {
				UARTprintf(DebugCom, "AK8975: Magnetometer: Xg = %d, Yg = %d, Zg = %d\n\r", AK8975_X_Axis, AK8975_Y_Axis, AK8975_Z_Axis);
			}
#endif
#if _USE_MPR121 == 1
			mpr121_ret_t mpr121_return;
			if(mpr121_idle(MPR121, &mpr121_return))
			{
				if(mpr121_return.pushed)
					UARTprintf(DebugCom, "MPR121: Pushed   > K0=%c, K1=%c, K2=%c, K3=%c, K4=%c, K5=%c, K6=%c, K7=%c, K8=%c, K9=%c, K10=%c, K11=%c\n\r",     (unsigned char)mpr121_return.pushed & 0x01,   (unsigned char)(mpr121_return.pushed >> 1) & 0x01,   (unsigned char)(mpr121_return.pushed >> 2) & 0x01,   (unsigned char)(mpr121_return.pushed >> 3) & 0x01,   (unsigned char)(mpr121_return.pushed >> 4) & 0x01,   (unsigned char)(mpr121_return.pushed >> 5) & 0x01,   (unsigned char)(mpr121_return.pushed >> 6) & 0x01,   (unsigned char)(mpr121_return.pushed >> 7) & 0x01,   (unsigned char)(mpr121_return.pushed >> 8) & 0x01,   (unsigned char)(mpr121_return.pushed >> 9) & 0x01,   (unsigned char)(mpr121_return.pushed >> 10) & 0x01,   (unsigned char)(mpr121_return.pushed >> 11) & 0x01);
				if(mpr121_return.released)
					UARTprintf(DebugCom, "MPR121: Released > K0=%c, K1=%c, K2=%c, K3=%c, K4=%c, K5=%c, K6=%c, K7=%c, K8=%c, K9=%c, K10=%c, K11=%c\n\r\n\r", (unsigned char)mpr121_return.released & 0x01, (unsigned char)(mpr121_return.released >> 1) & 0x01, (unsigned char)(mpr121_return.released >> 2) & 0x01, (unsigned char)(mpr121_return.released >> 3) & 0x01, (unsigned char)(mpr121_return.released >> 4) & 0x01, (unsigned char)(mpr121_return.released >> 5) & 0x01, (unsigned char)(mpr121_return.released >> 6) & 0x01, (unsigned char)(mpr121_return.released >> 7) & 0x01, (unsigned char)(mpr121_return.released >> 8) & 0x01, (unsigned char)(mpr121_return.released >> 9) & 0x01, (unsigned char)(mpr121_return.released >> 10) & 0x01, (unsigned char)(mpr121_return.released >> 11) & 0x01);
			}
#endif
#if _USE_LEPTON_FLIR == 1
			UARTPuts(DebugCom, (char *)flir_buff, ((LEPTON_FLIR_LINE_SIZE - 4) * LEPTON_FLIR_LINES_NR));
#endif
		}
	}
}
