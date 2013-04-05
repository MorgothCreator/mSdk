/*
 * core_init.c
 *
 * Created: 2/16/2013 9:50:36 PM
 *  Author: XxXx
 */ 

#include "core_init.h"
#include "sys/clock/sysclk.h"
#include "sys/clock/uc3l/sysclk.h"
#include "driver/tc.h"
#include "driver/intc.h"

void core_init()
{
		sysclk_init();
		INTC_init_interrupts();
}