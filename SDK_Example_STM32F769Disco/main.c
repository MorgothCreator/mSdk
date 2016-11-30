/*
 * main.c
 *
 *  Created on: Nov 25, 2016
 *      Author: John Smith
 */

#include <stdlib.h>
#include <stdarg.h>
#include "math.h"
#include "main.h"
#include "board_init.h"
#include "api/timer_api.h"
#include "api/usb_api.h"
#include "api/adc_def.h"
#include "api/adc_api.h"
#include "api/rtc_def.h"
#include "api/rtc_api.h"
#include "device/ft5x06.h"
#include "device/mmcsd_spi.h"
#include "device/adxl345.h"
#include "device/lepton_flir.h"
#include "driver/stm32f7xx_hal_adc.h"
#include "driver/stm32f7xx_hal_dma.h"

#include "lib/gfx/window.h"
#include "lib/gfx/window_def.h"
#include "lib/gfx/PasswordWindowNumeric.h"
#include "lib/util/hex_string.h"
#include "lib/generic.h"
#include "lib/fat_fs/inc/ff_util.h"
#include "lib/crypto/aes128.h"
#include "lib/string_lib.h"

#include "device/microe_touch.h"


#include "driver/stm32f7xx_hal_dsi.h"
tWindow *MainWindow = NULL;
tWindow *PlayerWindow = NULL;
//extern ADC_HandleTypeDef    AdcHandle[];
FIL fs_file_data;
timer(TimerDisplayLight);
bool fs_mounted = false;

//tControlCommandData FlirPictureBox_Cursor_Global;
//tPictureBox *FlirPictureBox_Global;
//char main_window_name[] = "Haxapod main_window-";
String_t *main_window_str;
String_t *card_status_str;

char terminal_receive_buff[128];

#if _USE_LEPTON_FLIR == 1
unsigned short flir_buff[LEPTON_FLIR_LINE_SIZE * LEPTON_FLIR_LINES_NR] __attribute__ ((aligned (4)));
unsigned int flir_buff_translated[LEPTON_FLIR_LINE_SIZE * LEPTON_FLIR_LINES_NR] __attribute__ ((aligned (4)));
#endif

timer(timer_led);
char led_state = 0;

void heart_beat_service(void) {
	if(timer_tick(&timer_led)) {
		switch(led_state) {
			case 0:
				led_state = 1;
				gpio.out(LED[0], 1);
				timer_interval(&timer_led, 10);
				break;
			case 1:
				led_state = 2;
				gpio.out(LED[0], 0);
				timer_interval(&timer_led, 200);
				break;
			case 2:
				led_state = 3;
				gpio.out(LED[0], 1);
				timer_interval(&timer_led, 10);
				break;
			case 3:
				led_state = 0;
				gpio.out(LED[0], 0);
				timer_interval(&timer_led, 780);
				break;
		}
	}
}


void update_main_window_status_bar(String_t *Windowstring)
{
	String.Clone(Windowstring, main_window_str);
	String.Append(Windowstring, card_status_str);
}

void display_light_enable()
{
	timer_enable(&TimerDisplayLight);
	gpio.out(LCD_BACKLIGHT, 1);
}

#if _USE_LEPTON_FLIR == 1
void *FlirPictureBox_Rfsh_Callback(tPictureBox *structure, tControlCommandData *cursor_ctrl)
{
	tRectangle _dest_rectangle;
	_dest_rectangle.sXMin = 0;
	_dest_rectangle.sXMax = LEPTON_FLIR_LINE_SIZE;
	_dest_rectangle.sYMin = 0;
	_dest_rectangle.sYMax = LEPTON_FLIR_LINES_NR;
	tRectangle _src_rectangle;
	_src_rectangle.sXMin = 0;
	_src_rectangle.sXMax = LEPTON_FLIR_LINE_SIZE;
	_src_rectangle.sYMin = 0;
	_src_rectangle.sYMax = LEPTON_FLIR_LINES_NR;
	structure->copy_rectangle(structure, flir_buff_translated, 0, &_dest_rectangle, &_src_rectangle, LEPTON_FLIR_LINE_SIZE, LEPTON_FLIR_LINES_NR);
	return NULL;
}
#endif

tTextBox *SensorResultTextboxGlobal;;

void read_and_display_all_sensors()
{
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
#if _USE_MS5611 == 1
		signed int Pressure = 0;
		signed int Temperature = 0;
		bool ms5611_read_status = true;
		//if(!ms5611_read_prom_cmd_send(prom_data, TwiStruct)) return false;
		if(!ms5611_read(MS5611, MS5611_CONVERT_OSR_1024, &Pressure, &Temperature))
			ms5611_read_status = false;
			//ms5611_display_pressure_result(MS5611, MS5611_CONVERT_OSR_1024);
#endif
#if _USE_MHC5883 == 1
		signed short Xc = 0;
		signed short Yc = 0;
		signed short Zc = 0;
		bool mhc5883_stat = mhc5883_data_get(MHC5883, &Xc, &Yc, &Zc);
#endif
#if _USE_SHT11 == 1
			sht11_display_data(SHT11);
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
			float mpl3115a2_pression = 0.0;
			float mpl3115a2_altitude = 0.0;
			float mpl3115a2_temp = 0.0;
			bool mpl3115a2_status = mpl3115a2_get_alt_temp(MPL3115A2, 2, &mpl3115a2_pression, &mpl3115a2_altitude, &mpl3115a2_temp);
#endif
/*
 * Display results.
 */
			String.SetF(SensorResultTextboxGlobal->Text, "");
#if _USE_INT_ADC == 1
			UARTprintf(DebugCom, "ADC1: CH0 = %d, CH1 = %d, TempSensor = %2.2f\n\r\n\r", ADC[0]->ConvResult[0], ADC[0]->ConvResult[1], (float)(((float)1775 - (float)ADC[0]->ConvResult[2]) / 5.337) + (float)25);
#endif
#if _USE_SRF02 == 1
			srf02_display_data(SRF02);
			switch(SRF02->measure_unit)
			{
			case SRF02_INCH:
				String.AppendF(SensorResultTextboxGlobal->Text, "SRF02: Range = %u inch\n\r", (unsigned long)SRF02->range_value);
				break;
			case SRF02_CENTIMETER:
				String.AppendF(SensorResultTextboxGlobal->Text, "SRF02: Range = %u centimeters\n\r", (unsigned long)SRF02->range_value);
				break;
			case SRF02_MICROSECONDS:
				String.AppendF(SensorResultTextboxGlobal->Text, "SRF02: Range = %u microseconds\n\r", (unsigned long)SRF02->range_value);
				break;
			}
#endif
#if _USE_BMP180 == 1
			if(bmp180_stat) {
				String.AppendF(SensorResultTextboxGlobal->Text, "BMP180: T = %2.1f, P = %4.2f, Alt = %4.2f\n\r", bmp180_temperature, bmp180_pressure, bmp180_altitude);
			}
#endif
#if _USE_MPL3115A2 == 1
			if(mpl3115a2_status) {
				String.AppendF(SensorResultTextboxGlobal->Text, "MPL3115A1: T = %3.3f, P = %3.5f, Alt = %4.3f\n\r", mpl3115a2_temp, mpl3115a2_pression, mpl3115a2_altitude);
			}
#endif
			#if _USE_HIH613x == 1
			switch(hih613x_status)
			{
			case 0:
				String.AppendF(SensorResultTextboxGlobal->Text, "HIH613x: T = %2.2f, H = %2.1f\n\r", hih613x_temp, hih613x_hum);
				break;
			case 1:
				String.AppendF(SensorResultTextboxGlobal->Text, "Stale Data\n\r");
							break;
			case 2:
				String.AppendF(SensorResultTextboxGlobal->Text, "In command mode\n\r");
				break;
			case 3:
				String.AppendF(SensorResultTextboxGlobal->Text, "Diagnostic\n\r");
				break;
			}
#endif
#if _USE_MPU60x0_9150 == 1
			if(mpu60x0_9150_temp_stat) {
#ifndef _TINY_PRINT_
				String.AppendF(SensorResultTextboxGlobal->Text, "MPU60x0: Temperature: %2.2f Gr Celsius\n\r", mpu60x0_9150_temp);
#else
				float GrCelsius = 0;
				float GrCelsiusMod = modff(mpu60x0_9150_temp, &GrCelsius);
				String.AppendF(SensorResultTextboxGlobal->Text, "MPU60x0: Temperature: %d.%u Gr Celsius\n\r", (unsigned int)GrCelsius, (unsigned int)(GrCelsiusMod*1000));
#endif
			}
			if(mpu60x0_9150_gyro_stat) {
				String.AppendF(SensorResultTextboxGlobal->Text, "MPU60x0: Giroscope: Xg = %6d, Yg = %6d, Zg = %6d\n\r", mpu60x0_9150_gyro_Xg, mpu60x0_9150_gyro_Yg, mpu60x0_9150_gyro_Zg);
			}
			if(mpu60x0_9150_accel_stat) {
				String.AppendF(SensorResultTextboxGlobal->Text, "MPU60x0: Accelerometer: Xa = %6d, Ya = %6d, Za = %6d\n\r", mpu60x0_9150_accel_Xa, mpu60x0_9150_accel_Ya, mpu60x0_9150_accel_Za);
			}
#endif
#if _USE_AK8975 == 1
			if(ak8975_stat) {
				String.AppendF(SensorResultTextboxGlobal->Text, "AK8975: Magnetometer: Xm = %6d, Ym = %6d, Zm = %6d\n\r", AK8975_X_Axis, AK8975_Y_Axis, AK8975_Z_Axis);
			}
#endif
#if _USE_MS5611 == 1
		//if(!ms5611_read_prom_cmd_send(prom_data, TwiStruct)) return false;
		if(ms5611_read_status)
		{
			float PressureInt = 0;
			float PressureDec = modff(((float)Pressure)/100.0, &PressureInt);
			float TemperatureInt = (float)Temperature;
			float TemperatureDec = modff(((float)Pressure)/100.0, &TemperatureInt);
			String.AppendF(SensorResultTextboxGlobal->Text, "MS5611: P = %d.%u milibar, T = %d.%u gr celsius\n\r", (signed int)PressureInt, (unsigned int)(PressureDec * 100.0), (signed int)TemperatureInt, (unsigned int)(TemperatureDec * 100.0));
		}
#endif
#if _USE_MHC5883 == 1
		if(mhc5883_stat)
		{
			String.AppendF(SensorResultTextboxGlobal->Text, "MHC5883: Compass: Xc = %d, Yc = %d, Zc = %d\n\r", Xc, Yc, Zc);
		}
#endif
#if _USE_MPR121 == 1
			mpr121_ret_t mpr121_return;
			if(mpr121_idle(MPR121, &mpr121_return))
			{
				if(mpr121_return.pushed)
					String.AppendF(SensorResultTextboxGlobal->Text, "MPR121: Pushed   - K0=%u, K1=%u, K2=%u, K3=%u, K4=%u, K5=%u, K6=%u, K7=%u, K8=%u, K9=%u, K10=%u, K11=%u\n\r",     (unsigned int)mpr121_return.pushed & 0x01,   (unsigned int)(mpr121_return.pushed >> 1) & 0x01,   (unsigned int)(mpr121_return.pushed >> 2) & 0x01,   (unsigned int)(mpr121_return.pushed >> 3) & 0x01,   (unsigned int)(mpr121_return.pushed >> 4) & 0x01,   (unsigned int)(mpr121_return.pushed >> 5) & 0x01,   (unsigned int)(mpr121_return.pushed >> 6) & 0x01,   (unsigned int)(mpr121_return.pushed >> 7) & 0x01,   (unsigned int)(mpr121_return.pushed >> 8) & 0x01,   (unsigned int)(mpr121_return.pushed >> 9) & 0x01,   (unsigned int)(mpr121_return.pushed >> 10) & 0x01,   (unsigned int)(mpr121_return.pushed >> 11) & 0x01);
				if(mpr121_return.released)
					String.AppendF(SensorResultTextboxGlobal->Text, "MPR121: Released - K0=%u, K1=%u, K2=%u, K3=%u, K4=%u, K5=%u, K6=%u, K7=%u, K8=%u, K9=%u, K10=%u, K11=%u\n\r\n\r", (unsigned int)mpr121_return.released & 0x01, (unsigned int)(mpr121_return.released >> 1) & 0x01, (unsigned int)(mpr121_return.released >> 2) & 0x01, (unsigned int)(mpr121_return.released >> 3) & 0x01, (unsigned int)(mpr121_return.released >> 4) & 0x01, (unsigned int)(mpr121_return.released >> 5) & 0x01, (unsigned int)(mpr121_return.released >> 6) & 0x01, (unsigned int)(mpr121_return.released >> 7) & 0x01, (unsigned int)(mpr121_return.released >> 8) & 0x01, (unsigned int)(mpr121_return.released >> 9) & 0x01, (unsigned int)(mpr121_return.released >> 10) & 0x01, (unsigned int)(mpr121_return.released >> 11) & 0x01);
			}
#endif
#if _USE_LEPTON_FLIR == 1
		if(timer_tick(&TimerReadFlir))
		{
			memset(&flir_buff, 0, (LEPTON_FLIR_LINE_SIZE * LEPTON_FLIR_LINES_NR));
			bool lepton_new_data = lepton_flir_get_image(LEPTON_FLIR, flir_buff);
			if(lepton_new_data)
			{
				unsigned int y_cnt = 0;
				for(; y_cnt < LEPTON_FLIR_LINES_NR; y_cnt++)
				{
					unsigned int x_cnt = 0;
					for(; x_cnt < LEPTON_FLIR_LINE_SIZE; x_cnt++)
					{
						unsigned char tmp = flir_buff[(y_cnt * LEPTON_FLIR_LINE_SIZE) + x_cnt] >> 8;
						flir_buff_translated[(y_cnt * LEPTON_FLIR_LINE_SIZE) + x_cnt] = (tmp | (tmp << 8) | (tmp << 16)) | 0xFF000000;
					}
				}


				tRectangle _dest_rectangle;
				_dest_rectangle.sXMin = 0;
				_dest_rectangle.sXMax = LEPTON_FLIR_LINE_SIZE;
				_dest_rectangle.sYMin = 0;
				_dest_rectangle.sYMax = LEPTON_FLIR_LINES_NR;
				tRectangle _src_rectangle;
				_src_rectangle.sXMin = 0;
				_src_rectangle.sXMax = LEPTON_FLIR_LINE_SIZE;
				_src_rectangle.sYMin = 0;
				_src_rectangle.sYMax = LEPTON_FLIR_LINES_NR;
				FlirPictureBox->copy_rectangle(FlirPictureBox, flir_buff_translated, 0, &_dest_rectangle, &_src_rectangle, LEPTON_FLIR_LINE_SIZE , LEPTON_FLIR_LINES_NR);

			}
		}
#endif
}

int main(void)
{
	board_init();
	timer(TimerReadSensors);
    timer_interval(&TimerReadSensors, 2);
	timer(TimerReadBattery);
    timer_interval(&TimerReadBattery, 500);
    MainWindow = new_window(NULL, Screen);
    timer_interval(&TimerDisplayLight, 10000);
#if _USE_LEPTON_FLIR == 1
	timer(TimerReadFlir);
    timer_interval(&TimerReadFlir, 300);
#endif
    //timer(TimerOutFromReceiveRoutine);
    //timer_interval(&TimerOutFromReceiveRoutine, 100);
    //main_window_template_init(MainWindow);
	int minimum_battery_level = to_percentage(2427, 3109, 100, _ADC[0]->ConvResult[2]);

	string(main_window_str_tmp,"Haxapod main window");
	main_window_str = &main_window_str_tmp;
	string(card_status_str_tmp,"");
	card_status_str = &card_status_str_tmp;

	/******************************* Main window template constructor ****************************************/
    MainWindow->AllowHScroll = false;
    MainWindow->AllowVScroll = false;
    MainWindow->HideHScroll = true;
    MainWindow->HideVScroll = true;
    MainWindow->MaxMinEnabled = false;
    MainWindow->MinimizeButonEnabled = false;
    MainWindow->CloseButonEnabled = false;
    MainWindow->Caption.TextAlign = Align_Left;
	update_main_window_status_bar(MainWindow->Caption.Text);
	//String.SetF(MainWindow->Caption.Text, "DLDR FOIL Gr %u,Bat:%u%%", group_number, minimum_battery_level);
    //MainWindow->MaxMinVisible = false;
    //MainWindow->HideHeader = true;
	/*
	 * Create picturebox for flir.
	 */
	window_new_picturebox(MainWindow, FlirPictureBox);
	//FlirPictureBox_Global = FlirPictureBox;
	FlirPictureBox->Position.X = 10;
	FlirPictureBox->Position.Y = 10;
	FlirPictureBox->Size.X = 460;
	FlirPictureBox->Size.Y = 460;
	FlirPictureBox->BackgroundColor = 0xFFFFFFFF;
#if _USE_LEPTON_FLIR == 1
	FlirPictureBox->Events.Refresh.CallBack = FlirPictureBox_Rfsh_Callback;
#endif

	window_new_textbox(MainWindow, SensorResultTextbox);
	SensorResultTextboxGlobal = SensorResultTextbox;
	SensorResultTextbox->Position.X = 10;
	SensorResultTextbox->Position.Y = FlirPictureBox->Position.Y + FlirPictureBox->Size.Y + 10;
	SensorResultTextbox->Size.X = 460;
	SensorResultTextbox->Size.Y = 300;

	tControlCommandData control_comand;
	//Cursor_Global = &control_comand;
	control_comand.Comand = Control_Nop;
	display_light_enable();

	while(1)
	{
		heart_beat_service();
		if(timer_tick(&TimerReadBattery))
		{
			int percent_battery = to_percentage(2427, 3109, 100, _ADC[0]->ConvResult[2]);
			if(minimum_battery_level > percent_battery && gpio.in(BAT_STAT) != 0)
			{
				minimum_battery_level = percent_battery;
			}
			else if(minimum_battery_level < percent_battery && gpio.in(BAT_STAT) == 0)
			{
				minimum_battery_level = percent_battery;
			}
			read_and_display_all_sensors();
		}
		if(timer_tick(&TimerDisplayLight))
		{
			timer_disable(&TimerDisplayLight);
			gpio.out(LCD_BACKLIGHT, 0);
		}
		if(timer_tick(&TimerReadSensors))
		{
			//read_and_display_all_sensors();
#if (USE_USB_HOST_MSC == true)
			usb_msc_host_idle(0);
#endif
			memset(&control_comand, 0, sizeof(tControlCommandData));
#if (USE_USB_HOST_MOUSE == false)
			ft5x06_TouchIdle(TOUCH);
			control_comand.X = TOUCH->TouchResponse.x1;
			control_comand.Y = TOUCH->TouchResponse.y1;
#else
			USB_HOST_MOUSE_IDLE(0, &control_comand);
#endif
			//if(gpio.in(LCD_BACKLIGHT) != 0)
				//control_comand.Cursor = (CursorState)TOUCH->TouchResponse.touch_event1;
			//if(TOUCH->TouchResponse.touch_event1 == Cursor_Up)
				//display_light_enable();
			MainWindow->idle(MainWindow, &control_comand);
			//if(control_comand.WindowRefresh == true)
				//display_light_enable();
			Screen->lcd_func.put_rectangle(Screen, control_comand.X, control_comand.Y, 2, 2, true, 0x00000000);

			//HAL_ADC_Start_DMA(&AdcHandle, (uint32_t*)&_ADC[0]->ConvResult[0], 2);
			//adc_start_conversion(_ADC[0]);
#if (USE_USB_DEV_MSC_BRIDGE_MMCSDSPI == true)
			mmcsd_spi_idle(0);
			if(MMCSD_SPI[0]->connected == true && old_connected == false)
			{
				old_connected = MMCSD_SPI[0]->connected;
				usb_msc_dev_media_change_state(0, true);
			}
			else if(MMCSD_SPI[0]->connected == false && old_connected == true)
			{
				old_connected = MMCSD_SPI[0]->connected;
				usb_msc_dev_media_change_state(0, false);
			}
#endif
			/*if(old_dev_connection_status != usb_msc_dev_media_connected(0))
			{
				old_dev_connection_status = usb_msc_dev_media_connected(0);
				//mmcsd_spi_idle(0);
			}*/
#if (USE_MMCSD == true || USE_USB_DEV_MSC_BRIDGE_MMCSD)
			if(fs_mounted == false && IDLE_MMCSD(0) == true)
			{
				fs_mounted = true;
				String.Set(card_status_str, ",Card inserted");
				update_main_window_status_bar(MainWindow->Caption.Text);
#if (USE_USB_DEV_MSC_BRIDGE_MMCSD == true)
				usb_msc_dev_media_change_state(0, true);
#endif
			}
			else if(fs_mounted == true && IDLE_MMCSD(0) == false)
			{
				fs_mounted = false;
				String.Set(card_status_str, "");
				update_main_window_status_bar(MainWindow->Caption.Text);
#if (USE_USB_DEV_MSC_BRIDGE_MMCSD == true)
				usb_msc_dev_media_change_state(0, false);
#endif
			}
#endif //!USE_MMCSD
#if _USE_ADXL345 == 1
			signed int Xaccel = 0, Yaccel = 0, Zaccel = 0;
			if(adxl345_x_read(ADXL345, &Xaccel) &&
					adxl345_y_read(ADXL345, &Yaccel) &&
						adxl345_z_read(ADXL345, &Zaccel))
				UARTprintf(DebugCom, "ADXL345: X = %d, Y = %d, Z = %d\n\r", Xaccel, Yaccel, Zaccel);
#endif
/*
 * Display results.
 */
#if _USE_INT_ADC == 1
			UARTprintf(DebugCom, "ADC1: CH0 = %d, CH1 = %d, TempSensor = %2.2f\n\r\n\r", ADC[0]->ConvResult[0], ADC[0]->ConvResult[1], (float)(((float)1775 - (float)ADC[0]->ConvResult[2]) / 5.337) + (float)25);
#endif
		}
		/*
		 * This will pop chars from virtual com port and parse string commands.
		 */
		signed short tmp_com_char = -1;
		if((tmp_com_char = uart.getc_no_blocking(DebugCom)) != -1)
		{
			if(tmp_com_char != '\r')
			{
				char tmp_buff[2];
				tmp_buff[0] = tmp_com_char;
				tmp_buff[1] = '\0';
				strncat(terminal_receive_buff, tmp_buff, sizeof(terminal_receive_buff) - 1);
			}
			else
			{
				if(!strncmp(terminal_receive_buff, "Command1:", sizeof("Command1:") - 1))
				{
					/*
					 * Your code
					 */
				}
				else if(!strncmp(terminal_receive_buff, "Command2:", sizeof("Command2:") - 1))
				{
					/*
					 * Your code
					 */
				}
				else
					uart.puts(DebugCom, "Unrecognized command\r\n", -1);
				terminal_receive_buff[0] = '\0';
			}
		}
#if USE_NARRATOR
		player_idle(VS10XX);
#endif
	}
}

