/*
 * adc_interface.c
 *
 * Created: 2/24/2013 5:40:31 PM
 *  Author: XxXx
 */ 
//#####################################################
#include "adc_interface.h"
#include "driver/adcifb.h"
#include "sys/clock/sysclk.h"
#include "sys/clock/uc3l/sysclk.h"
//#####################################################
void adc_init(unsigned long chanels_mask, unsigned char mode, unsigned short period)
{
	// Assign and enable GPIO pin to the ADC function.
	if(chanels_mask & (1 << 0))
	{
		gpio_enable_module_pin(AVR32_ADCIFB_AD_0_PIN, AVR32_ADCIFB_AD_0_FUNCTION);
		//adcifb_channels_enable(&AVR32_ADCIFB, 1 << 0);
	}
	if(chanels_mask & (1 << 1))
	{
		gpio_enable_module_pin(AVR32_ADCIFB_AD_1_PIN, AVR32_ADCIFB_AD_1_FUNCTION);
		//adcifb_channels_enable(&AVR32_ADCIFB, 1 << 1);
	}
	if(chanels_mask & (1 << 2))
	{
		gpio_enable_module_pin(AVR32_ADCIFB_AD_2_PIN, AVR32_ADCIFB_AD_2_FUNCTION);
		//adcifb_channels_enable(&AVR32_ADCIFB, 1 << 2);
	}
	if(chanels_mask & (1 << 3))
	{
		gpio_enable_module_pin(AVR32_ADCIFB_AD_3_PIN, AVR32_ADCIFB_AD_3_FUNCTION);
		//adcifb_channels_enable(&AVR32_ADCIFB, 1 << 3);
	}
	if(chanels_mask & (1 << 4))
	{
		gpio_enable_module_pin(AVR32_ADCIFB_AD_4_PIN, AVR32_ADCIFB_AD_4_FUNCTION);
		//adcifb_channels_enable(&AVR32_ADCIFB, 1 << 4);
	}
	if(chanels_mask & (1 << 5))
	{
		gpio_enable_module_pin(AVR32_ADCIFB_AD_5_PIN, AVR32_ADCIFB_AD_5_FUNCTION);
		//adcifb_channels_enable(&AVR32_ADCIFB, 1 << 5);
	}
	if(chanels_mask & (1 << 6))
	{
		gpio_enable_module_pin(AVR32_ADCIFB_AD_6_PIN, AVR32_ADCIFB_AD_6_FUNCTION);
		//adcifb_channels_enable(&AVR32_ADCIFB, 1 << 6);
	}
	if(chanels_mask & (1 << 7))
	{
		gpio_enable_module_pin(AVR32_ADCIFB_AD_7_PIN, AVR32_ADCIFB_AD_7_FUNCTION);
		//adcifb_channels_enable(&AVR32_ADCIFB, 1 << 7);
	}

	unsigned long pba_clock = sysclk_get_pba_hz();
	adcifb_opt_t adcifb_opt = {
		.resolution             = AVR32_ADCIFB_ACR_RES_8BIT,
		.shtim                  = 15,
		.ratio_clkadcifb_clkadc = pba_clock / 6000000,
		.startup                = 3,
		.sleep_mode_enable      = false
	};

	// Enable and configure the ADCIFB module
	sysclk_enable_peripheral_clock(&AVR32_ADCIFB);
	adcifb_configure(&AVR32_ADCIFB, &adcifb_opt);

	// Configure the trigger (No trigger, only software trigger)
	adcifb_configure_trigger(&AVR32_ADCIFB, mode /*AVR32_ADCIFB_TRGR_TRGMOD_PT*/, period);

	adcifb_channels_enable(&AVR32_ADCIFB, chanels_mask);
}
//#####################################################
void adc_channels_enable_pio(unsigned long chanels_mask)
{
	// Assign and enable GPIO pin to the ADC function.
	if(chanels_mask & (1 << 0))
	{
		gpio_enable_module_pin(AVR32_ADCIFB_AD_0_PIN, AVR32_ADCIFB_AD_0_FUNCTION);
		//adcifb_channels_enable(&AVR32_ADCIFB, 1 << 0);
	}
	if(chanels_mask & (1 << 1))
	{
		gpio_enable_module_pin(AVR32_ADCIFB_AD_1_PIN, AVR32_ADCIFB_AD_1_FUNCTION);
		//adcifb_channels_enable(&AVR32_ADCIFB, 1 << 1);
	}
	if(chanels_mask & (1 << 2))
	{
		gpio_enable_module_pin(AVR32_ADCIFB_AD_2_PIN, AVR32_ADCIFB_AD_2_FUNCTION);
		//adcifb_channels_enable(&AVR32_ADCIFB, 1 << 2);
	}
	if(chanels_mask & (1 << 3))
	{
		gpio_enable_module_pin(AVR32_ADCIFB_AD_3_PIN, AVR32_ADCIFB_AD_3_FUNCTION);
		//adcifb_channels_enable(&AVR32_ADCIFB, 1 << 3);
	}
	if(chanels_mask & (1 << 4))
	{
		gpio_enable_module_pin(AVR32_ADCIFB_AD_4_PIN, AVR32_ADCIFB_AD_4_FUNCTION);
		//adcifb_channels_enable(&AVR32_ADCIFB, 1 << 4);
	}
	if(chanels_mask & (1 << 5))
	{
		gpio_enable_module_pin(AVR32_ADCIFB_AD_5_PIN, AVR32_ADCIFB_AD_5_FUNCTION);
		//adcifb_channels_enable(&AVR32_ADCIFB, 1 << 5);
	}
	if(chanels_mask & (1 << 6))
	{
		gpio_enable_module_pin(AVR32_ADCIFB_AD_6_PIN, AVR32_ADCIFB_AD_6_FUNCTION);
		//adcifb_channels_enable(&AVR32_ADCIFB, 1 << 6);
	}
	if(chanels_mask & (1 << 7))
	{
		gpio_enable_module_pin(AVR32_ADCIFB_AD_7_PIN, AVR32_ADCIFB_AD_7_FUNCTION);
		//adcifb_channels_enable(&AVR32_ADCIFB, 1 << 7);
	}
	adcifb_channels_enable(&AVR32_ADCIFB, chanels_mask);
}
//#####################################################
void adc_channels_enable(unsigned long chanels_mask)
{
	adcifb_channels_enable(&AVR32_ADCIFB, chanels_mask);
}
//#####################################################
void adc_channels_disable(unsigned long chanels_mask)
{
	adcifb_channels_disable(&AVR32_ADCIFB, chanels_mask);
}
//#####################################################
unsigned short adc_get_latest_data(void)
{
	return adcifb_get_last_converted_data(&AVR32_ADCIFB);
}
//#####################################################
unsigned char adc_get_latest_converted_channel(void)
{
	return adcifb_get_last_converted_channel(&AVR32_ADCIFB);
}
//#####################################################
void adc_start_conversion(void)
{
	return adcifb_start_conversion_sequence(&AVR32_ADCIFB);
}
//#####################################################
void (*_service_function)(void);

#if __GNUC__
__attribute__((__interrupt__))
#elif __ICCAVR32__
#pragma handler = AVR32_TC_IRQ_GROUP, 1
__interrupt
#endif
void _service_function_(void)
{
	AVR32_ADCIFB.icr = 0xFFFFFFFF;
	if(_service_function) _service_function();
}
//#####################################################
void adc_enable_interrupts(unsigned long mask, void (*service_function)(void))
{
	Disable_global_interrupt();
	
	_service_function = service_function;
	// The INTC driver has to be used only for GNU GCC for AVR32.
#if __GNUC__
	// Initialize interrupt vectors.

	INTC_register_interrupt(&_service_function_, AVR32_ADCIFB_IRQ, AVR32_INTC_INT0);
#endif
	Enable_global_interrupt();

	adcifb_interrupt_enable(&AVR32_ADCIFB, mask);
}
//#####################################################
void adc_disable_interrupts(unsigned long mask)
{
	adcifb_interrupt_disable(&AVR32_ADCIFB, mask);
}
//#####################################################

