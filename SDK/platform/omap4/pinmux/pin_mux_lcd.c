/*
 * pin_mux_lcd.c
 *
 *  Created on: Feb 2, 2013
 *      Author: XxXx
 */

#include <stdbool.h>
#include "pin_mux_lcd.h"
#include "driver/control_module.h"

bool pin_mux_lcd_pandaboard_es1_rev_b1(void)
{

	cm_pin_set(w3_dispc2_hsync, false, 0, false);//Parallel Display Horizontal Sync
	cm_pin_set(w4_dispc2_pclk, false, 0, false);//Parallel Display Pixel Clock
	cm_pin_set(y2_dispc2_vsync, false, 0, false);//Parallel Display Vertical Sync
	cm_pin_set(y3_dispc2_de, false, 0, false);//Parallel Display Data Enable
	cm_pin_set(af12_dispc2_data23, false, 0, false);//Parallel Display Data Bit 23
	cm_pin_set(ae12_dispc2_data22, false, 0, false);//Parallel Display Data Bit 22
	cm_pin_set(ag13_dispc2_data21, false, 0, false);//Parallel Display Data Bit 21
	cm_pin_set(ae11_dispc2_data20, false, 0, false);//Parallel Display Data Bit 20
	cm_pin_set(af11_dispc2_data19, false, 0, false);//Parallel Display Data Bit 19
	cm_pin_set(ag11_dispc2_data18, false, 0, false);//Parallel Display Data Bit 18
	cm_pin_set(w2_dispc2_data17, false, 0, false);//Parallel Display Data Bit 17
	cm_pin_set(w1_dispc2_data16, false, 0, false);//Parallel Display Data Bit 16
	cm_pin_set(ah11_dispc2_data15, false, 0, false);//Parallel Display Data Bit 15
	cm_pin_set(ae10_dispc2_data14, false, 0, false);//Parallel Display Data Bit 14
	cm_pin_set(af10_dispc2_data13, false, 0, false);//Parallel Display Data Bit 13
	cm_pin_set(ag10_dispc2_data12, false, 0, false);//Parallel Display Data Bit 12
	cm_pin_set(ae9_dispc2_data11, false, 0, false);//Parallel Display Data Bit 11
	cm_pin_set(v1_dispc2_data10, false, 0, false);//Parallel Display Data Bit 10
	cm_pin_set(v2_dispc2_data9, false, 0, false);//Parallel Display Data Bit 9
	cm_pin_set(y4_dispc2_data8, false, 0, false);//Parallel Display Data Bit 8
	cm_pin_set(aa1_dispc2_data7, false, 0, false);//Parallel Display Data Bit 7
	cm_pin_set(aa2_dispc2_data6, false, 0, false);//Parallel Display Data Bit 6
	cm_pin_set(aa3_dispc2_data5, false, 0, false);//Parallel Display Data Bit 5
	cm_pin_set(aa4_dispc2_data4, false, 0, false);//Parallel Display Data Bit 4
	cm_pin_set(ab2_dispc2_data3, false, 0, false);//Parallel Display Data Bit 3
	cm_pin_set(ab3_dispc2_data2, false, 0, false);//Parallel Display Data Bit 2
	cm_pin_set(ab4_dispc2_data1, false, 0, false);//Parallel Display Data Bit 1
	cm_pin_set(ac4_dispc2_data0, false, 0, false);//Parallel Display Data Bit 0
	return true;
}

