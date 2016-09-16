/*
 * microe_touch.h
 *
 *  Created on: Aug 11, 2016
 *      Author: John Smith
 */

#ifndef MICROE_TOUCH_H_
#define MICROE_TOUCH_H_

#include "api/adc_def.h"
#include "api/gpio_def.h"
#include "api/touchscreen_def.h"

#define ANALOG_TOUCH_FILTER_LEVEL	4

typedef struct MicroeTouch_s{
	unsigned int x_data[ANALOG_TOUCH_FILTER_LEVEL];
	unsigned int y_data[ANALOG_TOUCH_FILTER_LEVEL];
	signed int ignore_cnt;
	unsigned int ignored_touch;
	unsigned int IsTSPress;
	unsigned int dbidx;
	unsigned char xy_state;
	bool touch_conv_finish;
	unsigned int AdcChannel_X;
	unsigned int AdcChannel_Y;
	Adc_t *Adc;
	Gpio_t *DriveA;
	Gpio_t *DriveB;
	LcdTouch_t *Touch;
}MicroeTouch_t;

bool mikroe_touch_init(LcdTouch_t *Touch, Adc_t *Adc, Gpio_t *DriveA, Gpio_t *DriveB, unsigned int AdcChannel_X, unsigned int AdcChannel_Y);
bool microe_touch_service(LcdTouch_t* Touch);
bool microe_touch_idle(LcdTouch_t* Touch);
void microe_touch_calibrate(LcdTouch_t* structure, void *pDisplay);

#endif /* MICROE_TOUCH_H_ */
