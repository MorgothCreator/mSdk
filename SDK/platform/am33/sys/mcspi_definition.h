/*
 * mcspi_def.h
 *
 *  Created on: Jul 15, 2012
 *      Author: Iulian Gheorghiu (morgoth.creator@gmail.com)
 */

#ifndef MCSPI_DEFINITION_H_
#define MCSPI_DEFINITION_H_

typedef volatile unsigned int MCSPI_REGISTER;

typedef struct mcspi_s
{
	MCSPI_REGISTER REVISION;		/* 0x00  McSPI revision register */
	MCSPI_REGISTER RESERVED_0x04_TO_0x10C[(0x110 - 0x4) / sizeof(MCSPI_REGISTER)];
	MCSPI_REGISTER SYSCONFIG;		/* 0x110 McSPI system configuration register */
	MCSPI_REGISTER SYSSTATUS;		/* 0x114 McSPI system status register */
	MCSPI_REGISTER IRQSTATUS;		/* 0x118 McSPI interrupt status register */
	MCSPI_REGISTER IRQENABLE;		/* 0x11C//McSPI interrupt enable register */
	MCSPI_REGISTER RESERVED_0x120;
	MCSPI_REGISTER SYST;			/* 0x124 McSPI system register */
	MCSPI_REGISTER MODULCTRL;		/* 0x128 McSPI module control register */
	MCSPI_REGISTER CH0CONF;			/* 0x12C McSPI channel i configuration register */
	MCSPI_REGISTER CH0STAT;			/* 0x130 McSPI channel i status register */
	MCSPI_REGISTER CH0CTRL;			/* 0x134 McSPI channel i control register */
	MCSPI_REGISTER TX0;				/* 0x138 McSPI channel i FIFO transmit buffer register */
	MCSPI_REGISTER RX0;				/* 0x13C McSPI channel i FIFO receive buffer register */
	MCSPI_REGISTER CH1CONF;			/* 0x140 McSPI channel i configuration register */
	MCSPI_REGISTER CH1STAT;			/* 0x144 McSPI channel i status register */
	MCSPI_REGISTER CH1CTRL;			/* 0x148 McSPI channel i control register */
	MCSPI_REGISTER TX1;				/* 0x14C McSPI channel i FIFO transmit buffer register */
	MCSPI_REGISTER RX1;				/* 0x150 McSPI channel i FIFO receive buffer register */
	MCSPI_REGISTER CH2CONF;			/* 0x154 McSPI channel i configuration register */
	MCSPI_REGISTER CH2STAT;			/* 0x158 McSPI channel i status register */
	MCSPI_REGISTER CH2CTRL;			/* 0x15C McSPI channel i control register */
	MCSPI_REGISTER TX2;				/* 0x160 McSPI channel i FIFO transmit buffer register */
	MCSPI_REGISTER RX2;				/* 0x164 McSPI channel i FIFO receive buffer register */
	MCSPI_REGISTER CH3CONF;			/* 0x168 McSPI channel i configuration register */
	MCSPI_REGISTER CH3STAT;			/* 0x16C McSPI channel i status register register */
	MCSPI_REGISTER CH3CTRL;			/* 0x170 McSPI channel i control register */
	MCSPI_REGISTER TX3;				/* 0x174 McSPI channel i FIFO transmit buffer register */
	MCSPI_REGISTER RX3;				/* 0x178 McSPI channel i FIFO receive buffer register */
	MCSPI_REGISTER XFERLEVEL;		/* 0x17C McSPI transfer levels register */
	MCSPI_REGISTER DAFTX;			/* 0x180 McSPI DMA address aligned FIFO transmitter register */
	MCSPI_REGISTER RESERVED_0x184_TO_0x198[(0x19C - 0x184) / sizeof(MCSPI_REGISTER)];
	MCSPI_REGISTER DAFRX;			/* 0x19C McSPI DMA address aligned FIFO receiver register */
}mcspi_t;

#define SPI0_addr (0x48030000)
#define SPI0 (*(mcspi_t *) 0x48030000)

#define MCSPI0_addr (0x48030000)
#define MCSPI0 (*(mcspi_t *) 0x48030000)

#define SPI1_addr (0x481A0000)
#define SPI1 (*(mcspi_t *) 0x481A0000)

#define MCSPI1_addr (0x481A0000)
#define MCSPI1 (*(mcspi_t *) 0x481A0000)


#endif /* MCSPI_DEF_H_ */
