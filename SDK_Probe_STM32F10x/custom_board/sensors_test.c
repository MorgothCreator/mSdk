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
			if(sht11(_SHT11, SHT11_START_MEASURE_TEMPERATURE, &sht11_status_reg)) {
				if(sht11_read(_SHT11))
					sht11_read_mode = false;

			}
		} else {
			if(sht11(_SHT11, SHT11_START_MEASURE_HUMIDITY, &sht11_status_reg)) {
				if(sht11_read(_SHT11))
					sht11_read_mode = true;
			}
		}
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
			UARTprintf(DebugCom, "SHT11: T = %u, H = %u\n\r", (unsigned long)_SHT11->temperature, (unsigned long)_SHT11->humidity);
			UARTprintf(DebugCom, "ADC1 CH0 = %d, ADC1 CH1 = %d, ADC1 TempSensor = %d\n\r\n\r", ADC[0]->ConvResult[0], ADC[0]->ConvResult[1], ADC[0]->ConvResult[2]);
		}
	}
}








#endif
