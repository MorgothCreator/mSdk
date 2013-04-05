/*
 * Ram.h
 *
 *  Created on: Feb 2, 2011
 *      Author: Nimeni
 */

#ifndef RAM_H_
#define RAM_H_

#define     MODE_NORMAL                          0                      /* SDRAMC operation mode: Normal operation                  */
#define     MODE_PRECHARGE                       2                      /* SDRAMC operation mode: Precharge                         */
#define     MODE_LOAD_MR                         3                      /* SDRAMC operation mode: Load Mode Register                */
#define     MODE_AUTOREFRESH                     4                      /* SDRAMC operation mode: Auto refresh                      */

#define     BSP_EBI_CS1_ADDRESS                  0x10000000

#define     AVR32_SMC_SETUP_reg(x)       (*(volatile unsigned long *)(AVR32_SMC_ADDRESS + (x) * 0x10 + 0x00))
#define     AVR32_SMC_PULSE_reg(x)       (*(volatile unsigned long *)(AVR32_SMC_ADDRESS + (x) * 0x10 + 0x04))
#define     AVR32_SMC_CYCLE_reg(x)       (*(volatile unsigned long *)(AVR32_SMC_ADDRESS + (x) * 0x10 + 0x08))
#define     AVR32_SMC_MODE_reg(x)        (*(volatile unsigned long *)(AVR32_SMC_ADDRESS + (x) * 0x10 + 0x0C))

#define     SMC_NWE_SETUP_OFFSET           0                            /* Bitfields in SMC SETUP reg.                              */
#define     SMC_NWE_SETUP_MASK          0x3F
#define     SMC_NCS_WR_SETUP_OFFSET        8
#define     SMC_NCS_WR_SETUP_MASK       0x3F
#define     SMC_NRD_SETUP_OFFSET          16
#define     SMC_NRD_SETUP_MASK          0x3F
#define     SMC_NCS_RD_SETUP_OFFSET       24
#define     SMC_NCS_RD_SETUP_MASK       0x3F

#define     SMC_NWE_PULSE_OFFSET           0                            /* Bitfields in SMC PULSE reg.                              */
#define     SMC_NWE_PULSE_MASK          0x7F
#define     SMC_NCS_WR_PULSE_OFFSET        8
#define     SMC_NCS_WR_PULSE_MASK       0x7F
#define     SMC_NRD_PULSE_OFFSET          16
#define     SMC_NRD_PULSE_MASK          0x7F
#define     SMC_NCS_RD_PULSE_OFFSET       24
#define     SMC_NCS_RD_PULSE_MASK       0x7F

#define     SMC_NWE_CYCLE_OFFSET           0                            /* Bitfields in SMC CYCLE reg.                              */
#define     SMC_NWE_CYCLE_MASK         0x1FF
#define     SMC_NRD_CYCLE_OFFSET          16
#define     SMC_NRD_CYCLE_MASK         0x1FF

#define     SMC_READ_MODE_OFFSET           0                            /* Bitfields in SMC MODE reg.                               */
#define     SMC_READ_MODE_MASK          0x01
#define     SMC_WRITE_MODE_OFFSET          1
#define     SMC_WRITE_MODE_MASK         0x01
#define     SMC_EXNW_MODE_OFFSET           4
#define     SMC_EXNW_MODE_MASK          0x03
#define     SMC_BAT_OFFSET                 8
#define     SMC_BAT_MASK                0x01
#define     SMC_DBW_OFFSET                12
#define     SMC_DBW_MASK                0x03
#define     SMC_TDF_CYCLES_OFFSET         16
#define     SMC_TDF_CYCLES_MASK         0x0F
#define     SMC_TDF_MODE_OFFSET           20
#define     SMC_TDF_MODE_MASK           0x01
#define     SMC_PMEN_OFFSET               24
#define     SMC_PMEN_MASK               0x01
#define     SMC_PS_OFFSET                 28
#define     SMC_PS_MASK                 0x03

#define     AVR32_PM_HSBMASK_reg         (*(volatile unsigned long *)(AVR32_PM_ADDRESS   + 0x00C))
#define     AVR32_PM_PBBMASK_reg         (*(volatile unsigned long *)(AVR32_PM_ADDRESS   + 0x014))

typedef  struct  bsp_smc_hdr {                                          /* All timing parameters are in nanoseconds.                */
    unsigned short		NCSReadSetup;                                       /* Delay from address valid to assertion of given strobe.   */
    unsigned short		NRDSetup;
    unsigned short		NCSWriteSetup;
    unsigned short		NWESetup;
    unsigned short		NCSReadPulse;                                       /* Pulse length of given strobe.                            */
    unsigned short		NRDPulse;
    unsigned short		NCSWritePulse;
    unsigned short		NWEPulse;
    unsigned short		ReadCycle;                                          /* Total cycle length of given operation.                   */
    unsigned short		WriteCycle;
    unsigned char		BusWidth;                                           /* Bus width in bytes.                                      */
    unsigned char		NRDControlled;                                      /* Data is sampled on rising edge of {0}:NCS {1}:NRD.       */
    unsigned char		NWEControlled;                                      /* Data is driven on falling edge of {0}:NCS {1}:NWR.       */
    unsigned char		ByteWrite;                                          /* Byte {0}:select {1}:write access type.                   */
    unsigned char		TDFOptimization;                                    /* Wait states {0}:inserted before the next access ...      */
                                                                        /* ... {1}:optimized using setup period of next read/write. */
    unsigned char		TDFCycles;                                          /* Cycles+1 to release data after rising edge {0-15}.       */
} BSP_SMC_HDR;


void  SMC_PreConfig (void *paddr, BSP_SMC_HDR *settings, unsigned long hsb_clock);
void  SMC_Config (void *paddr, BSP_SMC_HDR *settings);
void  SDRAMC_RefreshRateUpdate (void);
void  SDRAMC_Init (void);

#include "ram.c"

#endif /* RAM_H_ */
