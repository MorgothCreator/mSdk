/*
 * main.c
 *
 *  Created on: Mar 6, 2015
 *      Iulian Gheorghiu <morgoth.creator@gmail.com>
 */
#include <stdlib.h>
#include "main.h"
#include "board_init.h"
#include "api/timer_api.h"
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
#include "device/lepton_flir.h"

#include "interface/lwip/httpd.h"

int main(void)
{
	board_init();
	timer(TimerReadSensors);
    timer_interval(&TimerReadSensors, 300);
	timer(TimerBlinkLed);
    timer_interval(&TimerBlinkLed, 1000);
#if _USE_MPU60x0_9150 == 1
	mpu60x0_9150_init(MPU60x0_9150);
#endif
#if _USE_MHC5883 == 1
	mhc5883_init(MHC5883);
#endif
    bool Led1Status = false;
#if _USE_SHT11 == 1
	unsigned char sht11_status_reg = 0;
	bool sht11_read_mode = false;
#endif
#if _USE_LEPTON_FLIR == 1
	unsigned short flir_buff[((LEPTON_FLIR_LINE_SIZE / 2) - 2) * LEPTON_FLIR_LINES_NR];
#endif
	httpd_init();
	while(1)
	{
		if(timer_tick(&TimerBlinkLed)) {
			if(Led1Status) {
				gpio_out(LED, 1);
				timer_interval(&TimerBlinkLed, 100);
				timer_enable(&TimerBlinkLed);
				Led1Status = false;
			} else {
				gpio_out(LED, 0);
				timer_interval(&TimerBlinkLed, 900);
				timer_enable(&TimerBlinkLed);
				Led1Status = true;
			}
		}
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
#if _USE_LEPTON_FLIR == 1
			UARTPuts(DebugCom, (char *)flir_buff, ((LEPTON_FLIR_LINE_SIZE - 4) * LEPTON_FLIR_LINES_NR));
#endif
		}
	}
}
