/*
 * main.c
 *
 *  Created on: May 9, 2013
 *      Author: Iulian Gheorghiu (morgoth.creator@gmail.com)
 */

#include "board_init.h"
#include "device/ms5611.h"

int main(void)
{
	board_init();
	volatile int CntDelay;
	ms5611_prom_data ms5611_prom_data;
	ms5611_init(&ms5611_prom_data, TWI[0]);
	UARTprintf(DebugCom, "Prom data: W0=%d, W1=%d, W2=%d, W3=%d, W4=%d, W5=%d, W6=%d, W7=%d\n\r",
			ms5611_prom_data.C0,
			ms5611_prom_data.C1,
			ms5611_prom_data.C2,
			ms5611_prom_data.C3,
			ms5611_prom_data.C4,
			ms5611_prom_data.C5,
			ms5611_prom_data.C6,
			ms5611_prom_data.C7);
	while(1)
	{
		CntDelay = 5700000;
		do {
			if(!CntDelay) break;
			CntDelay--;
		} while(1);
		gpio_out(LED1, 1);
		UARTprintf(DebugCom, "Str\n");
	    CntDelay = 5700000;
	    do {
			if(!CntDelay) break;
			CntDelay--;
	    } while(1);
		gpio_out(LED1, 0);
		UARTprintf(DebugCom, "Rts\n");
		signed int Preasure = 0, Temperature = 0;
		memset(&ms5611_prom_data, 0, sizeof(ms5611_prom_data));
		ms5611_init(&ms5611_prom_data, TWI[0]);
		UARTprintf(DebugCom, "Prom data: W0=%d, W1=%d, W2=%d, W3=%d, W4=%d, W5=%d, W6=%d, W7=%d\n\r",
				ms5611_prom_data.C0,
				ms5611_prom_data.C1,
				ms5611_prom_data.C2,
				ms5611_prom_data.C3,
				ms5611_prom_data.C4,
				ms5611_prom_data.C5,
				ms5611_prom_data.C6,
				ms5611_prom_data.C7);
		if(ms5611_read(&ms5611_prom_data, TWI[0], MS5611_CONVERT_OSR_1024, &Preasure, &Temperature)) {
			UARTprintf(DebugCom, "Preasure = %d, Temperature = %d\n", Preasure, Temperature);
		}
		ms5611_display_preasure_result(&ms5611_prom_data, TWI[0], MS5611_CONVERT_OSR_1024);
		UARTprintf(DebugCom, "ADC1 CH0 = %d, ADC1 CH1 = %d, ADC1 TempSensor = %d\n", ADC[0]->ConvResult[0], ADC[0]->ConvResult[1], ADC[0]->ConvResult[2]);
	}
	return 0;
}
