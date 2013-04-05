/*
 * sysdelay.c
 *
 * Created: 2/8/2013 10:53:14 PM
 *  Author: XxXx
 */ 

#include "sysdelay.h"
#include "driver/intc.h"

/******************************************************************************
**                      INTERNAL VARIABLE DEFINITIONS
*******************************************************************************/
static volatile unsigned long long flagIsr = 1;
static volatile unsigned long long STimerCnt;
//#####################################################
#if __GNUC__
__attribute__((__interrupt__))
#elif __ICCAVR32__
#pragma handler = AVR32_TC_IRQ_GROUP, 1
__interrupt
#endif
static void tc_irq(void)
{
	tc_read_sr(&AVR32_TC0, 0);
	// Increment the ms seconds counter
	STimerCnt++;

	// Clear the interrupt flag. This is a side effect of reading the TC SR.
	//tc_read_sr(&AVR32_TC, TC_CHANNEL);
}
//#####################################################
void SysDelayTimerSetup(void)
{
	 
volatile avr32_tc_t *tc = &AVR32_TC0;

   // Options for waveform genration.
   static const tc_waveform_opt_t WAVEFORM_OPT =
   {
     .channel  = 0,									// Channel selection.

     .bswtrg   = TC_EVT_EFFECT_NOOP,                // Software trigger effect on TIOB.
     .beevt    = TC_EVT_EFFECT_NOOP,                // External event effect on TIOB.
     .bcpc     = TC_EVT_EFFECT_NOOP,                // RC compare effect on TIOB.
     .bcpb     = TC_EVT_EFFECT_NOOP,                // RB compare effect on TIOB.

     .aswtrg   = TC_EVT_EFFECT_NOOP,                // Software trigger effect on TIOA.
     .aeevt    = TC_EVT_EFFECT_NOOP,                // External event effect on TIOA.
     .acpc     = TC_EVT_EFFECT_NOOP,                // RC compare effect on TIOA: toggle.
     .acpa     = TC_EVT_EFFECT_NOOP,                // RA compare effect on TIOA: toggle (other possibilities are none, set and clear).

     .wavsel   = TC_WAVEFORM_SEL_UP_MODE_RC_TRIGGER,// Waveform selection: Up mode with automatic trigger(reset) on RC compare.
     .enetrg   = false,                             // External event trigger enable.
     .eevt     = 0,                                 // External event selection.
     .eevtedg  = TC_SEL_NO_EDGE,                    // External event edge selection.
     .cpcdis   = false,                             // Counter disable when RC compare.
     .cpcstop  = false,                             // Counter clock stopped with RC compare.

     .burst    = false,                             // Burst signal selection.
     .clki     = false,                             // Clock inversion.
     .tcclks   = TC_CLOCK_SOURCE_TC3                // Internal source clock 3, connected to fPBA / 8.
   };

   static const tc_interrupt_t TC_INTERRUPT =
   {
     .etrgs = 0,
     .ldrbs = 0,
     .ldras = 0,
     .cpcs  = 1,
     .cpbs  = 0,
     .cpas  = 0,
     .lovrs = 0,
     .covfs = 0
   };
	Disable_global_interrupt();
   // The INTC driver has to be used only for GNU GCC for AVR32.
#if __GNUC__
   // Register the RTC interrupt handler to the interrupt controller.
   INTC_register_interrupt(&tc_irq, AVR32_TC0_IRQ0, AVR32_INTC_INT0);
#endif
   Enable_global_interrupt();
   
	sysclk_enable_peripheral_clock((avr32_tc_t*)&AVR32_TC0);

// Initialize the timer/counter.
   tc_init_waveform(tc, &WAVEFORM_OPT);         // Initialize the timer/counter waveform.

   // Set the compare triggers.
   // Remember TC counter is 16-bits, so counting second is not possible with fPBA = 12 MHz.
   // We configure it to count ms.
   // We want: (1/(fPBA/8)) * RC = 0.001 s, hence RC = (fPBA/8) / 1000 = 1500 to get an interrupt every 1 ms.
   tc_write_rc(tc, 0, (sysclk_get_pba_hz() / 8) / 1000); // Set RC value.

   tc_configure_interrupts(tc, 0, &TC_INTERRUPT);

   // Start the timer/counter.
   tc_start(tc, 0);                    // And start the timer/counter.
}
//#####################################################
void Sysdelay(unsigned int milliSec)
{
	flagIsr = STimerCnt + milliSec;
	while(flagIsr < STimerCnt);
}
//#####################################################
