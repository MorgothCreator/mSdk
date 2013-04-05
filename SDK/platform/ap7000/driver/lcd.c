/*
 * Psp_Display.c
 *
 *  Created on: Feb 2, 2011
 *      Author: Nimeni
 */

#include "avr32/ap7000.h"
#include "malloc.h"
#include "string.h"
#include "stdlib.h"
#include "lcd.h"
#include "gpio.h"
#include "api/lcd_def.h"
//#include <Core.h>

volatile  avr32_lcdc_t  *lcdc = &AVR32_LCDC;

void  LCDC_InitHW (void)
{
	volatile avr32_pio_t *pioa = &AVR32_PIOA;
																		/* Assign GPIO pins to LCD controller                       */

    IO_SetFnct(AVR32_LCDC_HSYNC_0_PIN,   AVR32_LCDC_HSYNC_0_FUNCTION);
    IO_SetFnct(AVR32_LCDC_PCLK_0_PIN,    AVR32_LCDC_PCLK_0_FUNCTION);
    IO_SetFnct(AVR32_LCDC_VSYNC_0_PIN,   AVR32_LCDC_VSYNC_0_FUNCTION);

    IO_SetFnct(AVR32_LCDC_DATA_0_1_PIN,  AVR32_LCDC_DATA_0_1_FUNCTION);
    IO_SetFnct(AVR32_LCDC_DATA_1_1_PIN,  AVR32_LCDC_DATA_1_1_FUNCTION);
    IO_SetFnct(AVR32_LCDC_DATA_2_1_PIN,  AVR32_LCDC_DATA_1_1_FUNCTION);
    IO_SetFnct(AVR32_LCDC_DATA_3_1_PIN,  AVR32_LCDC_DATA_1_1_FUNCTION);
    IO_SetFnct(AVR32_LCDC_DATA_4_1_PIN,  AVR32_LCDC_DATA_1_1_FUNCTION);
    IO_SetFnct(AVR32_LCDC_DATA_5_PIN,    AVR32_LCDC_DATA_5_FUNCTION);
    IO_SetFnct(AVR32_LCDC_DATA_6_PIN,    AVR32_LCDC_DATA_6_FUNCTION);
    IO_SetFnct(AVR32_LCDC_DATA_7_PIN,    AVR32_LCDC_DATA_7_FUNCTION);
    IO_SetFnct(AVR32_LCDC_DATA_8_1_PIN,  AVR32_LCDC_DATA_8_1_FUNCTION);
    IO_SetFnct(AVR32_LCDC_DATA_9_1_PIN,  AVR32_LCDC_DATA_9_1_FUNCTION);
    IO_SetFnct(AVR32_LCDC_DATA_10_1_PIN, AVR32_LCDC_DATA_10_1_FUNCTION);
    IO_SetFnct(AVR32_LCDC_DATA_11_1_PIN, AVR32_LCDC_DATA_11_1_FUNCTION);
    IO_SetFnct(AVR32_LCDC_DATA_12_1_PIN, AVR32_LCDC_DATA_12_1_FUNCTION);
    IO_SetFnct(AVR32_LCDC_DATA_13_PIN,   AVR32_LCDC_DATA_13_FUNCTION);
    IO_SetFnct(AVR32_LCDC_DATA_14_PIN,   AVR32_LCDC_DATA_14_FUNCTION);
    IO_SetFnct(AVR32_LCDC_DATA_15_PIN,   AVR32_LCDC_DATA_15_FUNCTION);
    IO_SetFnct(AVR32_LCDC_DATA_16_1_PIN, AVR32_LCDC_DATA_16_1_FUNCTION);
    IO_SetFnct(AVR32_LCDC_DATA_17_1_PIN, AVR32_LCDC_DATA_17_1_FUNCTION);
    IO_SetFnct(AVR32_LCDC_DATA_18_1_PIN, AVR32_LCDC_DATA_18_1_FUNCTION);
    IO_SetFnct(AVR32_LCDC_DATA_19_1_PIN, AVR32_LCDC_DATA_19_1_FUNCTION);
    IO_SetFnct(AVR32_LCDC_DATA_20_1_PIN, AVR32_LCDC_DATA_20_1_FUNCTION);
    IO_SetFnct(AVR32_LCDC_DATA_21_1_PIN, AVR32_LCDC_DATA_21_1_FUNCTION);
    IO_SetFnct(AVR32_LCDC_DATA_22_PIN,   AVR32_LCDC_DATA_22_FUNCTION);
    IO_SetFnct(AVR32_LCDC_DATA_23_PIN,   AVR32_LCDC_DATA_23_FUNCTION);


    pioa->per  |= LCD_ON; // PIO Controller PIO Enable Register
    pioa->oer  |= LCD_ON; // PIO Controller Output Enable Register
    pioa->idr  |= LCD_ON; // PIO Controller Interrupt Disable Register
    pioa->puer |= LCD_ON; // PIO Pull Up Enable Register
    pioa->sodr |= LCD_ON; // TURN ON LCD AND BACKLIGHT



    /* SLOT_CYCLE: max burst cycles (slow bursts can be broken) */
    /* DEFMSTR_TYPE: 0- no default, 1- last default, 2- fixed   */
    /* FIXED_DEFMSTR: master number (5 = LCD Controller)        */
    /* ARBT: 0- round robin, 1- fixed priority                  */
    AVR32_HMATRIX.scfg[4] = ((0x40 << AVR32_HMATRIX_SCFG4_SLOT_CYCLE_OFFSET)    & AVR32_HMATRIX_SCFG4_SLOT_CYCLE_MASK)    |
    						(( 2 << AVR32_HMATRIX_SCFG4_DEFMSTR_TYPE_OFFSET)  & AVR32_HMATRIX_SCFG4_DEFMSTR_TYPE_MASK)  |
    						(( 5 << AVR32_HMATRIX_SCFG4_FIXED_DEFMSTR_OFFSET) & AVR32_HMATRIX_SCFG4_FIXED_DEFMSTR_MASK) |
                            (( 1 << AVR32_HMATRIX_SCFG4_ARBT_OFFSET)          & AVR32_HMATRIX_SCFG4_ARBT_MASK);

    AVR32_HMATRIX.prs[4].pras  = 0x00F00000;                            /* set maximum priority on HSB bus matrix to LCD controller */

    AVR32_PM_HSBMASK_reg      |= 0x00000080;                            /* enable LCD controller clock line in HSB           100       */
    AVR32_PM.gcctrl[7]        |= 0x00000006;                            /* enable generic clock PLL1 for LCD controller pixel clock 7 */
}



/*
*********************************************************************************************************
*                                  BSP, LCDC - INITIALIZE
*
* Description: This function initializes the LCD controller.
*
* Arguments  : settings   LCD settings for the LCD controller
*********************************************************************************************************
*/
//extern struct disp_res DisplayResolution;

int  LCDC_Init (BSP_LCDC_HDR *settings)
{

	unsigned char     datalines;
	unsigned short     lineval;
	unsigned short     hozval;
    unsigned long     pclk;
    unsigned long     framesize;
    unsigned char     pixel_size;
    unsigned char     clkval;


    lcdc          = &AVR32_LCDC;
    datalines     = 0;
    pixel_size    = 0;
    clkval        = 0;

    lcdc->pwrcon &= ~(1 << AVR32_LCDC_PWRCON_PWR_OFFSET);               /* Turn off LCD Controller (core first) ...                 */
    lcdc->dmacon &= ~(1 << AVR32_LCDC_DMACON_DMAEN_OFFSET);             /* ... then DMA controller                                  */



             hozval =  settings->ResX - 1;
             pclk   =  settings->FrameRate * settings->ResX * settings->ResY;


    lineval         =  settings->ResY - 1;
    lcdc->lcdfrmcfg = ((lineval<< AVR32_LCDC_LINEVAL) & AVR32_LCDC_LINEVAL_MASK) |
                      ((hozval << AVR32_LCDC_HOZVAL)  & AVR32_LCDC_HOZVAL_MASK);
    clkval          = (Get_CPU_Clk() / (2 * pclk));


    if (clkval == 0) {
        lcdc->lcdcon1 = 1;                                              /* bypass pixel clock                                       */
    } else {
        lcdc->lcdcon1 = ((clkval - 1) << AVR32_LCDC_LCDCON1_CLKVAL) & AVR32_LCDC_LCDCON1_CLKVAL_MASK;
    }

    switch(settings->PixelSize){
        case 1:
             pixel_size = 0;
             break;

        case 2:
             pixel_size = 1;
             break;

        case 4:
             pixel_size = 2;
             break;

        case 8:
             pixel_size = 3;
             break;

        case 16:
             pixel_size = 4;
             break;

        case 24:
             pixel_size = 5;
             break;

        case 32:
             pixel_size = 6;
             break;

        default:
             return 0;
    }

    lcdc->lcdcon2 = ((settings->DispType               << AVR32_LCDC_LCDCON2_DISTYPE)    & AVR32_LCDC_LCDCON2_DISTYPE_MASK)   |
                    ((settings->EnDualScanMode         << AVR32_LCDC_LCDCON2_SCANMOD)    & AVR32_LCDC_LCDCON2_SCANMOD_MASK)   |
                    ((settings->IFWidth                << AVR32_LCDC_LCDCON2_IFWIDTH)    & AVR32_LCDC_LCDCON2_IFWIDTH_MASK)   |
                    ((settings->InvPolarityData        << AVR32_LCDC_LCDCON2_INVVD)      & AVR32_LCDC_LCDCON2_INVVD_MASK)     |
                    ((settings->InvPolarityVSync       << AVR32_LCDC_LCDCON2_INVFRAME)   & AVR32_LCDC_LCDCON2_INVFRAME_MASK)  |
                    ((pixel_size                       << AVR32_LCDC_LCDCON2_PIXELSIZE)  & AVR32_LCDC_LCDCON2_PIXELSIZE_MASK) |
                    ((settings->InvPolarityHSync       << AVR32_LCDC_LCDCON2_INVLINE)    & AVR32_LCDC_LCDCON2_INVLINE_MASK)   |
                    ((settings->InvPolarityPixelClk    << AVR32_LCDC_LCDCON2_INVCLK)     & AVR32_LCDC_LCDCON2_INVCLK_MASK)    |
                    ((settings->InvPolarityDataValid   << AVR32_LCDC_LCDCON2_INVDVAL)    & AVR32_LCDC_LCDCON2_INVDVAL_MASK)   |
                    ((settings->EnPixelClkAlwaysActive << AVR32_LCDC_LCDCON2_CLKMOD)     & AVR32_LCDC_LCDCON2_CLKMOD_MASK)    |
                    ((settings->MemFormat              << AVR32_LCDC_LCDCON2_MEMOR)      & AVR32_LCDC_LCDCON2_MEMOR_MASK);

                                                                        /* Timings settings                                         */
    lcdc->lcdtim1 = ((settings->VFP         << AVR32_LCDC_LCDTIM1_VFP)        & AVR32_LCDC_LCDTIM1_VFP_MASK) |
                    ((settings->VBP         << AVR32_LCDC_LCDTIM1_VBP)        & AVR32_LCDC_LCDTIM1_VBP_MASK) |
                    (((settings->VPW - 1)   << AVR32_LCDC_LCDTIM1_VPW)        & AVR32_LCDC_LCDTIM1_VPW_MASK) |
                    ((settings->VHDly       << AVR32_LCDC_LCDTIM1_VHDLY)      & AVR32_LCDC_LCDTIM1_VHDLY_MASK);

    lcdc->lcdtim2 = ((settings->HBP         << AVR32_LCDC_LCDTIM2_HBP)        & AVR32_LCDC_LCDTIM2_HBP_MASK) |
                    (((settings->HPW - 1)   << AVR32_LCDC_LCDTIM2_HPW)        & AVR32_LCDC_LCDTIM2_HPW_MASK) |
                    ((settings->HFP         << AVR32_LCDC_LCDTIM2_HFP)        & AVR32_LCDC_LCDTIM2_HFP_MASK);

    lcdc->idr     = 0xFFFFFFFF;

    lcdc->lcdmval = ((settings->ModeToggleRate   << AVR32_LCDC_LCDMVAL_MVAL)  & AVR32_LCDC_LCDMVAL_MVAL_MASK) |
                    ((settings->EnModeToggleRate << AVR32_LCDC_LCDMVAL_MMODE) & AVR32_LCDC_LCDMVAL_MMODE_MASK);

    lcdc->contrast_val = settings->ContrastVal;
    lcdc->contrast_ctr = ((settings->ContrastPreScaler   << AVR32_LCDC_CONTRAST_CTR_PS)  & AVR32_LCDC_CONTRAST_CTR_PS_MASK)  |
                         ((settings->InvPolarityContrast << AVR32_LCDC_CONTRAST_CTR_POL) & AVR32_LCDC_CONTRAST_CTR_POL_MASK) |
                         ((settings->EnContrast          << AVR32_LCDC_CONTRAST_CTR_ENA) & AVR32_LCDC_CONTRAST_CTR_ENA_MASK);

    if (settings->EnDualScanMode == 1) {                                /* Setup FIFO                                               */
        lcdc->lcdfifo   = 256 - (2 * settings->DMABurstLen + 3);
        lcdc->dmabaddr1 = settings->DMABaseAddr1;
        lcdc->dmabaddr2 = settings->DMABaseAddr2;
    } else {
        lcdc->lcdfifo   = 512 - (2 * settings->DMABurstLen + 3);
        lcdc->dmabaddr1 = settings->DMABaseAddr1;
    }

                                                                        /* DMA frame configuration                                  */
    framesize  = settings->ResX * settings->ResY * settings->PixelSize;
    framesize += 31;
    framesize /= 32;

    /* Frame size in words (rounded up)                         */







    lcdc->dmafrmcfg = (framesize & AVR32_LCDC_DMAFRMCFG_FRMSIZE_MASK) |
                      (((settings->DMABurstLen - 1) << AVR32_LCDC_DMAFRMCFG_BRSTLEN) & AVR32_LCDC_DMAFRMCFG_BRSTLEN_MASK);

    if(settings->En2DMode){                                             /* 2D configuration                                         */
                                                                        /* Frame starts at word boundary (no pixel offset needed)   */
        lcdc->dma2dcfg = ((settings->VirtualResX - settings->ResX) * (settings->PixelSize / 8)) & AVR32_LCDC_DMA2DCFG_ADDRINC_MASK;
    }

    while (lcdc->dmacon & AVR32_LCDC_DMACON_DMABUSY) {                  /* wait DMA engine to become idle                           */
        ;
    }

    if(settings->En2DMode){                                             /* enable DMA with updated configuration                    */
        lcdc->dmacon  = (1 << AVR32_LCDC_DMACON_DMAEN_OFFSET)   |
                        (1 << AVR32_LCDC_DMACON_DMAUPDT_OFFSET) |
                        (1 << AVR32_LCDC_DMACON_DMA2DEN_OFFSET);
    }
    else{
        lcdc->dmacon  = (1 << AVR32_LCDC_DMACON_DMAEN_OFFSET)   |
                        (1 << AVR32_LCDC_DMACON_DMAUPDT_OFFSET);
    }

    lcdc->pwrcon     |= (settings->GuardTime << AVR32_LCDC_PWRCON_GUARD_TIME) &
                                                AVR32_LCDC_PWRCON_GUARD_TIME_MASK;

    while (lcdc->PWRCON.busy == 1) {                                    /* Wait LCDC core to become idle                            */
        ;
    }
                                                                        /* Enable LCD                                               */
    lcdc->PWRCON.pwr = 1;
    return 0;
}




void lcd_set_mode(tDisplay* LcdStruct)
{
    //DisplayResolution.Xres = 800;
    //DisplayResolution.Yres = 480;

    unsigned long ps;
	lcd.DMABaseAddr2			= 0;
	lcd.DMABurstLen 			= 16;
	lcd.ResX 					= LcdStruct->Width;
	lcd.ResY 					= LcdStruct->Height;
	lcd.En2DMode 				= 0;
	lcd.VirtualResX 			= LcdStruct->Width;
	lcd.VirtualResY 			= LcdStruct->Height;
	lcd.FrameRate 				= 40;
	lcd.LCDCClk 				= 150000000;		// not used
	lcd.GuardTime 				= 2;
	lcd.MemFormat 				= LCD_BIG_ENDIAN;
	lcd.IFWidth					= LCD_IFWIDTH_4; 	// not used
	lcd.EnDualScanMode 			= 0;	 	// not used
	lcd.DispType 				= LCD_TFT;
	lcd.InvPolarityDataValid 	= INVERTED;
	lcd.InvPolarityData 		= NORMAL;
	lcd.InvPolarityHSync 	 	= INVERTED;
	lcd.InvPolarityVSync 	 	= INVERTED;
	lcd.InvPolarityPixelClk  	= NORMAL;
	lcd.EnPixelClkAlwaysActive 	= ALWAYS_ACTIVE;
	lcd.PixelSize 			 	= 24;
	lcd.ContrastVal				= 0x0f;				// not used
	lcd.InvPolarityContrast		= INVERTED;			// not used
	lcd.EnContrast				= PRE_EIGTH;		// not used
	lcd.EnModeToggleRate		= 0;				// not used
	lcd.ModeToggleRate 	 		= EACH_FRAME;		// not used
	switch(LcdStruct->LcdType)
	{
	case AT070TN92:
		lcd.HPW 					= 40;
		lcd.HBP 					= 5;
		lcd.HFP 					= 255;
		lcd.VPW 					= 9;
		lcd.VBP 					= 23;
		lcd.VFP 					= 22;
		lcd.VHDly 					= 0;
		break;
	default:
		lcd.HPW 					= 40;
		lcd.HBP 					= 5;
		lcd.HFP 					= 255;
		lcd.VPW 					= 9;
		lcd.VBP 					= 23;
		lcd.VFP 					= 22;
		lcd.VHDly 					= 0;
		break;
	}


	switch(lcd.PixelSize)
	{
		case BPP_8: ps=1; break;
		case BPP_16: ps=2; break;
		case BPP_24: ps=3; break;
		case BPP_32: ps=4; break;
		default:ps=0; break;
	}

	fb_size=lcd.ResX*lcd.ResY*ps;
	//uart_printf("size =%#x \n",fb_size);

	lcd.active_buffer=(void*)memalign(32, fb_size);
	//if(!lcd.active_buffer) lcd.active_buffer=(void*)memalign(32, fb_size);
	//lcd.background_buffer=(void*)memalign(32, fb_size);
	//if(!lcd.background_buffer) lcd.background_buffer=(void*)memalign(32, fb_size);

	lcd.DMABaseAddr1 = (unsigned long)lcd.active_buffer;

	memset(lcd.active_buffer,0x00,fb_size);
	//memset(lcd.background_buffer,0x00,fb_size);
	//uart_printf("LCD Active buffer at     %d size =%d \n", (unsigned int) lcd.active_buffer-0x10000000,fb_size);
	//uart_printf("LCD Background buffer at %d size =%d \n", (unsigned int) lcd.background_buffer-0x10000000,fb_size);
	lcd_line_length = (lcd.ResX * lcd.PixelSize) / 8;
	LcdStruct->DisplayData = (unsigned int*)lcd.active_buffer;
	LCDC_Init (&lcd);
}


void lcd_init(tDisplay* LcdStruct)
{
	LCDC_InitHW();
	lcd_set_mode(LcdStruct);
}
