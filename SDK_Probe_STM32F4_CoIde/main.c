/*
 * main.c
 *
 *  Created on: Mar 6, 2013
 *      Author: Iulian Gheorghiu (morgoth.creator@gmail.com)
 */

#include "board_init.h"

int main(void)
{
	board_init();
	volatile int CntDelay;
	while(1)
	{
		CntDelay = 7000000;
		do {
			if(!CntDelay) break;
			CntDelay--;
		} while(1);
		gpio_out(LED1, 1);
		gpio_out(LED2, 1);
		gpio_out(LED3, 1);
		gpio_out(LED4, 1);
		UARTprintf(DebugCom, "Str\n");
	    CntDelay = 8000000;
	    do {
			if(!CntDelay) break;
			CntDelay--;
	    } while(1);
		gpio_out(LED1, 0);
		gpio_out(LED2, 0);
		gpio_out(LED3, 0);
		gpio_out(LED4, 0);
		UARTprintf(DebugCom, "Rts\n");
	}
	return 0;
}
