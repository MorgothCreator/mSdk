/*
 * main.c
 */
#include "board_init.h"

int main(void) {
	
	board_init();
	unsigned int cnt = 0;
	//unsigned int *tmp = (void*)(unsigned int)0x30080;
	//tmp[0] = cnt;

	while(1)
	{
		cnt = 2000000;
		while(--cnt);
		gpio_out(LED1, 1);
		gpio_out(LED2, 0);
		cnt = 2000000;
		while(--cnt);
		gpio_out(LED1, 0);
		gpio_out(LED2, 1);
	}
}
