/*
 * ph7_62_matrix_led.c
 *
 * Created: 2/15/2013 11:29:03 PM
 *  Author: XxXx
 */ 
//#####################################################
#include <stdbool.h>
#include "ph7_62_matrix_led.h"
//#include "driver/tc.h"
//#include "api/lcd_api.h"
#include "api/lcd_def.h"
#include "driver/gpio.h"
//#####################################################
extern unsigned long CoreFreq;
#define GPIO  AVR32_GPIO
tDisplay *ph7_62_pDisplay;
//#####################################################
#if __GNUC__
__attribute__((__interrupt__))
#elif __ICCAVR32__
#pragma handler = AVR32_TC_IRQ_GROUP, 1
__interrupt
#endif
static void ph7_62_int1_service_function(void)
{
	tDisplay *pDisplay = (tDisplay *)ph7_62_pDisplay;
	ph7_62_t *UserData = (ph7_62_t *)pDisplay->UserData;
	switch (UserData->UsedTimerNr)
	{
#ifdef AVR32_TC0
		case 0:
			tc_read_sr(&AVR32_TC0, 1);
			break;
#endif
#ifdef AVR32_TC1
		case 1:
			tc_read_sr(&AVR32_TC1, 1);
			break;
#endif
	}

	
	ph7_62_RGB_t *VideoRamPointerUp  = (ph7_62_RGB_t *)pDisplay->DisplayData + ((UserData->V_Cnt + 1) * pDisplay->Width);
	ph7_62_RGB_t *VideoRamPointerDn  = VideoRamPointerUp + (UserData->V_Cnt_Limit * pDisplay->Width);
	//unsigned char *VideoRamPointerUp  = (unsigned char *)_VideoRamPointerUp;
	//unsigned char *VideoRamPointerDn  = (unsigned char *)_VideoRamPointerDn;
	unsigned char H_Cnt = 0;
	unsigned char ColorCnt = UserData->ColorCnt;
	unsigned long Tmp = 0;
	//VideoRamPointerUp-=3;
	//VideoRamPointerDn-=3;
	volatile avr32_gpio_port_t *gpio_port = &GPIO.port[UserData->RGB_Port];
	volatile avr32_gpio_port_t *gpio_port_ABC = &GPIO.port[UserData->ABC_Port];
	gpio_set_group_high((UserData->Oe_Port << 5), UserData->Oe_PinMask);
	gpio_port_ABC->ovr = (gpio_port_ABC->ovr & (~(7 << UserData->ABC_PinPosition))) | ((~UserData->V_Cnt & 7)  << UserData->ABC_PinPosition);
	for (H_Cnt = 0;H_Cnt < pDisplay->Width; H_Cnt++)
	{
		VideoRamPointerUp--;
		VideoRamPointerDn--;

		Tmp = gpio_port->ovr;
		//Tmp &= ~UserData->RGB_Mask;
		if(VideoRamPointerUp->R > ColorCnt) Tmp |= UserData->R1_PinMask;//UserData->R1_PinMask;
		else  Tmp &= ~UserData->R1_PinMask;//UserData->R1_PinMask;
		if(VideoRamPointerDn->R > ColorCnt) Tmp |= UserData->R2_PinMask;//UserData->R2_PinMask;
		else  Tmp &= ~UserData->R2_PinMask;//UserData->R2_PinMask;
							
		if(VideoRamPointerUp->G > ColorCnt) Tmp |= UserData->G1_PinMask;//UserData->G1_PinMask;
		else  Tmp &= ~UserData->G1_PinMask;//UserData->G1_PinMask;
		if(VideoRamPointerDn->G > ColorCnt) Tmp |= UserData->G2_PinMask;//UserData->G2_PinMask;
		else  Tmp &= ~UserData->G2_PinMask;//UserData->G2_PinMask;
							
		if(VideoRamPointerUp->B > ColorCnt) Tmp |= UserData->B1_PinMask;//UserData->B1_PinMask;
		else  Tmp &= ~UserData->B1_PinMask;//UserData->B1_PinMask;
		if(VideoRamPointerDn->B > ColorCnt) Tmp |= UserData->B2_PinMask;//UserData->B2_PinMask;
		else  Tmp &= ~UserData->B2_PinMask;//UserData->B2_PinMask;
							
		gpio_port->ovr = Tmp;
		gpio_set_group_low((UserData->Sclk_Port << 5), UserData->Sclk_PinMask);
		gpio_set_group_high((UserData->Sclk_Port << 5), UserData->Sclk_PinMask);
	}
	gpio_set_group_low((UserData->Latch_Port << 5), UserData->Latch_PinMask);
	gpio_set_group_high((UserData->Latch_Port << 5), UserData->Latch_PinMask);
	gpio_set_group_low((UserData->Oe_Port << 5), UserData->Oe_PinMask);
	UserData->ColorCnt++;
	if(UserData->ColorCnt >= UserData->ColorNr) 
	{
		UserData->ColorCnt = 0;
		UserData->V_Cnt++;
		if(UserData->V_Cnt >= UserData->V_Cnt_Limit) UserData->V_Cnt = 0;
	}	
}
//#####################################################
bool ph7_62_init(tDisplay *pDisplay)
{
	if(!pDisplay) return false;
	ph7_62_pDisplay = pDisplay;
	ph7_62_t *UserData = (ph7_62_t *)pDisplay->UserData;
	//UserData->TimerBaseAddr = tc_get_set_base_address(TimerStruct, UserData->UsedTimerNr);
	
	volatile avr32_tc_t *tc;
	switch (UserData->UsedTimerNr)
	{
#ifdef AVR32_TC0
		case 0:
			tc = &AVR32_TC0;
			break;
#endif
#ifdef AVR32_TC1
		case 1:
			tc = &AVR32_TC1;
			break;
#endif
		default:
			return false;
	}

	pDisplay->DisplayData = (unsigned int *)calloc(1, sizeof(ph7_62_RGB_t) * pDisplay->Height * pDisplay->Width);
	
	if(!pDisplay->DisplayData)
	{
		if(pDisplay) free(pDisplay);
		return false;
	}
	
	
	pDisplay->sClipRegion.sXMin = 0;
	pDisplay->sClipRegion.sXMax = pDisplay->Width;
	pDisplay->sClipRegion.sYMin = 0;
	pDisplay->sClipRegion.sYMax = pDisplay->Height;

	gpio_set_gpio_pin((UserData->ABC_Port << 5) + UserData->ABC_PinPosition);
	gpio_set_gpio_pin((UserData->ABC_Port << 5) + UserData->ABC_PinPosition + 1);
	gpio_set_gpio_pin((UserData->ABC_Port << 5) + UserData->ABC_PinPosition + 2);

	gpio_set_gpio_pin((UserData->RGB_Port << 5) + UserData->R1_PinNr);
	gpio_set_gpio_pin((UserData->RGB_Port << 5) + UserData->R2_PinNr);
	gpio_set_gpio_pin((UserData->RGB_Port << 5) + UserData->G1_PinNr);
	gpio_set_gpio_pin((UserData->RGB_Port << 5) + UserData->G2_PinNr);
	gpio_set_gpio_pin((UserData->RGB_Port << 5) + UserData->B1_PinNr);
	gpio_set_gpio_pin((UserData->RGB_Port << 5) + UserData->B2_PinNr);
	gpio_set_gpio_pin((UserData->Latch_Port << 5) + UserData->Latch_PinNr);
	gpio_set_gpio_pin((UserData->Oe_Port << 5) + UserData->Oe_PinNr);
	gpio_set_gpio_pin((UserData->Sclk_Port << 5) + UserData->Sclk_PinNr);
	
	UserData->V_Cnt_Limit = pDisplay->Height / UserData->PanelsNr;
	
	UserData->RGB_Mask = UserData->R1_PinMask | UserData->R2_PinMask | UserData->G1_PinMask | UserData->G2_PinMask | UserData->B1_PinMask | UserData->B2_PinMask;

// Options for waveform genration.
	static const tc_waveform_opt_t WAVEFORM_OPT =
	{
		.channel  = 1,									// Channel selection.

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
// Initialize interrupt vectors.
	//INTC_init_interrupts();

// Register the RTC interrupt handler to the interrupt controller.
	INTC_register_interrupt(&ph7_62_int1_service_function, AVR32_TC0_IRQ1, AVR32_INTC_INT0);
#endif
	Enable_global_interrupt();

	sysclk_enable_peripheral_clock(tc);

// Initialize the timer/counter.
	tc_init_waveform(tc, &WAVEFORM_OPT);         // Initialize the timer/counter waveform.

// Set the compare triggers.
// Remember TC counter is 16-bits, so counting second is not possible with fPBA = 12 MHz.
// We configure it to count ms.
// We want: (1/(fPBA/8)) * RC = 0.001 s, hence RC = (fPBA/8) / 1000 = 1500 to get an interrupt every 1 ms.
	tc_write_rc(tc, UserData->UsedTimerChanel, sysclk_get_pba_hz() / 8 / (pDisplay->Height / UserData->PanelsNr)/ UserData->ColorNr/ UserData->FpsNr); // Set RC value.

	tc_configure_interrupts(tc, UserData->UsedTimerChanel, &TC_INTERRUPT);

// Start the timer/counter.
	tc_start(tc, UserData->UsedTimerChanel);                    // And start the timer/counter.
	return true;
}
//#####################################################
void screen_put_pixel16(tDisplay* LcdStruct, signed int X_Coordonate, signed int Y_Coordonate, unsigned int Color16)
{
	if (X_Coordonate >= LcdStruct->sClipRegion.sXMin && Y_Coordonate >= LcdStruct->sClipRegion.sYMin && X_Coordonate < LcdStruct->sClipRegion.sXMax && Y_Coordonate < LcdStruct->sClipRegion.sYMax)
	{
		ph7_62_RGB_t *_screen_buff = (ph7_62_RGB_t *)LcdStruct->DisplayData;
		ph7_62_RGB_t *screen_buff = &_screen_buff[X_Coordonate + (Y_Coordonate * LcdStruct->Width)];
		screen_buff->B = Color16 & 0x0F;
		screen_buff->G = (Color16 >> 6) & 0x0F;
		screen_buff->R = (Color16 >> 12) & 0x0F;
	}
}
//#####################################################
void screen_fill_area(tDisplay* LcdStruct, signed int x1, signed int y1, signed int x2, signed int y2, unsigned int color)
{
	int16_t _x1 = x1, _x2 = (x1 + x2), _y1 = y1, _y2 = (y1 + y2);
	if(_x1 < 0) _x1 = 0;
	if(_x1 > (int16_t)LcdStruct->Width) _x1 = (int16_t)LcdStruct->Width;
	if(_x2 < 0) _x2 = 0;
	if(_x2 > (int16_t)LcdStruct->Width) _x2 = (int16_t)LcdStruct->Width;
	if(_y1 < 0) _y1 = 0;
	if(_y1 > (int16_t)LcdStruct->Height) _y1 = (int16_t)LcdStruct->Height;
	if(_y2 < 0) _y2 = 0;
	if(_y2 > (int16_t)LcdStruct->Height) _y2 = (int16_t)LcdStruct->Height;
	unsigned char Y_Cnt = 0;
	for(Y_Cnt = _y1; Y_Cnt < _y2; Y_Cnt++)
	{
		unsigned char X_Cnt = 0;
		for(X_Cnt = _x1; X_Cnt < _x2; X_Cnt++)
		{
			screen_put_pixel16(LcdStruct, X_Cnt, Y_Cnt, color);
		}
	}
}
//#####################################################
void screen_clear(tDisplay* LcdStruct, unsigned int color)
{
	screen_fill_area(LcdStruct, 0, 0, LcdStruct->Width, LcdStruct->Height, color);
}
//#####################################################
void screen_draw_rectangle(tDisplay* LcdStruct, signed int x1, signed int y1, signed int x2, signed int y2, bool fill, unsigned int color)
{
	if(fill)
	{
		screen_fill_area(LcdStruct,x1, y1, x2, y2, color);
	}
	else
	{
		int16_t _x1 = x1, _x2 = (x1 + x2)-1, _y1 = y1, _y2 = (y1 + y2)-1;
		if(_x1 < 0) _x1 = 0;
		if(_x1 > (int16_t)LcdStruct->Width) _x1 = (int16_t)LcdStruct->Width;
		if(_x2 < 0) _x2 = 0;
		if(_x2 > (int16_t)LcdStruct->Width) _x2 = (int16_t)LcdStruct->Width;
		if(_y1 < 0) _y1 = 0;
		if(_y1 > (int16_t)LcdStruct->Height) _y1 = (int16_t)LcdStruct->Height;
		if(_y2 < 0) _y2 = 0;
		if(_y2 > (int16_t)LcdStruct->Height) _y2 = (int16_t)LcdStruct->Height;
	}
}
//#####################################################
void screen_put_horizontal_line(tDisplay *LcdStruct, signed int X1, signed int X2, signed int Y, unsigned char width, unsigned int color)
{
	if(Y < 0) return;
	if(Y > (int16_t)LcdStruct->Height) return;
	if(X1 < 0 && X2 < 0) return;
	int16_t _x1 = X1, _x2 = (X1 + X2), _y = Y;
	if(_x1 < 0) _x1 = 0;
	if(_x1 > (int16_t)LcdStruct->Width) _x1 = (int16_t)LcdStruct->Width;
	if(_x2 < 0) _x2 = 0;
	if(_x2 > (int16_t)LcdStruct->Width) _x2 = (int16_t)LcdStruct->Width;
	signed short ScanX;
	signed short Half_width1 = (width>>1);
	signed short Half_width2 = width-Half_width1;
	for(ScanX = _x1; ScanX < _x2; ScanX++)
	{
		for (signed short ScanY = _y - Half_width1; ScanY < _y + Half_width2; ScanY++)
		{
			screen_put_pixel16(LcdStruct, ScanX, ScanY, color);
		}
	}
}
//#####################################################
void screen_put_vertical_line(tDisplay *LcdStruct, signed int Y1, signed int Y2, signed int X, unsigned char width, unsigned int color)
{
	if(X < 0) return;
	if(X > (int16_t)LcdStruct->Width) return;
	if(Y1 < 0 && Y2 < 0) return;
	int16_t _y1 = Y1, _y2 = (Y1 + Y2), _x = X;
	if(_y1 < 0) _y1 = 0;
	if(_y1 > (int16_t)LcdStruct->Height) _y1 = (int16_t)LcdStruct->Height;
	if(_y2 < 0) _y2 = 0;
	if(_y2 > (int16_t)LcdStruct->Height) _y2 = (int16_t)LcdStruct->Height;
	signed short ScanY;
	signed short Half_width1 = (width>>1);
	signed short Half_width2 = width-Half_width1;
	for(ScanY = _y1; ScanY < _y2; ScanY++)
	{
		for (signed short ScanX = _x - Half_width1; ScanX < _x + Half_width2; ScanX++)
		{
			screen_put_pixel16(LcdStruct, ScanX, ScanY, color);
		}
	}
}
//#####################################################
