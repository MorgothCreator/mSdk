/*
 * Uart.c
 *
 *  Created on: Feb 2, 2011
 *      Author: Nimeni
 */

#include "avr32/ap7000.h"
#include "stdarg.h"
#include "stdio.h"
#include "uart.h"
#include "gpio.h"
#include "sys/core/core.h"

void  uart_init ( unsigned long baud_rate)
{
    volatile  avr32_usart_t  *usart;
    unsigned short      cd;                                       /* Clock divider                                            */
    unsigned char      fp;                                       /* Fractional part for clock divider                        */
    unsigned long      pba_freq;

    IO_SetFnct(AVR32_USART1_RXD_0_PIN, AVR32_USART1_RXD_0_FUNCTION);
    IO_SetFnct(AVR32_USART1_TXD_0_PIN, AVR32_USART1_TXD_0_FUNCTION);

    usart  = &AVR32_USART1;

    usart->idr      =  0xFFFFFFFF;                                      /* Disable all USART interrupts                             */

    usart->mr       =  0;                                               /* Reset MODE regsiter                                      */
    usart->rtor     =  0;                                               /* Reset Receiver Time-out register                         */
    usart->ttgr     =  0;                                               /* Reset Transmitter Timeguard register                     */

    usart->cr       =  AVR32_USART_CR_RSTRX_MASK   |                    /* Reset RX                                                 */
                       AVR32_USART_CR_RSTTX_MASK   |                    /* Reset TX                                                 */
                       AVR32_USART_CR_RXDIS_MASK   |                    /* Disable RX                                               */
                       AVR32_USART_CR_TXDIS_MASK   |                    /* Disable TX                                               */
                       AVR32_USART_CR_RSTSTA_MASK  |                    /* Reset Status Bits                                        */
                       AVR32_USART_CR_RSTIT_MASK   |                    /* Reset Iterations                                         */
                       AVR32_USART_CR_RSTNACK_MASK |                    /* Reset Non Acknowledge                                    */
                       AVR32_USART_CR_DTRDIS_MASK  |                    /* Disable DTR                                              */
                       AVR32_USART_CR_RTSDIS_MASK;                      /* Disable RTS                                              */

    pba_freq        =  Get_PBA_Clk();                               /* Retrieve Peripheral Bus A operating frequency            */

    if (baud_rate < (pba_freq / 16)) {                                  /* 8x oversampling                                          */
        usart->mr  |=  AVR32_USART_MR_OVER_MASK;                        /* Set oversampling mask                                    */
        cd          =  pba_freq / ( 8 * baud_rate);
        fp          = (pba_freq /       baud_rate) % 8;
    } else {                                                            /* 16x oversampling                                         */
        usart->mr  &= ~AVR32_USART_MR_OVER_MASK;                        /* Clear oversampling mask                                  */
        cd          =  pba_freq / (16 * baud_rate);
        fp          = (pba_freq / ( 2 * baud_rate)) % 8;
    }

    usart->brgr     =  cd << AVR32_USART_BRGR_CD_OFFSET;                /* Set CR on Baud Rate Generator register                   */
    usart->brgr    |=  fp << AVR32_USART_BRGR_FP_OFFSET;                /* Set FP on Baud Rate Generator register                   */
                                                                        /* Set normal RS232 mode, no parity bit, 1 stop bit,        */
    usart->mr      |= (AVR32_USART_MR_MODE_NORMAL << AVR32_USART_MR_CHMODE_OFFSET) |
                      (AVR32_USART_MR_PAR_NONE    << AVR32_USART_MR_PAR_OFFSET)    |
                      (AVR32_USART_MR_NBSTOP_1    << AVR32_USART_MR_NBSTOP_OFFSET) |
                      ((8 - 5) << AVR32_USART_MR_CHRL_OFFSET);          /* Set character length (5-8 bits). if 9-bits, set MODE9    */

    usart->cr      |=  AVR32_USART_CR_TXEN_MASK |                       /* Enable TX                                                */
                       AVR32_USART_CR_RXEN_MASK;                        /* Enable RX                                                */
}
