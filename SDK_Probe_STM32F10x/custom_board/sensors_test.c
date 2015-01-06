/*
 * bioloid_test.c
 *
 *  Created on: jan 5, 2013
 *      Author: Iulian Gheorghiu (morgoth.creator@gmail.com)
 */
#include "project.h"

#ifdef SENSORS_TEST
#include "string.h"
#include "bioloid_test.h"
#include "board_init.h"
#include "api/timer_api.h"
#include "device/ms5611.h"
#include "device/mpu60x0.h"
#include "device/mhc5883.h"
#include "device/sht11.h"

#include "stdio.h"

FILE aa;



int main(void)
{
	board_init();
	timer(TimerReadSensors);
    timer_interval(&TimerReadSensors, 1000);
	timer(TimerBlinkLed);
    timer_interval(&TimerBlinkLed, 1000);
	ms5611_prom_data ms5611_prom_data;
	ms5611_init(&ms5611_prom_data, TWI[0]);
	mpu60x0_init(TWI[0], 0);
	mhc5883_init(TWI[0]);
    bool Led1Status = false;
	unsigned char sht11_status_reg = 0;
	bool sht11_read_mode = false;
	while(1)
	{
		if(sht11_read_mode) {
			/* Send start temperature measure */
			if(sht11_write(SHT11, SHT11_START_MEASURE_TEMPERATURE, &sht11_status_reg)) {
				/* Pool SGT11 and read data if is ready */
				if(sht11_read(SHT11))
					/* Read complete */
					sht11_read_mode = false;
			}
		} else {
			/* Send start humidity measure */
			if(sht11_write(SHT11, SHT11_START_MEASURE_HUMIDITY, &sht11_status_reg)) {
				/* Pool SGT11 and read data if is ready */
				if(sht11_read(SHT11))
					/* Read complete */
					sht11_read_mode = true;
			}
		}
		/* Send start ranging */
		if(srf02_start(SRF02, SRF02_START_RANGING))
			/* Pool SRF02 and read data if is ready */
			srf02_read(SRF02);

		if(timer_tick(&TimerBlinkLed)) {
			if(Led1Status) {
				gpio_out(LED1, 0);
				timer_interval(&TimerBlinkLed, 100);
				timer_enable(&TimerBlinkLed);
				Led1Status = false;
			} else {
				gpio_out(LED1, 1);
				timer_interval(&TimerBlinkLed, 900);
				timer_enable(&TimerBlinkLed);
				Led1Status = true;
			}
		}
		if(timer_tick(&TimerReadSensors)) {
			ms5611_display_preasure_result(&ms5611_prom_data, TWI[0], MS5611_CONVERT_OSR_1024);
			mpu60x0_temperature_display_result(TWI[0], 0);
			mpu60x0_giroscope_display_result(TWI[0], 0);
			mpu60x0_accelerometer_display_result(TWI[0], 0);
			mhc5883_display_result(TWI[0]);
			//UARTprintf(DebugCom, "SHT11: T = %u, H = %u\n\r", (unsigned long)_SHT11->temperature, (unsigned long)_SHT11->humidity);
			sht11_display_data(SHT11);
			srf02_display_data(SRF02);
			UARTprintf(DebugCom, "ADC1:\n\rCH0 = %d, CH1 = %d, TempSensor = %f\n\r\n\r", ADC[0]->ConvResult[0], ADC[0]->ConvResult[1], (float)(((float)1775 - (float)ADC[0]->ConvResult[2]) / 5.337) + (float)25);//Temperature (in °C) = {(V25 - VSENSE) / Avg_Slope} + 25.
		}
	}
}








#endif
