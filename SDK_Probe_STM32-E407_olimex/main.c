/*
 * main.c
 *
 *  Created on: Mar 6, 2013
 *      Iulian Gheorghiu <morgoth.creator@gmail.com>
 */
#include "main.h"
#include "board_init.h"
#include "api/timer_api.h"
#include "device/mpu60x0.h"
#include "device/ak8975.h"

int main(void)
{
	board_init();
	timer(TimerReadSensors);
    timer_interval(&TimerReadSensors, 250);
	timer(TimerBlinkLed);
    timer_interval(&TimerBlinkLed, 1000);
	mpu60x0_init(MPU60x0, 0);
    bool Led1Status = false;
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
		if(timer_tick(&TimerReadSensors)) {
			mpu60x0_temperature_display_result(MPU60x0, 0);
			mpu60x0_giroscope_display_result(MPU60x0, 0);
			mpu60x0_accelerometer_display_result(MPU60x0, 0);
			ak8975_display_result(AK8975);
			UARTprintf(DebugCom, "ADC1:\n\rCH0 = %d, CH1 = %d, TempSensor = %2.2f\n\r\n\r", ADC[0]->ConvResult[0], ADC[0]->ConvResult[1], (float)(((float)1775 - (float)ADC[0]->ConvResult[2]) / 5.337) + (float)25);

		}
	}
}
