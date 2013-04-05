/*
 *  \file   interrupt.h
 *
 *  \brief  Interrupt related API declarations.
 *
 *   This file contains the API prototypes for configuring INTC for 
 *   ARM Cortex-A8
*/

/*
* Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
*/
/*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*    Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/


#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "include/hw/hw_intc.h"
#include "include/hw/hw_types.h"

#ifdef __cplusplus
extern "C" {
#endif
/****************************************************************************
**                       MACRO DEFINITIONS
****************************************************************************/
/*
** Macros which can be passed to IntPrioritySet API as hostIntRoute.
*/
/* To route an interrupt to IRQ */
#define AINTC_HOSTINT_ROUTE_IRQ                (0)

/* To route an interrupt to FIQ */
#define AINTC_HOSTINT_ROUTE_FIQ                (INTC_ILR_FIQNIRQ)

/*
** Interrupt number list
*/
#define SYS_INT_L2_CACHE_IRQ                         (0)
#define SYS_INT_CTI_IRQ_0                         (1)
#define SYS_INT_CTI_IRQ_1                         (2)
#define SYS_INT_ELM_IRQ                         (4)
#define SYS_INT_CMU_IRQ_P                         (5)
#define SYS_INT_SYS_NIRQ1                         (7)
#define SYS_INT_L3_DBG_IRQ                         (9)
#define SYS_INT_L3_APP_IRQ                         (10)
#define SYS_INT_PRCM_MPU_IRQ                         (11)
#define SYS_INT_SDMA_IRQ_0                         (12)
#define SYS_INT_SDMA_IRQ_1                         (13)
#define SYS_INT_SDMA_IRQ_2                         (14)
#define SYS_INT_SDMA_IRQ_3                         (15)
#define SYS_INT_MCBSP4_IRQ                         (16)
#define SYS_INT_MCBSP1_IRQ                         (17)
#define SYS_INT_SR_MPU_IRQ                         (18)
#define SYS_INT_SR_CORE_IRQ                         (19)
#define SYS_INT_GPMC_IRQ                         (20)
#define SYS_INT_SGX_IRQ                         (21)
#define SYS_INT_MCBSP2_IRQ                         (22)
#define SYS_INT_MCBSP3_IRQ                         (23)
#define SYS_INT_ISS_IRQ5                         (24)
#define SYS_INT_DSS_DISPC_IRQ                         (25)
#define SYS_INT_MAIL_U0_MPU_IRQ                         (26)
#define SYS_INT_C2C_SSCM_IRQ0                         (27)
#define SYS_INT_DSP_MMU_IRQ                         (28)
#define SYS_INT_GPIO1_MPU_IRQ                         (29)
#define SYS_INT_GPIO2_MPU_IRQ                         (30)
#define SYS_INT_GPIO3_MPU_IRQ                         (31)
#define SYS_INT_GPIO4_MPU_IRQ                         (32)
#define SYS_INT_GPIO5_MPU_IRQ                         (33)
#define SYS_INT_GPIO6_MPU_IRQ                         (34)
#define SYS_INT_WDT3_IRQ                         (36)
#define SYS_INT_GPT1_IRQ                         (37)
#define SYS_INT_GPT2_IRQ                         (38)
#define SYS_INT_GPT3_IRQ                         (39)
#define SYS_INT_GPT4_IRQ                         (40)
#define SYS_INT_GPT5_IRQ                         (41)
#define SYS_INT_GPT6_IRQ                         (42)
#define SYS_INT_GPT7_IRQ                         (43)
#define SYS_INT_GPT8_IRQ                         (44)
#define SYS_INT_GPT9_IRQ                         (45)
#define SYS_INT_GPT10_IRQ                         (46)
#define SYS_INT_GPT11_IRQ                         (47)
#define SYS_INT_MCSPI4_IRQ                         (48)
#define SYS_INT_DSS_DSI1_IRQ                         (53)
#define SYS_INT_CORTEXA9_CPU0_PMU_IRQ                         (54)
#define SYS_INT_CORTEXA9_CPU1_PMU_IRQ                         (55)
#define SYS_INT_I2C1_IRQ                         (56)
#define SYS_INT_I2C2_IRQ                         (57)
#define SYS_INT_HDQ_IRQ                         (58)
#define SYS_INT_MMC5_IRQ                         (59)
#define SYS_INT_I2C3_IRQ                         (61)
#define SYS_INT_I2C4_IRQ                         (62)
#define SYS_INT_MCSPI1_IRQ                         (65)
#define SYS_INT_MCSPI2_IRQ                         (66)
#define SYS_INT_HSI_P1_MPU_IRQ                         (67)
#define SYS_INT_HSI_P2_MPU_IRQ                         (68)
#define SYS_INT_FDIF_IRQ_3                         (69)
#define SYS_INT_UART4_IRQ                         (70)
#define SYS_INT_HSI_DMA_MPU_IRQ                         (71)
#define SYS_INT_UART1_IRQ                         (72)
#define SYS_INT_UART2_IRQ                         (73)
#define SYS_INT_UART3_IRQ                         (74)
#define SYS_INT_PBIAS_IRQ                         (75)
#define SYS_INT_HSUSB_OHCI_IRQ                         (76)
#define SYS_INT_HSUSB_EHCI_IRQ                         (77)
#define SYS_INT_HSUSB_TLL_IRQ                         (78)
#define SYS_INT_WDT2_IRQ                         (80)
#define SYS_INT_MMC1_IRQ                         (83)
#define SYS_INT_DSS_DSI2_IRQ                         (84)
#define SYS_INT_MMC2_IRQ                         (86)
#define SYS_INT_MPU_ICR_IRQ                         (87)
#define SYS_INT_C2C_SSCM_IRQ1                         (88)
#define SYS_INT_FSUSB_IRQ                         (89)
#define SYS_INT_FSUSB_SMI_IRQ                         (90)
#define SYS_INT_MCSPI3_IRQ                         (91)
#define SYS_INT_HSUSB_OTG_IRQ                         (925)
#define SYS_INT_HSUSB_OTG_DMA_IRQ                         (93)
#define SYS_INT_MMC3_IRQ                         (94)
#define SYS_INT_MMC4_IRQ                         (96)
#define SYS_INT_SLIMBUS1_IRQ                         (97)
#define SYS_INT_SLIMBUS2_IRQ                         (98)
#define SYS_INT_ABE_MPU_IRQ                         (99)
#define SYS_INT_CORTEXM3_MMU_IRQ                         (100)
#define SYS_INT_DSS_HDMI_IRQ                         (101)
#define SYS_INT_SR_IVA_IRQ                         (102)
#define SYS_INT_IVAHD_IRQ2                         (103)
#define SYS_INT_IVAHD_IRQ1                         (104)
#define SYS_INT_IVAHD_MAILBOX_IRQ_0                         (107)
#define SYS_INT_MCASP1_AXINT                         (109)
#define SYS_INT_EMIF1_IRQ                         (110)
#define SYS_INT_EMIF2_IRQ                         (111)
#define SYS_INT_MCPDM_IRQ                         (112)
#define SYS_INT_DMM_IRQ                         (113)
#define SYS_INT_DMIC_IRQ                         (114)
#define SYS_INT_SYS_NIRQ2                         (119)
#define SYS_INT_KBD_CTL_IRQ                         (120)
#define SYS_INT_THERMAL_ALERT                         (126)




/* OMAP44XX IRQs numbers definitions */
#define OMAP44XX_IRQ_LOCALTIMER			29
#define OMAP44XX_IRQ_LOCALWDT			30

#define OMAP44XX_IRQ_GIC_START			32

#define OMAP44XX_IRQ_PL310			(0 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_CTI0			(1 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_CTI1			(2 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_ELM			(4 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_SYS_1N			(7 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_SECURITY_EVENTS		(8 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_L3_DBG			(9 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_L3_APP			(10 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_PRCM			(11 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_SDMA_0			(12 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_SDMA_1			(13 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_SDMA_2			(14 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_SDMA_3			(15 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_MCBSP4			(16 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_MCBSP1			(17 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_SR_MCU			(18 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_SR_CORE			(19 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_GPMC			(20 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_GFX			(21 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_MCBSP2			(22 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_MCBSP3			(23 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_ISS_5			(24 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_DSS_DISPC			(25 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_MAIL_U0			(26 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_C2C_SSCM_0			(27 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_TESLA_MMU			(28 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_GPIO1			(29 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_GPIO2			(30 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_GPIO3			(31 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_GPIO4			(32 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_GPIO5			(33 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_GPIO6			(34 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_USIM			(35 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_WDT3			(36 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_GPT1			(37 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_GPT2			(38 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_GPT3			(39 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_GPT4			(40 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_GPT5			(41 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_GPT6			(42 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_GPT7			(43 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_GPT8			(44 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_GPT9			(45 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_GPT10			(46 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_GPT11			(47 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_SPI4			(48 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_SHA1_S			(49 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_FPKA_SINTREQUEST_S		(50 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_SHA1_P			(51 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_RNG			(52 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_DSS_DSI1			(53 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_I2C1			(56 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_I2C2			(57 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_HDQ			(58 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_MMC5			(59 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_I2C3			(61 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_I2C4			(62 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_AES2_S			(63 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_AES2_P			(64 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_SPI1			(65 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_SPI2			(66 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_HSI_P1			(67 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_HSI_P2			(68 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_FDIF_3			(69 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_UART4			(70 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_HSI_DMA			(71 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_UART1			(72 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_UART2			(73 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_UART3			(74 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_PBIAS			(75 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_OHCI			(76 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_EHCI			(77 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_TLL			(78 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_AES1_S			(79 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_WDT2			(80 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_DES_S			(81 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_DES_P			(82 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_MMC1			(83 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_DSS_DSI2			(84 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_AES1_P			(85 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_MMC2			(86 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_MPU_ICR			(87 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_C2C_SSCM_1			(88 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_FSUSB			(89 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_FSUSB_SMI			(90 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_SPI3			(91 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_HS_USB_MC_N		(92 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_HS_USB_DMA_N		(93 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_MMC3			(94 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_GPT12			(95 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_MMC4			(96 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_SLIMBUS1			(97 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_SLIMBUS2			(98 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_ABE			(99 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_DUCATI_MMU			(100 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_DSS_HDMI			(101 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_SR_IVA			(102 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_IVA_HD_POSYNCITRPEND_1	(103 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_IVA_HD_POSYNCITRPEND_0	(104 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_IVA_HD_POMBINTRPEND_0	(107 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_MCASP1_AR			(108 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_MCASP1_AX			(109 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_EMIF4_1			(110 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_EMIF4_2			(111 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_MCPDM			(112 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_DMM			(113 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_DMIC			(114 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_CDMA_0			(115 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_CDMA_1			(116 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_CDMA_2			(117 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_CDMA_3			(118 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_SYS_2N			(119 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_KBD_CTL			(120 + OMAP44XX_IRQ_GIC_START)
#define OMAP44XX_IRQ_UNIPRO1			(124 + OMAP44XX_IRQ_GIC_START)


/*****************************************************************************
**                     API FUNCTION PROTOTYPES
*****************************************************************************/
void IntAINTCInit (void);
void IntIfClkFreeRunSet(void);
void IntIfClkAutoGateSet(void);
void IntProtectionEnable(void);
void IntProtectionDisable(void);
void IntSyncClkFreeRunSet(void);
void IntSyncClkAutoGateSet(void);
void IntFuncClkFreeRunSet(void);
void IntFuncClkAutoGateSet(void);
void IntPriorityThresholdSet(unsigned int threshold);
void IntSoftwareIntSet(unsigned int intrNum);
void IntSoftwareIntClear(unsigned int intrNum);
void IntMasterIRQEnable(void);
void IntMasterIRQDisable(void);
void IntMasterFIQEnable(void);
void IntMasterFIQDisable(void);
void IntSystemEnable(unsigned int intrNum);
void IntSystemDisable(unsigned int intrNum);
void IntUnRegister(unsigned int intrNum);
void IntEnable(unsigned char  status);
void IntRegister(unsigned int intrNum, void (*pfnHandler)(void));
void IntPrioritySet(unsigned int intrNum, unsigned int priority,
                           unsigned int hostIntRoute);
unsigned char IntDisable(void);
unsigned int IntMasterStatusGet(void);
unsigned int IntActiveIrqNumGet(void);
unsigned int IntActiveFiqNumGet(void);
unsigned int IntSpurIrqFlagGet(void);
unsigned int IntSpurFiqFlagGet(void);
unsigned int IntCurrIrqPriorityGet(void);
unsigned int IntCurrFiqPriorityGet(void);
unsigned int IntPriorityThresholdGet(void);
unsigned int IntRawStatusGet(unsigned int intrNum);
unsigned int IntPendingIrqMaskedStatusGet(unsigned int intrNum);
unsigned int IntPendingFiqMaskedStatusGet(unsigned int intrNum);

#ifdef __cplusplus
}
#endif

#ifdef HEADER_INCLUDE_C_FILES
#include "interrupt.c"
#endif
#endif

