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
#include "driver/ebi.h"
/*#####################################################*/
#define F_CPU	44000000
extern unsigned long CoreFreq;
#define CoreSourceClock (CoreClk_Source_PLL)
#define CoreClkDivider	1
#define CoreExternClk	false
/*#####################################################*/
#define FLASH_DEVICE
//#define USE_SELECTED_FONTS
#define INT_SIZE_IS_2_BYTES
#define USE_16_BIT_COLOR_DEPTH
#define USE_AVR_STUDIO
#define NO_ENUM_ON_SWITCH
#define USE_RTC_100_MS
#define USED_LCD_DMA	3
#define USED_MMCSD_DMA	2
#define NO_USE_MALIGN
#define Fs_RW
#define USE_FFT_TABLES
#define USE_FFT_BUFFERS	float
#define FFT_BUFFER_SIZE_LOG 8
#define USE_FFT_STATIC_BUFF
/*#####################################################*/
#define USE_FONT_CMSS14B
/*#####################################################*/
#define use_usart4
#define use_usart7
#define use_twi3
#define use_mi0283
#define use_tc1_ovf_int
/*#####################################################*/
#define board_xmega_mediaplayer
/*#####################################################*/
#define Sys_Reset_Port					PORTE
#define Sys_Reset_Pin					0
#define Sys_Reset_Pin_CTRL				PIN0CTRL
//--------------------------------------
#define I2C_Default_Unit				3
#define I2C_Default_Port				PORTF
#define I2C_Default_SDA_Pin				0
#define I2C_Default_SDA_Pin_CTRL		PIN0CTRL
#define I2C_Default_SDA_Pin_CTRL_OPC	PORT_OPC_PULLUP_gc
#define I2C_Default_SCL_Pin				1
#define I2C_Default_SCL_Pin_CTRL		PIN1CTRL
//--------------------------------------
#define uSD_Usart_Unit					7
#define uSD_Usart_Port					PORTF
#define uSD_Usart_MOSI_Pin				7
#define uSD_Usart_MOSI_Pin_CTRL			PIN7CTRL
#define uSD_Usart_MISO_Pin				6
#define uSD_Usart_MISO_Pin_CTRL			PIN6CTRL
#define uSD_Usart_MISO_Pin_CTRL_OPC		PORT_OPC_PULLUP_gc
#define uSD_Usart_SCK_Pin				5
#define uSD_Usart_SCK_Pin_CTRL			PIN5CTRL
#define uSD_CS1_Port					PORTF
#define uSD_CS1_Pin						3
#define uSD_CS1_Pin_pm					(1 << uSD_CS1_Pin)
#define uSD_CS1_Pin_CTRL				PIN2CTRL
#define uSD_CS2_Port					PORTF
#define uSD_CS2_Pin						4
#define uSD_CS2_Pin_pm					(1 << uSD_CS2_Pin)
#define uSD_CS2_Pin_CTRL				PIN3CTRL


#define SD_CS_Port						uSD_CS1_Port
#define SD_CS_Pin						uSD_CS1_Pin
#define SD_SPI_Comunication				uSD_Usart_Unit

#define SD_UseUsartSpi
//#define SD_UseSpi
//--------------------------------------
#define Usb_Usart_Unit					4
#define Usb_Usart_Port					PORTE
#define Usb_Usart_TX_Pin				2
#define Usb_Usart_TX_Pin_CTRL			PIN2CTRL
#define Usb_Usart_TX_Pin_CTRL_OPC		PORT_OPC_PULLUP_gc
#define Usb_Usart_RX_Pin				3
#define Usb_Usart_RX_Pin_CTRL			PIN3CTRL
#define Usb_Usart_RX_Pin_CTRL_OPC		PORT_OPC_PULLUP_gc
//--------------------------------------
#define VS_Usart_Unit					5
#define VS_XCS_Port						PORTE
#define VS_XCS_Pin						1
#define VS_XCS_Pin_CTRL					PIN1CTRL
#define VS_XDCS_Port					PORTE
#define VS_XDCS_Pin						4
#define VS_XDCS_Pin_CTRL				PIN4CTRL
#define VS_SCK_Port						PORTE
#define VS_SCK_Pin						5
#define VS_SCK_Pin_CTRL					PIN5CTRL
#define VS_MISO_Port					PORTE
#define VS_MISO_Pin						6
#define VS_MISO_Pin_CTRL				PIN6CTRL
#define VS_MISO_Pin_CTRL_OPC			PORT_OPC_PULLUP_gc
#define VS_MOSI_Port					PORTE
#define VS_MOSI_Pin						7
#define VS_MOSI_Pin_CTRL				PIN7CTRL
#define VS_DREQ_Port					PORTH
#define VS_DREQ_Pin						3
#define VS_DREQ_Pin_CTRL				PIN3CTRL
#define VS_DREQ_Pin_CTRL_OPC			PORT_OPC_PULLUP_gc
//--------------------------------------
#define MCP73871_PG_Port				PORTQ
#define MCP73871_PG_Pin					2
#define MCP73871_PG_Pin_CTRL			PIN2CTRL
#define MCP73871_PG_Pin_CTRL_OPC		PORT_OPC_PULLUP_gc
#define MCP73871_CE_Port				PORTQ
#define MCP73871_CE_Pin					3
#define MCP73871_CE_Pin_CTRL			PIN3CTRL
#define MCP73871_STAT1_Port				PORTR
#define MCP73871_STAT1_Pin				0
#define MCP73871_STAT1_Pin_CTRL			PIN0CTRL
#define MCP73871_STAT1_Pin_CTRL_OPC		PORT_OPC_PULLUP_gc
#define MCP73871_STAT2_Port				PORTR
#define MCP73871_STAT2_Pin				1
#define MCP73871_STAT2_Pin_CTRL			PIN1CTRL
#define MCP73871_STAT2_Pin_CTRL_OPC		PORT_OPC_PULLUP_gc
//--------------------------------------
#define AR1020_I2C_Unit					I2C_Default_Unit
#define AR1020_Port						I2C_Default_Port
#define AR1020_SDA_Pin					I2C_Default_SDA_Pin
#define AR1020_SDA_Pin_CTRL				I2C_Default_SDA_Pin_CTRL
#define AR1020_SCL_Pin					I2C_Default_SCL_Pin
#define AR1020_SCL_Pin_CTRL				I2C_Default_SCL_Pin_CTRL
#define AR1020_IRQ_Port					PORTF
#define AR1020_IRQ_Pin					2
#define AR1020_IRQ_Pin_CTRL				PIN2CTRL
#define AR1020_IRQ_Pin_CTRL_OPC			PORT_OPC_PULLUP_gc
//--------------------------------------
#define ADXL345_I2C_Unit				I2C_Default_Unit
#define ADXL345_Port					I2C_Default_Port
#define ADXL345_SDA_Pin					I2C_Default_SDA_Pin
#define ADXL345_SDA_Pin_CTRL			I2C_Default_SDA_Pin_CTRL
#define ADXL345_SCL_Pin					I2C_Default_SCL_Pin
#define ADXL345_SCL_Pin_CTRL			I2C_Default_SCL_Pin_CTRL
//--------------------------------------
#define MCP79410_I2C_Unit				I2C_Default_Unit
#define MCP79410_Port					I2C_Default_Port
#define MCP79410_SDA_Pin				I2C_Default_SDA_Pin
#define MCP79410_SDA_Pin_CTRL			I2C_Default_SDA_Pin_CTRL
#define MCP79410_SCL_Pin				I2C_Default_SCL_Pin
#define MCP79410_SCL_Pin_CTRL			I2C_Default_SCL_Pin_CTRL
//--------------------------------------
#define USR_SW_Port						PORTA
#define USR_SW_Pin						1
#define USR_SW_Pin_CTRL					PIN1CTRL
#define USR_SW_Pin_CTRL_OPC				PORT_OPC_PULLUP_gc
//--------------------------------------
#define ExtensionBus_CS0				(0x80000)
#define ExtensionBus_CS1				(0x90000)
#define ExtensionBus_CS2				(0xA0000)
#define ExtensionBus_CS3				(0xB0000)
#define ExtensionBus_CS4				(0xC0000)
#define ExtensionBus_CS5				(0xD0000)
#define ExtensionBus_CS6				(0xE0000)
#define ExtensionBus_CS7				(0xF0000)
//--------------------------------------
#define MI0283QT2_Light_Port			PORTD
#define MI0283QT2_Light_Port_Nr			3
#define MI0283QT2_Light_Pin				0
#define MI0283QT2_Light_Pin_CTRL		PIN0CTRL
#define MI0283QT2_RS_Port				PORTQ
#define MI0283QT2_RS_Pin				1
#define MI0283QT2_RS_Pin_bm				1 << MI0283QT2_RS_Pin
#define MI0283QT2_RS_Pin_CTRL			PIN1CTRL
#define MI0283QT2_Addr					ExtensionBus_CS0
//--------------------------------------
#define SRAM_SIZE						0x100000UL
#define MEMORY_BLOCK					1
#define SRAM_ADDR						(INTERNAL_SRAM_END + 1)//(SRAM_SIZE * MEMORY_BLOCK)
//--------------------------------------
//--------------------------------------
#define set_Default_I2C					I2C_Default_Port.DIRSET = 1<<I2C_Default_SCL_Pin;\
										I2C_Default_Port.DIRCLR = 1<<I2C_Default_SDA_Pin;\
										I2C_Default_Port.I2C_Default_SDA_Pin_CTRL = I2C_Default_SDA_Pin_CTRL_OPC;
//--------------------------------------
#define set_uSD							uSD_CS1_Port.DIRSET = 1<<uSD_CS1_Pin;\
										uSD_CS1_Port.OUTSET = 1<<uSD_CS1_Pin;\
										uSD_CS2_Port.DIRSET = 1<<uSD_CS2_Pin;\
										uSD_CS2_Port.OUTSET = 1<<uSD_CS2_Pin;\
										uSD_Usart_Port.DIRSET = 1<<uSD_Usart_MOSI_Pin || 1<<uSD_Usart_SCK_Pin;\
										uSD_Usart_Port.DIRCLR = 1<<uSD_Usart_MISO_Pin;\
										uSD_Usart_Port.uSD_Usart_MISO_Pin_CTRL = uSD_Usart_MISO_Pin_CTRL_OPC;
//--------------------------------------
#define set_Usb_Usart					Usb_Usart_Port.DIRCLR = 1<<Usb_Usart_TX_Pin;\
										Usb_Usart_Port.DIRSET = 1<<Usb_Usart_RX_Pin;\
										Usb_Usart_Port.Usb_Usart_TX_Pin_CTRL = Usb_Usart_TX_Pin_CTRL_OPC;
//--------------------------------------
#define set_VS							VS_DREQ_Port.DIRCLR = 1<<VS_DREQ_Pin;\
										VS_DREQ_Port.VS_DREQ_Pin_CTRL = VS_DREQ_Pin_CTRL_OPC;\
										VS_MISO_Port.DIRCLR = 1<<VS_MISO_Pin;\
										VS_MISO_Port.VS_MISO_Pin_CTRL = VS_MISO_Pin_CTRL_OPC;\
										VS_MOSI_Port.DIRSET = 1<<VS_MOSI_Pin;\
										VS_MOSI_Port.OUTSET = 1<<VS_MOSI_Pin;\
										VS_SCK_Port.DIRSET = 1<<VS_SCK_Pin;\
										VS_SCK_Port.OUTSET = 1<<VS_SCK_Pin;\
										VS_XCS_Port.DIRSET = 1<<VS_XCS_Pin;\
										VS_XCS_Port.OUTSET = 1<<VS_XCS_Pin;\
										VS_XDCS_Port.DIRSET = 1<<VS_XDCS_Pin;\
										VS_XDCS_Port.OUTSET = 1<<VS_XDCS_Pin;\
										Sys_Reset_Port.DIRSET = 1<<Sys_Reset_Pin;\
										Sys_Reset_Port.OUTSET = 1<<Sys_Reset_Pin;
//--------------------------------------
#define set_MCP73871					MCP73871_PG_Port.DIRCLR = 1<<MCP73871_PG_Pin;\
										MCP73871_PG_Port.MCP73871_PG_Pin_CTRL = MCP73871_PG_Pin_CTRL_OPC;\
										MCP73871_CE_Port.DIRSET = 1<<MCP73871_CE_Pin;\
										MCP73871_CE_Port.OUTSET = 1<<MCP73871_CE_Pin;\
										MCP73871_STAT1_Port.DIRCLR = 1<<MCP73871_STAT1_Pin;\
										MCP73871_STAT1_Port.MCP73871_STAT1_Pin_CTRL = MCP73871_STAT1_Pin_CTRL_OPC;\
										MCP73871_STAT2_Port.DIRCLR = 1<<MCP73871_STAT2_Pin;\
										MCP73871_STAT2_Port.MCP73871_STAT2_Pin_CTRL = MCP73871_STAT2_Pin_CTRL_OPC;
//--------------------------------------
#define set_AR1020						set_Default_I2C\
										AR1020_IRQ_Port.DIRCLR = 1<<AR1020_IRQ_Pin;\
										AR1020_IRQ_Port.AR1020_IRQ_Pin_CTRL = AR1020_IRQ_Pin_CTRL_OPC;
//--------------------------------------
#define set_ADXL345						set_Default_I2C
//--------------------------------------
#define set_MCP79410					set_Default_I2C
//--------------------------------------
#define set_USR_SW						USR_SW_Port.DIRCLR = 1<<USR_SW_Pin;\
										USR_SW_Port.USR_SW_Pin_CTRL = USR_SW_Pin_CTRL_OPC;
//--------------------------------------
#define set_MI0283QT2					MI0283QT2_Light_Port.DIRSET = 1<<MI0283QT2_Light_Pin;\
										MI0283QT2_Light_Port.OUTSET = 1<<MI0283QT2_Light_Pin;\
										MI0283QT2_RS_Port.DIRSET = 1<<MI0283QT2_RS_Pin;\
										MI0283QT2_RS_Port.OUTSET = 1<<MI0283QT2_RS_Pin;

#define sMI0283QT2_LightOn				MI0283QT2_Light_Port.OUTSET = 1<<MI0283QT2_Light_Pin

#define sMI0283QT2_LightOff				MI0283QT2_Light_Port.OUTCLR = 1<<MI0283QT2_Light_Pin
//--------------------------------------
#define set_SRAM						/* Counter indicating correct data transfer to and from SDRAM */\
\
/* Configure bus pins as outputs(except for data lines). */\
										PORTH.DIRSET = 0xFF;\
										PORTK.DIRSET = 0xFF;\
										PORTJ.DIRCLR = 0xFF;\
\
/* Initialize EBI. */\
										EBI_Enable( EBI_SDDATAW_8BIT_gc,\
										EBI_LPCMODE_ALE1_gc,\
										EBI_SRMODE_ALE1_gc,\
										EBI_IFMODE_3PORT_gc );\
\
/* Initialize SRAM */\
										EBI_EnableLPC( &EBI.CS0,				/* Chip Select 0. */\
										EBI_CS_ASPACE_1MB_gc,	/* 1 MB Address space. */\
										SRAM_ADDR,              /* Base address. */\
										2 );                    /* 0 wait states. */


//--------------------------------------
#define set_Board						Sys_Reset_Port.DIRSET = 1<<Sys_Reset_Pin;\
										Sys_Reset_Port.OUTCLR = 1<<Sys_Reset_Pin;\
										_delay_ms(50);\
										Sys_Reset_Port.OUTSET = 1<<Sys_Reset_Pin;\
										_delay_ms(50);\
										set_SRAM\
										set_uSD\
										set_Usb_Usart\
										set_VS\
										set_AR1020\
										set_MCP73871\
										set_ADXL345\
										set_MCP79410\
										set_USR_SW\
										set_MI0283QT2
//--------------------------------------

/*#####################################################*/
#define ph7_62_RGB_Port					PORTD

#define ph7_62_R1_PinPosition			2
#define ph7_62_R1_PinMask				(1 << ph7_62_R1_PinPosition)
#define ph7_62_R2_PinPosition			3
#define ph7_62_R2_PinMask				(1 << ph7_62_R2_PinPosition)

#define ph7_62_G1_PinPosition			4
#define ph7_62_G1_PinMask				(1 << ph7_62_G1_PinPosition)
#define ph7_62_G2_PinPosition			5
#define ph7_62_G2_PinMask				(1 << ph7_62_G2_PinPosition)

#define ph7_62_B1_PinPosition			6
#define ph7_62_B1_PinMask				(1 << ph7_62_B1_PinPosition)
#define ph7_62_B2_PinPosition			7
#define ph7_62_B2_PinMask				(1 << ph7_62_B2_PinPosition)

#define ph7_62_Sclk_Port				PORTD
#define ph7_62_Sclk_PinPosition			1
#define ph7_62_Sclk_PinMask				(1 << ph7_62_Sclk_PinPosition)

#define ph7_62_Latch_Port				PORTC
#define ph7_62_Latch_PinPosition		4
#define ph7_62_Latch_PinMask			(1 << ph7_62_Latch_PinPosition)

#define ph7_62_Oe_Port					PORTC
#define ph7_62_Oe_PinPosition			3
#define ph7_62_Oe_PinMask				(1 << ph7_62_Oe_PinPosition)

#define ph7_62_ABC_Port					PORTC
#define ph7_62_ABC_PinPosition			0
#define ph7_62_ABC_PinMask				(7 << ph7_62_ABC_PinPosition)

#define ph7_62_Width					32
#define ph7_62_Height					16

#define ph7_62_PanelsNr					2
/*#####################################################*/
#endif /* BOARD_PROPERTIES_H_ */
/*#####################################################*/
