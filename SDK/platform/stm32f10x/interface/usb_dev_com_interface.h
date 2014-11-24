#ifndef USB_DEV_COM_INTERFACE_H__
#define USB_DEV_COM_INTERFACE_H__

void _usb_com_dev_init(unsigned int instance);
unsigned int _usb_com_dev_receive(unsigned char* buff);
unsigned int _usb_com_dev_send(unsigned char* buff, unsigned int nbytes);
//void _usb_com_dev_idle(unsigned int instance);


#endif
