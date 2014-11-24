#include "dev_com/hw_config.h"
#include "core/usb_lib.h"
#include "dev_com/usb_desc.h"
#include "dev_com/usb_pwr.h"
#include "sys/core_cm3.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
extern volatile uint8_t Receive_Buffer[64];
extern volatile  uint32_t Receive_length ;
extern volatile  uint32_t length ;
uint8_t Send_Buffer[64];
uint32_t packet_sent=1;
uint32_t packet_receive=1;

void _usb_com_dev_init(unsigned int instance)
{
	  Set_System();
	  Set_USBClock();
	  USB_Interrupts_Config();
	  USB_Init();
}

unsigned int _usb_com_dev_receive(unsigned char* buff)
{
    if (bDeviceState == CONFIGURED)
    {
        CDC_Receive_DATA();
        return Receive_length;
    } else return 0;
}

unsigned int _usb_com_dev_send(unsigned char* buff, unsigned int nbytes)
{
	if(nbytes == 0 || bDeviceState != CONFIGURED) return 0;
	unsigned int bytes_send = nbytes;
	if(bytes_send > sizeof(Receive_Buffer)) bytes_send = sizeof(Receive_Buffer);
    if (packet_sent == 1)
    {
    	CDC_Send_DATA ((unsigned char*)buff,bytes_send);
    	return bytes_send;
    }
    else return 0;
}
/* Not tested ( is on default ST configuration Loopback) */
//void _usb_com_dev_idle(unsigned int instance)
//{
//    if (bDeviceState == CONFIGURED)
//    {
//      CDC_Receive_DATA();
      /*Check to see if we have data yet */
//      if (Receive_length  != 0)
//      {
//        if (packet_sent == 1)
//          CDC_Send_DATA ((unsigned char*)Receive_Buffer,Receive_length);
//        Receive_length = 0;
//      }
//    }
//}


