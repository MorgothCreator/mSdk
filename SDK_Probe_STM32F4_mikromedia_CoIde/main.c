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
	volatile long CntDelay;
	while(1)
	{
		CntDelay = 7000000;
		do {
			if(!CntDelay) break;
			CntDelay--;
		} while(1);
		gpio_out(LED1, 1);
		//UARTprintf(DebugCom, "Str\n");
	    CntDelay = 7000000;
	    do {
			if(!CntDelay) break;
			CntDelay--;
	    } while(1);
		gpio_out(LED1, 0);
		//UARTprintf(DebugCom, "Rts\n");
	}
	return 0;
}
