/*
 * Io.h
 *
 *  Created on: Feb 2, 2011
 *      Author: Nimeni
 */

#ifndef IO_H_
#define IO_H_

#define     BSP_PORT_A                         0                        /* GPIO Port A pins Offset                                  */
#define     BSP_PORT_B                        32                        /* GPIO Port B pins Offset                                  */
#define     BSP_PORT_C                        64                        /* GPIO Port C pins Offset                                  */
#define     BSP_PORT_D                        96                        /* GPIO Port D pins Offset                                  */
#define     BSP_PORT_E                       128                        /* GPIO Port E pins Offset                                  */


void  IO_pioen (unsigned char port, unsigned char pin);
void  IO_intenable (unsigned char port, unsigned char pin);
char  IO_intstatus (unsigned char port, unsigned char pin);
void  IO_SetFnct (unsigned char pin, unsigned char fnct);
void  _IO_SetFnct (unsigned char port, unsigned char pin, unsigned char fnct);
void  IO_setinput (unsigned short port, unsigned long pins);
void  IO_setoutput (unsigned short port, unsigned long pins);
char  IO_pinget (unsigned char port, unsigned char pin);
void  IO_pinset (unsigned char port, unsigned char pin);
void  IO_pinclear (unsigned char port, unsigned char pin);
void IO_pintogle (unsigned char port, unsigned char pin);

#include "gpio.c"

#endif /* IO_H_ */
