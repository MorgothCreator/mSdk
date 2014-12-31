/*
 * main.c
 *
 *  Created on: May 9, 2013
 *      Author: Iulian Gheorghiu (morgoth.creator@gmail.com)
 */

#include "board_init.h"
#include "api/timer_api.h"
#include "device/ms5611.h"
#include "device/mpu60x0.h"
#include "device/mhc5883.h"

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
	while(1)
	{
		if(timer_tick(&TimerBlinkLed)) {
			if(!gpio_in(LED1)) {
				gpio_out(LED1, 1);
				timer_interval(&TimerBlinkLed, 100);
				timer_enable(&TimerBlinkLed);
			} else {
				gpio_out(LED1, 0);
				timer_interval(&TimerBlinkLed, 900);
				timer_enable(&TimerBlinkLed);
			}
		}

		if(timer_tick(&TimerReadSensors)) {
			ms5611_display_preasure_result(&ms5611_prom_data, TWI[0], MS5611_CONVERT_OSR_1024);
			mpu60x0_temperature_display_result(TWI[0], 0);
			mpu60x0_giroscope_display_result(TWI[0], 0);
			mpu60x0_accelerometer_display_result(TWI[0], 0);
			mhc5883_display_result(TWI[0]);
			UARTprintf(DebugCom, "ADC1 CH0 = %d, ADC1 CH1 = %d, ADC1 TempSensor = %d\n\n", ADC[0]->ConvResult[0], ADC[0]->ConvResult[1], ADC[0]->ConvResult[2]);
		}
	}
	return 0;
}
