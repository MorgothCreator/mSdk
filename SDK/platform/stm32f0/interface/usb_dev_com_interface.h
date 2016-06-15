/*
 * usb_dev_com_interface.h
 *
 *  Created on: Nov 23, 2014
 *      Author: Morgoth
 */

#ifndef USB_DEV_COM_INTERFACE_H_
#define USB_DEV_COM_INTERFACE_H_


void _usb_com_dev_init(unsigned int instance);
unsigned int _usb_com_dev_receive(unsigned char* buff);
unsigned int _usb_com_dev_send(unsigned char* buff, unsigned int nbytes);



#endif /* USB_DEV_COM_INTERFACE_H_ */
