/*
 * example1.h
 *
 *  Created on: Nov 23, 2014
 *      Author: Morgoth
 */

#ifndef EXAMPLE1_H_
#define EXAMPLE1_H_
#include "main.h"

#ifdef test1

/*-----------------------------------------------------*/
#define lcd
//#define touch
//#define usb_1_msc
#define usb_1_mouse
#define usb_0_dev_msc
#define BridgeUsbDev0ToMmcSd0
//#define BridgeUsbDev0ToMmcSd1
//#define BridgeUsbDev0ToUsbHost1


/*-----------------------------------------------------*/
#define HDMI_DISPLAY_MODE_STRUCT	lcd_720p_50hz_beaglebone_exp
//#define HDMI_DISPLAY_MODE_STRUCT	lcd_1080p_24hz_beaglebone_exp
//#define HDMI_DISPLAY_MODE_STRUCT	lcd_720p_480_60hz_beaglebone_exp
#endif

#endif /* EXAMPLE1_H_ */
