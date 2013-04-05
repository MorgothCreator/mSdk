/*
*********************************************************************************************************
*                                  BSP, SPI - INITIALIZE SPI MASTER
*
* Description: This function initializes the SPI port in master mode.
*
* Arguments  : com          SPI port: 0..1
*              master       SPI mode: {0} slave, {1} master
*              fdiv         clock selection {0} MCK, {1} MCK/N
*              modfdis      mode fault protection {0} enabled, {1} disabled
*********************************************************************************************************
*/
#include "spi.h"
#include "sys/core.h"
#include "gpio.h"
#include "usart.h"

void  BSP_SPI_Init (unsigned char com, unsigned char master, unsigned char fdiv, unsigned char modfdis)
{
    volatile  avr32_spi_t  *spi;


    switch (com) {
        case 0:
             spi = &AVR32_SPI0;
                                                                        /* Assign GPIO pins to SPI controller                       */
             IO_SetFnct(AVR32_SPI0_SCK_0_PIN,  AVR32_SPI0_SCK_0_FUNCTION);
             IO_SetFnct(AVR32_SPI0_MISO_0_PIN, AVR32_SPI0_MISO_0_FUNCTION);
             IO_SetFnct(AVR32_SPI0_MOSI_0_PIN, AVR32_SPI0_MOSI_0_FUNCTION);

             //IO_SetFnct(AVR32_SPI0_NPCS_0_PIN, AVR32_SPI0_NPCS_0_FUNCTION);
             //IO_SetFnct(AVR32_SPI0_NPCS_1_PIN, AVR32_SPI0_NPCS_1_FUNCTION);
             //IO_SetFnct(AVR32_SPI0_NPCS_2_PIN, AVR32_SPI0_NPCS_2_FUNCTION);
             //IO_SetFnct(AVR32_SPI0_NPCS_3_PIN, AVR32_SPI0_NPCS_3_FUNCTION);
             break;

        case 1:
             spi = &AVR32_SPI1;
                                                                        /* Assign GPIO pins to SPI controller                       */
             IO_SetFnct(AVR32_SPI1_SCK_0_PIN,  AVR32_SPI1_SCK_0_FUNCTION);
             IO_SetFnct(AVR32_SPI1_MISO_0_PIN, AVR32_SPI1_MISO_0_FUNCTION);
             IO_SetFnct(AVR32_SPI1_MOSI_0_PIN, AVR32_SPI1_MOSI_0_FUNCTION);

             //IO_SetFnct(AVR32_SPI1_NPCS_0_PIN, AVR32_SPI1_NPCS_0_FUNCTION);
             //IO_SetFnct(AVR32_SPI1_NPCS_1_PIN, AVR32_SPI1_NPCS_1_FUNCTION);
            // IO_SetFnct(AVR32_SPI1_NPCS_2_PIN, AVR32_SPI1_NPCS_2_FUNCTION);
             //IO_SetFnct(AVR32_SPI1_NPCS_3_PIN, AVR32_SPI1_NPCS_3_FUNCTION);
             break;

        default:
             return;
    }

    spi->cr =  AVR32_SPI_CR_SWRST_MASK;                                 /* reset SPI controller                                     */
    spi->mr = (master  << AVR32_SPI_MR_MSTR_OFFSET) |                   /* enable master/slave mode                                 */
              (fdiv    << AVR32_SPI_MR_FDIV_OFFSET) |                   /* clock selection {0} MCK, {1} MCK/N                       */
              (modfdis << AVR32_SPI_MR_MODFDIS_OFFSET);                 /* mode fault protection {0} enabled, {1} disabled          */
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                  BSP, SPI - SELECTION MODE
*
* Description: This function sets slave chip selection in master mode.
*
* Arguments  : com          SPI port: 0..1
*              ps           peripheral select {0} fixed, {1} variable
*              pcsdec       chip select decode {0} direct, {1} 4-to-16 dec
*              dly_bcs      delay between chip selects
*********************************************************************************************************
*/

void  BSP_SPI_SelectionMode (unsigned char com, char ps, char pcsdec, unsigned char dly_bcs)
{
    volatile  avr32_spi_t  *spi;


    switch (com) {
        case 0:
             spi = &AVR32_SPI0;
             break;

        case 1:
             spi = &AVR32_SPI1;
             break;

        default:
             return;
    }

    spi->mr |= (ps      << AVR32_SPI_MR_PS_OFFSET)     |                /* peripheral select                                        */
               (pcsdec  << AVR32_SPI_MR_PCSDEC_OFFSET) |                /* chip select decode                                       */
               (dly_bcs << AVR32_SPI_MR_DLYBCS_OFFSET);                 /* delay between chip selects                               */
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                  BSP, SPI - SELECT CHIP
*
* Description: This function selects slave chip.
*
* Arguments  : com          SPI port: 0..1
*              cs           chip select number. {0-15} with 4-to-16 decoder, {0-3} direct
*********************************************************************************************************
*/

unsigned char  BSP_SPI_ChipSel (unsigned char com, unsigned char cs)
{
    volatile  avr32_spi_t  *spi;


    switch (com) {
        case 0:
             spi = &AVR32_SPI0;
             break;

        case 1:
             spi = &AVR32_SPI1;
             break;

        default:
             return (0);
    }

    spi->mr     |=  0xF << AVR32_SPI_MR_PCS_OFFSET;                     /* no peripheral is selected                                */

    if ((spi->mr & AVR32_SPI_MR_PCSDEC_MASK) != 0) {                    /* chip select is in 4-to-16 decoder mode                   */
        if (cs > 15) {
            return (0);
        }

        spi->mr |= cs << AVR32_SPI_MR_PCS_OFFSET;
    } else {
        if (cs > 3) {
            return (0);
        }

        spi->mr &= ~(1 << (AVR32_SPI_MR_PCS_OFFSET + cs));
    }

    return (1);
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                  BSP, SPI - SETUP CHIP SELECT
*
* Description: This function sets options for slave chip.
*
* Arguments  : com          SPI port: 0..1
*              reg          SPI chip select register: 0..3
*              mode         SPI mode. Polarity and phase combination for data transfer.
*              baudrate     SPI baudrate
*              bits         number of bits in each character
*              active       chip stays active after last transfer
*              dly_start    amount of delay before first clock pulse
*              dly_trans    amount of delay between each transfer
*********************************************************************************************************
*/

unsigned char  BSP_SPI_ChipSelSetup (unsigned char com, unsigned char reg, unsigned char mode, unsigned long baudrate, unsigned char bits, char active, unsigned char dly_start, unsigned char dly_trans)
{
    volatile  avr32_spi_t  *spi;
    unsigned short    cd;
    unsigned short    csr;


    switch (com) {
        case 0:
             spi = &AVR32_SPI0;
             break;

        case 1:
             spi = &AVR32_SPI1;
             break;

        default:
             return (1);
    }

    if (reg > 3) {                                                      /* check if SPI chip select is valid                        */
        return (2);
    }

    if (mode > 3) {                                                     /* check if SPI mode is valid                               */
        return (3);
    }

    if ((bits > 16) || (bits < 8)) {                                    /* check if bits per transfer is in a valid range           */
        return (4);
    }

    if ((spi->mr & AVR32_SPI_MR_FDIV_MASK) == 0) {
        cd = Get_PBA_Clk() / baudrate;
    } else {
        cd = Get_PBA_Clk() / (32 * baudrate);
    }

    if (cd > 255) {                                                     /* check if clock divider is within valid range             */
        return (5);
    }

    csr    =  (active    << AVR32_SPI_CSR0_CSAAT_OFFSET) |
             ((bits - 8) << AVR32_SPI_CSR0_BITS_OFFSET)  |
              (cd        << AVR32_SPI_CSR0_SCBR_OFFSET)  |
              (dly_start << AVR32_SPI_CSR0_DLYBS_OFFSET) |
              (dly_trans << AVR32_SPI_CSR0_DLYBCT_OFFSET);

    switch (mode) {
        case 0:
             csr |=   1 << AVR32_SPI_CSR0_NCPHA_OFFSET;
             csr &= ~(1 << AVR32_SPI_CSR0_CPOL_OFFSET);
             break;

        case 1:
             csr &= ~(1 << AVR32_SPI_CSR0_NCPHA_OFFSET);
             csr &= ~(1 << AVR32_SPI_CSR0_CPOL_OFFSET);
             break;

        case 2:
             csr |=   1 << AVR32_SPI_CSR0_NCPHA_OFFSET;
             csr &= ~(1 << AVR32_SPI_CSR0_CPOL_OFFSET);
             break;

        case 3:
             csr &= ~(1 << AVR32_SPI_CSR0_NCPHA_OFFSET);
             csr |=   1 << AVR32_SPI_CSR0_CPOL_OFFSET;
             break;
    }

    switch (reg) {
        case 0:
             spi->csr0 = csr;
             break;

        case 1:
             spi->csr1 = csr;
             break;

        case 2:
             spi->csr2 = csr;
             break;

        case 3:
             spi->csr3 = csr;
             break;
    }

    return (0);
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                  BSP, SPI - ENABLE
*
* Description: This function enables SPI to transfer and receive data.
*
* Arguments  : com          SPI port: 0..1
*********************************************************************************************************
*/

void  BSP_SPI_En (unsigned char com)
{
    volatile  avr32_spi_t  *spi;


    switch (com) {
        case 0:
             spi = &AVR32_SPI0;
             break;

        case 1:
             spi = &AVR32_SPI1;
             break;

        default:
             return;
    }

    spi-> cr = 1 << AVR32_SPI_CR_SPIEN_OFFSET;
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                  BSP, SPI - DISABLE
*
* Description: This function disables SPI.
*
* Arguments  : com          SPI port: 0..1
*
* Note(s)    : If a transfer is in progress, the transfer is finished before the SPI is disabled.
*********************************************************************************************************
*/

void  BSP_SPI_Dis (unsigned char com)
{
    volatile  avr32_spi_t  *spi;


    switch (com) {
        case 0:
             spi = &AVR32_SPI0;
             break;

        case 1:
             spi = &AVR32_SPI1;
             break;

        default:
             return;
    }

    spi-> cr = 1 << AVR32_SPI_CR_SPIDIS_OFFSET;
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                  BSP, SPI - WRITE
*
* Description: This function selects slave and writes a block of data.
*
* Arguments  : com          SPI port: 0..1
*              data         data block to be transfered
*              cs           chip select
*              lastxfer     last transfer on the slave chip
*
* Return     : {1}          if SPI # is not valid or timeout has occured in the transfer.
*              {0}          transfer successfully completed
*********************************************************************************************************
*/

unsigned char  BSP_SPI_Wr (unsigned char com, unsigned short data, unsigned char cs, unsigned char lastxfer)
{
    volatile  avr32_spi_t  *spi;
    unsigned short    timeout;


    switch (com) {
        case 0:
             spi = &AVR32_SPI0;
             break;

        case 1:
             spi = &AVR32_SPI1;
             break;

        default:
             return (1);
    }

    timeout = 10000;

    while ((spi->sr & AVR32_SPI_SR_TXEMPTY_MASK) == 0 && (timeout > 0)) {
        timeout--;
    }

    if (timeout == 0) {
    	uart_printf("SPI wr: timeout\n");
        return(2);
    }

    spi->tdr = (data     << AVR32_SPI_TDR_TD_OFFSET)  |
               (cs       << AVR32_SPI_TDR_PCS_OFFSET) |
               (lastxfer << AVR32_SPI_TDR_LASTXFER_OFFSET);

    return (0);
}




unsigned char  SPI_Write ( unsigned short data, unsigned char cs, unsigned char lastxfer)
{
    volatile  avr32_spi_t  *spi;
    unsigned short    timeout;

             spi = &AVR32_SPI0;


    timeout = 10000;

    while ((spi->sr & AVR32_SPI_SR_TXEMPTY_MASK) == 0 && (timeout > 0)) {
        timeout--;
    }

    if (timeout == 0) {
    	uart_printf("SPI write: timeout\n");
        return(2);
    }
    //uart_printf("SPI: Writing 0x%x\n",data);
    spi->tdr = (data     << AVR32_SPI_TDR_TD_OFFSET)  |
               (cs       << AVR32_SPI_TDR_PCS_OFFSET) |
               (lastxfer << AVR32_SPI_TDR_LASTXFER_OFFSET);

    return (0);
}



/*$PAGE*/
/*
*********************************************************************************************************
*                                  BSP, SPI - READ
*
* Description: This function reads a block of data from selected slave.
*
* Arguments  : com          SPI port: 0..1
*              data         data block to be read
*              cs           chip select value at the end of a transfer (master mode only)
*
* Return     : {0}          if SPI # is not valid or timeout has occured in the transfer.
*              {1}          transfer successfully completed
*********************************************************************************************************
*/

unsigned char  BSP_SPI_Rd (unsigned char com, unsigned short *data, unsigned char *cs)
{
    volatile  avr32_spi_t  *spi;
    unsigned short    timeout;
    unsigned short    tdr;


    switch (com) {
        case 0:
             spi = &AVR32_SPI0;
             break;

        case 1:
             spi = &AVR32_SPI1;
             break;

        default:
             return (0);
    }

    timeout = 10000;

    while ((spi->sr & AVR32_SPI_SR_RDRF_MASK) == 0 && (timeout > 0)) {
        timeout--;
    }

    if (timeout == 0) {
    	uart_printf("SPI rd: timeout\n");
        return(0);
    }

    tdr   =  spi->tdr;

    *data = (tdr & AVR32_SPI_TDR_TD_MASK)  >> AVR32_SPI_TDR_TD_OFFSET;
    *cs   = (tdr & AVR32_SPI_TDR_PCS_MASK) >> AVR32_SPI_TDR_PCS_OFFSET;

    return (1);
}





unsigned char spi_readRegisterFullCheck(volatile avr32_spi_t * spi)
{
	if ((spi->sr & AVR32_SPI_SR_RDRF_MASK) != 0) {
		return 1;
	} else {
		return 0;
	}
}






unsigned char  BSP_SPI_Read ()
{
    volatile  avr32_spi_t  *spi= &AVR32_SPI0;
    unsigned long    timeout;
              unsigned short    rdr;
              unsigned char data;





    timeout = 1000000;
    while(spi_readRegisterFullCheck(spi) != 1){
    	uart_printf("SPI read: waiting for data\n");
    }
    while ((spi->sr & AVR32_SPI_SR_RDRF_MASK) == 0 && (timeout > 0)) {
        timeout--;
    }

    if (timeout == 0) {
    	uart_printf("SPI read: timeout\n");
        return(1);
    }

    rdr   =  spi->rdr;
    uart_printf("SPI: reading 0x%08x\n",rdr);
    data = (rdr & AVR32_SPI_TDR_TD_MASK)  >> AVR32_SPI_TDR_TD_OFFSET;
    //*cs   = (rdr & AVR32_SPI_TDR_PCS_MASK) >> AVR32_SPI_TDR_PCS_OFFSET;

    return (data);
}




void spi_getStatus()
{
	volatile  avr32_spi_t  *spi = &AVR32_SPI0;

	if ((spi->sr & AVR32_SPI_SR_OVRES_MASK) != 0) {
		 uart_printf("SPI_ERROR_OVERRUN\n");
	}

	if ((spi->sr & AVR32_SPI_SR_MODF_MASK) != 0) {
		uart_printf("SPI_ERROR_MODE_FAULT\n");
	}

		 uart_printf("status=0x08%x\n",spi->sr);

return;
}

