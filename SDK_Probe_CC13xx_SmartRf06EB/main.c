/*
 * main.c
 */

#include "api/gpio_api.h"
#include "api/gpio_def.h"

#include "board_init.h"

new_gpio* LED[4];

int main(void) {
	board_init();
	//volatile int cnt = 0;
	while(1)
	{
		/*for(cnt = 0; cnt < 2500000; cnt++);
		gpio_out(LED[0], 1);
		gpio_out(LED[1], 0);
		gpio_out(LED[2], 0);
		gpio_out(LED[3], 0);
		for(cnt = 0; cnt < 2500000; cnt++);
		gpio_out(LED[0], 0);
		gpio_out(LED[1], 1);
		gpio_out(LED[2], 0);
		gpio_out(LED[3], 0);
		for(cnt = 0; cnt < 2500000; cnt++);
		gpio_out(LED[0], 0);
		gpio_out(LED[1], 0);
		gpio_out(LED[2], 1);
		gpio_out(LED[3], 0);
		for(cnt = 0; cnt < 2500000; cnt++);
		gpio_out(LED[0], 0);
		gpio_out(LED[1], 0);
		gpio_out(LED[2], 0);
		gpio_out(LED[3], 1);*/
		if(gpio_in(PUSHBTN[0]))
			gpio_out(LED[0], 1);
		else
			gpio_out(LED[0], 0);

		if(gpio_in(PUSHBTN[1]))
			gpio_out(LED[1], 1);
		else
			gpio_out(LED[1], 0);

		if(gpio_in(PUSHBTN[2]))
			gpio_out(LED[2], 1);
		else
			gpio_out(LED[2], 0);

		if(gpio_in(PUSHBTN[3]))
			gpio_out(LED[3], 1);
		else
			gpio_out(LED[3], 0);
	}
	return 0;
}
