/*
 * board_properties.h
 *
 * Created: 2/8/2013 12:23:12 AM
 *  Author: XxXx
 */ 
/*#####################################################*/
#ifndef BOARD_PROPERTIES_H_
#define BOARD_PROPERTIES_H_
/*#####################################################*/
#define F_CPU	40000000
extern unsigned long CoreFreq;
#define SourceClock (CoreClk_Source_PLL)
#define CoreClkDivider 1
#define CoreExternClk	false
/*#####################################################*/
#define FLASH_DEVICE
//#define USE_SELECTED_FONTS
#define INT_SIZE_IS_2_BYTES
#define USE_16_BIT_COLOR_DEPTH
#define NO_ENUM_ON_SWITCH
#define USE_RTC_100_MS
#define USED_LCD_DMA	3
#define NO_USE_MALIGN

#define FFT_BUFFER_SIZE_LOG		7
#define USE_FFT_TABLES
#define USED_FFT_TEMP_BUFFERS			double
#define USE_FFT_STATIC_BUFF
#define FFT_INPUT_DATA_TYPE		signed char
#define FFT_OUTPUT_DATA_TYPE	signed long
/*#####################################################*/
//#define USE_G_SFONT_CMSS_14B
/*#####################################################*/
#define use_twi0
#define use_usart1
#define use_usart3
#define use_usart4

#define use_adca_ch0
#define use_adca_ch1

#define use_tc1_ovf_int
#define use_tc1_cca_int


#define use_20x20MatrixLedUsartSpiDma
/*#####################################################*/
#define BTN1_Port						4
#define BTN1_bp							0

#define BTN2_Port						10
#define BTN2_bp							1

#define BTN3_Port						10
#define BTN3_bp							0

#define PWR_STBY_Port					4
#define PWR_STBY_bp						1

#define USB_CONNECT_Port				3
#define USB_CONNECT_bp					0

#define SCLR_Port						PORTD
#define SCLR_bp							1
#define SCLR_bm							(1 << SCLR_bp)

#define SCK_Port						PORTD
#define SCK_bp							5
#define SCK_bm							(1 << SCK_bp)

#define SCS_Port						PORTD
#define SCS_bp							4
#define SCS_bm							(1 << SCS_bp)
/*#####################################################*/
#define board_xmega_mediaplayer
/*#####################################################*/
/*#####################################################*/
#endif /* BOARD_PROPERTIES_H_ */
/*#####################################################*/
