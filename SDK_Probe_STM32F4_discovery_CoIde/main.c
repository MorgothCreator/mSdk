/*
 * main.c
 *
 *  Created on: Mar 6, 2013
 *      Iulian Gheorghiu <morgoth.creator@gmail.com>
 */
#include "board_init.h"
#include "device/ms5611.h"

int main(void)
{
	board_init();
	volatile int CntDelay;
	ms5611_prom_data ms5611_prom_data;
	ms5611_init(&ms5611_prom_data, TWI[0]);
	while(1)
	{
		CntDelay = 6200000;
		do {
			if(!CntDelay) break;
			CntDelay--;
		} while(1);
		gpio_out(LED[0], 1);
		gpio_out(LED[1], 1);
		gpio_out(LED[2], 1);
		gpio_out(LED[3], 1);
		//UARTprintf(DebugCom, "Str\n");
	    CntDelay = 6200000;
	    do {
			if(!CntDelay) break;
			CntDelay--;
	    } while(1);
		gpio_out(LED[0], 0);
		gpio_out(LED[1], 0);
		gpio_out(LED[2], 0);
		gpio_out(LED[3], 0);
		//UARTprintf(DebugCom, "Rts\n");
		ms5611_display_preasure_result(&ms5611_prom_data, TWI[0], MS5611_CONVERT_OSR_1024);
		UARTprintf(DebugCom, "ADC1 CH0 = %d, ADC1 CH1 = %d, ADC1 TempSensor = %d\n\n", ADC[0]->ConvResult[0], ADC[0]->ConvResult[1], ADC[0]->ConvResult[2]);
	}
	return 0;
}
