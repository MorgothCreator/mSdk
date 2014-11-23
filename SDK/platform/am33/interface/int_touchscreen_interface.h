/*
 * int_touchscreen_interface.h
 *
 *  Created on: Dec 11, 2012
 *      Author: XxXx
 */
/*#####################################################*/
#ifndef INT_TOUCHSCREEN_INTERFACE_H_
#define INT_TOUCHSCREEN_INTERFACE_H_
/*#####################################################*/
#include "api/touchscreen_def.h"
#include "clk/clk_tsc_adc.h"
#include "api/lcd_def.h"
/*#####################################################*/
void InitTouchScreen(new_touchscreen* structure);
bool TouchIdle(new_touchscreen* structure);
void TouchIntEnable(void);
void TouchIntRegister(void);
void TouchEnable(void);
void StepEnable(void);
void StepDisable(void);
void TouchCalibrate(new_touchscreen* structure, tDisplay *pDisplay);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "int_touchscreen_interface.c"
#endif
/*#####################################################*/
#endif /* INT_TOUCHSCREEN_INTERFACE_H_ */
/*#####################################################*/
