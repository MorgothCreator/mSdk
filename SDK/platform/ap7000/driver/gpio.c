/*
 * Io.c
 *
 *  Created on: Feb 2, 2011
 *      Author: Nimeni
 */

#include <avr32/ap7000.h>
#include <avr32/io.h>
#include "gpio.h"
//#include "libio.h"
//#include "types.h"
#define     AVR32_PM_PBBMASK_reg         (*(volatile unsigned long *)(AVR32_PM_ADDRESS   + 0x014))

void  IO_pioen (unsigned char port, unsigned char pin)
{
    volatile  avr32_pio_t  *pio;


    switch (port) {
        case 0:
             pio = &AVR32_PIOA;
             break;

        case 1:
             pio = &AVR32_PIOB;
             break;

        case 2:
             pio = &AVR32_PIOC;
             break;

        case 3:
             pio = &AVR32_PIOD;
             break;

        case 4:
             pio = &AVR32_PIOE;
             break;

        default:
             return;
    }

    pio->per  = 1 << (pin % 32);        /* PIO controller enable                                    */
	//pio->per = 1 << (pin % 32); 		/* Set pio enable for PA19 of PORTA */
	pio->oer = 1 << (pin % 32); 		/* Make them output port */
	pio->idr = 1 << (pin % 32); 		/* Disable interrupts on these pins */
	pio->pudr =1 << (pin % 32); 		/* disable pull-ups */
	pio->sodr = 1 << (pin % 32);		/* se t PA19 to high*/
}

void  IO_intenable (unsigned char port, unsigned char pin)
{
    volatile  avr32_pio_t  *pio;


    switch (port) {
        case 0:
             pio = &AVR32_PIOA;
             break;

        case 1:
             pio = &AVR32_PIOB;
             break;

        case 2:
             pio = &AVR32_PIOC;
             break;

        case 3:
             pio = &AVR32_PIOD;
             break;

        case 4:
             pio = &AVR32_PIOE;
             break;

        default:
             return;
    }

    pio->per  = 1 << (pin % 32);                                        /* PIO controller enable                                    */
    pio->ifer = 1 << (pin % 32);                                        /* PIO glitch filter enable                                 */
    pio->ier  = 1 << (pin % 32);                                        /* PIO interrupt enable                                     */
}



char  IO_intstatus (unsigned char port, unsigned char pin)
{
    volatile  avr32_pio_t  *pio;
    unsigned long    pin_mask;


    switch (port) {
        case 0:
             pio = &AVR32_PIOA;
             break;

        case 1:
             pio = &AVR32_PIOB;
             break;

        case 2:
             pio = &AVR32_PIOC;
             break;

        case 3:
             pio = &AVR32_PIOD;
             break;

        case 4:
             pio = &AVR32_PIOE;
             break;

        default:
             return (0);
    }

    pin_mask  = 1 << (pin % 32);

    if ((pio->isr & pin_mask) == pin_mask) {
        return (1);
    } else {
        return (0);
    }
}





void  IO_SetFnct (unsigned char pin, unsigned char fnct)
{
    volatile  avr32_pio_t  *pio;
    unsigned long    pin_mask;


    switch (pin/32) {
        case 0:
             pio      = &AVR32_PIOA;
             break;

        case 1:
             pio      = &AVR32_PIOB;
             break;

        case 2:
             pio      = &AVR32_PIOC;
             break;

        case 3:
             pio      = &AVR32_PIOD;
             break;

        case 4:
             pio      = &AVR32_PIOE;
             break;

        default:
             return;
    }

    pin_mask  = 1 << (pin % 32);

    switch (fnct) {
        case 0:                                                         /* A function                                               */
             pio->asr = pin_mask;
             break;

        case 1:                                                         /* B function                                               */
             pio->bsr = pin_mask;
             break;
    }

    pio->pdr  = pin_mask;                                               /* disable PIO control, pin controlled by the peripheral    */
}

void  _IO_SetFnct (unsigned char port, unsigned char pin, unsigned char fnct)
{
    volatile  avr32_pio_t  *pio;
    unsigned long    pin_mask;


    switch (port) {
        case 0:
             pio      = &AVR32_PIOA;
             break;

        case 1:
             pio      = &AVR32_PIOB;
             break;

        case 2:
             pio      = &AVR32_PIOC;
             break;

        case 3:
             pio      = &AVR32_PIOD;
             break;

        case 4:
             pio      = &AVR32_PIOE;
             break;

        default:
             return;
    }

    pin_mask  = 1 << (pin % 32);

    switch (fnct) {
        case 0:                                                         /* A function                                               */
             pio->asr = pin_mask;
             break;

        case 1:                                                         /* B function                                               */
             pio->bsr = pin_mask;
             break;
    }

    pio->pdr  = pin_mask;                                               /* disable PIO control, pin controlled by the peripheral    */
}




void  IO_setinput (unsigned short port, unsigned long pins)
{
    volatile  avr32_pio_t  *pio;


    switch (port) {
        case 0:
             pio = &AVR32_PIOA;
             break;

        case 1:
             pio = &AVR32_PIOB;
             break;

        case 2:
             pio = &AVR32_PIOC;
             break;

        case 3:
             pio = &AVR32_PIOD;
             break;

        case 4:
             pio = &AVR32_PIOE;
             break;

        default:
             return;
    }

    pio->per  =  pins;                                                  /* PIO controller enable                                    */
    pio->odr  =  pins;                                                  /* Disable PIO output driver                                */
    pio->puer =  pins;                                                  /* Enable pull-ups                                          */
    pio->codr =  pins;                                                  /* Clear Output data register                               */
}


void  IO_setoutput (unsigned short port, unsigned long pins)
{
    volatile  avr32_pio_t  *pio;


    switch (port) {
        case 0:
             pio = &AVR32_PIOA;
             break;

        case 1:
             pio = &AVR32_PIOB;
             break;

        case 2:
             pio = &AVR32_PIOC;
             break;

        case 3:
             pio = &AVR32_PIOD;
             break;

        case 4:
             pio = &AVR32_PIOE;
             break;

        default:
             return;
    }

    pio->per  =  pins;                                                  /* PIO controller enable                                    */
    pio->oer  =  pins;                                                  /* Enable PIO output driver                                 */
    pio->puer =  pins;                                                  /* Enable pull-ups                                          */
}


char  IO_pinget (unsigned char port, unsigned char pin)
{
    volatile  avr32_pio_t  *pio;
    unsigned long    val;


    switch (port) {
        case 0:
             pio = &AVR32_PIOA;
             break;

        case 1:
             pio = &AVR32_PIOB;
             break;

        case 2:
             pio = &AVR32_PIOC;
             break;

        case 3:
             pio = &AVR32_PIOD;
             break;

        case 4:
             pio = &AVR32_PIOE;
             break;

        default:
             return (0);
    }

    val  =  pio->pdsr;                                                  /* Retrieve pin level for selected port                     */
    val &=  1 << (pin  % 32);

    return (val != 0);
}

void  IO_pinset (unsigned char port, unsigned char pin)
{
    volatile  avr32_pio_t  *pio;


    switch (port) {
        case 0:
             pio = &AVR32_PIOA;
             break;

        case 1:
             pio = &AVR32_PIOB;
             break;

        case 2:
             pio = &AVR32_PIOC;
             break;

        case 3:
             pio = &AVR32_PIOD;
             break;

        case 4:
             pio = &AVR32_PIOE;
             break;

        default:
             return;
    }

    pio->sodr = 1 << (pin  % 32);                                        /* Set high level on the pin                                */
}



void  IO_pinclear (unsigned char port, unsigned char pin)
{
    volatile  avr32_pio_t  *pio;


    switch (port) {
        case 0:
             pio = &AVR32_PIOA;
             break;

        case 1:
             pio = &AVR32_PIOB;
             break;

        case 2:
             pio = &AVR32_PIOC;
             break;

        case 3:
             pio = &AVR32_PIOD;
             break;

        case 4:
             pio = &AVR32_PIOE;
             break;

        default:
             return;
    }

    pio->codr = 1 << (pin  % 32);                                        /* Set low level on the pin                                 */
}

void IO_pintogle (unsigned char port, unsigned char pin)
{
    volatile  avr32_pio_t  *pio;
    unsigned long    pin_mask;


    switch (port) {
        case 0:
             pio  = &AVR32_PIOA;
             break;

        case 1:
             pio  = &AVR32_PIOB;
             break;

        case 2:
             pio  = &AVR32_PIOC;
             break;

        case 3:
             pio  = &AVR32_PIOD;
             break;

        case 4:
             pio  = &AVR32_PIOE;
             break;

        default:
             return;
    }

    pin_mask      = 1 << (pin  % 32);

    if ((pio->odsr & pin_mask) == 0) {
        pio->sodr = pin_mask;                                           /* Set high level on the pin                                */
    } else {
        pio->codr = pin_mask;                                           /* Set low level on the pin                                 */
    }
}




