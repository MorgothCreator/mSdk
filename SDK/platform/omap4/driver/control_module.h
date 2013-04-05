/*
 * pin_mux_gpio_def.h
 *
 *  Created on: Jan 12, 2013
 *      Author: XxXx
 */

#ifndef PIN_MUX_GPIO_DEF_H_
#define PIN_MUX_GPIO_DEF_H_

#define pad_indicator_bp (3)
#define pad_indicator_bm (1 << pad_indicator_bp)

#define pad_padconf_type_gp (4)
#define pad_padconf_type_gm (3)
#define pad_padconf_core_bm (0 << pad_padconf_type_gp)
#define pad_padconf_wkup_bm (1 << pad_padconf_type_gp)

#define reg_offset_gp (8)

#define pin_function_gp (0)
#define pin_function_gm (0x07<<pin_function_gp)


#define cm_wakeupevent_bp (15)
#define cm_wakeupevent_bm (0x01 << cm_wakeupevent_bp)

#define cm_wakeupenable_bp (14)
#define cm_wakeupenable_bm (0x01 << cm_wakeupevent_bp)

#define cm_offmodepulltypeselect_bp (13)
#define cm_offmodepulltypeselect_bm (0x01 << cm_offmodepulltypeselect_bp)

#define cm_offmodepulludenable_bp (12)
#define cm_offmodepulludenable_bm (0x01 << cm_offmodepulludenable_bp)

#define cm_offmodeoutvalue_bp (11)
#define cm_offmodeoutvalue_bm (0x01 << cm_offmodeoutvalue_bp)

#define cm_offmodeoutenable_bp (10)
#define cm_offmodeoutenable_bm (0x01 << cm_offmodeoutenable_bp)

#define cm_offmodeenable_bp (9)
#define cm_offmodeenable_bm (0x01 << cm_offmodeenable_bp)

#define cm_inputenable_bp (8)
#define cm_inputenable_bm (0x01 << cm_inputenable_bp)

#define cm_pulltypeselect_bp (4)
#define cm_pulltypeselect_bm (0x01 << cm_pulltypeselect_bp)

#define cm_pulludenable_bp (3)
#define cm_pulludenable_bm (0x01 << cm_pulludenable_bp)

#define cm_muxmode_gp (0)
#define cm_muxmode_gm (0x07 << cm_muxmode_gp)

#define cm_pad0_gp (0)
#define cm_pad0_gm (0xFFFF << cm_pad0_gp)

#define cm_pad1_gp (16)
#define cm_pad1_gm (0xFFFF << cm_pad1_gp)
//*****************************************************************************
#define SYSCTRL_PADCONF_CORE_BASE_ADDR   0x4A100000

#define CONTROL_PADCONF_WKUP_REVISION   0x00000000
#define CONTROL_PADCONF_WKUP_HWINFO      0x00000004
#define CONTROL_PADCONF_WKUP_SYSCONFIG   0x00000010

#define CONTROL_CORE_PAD0_GPMC_AD0_PAD1_GPMC_AD1   0x00000040
#define CONTROL_CORE_PAD0_GPMC_AD2_PAD1_GPMC_AD3   0x00000044
#define CONTROL_CORE_PAD0_GPMC_AD4_PAD1_GPMC_AD5   0x00000048
#define CONTROL_CORE_PAD0_GPMC_AD6_PAD1_GPMC_AD7   0x0000004C
#define CONTROL_CORE_PAD0_GPMC_AD8_PAD1_GPMC_AD9   0x00000050
#define CONTROL_CORE_PAD0_GPMC_AD10_PAD1_GPMC_AD11   0x00000054
#define CONTROL_CORE_PAD0_GPMC_AD12_PAD1_GPMC_AD13   0x00000058
#define CONTROL_CORE_PAD0_GPMC_AD14_PAD1_GPMC_AD15   0x0000005C
#define CONTROL_CORE_PAD0_GPMC_A16_PAD1_GPMC_A17   0x00000060
#define CONTROL_CORE_PAD0_GPMC_A18_PAD1_GPMC_A19   0x00000064
#define CONTROL_CORE_PAD0_GPMC_A20_PAD1_GPMC_A21   0x00000068
#define CONTROL_CORE_PAD0_GPMC_A22_PAD1_GPMC_A23   0x0000006C
#define CONTROL_CORE_PAD0_GPMC_A24_PAD1_GPMC_A25   0x00000070
#define CONTROL_CORE_PAD0_GPMC_NCS0_PAD1_GPMC_NCS1   0x00000074
#define CONTROL_CORE_PAD0_GPMC_NCS2_PAD1_GPMC_NCS3   0x00000078
#define CONTROL_CORE_PAD0_GPMC_NWP_PAD1_GPMC_CLK   0x0000007C
#define CONTROL_CORE_PAD0_GPMC_NADV_ALE_PAD1_GPMC_NOE      0x00000080
#define CONTROL_CORE_PAD0_GPMC_NWE_PAD1_GPMC_NBE0_CLE      0x00000084
#define CONTROL_CORE_PAD0_GPMC_NBE1_PAD1_GPMC_WAIT0   0x00000088
#define CONTROL_CORE_PAD0_GPMC_WAIT1_PAD1_GPMC_WAIT2      0x0000008C
#define CONTROL_CORE_PAD0_GPMC_NCS4_PAD1_GPMC_NCS5   0x00000090
#define CONTROL_CORE_PAD0_GPMC_NCS6_PAD1_GPMC_NCS7   0x00000094
#define CONTROL_CORE_PAD0_GPIO63_PAD1_GPIO64      0x00000098
#define CONTROL_CORE_PAD0_GPIO65_PAD1_GPIO66      0x0000009C
#define CONTROL_CORE_PAD0_CSI21_DX0_PAD1_CSI21_DY0   0x000000A0
#define CONTROL_CORE_PAD0_CSI21_DX1_PAD1_CSI21_DY1   0x000000A4
#define CONTROL_CORE_PAD0_CSI21_DX2_PAD1_CSI21_DY2   0x000000A8
#define CONTROL_CORE_PAD0_CSI21_DX3_PAD1_CSI21_DY3   0x000000AC
#define CONTROL_CORE_PAD0_CSI21_DX4_PAD1_CSI21_DY4   0x000000B0
#define CONTROL_CORE_PAD0_CSI22_DX0_PAD1_CSI22_DY0   0x000000B4
#define CONTROL_CORE_PAD0_CSI22_DX1_PAD1_CSI22_DY1   0x000000B8
#define CONTROL_CORE_PAD0_CAM_SHUTTER_PAD1_CAM_STROBE      0x000000BC
#define CONTROL_CORE_PAD0_CAM_GLOBALRESET_PAD1_USBB1_ULPITLL_CLK   0x000000C0
#define CONTROL_CORE_PAD0_USBB1_ULPITLL_STP_PAD1_USBB1_ULPITLL_DIR   0x000000C4
#define CONTROL_CORE_PAD0_USBB1_ULPITLL_NXT_PAD1_USBB1_ULPITLL_DAT0   0x000000C8
#define CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT1_PAD1_USBB1_ULPITLL_DAT2   0x000000CC
#define CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT3_PAD1_USBB1_ULPITLL_DAT4   0x000000D0
#define CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT5_PAD1_USBB1_ULPITLL_DAT6   0x000000D4
#define CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT7_PAD1_USBB1_HSIC_DATA   0x000000D8
#define CONTROL_CORE_PAD0_USBB1_HSIC_STROBE_PAD1_USBC1_ICUSB_DP   0x000000DC
#define CONTROL_CORE_PAD0_USBC1_ICUSB_DM_PAD1_SDMMC1_CLK      0x000000E0
#define CONTROL_CORE_PAD0_SDMMC1_CMD_PAD1_SDMMC1_DAT0      0x000000E4
#define CONTROL_CORE_PAD0_SDMMC1_DAT1_PAD1_SDMMC1_DAT2      0x000000E8
#define CONTROL_CORE_PAD0_SDMMC1_DAT3_PAD1_SDMMC1_DAT4      0x000000EC
#define CONTROL_CORE_PAD0_SDMMC1_DAT5_PAD1_SDMMC1_DAT6      0x000000F0
#define CONTROL_CORE_PAD0_SDMMC1_DAT7_PAD1_ABE_MCBSP2_CLKX      0x000000F4
#define CONTROL_CORE_PAD0_ABE_MCBSP2_DR_PAD1_ABE_MCBSP2_DX      0x000000F8
#define CONTROL_CORE_PAD0_ABE_MCBSP2_FSX_PAD1_ABE_MCBSP1_CLKX   0x000000FC
#define CONTROL_CORE_PAD0_ABE_MCBSP1_DR_PAD1_ABE_MCBSP1_DX      0x00000100
#define CONTROL_CORE_PAD0_ABE_MCBSP1_FSX_PAD1_ABE_PDM_UL_DATA   0x00000104
#define CONTROL_CORE_PAD0_ABE_PDM_DL_DATA_PAD1_ABE_PDM_FRAME   0x00000108
#define CONTROL_CORE_PAD0_ABE_PDM_LB_CLK_PAD1_ABE_CLKS      0x0000010C
#define CONTROL_CORE_PAD0_ABE_DMIC_CLK1_PAD1_ABE_DMIC_DIN1      0x00000110
#define CONTROL_CORE_PAD0_ABE_DMIC_DIN2_PAD1_ABE_DMIC_DIN3      0x00000114
#define CONTROL_CORE_PAD0_UART2_CTS_PAD1_UART2_RTS   0x00000118
#define CONTROL_CORE_PAD0_UART2_RX_PAD1_UART2_TX   0x0000011C
#define CONTROL_CORE_PAD0_HDQ_SIO_PAD1_I2C1_SCL      0x00000120
#define CONTROL_CORE_PAD0_I2C1_SDA_PAD1_I2C2_SCL   0x00000124
#define CONTROL_CORE_PAD0_I2C2_SDA_PAD1_I2C3_SCL   0x00000128
#define CONTROL_CORE_PAD0_I2C3_SDA_PAD1_I2C4_SCL   0x0000012C
#define CONTROL_CORE_PAD0_I2C4_SDA_PAD1_MCSPI1_CLK   0x00000130
#define CONTROL_CORE_PAD0_MCSPI1_SOMI_PAD1_MCSPI1_SIMO      0x00000134
#define CONTROL_CORE_PAD0_MCSPI1_CS0_PAD1_MCSPI1_CS1      0x00000138
#define CONTROL_CORE_PAD0_MCSPI1_CS2_PAD1_MCSPI1_CS3      0x0000013C
#define CONTROL_CORE_PAD0_UART3_CTS_RCTX_PAD1_UART3_RTS_SD      0x00000140
#define CONTROL_CORE_PAD0_UART3_RX_IRRX_PAD1_UART3_TX_IRTX      0x00000144
#define CONTROL_CORE_PAD0_SDMMC5_CLK_PAD1_SDMMC5_CMD      0x00000148
#define CONTROL_CORE_PAD0_SDMMC5_DAT0_PAD1_SDMMC5_DAT1      0x0000014C
#define CONTROL_CORE_PAD0_SDMMC5_DAT2_PAD1_SDMMC5_DAT3      0x00000150
#define CONTROL_CORE_PAD0_MCSPI4_CLK_PAD1_MCSPI4_SIMO      0x00000154
#define CONTROL_CORE_PAD0_MCSPI4_SOMI_PAD1_MCSPI4_CS0      0x00000158
#define CONTROL_CORE_PAD0_UART4_RX_PAD1_UART4_TX   0x0000015C
#define CONTROL_CORE_PAD0_USBB2_ULPITLL_CLK_PAD1_USBB2_ULPITLL_STP   0x00000160
#define CONTROL_CORE_PAD0_USBB2_ULPITLL_DIR_PAD1_USBB2_ULPITLL_NXT   0x00000164
#define CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT0_PAD1_USBB2_ULPITLL_DAT1   0x00000168
#define CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT2_PAD1_USBB2_ULPITLL_DAT3   0x0000016C
#define CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT4_PAD1_USBB2_ULPITLL_DAT5   0x00000170
#define CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT6_PAD1_USBB2_ULPITLL_DAT7   0x00000174
#define CONTROL_CORE_PAD0_USBB2_HSIC_DATA_PAD1_USBB2_HSIC_STROBE   0x00000178
#define CONTROL_CORE_PAD0_KPD_COL3_PAD1_KPD_COL4   0x0000017C
#define CONTROL_CORE_PAD0_KPD_COL5_PAD1_KPD_COL0   0x00000180
#define CONTROL_CORE_PAD0_KPD_COL1_PAD1_KPD_COL2   0x00000184
#define CONTROL_CORE_PAD0_KPD_ROW3_PAD1_KPD_ROW4   0x00000188
#define CONTROL_CORE_PAD0_KPD_ROW5_PAD1_KPD_ROW0   0x0000018C
#define CONTROL_CORE_PAD0_KPD_ROW1_PAD1_KPD_ROW2   0x00000190
#define CONTROL_CORE_PAD0_USBA0_OTG_CE_PAD1_USBA0_OTG_DP      0x00000194
#define CONTROL_CORE_PAD0_USBA0_OTG_DM_PAD1_FREF_CLK1_OUT      0x00000198
#define CONTROL_CORE_PAD0_FREF_CLK2_OUT_PAD1_SYS_NIRQ1      0x0000019C
#define CONTROL_CORE_PAD0_SYS_NIRQ2_PAD1_SYS_BOOT0   0x000001A0
#define CONTROL_CORE_PAD0_SYS_BOOT1_PAD1_SYS_BOOT2   0x000001A4
#define CONTROL_CORE_PAD0_SYS_BOOT3_PAD1_SYS_BOOT4   0x000001A8
#define CONTROL_CORE_PAD0_SYS_BOOT5_PAD1_DPM_EMU0   0x000001AC
#define CONTROL_CORE_PAD0_DPM_EMU1_PAD1_DPM_EMU2   0x000001B0
#define CONTROL_CORE_PAD0_DPM_EMU3_PAD1_DPM_EMU4   0x000001B4
#define CONTROL_CORE_PAD0_DPM_EMU5_PAD1_DPM_EMU6   0x000001B8
#define CONTROL_CORE_PAD0_DPM_EMU7_PAD1_DPM_EMU8   0x000001BC
#define CONTROL_CORE_PAD0_DPM_EMU9_PAD1_DPM_EMU10   0x000001C0
#define CONTROL_CORE_PAD0_DPM_EMU11_PAD1_DPM_EMU12   0x000001C4
#define CONTROL_CORE_PAD0_DPM_EMU13_PAD1_DPM_EMU14   0x000001C8
#define CONTROL_CORE_PAD0_DPM_EMU15_PAD1_DPM_EMU16   0x000001CC
#define CONTROL_CORE_PAD0_DPM_EMU17_PAD1_DPM_EMU18   0x000001D0
#define CONTROL_CORE_PAD0_DPM_EMU19_PAD1_CSI22_DX2   0x000001D4

#define CONTROL_PADCONF_WAKEUPEVENT_0   0x000001D8
#define CONTROL_PADCONF_WAKEUPEVENT_1   0x000001DC
#define CONTROL_PADCONF_WAKEUPEVENT_2   0x000001E0
#define CONTROL_PADCONF_WAKEUPEVENT_3   0x000001E4
#define CONTROL_PADCONF_WAKEUPEVENT_4   0x000001E8
#define CONTROL_PADCONF_WAKEUPEVENT_5   0x000001EC
#define CONTROL_PADCONF_WAKEUPEVENT_6   0x000001F0
#define CONTROL_CORE_PAD0_CSI22_DY2      0x000001F4
#define CONTROL_PADCONF_GLOBAL      0x000005A0
#define CONTROL_CORE_PADCONF_MODE      0x000005A4
#define CONTROL_SMART1IO_PADCONF_0      0x000005A8
#define CONTROL_SMART1IO_PADCONF_1      0x000005AC
#define CONTROL_SMART2IO_PADCONF_0      0x000005B0
#define CONTROL_SMART2IO_PADCONF_1      0x000005B4
#define CONTROL_SMART3IO_PADCONF_0      0x000005B8
#define CONTROL_SMART3IO_PADCONF_1      0x000005BC
#define CONTROL_SMART3IO_PADCONF_2      0x000005C0
#define CONTROL_USBB_HSIC      0x000005C4
#define CONTROL_SMART3IO_PADCONF_3      0x000005C8
#define CONTROL_SMART2IO_PADCONF_2      0x000005CC
#define CONTROL_SMART1IO_PADCONF_2      0x000005D0
#define CONTROL_SMART1IO_PADCONF_3      0x000005D4
#define CONTROL_C2CIO_PADCONF_0      0x000005D8
#define CONTROL_PBIASLITE      0x00000600
#define CONTROL_I2C_0         0x00000604
#define CONTROL_CAMERA_RX      0x00000608
#define CONTROL_AVDAC         0x0000060C
#define CONTROL_MMC2         0x00000614
#define CONTROL_DSIPHY         0x00000618
#define CONTROL_MCBSPLP         0x0000061C
#define CONTROL_USB2PHYCORE      0x00000620
#define CONTROL_I2C_1         0x00000624
#define CONTROL_MMC1         0x00000628
#define CONTROL_HSI         0x0000062C
#define CONTROL_USB         0x00000630
#define CONTROL_HDQ         0x00000634
#define CONTROL_LPDDR2IO1_0      0x00000638
#define CONTROL_LPDDR2IO1_1      0x0000063C
#define CONTROL_LPDDR2IO1_2      0x00000640
#define CONTROL_LPDDR2IO1_3      0x00000644
#define CONTROL_LPDDR2IO2_0      0x00000648
#define CONTROL_LPDDR2IO2_1      0x0000064C
#define CONTROL_LPDDR2IO2_2      0x00000650
#define CONTROL_LPDDR2IO2_3      0x00000654
#define CONTROL_BUS_HOLD      0x00000658
#define CONTROL_C2C         0x0000065C
#define CONTROL_CORE_CONTROL_SPARE_RW   0x00000660
#define CONTROL_CORE_CONTROL_SPARE_R   0x00000664
#define CONTROL_CORE_CONTROL_SPARE_R_C0   0x00000668
#define CONTROL_CORE_CONTROL_SPARE_RW1   0x0000066C
#define CONTROL_CORE_CONTROL_SPARE_RW2   0x00000670
#define CONTROL_CORE_CONTROL_SPARE_RW3   0x00000674
#define CONTROL_CORE_CONTROL_SPARE_RW4   0x00000678
#define CONTROL_CORE_CONTROL_SPARE_RW5   0x0000067C
#define CONTROL_CORE_CONTROL_SPARE_RW6   0x00000680
#define CONTROL_CORE_CONTROL_SPARE_RW7   0x00000684
#define CONTROL_CORE_CONTROL_SPARE_RW8   0x00000688
#define CONTROL_CORE_CONTROL_SPARE_RW9   0x0000068C
#define CONTROL_CORE_CONTROL_SPARE_R1   0x00000690
#define CONTROL_EFUSE_1         0x00000700
#define CONTROL_EFUSE_2         0x00000704
#define CONTROL_EFUSE_3         0x00000708
#define CONTROL_EFUSE_4         0x0000070C
//*****************************************************************************
#define SYSCTRL_PADCONF_WKUP_BASE_ADDR   0x4A31E000

#define CONTROL_PADCONF_WKUP_REVISION   0x00000000
#define CONTROL_PADCONF_WKUP_HWINFO      0x00000004
#define CONTROL_PADCONF_WKUP_SYSCONFIG   0x00000010
#define CONTROL_WKUP_PAD0_GPIO_WK0_PAD1_GPIO_WK1   0x00000040
#define CONTROL_WKUP_PAD0_GPIO_WK2_PAD1_GPIO_WK3   0x00000044
#define CONTROL_WKUP_PAD0_GPIO_WK4_PAD1_SR_SCL      0x00000048
#define CONTROL_WKUP_PAD0_SR_SDA_PAD1_FREF_XTAL_IN   0x0000004C
#define CONTROL_WKUP_PAD0_FREF_SLICER_IN_PAD1_FREF_CLK_IOREQ   0x00000050
#define CONTROL_WKUP_PAD0_FREF_CLK0_OUT_PAD1_FREF_CLK3_REQ      0x00000054
#define CONTROL_WKUP_PAD0_FREF_CLK3_OUT_PAD1_FREF_CLK4_REQ      0x00000058
#define CONTROL_WKUP_PAD0_FREF_CLK4_OUT_PAD1_SYS_32K      0x0000005C
#define CONTROL_WKUP_PAD0_SYS_NRESPWRON_PAD1_SYS_NRESWARM      0x00000060
#define CONTROL_WKUP_PAD0_SYS_PWR_REQ_PAD1_SYS_PWRON_RESET_OUT   0x00000064
#define CONTROL_WKUP_PAD0_SYS_BOOT6_PAD1_SYS_BOOT7   0x00000068
#define CONTROL_WKUP_PAD0_JTAG_NTRST_PAD1_JTAG_TCK   0x0000006C
#define CONTROL_WKUP_PAD0_JTAG_RTCK_PAD1_JTAG_TMS_TMSC      0x00000070
#define CONTROL_WKUP_PAD0_JTAG_TDI_PAD1_JTAG_TDO   0x00000074
#define CONTROL_WKUP_PADCONF_WAKEUPEVENT_0      0x0000007C
#define CONTROL_SMART1NOPMIO_PADCONF_0   0x000005A0
#define CONTROL_SMART1NOPMIO_PADCONF_1   0x000005A4
#define CONTROL_WKUP_PADCONF_MODE      0x000005A8
#define CONTROL_XTAL_OSCILLATOR      0x000005AC
#define CONTROL_SMART3NOPMIO_PADCONF_0   0x000005B0
#define CONTROL_SMART3NOPMIO_PADCONF_1   0x000005B4
#define CONTROL_GPIOWK         0x00000600
#define CONTROL_I2C_2         0x00000604
#define CONTROL_JTAG         0x00000608
#define CONTROL_SYS         0x0000060C
#define CONTROL_WKUP_CONTROL_SPARE_RW   0x00000614
#define CONTROL_WKUP_CONTROL_SPARE_R   0x00000618
#define CONTROL_WKUP_CONTROL_SPARE_R_C0   0x0000061C
#define CONTROL_WKUP_CONTROL_SPARE_RW1   0x00000620
#define CONTROL_WKUP_CONTROL_SPARE_RW2   0x00000624
#define CONTROL_WKUP_CONTROL_SPARE_RW3   0x00000628
#define CONTROL_WKUP_CONTROL_SPARE_RW4   0x0000062C
#define CONTROL_WKUP_CONTROL_SPARE_RW5   0x00000630
#define CONTROL_WKUP_CONTROL_SPARE_RW6   0x00000634
#define CONTROL_WKUP_CONTROL_SPARE_RW7   0x00000638
#define CONTROL_WKUP_CONTROL_SPARE_RW8   0x0000063C
#define CONTROL_WKUP_CONTROL_SPARE_RW9   0x00000640
#define CONTROL_WKUP_CONTROL_SPARE_R1   0x0000064C
//*****************************************************************************
#define c12_gpmc_ad0   (0 | (CONTROL_CORE_PAD0_GPMC_AD0_PAD1_GPMC_AD1 << reg_offset_gp) | (0<<pad_indicator_bp))
#define c12_sdmmc2_dat0   (1 | (CONTROL_CORE_PAD0_GPMC_AD0_PAD1_GPMC_AD1 << reg_offset_gp) | (0<<pad_indicator_bp))

#define d12_gpmc_ad1   (0 | (CONTROL_CORE_PAD0_GPMC_AD0_PAD1_GPMC_AD1 << reg_offset_gp) | (1<<pad_indicator_bp))
#define d12_sdmmc2_dat1   (1 | (CONTROL_CORE_PAD0_GPMC_AD0_PAD1_GPMC_AD1 << reg_offset_gp) | (1<<pad_indicator_bp))

#define c13_gpmc_ad2   (0 | (CONTROL_CORE_PAD0_GPMC_AD2_PAD1_GPMC_AD3 << reg_offset_gp) | (0<<pad_indicator_bp))
#define c13_sdmmc2_dat2   (1 | (CONTROL_CORE_PAD0_GPMC_AD2_PAD1_GPMC_AD3 << reg_offset_gp) | (0<<pad_indicator_bp))

#define d13_gpmc_ad3   (0 | (CONTROL_CORE_PAD0_GPMC_AD2_PAD1_GPMC_AD3 << reg_offset_gp) | (1<<pad_indicator_bp))
#define d13_sdmmc2_dat3   (1 | (CONTROL_CORE_PAD0_GPMC_AD2_PAD1_GPMC_AD3 << reg_offset_gp) | (1<<pad_indicator_bp))

#define c15_gpmc_ad4   (0 | (CONTROL_CORE_PAD0_GPMC_AD4_PAD1_GPMC_AD5 << reg_offset_gp) | (0<<pad_indicator_bp))
#define c15_sdmmc2_dat4   (1 | (CONTROL_CORE_PAD0_GPMC_AD4_PAD1_GPMC_AD5 << reg_offset_gp) | (0<<pad_indicator_bp))
#define c15_sdmmc2_dir_dat0   (2 | (CONTROL_CORE_PAD0_GPMC_AD4_PAD1_GPMC_AD5 << reg_offset_gp) | (0<<pad_indicator_bp))

#define d15_gpmc_ad5   (0 | (CONTROL_CORE_PAD0_GPMC_AD4_PAD1_GPMC_AD5 << reg_offset_gp) | (1<<pad_indicator_bp))
#define d15_sdmmc2_dat5   (1 | (CONTROL_CORE_PAD0_GPMC_AD4_PAD1_GPMC_AD5 << reg_offset_gp) | (1<<pad_indicator_bp))
#define d15_sdmmc2_dir_dat1   (2 | (CONTROL_CORE_PAD0_GPMC_AD4_PAD1_GPMC_AD5 << reg_offset_gp) | (1<<pad_indicator_bp))

#define a16_gpmc_ad6   (0 | (CONTROL_CORE_PAD0_GPMC_AD6_PAD1_GPMC_AD7 << reg_offset_gp) | (0<<pad_indicator_bp))
#define a16_sdmmc2_dat6   (1 | (CONTROL_CORE_PAD0_GPMC_AD6_PAD1_GPMC_AD7 << reg_offset_gp) | (0<<pad_indicator_bp))
#define a16_sdmmc2_dir_cmd   (2 | (CONTROL_CORE_PAD0_GPMC_AD6_PAD1_GPMC_AD7 << reg_offset_gp) | (0<<pad_indicator_bp))

#define b16_gpmc_ad7   (0 | (CONTROL_CORE_PAD0_GPMC_AD6_PAD1_GPMC_AD7 << reg_offset_gp) | (1<<pad_indicator_bp))
#define b16_sdmmc2_dat7   (1 | (CONTROL_CORE_PAD0_GPMC_AD6_PAD1_GPMC_AD7 << reg_offset_gp) | (1<<pad_indicator_bp))
#define b16_sdmmc2_dir_fdbk   (2 | (CONTROL_CORE_PAD0_GPMC_AD6_PAD1_GPMC_AD7 << reg_offset_gp) | (1<<pad_indicator_bp))

#define c16_gpmc_ad8   (0 | (CONTROL_CORE_PAD0_GPMC_AD8_PAD1_GPMC_AD9 << reg_offset_gp) | (0<<pad_indicator_bp))
#define c16_kpd_row0   (1 | (CONTROL_CORE_PAD0_GPMC_AD8_PAD1_GPMC_AD9 << reg_offset_gp) | (0<<pad_indicator_bp))
#define c16_gpio_32   (3 | (CONTROL_CORE_PAD0_GPMC_AD8_PAD1_GPMC_AD9 << reg_offset_gp) | (0<<pad_indicator_bp))
#define c16_sdmmc1_dat0   (5 | (CONTROL_CORE_PAD0_GPMC_AD8_PAD1_GPMC_AD9 << reg_offset_gp) | (0<<pad_indicator_bp))

#define d16_gpmc_ad9   (0 | (CONTROL_CORE_PAD0_GPMC_AD8_PAD1_GPMC_AD9 << reg_offset_gp) | (1<<pad_indicator_bp))
#define d16_kpd_row1   (1 | (CONTROL_CORE_PAD0_GPMC_AD8_PAD1_GPMC_AD9 << reg_offset_gp) | (1<<pad_indicator_bp))
#define d16_gpio_33   (3 | (CONTROL_CORE_PAD0_GPMC_AD8_PAD1_GPMC_AD9 << reg_offset_gp) | (1<<pad_indicator_bp))
#define d16_sdmmc1_dat1   (5 | (CONTROL_CORE_PAD0_GPMC_AD8_PAD1_GPMC_AD9 << reg_offset_gp) | (1<<pad_indicator_bp))

#define c17_gpmc_ad10   (0 | (CONTROL_CORE_PAD0_GPMC_AD10_PAD1_GPMC_AD11 << reg_offset_gp) | (0<<pad_indicator_bp))
#define c17_kpd_row2   (1 | (CONTROL_CORE_PAD0_GPMC_AD10_PAD1_GPMC_AD11 << reg_offset_gp) | (0<<pad_indicator_bp))
#define c17_gpio_34   (3 | (CONTROL_CORE_PAD0_GPMC_AD10_PAD1_GPMC_AD11 << reg_offset_gp) | (0<<pad_indicator_bp))
#define c17_sdmmc1_dat2   (5 | (CONTROL_CORE_PAD0_GPMC_AD10_PAD1_GPMC_AD11 << reg_offset_gp) | (0<<pad_indicator_bp))

#define d17_gpmc_ad11   (0 | (CONTROL_CORE_PAD0_GPMC_AD10_PAD1_GPMC_AD11 << reg_offset_gp) | (1<<pad_indicator_bp))
#define d17_kpd_row3   (1 | (CONTROL_CORE_PAD0_GPMC_AD10_PAD1_GPMC_AD11 << reg_offset_gp) | (1<<pad_indicator_bp))
#define d17_gpio_35   (3 | (CONTROL_CORE_PAD0_GPMC_AD10_PAD1_GPMC_AD11 << reg_offset_gp) | (1<<pad_indicator_bp))
#define d17_sdmmc1_dat3   (5 | (CONTROL_CORE_PAD0_GPMC_AD10_PAD1_GPMC_AD11 << reg_offset_gp) | (1<<pad_indicator_bp))

#define c18_gpmc_ad12   (0 | (CONTROL_CORE_PAD0_GPMC_AD12_PAD1_GPMC_AD13 << reg_offset_gp) | (0<<pad_indicator_bp))
#define c18_kpd_col0   (1 | (CONTROL_CORE_PAD0_GPMC_AD12_PAD1_GPMC_AD13 << reg_offset_gp) | (0<<pad_indicator_bp))
#define c18_gpio_36   (3 | (CONTROL_CORE_PAD0_GPMC_AD12_PAD1_GPMC_AD13 << reg_offset_gp) | (0<<pad_indicator_bp))
#define c18_sdmmc1_dat4   (5 | (CONTROL_CORE_PAD0_GPMC_AD12_PAD1_GPMC_AD13 << reg_offset_gp) | (0<<pad_indicator_bp))

#define d18_gpmc_ad13   (0 | (CONTROL_CORE_PAD0_GPMC_AD12_PAD1_GPMC_AD13 << reg_offset_gp) | (1<<pad_indicator_bp))
#define d18_kpd_col1   (1 | (CONTROL_CORE_PAD0_GPMC_AD12_PAD1_GPMC_AD13 << reg_offset_gp) | (1<<pad_indicator_bp))
#define d18_gpio_37   (3 | (CONTROL_CORE_PAD0_GPMC_AD12_PAD1_GPMC_AD13 << reg_offset_gp) | (1<<pad_indicator_bp))
#define d18_sdmmc1_dat5   (5 | (CONTROL_CORE_PAD0_GPMC_AD12_PAD1_GPMC_AD13 << reg_offset_gp) | (1<<pad_indicator_bp))

#define c19_gpmc_ad14   (0 | (CONTROL_CORE_PAD0_GPMC_AD14_PAD1_GPMC_AD15 << reg_offset_gp) | (0<<pad_indicator_bp))
#define c19_kpd_col2   (1 | (CONTROL_CORE_PAD0_GPMC_AD14_PAD1_GPMC_AD15 << reg_offset_gp) | (0<<pad_indicator_bp))
#define c19_gpio_38   (3 | (CONTROL_CORE_PAD0_GPMC_AD14_PAD1_GPMC_AD15 << reg_offset_gp) | (0<<pad_indicator_bp))
#define c19_sdmmc1_dat6   (5 | (CONTROL_CORE_PAD0_GPMC_AD14_PAD1_GPMC_AD15 << reg_offset_gp) | (0<<pad_indicator_bp))

#define d19_gpmc_ad15   (0 | (CONTROL_CORE_PAD0_GPMC_AD14_PAD1_GPMC_AD15 << reg_offset_gp) | (1<<pad_indicator_bp))
#define d19_kpd_col3   (1 | (CONTROL_CORE_PAD0_GPMC_AD14_PAD1_GPMC_AD15 << reg_offset_gp) | (1<<pad_indicator_bp))
#define d19_gpio_39   (3 | (CONTROL_CORE_PAD0_GPMC_AD14_PAD1_GPMC_AD15 << reg_offset_gp) | (1<<pad_indicator_bp))
#define d19_sdmmc1_dat7   (5 | (CONTROL_CORE_PAD0_GPMC_AD14_PAD1_GPMC_AD15 << reg_offset_gp) | (1<<pad_indicator_bp))

#define b17_gpmc_a16   (0 | (CONTROL_CORE_PAD0_GPMC_A16_PAD1_GPMC_A17 << reg_offset_gp) | (0<<pad_indicator_bp))
#define b17_kpd_row4   (1 | (CONTROL_CORE_PAD0_GPMC_A16_PAD1_GPMC_A17 << reg_offset_gp) | (0<<pad_indicator_bp))
#define b17_gpio_40   (3 | (CONTROL_CORE_PAD0_GPMC_A16_PAD1_GPMC_A17 << reg_offset_gp) | (0<<pad_indicator_bp))
#define b17_venc_656_data0   (4 | (CONTROL_CORE_PAD0_GPMC_A16_PAD1_GPMC_A17 << reg_offset_gp) | (0<<pad_indicator_bp))
#define b17_safe_mode   (7 | (CONTROL_CORE_PAD0_GPMC_A16_PAD1_GPMC_A17 << reg_offset_gp) | (0<<pad_indicator_bp))

#define a18_gpmc_a17   (0 | (CONTROL_CORE_PAD0_GPMC_A16_PAD1_GPMC_A17 << reg_offset_gp) | (1<<pad_indicator_bp))
#define a18_kpd_row5   (1 | (CONTROL_CORE_PAD0_GPMC_A16_PAD1_GPMC_A17 << reg_offset_gp) | (1<<pad_indicator_bp))
#define a18_gpio_41   (3 | (CONTROL_CORE_PAD0_GPMC_A16_PAD1_GPMC_A17 << reg_offset_gp) | (1<<pad_indicator_bp))
#define a18_venc_656_data1   (4 | (CONTROL_CORE_PAD0_GPMC_A16_PAD1_GPMC_A17 << reg_offset_gp) | (1<<pad_indicator_bp))
#define a18_safe_mode   (7 | (CONTROL_CORE_PAD0_GPMC_A16_PAD1_GPMC_A17 << reg_offset_gp) | (1<<pad_indicator_bp))

#define b18_gpmc_a18   (0 | (CONTROL_CORE_PAD0_GPMC_A18_PAD1_GPMC_A19 << reg_offset_gp) | (0<<pad_indicator_bp))
#define b18_kpd_row6   (1 | (CONTROL_CORE_PAD0_GPMC_A18_PAD1_GPMC_A19 << reg_offset_gp) | (0<<pad_indicator_bp))
#define b18_gpio_42   (3 | (CONTROL_CORE_PAD0_GPMC_A18_PAD1_GPMC_A19 << reg_offset_gp) | (0<<pad_indicator_bp))
#define b18_venc_656_data2   (4 | (CONTROL_CORE_PAD0_GPMC_A18_PAD1_GPMC_A19 << reg_offset_gp) | (0<<pad_indicator_bp))
#define b18_safe_mode   (7 | (CONTROL_CORE_PAD0_GPMC_A18_PAD1_GPMC_A19 << reg_offset_gp) | (0<<pad_indicator_bp))

#define a19_gpmc_a19   (0 | (CONTROL_CORE_PAD0_GPMC_A18_PAD1_GPMC_A19 << reg_offset_gp) | (1<<pad_indicator_bp))
#define a19_kpd_row7   (1 | (CONTROL_CORE_PAD0_GPMC_A18_PAD1_GPMC_A19 << reg_offset_gp) | (1<<pad_indicator_bp))
#define a19_gpio_43   (3 | (CONTROL_CORE_PAD0_GPMC_A18_PAD1_GPMC_A19 << reg_offset_gp) | (1<<pad_indicator_bp))
#define a19_venc_656_data3   (4 | (CONTROL_CORE_PAD0_GPMC_A18_PAD1_GPMC_A19 << reg_offset_gp) | (1<<pad_indicator_bp))
#define a19_safe_mode   (7 | (CONTROL_CORE_PAD0_GPMC_A18_PAD1_GPMC_A19 << reg_offset_gp) | (1<<pad_indicator_bp))

#define b19_gpmc_a20   (0 | (CONTROL_CORE_PAD0_GPMC_A20_PAD1_GPMC_A21 << reg_offset_gp) | (0<<pad_indicator_bp))
#define b19_kpd_col4   (1 | (CONTROL_CORE_PAD0_GPMC_A20_PAD1_GPMC_A21 << reg_offset_gp) | (0<<pad_indicator_bp))
#define b19_gpio_44   (3 | (CONTROL_CORE_PAD0_GPMC_A20_PAD1_GPMC_A21 << reg_offset_gp) | (0<<pad_indicator_bp))
#define b19_venc_656_data4   (4 | (CONTROL_CORE_PAD0_GPMC_A20_PAD1_GPMC_A21 << reg_offset_gp) | (0<<pad_indicator_bp))
#define b19_safe_mode   (7 | (CONTROL_CORE_PAD0_GPMC_A20_PAD1_GPMC_A21 << reg_offset_gp) | (0<<pad_indicator_bp))

#define b20_gpmc_a21   (0 | (CONTROL_CORE_PAD0_GPMC_A20_PAD1_GPMC_A21 << reg_offset_gp) | (1<<pad_indicator_bp))
#define b20_kpd_col5   (1 | (CONTROL_CORE_PAD0_GPMC_A20_PAD1_GPMC_A21 << reg_offset_gp) | (1<<pad_indicator_bp))
#define b20_gpio_45   (3 | (CONTROL_CORE_PAD0_GPMC_A20_PAD1_GPMC_A21 << reg_offset_gp) | (1<<pad_indicator_bp))
#define b20_venc_656_data5   (4 | (CONTROL_CORE_PAD0_GPMC_A20_PAD1_GPMC_A21 << reg_offset_gp) | (1<<pad_indicator_bp))
#define b20_safe_mode   (7 | (CONTROL_CORE_PAD0_GPMC_A20_PAD1_GPMC_A21 << reg_offset_gp) | (1<<pad_indicator_bp))

#define a21_gpmc_a22   (0 | (CONTROL_CORE_PAD0_GPMC_A22_PAD1_GPMC_A23 << reg_offset_gp) | (0<<pad_indicator_bp))
#define a21_kpd_col6   (1 | (CONTROL_CORE_PAD0_GPMC_A22_PAD1_GPMC_A23 << reg_offset_gp) | (0<<pad_indicator_bp))
#define a21_gpio_46   (3 | (CONTROL_CORE_PAD0_GPMC_A22_PAD1_GPMC_A23 << reg_offset_gp) | (0<<pad_indicator_bp))
#define a21_venc_656_data6   (4 | (CONTROL_CORE_PAD0_GPMC_A22_PAD1_GPMC_A23 << reg_offset_gp) | (0<<pad_indicator_bp))
#define a21_safe_mode   (7 | (CONTROL_CORE_PAD0_GPMC_A22_PAD1_GPMC_A23 << reg_offset_gp) | (0<<pad_indicator_bp))

#define b21_gpmc_a23   (0 | (CONTROL_CORE_PAD0_GPMC_A22_PAD1_GPMC_A23 << reg_offset_gp) | (1<<pad_indicator_bp))
#define b21_kpd_col7   (1 | (CONTROL_CORE_PAD0_GPMC_A22_PAD1_GPMC_A23 << reg_offset_gp) | (1<<pad_indicator_bp))
#define b21_gpio_47   (3 | (CONTROL_CORE_PAD0_GPMC_A22_PAD1_GPMC_A23 << reg_offset_gp) | (1<<pad_indicator_bp))
#define b21_venc_656_data7   (4 | (CONTROL_CORE_PAD0_GPMC_A22_PAD1_GPMC_A23 << reg_offset_gp) | (1<<pad_indicator_bp))
#define b21_safe_mode   (7 | (CONTROL_CORE_PAD0_GPMC_A22_PAD1_GPMC_A23 << reg_offset_gp) | (1<<pad_indicator_bp))

#define c20_gpmc_a24   (0 | (CONTROL_CORE_PAD0_GPMC_A24_PAD1_GPMC_A25 << reg_offset_gp) | (0<<pad_indicator_bp))
#define c20_kpd_col8   (1 | (CONTROL_CORE_PAD0_GPMC_A24_PAD1_GPMC_A25 << reg_offset_gp) | (0<<pad_indicator_bp))
#define c20_gpio_48   (3 | (CONTROL_CORE_PAD0_GPMC_A24_PAD1_GPMC_A25 << reg_offset_gp) | (0<<pad_indicator_bp))
#define c20_safe_mode   (7 | (CONTROL_CORE_PAD0_GPMC_A24_PAD1_GPMC_A25 << reg_offset_gp) | (0<<pad_indicator_bp))

#define d20_gpmc_a25   (0 | (CONTROL_CORE_PAD0_GPMC_A24_PAD1_GPMC_A25 << reg_offset_gp) | (1<<pad_indicator_bp))
#define d20_gpio_49   (3 | (CONTROL_CORE_PAD0_GPMC_A24_PAD1_GPMC_A25 << reg_offset_gp) | (1<<pad_indicator_bp))
#define d20_safe_mode   (7 | (CONTROL_CORE_PAD0_GPMC_A24_PAD1_GPMC_A25 << reg_offset_gp) | (1<<pad_indicator_bp))

#define b25_gpmc_ncs0   (0 | (CONTROL_CORE_PAD0_GPMC_NCS0_PAD1_GPMC_NCS1 << reg_offset_gp) | (0<<pad_indicator_bp))
#define b25_gpio_50   (3 | (CONTROL_CORE_PAD0_GPMC_NCS0_PAD1_GPMC_NCS1 << reg_offset_gp) | (0<<pad_indicator_bp))
#define b25_sys_ndmareq0   (4 | (CONTROL_CORE_PAD0_GPMC_NCS0_PAD1_GPMC_NCS1 << reg_offset_gp) | (0<<pad_indicator_bp))

#define c21_gpmc_ncs1   (0 | (CONTROL_CORE_PAD0_GPMC_NCS0_PAD1_GPMC_NCS1 << reg_offset_gp) | (1<<pad_indicator_bp))
#define c21_gpio_51   (3 | (CONTROL_CORE_PAD0_GPMC_NCS0_PAD1_GPMC_NCS1 << reg_offset_gp) | (1<<pad_indicator_bp))
#define c21_safe_mode   (7 | (CONTROL_CORE_PAD0_GPMC_NCS0_PAD1_GPMC_NCS1 << reg_offset_gp) | (1<<pad_indicator_bp))

#define d21_gpmc_ncs2   (0 | (CONTROL_CORE_PAD0_GPMC_NCS2_PAD1_GPMC_NCS3 << reg_offset_gp) | (0<<pad_indicator_bp))
#define d21_kpd_row8   (1 | (CONTROL_CORE_PAD0_GPMC_NCS2_PAD1_GPMC_NCS3 << reg_offset_gp) | (0<<pad_indicator_bp))
#define d21_gpio_52   (3 | (CONTROL_CORE_PAD0_GPMC_NCS2_PAD1_GPMC_NCS3 << reg_offset_gp) | (0<<pad_indicator_bp))
#define d21_safe_mode   (7 | (CONTROL_CORE_PAD0_GPMC_NCS2_PAD1_GPMC_NCS3 << reg_offset_gp) | (0<<pad_indicator_bp))

#define c22_gpmc_ncs3   (0 | (CONTROL_CORE_PAD0_GPMC_NCS2_PAD1_GPMC_NCS3 << reg_offset_gp) | (1<<pad_indicator_bp))
#define c22_gpmc_dir   (1 | (CONTROL_CORE_PAD0_GPMC_NCS2_PAD1_GPMC_NCS3 << reg_offset_gp) | (1<<pad_indicator_bp))
#define c22_gpio_53   (3 | (CONTROL_CORE_PAD0_GPMC_NCS2_PAD1_GPMC_NCS3 << reg_offset_gp) | (1<<pad_indicator_bp))
#define c22_safe_mode   (7 | (CONTROL_CORE_PAD0_GPMC_NCS2_PAD1_GPMC_NCS3 << reg_offset_gp) | (1<<pad_indicator_bp))

#define c25_gpmc_nwp   (0 | (CONTROL_CORE_PAD0_GPMC_NWP_PAD1_GPMC_CLK << reg_offset_gp) | (0<<pad_indicator_bp))
#define c25_dsi1_te0   (1 | (CONTROL_CORE_PAD0_GPMC_NWP_PAD1_GPMC_CLK << reg_offset_gp) | (0<<pad_indicator_bp))
#define c25_gpio_54   (3 | (CONTROL_CORE_PAD0_GPMC_NWP_PAD1_GPMC_CLK << reg_offset_gp) | (0<<pad_indicator_bp))
#define c25_sys_ndmareq1   (4 | (CONTROL_CORE_PAD0_GPMC_NWP_PAD1_GPMC_CLK << reg_offset_gp) | (0<<pad_indicator_bp))

#define b22_gpmc_clk   (0 | (CONTROL_CORE_PAD0_GPMC_NWP_PAD1_GPMC_CLK << reg_offset_gp) | (1<<pad_indicator_bp))
#define b22_gpio_55   (3 | (CONTROL_CORE_PAD0_GPMC_NWP_PAD1_GPMC_CLK << reg_offset_gp) | (1<<pad_indicator_bp))
#define b22_sys_ndmareq2   (4 | (CONTROL_CORE_PAD0_GPMC_NWP_PAD1_GPMC_CLK << reg_offset_gp) | (1<<pad_indicator_bp))
#define b22_sdmmc1_cmd   (5 | (CONTROL_CORE_PAD0_GPMC_NWP_PAD1_GPMC_CLK << reg_offset_gp) | (1<<pad_indicator_bp))

#define d25_gpmc_nadv_ale   (0 | (CONTROL_CORE_PAD0_GPMC_NADV_ALE_PAD1_GPMC_NOE << reg_offset_gp) | (0<<pad_indicator_bp))
#define d25_dsi1_te1   (1 | (CONTROL_CORE_PAD0_GPMC_NADV_ALE_PAD1_GPMC_NOE << reg_offset_gp) | (0<<pad_indicator_bp))
#define d25_gpio_56   (3 | (CONTROL_CORE_PAD0_GPMC_NADV_ALE_PAD1_GPMC_NOE << reg_offset_gp) | (0<<pad_indicator_bp))
#define d25_sys_ndmareq3   (4 | (CONTROL_CORE_PAD0_GPMC_NADV_ALE_PAD1_GPMC_NOE << reg_offset_gp) | (0<<pad_indicator_bp))
#define d25_sdmmc1_clk   (5 | (CONTROL_CORE_PAD0_GPMC_NADV_ALE_PAD1_GPMC_NOE << reg_offset_gp) | (0<<pad_indicator_bp))

#define b11_gpmc_noe   (0 | (CONTROL_CORE_PAD0_GPMC_NADV_ALE_PAD1_GPMC_NOE << reg_offset_gp) | (1<<pad_indicator_bp))
#define b11_sdmmc2_clk   (1 | (CONTROL_CORE_PAD0_GPMC_NADV_ALE_PAD1_GPMC_NOE << reg_offset_gp) | (1<<pad_indicator_bp))

#define b12_gpmc_nwe   (0 | (CONTROL_CORE_PAD0_GPMC_NWE_PAD1_GPMC_NBE0_CLE << reg_offset_gp) | (0<<pad_indicator_bp))
#define b12_sdmmc2_cmd   (1 | (CONTROL_CORE_PAD0_GPMC_NWE_PAD1_GPMC_NBE0_CLE << reg_offset_gp) | (0<<pad_indicator_bp))

#define c23_gpmc_nbe0_cle   (0 | (CONTROL_CORE_PAD0_GPMC_NWE_PAD1_GPMC_NBE0_CLE << reg_offset_gp) | (1<<pad_indicator_bp))
#define c23_dsi2_te0   (1 | (CONTROL_CORE_PAD0_GPMC_NWE_PAD1_GPMC_NBE0_CLE << reg_offset_gp) | (1<<pad_indicator_bp))
#define c23_gpio_59   (3 | (CONTROL_CORE_PAD0_GPMC_NWE_PAD1_GPMC_NBE0_CLE << reg_offset_gp) | (1<<pad_indicator_bp))

#define d22_gpmc_nbe1   (0 | (CONTROL_CORE_PAD0_GPMC_NBE1_PAD1_GPMC_WAIT0 << reg_offset_gp) | (0<<pad_indicator_bp))
#define d22_gpio_60   (3 | (CONTROL_CORE_PAD0_GPMC_NBE1_PAD1_GPMC_WAIT0 << reg_offset_gp) | (0<<pad_indicator_bp))
#define d22_safe_mode   (7 | (CONTROL_CORE_PAD0_GPMC_NBE1_PAD1_GPMC_WAIT0 << reg_offset_gp) | (0<<pad_indicator_bp))

#define b26_gpmc_wait0   (0 | (CONTROL_CORE_PAD0_GPMC_NBE1_PAD1_GPMC_WAIT0 << reg_offset_gp) | (1<<pad_indicator_bp))
#define b26_dsi2_te1   (1 | (CONTROL_CORE_PAD0_GPMC_NBE1_PAD1_GPMC_WAIT0 << reg_offset_gp) | (1<<pad_indicator_bp))
#define b26_gpio_61   (3 | (CONTROL_CORE_PAD0_GPMC_NBE1_PAD1_GPMC_WAIT0 << reg_offset_gp) | (1<<pad_indicator_bp))

#define b23_gpmc_wait1   (0 | (CONTROL_CORE_PAD0_GPMC_WAIT1_PAD1_GPMC_WAIT2 << reg_offset_gp) | (0<<pad_indicator_bp))
#define b23_gpio_62   (3 | (CONTROL_CORE_PAD0_GPMC_WAIT1_PAD1_GPMC_WAIT2 << reg_offset_gp) | (0<<pad_indicator_bp))
#define b23_safe_mode   (7 | (CONTROL_CORE_PAD0_GPMC_WAIT1_PAD1_GPMC_WAIT2 << reg_offset_gp) | (0<<pad_indicator_bp))

#define d23_gpmc_wait2   (0 | (CONTROL_CORE_PAD0_GPMC_WAIT1_PAD1_GPMC_WAIT2 << reg_offset_gp) | (1<<pad_indicator_bp))
#define d23_usbc1_icusb_txen   (1 | (CONTROL_CORE_PAD0_GPMC_WAIT1_PAD1_GPMC_WAIT2 << reg_offset_gp) | (1<<pad_indicator_bp))
#define d23_gpio_100   (3 | (CONTROL_CORE_PAD0_GPMC_WAIT1_PAD1_GPMC_WAIT2 << reg_offset_gp) | (1<<pad_indicator_bp))
#define d23_sys_ndmareq0   (4 | (CONTROL_CORE_PAD0_GPMC_WAIT1_PAD1_GPMC_WAIT2 << reg_offset_gp) | (1<<pad_indicator_bp))
#define d23_safe_mode   (7 | (CONTROL_CORE_PAD0_GPMC_WAIT1_PAD1_GPMC_WAIT2 << reg_offset_gp) | (1<<pad_indicator_bp))

#define a24_gpmc_ncs4   (0 | (CONTROL_CORE_PAD0_GPMC_NCS4_PAD1_GPMC_NCS5 << reg_offset_gp) | (0<<pad_indicator_bp))
#define a24_dsi1_te0   (1 | (CONTROL_CORE_PAD0_GPMC_NCS4_PAD1_GPMC_NCS5 << reg_offset_gp) | (0<<pad_indicator_bp))
#define a24_gpio_101   (3 | (CONTROL_CORE_PAD0_GPMC_NCS4_PAD1_GPMC_NCS5 << reg_offset_gp) | (0<<pad_indicator_bp))
#define a24_sys_ndmareq1   (4 | (CONTROL_CORE_PAD0_GPMC_NCS4_PAD1_GPMC_NCS5 << reg_offset_gp) | (0<<pad_indicator_bp))
#define a24_safe_mode   (7 | (CONTROL_CORE_PAD0_GPMC_NCS4_PAD1_GPMC_NCS5 << reg_offset_gp) | (0<<pad_indicator_bp))

#define b24_gpmc_ncs5   (0 | (CONTROL_CORE_PAD0_GPMC_NCS4_PAD1_GPMC_NCS5 << reg_offset_gp) | (1<<pad_indicator_bp))
#define b24_dsi1_te1   (1 | (CONTROL_CORE_PAD0_GPMC_NCS4_PAD1_GPMC_NCS5 << reg_offset_gp) | (1<<pad_indicator_bp))
#define b24_gpio_102   (3 | (CONTROL_CORE_PAD0_GPMC_NCS4_PAD1_GPMC_NCS5 << reg_offset_gp) | (1<<pad_indicator_bp))
#define b24_sys_ndmareq2   (4 | (CONTROL_CORE_PAD0_GPMC_NCS4_PAD1_GPMC_NCS5 << reg_offset_gp) | (1<<pad_indicator_bp))
#define b24_safe_mode   (7 | (CONTROL_CORE_PAD0_GPMC_NCS4_PAD1_GPMC_NCS5 << reg_offset_gp) | (1<<pad_indicator_bp))

#define c24_gpmc_ncs6   (0 | (CONTROL_CORE_PAD0_GPMC_NCS6_PAD1_GPMC_NCS7 << reg_offset_gp) | (0<<pad_indicator_bp))
#define c24_dsi2_te0   (1 | (CONTROL_CORE_PAD0_GPMC_NCS6_PAD1_GPMC_NCS7 << reg_offset_gp) | (0<<pad_indicator_bp))
#define c24_gpio_103   (3 | (CONTROL_CORE_PAD0_GPMC_NCS6_PAD1_GPMC_NCS7 << reg_offset_gp) | (0<<pad_indicator_bp))
#define c24_sys_ndmareq3   (4 | (CONTROL_CORE_PAD0_GPMC_NCS6_PAD1_GPMC_NCS7 << reg_offset_gp) | (0<<pad_indicator_bp))
#define c24_safe_mode   (7 | (CONTROL_CORE_PAD0_GPMC_NCS6_PAD1_GPMC_NCS7 << reg_offset_gp) | (0<<pad_indicator_bp))

#define d24_gpmc_ncs7   (0 | (CONTROL_CORE_PAD0_GPMC_NCS6_PAD1_GPMC_NCS7 << reg_offset_gp) | (1<<pad_indicator_bp))
#define d24_dsi2_te1   (1 | (CONTROL_CORE_PAD0_GPMC_NCS6_PAD1_GPMC_NCS7 << reg_offset_gp) | (1<<pad_indicator_bp))
#define d24_gpio_102   (3 | (CONTROL_CORE_PAD0_GPMC_NCS6_PAD1_GPMC_NCS7 << reg_offset_gp) | (1<<pad_indicator_bp))
#define d24_safe_mode   (7 | (CONTROL_CORE_PAD0_GPMC_NCS6_PAD1_GPMC_NCS7 << reg_offset_gp) | (1<<pad_indicator_bp))

#define p3_dsi1_dx0   (0 | (0 << reg_offset_gp) | (0<<pad_indicator_bp))

#define p4_dsi1_dy0   (0 | (0 << reg_offset_gp) | (0<<pad_indicator_bp))

#define n3_dsi1_dx1   (0 | (0 << reg_offset_gp) | (0<<pad_indicator_bp))

#define n4_dsi1_dy1   (0 | (0 << reg_offset_gp) | (0<<pad_indicator_bp))

#define m3_dsi1_dx2   (0 | (0 << reg_offset_gp) | (0<<pad_indicator_bp))

#define m4_dsi1_dy2   (0 | (0 << reg_offset_gp) | (0<<pad_indicator_bp))

#define l3_dsi1_dx3   (0 | (0 << reg_offset_gp) | (0<<pad_indicator_bp))

#define l4_dsi1_dy3   (0 | (0 << reg_offset_gp) | (0<<pad_indicator_bp))

#define k3_dsi1_dx4   (0 | (0 << reg_offset_gp) | (0<<pad_indicator_bp))

#define k4_dsi1_dy4   (0 | (0 << reg_offset_gp) | (0<<pad_indicator_bp))

#define t3_dsi2_dx0   (0 | (0 << reg_offset_gp) | (0<<pad_indicator_bp))

#define t4_dsi2_dy0   (0 | (0 << reg_offset_gp) | (0<<pad_indicator_bp))

#define u3_dsi2_dx1   (0 | (0 << reg_offset_gp) | (0<<pad_indicator_bp))

#define u4_dsi2_dy1   (0 | (0 << reg_offset_gp) | (0<<pad_indicator_bp))

#define v3_dsi2_dx2   (0 | (0 << reg_offset_gp) | (0<<pad_indicator_bp))

#define v4_dsi2_dy2   (0 | (0 << reg_offset_gp) | (0<<pad_indicator_bp))

#define b7_cvideo_tvout   (0 | (0 << reg_offset_gp) | (0<<pad_indicator_bp))

#define c7_cvideo_vfb   (0 | (0 << reg_offset_gp) | (0<<pad_indicator_bp))

#define d7_cvideo_rset   (0 | (0 << reg_offset_gp) | (0<<pad_indicator_bp))

#define b9_hdmi_hpd   (0 | (0 << reg_offset_gp) | (0<<pad_indicator_bp))
#define b9_gpio_63   (3 | (0 << reg_offset_gp) | (0<<pad_indicator_bp))
#define b9_safe_mode   (7 | (0 << reg_offset_gp) | (0<<pad_indicator_bp))

#define b10_hdmi_cec   (0 | (0 << reg_offset_gp) | (0<<pad_indicator_bp))
#define b10_gpio_64   (3 | (0 << reg_offset_gp) | (0<<pad_indicator_bp))
#define b10_safe_mode   (7 | (0 << reg_offset_gp) | (0<<pad_indicator_bp))

#define a8_hdmi_ddc_scl   (0 | (0 << reg_offset_gp) | (0<<pad_indicator_bp))
#define a8_gpio_65   (3 | (0 << reg_offset_gp) | (0<<pad_indicator_bp))
#define a8_safe_mode   (7 | (0 << reg_offset_gp) | (0<<pad_indicator_bp))

#define b8_hdmi_ddc_sda   (0 | (0 << reg_offset_gp) | (0<<pad_indicator_bp))
#define b8_gpio_66   (3 | (0 << reg_offset_gp) | (0<<pad_indicator_bp))
#define b8_safe_mode   (7 | (0 << reg_offset_gp) | (0<<pad_indicator_bp))

#define c8_hdmi_data2x   (0 | (0 << reg_offset_gp) | (0<<pad_indicator_bp))

#define d8_hdmi_data2y   (0 | (0 << reg_offset_gp) | (0<<pad_indicator_bp))

#define c9_hdmi_data1x   (0 | (0 << reg_offset_gp) | (0<<pad_indicator_bp))

#define d9_hdmi_data1y   (0 | (0 << reg_offset_gp) | (0<<pad_indicator_bp))

#define c10_hdmi_data0x   (0 | (0 << reg_offset_gp) | (0<<pad_indicator_bp))

#define d10_hdmi_data0y   (0 | (0 << reg_offset_gp) | (0<<pad_indicator_bp))

#define c11_hdmi_clockx   (0 | (0 << reg_offset_gp) | (0<<pad_indicator_bp))

#define d11_hdmi_clocky   (0 | (0 << reg_offset_gp) | (0<<pad_indicator_bp))

#define r26_csi21_dx0   (0 | (CONTROL_CORE_PAD0_CSI21_DX0_PAD1_CSI21_DY0 << reg_offset_gp) | (0<<pad_indicator_bp))
#define r26_gpio_67   (3 | (CONTROL_CORE_PAD0_CSI21_DX0_PAD1_CSI21_DY0 << reg_offset_gp) | (0<<pad_indicator_bp))
#define r26_safe_mode   (7 | (CONTROL_CORE_PAD0_CSI21_DX0_PAD1_CSI21_DY0 << reg_offset_gp) | (0<<pad_indicator_bp))

#define r25_csi21_dy0   (0 | (CONTROL_CORE_PAD0_CSI21_DX0_PAD1_CSI21_DY0 << reg_offset_gp) | (1<<pad_indicator_bp))
#define r25_gpio_68   (3 | (CONTROL_CORE_PAD0_CSI21_DX0_PAD1_CSI21_DY0 << reg_offset_gp) | (1<<pad_indicator_bp))
#define r25_safe_mode   (7 | (CONTROL_CORE_PAD0_CSI21_DX0_PAD1_CSI21_DY0 << reg_offset_gp) | (1<<pad_indicator_bp))

#define t26_csi21_dx1   (0 | (CONTROL_CORE_PAD0_CSI21_DX1_PAD1_CSI21_DY1 << reg_offset_gp) | (0<<pad_indicator_bp))
#define t26_gpio_69   (3 | (CONTROL_CORE_PAD0_CSI21_DX1_PAD1_CSI21_DY1 << reg_offset_gp) | (0<<pad_indicator_bp))
#define t26_safe_mode   (7 | (CONTROL_CORE_PAD0_CSI21_DX1_PAD1_CSI21_DY1 << reg_offset_gp) | (0<<pad_indicator_bp))

#define t25_csi21_dy1   (0 | (CONTROL_CORE_PAD0_CSI21_DX1_PAD1_CSI21_DY1 << reg_offset_gp) | (1<<pad_indicator_bp))
#define t25_gpio_70   (3 | (CONTROL_CORE_PAD0_CSI21_DX1_PAD1_CSI21_DY1 << reg_offset_gp) | (1<<pad_indicator_bp))
#define t25_safe_mode   (7 | (CONTROL_CORE_PAD0_CSI21_DX1_PAD1_CSI21_DY1 << reg_offset_gp) | (1<<pad_indicator_bp))

#define u26_csi21_dx2   (0 | (CONTROL_CORE_PAD0_CSI21_DX2_PAD1_CSI21_DY2 << reg_offset_gp) | (0<<pad_indicator_bp))
#define u26_gpio_71   (3 | (CONTROL_CORE_PAD0_CSI21_DX2_PAD1_CSI21_DY2 << reg_offset_gp) | (0<<pad_indicator_bp))
#define u26_safe_mode   (7 | (CONTROL_CORE_PAD0_CSI21_DX2_PAD1_CSI21_DY2 << reg_offset_gp) | (0<<pad_indicator_bp))

#define u25_csi21_dy2   (0 | (CONTROL_CORE_PAD0_CSI21_DX2_PAD1_CSI21_DY2 << reg_offset_gp) | (1<<pad_indicator_bp))
#define u25_gpio_72   (3 | (CONTROL_CORE_PAD0_CSI21_DX2_PAD1_CSI21_DY2 << reg_offset_gp) | (1<<pad_indicator_bp))
#define u25_safe_mode   (7 | (CONTROL_CORE_PAD0_CSI21_DX2_PAD1_CSI21_DY2 << reg_offset_gp) | (1<<pad_indicator_bp))

#define v26_csi21_dx3   (0 | (CONTROL_CORE_PAD0_CSI21_DX3_PAD1_CSI21_DY3 << reg_offset_gp) | (0<<pad_indicator_bp))
#define v26_cam2_d7   (1 | (CONTROL_CORE_PAD0_CSI21_DX3_PAD1_CSI21_DY3 << reg_offset_gp) | (0<<pad_indicator_bp))
#define v26_gpio_73   (3 | (CONTROL_CORE_PAD0_CSI21_DX3_PAD1_CSI21_DY3 << reg_offset_gp) | (0<<pad_indicator_bp))
#define v26_safe_mode   (7 | (CONTROL_CORE_PAD0_CSI21_DX3_PAD1_CSI21_DY3 << reg_offset_gp) | (0<<pad_indicator_bp))

#define v25_csi21_dy3   (0 | (CONTROL_CORE_PAD0_CSI21_DX3_PAD1_CSI21_DY3 << reg_offset_gp) | (1<<pad_indicator_bp))
#define v25_cam2_d6   (1 | (CONTROL_CORE_PAD0_CSI21_DX3_PAD1_CSI21_DY3 << reg_offset_gp) | (1<<pad_indicator_bp))
#define v25_gpio_74   (3 | (CONTROL_CORE_PAD0_CSI21_DX3_PAD1_CSI21_DY3 << reg_offset_gp) | (1<<pad_indicator_bp))
#define v25_safe_mode   (7 | (CONTROL_CORE_PAD0_CSI21_DX3_PAD1_CSI21_DY3 << reg_offset_gp) | (1<<pad_indicator_bp))

#define w26_csi21_dx4   (0 | (CONTROL_CORE_PAD0_CSI21_DX4_PAD1_CSI21_DY4 << reg_offset_gp) | (0<<pad_indicator_bp))
#define w26_cam2_d5   (1 | (CONTROL_CORE_PAD0_CSI21_DX4_PAD1_CSI21_DY4 << reg_offset_gp) | (0<<pad_indicator_bp))
#define w26_gpio_75   (3 | (CONTROL_CORE_PAD0_CSI21_DX4_PAD1_CSI21_DY4 << reg_offset_gp) | (0<<pad_indicator_bp))
#define w26_safe_mode   (7 | (CONTROL_CORE_PAD0_CSI21_DX4_PAD1_CSI21_DY4 << reg_offset_gp) | (0<<pad_indicator_bp))

#define w25_csi21_dy4   (0 | (CONTROL_CORE_PAD0_CSI21_DX4_PAD1_CSI21_DY4 << reg_offset_gp) | (1<<pad_indicator_bp))
#define w25_cam2_d4   (1 | (CONTROL_CORE_PAD0_CSI21_DX4_PAD1_CSI21_DY4 << reg_offset_gp) | (1<<pad_indicator_bp))
#define w25_gpio_76   (3 | (CONTROL_CORE_PAD0_CSI21_DX4_PAD1_CSI21_DY4 << reg_offset_gp) | (1<<pad_indicator_bp))
#define w25_safe_mode   (7 | (CONTROL_CORE_PAD0_CSI21_DX4_PAD1_CSI21_DY4 << reg_offset_gp) | (1<<pad_indicator_bp))

#define m26_csi22_dx0   (0 | (CONTROL_CORE_PAD0_CSI22_DX0_PAD1_CSI22_DY0 << reg_offset_gp) | (0<<pad_indicator_bp))
#define m26_cam2_d3   (1 | (CONTROL_CORE_PAD0_CSI22_DX0_PAD1_CSI22_DY0 << reg_offset_gp) | (0<<pad_indicator_bp))
#define m26_cam2_d12   (2 | (CONTROL_CORE_PAD0_CSI22_DX0_PAD1_CSI22_DY0 << reg_offset_gp) | (0<<pad_indicator_bp))
#define m26_gpio_77   (3 | (CONTROL_CORE_PAD0_CSI22_DX0_PAD1_CSI22_DY0 << reg_offset_gp) | (0<<pad_indicator_bp))
#define m26_safe_mode   (7 | (CONTROL_CORE_PAD0_CSI22_DX0_PAD1_CSI22_DY0 << reg_offset_gp) | (0<<pad_indicator_bp))

#define m25_csi22_dy0   (0 | (CONTROL_CORE_PAD0_CSI22_DX0_PAD1_CSI22_DY0 << reg_offset_gp) | (1<<pad_indicator_bp))
#define m25_cam2_d2   (1 | (CONTROL_CORE_PAD0_CSI22_DX0_PAD1_CSI22_DY0 << reg_offset_gp) | (1<<pad_indicator_bp))
#define m25_cam2_d13   (2 | (CONTROL_CORE_PAD0_CSI22_DX0_PAD1_CSI22_DY0 << reg_offset_gp) | (1<<pad_indicator_bp))
#define m25_gpio_78   (3 | (CONTROL_CORE_PAD0_CSI22_DX0_PAD1_CSI22_DY0 << reg_offset_gp) | (1<<pad_indicator_bp))
#define m25_safe_mode   (7 | (CONTROL_CORE_PAD0_CSI22_DX0_PAD1_CSI22_DY0 << reg_offset_gp) | (1<<pad_indicator_bp))

#define n26_csi22_dx1   (0 | (CONTROL_CORE_PAD0_CSI22_DX1_PAD1_CSI22_DY1 << reg_offset_gp) | (0<<pad_indicator_bp))
#define n26_cam2_d1   (1 | (CONTROL_CORE_PAD0_CSI22_DX1_PAD1_CSI22_DY1 << reg_offset_gp) | (0<<pad_indicator_bp))
#define n26_cam2_d14   (2 | (CONTROL_CORE_PAD0_CSI22_DX1_PAD1_CSI22_DY1 << reg_offset_gp) | (0<<pad_indicator_bp))
#define n26_gpio_79   (3 | (CONTROL_CORE_PAD0_CSI22_DX1_PAD1_CSI22_DY1 << reg_offset_gp) | (0<<pad_indicator_bp))
#define n26_safe_mode   (7 | (CONTROL_CORE_PAD0_CSI22_DX1_PAD1_CSI22_DY1 << reg_offset_gp) | (0<<pad_indicator_bp))

#define n25_csi22_dy1   (0 | (CONTROL_CORE_PAD0_CSI22_DX1_PAD1_CSI22_DY1 << reg_offset_gp) | (1<<pad_indicator_bp))
#define n25_cam2_d0   (1 | (CONTROL_CORE_PAD0_CSI22_DX1_PAD1_CSI22_DY1 << reg_offset_gp) | (1<<pad_indicator_bp))
#define n25_cam2_d15   (2 | (CONTROL_CORE_PAD0_CSI22_DX1_PAD1_CSI22_DY1 << reg_offset_gp) | (1<<pad_indicator_bp))
#define n25_gpio_80   (3 | (CONTROL_CORE_PAD0_CSI22_DX1_PAD1_CSI22_DY1 << reg_offset_gp) | (1<<pad_indicator_bp))
#define n25_safe_mode   (7 | (CONTROL_CORE_PAD0_CSI22_DX1_PAD1_CSI22_DY1 << reg_offset_gp) | (1<<pad_indicator_bp))

#define n27_csi22_dx2   (0 | (CONTROL_CORE_PAD0_DPM_EMU19_PAD1_CSI22_DX2 << reg_offset_gp) | (1<<pad_indicator_bp))
#define n27_cam2_fid   (2 | (CONTROL_CORE_PAD0_DPM_EMU19_PAD1_CSI22_DX2 << reg_offset_gp) | (1<<pad_indicator_bp))
#define n27_safe_mode   (7 | (CONTROL_CORE_PAD0_DPM_EMU19_PAD1_CSI22_DX2 << reg_offset_gp) | (1<<pad_indicator_bp))

#define m27_csi22_dy2   (0 | (CONTROL_CORE_PAD0_CSI22_DY2 << reg_offset_gp) | (0<<pad_indicator_bp))
#define m27_cam2_wen   (2 | (CONTROL_CORE_PAD0_CSI22_DY2 << reg_offset_gp) | (0<<pad_indicator_bp))
#define m27_safe_mode   (7 | (CONTROL_CORE_PAD0_CSI22_DY2 << reg_offset_gp) | (0<<pad_indicator_bp))

#define t27_cam_shutter   (0 | (CONTROL_CORE_PAD0_CAM_SHUTTER_PAD1_CAM_STROBE << reg_offset_gp) | (0<<pad_indicator_bp))
#define t27_cam2_hs   (2 | (CONTROL_CORE_PAD0_CAM_SHUTTER_PAD1_CAM_STROBE << reg_offset_gp) | (0<<pad_indicator_bp))
#define t27_gpio_81   (3 | (CONTROL_CORE_PAD0_CAM_SHUTTER_PAD1_CAM_STROBE << reg_offset_gp) | (0<<pad_indicator_bp))
#define t27_safe_mode   (7 | (CONTROL_CORE_PAD0_CAM_SHUTTER_PAD1_CAM_STROBE << reg_offset_gp) | (0<<pad_indicator_bp))

#define u27_cam_strobe   (0 | (CONTROL_CORE_PAD0_CAM_SHUTTER_PAD1_CAM_STROBE << reg_offset_gp) | (1<<pad_indicator_bp))
#define u27_cam2_vs   (2 | (CONTROL_CORE_PAD0_CAM_SHUTTER_PAD1_CAM_STROBE << reg_offset_gp) | (1<<pad_indicator_bp))
#define u27_gpio_82   (3 | (CONTROL_CORE_PAD0_CAM_SHUTTER_PAD1_CAM_STROBE << reg_offset_gp) | (1<<pad_indicator_bp))
#define u27_safe_mode   (7 | (CONTROL_CORE_PAD0_CAM_SHUTTER_PAD1_CAM_STROBE << reg_offset_gp) | (1<<pad_indicator_bp))

#define v27_cam_globalreset   (0 | (CONTROL_CORE_PAD0_CAM_GLOBALRESET_PAD1_USBB1_ULPITLL_CLK << reg_offset_gp) | (0<<pad_indicator_bp))
#define v27_cam2_pclk   (2 | (CONTROL_CORE_PAD0_CAM_GLOBALRESET_PAD1_USBB1_ULPITLL_CLK << reg_offset_gp) | (0<<pad_indicator_bp))
#define v27_gpio_83   (3 | (CONTROL_CORE_PAD0_CAM_GLOBALRESET_PAD1_USBB1_ULPITLL_CLK << reg_offset_gp) | (0<<pad_indicator_bp))
#define v27_safe_mode   (7 | (CONTROL_CORE_PAD0_CAM_GLOBALRESET_PAD1_USBB1_ULPITLL_CLK << reg_offset_gp) | (0<<pad_indicator_bp))

#define ae18_usbb1_ulpitll_clk   (0 | (CONTROL_CORE_PAD0_CAM_GLOBALRESET_PAD1_USBB1_ULPITLL_CLK << reg_offset_gp) | (1<<pad_indicator_bp))
#define ae18_hsi1_cawake   (1 | (CONTROL_CORE_PAD0_CAM_GLOBALRESET_PAD1_USBB1_ULPITLL_CLK << reg_offset_gp) | (1<<pad_indicator_bp))
#define ae18_gpio_84   (3 | (CONTROL_CORE_PAD0_CAM_GLOBALRESET_PAD1_USBB1_ULPITLL_CLK << reg_offset_gp) | (1<<pad_indicator_bp))
#define ae18_usbb1_ulpiphy_clk   (4 | (CONTROL_CORE_PAD0_CAM_GLOBALRESET_PAD1_USBB1_ULPITLL_CLK << reg_offset_gp) | (1<<pad_indicator_bp))
#define ae18_attila_hw_dbg20   (6 | (CONTROL_CORE_PAD0_CAM_GLOBALRESET_PAD1_USBB1_ULPITLL_CLK << reg_offset_gp) | (1<<pad_indicator_bp))
#define ae18_safe_mode   (7 | (CONTROL_CORE_PAD0_CAM_GLOBALRESET_PAD1_USBB1_ULPITLL_CLK << reg_offset_gp) | (1<<pad_indicator_bp))

#define ag19_usbb1_ulpitll_stp   (0 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_STP_PAD1_USBB1_ULPITLL_DIR << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag19_hsi1_cadata   (1 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_STP_PAD1_USBB1_ULPITLL_DIR << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag19_mcbsp4_clkr   (2 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_STP_PAD1_USBB1_ULPITLL_DIR << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag19_gpio_85   (3 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_STP_PAD1_USBB1_ULPITLL_DIR << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag19_usbb1_ulpiphy_stp   (4 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_STP_PAD1_USBB1_ULPITLL_DIR << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag19_usbb1_mm_rxdp   (5 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_STP_PAD1_USBB1_ULPITLL_DIR << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag19_attila_hw_dbg21   (6 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_STP_PAD1_USBB1_ULPITLL_DIR << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag19_safe_mode   (7 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_STP_PAD1_USBB1_ULPITLL_DIR << reg_offset_gp) | (0<<pad_indicator_bp))

#define af19_usbb1_ulpitll_dir   (0 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_STP_PAD1_USBB1_ULPITLL_DIR << reg_offset_gp) | (1<<pad_indicator_bp))
#define af19_hsi1_caflag   (1 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_STP_PAD1_USBB1_ULPITLL_DIR << reg_offset_gp) | (1<<pad_indicator_bp))
#define af19_mcbsp4_fsr   (2 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_STP_PAD1_USBB1_ULPITLL_DIR << reg_offset_gp) | (1<<pad_indicator_bp))
#define af19_gpio_86   (3 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_STP_PAD1_USBB1_ULPITLL_DIR << reg_offset_gp) | (1<<pad_indicator_bp))
#define af19_usbb1_ulpiphy_dir   (4 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_STP_PAD1_USBB1_ULPITLL_DIR << reg_offset_gp) | (1<<pad_indicator_bp))
#define ag19_attila_hw_dbg22   (6 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_STP_PAD1_USBB1_ULPITLL_DIR << reg_offset_gp) | (1<<pad_indicator_bp))
#define af19_safe_mode   (7 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_STP_PAD1_USBB1_ULPITLL_DIR << reg_offset_gp) | (1<<pad_indicator_bp))

#define ae19_usbb1_ulpitll_nxt   (0 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_NXT_PAD1_USBB1_ULPITLL_DAT0 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae19_hsi1_acready   (1 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_NXT_PAD1_USBB1_ULPITLL_DAT0 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae19_mcbsp4_fsx   (2 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_NXT_PAD1_USBB1_ULPITLL_DAT0 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae19_gpio_87   (3 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_NXT_PAD1_USBB1_ULPITLL_DAT0 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae19_usbb1_ulpiphy_nxt   (4 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_NXT_PAD1_USBB1_ULPITLL_DAT0 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae19_usbb1_mm_rxdm   (5 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_NXT_PAD1_USBB1_ULPITLL_DAT0 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae19_attila_hw_dbg23   (6 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_NXT_PAD1_USBB1_ULPITLL_DAT0 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae19_safe_mode   (7 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_NXT_PAD1_USBB1_ULPITLL_DAT0 << reg_offset_gp) | (0<<pad_indicator_bp))

#define af18_usbb1_ulpitll_dat0   (0 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_NXT_PAD1_USBB1_ULPITLL_DAT0 << reg_offset_gp) | (1<<pad_indicator_bp))
#define af18_hsi1_acwake   (1 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_NXT_PAD1_USBB1_ULPITLL_DAT0 << reg_offset_gp) | (1<<pad_indicator_bp))
#define af18_mcbsp4_clkx   (2 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_NXT_PAD1_USBB1_ULPITLL_DAT0 << reg_offset_gp) | (1<<pad_indicator_bp))
#define af18_gpio_88   (3 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_NXT_PAD1_USBB1_ULPITLL_DAT0 << reg_offset_gp) | (1<<pad_indicator_bp))
#define af18_usbb1_ulpiphy_dat0   (4 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_NXT_PAD1_USBB1_ULPITLL_DAT0 << reg_offset_gp) | (1<<pad_indicator_bp))
#define af18_usbb1_mm_txen   (5 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_NXT_PAD1_USBB1_ULPITLL_DAT0 << reg_offset_gp) | (1<<pad_indicator_bp))
#define af18_attila_hw_dbg24   (6 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_NXT_PAD1_USBB1_ULPITLL_DAT0 << reg_offset_gp) | (1<<pad_indicator_bp))
#define af18_safe_mode   (7 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_NXT_PAD1_USBB1_ULPITLL_DAT0 << reg_offset_gp) | (1<<pad_indicator_bp))

#define ag18_usbb1_ulpitll_dat1   (0 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT1_PAD1_USBB1_ULPITLL_DAT2 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag18_hsi1_acdata   (1 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT1_PAD1_USBB1_ULPITLL_DAT2 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag18_mcbsp4_dx   (2 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT1_PAD1_USBB1_ULPITLL_DAT2 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag18_gpio_89   (3 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT1_PAD1_USBB1_ULPITLL_DAT2 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag18_usbb1_ulpiphy_dat1   (4 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT1_PAD1_USBB1_ULPITLL_DAT2 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag18_usbb1_mm_txdat   (5 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT1_PAD1_USBB1_ULPITLL_DAT2 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag18_attila_hw_dbg25   (6 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT1_PAD1_USBB1_ULPITLL_DAT2 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag18_safe_mode   (7 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT1_PAD1_USBB1_ULPITLL_DAT2 << reg_offset_gp) | (0<<pad_indicator_bp))

#define ae17_usbb1_ulpitll_dat2   (0 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT1_PAD1_USBB1_ULPITLL_DAT2 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ae17_hsi1_acflag   (1 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT1_PAD1_USBB1_ULPITLL_DAT2 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ae17_mcbsp4_dr   (2 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT1_PAD1_USBB1_ULPITLL_DAT2 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ae17_gpio_90   (3 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT1_PAD1_USBB1_ULPITLL_DAT2 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ae17_usbb1_ulpiphy_dat2   (4 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT1_PAD1_USBB1_ULPITLL_DAT2 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ae17_usbb1_mm_txse0   (5 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT1_PAD1_USBB1_ULPITLL_DAT2 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ae17_attila_hw_dbg26   (6 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT1_PAD1_USBB1_ULPITLL_DAT2 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ae17_safe_mode   (7 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT1_PAD1_USBB1_ULPITLL_DAT2 << reg_offset_gp) | (1<<pad_indicator_bp))

#define af17_usbb1_ulpitll_dat3   (0 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT3_PAD1_USBB1_ULPITLL_DAT4 << reg_offset_gp) | (0<<pad_indicator_bp))
#define af17_hsi1_caready   (1 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT3_PAD1_USBB1_ULPITLL_DAT4 << reg_offset_gp) | (0<<pad_indicator_bp))
#define af17_gpio_91   (3 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT3_PAD1_USBB1_ULPITLL_DAT4 << reg_offset_gp) | (0<<pad_indicator_bp))
#define af17_usbb1_ulpiphy_dat3   (4 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT3_PAD1_USBB1_ULPITLL_DAT4 << reg_offset_gp) | (0<<pad_indicator_bp))
#define af17_usbb1_mm_rxrcv   (5 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT3_PAD1_USBB1_ULPITLL_DAT4 << reg_offset_gp) | (0<<pad_indicator_bp))
#define af17_attila_hw_dbg27   (6 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT3_PAD1_USBB1_ULPITLL_DAT4 << reg_offset_gp) | (0<<pad_indicator_bp))
#define af17_safe_mode   (7 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT3_PAD1_USBB1_ULPITLL_DAT4 << reg_offset_gp) | (0<<pad_indicator_bp))

#define ah17_usbb1_ulpitll_dat4   (0 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT3_PAD1_USBB1_ULPITLL_DAT4 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ah17_dmtimer8_pwm_evt   (1 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT3_PAD1_USBB1_ULPITLL_DAT4 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ah17_abe_mcbsp3_dr   (2 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT3_PAD1_USBB1_ULPITLL_DAT4 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ah17_gpio_92   (3 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT3_PAD1_USBB1_ULPITLL_DAT4 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ah17_usbb1_ulpiphy_dat4   (4 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT3_PAD1_USBB1_ULPITLL_DAT4 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ah17_attila_hw_dbg28   (6 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT3_PAD1_USBB1_ULPITLL_DAT4 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ah17_safe_mode   (7 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT3_PAD1_USBB1_ULPITLL_DAT4 << reg_offset_gp) | (1<<pad_indicator_bp))

#define ae16_usbb1_ulpitll_dat5   (0 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT5_PAD1_USBB1_ULPITLL_DAT6 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae16_dmtimer9_pwm_evt   (1 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT5_PAD1_USBB1_ULPITLL_DAT6 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae16_abe_mcbsp3_dx   (2 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT5_PAD1_USBB1_ULPITLL_DAT6 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae16_gpio_93   (3 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT5_PAD1_USBB1_ULPITLL_DAT6 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae16_usbb1_ulpiphy_dat5   (4 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT5_PAD1_USBB1_ULPITLL_DAT6 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae16_attila_hw_dbg29   (6 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT5_PAD1_USBB1_ULPITLL_DAT6 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae16_safe_mode   (7 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT5_PAD1_USBB1_ULPITLL_DAT6 << reg_offset_gp) | (0<<pad_indicator_bp))

#define af16_usbb1_ulpitll_dat6   (0 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT5_PAD1_USBB1_ULPITLL_DAT6 << reg_offset_gp) | (1<<pad_indicator_bp))
#define af16_dmtimer10_pwm_evt   (1 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT5_PAD1_USBB1_ULPITLL_DAT6 << reg_offset_gp) | (1<<pad_indicator_bp))
#define af16_abe_mcbsp3_clkx   (2 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT5_PAD1_USBB1_ULPITLL_DAT6 << reg_offset_gp) | (1<<pad_indicator_bp))
#define af16_gpio_94   (3 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT5_PAD1_USBB1_ULPITLL_DAT6 << reg_offset_gp) | (1<<pad_indicator_bp))
#define af16_usbb1_ulpiphy_dat6   (4 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT5_PAD1_USBB1_ULPITLL_DAT6 << reg_offset_gp) | (1<<pad_indicator_bp))
#define af16_attila_hw_dbg30   (6 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT5_PAD1_USBB1_ULPITLL_DAT6 << reg_offset_gp) | (1<<pad_indicator_bp))
#define af16_safe_mode   (7 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT5_PAD1_USBB1_ULPITLL_DAT6 << reg_offset_gp) | (1<<pad_indicator_bp))

#define ag16_usbb1_ulpitll_dat7   (0 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT7_PAD1_USBB1_HSIC_DATA << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag16_dmtimer11_pwm_evt   (1 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT7_PAD1_USBB1_HSIC_DATA << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag16_abe_mcbsp3_fsx   (2 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT7_PAD1_USBB1_HSIC_DATA << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag16_gpio_95   (3 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT7_PAD1_USBB1_HSIC_DATA << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag16_usbb1_ulpiphy_dat7   (4 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT7_PAD1_USBB1_HSIC_DATA << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag16_attila_hw_dbg31   (6 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT7_PAD1_USBB1_HSIC_DATA << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag16_safe_mode   (7 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT7_PAD1_USBB1_HSIC_DATA << reg_offset_gp) | (0<<pad_indicator_bp))

#define af14_usbb1_hsic_data   (0 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT7_PAD1_USBB1_HSIC_DATA << reg_offset_gp) | (1<<pad_indicator_bp))
#define af14_gpio_96   (3 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT7_PAD1_USBB1_HSIC_DATA << reg_offset_gp) | (1<<pad_indicator_bp))
#define af14_safe_mode   (7 | (CONTROL_CORE_PAD0_USBB1_ULPITLL_DAT7_PAD1_USBB1_HSIC_DATA << reg_offset_gp) | (1<<pad_indicator_bp))

#define ae14_usbb1_hsic_strobe   (0 | (CONTROL_CORE_PAD0_USBB1_HSIC_STROBE_PAD1_USBC1_ICUSB_DP << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae14_gpio_97   (3 | (CONTROL_CORE_PAD0_USBB1_HSIC_STROBE_PAD1_USBC1_ICUSB_DP << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae14_safe_mode   (7 | (CONTROL_CORE_PAD0_USBB1_HSIC_STROBE_PAD1_USBC1_ICUSB_DP << reg_offset_gp) | (0<<pad_indicator_bp))

#define h4_sim_io   (0 | (CONTROL_WKUP_PAD0_GPIO_WK0_PAD1_GPIO_WK1 << reg_offset_gp) | (0<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define h4_gpio_wk0   (3 | (CONTROL_WKUP_PAD0_GPIO_WK0_PAD1_GPIO_WK1 << reg_offset_gp) | (0<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define h4_attila_hw_dbg1   (6 | (CONTROL_WKUP_PAD0_GPIO_WK0_PAD1_GPIO_WK1 << reg_offset_gp) | (0<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define h4_safe_mode   (7 | (CONTROL_WKUP_PAD0_GPIO_WK0_PAD1_GPIO_WK1 << reg_offset_gp) | (0<<pad_indicator_bp) | pad_padconf_wkup_bm)

#define j2_sim_clk   (0 | (CONTROL_WKUP_PAD0_GPIO_WK0_PAD1_GPIO_WK1 << reg_offset_gp) | (1<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define j2_gpio_wk1   (3 | (CONTROL_WKUP_PAD0_GPIO_WK0_PAD1_GPIO_WK1 << reg_offset_gp) | (1<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define j2_attila_hw_dbg2   (6 | (CONTROL_WKUP_PAD0_GPIO_WK0_PAD1_GPIO_WK1 << reg_offset_gp) | (1<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define j2_safe_mode   (7 | (CONTROL_WKUP_PAD0_GPIO_WK0_PAD1_GPIO_WK1 << reg_offset_gp) | (1<<pad_indicator_bp) | pad_padconf_wkup_bm)

#define g2_sim_reset   (0 | (CONTROL_WKUP_PAD0_GPIO_WK2_PAD1_GPIO_WK3 << reg_offset_gp) | (0<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define g2_gpio_wk2   (3 | (CONTROL_WKUP_PAD0_GPIO_WK2_PAD1_GPIO_WK3 << reg_offset_gp) | (0<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define g2_attila_hw_dbg3   (6 | (CONTROL_WKUP_PAD0_GPIO_WK2_PAD1_GPIO_WK3 << reg_offset_gp) | (0<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define g2_safe_mode   (7 | (CONTROL_WKUP_PAD0_GPIO_WK2_PAD1_GPIO_WK3 << reg_offset_gp) | (0<<pad_indicator_bp) | pad_padconf_wkup_bm)

#define j1_sim_cd   (0 | (CONTROL_WKUP_PAD0_GPIO_WK2_PAD1_GPIO_WK3 << reg_offset_gp) | (1<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define j1_gpio_wk3   (3 | (CONTROL_WKUP_PAD0_GPIO_WK2_PAD1_GPIO_WK3 << reg_offset_gp) | (1<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define j1_attila_hw_dbg4   (6 | (CONTROL_WKUP_PAD0_GPIO_WK2_PAD1_GPIO_WK3 << reg_offset_gp) | (1<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define j1_safe_mode   (7 | (CONTROL_WKUP_PAD0_GPIO_WK2_PAD1_GPIO_WK3 << reg_offset_gp) | (1<<pad_indicator_bp) | pad_padconf_wkup_bm)

#define k1_sim_pwrctrl   (CONTROL_WKUP_PAD0_GPIO_WK4_PAD1_SR_SCL | (0 << reg_offset_gp) | (0<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define k1_gpio_wk4   (3 | (CONTROL_WKUP_PAD0_GPIO_WK4_PAD1_SR_SCL << reg_offset_gp) | (0<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define k1_attila_hw_dbg5   (6 | (CONTROL_WKUP_PAD0_GPIO_WK4_PAD1_SR_SCL << reg_offset_gp) | (0<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define k1_safe_mode   (7 | (CONTROL_WKUP_PAD0_GPIO_WK4_PAD1_SR_SCL << reg_offset_gp) | (0<<pad_indicator_bp) | pad_padconf_wkup_bm)

#define h2_usbc1_icusb_dp   (0 | (CONTROL_CORE_PAD0_USBB1_HSIC_STROBE_PAD1_USBC1_ICUSB_DP << reg_offset_gp) | (1<<pad_indicator_bp))
#define h2_gpio_98   (3 | (CONTROL_CORE_PAD0_USBB1_HSIC_STROBE_PAD1_USBC1_ICUSB_DP << reg_offset_gp) | (1<<pad_indicator_bp))
#define h2_safe_mode   (7 | (CONTROL_CORE_PAD0_USBB1_HSIC_STROBE_PAD1_USBC1_ICUSB_DP << reg_offset_gp) | (1<<pad_indicator_bp))

#define h3_usbc1_icusb_dm   (0 | (CONTROL_CORE_PAD0_USBC1_ICUSB_DM_PAD1_SDMMC1_CLK << reg_offset_gp) | (0<<pad_indicator_bp))
#define h3_gpio_99   (3 | (CONTROL_CORE_PAD0_USBC1_ICUSB_DM_PAD1_SDMMC1_CLK << reg_offset_gp) | (0<<pad_indicator_bp))
#define h3_safe_mode   (7 | (CONTROL_CORE_PAD0_USBC1_ICUSB_DM_PAD1_SDMMC1_CLK << reg_offset_gp) | (0<<pad_indicator_bp))

#define d2_sdmmc1_clk   (0 | (CONTROL_CORE_PAD0_USBC1_ICUSB_DM_PAD1_SDMMC1_CLK << reg_offset_gp) | (1<<pad_indicator_bp))
#define d2_dpm_emu19   (2 | (CONTROL_CORE_PAD0_USBC1_ICUSB_DM_PAD1_SDMMC1_CLK << reg_offset_gp) | (1<<pad_indicator_bp))
#define d2_gpio_100   (3 | (CONTROL_CORE_PAD0_USBC1_ICUSB_DM_PAD1_SDMMC1_CLK << reg_offset_gp) | (1<<pad_indicator_bp))
#define d2_safe_mode   (7 | (CONTROL_CORE_PAD0_USBC1_ICUSB_DM_PAD1_SDMMC1_CLK << reg_offset_gp) | (1<<pad_indicator_bp))

#define e3_sdmmc1_cmd   (0 | (CONTROL_CORE_PAD0_SDMMC1_CMD_PAD1_SDMMC1_DAT0 << reg_offset_gp) | (0<<pad_indicator_bp))
#define e3_uart1_rx   (2 | (CONTROL_CORE_PAD0_SDMMC1_CMD_PAD1_SDMMC1_DAT0 << reg_offset_gp) | (0<<pad_indicator_bp))
#define e3_gpio_101   (3 | (CONTROL_CORE_PAD0_SDMMC1_CMD_PAD1_SDMMC1_DAT0 << reg_offset_gp) | (0<<pad_indicator_bp))
#define e3_safe_mode   (7 | (CONTROL_CORE_PAD0_SDMMC1_CMD_PAD1_SDMMC1_DAT0 << reg_offset_gp) | (0<<pad_indicator_bp))

#define e4_sdmmc1_dat0   (0 | (CONTROL_CORE_PAD0_SDMMC1_CMD_PAD1_SDMMC1_DAT0 << reg_offset_gp) | (1<<pad_indicator_bp))
#define e4_dpm_emu18   (2 | (CONTROL_CORE_PAD0_SDMMC1_CMD_PAD1_SDMMC1_DAT0 << reg_offset_gp) | (1<<pad_indicator_bp))
#define e4_gpio_102   (3 | (CONTROL_CORE_PAD0_SDMMC1_CMD_PAD1_SDMMC1_DAT0 << reg_offset_gp) | (1<<pad_indicator_bp))
#define e4_safe_mode   (7 | (CONTROL_CORE_PAD0_SDMMC1_CMD_PAD1_SDMMC1_DAT0 << reg_offset_gp) | (1<<pad_indicator_bp))

#define e2_sdmmc1_dat1   (0 | (CONTROL_CORE_PAD0_SDMMC1_DAT1_PAD1_SDMMC1_DAT2 << reg_offset_gp) | (0<<pad_indicator_bp))
#define e2_dpm_emu17   (2 | (CONTROL_CORE_PAD0_SDMMC1_DAT1_PAD1_SDMMC1_DAT2 << reg_offset_gp) | (0<<pad_indicator_bp))
#define e2_gpio_103   (3 | (CONTROL_CORE_PAD0_SDMMC1_DAT1_PAD1_SDMMC1_DAT2 << reg_offset_gp) | (0<<pad_indicator_bp))
#define e2_safe_mode   (7 | (CONTROL_CORE_PAD0_SDMMC1_DAT1_PAD1_SDMMC1_DAT2 << reg_offset_gp) | (0<<pad_indicator_bp))

#define e1_sdmmc1_dat2   (0 | (CONTROL_CORE_PAD0_SDMMC1_DAT1_PAD1_SDMMC1_DAT2 << reg_offset_gp) | (1<<pad_indicator_bp))
#define e1_dpm_emu16   (2 | (CONTROL_CORE_PAD0_SDMMC1_DAT1_PAD1_SDMMC1_DAT2 << reg_offset_gp) | (1<<pad_indicator_bp))
#define e1_gpio_104   (3 | (CONTROL_CORE_PAD0_SDMMC1_DAT1_PAD1_SDMMC1_DAT2 << reg_offset_gp) | (1<<pad_indicator_bp))
#define e1_jtag_tms_tmsc   (4 | (CONTROL_CORE_PAD0_SDMMC1_DAT1_PAD1_SDMMC1_DAT2 << reg_offset_gp) | (1<<pad_indicator_bp))
#define e1_safe_mode   (7 | (CONTROL_CORE_PAD0_SDMMC1_DAT1_PAD1_SDMMC1_DAT2 << reg_offset_gp) | (1<<pad_indicator_bp))

#define f4_sdmmc1_dat3   (0 | (CONTROL_CORE_PAD0_SDMMC1_DAT3_PAD1_SDMMC1_DAT4 << reg_offset_gp) | (0<<pad_indicator_bp))
#define f4_dpm_emu15   (2 | (CONTROL_CORE_PAD0_SDMMC1_DAT3_PAD1_SDMMC1_DAT4 << reg_offset_gp) | (0<<pad_indicator_bp))
#define f4_gpio_105   (3 | (CONTROL_CORE_PAD0_SDMMC1_DAT3_PAD1_SDMMC1_DAT4 << reg_offset_gp) | (0<<pad_indicator_bp))
#define f4_jtag_tck   (4 | (CONTROL_CORE_PAD0_SDMMC1_DAT3_PAD1_SDMMC1_DAT4 << reg_offset_gp) | (0<<pad_indicator_bp))
#define f4_safe_mode   (7 | (CONTROL_CORE_PAD0_SDMMC1_DAT3_PAD1_SDMMC1_DAT4 << reg_offset_gp) | (0<<pad_indicator_bp))

#define f3_sdmmc1_dat4   (0 | (CONTROL_CORE_PAD0_SDMMC1_DAT3_PAD1_SDMMC1_DAT4 << reg_offset_gp) | (1<<pad_indicator_bp))
#define f3_gpio_106   (3 | (CONTROL_CORE_PAD0_SDMMC1_DAT3_PAD1_SDMMC1_DAT4 << reg_offset_gp) | (1<<pad_indicator_bp))
#define f3_safe_mode   (7 | (CONTROL_CORE_PAD0_SDMMC1_DAT3_PAD1_SDMMC1_DAT4 << reg_offset_gp) | (1<<pad_indicator_bp))

#define f1_sdmmc1_dat5   (0 | (CONTROL_CORE_PAD0_SDMMC1_DAT5_PAD1_SDMMC1_DAT6 << reg_offset_gp) | (0<<pad_indicator_bp))
#define f1_gpio_107   (3 | (CONTROL_CORE_PAD0_SDMMC1_DAT5_PAD1_SDMMC1_DAT6 << reg_offset_gp) | (0<<pad_indicator_bp))
#define f1_safe_mode   (7 | (CONTROL_CORE_PAD0_SDMMC1_DAT5_PAD1_SDMMC1_DAT6 << reg_offset_gp) | (0<<pad_indicator_bp))

#define g4_sdmmc1_dat6   (0 | (CONTROL_CORE_PAD0_SDMMC1_DAT5_PAD1_SDMMC1_DAT6 << reg_offset_gp) | (1<<pad_indicator_bp))
#define g4_gpio_108   (3 | (CONTROL_CORE_PAD0_SDMMC1_DAT5_PAD1_SDMMC1_DAT6 << reg_offset_gp) | (1<<pad_indicator_bp))
#define g4_safe_mode   (7 | (CONTROL_CORE_PAD0_SDMMC1_DAT5_PAD1_SDMMC1_DAT6 << reg_offset_gp) | (1<<pad_indicator_bp))

#define g3_sdmmc1_dat7   (0 | (CONTROL_CORE_PAD0_SDMMC1_DAT7_PAD1_ABE_MCBSP2_CLKX << reg_offset_gp) | (0<<pad_indicator_bp))
#define g3_gpio_109   (3 | (CONTROL_CORE_PAD0_SDMMC1_DAT7_PAD1_ABE_MCBSP2_CLKX << reg_offset_gp) | (0<<pad_indicator_bp))
#define g3_safe_mode   (7 | (CONTROL_CORE_PAD0_SDMMC1_DAT7_PAD1_ABE_MCBSP2_CLKX << reg_offset_gp) | (0<<pad_indicator_bp))

#define ad27_abe_mcbsp2_clkx   (0 | (CONTROL_CORE_PAD0_SDMMC1_DAT7_PAD1_ABE_MCBSP2_CLKX << reg_offset_gp) | (1<<pad_indicator_bp))
#define ad27_mcspi2_clk   (1 | (CONTROL_CORE_PAD0_SDMMC1_DAT7_PAD1_ABE_MCBSP2_CLKX << reg_offset_gp) | (1<<pad_indicator_bp))
#define ad27_abe_mcasp_ahclkx   (2 | (CONTROL_CORE_PAD0_SDMMC1_DAT7_PAD1_ABE_MCBSP2_CLKX << reg_offset_gp) | (1<<pad_indicator_bp))
#define ad27_gpio_110   (3 | (CONTROL_CORE_PAD0_SDMMC1_DAT7_PAD1_ABE_MCBSP2_CLKX << reg_offset_gp) | (1<<pad_indicator_bp))
#define ad27_usbb2_mm_rxdm   (4 | (CONTROL_CORE_PAD0_SDMMC1_DAT7_PAD1_ABE_MCBSP2_CLKX << reg_offset_gp) | (1<<pad_indicator_bp))
#define ad27_safe_mode   (7 | (CONTROL_CORE_PAD0_SDMMC1_DAT7_PAD1_ABE_MCBSP2_CLKX << reg_offset_gp) | (1<<pad_indicator_bp))

#define ad26_abe_mcbsp2_dr   (0 | (CONTROL_CORE_PAD0_ABE_MCBSP2_DR_PAD1_ABE_MCBSP2_DX << reg_offset_gp) | (0<<pad_indicator_bp))
#define ad26_mcspi2_somi   (1 | (CONTROL_CORE_PAD0_ABE_MCBSP2_DR_PAD1_ABE_MCBSP2_DX << reg_offset_gp) | (0<<pad_indicator_bp))
#define ad26_abe_mcasp_axr   (2 | (CONTROL_CORE_PAD0_ABE_MCBSP2_DR_PAD1_ABE_MCBSP2_DX << reg_offset_gp) | (0<<pad_indicator_bp))
#define ad26_gpio_111   (3 | (CONTROL_CORE_PAD0_ABE_MCBSP2_DR_PAD1_ABE_MCBSP2_DX << reg_offset_gp) | (0<<pad_indicator_bp))
#define ad26_usbb2_mm_rxdp   (4 | (CONTROL_CORE_PAD0_ABE_MCBSP2_DR_PAD1_ABE_MCBSP2_DX << reg_offset_gp) | (0<<pad_indicator_bp))
#define ad26_safe_mode   (7 | (CONTROL_CORE_PAD0_ABE_MCBSP2_DR_PAD1_ABE_MCBSP2_DX << reg_offset_gp) | (0<<pad_indicator_bp))

#define ad25_abe_mcbsp2_dx   (0 | (CONTROL_CORE_PAD0_ABE_MCBSP2_DR_PAD1_ABE_MCBSP2_DX << reg_offset_gp) | (1<<pad_indicator_bp))
#define ad25_mcspi2_simo   (1 | (CONTROL_CORE_PAD0_ABE_MCBSP2_DR_PAD1_ABE_MCBSP2_DX << reg_offset_gp) | (1<<pad_indicator_bp))
#define ad25_abe_mcasp_amute   (2 | (CONTROL_CORE_PAD0_ABE_MCBSP2_DR_PAD1_ABE_MCBSP2_DX << reg_offset_gp) | (1<<pad_indicator_bp))
#define ad25_gpio_112   (3 | (CONTROL_CORE_PAD0_ABE_MCBSP2_DR_PAD1_ABE_MCBSP2_DX << reg_offset_gp) | (1<<pad_indicator_bp))
#define ad25_usbb2_mm_rxrcv   (4 | (CONTROL_CORE_PAD0_ABE_MCBSP2_DR_PAD1_ABE_MCBSP2_DX << reg_offset_gp) | (1<<pad_indicator_bp))
#define ad25_safe_mode   (7 | (CONTROL_CORE_PAD0_ABE_MCBSP2_DR_PAD1_ABE_MCBSP2_DX << reg_offset_gp) | (1<<pad_indicator_bp))

#define ac28_abe_mcbsp2_fsx   (0 | (CONTROL_CORE_PAD0_ABE_MCBSP2_FSX_PAD1_ABE_MCBSP1_CLKX << reg_offset_gp) | (0<<pad_indicator_bp))
#define ac28_mcspi2_cs0   (1 | (CONTROL_CORE_PAD0_ABE_MCBSP2_FSX_PAD1_ABE_MCBSP1_CLKX << reg_offset_gp) | (0<<pad_indicator_bp))
#define ac28_abe_mcasp_afsx   (2 | (CONTROL_CORE_PAD0_ABE_MCBSP2_FSX_PAD1_ABE_MCBSP1_CLKX << reg_offset_gp) | (0<<pad_indicator_bp))
#define ac28_gpio_113   (3 | (CONTROL_CORE_PAD0_ABE_MCBSP2_FSX_PAD1_ABE_MCBSP1_CLKX << reg_offset_gp) | (0<<pad_indicator_bp))
#define ac28_usbb2_mm_txen   (4 | (CONTROL_CORE_PAD0_ABE_MCBSP2_FSX_PAD1_ABE_MCBSP1_CLKX << reg_offset_gp) | (0<<pad_indicator_bp))
#define ac28_safe_mode   (7 | (CONTROL_CORE_PAD0_ABE_MCBSP2_FSX_PAD1_ABE_MCBSP1_CLKX << reg_offset_gp) | (0<<pad_indicator_bp))

#define ac26_abe_mcbsp1_clkx   (0 | (CONTROL_CORE_PAD0_ABE_MCBSP2_FSX_PAD1_ABE_MCBSP1_CLKX << reg_offset_gp) | (1<<pad_indicator_bp))
#define ac26_abe_slimbus1_clock   (1 | (CONTROL_CORE_PAD0_ABE_MCBSP2_FSX_PAD1_ABE_MCBSP1_CLKX << reg_offset_gp) | (1<<pad_indicator_bp))
#define ac26_gpio_114   (3 | (CONTROL_CORE_PAD0_ABE_MCBSP2_FSX_PAD1_ABE_MCBSP1_CLKX << reg_offset_gp) | (1<<pad_indicator_bp))
#define ac26_safe_mode   (7 | (CONTROL_CORE_PAD0_ABE_MCBSP2_FSX_PAD1_ABE_MCBSP1_CLKX << reg_offset_gp) | (1<<pad_indicator_bp))

#define ac25_abe_mcbsp1_dr   (0 | (CONTROL_CORE_PAD0_ABE_MCBSP1_DR_PAD1_ABE_MCBSP1_DX << reg_offset_gp) | (0<<pad_indicator_bp))
#define ac25_abe_slimbus1_data   (1 | (CONTROL_CORE_PAD0_ABE_MCBSP1_DR_PAD1_ABE_MCBSP1_DX << reg_offset_gp) | (0<<pad_indicator_bp))
#define ac25_gpio_115   (3 | (CONTROL_CORE_PAD0_ABE_MCBSP1_DR_PAD1_ABE_MCBSP1_DX << reg_offset_gp) | (0<<pad_indicator_bp))
#define ac25_safe_mode   (7 | (CONTROL_CORE_PAD0_ABE_MCBSP1_DR_PAD1_ABE_MCBSP1_DX << reg_offset_gp) | (0<<pad_indicator_bp))

#define ab25_abe_mcbsp1_dx   (0 | (CONTROL_CORE_PAD0_ABE_MCBSP1_DR_PAD1_ABE_MCBSP1_DX << reg_offset_gp) | (1<<pad_indicator_bp))
#define ab25_sdmmc3_dat2   (1 | (CONTROL_CORE_PAD0_ABE_MCBSP1_DR_PAD1_ABE_MCBSP1_DX << reg_offset_gp) | (1<<pad_indicator_bp))
#define ab25_abe_mcasp_aclkx   (2 | (CONTROL_CORE_PAD0_ABE_MCBSP1_DR_PAD1_ABE_MCBSP1_DX << reg_offset_gp) | (1<<pad_indicator_bp))
#define ab25_gpio_116   (3 | (CONTROL_CORE_PAD0_ABE_MCBSP1_DR_PAD1_ABE_MCBSP1_DX << reg_offset_gp) | (1<<pad_indicator_bp))
#define ab25_safe_mode   (7 | (CONTROL_CORE_PAD0_ABE_MCBSP1_DR_PAD1_ABE_MCBSP1_DX << reg_offset_gp) | (1<<pad_indicator_bp))

#define ac27_abe_mcbsp1_fsx   (0 | (CONTROL_CORE_PAD0_ABE_MCBSP1_FSX_PAD1_ABE_PDM_UL_DATA << reg_offset_gp) | (0<<pad_indicator_bp))
#define ac27_sdmmc3_dat3   (1 | (CONTROL_CORE_PAD0_ABE_MCBSP1_FSX_PAD1_ABE_PDM_UL_DATA << reg_offset_gp) | (0<<pad_indicator_bp))
#define ac27_abe_mcasp_amutein   (2 | (CONTROL_CORE_PAD0_ABE_MCBSP1_FSX_PAD1_ABE_PDM_UL_DATA << reg_offset_gp) | (0<<pad_indicator_bp))
#define ac27_gpio_117   (3 | (CONTROL_CORE_PAD0_ABE_MCBSP1_FSX_PAD1_ABE_PDM_UL_DATA << reg_offset_gp) | (0<<pad_indicator_bp))
#define ac27_safe_mode   (7 | (CONTROL_CORE_PAD0_ABE_MCBSP1_FSX_PAD1_ABE_PDM_UL_DATA << reg_offset_gp) | (0<<pad_indicator_bp))

#define ag25_abe_pdm_ul_data   (0 | (CONTROL_CORE_PAD0_ABE_MCBSP1_FSX_PAD1_ABE_PDM_UL_DATA << reg_offset_gp) | (1<<pad_indicator_bp))
#define ag25_abe_mcbsp3_dr   (1 | (CONTROL_CORE_PAD0_ABE_MCBSP1_FSX_PAD1_ABE_PDM_UL_DATA << reg_offset_gp) | (1<<pad_indicator_bp))
#define ag25_safe_mode   (7 | (CONTROL_CORE_PAD0_ABE_MCBSP1_FSX_PAD1_ABE_PDM_UL_DATA << reg_offset_gp) | (1<<pad_indicator_bp))

#define af25_abe_pdm_dl_data   (0 | (CONTROL_CORE_PAD0_ABE_PDM_DL_DATA_PAD1_ABE_PDM_FRAME << reg_offset_gp) | (0<<pad_indicator_bp))
#define af25_abe_mcbsp3_dx   (1 | (CONTROL_CORE_PAD0_ABE_PDM_DL_DATA_PAD1_ABE_PDM_FRAME << reg_offset_gp) | (0<<pad_indicator_bp))
#define af25_safe_mode   (7 | (CONTROL_CORE_PAD0_ABE_PDM_DL_DATA_PAD1_ABE_PDM_FRAME << reg_offset_gp) | (0<<pad_indicator_bp))

#define ae25_abe_pdm_frame   (0 | (CONTROL_CORE_PAD0_ABE_PDM_DL_DATA_PAD1_ABE_PDM_FRAME << reg_offset_gp) | (1<<pad_indicator_bp))
#define ae25_abe_mcbsp3_clkx   (1 | (CONTROL_CORE_PAD0_ABE_PDM_DL_DATA_PAD1_ABE_PDM_FRAME << reg_offset_gp) | (1<<pad_indicator_bp))
#define ae25_safe_mode   (7 | (CONTROL_CORE_PAD0_ABE_PDM_DL_DATA_PAD1_ABE_PDM_FRAME << reg_offset_gp) | (1<<pad_indicator_bp))

#define af26_abe_pdm_lb_clk   (0 | (CONTROL_CORE_PAD0_ABE_PDM_LB_CLK_PAD1_ABE_CLKS << reg_offset_gp) | (0<<pad_indicator_bp))
#define af26_abe_mcbsp3_fsx   (1 | (CONTROL_CORE_PAD0_ABE_PDM_LB_CLK_PAD1_ABE_CLKS << reg_offset_gp) | (0<<pad_indicator_bp))
#define af26_safe_mode   (7 | (CONTROL_CORE_PAD0_ABE_PDM_LB_CLK_PAD1_ABE_CLKS << reg_offset_gp) | (0<<pad_indicator_bp))

#define ah26_abe_clks   (0 | (CONTROL_CORE_PAD0_ABE_PDM_LB_CLK_PAD1_ABE_CLKS << reg_offset_gp) | (1<<pad_indicator_bp))
#define ah26_gpio_118   (3 | (CONTROL_CORE_PAD0_ABE_PDM_LB_CLK_PAD1_ABE_CLKS << reg_offset_gp) | (1<<pad_indicator_bp))
#define ah26_safe_mode   (7 | (CONTROL_CORE_PAD0_ABE_PDM_LB_CLK_PAD1_ABE_CLKS << reg_offset_gp) | (1<<pad_indicator_bp))

#define ae24_abe_dmic_clk1   (0 | (CONTROL_CORE_PAD0_ABE_DMIC_CLK1_PAD1_ABE_DMIC_DIN1 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae24_gpio_119   (3 | (CONTROL_CORE_PAD0_ABE_DMIC_CLK1_PAD1_ABE_DMIC_DIN1 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae24_usbb2_mm_txse0   (4 | (CONTROL_CORE_PAD0_ABE_DMIC_CLK1_PAD1_ABE_DMIC_DIN1 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae24_uart4_cts   (5 | (CONTROL_CORE_PAD0_ABE_DMIC_CLK1_PAD1_ABE_DMIC_DIN1 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae24_safe_mode   (7 | (CONTROL_CORE_PAD0_ABE_DMIC_CLK1_PAD1_ABE_DMIC_DIN1 << reg_offset_gp) | (0<<pad_indicator_bp))

#define af24_abe_dmic_din1   (0 | (CONTROL_CORE_PAD0_ABE_DMIC_CLK1_PAD1_ABE_DMIC_DIN1 << reg_offset_gp) | (1<<pad_indicator_bp))
#define af24_gpio_120   (3 | (CONTROL_CORE_PAD0_ABE_DMIC_CLK1_PAD1_ABE_DMIC_DIN1 << reg_offset_gp) | (1<<pad_indicator_bp))
#define af24_usbb2_mm_txdat   (4 | (CONTROL_CORE_PAD0_ABE_DMIC_CLK1_PAD1_ABE_DMIC_DIN1 << reg_offset_gp) | (1<<pad_indicator_bp))
#define af24_uart4_rts   (5 | (CONTROL_CORE_PAD0_ABE_DMIC_CLK1_PAD1_ABE_DMIC_DIN1 << reg_offset_gp) | (1<<pad_indicator_bp))
#define af24_safe_mode   (7 | (CONTROL_CORE_PAD0_ABE_DMIC_CLK1_PAD1_ABE_DMIC_DIN1 << reg_offset_gp) | (1<<pad_indicator_bp))

#define ag24_abe_dmic_din2   (0 | (CONTROL_CORE_PAD0_ABE_DMIC_DIN2_PAD1_ABE_DMIC_DIN3 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag24_slimbus2_clock   (1 | (CONTROL_CORE_PAD0_ABE_DMIC_DIN2_PAD1_ABE_DMIC_DIN3 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag24_abe_mcasp_axr   (2 | (CONTROL_CORE_PAD0_ABE_DMIC_DIN2_PAD1_ABE_DMIC_DIN3 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag24_gpio_121   (3 | (CONTROL_CORE_PAD0_ABE_DMIC_DIN2_PAD1_ABE_DMIC_DIN3 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag24_dmtimer11_pwm_evt   (5 | (CONTROL_CORE_PAD0_ABE_DMIC_DIN2_PAD1_ABE_DMIC_DIN3 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag24_safe_mode   (7 | (CONTROL_CORE_PAD0_ABE_DMIC_DIN2_PAD1_ABE_DMIC_DIN3 << reg_offset_gp) | (0<<pad_indicator_bp))

#define ah24_abe_dmic_din3   (0 | (CONTROL_CORE_PAD0_ABE_DMIC_DIN2_PAD1_ABE_DMIC_DIN3 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ah24_slimbus2_data   (1 | (CONTROL_CORE_PAD0_ABE_DMIC_DIN2_PAD1_ABE_DMIC_DIN3 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ah24_abe_dmic_clk2   (2 | (CONTROL_CORE_PAD0_ABE_DMIC_DIN2_PAD1_ABE_DMIC_DIN3 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ah24_gpio_122   (3 | (CONTROL_CORE_PAD0_ABE_DMIC_DIN2_PAD1_ABE_DMIC_DIN3 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ah24_dmtimer9_pwm_evt   (5 | (CONTROL_CORE_PAD0_ABE_DMIC_DIN2_PAD1_ABE_DMIC_DIN3 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ah24_safe_mode   (7 | (CONTROL_CORE_PAD0_ABE_DMIC_DIN2_PAD1_ABE_DMIC_DIN3 << reg_offset_gp) | (1<<pad_indicator_bp))

#define ab26_uart2_cts   (0 | (CONTROL_CORE_PAD0_UART2_CTS_PAD1_UART2_RTS << reg_offset_gp) | (0<<pad_indicator_bp))
#define ab26_sdmmc3_clk   (1 | (CONTROL_CORE_PAD0_UART2_CTS_PAD1_UART2_RTS << reg_offset_gp) | (0<<pad_indicator_bp))
#define ab26_gpio_123   (3 | (CONTROL_CORE_PAD0_UART2_CTS_PAD1_UART2_RTS << reg_offset_gp) | (0<<pad_indicator_bp))
#define ab26_safe_mode   (7 | (CONTROL_CORE_PAD0_UART2_CTS_PAD1_UART2_RTS << reg_offset_gp) | (0<<pad_indicator_bp))

#define ab27_uart2_rts   (0 | (CONTROL_CORE_PAD0_UART2_CTS_PAD1_UART2_RTS << reg_offset_gp) | (1<<pad_indicator_bp))
#define ab27_sdmmc3_cmd   (1 | (CONTROL_CORE_PAD0_UART2_CTS_PAD1_UART2_RTS << reg_offset_gp) | (1<<pad_indicator_bp))
#define ab27_gpio_124   (3 | (CONTROL_CORE_PAD0_UART2_CTS_PAD1_UART2_RTS << reg_offset_gp) | (1<<pad_indicator_bp))
#define ab27_safe_mode   (7 | (CONTROL_CORE_PAD0_UART2_CTS_PAD1_UART2_RTS << reg_offset_gp) | (1<<pad_indicator_bp))

#define aa25_uart2_rx   (0 | (CONTROL_CORE_PAD0_UART2_RX_PAD1_UART2_TX << reg_offset_gp) | (0<<pad_indicator_bp))
#define aa25_sdmmc3_dat0   (1 | (CONTROL_CORE_PAD0_UART2_RX_PAD1_UART2_TX << reg_offset_gp) | (0<<pad_indicator_bp))
#define aa25_gpio_125   (3 | (CONTROL_CORE_PAD0_UART2_RX_PAD1_UART2_TX << reg_offset_gp) | (0<<pad_indicator_bp))
#define aa25_safe_mode   (7 | (CONTROL_CORE_PAD0_UART2_RX_PAD1_UART2_TX << reg_offset_gp) | (0<<pad_indicator_bp))

#define aa26_uart2_tx   (0 | (CONTROL_CORE_PAD0_UART2_RX_PAD1_UART2_TX << reg_offset_gp) | (1<<pad_indicator_bp))
#define aa26_sdmmc3_dat1   (1 | (CONTROL_CORE_PAD0_UART2_RX_PAD1_UART2_TX << reg_offset_gp) | (1<<pad_indicator_bp))
#define aa26_gpio_126   (3 | (CONTROL_CORE_PAD0_UART2_RX_PAD1_UART2_TX << reg_offset_gp) | (1<<pad_indicator_bp))
#define aa26_safe_mode   (7 | (CONTROL_CORE_PAD0_UART2_RX_PAD1_UART2_TX << reg_offset_gp) | (1<<pad_indicator_bp))

#define aa27_hdq_sio   (0 | (CONTROL_CORE_PAD0_HDQ_SIO_PAD1_I2C1_SCL << reg_offset_gp) | (0<<pad_indicator_bp))
#define aa27_i2c3_sccb   (1 | (CONTROL_CORE_PAD0_HDQ_SIO_PAD1_I2C1_SCL << reg_offset_gp) | (0<<pad_indicator_bp))
#define aa27_i2c2_sccb   (2 | (CONTROL_CORE_PAD0_HDQ_SIO_PAD1_I2C1_SCL << reg_offset_gp) | (0<<pad_indicator_bp))
#define aa27_gpio_127   (3 | (CONTROL_CORE_PAD0_HDQ_SIO_PAD1_I2C1_SCL << reg_offset_gp) | (0<<pad_indicator_bp))
#define aa27_safe_mode   (7 | (CONTROL_CORE_PAD0_HDQ_SIO_PAD1_I2C1_SCL << reg_offset_gp) | (0<<pad_indicator_bp))

#define ae28_i2c1_scl   (0 | (CONTROL_CORE_PAD0_HDQ_SIO_PAD1_I2C1_SCL << reg_offset_gp) | (1<<pad_indicator_bp))

#define ae26_i2c1_sda   (0 | (CONTROL_CORE_PAD0_I2C1_SDA_PAD1_I2C2_SCL << reg_offset_gp) | (0<<pad_indicator_bp))

#define c26_i2c2_scl   (0 | (CONTROL_CORE_PAD0_I2C1_SDA_PAD1_I2C2_SCL << reg_offset_gp) | (1<<pad_indicator_bp))
#define c26_uart1_rx   (1 | (CONTROL_CORE_PAD0_I2C1_SDA_PAD1_I2C2_SCL << reg_offset_gp) | (1<<pad_indicator_bp))
#define c26_gpio_128   (3 | (CONTROL_CORE_PAD0_I2C1_SDA_PAD1_I2C2_SCL << reg_offset_gp) | (1<<pad_indicator_bp))
#define c26_safe_mode   (7 | (CONTROL_CORE_PAD0_I2C1_SDA_PAD1_I2C2_SCL << reg_offset_gp) | (1<<pad_indicator_bp))

#define d26_i2c2_sda   (0 | (CONTROL_CORE_PAD0_I2C2_SDA_PAD1_I2C3_SCL << reg_offset_gp) | (0<<pad_indicator_bp))
#define d26_uart1_tx   (1 | (CONTROL_CORE_PAD0_I2C2_SDA_PAD1_I2C3_SCL << reg_offset_gp) | (0<<pad_indicator_bp))
#define d26_gpio_129   (3 | (CONTROL_CORE_PAD0_I2C2_SDA_PAD1_I2C3_SCL << reg_offset_gp) | (0<<pad_indicator_bp))
#define d26_safe_mode   (7 | (CONTROL_CORE_PAD0_I2C2_SDA_PAD1_I2C3_SCL << reg_offset_gp) | (0<<pad_indicator_bp))

#define w27_i2c3_scl   (0 | (CONTROL_CORE_PAD0_I2C2_SDA_PAD1_I2C3_SCL << reg_offset_gp) | (1<<pad_indicator_bp))
#define w27_gpio_130   (3 | (CONTROL_CORE_PAD0_I2C2_SDA_PAD1_I2C3_SCL << reg_offset_gp) | (1<<pad_indicator_bp))
#define w27_safe_mode   (7 | (CONTROL_CORE_PAD0_I2C2_SDA_PAD1_I2C3_SCL << reg_offset_gp) | (1<<pad_indicator_bp))

#define y27_i2c3_sda   (0 | (CONTROL_CORE_PAD0_I2C3_SDA_PAD1_I2C4_SCL << reg_offset_gp) | (0<<pad_indicator_bp))
#define y27_gpio_131   (3 | (CONTROL_CORE_PAD0_I2C3_SDA_PAD1_I2C4_SCL << reg_offset_gp) | (0<<pad_indicator_bp))
#define y27_safe_mode   (7 | (CONTROL_CORE_PAD0_I2C3_SDA_PAD1_I2C4_SCL << reg_offset_gp) | (0<<pad_indicator_bp))

#define ag21_i2c4_scl   (0 | (CONTROL_CORE_PAD0_I2C3_SDA_PAD1_I2C4_SCL << reg_offset_gp) | (1<<pad_indicator_bp))
#define ag21_gpio_132   (3 | (CONTROL_CORE_PAD0_I2C3_SDA_PAD1_I2C4_SCL << reg_offset_gp) | (1<<pad_indicator_bp))
#define ag21_safe_mode   (7 | (CONTROL_CORE_PAD0_I2C3_SDA_PAD1_I2C4_SCL << reg_offset_gp) | (1<<pad_indicator_bp))

#define ah22_i2c4_sda   (0 | (CONTROL_CORE_PAD0_I2C4_SDA_PAD1_MCSPI1_CLK << reg_offset_gp) | (0<<pad_indicator_bp))
#define ah22_gpio_133   (3 | (CONTROL_CORE_PAD0_I2C4_SDA_PAD1_MCSPI1_CLK << reg_offset_gp) | (0<<pad_indicator_bp))
#define ah22_safe_mode   (7 | (CONTROL_CORE_PAD0_I2C4_SDA_PAD1_MCSPI1_CLK << reg_offset_gp) | (0<<pad_indicator_bp))

#define ag9_sr_scl   (0 | (CONTROL_WKUP_PAD0_GPIO_WK4_PAD1_SR_SCL << reg_offset_gp) | (1<<pad_indicator_bp) | pad_padconf_wkup_bm)

#define af9_sr_sda   (0 | (CONTROL_WKUP_PAD0_SR_SDA_PAD1_FREF_XTAL_IN << reg_offset_gp) | (0<<pad_indicator_bp) | pad_padconf_wkup_bm)

#define af22_mcspi1_clk   (0 | (CONTROL_CORE_PAD0_I2C4_SDA_PAD1_MCSPI1_CLK << reg_offset_gp) | (1<<pad_indicator_bp))
#define af22_gpio_134   (3 | (CONTROL_CORE_PAD0_I2C4_SDA_PAD1_MCSPI1_CLK << reg_offset_gp) | (1<<pad_indicator_bp))
#define af22_safe_mode   (7 | (CONTROL_CORE_PAD0_I2C4_SDA_PAD1_MCSPI1_CLK << reg_offset_gp) | (1<<pad_indicator_bp))

#define ae22_mcspi1_somi   (0 | (CONTROL_CORE_PAD0_MCSPI1_SOMI_PAD1_MCSPI1_SIMO << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae22_gpio_135   (3 | (CONTROL_CORE_PAD0_MCSPI1_SOMI_PAD1_MCSPI1_SIMO << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae22_safe_mode   (7 | (CONTROL_CORE_PAD0_MCSPI1_SOMI_PAD1_MCSPI1_SIMO << reg_offset_gp) | (0<<pad_indicator_bp))

#define ag22_mcspi1_simo   (0 | (CONTROL_CORE_PAD0_MCSPI1_SOMI_PAD1_MCSPI1_SIMO << reg_offset_gp) | (1<<pad_indicator_bp))
#define ag22_gpio_136   (3 | (CONTROL_CORE_PAD0_MCSPI1_SOMI_PAD1_MCSPI1_SIMO << reg_offset_gp) | (1<<pad_indicator_bp))
#define ag22_safe_mode   (7 | (CONTROL_CORE_PAD0_MCSPI1_SOMI_PAD1_MCSPI1_SIMO << reg_offset_gp) | (1<<pad_indicator_bp))

#define ae23_mcspi1_cs0   (0 | (CONTROL_CORE_PAD0_MCSPI1_CS0_PAD1_MCSPI1_CS1 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae23_gpio_137   (3 | (CONTROL_CORE_PAD0_MCSPI1_CS0_PAD1_MCSPI1_CS1 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae23_safe_mode   (7 | (CONTROL_CORE_PAD0_MCSPI1_CS0_PAD1_MCSPI1_CS1 << reg_offset_gp) | (0<<pad_indicator_bp))

#define af23_mcspi1_cs1   (0 | (CONTROL_CORE_PAD0_MCSPI1_CS0_PAD1_MCSPI1_CS1 << reg_offset_gp) | (1<<pad_indicator_bp))
#define af23_uart1_rx   (1 | (CONTROL_CORE_PAD0_MCSPI1_CS0_PAD1_MCSPI1_CS1 << reg_offset_gp) | (1<<pad_indicator_bp))
#define af23_gpio_138   (3 | (CONTROL_CORE_PAD0_MCSPI1_CS0_PAD1_MCSPI1_CS1 << reg_offset_gp) | (1<<pad_indicator_bp))
#define af23_safe_mode   (7 | (CONTROL_CORE_PAD0_MCSPI1_CS0_PAD1_MCSPI1_CS1 << reg_offset_gp) | (1<<pad_indicator_bp))

#define ag23_mcspi1_cs2   (0 | (CONTROL_CORE_PAD0_MCSPI1_CS2_PAD1_MCSPI1_CS3 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag23_uart1_cts   (1 | (CONTROL_CORE_PAD0_MCSPI1_CS2_PAD1_MCSPI1_CS3 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag23_slimbus2_clock   (2 | (CONTROL_CORE_PAD0_MCSPI1_CS2_PAD1_MCSPI1_CS3 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag23_gpio_139   (3 | (CONTROL_CORE_PAD0_MCSPI1_CS2_PAD1_MCSPI1_CS3 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag23_safe_mode   (7 | (CONTROL_CORE_PAD0_MCSPI1_CS2_PAD1_MCSPI1_CS3 << reg_offset_gp) | (0<<pad_indicator_bp))

#define ah23_mcspi1_cs3   (0 | (CONTROL_CORE_PAD0_MCSPI1_CS2_PAD1_MCSPI1_CS3 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ah23_uart1_rts   (1 | (CONTROL_CORE_PAD0_MCSPI1_CS2_PAD1_MCSPI1_CS3 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ah23_slimbus2_data   (2 | (CONTROL_CORE_PAD0_MCSPI1_CS2_PAD1_MCSPI1_CS3 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ah23_gpio_140   (3 | (CONTROL_CORE_PAD0_MCSPI1_CS2_PAD1_MCSPI1_CS3 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ah23_safe_mode   (7 | (CONTROL_CORE_PAD0_MCSPI1_CS2_PAD1_MCSPI1_CS3 << reg_offset_gp) | (1<<pad_indicator_bp))

#define f27_uart3_cts_rctx   (0 | (CONTROL_CORE_PAD0_UART3_CTS_RCTX_PAD1_UART3_RTS_SD << reg_offset_gp) | (0<<pad_indicator_bp))
#define f27_uart1_tx   (1 | (CONTROL_CORE_PAD0_UART3_CTS_RCTX_PAD1_UART3_RTS_SD << reg_offset_gp) | (0<<pad_indicator_bp))
#define f27_gpio_141   (3 | (CONTROL_CORE_PAD0_UART3_CTS_RCTX_PAD1_UART3_RTS_SD << reg_offset_gp) | (0<<pad_indicator_bp))
#define f27_safe_mode   (7 | (CONTROL_CORE_PAD0_UART3_CTS_RCTX_PAD1_UART3_RTS_SD << reg_offset_gp) | (0<<pad_indicator_bp))

#define f28_uart3_rts_sd   (0 | (CONTROL_CORE_PAD0_UART3_CTS_RCTX_PAD1_UART3_RTS_SD << reg_offset_gp) | (1<<pad_indicator_bp))
#define f28_cam_globalreset   (2 | (CONTROL_CORE_PAD0_UART3_CTS_RCTX_PAD1_UART3_RTS_SD << reg_offset_gp) | (1<<pad_indicator_bp))
#define f28_gpio_142   (3 | (CONTROL_CORE_PAD0_UART3_CTS_RCTX_PAD1_UART3_RTS_SD << reg_offset_gp) | (1<<pad_indicator_bp))
#define f28_safe_mode   (7 | (CONTROL_CORE_PAD0_UART3_CTS_RCTX_PAD1_UART3_RTS_SD << reg_offset_gp) | (1<<pad_indicator_bp))

#define g27_uart3_rx_irrx   (0 | (CONTROL_CORE_PAD0_UART3_RX_IRRX_PAD1_UART3_TX_IRTX << reg_offset_gp) | (0<<pad_indicator_bp))
#define g27_dmtimer8_pwm_evt   (1 | (CONTROL_CORE_PAD0_UART3_RX_IRRX_PAD1_UART3_TX_IRTX << reg_offset_gp) | (0<<pad_indicator_bp))
#define g27_cam_shutter   (2 | (CONTROL_CORE_PAD0_UART3_RX_IRRX_PAD1_UART3_TX_IRTX << reg_offset_gp) | (0<<pad_indicator_bp))
#define g27_gpio_143   (3 | (CONTROL_CORE_PAD0_UART3_RX_IRRX_PAD1_UART3_TX_IRTX << reg_offset_gp) | (0<<pad_indicator_bp))
#define g27_safe_mode   (7 | (CONTROL_CORE_PAD0_UART3_RX_IRRX_PAD1_UART3_TX_IRTX << reg_offset_gp) | (0<<pad_indicator_bp))

#define g28_uart3_tx_irtx   (0 | (CONTROL_CORE_PAD0_UART3_RX_IRRX_PAD1_UART3_TX_IRTX << reg_offset_gp) | (1<<pad_indicator_bp))
#define g28_dmtimer9_pwm_evt   (1 | (CONTROL_CORE_PAD0_UART3_RX_IRRX_PAD1_UART3_TX_IRTX << reg_offset_gp) | (1<<pad_indicator_bp))
#define g28_cam_strobe   (2 | (CONTROL_CORE_PAD0_UART3_RX_IRRX_PAD1_UART3_TX_IRTX << reg_offset_gp) | (1<<pad_indicator_bp))
#define g28_gpio_144   (3 | (CONTROL_CORE_PAD0_UART3_RX_IRRX_PAD1_UART3_TX_IRTX << reg_offset_gp) | (1<<pad_indicator_bp))
#define g28_safe_mode   (7 | (CONTROL_CORE_PAD0_UART3_RX_IRRX_PAD1_UART3_TX_IRTX << reg_offset_gp) | (1<<pad_indicator_bp))

#define ae5_sdmmc5_clk   (0 | (CONTROL_CORE_PAD0_SDMMC5_CLK_PAD1_SDMMC5_CMD << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae5_mcspi2_clk   (1 | (CONTROL_CORE_PAD0_SDMMC5_CLK_PAD1_SDMMC5_CMD << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae5_usbc1_icusb_dp   (2 | (CONTROL_CORE_PAD0_SDMMC5_CLK_PAD1_SDMMC5_CMD << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae5_gpio_145   (3 | (CONTROL_CORE_PAD0_SDMMC5_CLK_PAD1_SDMMC5_CMD << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae5_sdmmc2_clk   (5 | (CONTROL_CORE_PAD0_SDMMC5_CLK_PAD1_SDMMC5_CMD << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae5_safe_mode   (7 | (CONTROL_CORE_PAD0_SDMMC5_CLK_PAD1_SDMMC5_CMD << reg_offset_gp) | (0<<pad_indicator_bp))

#define af5_sdmmc5_cmd   (0 | (CONTROL_CORE_PAD0_SDMMC5_CLK_PAD1_SDMMC5_CMD << reg_offset_gp) | (1<<pad_indicator_bp))
#define af5_mcspi2_simo   (1 | (CONTROL_CORE_PAD0_SDMMC5_CLK_PAD1_SDMMC5_CMD << reg_offset_gp) | (1<<pad_indicator_bp))
#define af5_usbc1_icusb_dm   (2 | (CONTROL_CORE_PAD0_SDMMC5_CLK_PAD1_SDMMC5_CMD << reg_offset_gp) | (1<<pad_indicator_bp))
#define af5_gpio_146   (3 | (CONTROL_CORE_PAD0_SDMMC5_CLK_PAD1_SDMMC5_CMD << reg_offset_gp) | (1<<pad_indicator_bp))
#define af5_sdmmc2_cmd   (5 | (CONTROL_CORE_PAD0_SDMMC5_CLK_PAD1_SDMMC5_CMD << reg_offset_gp) | (1<<pad_indicator_bp))
#define af5_safe_mode   (7 | (CONTROL_CORE_PAD0_SDMMC5_CLK_PAD1_SDMMC5_CMD << reg_offset_gp) | (1<<pad_indicator_bp))

#define ae4_sdmmc5_dat0   (0 | (CONTROL_CORE_PAD0_SDMMC5_DAT0_PAD1_SDMMC5_DAT1 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae4_mcspi2_somi   (1 | (CONTROL_CORE_PAD0_SDMMC5_DAT0_PAD1_SDMMC5_DAT1 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae4_usbc1_icusb_rcv   (2 | (CONTROL_CORE_PAD0_SDMMC5_DAT0_PAD1_SDMMC5_DAT1 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae4_gpio_147   (3 | (CONTROL_CORE_PAD0_SDMMC5_DAT0_PAD1_SDMMC5_DAT1 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae4_sdmmc2_dat0   (5 | (CONTROL_CORE_PAD0_SDMMC5_DAT0_PAD1_SDMMC5_DAT1 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae4_safe_mode   (7 | (CONTROL_CORE_PAD0_SDMMC5_DAT0_PAD1_SDMMC5_DAT1 << reg_offset_gp) | (0<<pad_indicator_bp))

#define af4_sdmmc5_dat1   (0 | (CONTROL_CORE_PAD0_SDMMC5_DAT0_PAD1_SDMMC5_DAT1 << reg_offset_gp) | (1<<pad_indicator_bp))
#define af4_usbc1_icusb_txen   (2 | (CONTROL_CORE_PAD0_SDMMC5_DAT0_PAD1_SDMMC5_DAT1 << reg_offset_gp) | (1<<pad_indicator_bp))
#define af4_gpio_148   (3 | (CONTROL_CORE_PAD0_SDMMC5_DAT0_PAD1_SDMMC5_DAT1 << reg_offset_gp) | (1<<pad_indicator_bp))
#define af4_sdmmc2_dat1   (5 | (CONTROL_CORE_PAD0_SDMMC5_DAT0_PAD1_SDMMC5_DAT1 << reg_offset_gp) | (1<<pad_indicator_bp))
#define af4_safe_mode   (7 | (CONTROL_CORE_PAD0_SDMMC5_DAT0_PAD1_SDMMC5_DAT1 << reg_offset_gp) | (1<<pad_indicator_bp))

#define ag3_sdmmc5_dat2   (0 | (CONTROL_CORE_PAD0_SDMMC5_DAT2_PAD1_SDMMC5_DAT3 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag3_mcspi2_cs1   (1 | (CONTROL_CORE_PAD0_SDMMC5_DAT2_PAD1_SDMMC5_DAT3 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag3_gpio_149   (3 | (CONTROL_CORE_PAD0_SDMMC5_DAT2_PAD1_SDMMC5_DAT3 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag3_sdmmc2_dat2   (5 | (CONTROL_CORE_PAD0_SDMMC5_DAT2_PAD1_SDMMC5_DAT3 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag3_safe_mode   (7 | (CONTROL_CORE_PAD0_SDMMC5_DAT2_PAD1_SDMMC5_DAT3 << reg_offset_gp) | (0<<pad_indicator_bp))

#define af3_sdmmc5_dat3   (0 | (CONTROL_CORE_PAD0_SDMMC5_DAT2_PAD1_SDMMC5_DAT3 << reg_offset_gp) | (1<<pad_indicator_bp))
#define af3_mcspi2_cs0   (1 | (CONTROL_CORE_PAD0_SDMMC5_DAT2_PAD1_SDMMC5_DAT3 << reg_offset_gp) | (1<<pad_indicator_bp))
#define af3_gpio_150   (3 | (CONTROL_CORE_PAD0_SDMMC5_DAT2_PAD1_SDMMC5_DAT3 << reg_offset_gp) | (1<<pad_indicator_bp))
#define af3_sdmmc2_dat3   (5 | (CONTROL_CORE_PAD0_SDMMC5_DAT2_PAD1_SDMMC5_DAT3 << reg_offset_gp) | (1<<pad_indicator_bp))
#define af3_safe_mode   (7 | (CONTROL_CORE_PAD0_SDMMC5_DAT2_PAD1_SDMMC5_DAT3 << reg_offset_gp) | (1<<pad_indicator_bp))

#define ae21_mcspi4_clk   (0 | (CONTROL_CORE_PAD0_MCSPI4_CLK_PAD1_MCSPI4_SIMO << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae21_sdmmc4_clk   (1 | (CONTROL_CORE_PAD0_MCSPI4_CLK_PAD1_MCSPI4_SIMO << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae21_kpd_col6   (2 | (CONTROL_CORE_PAD0_MCSPI4_CLK_PAD1_MCSPI4_SIMO << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae21_gpio_151   (3 | (CONTROL_CORE_PAD0_MCSPI4_CLK_PAD1_MCSPI4_SIMO << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae21_safe_mode   (7 | (CONTROL_CORE_PAD0_MCSPI4_CLK_PAD1_MCSPI4_SIMO << reg_offset_gp) | (0<<pad_indicator_bp))

#define af20_mcspi4_simo   (0 | (CONTROL_CORE_PAD0_MCSPI4_CLK_PAD1_MCSPI4_SIMO << reg_offset_gp) | (1<<pad_indicator_bp))
#define af20_sdmmc4_cmd   (1 | (CONTROL_CORE_PAD0_MCSPI4_CLK_PAD1_MCSPI4_SIMO << reg_offset_gp) | (1<<pad_indicator_bp))
#define af20_kpd_col7   (2 | (CONTROL_CORE_PAD0_MCSPI4_CLK_PAD1_MCSPI4_SIMO << reg_offset_gp) | (1<<pad_indicator_bp))
#define af20_gpio_152   (3 | (CONTROL_CORE_PAD0_MCSPI4_CLK_PAD1_MCSPI4_SIMO << reg_offset_gp) | (1<<pad_indicator_bp))
#define af20_safe_mode   (7 | (CONTROL_CORE_PAD0_MCSPI4_CLK_PAD1_MCSPI4_SIMO << reg_offset_gp) | (1<<pad_indicator_bp))

#define af21_mcspi4_somi   (0 | (CONTROL_CORE_PAD0_MCSPI4_SOMI_PAD1_MCSPI4_CS0 << reg_offset_gp) | (0<<pad_indicator_bp))
#define af21_sdmmc4_dat0   (1 | (CONTROL_CORE_PAD0_MCSPI4_SOMI_PAD1_MCSPI4_CS0 << reg_offset_gp) | (0<<pad_indicator_bp))
#define af21_kpd_row6   (2 | (CONTROL_CORE_PAD0_MCSPI4_SOMI_PAD1_MCSPI4_CS0 << reg_offset_gp) | (0<<pad_indicator_bp))
#define af21_gpio_153   (3 | (CONTROL_CORE_PAD0_MCSPI4_SOMI_PAD1_MCSPI4_CS0 << reg_offset_gp) | (0<<pad_indicator_bp))
#define af21_safe_mode   (7 | (CONTROL_CORE_PAD0_MCSPI4_SOMI_PAD1_MCSPI4_CS0 << reg_offset_gp) | (0<<pad_indicator_bp))

#define ae20_mcspi4_cs0   (0 | (CONTROL_CORE_PAD0_MCSPI4_SOMI_PAD1_MCSPI4_CS0 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ae20_sdmmc4_dat3   (1 | (CONTROL_CORE_PAD0_MCSPI4_SOMI_PAD1_MCSPI4_CS0 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ae20_kpd_row7   (2 | (CONTROL_CORE_PAD0_MCSPI4_SOMI_PAD1_MCSPI4_CS0 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ae20_gpio_154   (3 | (CONTROL_CORE_PAD0_MCSPI4_SOMI_PAD1_MCSPI4_CS0 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ae20_safe_mode   (7 | (CONTROL_CORE_PAD0_MCSPI4_SOMI_PAD1_MCSPI4_CS0 << reg_offset_gp) | (1<<pad_indicator_bp))

#define ag20_uart4_rx   (0 | (CONTROL_CORE_PAD0_UART4_RX_PAD1_UART4_TX << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag20_sdmmc4_dat2   (1 | (CONTROL_CORE_PAD0_UART4_RX_PAD1_UART4_TX << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag20_kpd_row8   (2 | (CONTROL_CORE_PAD0_UART4_RX_PAD1_UART4_TX << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag20_gpio_155   (3 | (CONTROL_CORE_PAD0_UART4_RX_PAD1_UART4_TX << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag20_safe_mode   (7 | (CONTROL_CORE_PAD0_UART4_RX_PAD1_UART4_TX << reg_offset_gp) | (0<<pad_indicator_bp))

#define ah19_uart4_tx   (0 | (CONTROL_CORE_PAD0_UART4_RX_PAD1_UART4_TX << reg_offset_gp) | (1<<pad_indicator_bp))
#define ah19_sdmmc4_dat1   (1 | (CONTROL_CORE_PAD0_UART4_RX_PAD1_UART4_TX << reg_offset_gp) | (1<<pad_indicator_bp))
#define ah19_kpd_col8   (2 | (CONTROL_CORE_PAD0_UART4_RX_PAD1_UART4_TX << reg_offset_gp) | (1<<pad_indicator_bp))
#define ah19_gpio_156   (3 | (CONTROL_CORE_PAD0_UART4_RX_PAD1_UART4_TX << reg_offset_gp) | (1<<pad_indicator_bp))
#define ah19_safe_mode   (7 | (CONTROL_CORE_PAD0_UART4_RX_PAD1_UART4_TX << reg_offset_gp) | (1<<pad_indicator_bp))

#define ag12_usbb2_ulpitll_clk   (0 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_CLK_PAD1_USBB2_ULPITLL_STP << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag12_usbb2_ulpiphy_clk   (1 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_CLK_PAD1_USBB2_ULPITLL_STP << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag12_sdmmc4_cmd   (2 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_CLK_PAD1_USBB2_ULPITLL_STP << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag12_gpio_157   (3 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_CLK_PAD1_USBB2_ULPITLL_STP << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag12_hsi2_cawake   (4 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_CLK_PAD1_USBB2_ULPITLL_STP << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag12_safe_mode   (7 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_CLK_PAD1_USBB2_ULPITLL_STP << reg_offset_gp) | (0<<pad_indicator_bp))

#define af12_usbb2_ulpitll_stp   (0 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_CLK_PAD1_USBB2_ULPITLL_STP << reg_offset_gp) | (1<<pad_indicator_bp))
#define af12_usbb2_ulpiphy_stp   (1 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_CLK_PAD1_USBB2_ULPITLL_STP << reg_offset_gp) | (1<<pad_indicator_bp))
#define af12_sdmmc4_clk   (2 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_CLK_PAD1_USBB2_ULPITLL_STP << reg_offset_gp) | (1<<pad_indicator_bp))
#define af12_gpio_158   (3 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_CLK_PAD1_USBB2_ULPITLL_STP << reg_offset_gp) | (1<<pad_indicator_bp))
#define af12_hsi2_cadata   (4 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_CLK_PAD1_USBB2_ULPITLL_STP << reg_offset_gp) | (1<<pad_indicator_bp))
#define af12_dispc2_data23   (5 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_CLK_PAD1_USBB2_ULPITLL_STP << reg_offset_gp) | (1<<pad_indicator_bp))
#define af12_safe_mode   (7 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_CLK_PAD1_USBB2_ULPITLL_STP << reg_offset_gp) | (1<<pad_indicator_bp))

#define ae12_usbb2_ulpitll_dir   (0 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DIR_PAD1_USBB2_ULPITLL_NXT << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae12_usbb2_ulpiphy_dir   (1 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DIR_PAD1_USBB2_ULPITLL_NXT << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae12_sdmmc4_dat0   (2 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DIR_PAD1_USBB2_ULPITLL_NXT << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae12_gpio_159   (3 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DIR_PAD1_USBB2_ULPITLL_NXT << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae12_hsi2_caflag   (4 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DIR_PAD1_USBB2_ULPITLL_NXT << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae12_dispc2_data22   (5 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DIR_PAD1_USBB2_ULPITLL_NXT << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae12_safe_mode   (7 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DIR_PAD1_USBB2_ULPITLL_NXT << reg_offset_gp) | (0<<pad_indicator_bp))

#define ag13_usbb2_ulpitll_nxt   (0 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DIR_PAD1_USBB2_ULPITLL_NXT << reg_offset_gp) | (1<<pad_indicator_bp))
#define ag13_usbb2_ulpiphy_nxt   (1 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DIR_PAD1_USBB2_ULPITLL_NXT << reg_offset_gp) | (1<<pad_indicator_bp))
#define ag13_sdmmc4_dat1   (2 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DIR_PAD1_USBB2_ULPITLL_NXT << reg_offset_gp) | (1<<pad_indicator_bp))
#define ag13_gpio_160   (3 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DIR_PAD1_USBB2_ULPITLL_NXT << reg_offset_gp) | (1<<pad_indicator_bp))
#define ag13_hsi2_acready   (4 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DIR_PAD1_USBB2_ULPITLL_NXT << reg_offset_gp) | (1<<pad_indicator_bp))
#define ag13_dispc2_data21   (5 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DIR_PAD1_USBB2_ULPITLL_NXT << reg_offset_gp) | (1<<pad_indicator_bp))
#define ag13_safe_mode   (7 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DIR_PAD1_USBB2_ULPITLL_NXT << reg_offset_gp) | (1<<pad_indicator_bp))

#define ae11_usbb2_ulpitll_dat0   (0 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT0_PAD1_USBB2_ULPITLL_DAT1 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae11_usbb2_ulpiphy_dat0   (1 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT0_PAD1_USBB2_ULPITLL_DAT1 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae11_sdmmc4_dat2   (2 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT0_PAD1_USBB2_ULPITLL_DAT1 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae11_gpio_161   (3 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT0_PAD1_USBB2_ULPITLL_DAT1 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae11_hsi2_acwake   (4 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT0_PAD1_USBB2_ULPITLL_DAT1 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae11_dispc2_data20   (5 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT0_PAD1_USBB2_ULPITLL_DAT1 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae11_usbb2_mm_txen   (6 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT0_PAD1_USBB2_ULPITLL_DAT1 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae11_safe_mode   (7 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT0_PAD1_USBB2_ULPITLL_DAT1 << reg_offset_gp) | (0<<pad_indicator_bp))

#define af11_usbb2_ulpitll_dat1   (0 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT0_PAD1_USBB2_ULPITLL_DAT1 << reg_offset_gp) | (1<<pad_indicator_bp))
#define af11_usbb2_ulpiphy_dat1   (1 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT0_PAD1_USBB2_ULPITLL_DAT1 << reg_offset_gp) | (1<<pad_indicator_bp))
#define af11_sdmmc4_dat3   (2 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT0_PAD1_USBB2_ULPITLL_DAT1 << reg_offset_gp) | (1<<pad_indicator_bp))
#define af11_gpio_162   (3 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT0_PAD1_USBB2_ULPITLL_DAT1 << reg_offset_gp) | (1<<pad_indicator_bp))
#define af11_hsi2_acdata   (4 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT0_PAD1_USBB2_ULPITLL_DAT1 << reg_offset_gp) | (1<<pad_indicator_bp))
#define af11_dispc2_data19   (5 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT0_PAD1_USBB2_ULPITLL_DAT1 << reg_offset_gp) | (1<<pad_indicator_bp))
#define af11_usbb2_mm_txdat   (6 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT0_PAD1_USBB2_ULPITLL_DAT1 << reg_offset_gp) | (1<<pad_indicator_bp))
#define af11_safe_mode   (7 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT0_PAD1_USBB2_ULPITLL_DAT1 << reg_offset_gp) | (1<<pad_indicator_bp))

#define ag11_usbb2_ulpitll_dat2   (0 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT2_PAD1_USBB2_ULPITLL_DAT3 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag11_usbb2_ulpiphy_dat2   (1 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT2_PAD1_USBB2_ULPITLL_DAT3 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag11_sdmmc3_dat2   (2 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT2_PAD1_USBB2_ULPITLL_DAT3 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag11_gpio_163   (3 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT2_PAD1_USBB2_ULPITLL_DAT3 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag11_hsi2_acflag   (4 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT2_PAD1_USBB2_ULPITLL_DAT3 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag11_dispc2_data18   (5 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT2_PAD1_USBB2_ULPITLL_DAT3 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag11_usbb2_mm_txse0   (6 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT2_PAD1_USBB2_ULPITLL_DAT3 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag11_safe_mode   (7 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT2_PAD1_USBB2_ULPITLL_DAT3 << reg_offset_gp) | (0<<pad_indicator_bp))

#define ah11_usbb2_ulpitll_dat3   (0 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT2_PAD1_USBB2_ULPITLL_DAT3 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ah11_usbb2_ulpiphy_dat3   (1 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT2_PAD1_USBB2_ULPITLL_DAT3 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ah11_sdmmc3_dat1   (2 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT2_PAD1_USBB2_ULPITLL_DAT3 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ah11_gpio_164   (3 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT2_PAD1_USBB2_ULPITLL_DAT3 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ah11_hsi2_caready   (4 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT2_PAD1_USBB2_ULPITLL_DAT3 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ah11_dispc2_data15   (5 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT2_PAD1_USBB2_ULPITLL_DAT3 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ah11_rfbi_data15   (6 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT2_PAD1_USBB2_ULPITLL_DAT3 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ah11_safe_mode   (7 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT2_PAD1_USBB2_ULPITLL_DAT3 << reg_offset_gp) | (1<<pad_indicator_bp))

#define ae10_usbb2_ulpitll_dat4   (0 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT4_PAD1_USBB2_ULPITLL_DAT5 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae10_usbb2_ulpiphy_dat4   (1 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT4_PAD1_USBB2_ULPITLL_DAT5 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae10_sdmmc3_dat0   (2 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT4_PAD1_USBB2_ULPITLL_DAT5 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae10_gpio_165   (3 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT4_PAD1_USBB2_ULPITLL_DAT5 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae10_mcspi3_somi   (4 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT4_PAD1_USBB2_ULPITLL_DAT5 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae10_dispc2_data14   (5 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT4_PAD1_USBB2_ULPITLL_DAT5 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae10_rfbi_data14   (6 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT4_PAD1_USBB2_ULPITLL_DAT5 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ae10_safe_mode   (7 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT4_PAD1_USBB2_ULPITLL_DAT5 << reg_offset_gp) | (0<<pad_indicator_bp))

#define af10_usbb2_ulpitll_dat5   (0 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT4_PAD1_USBB2_ULPITLL_DAT5 << reg_offset_gp) | (1<<pad_indicator_bp))
#define af10_usbb2_ulpiphy_dat5   (1 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT4_PAD1_USBB2_ULPITLL_DAT5 << reg_offset_gp) | (1<<pad_indicator_bp))
#define af10_sdmmc3_dat3   (2 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT4_PAD1_USBB2_ULPITLL_DAT5 << reg_offset_gp) | (1<<pad_indicator_bp))
#define af10_gpio_166   (3 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT4_PAD1_USBB2_ULPITLL_DAT5 << reg_offset_gp) | (1<<pad_indicator_bp))
#define af10_mcspi3_cs0   (4 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT4_PAD1_USBB2_ULPITLL_DAT5 << reg_offset_gp) | (1<<pad_indicator_bp))
#define af10_dispc2_data13   (5 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT4_PAD1_USBB2_ULPITLL_DAT5 << reg_offset_gp) | (1<<pad_indicator_bp))
#define af10_rfbi_data13   (6 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT4_PAD1_USBB2_ULPITLL_DAT5 << reg_offset_gp) | (1<<pad_indicator_bp))
#define af10_safe_mode   (7 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT4_PAD1_USBB2_ULPITLL_DAT5 << reg_offset_gp) | (1<<pad_indicator_bp))

#define ag10_usbb2_ulpitll_dat6   (0 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT6_PAD1_USBB2_ULPITLL_DAT7 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag10_usbb2_ulpiphy_dat6   (1 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT6_PAD1_USBB2_ULPITLL_DAT7 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag10_sdmmc3_cmd   (2 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT6_PAD1_USBB2_ULPITLL_DAT7 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag10_gpio_167   (3 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT6_PAD1_USBB2_ULPITLL_DAT7 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag10_mcspi3_simo   (4 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT6_PAD1_USBB2_ULPITLL_DAT7 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag10_dispc2_data12   (5 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT6_PAD1_USBB2_ULPITLL_DAT7 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag10_rfbi_data12   (6 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT6_PAD1_USBB2_ULPITLL_DAT7 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ag10_safe_mode   (7 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT6_PAD1_USBB2_ULPITLL_DAT7 << reg_offset_gp) | (0<<pad_indicator_bp))

#define ae9_usbb2_ulpitll_dat7   (0 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT6_PAD1_USBB2_ULPITLL_DAT7 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ae9_usbb2_ulpiphy_dat7   (1 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT6_PAD1_USBB2_ULPITLL_DAT7 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ae9_sdmmc3_clk   (2 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT6_PAD1_USBB2_ULPITLL_DAT7 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ae9_gpio_168   (3 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT6_PAD1_USBB2_ULPITLL_DAT7 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ae9_mcspi3_clk   (4 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT6_PAD1_USBB2_ULPITLL_DAT7 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ae9_dispc2_data11   (5 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT6_PAD1_USBB2_ULPITLL_DAT7 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ae9_rfbi_data11   (6 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT6_PAD1_USBB2_ULPITLL_DAT7 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ae9_safe_mode   (7 | (CONTROL_CORE_PAD0_USBB2_ULPITLL_DAT6_PAD1_USBB2_ULPITLL_DAT7 << reg_offset_gp) | (1<<pad_indicator_bp))

#define af13_usbb2_hsic_data   (0 | (CONTROL_CORE_PAD0_USBB2_HSIC_DATA_PAD1_USBB2_HSIC_STROBE << reg_offset_gp) | (0<<pad_indicator_bp))
#define af13_gpio_169   (3 | (CONTROL_CORE_PAD0_USBB2_HSIC_DATA_PAD1_USBB2_HSIC_STROBE << reg_offset_gp) | (0<<pad_indicator_bp))
#define af13_safe_mode   (7 | (CONTROL_CORE_PAD0_USBB2_HSIC_DATA_PAD1_USBB2_HSIC_STROBE << reg_offset_gp) | (0<<pad_indicator_bp))

#define ae13_usbb2_hsic_strobe   (0 | (CONTROL_CORE_PAD0_USBB2_HSIC_DATA_PAD1_USBB2_HSIC_STROBE << reg_offset_gp) | (1<<pad_indicator_bp))
#define ae13_gpio_170   (3 | (CONTROL_CORE_PAD0_USBB2_HSIC_DATA_PAD1_USBB2_HSIC_STROBE << reg_offset_gp) | (1<<pad_indicator_bp))
#define ae13_safe_mode   (7 | (CONTROL_CORE_PAD0_USBB2_HSIC_DATA_PAD1_USBB2_HSIC_STROBE << reg_offset_gp) | (1<<pad_indicator_bp))

#define g26_kpd_col3   (0 | (CONTROL_CORE_PAD0_KPD_COL3_PAD1_KPD_COL4 << reg_offset_gp) | (0<<pad_indicator_bp))
#define g26_kpd_col0   (1 | (CONTROL_CORE_PAD0_KPD_COL3_PAD1_KPD_COL4 << reg_offset_gp) | (0<<pad_indicator_bp))
#define g26_cam2_d0   (2 | (CONTROL_CORE_PAD0_KPD_COL3_PAD1_KPD_COL4 << reg_offset_gp) | (0<<pad_indicator_bp))
#define g26_gpio_171   (3 | (CONTROL_CORE_PAD0_KPD_COL3_PAD1_KPD_COL4 << reg_offset_gp) | (0<<pad_indicator_bp))
#define g26_safe_mode   (7 | (CONTROL_CORE_PAD0_KPD_COL3_PAD1_KPD_COL4 << reg_offset_gp) | (0<<pad_indicator_bp))

#define g25_kpd_col4   (0 | (CONTROL_CORE_PAD0_KPD_COL3_PAD1_KPD_COL4 << reg_offset_gp) | (1<<pad_indicator_bp))
#define g25_kpd_col1   (1 | (CONTROL_CORE_PAD0_KPD_COL3_PAD1_KPD_COL4 << reg_offset_gp) | (1<<pad_indicator_bp))
#define g25_cam2_d1   (2 | (CONTROL_CORE_PAD0_KPD_COL3_PAD1_KPD_COL4 << reg_offset_gp) | (1<<pad_indicator_bp))
#define g25_gpio_172   (3 | (CONTROL_CORE_PAD0_KPD_COL3_PAD1_KPD_COL4 << reg_offset_gp) | (1<<pad_indicator_bp))
#define g25_safe_mode   (7 | (CONTROL_CORE_PAD0_KPD_COL3_PAD1_KPD_COL4 << reg_offset_gp) | (1<<pad_indicator_bp))

#define h26_kpd_col5   (0 | (CONTROL_CORE_PAD0_KPD_COL5_PAD1_KPD_COL0 << reg_offset_gp) | (0<<pad_indicator_bp))
#define h26_kpd_col2   (1 | (CONTROL_CORE_PAD0_KPD_COL5_PAD1_KPD_COL0 << reg_offset_gp) | (0<<pad_indicator_bp))
#define h26_cam2_d2   (2 | (CONTROL_CORE_PAD0_KPD_COL5_PAD1_KPD_COL0 << reg_offset_gp) | (0<<pad_indicator_bp))
#define h26_gpio_173   (3 | (CONTROL_CORE_PAD0_KPD_COL5_PAD1_KPD_COL0 << reg_offset_gp) | (0<<pad_indicator_bp))
#define h26_safe_mode   (7 | (CONTROL_CORE_PAD0_KPD_COL5_PAD1_KPD_COL0 << reg_offset_gp) | (0<<pad_indicator_bp))

#define h25_kpd_col0   (0 | (CONTROL_CORE_PAD0_KPD_COL5_PAD1_KPD_COL0 << reg_offset_gp) | (1<<pad_indicator_bp))
#define h25_kpd_col3   (1 | (CONTROL_CORE_PAD0_KPD_COL5_PAD1_KPD_COL0 << reg_offset_gp) | (1<<pad_indicator_bp))
#define h25_cam2_d3   (2 | (CONTROL_CORE_PAD0_KPD_COL5_PAD1_KPD_COL0 << reg_offset_gp) | (1<<pad_indicator_bp))
#define h25_gpio_174   (3 | (CONTROL_CORE_PAD0_KPD_COL5_PAD1_KPD_COL0 << reg_offset_gp) | (1<<pad_indicator_bp))
#define h25_safe_mode   (7 | (CONTROL_CORE_PAD0_KPD_COL5_PAD1_KPD_COL0 << reg_offset_gp) | (1<<pad_indicator_bp))

#define j27_kpd_col1   (0 | (CONTROL_CORE_PAD0_KPD_COL1_PAD1_KPD_COL2 << reg_offset_gp) | (0<<pad_indicator_bp))
#define j27_kpd_col4   (1 | (CONTROL_CORE_PAD0_KPD_COL1_PAD1_KPD_COL2 << reg_offset_gp) | (0<<pad_indicator_bp))
#define j27_cam2_d8   (2 | (CONTROL_CORE_PAD0_KPD_COL1_PAD1_KPD_COL2 << reg_offset_gp) | (0<<pad_indicator_bp))
#define j27_gpio_0   (3 | (CONTROL_CORE_PAD0_KPD_COL1_PAD1_KPD_COL2 << reg_offset_gp) | (0<<pad_indicator_bp))
#define j27_safe_mode   (7 | (CONTROL_CORE_PAD0_KPD_COL1_PAD1_KPD_COL2 << reg_offset_gp) | (0<<pad_indicator_bp))

#define h27_kpd_col2   (0 | (CONTROL_CORE_PAD0_KPD_COL1_PAD1_KPD_COL2 << reg_offset_gp) | (1<<pad_indicator_bp))
#define h27_kpd_col5   (1 | (CONTROL_CORE_PAD0_KPD_COL1_PAD1_KPD_COL2 << reg_offset_gp) | (1<<pad_indicator_bp))
#define h27_cam2_d10   (2 | (CONTROL_CORE_PAD0_KPD_COL1_PAD1_KPD_COL2 << reg_offset_gp) | (1<<pad_indicator_bp))
#define h27_gpio_1   (3 | (CONTROL_CORE_PAD0_KPD_COL1_PAD1_KPD_COL2 << reg_offset_gp) | (1<<pad_indicator_bp))
#define h27_safe_mode   (7 | (CONTROL_CORE_PAD0_KPD_COL1_PAD1_KPD_COL2 << reg_offset_gp) | (1<<pad_indicator_bp))

#define j26_kpd_row3   (0 | (CONTROL_CORE_PAD0_KPD_ROW3_PAD1_KPD_ROW4 << reg_offset_gp) | (0<<pad_indicator_bp))
#define j26_kpd_row0   (1 | (CONTROL_CORE_PAD0_KPD_ROW3_PAD1_KPD_ROW4 << reg_offset_gp) | (0<<pad_indicator_bp))
#define j26_cam2_d4   (2 | (CONTROL_CORE_PAD0_KPD_ROW3_PAD1_KPD_ROW4 << reg_offset_gp) | (0<<pad_indicator_bp))
#define j26_gpio_175   (3 | (CONTROL_CORE_PAD0_KPD_ROW3_PAD1_KPD_ROW4 << reg_offset_gp) | (0<<pad_indicator_bp))
#define j26_safe_mode   (7 | (CONTROL_CORE_PAD0_KPD_ROW3_PAD1_KPD_ROW4 << reg_offset_gp) | (0<<pad_indicator_bp))

#define j25_kpd_row4   (0 | (CONTROL_CORE_PAD0_KPD_ROW3_PAD1_KPD_ROW4 << reg_offset_gp) | (1<<pad_indicator_bp))
#define j25_kpd_row1   (1 | (CONTROL_CORE_PAD0_KPD_ROW3_PAD1_KPD_ROW4 << reg_offset_gp) | (1<<pad_indicator_bp))
#define j25_cam2_d5   (2 | (CONTROL_CORE_PAD0_KPD_ROW3_PAD1_KPD_ROW4 << reg_offset_gp) | (1<<pad_indicator_bp))
#define j25_gpio_176   (3 | (CONTROL_CORE_PAD0_KPD_ROW3_PAD1_KPD_ROW4 << reg_offset_gp) | (1<<pad_indicator_bp))
#define j25_safe_mode   (7 | (CONTROL_CORE_PAD0_KPD_ROW3_PAD1_KPD_ROW4 << reg_offset_gp) | (1<<pad_indicator_bp))

#define k26_kpd_row5   (0 | (CONTROL_CORE_PAD0_KPD_ROW5_PAD1_KPD_ROW0 << reg_offset_gp) | (0<<pad_indicator_bp))
#define k26_kpd_row2   (1 | (CONTROL_CORE_PAD0_KPD_ROW5_PAD1_KPD_ROW0 << reg_offset_gp) | (0<<pad_indicator_bp))
#define k26_cam2_d6   (2 | (CONTROL_CORE_PAD0_KPD_ROW5_PAD1_KPD_ROW0 << reg_offset_gp) | (0<<pad_indicator_bp))
#define k26_gpio_177   (3 | (CONTROL_CORE_PAD0_KPD_ROW5_PAD1_KPD_ROW0 << reg_offset_gp) | (0<<pad_indicator_bp))
#define k26_safe_mode   (7 | (CONTROL_CORE_PAD0_KPD_ROW5_PAD1_KPD_ROW0 << reg_offset_gp) | (0<<pad_indicator_bp))

#define k25_kpd_row0   (0 | (CONTROL_CORE_PAD0_KPD_ROW5_PAD1_KPD_ROW0 << reg_offset_gp) | (1<<pad_indicator_bp))
#define k25_kpd_row3   (1 | (CONTROL_CORE_PAD0_KPD_ROW5_PAD1_KPD_ROW0 << reg_offset_gp) | (1<<pad_indicator_bp))
#define k25_cam2_d7   (2 | (CONTROL_CORE_PAD0_KPD_ROW5_PAD1_KPD_ROW0 << reg_offset_gp) | (1<<pad_indicator_bp))
#define k25_gpio_178   (3 | (CONTROL_CORE_PAD0_KPD_ROW5_PAD1_KPD_ROW0 << reg_offset_gp) | (1<<pad_indicator_bp))
#define k25_safe_mode   (7 | (CONTROL_CORE_PAD0_KPD_ROW5_PAD1_KPD_ROW0 << reg_offset_gp) | (1<<pad_indicator_bp))

#define l27_kpd_row1   (0 | (CONTROL_CORE_PAD0_KPD_ROW1_PAD1_KPD_ROW2 << reg_offset_gp) | (0<<pad_indicator_bp))
#define l27_kpd_row4   (1 | (CONTROL_CORE_PAD0_KPD_ROW1_PAD1_KPD_ROW2 << reg_offset_gp) | (0<<pad_indicator_bp))
#define l27_cam2_d9   (2 | (CONTROL_CORE_PAD0_KPD_ROW1_PAD1_KPD_ROW2 << reg_offset_gp) | (0<<pad_indicator_bp))
#define l27_gpio_2   (3 | (CONTROL_CORE_PAD0_KPD_ROW1_PAD1_KPD_ROW2 << reg_offset_gp) | (0<<pad_indicator_bp))
#define l27_safe_mode   (7 | (CONTROL_CORE_PAD0_KPD_ROW1_PAD1_KPD_ROW2 << reg_offset_gp) | (0<<pad_indicator_bp))

#define k27_kpd_row2   (0 | (CONTROL_CORE_PAD0_KPD_ROW1_PAD1_KPD_ROW2 << reg_offset_gp) | (1<<pad_indicator_bp))
#define k27_kpd_row5   (1 | (CONTROL_CORE_PAD0_KPD_ROW1_PAD1_KPD_ROW2 << reg_offset_gp) | (1<<pad_indicator_bp))
#define k27_cam2_d11   (2 | (CONTROL_CORE_PAD0_KPD_ROW1_PAD1_KPD_ROW2 << reg_offset_gp) | (1<<pad_indicator_bp))
#define k27_gpio_3   (3 | (CONTROL_CORE_PAD0_KPD_ROW1_PAD1_KPD_ROW2 << reg_offset_gp) | (1<<pad_indicator_bp))
#define k27_safe_mode   (7 | (CONTROL_CORE_PAD0_KPD_ROW1_PAD1_KPD_ROW2 << reg_offset_gp) | (1<<pad_indicator_bp))

#define c3_usba0_otg_ce   (0 | (CONTROL_CORE_PAD0_USBA0_OTG_CE_PAD1_USBA0_OTG_DP << reg_offset_gp) | (0<<pad_indicator_bp))

#define b5_usba0_otg_dp   (0 | (CONTROL_CORE_PAD0_USBA0_OTG_CE_PAD1_USBA0_OTG_DP << reg_offset_gp) | (1<<pad_indicator_bp))
#define b5_uart3_rx_irrx   (1 | (CONTROL_CORE_PAD0_USBA0_OTG_CE_PAD1_USBA0_OTG_DP << reg_offset_gp) | (1<<pad_indicator_bp))
#define b5_uart2_rx   (2 | (CONTROL_CORE_PAD0_USBA0_OTG_CE_PAD1_USBA0_OTG_DP << reg_offset_gp) | (1<<pad_indicator_bp))
#define b5_safe_mode   (7 | (CONTROL_CORE_PAD0_USBA0_OTG_CE_PAD1_USBA0_OTG_DP << reg_offset_gp) | (1<<pad_indicator_bp))

#define b4_usba0_otg_dm   (0 | (CONTROL_CORE_PAD0_USBA0_OTG_DM_PAD1_FREF_CLK1_OUT << reg_offset_gp) | (0<<pad_indicator_bp))
#define b4_uart3_tx_irtx   (1 | (CONTROL_CORE_PAD0_USBA0_OTG_DM_PAD1_FREF_CLK1_OUT << reg_offset_gp) | (0<<pad_indicator_bp))
#define b4_uart2_tx   (2 | (CONTROL_CORE_PAD0_USBA0_OTG_DM_PAD1_FREF_CLK1_OUT << reg_offset_gp) | (0<<pad_indicator_bp))
#define b4_safe_mode   (7 | (CONTROL_CORE_PAD0_USBA0_OTG_DM_PAD1_FREF_CLK1_OUT << reg_offset_gp) | (0<<pad_indicator_bp))

#define ah6_fref_xtal_in   (0 | (CONTROL_WKUP_PAD0_SR_SDA_PAD1_FREF_XTAL_IN << reg_offset_gp) | (1<<pad_indicator_bp) | pad_padconf_wkup_bm)

#define ah5_fref_xtal_out   (0 | (0 << reg_offset_gp) | (0<<pad_indicator_bp))

#define ag5_fref_xtal_vssosc   (0 | (0 << reg_offset_gp) | (0<<pad_indicator_bp))

#define ag8_fref_slicer_in   (CONTROL_WKUP_PAD0_FREF_SLICER_IN_PAD1_FREF_CLK_IOREQ | (0 << reg_offset_gp) | (0<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define ag8_gpi_wk5   (3 | (CONTROL_WKUP_PAD0_FREF_SLICER_IN_PAD1_FREF_CLK_IOREQ << reg_offset_gp) | (0<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define ag8_safe_mode   (7 | (CONTROL_WKUP_PAD0_FREF_SLICER_IN_PAD1_FREF_CLK_IOREQ << reg_offset_gp) | (0<<pad_indicator_bp) | pad_padconf_wkup_bm)

#define ad1_fref_clk_ioreq   (0 | (CONTROL_WKUP_PAD0_FREF_SLICER_IN_PAD1_FREF_CLK_IOREQ << reg_offset_gp) | (1<<pad_indicator_bp) | pad_padconf_wkup_bm)

#define ad2_fref_clk0_out   (0 | (CONTROL_WKUP_PAD0_FREF_CLK0_OUT_PAD1_FREF_CLK3_REQ << reg_offset_gp) | (0<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define ad2_fref_clk1_req   (1 | (CONTROL_WKUP_PAD0_FREF_CLK0_OUT_PAD1_FREF_CLK3_REQ << reg_offset_gp) | (0<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define ad2_sys_drm_msecure   (2 | (CONTROL_WKUP_PAD0_FREF_CLK0_OUT_PAD1_FREF_CLK3_REQ << reg_offset_gp) | (0<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define ad2_gpio_wk6   (3 | (CONTROL_WKUP_PAD0_FREF_CLK0_OUT_PAD1_FREF_CLK3_REQ << reg_offset_gp) | (0<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define ad2_sdmmc2_dat7   (5 | (CONTROL_WKUP_PAD0_FREF_CLK0_OUT_PAD1_FREF_CLK3_REQ << reg_offset_gp) | (0<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define ad2_attila_hw_dbg6   (6 | (CONTROL_WKUP_PAD0_FREF_CLK0_OUT_PAD1_FREF_CLK3_REQ << reg_offset_gp) | (0<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define ad2_safe_mode   (7 | (CONTROL_WKUP_PAD0_FREF_CLK0_OUT_PAD1_FREF_CLK3_REQ << reg_offset_gp) | (0<<pad_indicator_bp) | pad_padconf_wkup_bm)

#define aa28_fref_clk1_out   (0 | (CONTROL_CORE_PAD0_USBA0_OTG_DM_PAD1_FREF_CLK1_OUT << reg_offset_gp) | (1<<pad_indicator_bp))
#define aa28_gpio_181   (3 | (CONTROL_CORE_PAD0_USBA0_OTG_DM_PAD1_FREF_CLK1_OUT << reg_offset_gp) | (1<<pad_indicator_bp))
#define aa28_safe_mode   (7 | (CONTROL_CORE_PAD0_USBA0_OTG_DM_PAD1_FREF_CLK1_OUT << reg_offset_gp) | (1<<pad_indicator_bp))

#define y28_fref_clk2_out   (0 | (CONTROL_CORE_PAD0_FREF_CLK2_OUT_PAD1_SYS_NIRQ1 << reg_offset_gp) | (0<<pad_indicator_bp))
#define y28_gpio_182   (3 | (CONTROL_CORE_PAD0_FREF_CLK2_OUT_PAD1_SYS_NIRQ1 << reg_offset_gp) | (0<<pad_indicator_bp))
#define y28_safe_mode   (7 | (CONTROL_CORE_PAD0_FREF_CLK2_OUT_PAD1_SYS_NIRQ1 << reg_offset_gp) | (0<<pad_indicator_bp))

#define ad3_fref_clk3_req   (0 | (CONTROL_WKUP_PAD0_FREF_CLK0_OUT_PAD1_FREF_CLK3_REQ << reg_offset_gp) | (1<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define ad3_fref_clk1_req   (1 | (CONTROL_WKUP_PAD0_FREF_CLK0_OUT_PAD1_FREF_CLK3_REQ << reg_offset_gp) | (1<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define ad3_sys_drm_msecure   (2 | (CONTROL_WKUP_PAD0_FREF_CLK0_OUT_PAD1_FREF_CLK3_REQ << reg_offset_gp) | (1<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define ad3_gpio_wk30   (3 | (CONTROL_WKUP_PAD0_FREF_CLK0_OUT_PAD1_FREF_CLK3_REQ << reg_offset_gp) | (1<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define ad3_sdmmc2_dat4   (5 | (CONTROL_WKUP_PAD0_FREF_CLK0_OUT_PAD1_FREF_CLK3_REQ << reg_offset_gp) | (1<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define ad3_attila_hw_dbg7   (6 | (CONTROL_WKUP_PAD0_FREF_CLK0_OUT_PAD1_FREF_CLK3_REQ << reg_offset_gp) | (1<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define ad3_safe_mode   (7 | (CONTROL_WKUP_PAD0_FREF_CLK0_OUT_PAD1_FREF_CLK3_REQ << reg_offset_gp) | (1<<pad_indicator_bp) | pad_padconf_wkup_bm)

#define ad4_fref_clk3_out   (0 | (CONTROL_WKUP_PAD0_FREF_CLK3_OUT_PAD1_FREF_CLK4_REQ << reg_offset_gp) | (0<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define ad4_fref_clk2_req   (1 | (CONTROL_WKUP_PAD0_FREF_CLK3_OUT_PAD1_FREF_CLK4_REQ << reg_offset_gp) | (0<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define ad4_sys_drm_indicator   (2 | (CONTROL_WKUP_PAD0_FREF_CLK3_OUT_PAD1_FREF_CLK4_REQ << reg_offset_gp) | (0<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define ad4_gpio_wk31   (3 | (CONTROL_WKUP_PAD0_FREF_CLK3_OUT_PAD1_FREF_CLK4_REQ << reg_offset_gp) | (0<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define ad4_sdmmc2_dat5   (5 | (CONTROL_WKUP_PAD0_FREF_CLK3_OUT_PAD1_FREF_CLK4_REQ << reg_offset_gp) | (0<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define ad4_attila_hw_dbg8   (6 | (CONTROL_WKUP_PAD0_FREF_CLK3_OUT_PAD1_FREF_CLK4_REQ << reg_offset_gp) | (0<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define ad4_safe_mode   (7 | (CONTROL_WKUP_PAD0_FREF_CLK3_OUT_PAD1_FREF_CLK4_REQ << reg_offset_gp) | (0<<pad_indicator_bp) | pad_padconf_wkup_bm)

#define ac2_fref_clk4_req   (0 | (CONTROL_WKUP_PAD0_FREF_CLK3_OUT_PAD1_FREF_CLK4_REQ << reg_offset_gp) | (1<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define ac2_fref_clk5_out   (1 | (CONTROL_WKUP_PAD0_FREF_CLK3_OUT_PAD1_FREF_CLK4_REQ << reg_offset_gp) | (1<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define ac2_gpio_wk7   (3 | (CONTROL_WKUP_PAD0_FREF_CLK3_OUT_PAD1_FREF_CLK4_REQ << reg_offset_gp) | (1<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define ac2_sdmmc2_dat6   (5 | (CONTROL_WKUP_PAD0_FREF_CLK3_OUT_PAD1_FREF_CLK4_REQ << reg_offset_gp) | (1<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define ac2_attila_hw_dbg9   (6 | (CONTROL_WKUP_PAD0_FREF_CLK3_OUT_PAD1_FREF_CLK4_REQ << reg_offset_gp) | (1<<pad_indicator_bp) | pad_padconf_wkup_bm)

#define ac3_fref_clk4_out   (0 | (CONTROL_WKUP_PAD0_FREF_CLK4_OUT_PAD1_SYS_32K << reg_offset_gp) | (0<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define ac3_gpio_wk8   (3 | (CONTROL_WKUP_PAD0_FREF_CLK4_OUT_PAD1_SYS_32K << reg_offset_gp) | (0<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define ac3_attila_hw_dbg10   (6 | (CONTROL_WKUP_PAD0_FREF_CLK4_OUT_PAD1_SYS_32K << reg_offset_gp) | (0<<pad_indicator_bp) | pad_padconf_wkup_bm)

#define ag7_sys_32k   (0 | (CONTROL_WKUP_PAD0_FREF_CLK4_OUT_PAD1_SYS_32K << reg_offset_gp) | (1<<pad_indicator_bp) | pad_padconf_wkup_bm)

#define ae7_sys_nrespwron   (0 | (CONTROL_WKUP_PAD0_SYS_NRESPWRON_PAD1_SYS_NRESWARM << reg_offset_gp) | (0<<pad_indicator_bp) | pad_padconf_wkup_bm)

#define af7_sys_nreswarm   (0 | (CONTROL_WKUP_PAD0_SYS_NRESPWRON_PAD1_SYS_NRESWARM << reg_offset_gp) | (1<<pad_indicator_bp) | pad_padconf_wkup_bm)

#define ah7_sys_pwr_req   (0 | (CONTROL_WKUP_PAD0_SYS_PWR_REQ_PAD1_SYS_PWRON_RESET_OUT << reg_offset_gp) | (0<<pad_indicator_bp) | pad_padconf_wkup_bm)

#define ag6_sys_pwron_reset_out   (CONTROL_WKUP_PAD0_SYS_PWR_REQ_PAD1_SYS_PWRON_RESET_OUT | (0 << reg_offset_gp) | (1<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define ag6_gpio_wk29   (3 | (CONTROL_WKUP_PAD0_SYS_PWR_REQ_PAD1_SYS_PWRON_RESET_OUT << reg_offset_gp) | (1<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define ag6_attila_hw_dbd0   (5 | (CONTROL_WKUP_PAD0_SYS_PWR_REQ_PAD1_SYS_PWRON_RESET_OUT << reg_offset_gp) | (1<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define ag6_attila_hw_dbg9   (6 | (CONTROL_WKUP_PAD0_SYS_PWR_REQ_PAD1_SYS_PWRON_RESET_OUT << reg_offset_gp) | (1<<pad_indicator_bp) | pad_padconf_wkup_bm)

#define ae6_sys_nirq1   (0 | (CONTROL_CORE_PAD0_FREF_CLK2_OUT_PAD1_SYS_NIRQ1 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ae6_safe_mode   (7 | (CONTROL_CORE_PAD0_FREF_CLK2_OUT_PAD1_SYS_NIRQ1 << reg_offset_gp) | (1<<pad_indicator_bp))

#define af6_sys_nirq2   (0 | (CONTROL_CORE_PAD0_SYS_NIRQ2_PAD1_SYS_BOOT0 << reg_offset_gp) | (0<<pad_indicator_bp))
#define af6_gpio_183   (3 | (CONTROL_CORE_PAD0_SYS_NIRQ2_PAD1_SYS_BOOT0 << reg_offset_gp) | (0<<pad_indicator_bp))
#define af6_safe_mode   (7 | (CONTROL_CORE_PAD0_SYS_NIRQ2_PAD1_SYS_BOOT0 << reg_offset_gp) | (0<<pad_indicator_bp))

#define f26_sys_boot0   (0 | (CONTROL_CORE_PAD0_SYS_NIRQ2_PAD1_SYS_BOOT0 << reg_offset_gp) | (1<<pad_indicator_bp))
#define f26_gpio_184   (3 | (CONTROL_CORE_PAD0_SYS_NIRQ2_PAD1_SYS_BOOT0 << reg_offset_gp) | (1<<pad_indicator_bp))
#define f26_safe_mode   (7 | (CONTROL_CORE_PAD0_SYS_NIRQ2_PAD1_SYS_BOOT0 << reg_offset_gp) | (1<<pad_indicator_bp))

#define e27_sys_boot1   (0 | (CONTROL_CORE_PAD0_SYS_BOOT1_PAD1_SYS_BOOT2 << reg_offset_gp) | (0<<pad_indicator_bp))
#define e27_gpio_185   (3 | (CONTROL_CORE_PAD0_SYS_BOOT1_PAD1_SYS_BOOT2 << reg_offset_gp) | (0<<pad_indicator_bp))
#define e27_safe_mode   (7 | (CONTROL_CORE_PAD0_SYS_BOOT1_PAD1_SYS_BOOT2 << reg_offset_gp) | (0<<pad_indicator_bp))

#define e26_sys_boot2   (0 | (CONTROL_CORE_PAD0_SYS_BOOT1_PAD1_SYS_BOOT2 << reg_offset_gp) | (1<<pad_indicator_bp))
#define e26_gpio_186   (3 | (CONTROL_CORE_PAD0_SYS_BOOT1_PAD1_SYS_BOOT2 << reg_offset_gp) | (1<<pad_indicator_bp))
#define e26_safe_mode   (7 | (CONTROL_CORE_PAD0_SYS_BOOT1_PAD1_SYS_BOOT2 << reg_offset_gp) | (1<<pad_indicator_bp))

#define e25_sys_boot3   (0 | (CONTROL_CORE_PAD0_SYS_BOOT3_PAD1_SYS_BOOT4 << reg_offset_gp) | (0<<pad_indicator_bp))
#define e25_gpio_187   (3 | (CONTROL_CORE_PAD0_SYS_BOOT3_PAD1_SYS_BOOT4 << reg_offset_gp) | (0<<pad_indicator_bp))
#define e25_safe_mode   (7 | (CONTROL_CORE_PAD0_SYS_BOOT3_PAD1_SYS_BOOT4 << reg_offset_gp) | (0<<pad_indicator_bp))

#define d28_sys_boot4   (0 | (CONTROL_CORE_PAD0_SYS_BOOT3_PAD1_SYS_BOOT4 << reg_offset_gp) | (1<<pad_indicator_bp))
#define d28_gpio_188   (3 | (CONTROL_CORE_PAD0_SYS_BOOT3_PAD1_SYS_BOOT4 << reg_offset_gp) | (1<<pad_indicator_bp))
#define d28_safe_mode   (7 | (CONTROL_CORE_PAD0_SYS_BOOT3_PAD1_SYS_BOOT4 << reg_offset_gp) | (1<<pad_indicator_bp))

#define d27_sys_boot5   (0 | (CONTROL_CORE_PAD0_SYS_BOOT5_PAD1_DPM_EMU0 << reg_offset_gp) | (0<<pad_indicator_bp))
#define d27_gpio_189   (3 | (CONTROL_CORE_PAD0_SYS_BOOT5_PAD1_DPM_EMU0 << reg_offset_gp) | (0<<pad_indicator_bp))
#define d27_safe_mode   (7 | (CONTROL_CORE_PAD0_SYS_BOOT5_PAD1_DPM_EMU0 << reg_offset_gp) | (0<<pad_indicator_bp))

#define af8_sys_boot6   (0 | (CONTROL_WKUP_PAD0_SYS_BOOT6_PAD1_SYS_BOOT7 << reg_offset_gp) | (0<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define af8_dpm_emu18   (1 | (CONTROL_WKUP_PAD0_SYS_BOOT6_PAD1_SYS_BOOT7 << reg_offset_gp) | (0<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define af8_gpio_wk9   (3 | (CONTROL_WKUP_PAD0_SYS_BOOT6_PAD1_SYS_BOOT7 << reg_offset_gp) | (0<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define af8_attila_hw_dbg12   (6 | (CONTROL_WKUP_PAD0_SYS_BOOT6_PAD1_SYS_BOOT7 << reg_offset_gp) | (0<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define af8_safe_mode   (7 | (CONTROL_WKUP_PAD0_SYS_BOOT6_PAD1_SYS_BOOT7 << reg_offset_gp) | (0<<pad_indicator_bp) | pad_padconf_wkup_bm)

#define ae8_sys_boot7   (0 | (CONTROL_WKUP_PAD0_SYS_BOOT6_PAD1_SYS_BOOT7 << reg_offset_gp) | (1<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define ae8_dpm_emu19   (1 | (CONTROL_WKUP_PAD0_SYS_BOOT6_PAD1_SYS_BOOT7 << reg_offset_gp) | (1<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define ae8_gpio_wk10   (3 | (CONTROL_WKUP_PAD0_SYS_BOOT6_PAD1_SYS_BOOT7 << reg_offset_gp) | (1<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define ae8_attila_hw_dbg13   (6 | (CONTROL_WKUP_PAD0_SYS_BOOT6_PAD1_SYS_BOOT7 << reg_offset_gp) | (1<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define ae8_safe_mode   (7 | (CONTROL_WKUP_PAD0_SYS_BOOT6_PAD1_SYS_BOOT7 << reg_offset_gp) | (1<<pad_indicator_bp) | pad_padconf_wkup_bm)

#define ah2_jtag_ntrst   (0 | (CONTROL_WKUP_PAD0_JTAG_NTRST_PAD1_JTAG_TCK << reg_offset_gp) | (0<<pad_indicator_bp) | pad_padconf_wkup_bm)

#define ag1_jtag_tck   (0 | (CONTROL_WKUP_PAD0_JTAG_NTRST_PAD1_JTAG_TCK << reg_offset_gp) | (1<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define ag1_safe_mode   (7 | (CONTROL_WKUP_PAD0_JTAG_NTRST_PAD1_JTAG_TCK << reg_offset_gp) | (1<<pad_indicator_bp) | pad_padconf_wkup_bm)

#define ae3_jtag_rtck   (0 | (CONTROL_WKUP_PAD0_JTAG_RTCK_PAD1_JTAG_TMS_TMSC << reg_offset_gp) | (0<<pad_indicator_bp) | pad_padconf_wkup_bm)

#define ah1_jtag_tms_tmsc   (0 | (CONTROL_WKUP_PAD0_JTAG_RTCK_PAD1_JTAG_TMS_TMSC << reg_offset_gp) | (1<<pad_indicator_bp) | pad_padconf_wkup_bm)
#define ah1_safe_mode   (7 | (CONTROL_WKUP_PAD0_JTAG_RTCK_PAD1_JTAG_TMS_TMSC << reg_offset_gp) | (1<<pad_indicator_bp) | pad_padconf_wkup_bm)

#define ae1_jtag_tdi   (0 | (CONTROL_WKUP_PAD0_JTAG_TDI_PAD1_JTAG_TDO << reg_offset_gp) | (0<<pad_indicator_bp) | pad_padconf_wkup_bm)

#define ae2_jtag_tdo   (0 | (CONTROL_WKUP_PAD0_JTAG_TDI_PAD1_JTAG_TDO << reg_offset_gp) | (1<<pad_indicator_bp) | pad_padconf_wkup_bm)

#define m2_dpm_emu0   (0 | (CONTROL_CORE_PAD0_SYS_BOOT5_PAD1_DPM_EMU0 << reg_offset_gp) | (1<<pad_indicator_bp))
#define m2_gpio_11   (3 | (CONTROL_CORE_PAD0_SYS_BOOT5_PAD1_DPM_EMU0 << reg_offset_gp) | (1<<pad_indicator_bp))
#define m2_attila_hw_dbg0   (6 | (CONTROL_CORE_PAD0_SYS_BOOT5_PAD1_DPM_EMU0 << reg_offset_gp) | (1<<pad_indicator_bp))
#define m2_safe_mode   (7 | (CONTROL_CORE_PAD0_SYS_BOOT5_PAD1_DPM_EMU0 << reg_offset_gp) | (1<<pad_indicator_bp))

#define n2_dpm_emu1   (0 | (CONTROL_CORE_PAD0_DPM_EMU1_PAD1_DPM_EMU2 << reg_offset_gp) | (0<<pad_indicator_bp))
#define n2_gpio_12   (3 | (CONTROL_CORE_PAD0_DPM_EMU1_PAD1_DPM_EMU2 << reg_offset_gp) | (0<<pad_indicator_bp))
#define n2_attila_hw_dbg1   (6 | (CONTROL_CORE_PAD0_DPM_EMU1_PAD1_DPM_EMU2 << reg_offset_gp) | (0<<pad_indicator_bp))
#define n2_safe_mode   (7 | (CONTROL_CORE_PAD0_DPM_EMU1_PAD1_DPM_EMU2 << reg_offset_gp) | (0<<pad_indicator_bp))

#define p2_dpm_emu2   (0 | (CONTROL_CORE_PAD0_DPM_EMU1_PAD1_DPM_EMU2 << reg_offset_gp) | (1<<pad_indicator_bp))
#define p2_usba0_ulpiphy_clk   (1 | (CONTROL_CORE_PAD0_DPM_EMU1_PAD1_DPM_EMU2 << reg_offset_gp) | (1<<pad_indicator_bp))
#define p2_gpio_13   (3 | (CONTROL_CORE_PAD0_DPM_EMU1_PAD1_DPM_EMU2 << reg_offset_gp) | (1<<pad_indicator_bp))
#define p2_dispc2_fid   (5 | (CONTROL_CORE_PAD0_DPM_EMU1_PAD1_DPM_EMU2 << reg_offset_gp) | (1<<pad_indicator_bp))
#define p2_attila_hw_dbg2   (6 | (CONTROL_CORE_PAD0_DPM_EMU1_PAD1_DPM_EMU2 << reg_offset_gp) | (1<<pad_indicator_bp))
#define p2_safe_mode   (7 | (CONTROL_CORE_PAD0_DPM_EMU1_PAD1_DPM_EMU2 << reg_offset_gp) | (1<<pad_indicator_bp))

#define v1_dpm_emu3   (0 | (CONTROL_CORE_PAD0_DPM_EMU3_PAD1_DPM_EMU4 << reg_offset_gp) | (0<<pad_indicator_bp))
#define v1_usba0_ulpiphy_stp   (1 | (CONTROL_CORE_PAD0_DPM_EMU3_PAD1_DPM_EMU4 << reg_offset_gp) | (0<<pad_indicator_bp))
#define v1_gpio_14   (3 | (CONTROL_CORE_PAD0_DPM_EMU3_PAD1_DPM_EMU4 << reg_offset_gp) | (0<<pad_indicator_bp))
#define v1_rfbi_data10   (4 | (CONTROL_CORE_PAD0_DPM_EMU3_PAD1_DPM_EMU4 << reg_offset_gp) | (0<<pad_indicator_bp))
#define v1_dispc2_data10   (5 | (CONTROL_CORE_PAD0_DPM_EMU3_PAD1_DPM_EMU4 << reg_offset_gp) | (0<<pad_indicator_bp))
#define v1_attila_hw_dbg3   (6 | (CONTROL_CORE_PAD0_DPM_EMU3_PAD1_DPM_EMU4 << reg_offset_gp) | (0<<pad_indicator_bp))
#define v1_safe_mode   (7 | (CONTROL_CORE_PAD0_DPM_EMU3_PAD1_DPM_EMU4 << reg_offset_gp) | (0<<pad_indicator_bp))

#define v2_dpm_emu4   (0 | (CONTROL_CORE_PAD0_DPM_EMU3_PAD1_DPM_EMU4 << reg_offset_gp) | (1<<pad_indicator_bp))
#define v2_usba0_ulpiphy_dir   (1 | (CONTROL_CORE_PAD0_DPM_EMU3_PAD1_DPM_EMU4 << reg_offset_gp) | (1<<pad_indicator_bp))
#define v2_gpio_15   (3 | (CONTROL_CORE_PAD0_DPM_EMU3_PAD1_DPM_EMU4 << reg_offset_gp) | (1<<pad_indicator_bp))
#define v2_rfbi_data9   (4 | (CONTROL_CORE_PAD0_DPM_EMU3_PAD1_DPM_EMU4 << reg_offset_gp) | (1<<pad_indicator_bp))
#define v2_dispc2_data9   (5 | (CONTROL_CORE_PAD0_DPM_EMU3_PAD1_DPM_EMU4 << reg_offset_gp) | (1<<pad_indicator_bp))
#define v2_attila_hw_dbg4   (6 | (CONTROL_CORE_PAD0_DPM_EMU3_PAD1_DPM_EMU4 << reg_offset_gp) | (1<<pad_indicator_bp))
#define v2_safe_mode   (7 | (CONTROL_CORE_PAD0_DPM_EMU3_PAD1_DPM_EMU4 << reg_offset_gp) | (1<<pad_indicator_bp))

#define w1_dpm_emu5   (0 | (CONTROL_CORE_PAD0_DPM_EMU5_PAD1_DPM_EMU6 << reg_offset_gp) | (0<<pad_indicator_bp))
#define w1_usba0_ulpiphy_nxt   (1 | (CONTROL_CORE_PAD0_DPM_EMU5_PAD1_DPM_EMU6 << reg_offset_gp) | (0<<pad_indicator_bp))
#define w1_gpio_16   (3 | (CONTROL_CORE_PAD0_DPM_EMU5_PAD1_DPM_EMU6 << reg_offset_gp) | (0<<pad_indicator_bp))
#define w1_rfbi_te_vsync0   (4 | (CONTROL_CORE_PAD0_DPM_EMU5_PAD1_DPM_EMU6 << reg_offset_gp) | (0<<pad_indicator_bp))
#define w1_dispc2_data16   (5 | (CONTROL_CORE_PAD0_DPM_EMU5_PAD1_DPM_EMU6 << reg_offset_gp) | (0<<pad_indicator_bp))
#define w1_attila_hw_dbg5   (6 | (CONTROL_CORE_PAD0_DPM_EMU5_PAD1_DPM_EMU6 << reg_offset_gp) | (0<<pad_indicator_bp))
#define w1_safe_mode   (7 | (CONTROL_CORE_PAD0_DPM_EMU5_PAD1_DPM_EMU6 << reg_offset_gp) | (0<<pad_indicator_bp))

#define w2_dpm_emu6   (0 | (CONTROL_CORE_PAD0_DPM_EMU5_PAD1_DPM_EMU6 << reg_offset_gp) | (1<<pad_indicator_bp))
#define w2_usba0_ulpiphy_dat0   (1 | (CONTROL_CORE_PAD0_DPM_EMU5_PAD1_DPM_EMU6 << reg_offset_gp) | (1<<pad_indicator_bp))
#define w2_uart3_tx_irtx   (2 | (CONTROL_CORE_PAD0_DPM_EMU5_PAD1_DPM_EMU6 << reg_offset_gp) | (1<<pad_indicator_bp))
#define w2_gpio_17   (3 | (CONTROL_CORE_PAD0_DPM_EMU5_PAD1_DPM_EMU6 << reg_offset_gp) | (1<<pad_indicator_bp))
#define w2_rfbi_hsync0   (4 | (CONTROL_CORE_PAD0_DPM_EMU5_PAD1_DPM_EMU6 << reg_offset_gp) | (1<<pad_indicator_bp))
#define w2_dispc2_data17   (5 | (CONTROL_CORE_PAD0_DPM_EMU5_PAD1_DPM_EMU6 << reg_offset_gp) | (1<<pad_indicator_bp))
#define w2_attila_hw_dbg6   (6 | (CONTROL_CORE_PAD0_DPM_EMU5_PAD1_DPM_EMU6 << reg_offset_gp) | (1<<pad_indicator_bp))
#define w2_safe_mode   (7 | (CONTROL_CORE_PAD0_DPM_EMU5_PAD1_DPM_EMU6 << reg_offset_gp) | (1<<pad_indicator_bp))

#define w3_dpm_emu7   (0 | (CONTROL_CORE_PAD0_DPM_EMU7_PAD1_DPM_EMU8 << reg_offset_gp) | (0<<pad_indicator_bp))
#define w3_usba0_ulpiphy_dat1   (1 | (CONTROL_CORE_PAD0_DPM_EMU7_PAD1_DPM_EMU8 << reg_offset_gp) | (0<<pad_indicator_bp))
#define w3_uart3_rx_irrx   (2 | (CONTROL_CORE_PAD0_DPM_EMU7_PAD1_DPM_EMU8 << reg_offset_gp) | (0<<pad_indicator_bp))
#define w3_gpio_18   (3 | (CONTROL_CORE_PAD0_DPM_EMU7_PAD1_DPM_EMU8 << reg_offset_gp) | (0<<pad_indicator_bp))
#define w3_rfbi_cs0   (4 | (CONTROL_CORE_PAD0_DPM_EMU7_PAD1_DPM_EMU8 << reg_offset_gp) | (0<<pad_indicator_bp))
#define w3_dispc2_hsync   (5 | (CONTROL_CORE_PAD0_DPM_EMU7_PAD1_DPM_EMU8 << reg_offset_gp) | (0<<pad_indicator_bp))
#define w3_attila_hw_dbg7   (6 | (CONTROL_CORE_PAD0_DPM_EMU7_PAD1_DPM_EMU8 << reg_offset_gp) | (0<<pad_indicator_bp))
#define w3_safe_mode   (7 | (CONTROL_CORE_PAD0_DPM_EMU7_PAD1_DPM_EMU8 << reg_offset_gp) | (0<<pad_indicator_bp))

#define w4_dpm_emu8   (0 | (CONTROL_CORE_PAD0_DPM_EMU7_PAD1_DPM_EMU8 << reg_offset_gp) | (1<<pad_indicator_bp))
#define w4_usba0_ulpiphy_dat2   (1 | (CONTROL_CORE_PAD0_DPM_EMU7_PAD1_DPM_EMU8 << reg_offset_gp) | (1<<pad_indicator_bp))
#define w4_uart3_rts_sd   (2 | (CONTROL_CORE_PAD0_DPM_EMU7_PAD1_DPM_EMU8 << reg_offset_gp) | (1<<pad_indicator_bp))
#define w4_gpio_19   (3 | (CONTROL_CORE_PAD0_DPM_EMU7_PAD1_DPM_EMU8 << reg_offset_gp) | (1<<pad_indicator_bp))
#define w4_rfbi_re   (4 | (CONTROL_CORE_PAD0_DPM_EMU7_PAD1_DPM_EMU8 << reg_offset_gp) | (1<<pad_indicator_bp))
#define w4_dispc2_pclk   (5 | (CONTROL_CORE_PAD0_DPM_EMU7_PAD1_DPM_EMU8 << reg_offset_gp) | (1<<pad_indicator_bp))
#define w4_attila_hw_dbg8   (6 | (CONTROL_CORE_PAD0_DPM_EMU7_PAD1_DPM_EMU8 << reg_offset_gp) | (1<<pad_indicator_bp))
#define w4_safe_mode   (7 | (CONTROL_CORE_PAD0_DPM_EMU7_PAD1_DPM_EMU8 << reg_offset_gp) | (1<<pad_indicator_bp))

#define y2_dpm_emu9   (0 | (CONTROL_CORE_PAD0_DPM_EMU9_PAD1_DPM_EMU10 << reg_offset_gp) | (0<<pad_indicator_bp))
#define y2_usba0_ulpiphy_dat3   (1 | (CONTROL_CORE_PAD0_DPM_EMU9_PAD1_DPM_EMU10 << reg_offset_gp) | (0<<pad_indicator_bp))
#define y2_uart3_cts_rctx   (2 | (CONTROL_CORE_PAD0_DPM_EMU9_PAD1_DPM_EMU10 << reg_offset_gp) | (0<<pad_indicator_bp))
#define y2_gpio_20   (3 | (CONTROL_CORE_PAD0_DPM_EMU9_PAD1_DPM_EMU10 << reg_offset_gp) | (0<<pad_indicator_bp))
#define y2_rfbi_we   (4 | (CONTROL_CORE_PAD0_DPM_EMU9_PAD1_DPM_EMU10 << reg_offset_gp) | (0<<pad_indicator_bp))
#define y2_dispc2_vsync   (5 | (CONTROL_CORE_PAD0_DPM_EMU9_PAD1_DPM_EMU10 << reg_offset_gp) | (0<<pad_indicator_bp))
#define y2_attila_hw_dbg9   (6 | (CONTROL_CORE_PAD0_DPM_EMU9_PAD1_DPM_EMU10 << reg_offset_gp) | (0<<pad_indicator_bp))
#define y2_safe_mode   (7 | (CONTROL_CORE_PAD0_DPM_EMU9_PAD1_DPM_EMU10 << reg_offset_gp) | (0<<pad_indicator_bp))

#define y3_dpm_emu10   (0 | (CONTROL_CORE_PAD0_DPM_EMU9_PAD1_DPM_EMU10 << reg_offset_gp) | (1<<pad_indicator_bp))
#define y3_usba0_ulpiphy_dat4   (1 | (CONTROL_CORE_PAD0_DPM_EMU9_PAD1_DPM_EMU10 << reg_offset_gp) | (1<<pad_indicator_bp))
#define y3_gpio_21   (3 | (CONTROL_CORE_PAD0_DPM_EMU9_PAD1_DPM_EMU10 << reg_offset_gp) | (1<<pad_indicator_bp))
#define y3_rfbi_a0   (4 | (CONTROL_CORE_PAD0_DPM_EMU9_PAD1_DPM_EMU10 << reg_offset_gp) | (1<<pad_indicator_bp))
#define y3_dispc2_de   (5 | (CONTROL_CORE_PAD0_DPM_EMU9_PAD1_DPM_EMU10 << reg_offset_gp) | (1<<pad_indicator_bp))
#define y3_attila_hw_dbg10   (6 | (CONTROL_CORE_PAD0_DPM_EMU9_PAD1_DPM_EMU10 << reg_offset_gp) | (1<<pad_indicator_bp))
#define y3_safe_mode   (7 | (CONTROL_CORE_PAD0_DPM_EMU9_PAD1_DPM_EMU10 << reg_offset_gp) | (1<<pad_indicator_bp))

#define y4_dpm_emu11   (0 | (CONTROL_CORE_PAD0_DPM_EMU11_PAD1_DPM_EMU12 << reg_offset_gp) | (0<<pad_indicator_bp))
#define y4_usba0_ulpiphy_dat5   (1 | (CONTROL_CORE_PAD0_DPM_EMU11_PAD1_DPM_EMU12 << reg_offset_gp) | (0<<pad_indicator_bp))
#define y4_gpio_22   (3 | (CONTROL_CORE_PAD0_DPM_EMU11_PAD1_DPM_EMU12 << reg_offset_gp) | (0<<pad_indicator_bp))
#define y4_rfbi_data8   (4 | (CONTROL_CORE_PAD0_DPM_EMU11_PAD1_DPM_EMU12 << reg_offset_gp) | (0<<pad_indicator_bp))
#define y4_dispc2_data8   (5 | (CONTROL_CORE_PAD0_DPM_EMU11_PAD1_DPM_EMU12 << reg_offset_gp) | (0<<pad_indicator_bp))
#define y4_attila_hw_dbg11   (6 | (CONTROL_CORE_PAD0_DPM_EMU11_PAD1_DPM_EMU12 << reg_offset_gp) | (0<<pad_indicator_bp))
#define y4_safe_mode   (7 | (CONTROL_CORE_PAD0_DPM_EMU11_PAD1_DPM_EMU12 << reg_offset_gp) | (0<<pad_indicator_bp))

#define aa1_dpm_emu12   (0 | (CONTROL_CORE_PAD0_DPM_EMU11_PAD1_DPM_EMU12 << reg_offset_gp) | (1<<pad_indicator_bp))
#define aa1_usba0_ulpiphy_dat6   (1 | (CONTROL_CORE_PAD0_DPM_EMU11_PAD1_DPM_EMU12 << reg_offset_gp) | (1<<pad_indicator_bp))
#define aa1_gpio_23   (3 | (CONTROL_CORE_PAD0_DPM_EMU11_PAD1_DPM_EMU12 << reg_offset_gp) | (1<<pad_indicator_bp))
#define aa1_rfbi_data7   (4 | (CONTROL_CORE_PAD0_DPM_EMU11_PAD1_DPM_EMU12 << reg_offset_gp) | (1<<pad_indicator_bp))
#define aa1_dispc2_data7   (5 | (CONTROL_CORE_PAD0_DPM_EMU11_PAD1_DPM_EMU12 << reg_offset_gp) | (1<<pad_indicator_bp))
#define aa1_attila_hw_dbg12   (6 | (CONTROL_CORE_PAD0_DPM_EMU11_PAD1_DPM_EMU12 << reg_offset_gp) | (1<<pad_indicator_bp))
#define aa1_safe_mode   (7 | (CONTROL_CORE_PAD0_DPM_EMU11_PAD1_DPM_EMU12 << reg_offset_gp) | (1<<pad_indicator_bp))

#define aa2_dpm_emu13   (0 | (CONTROL_CORE_PAD0_DPM_EMU13_PAD1_DPM_EMU14 << reg_offset_gp) | (0<<pad_indicator_bp))
#define aa2_usba0_ulpiphy_dat7   (1 | (CONTROL_CORE_PAD0_DPM_EMU13_PAD1_DPM_EMU14 << reg_offset_gp) | (0<<pad_indicator_bp))
#define aa2_gpio_24   (3 | (CONTROL_CORE_PAD0_DPM_EMU13_PAD1_DPM_EMU14 << reg_offset_gp) | (0<<pad_indicator_bp))
#define aa2_rfbi_data6   (4 | (CONTROL_CORE_PAD0_DPM_EMU13_PAD1_DPM_EMU14 << reg_offset_gp) | (0<<pad_indicator_bp))
#define aa2_dispc2_data6   (5 | (CONTROL_CORE_PAD0_DPM_EMU13_PAD1_DPM_EMU14 << reg_offset_gp) | (0<<pad_indicator_bp))
#define aa2_attila_hw_dbg13   (6 | (CONTROL_CORE_PAD0_DPM_EMU13_PAD1_DPM_EMU14 << reg_offset_gp) | (0<<pad_indicator_bp))
#define aa2_safe_mode   (7 | (CONTROL_CORE_PAD0_DPM_EMU13_PAD1_DPM_EMU14 << reg_offset_gp) | (0<<pad_indicator_bp))

#define aa3_dpm_emu14   (0 | (CONTROL_CORE_PAD0_DPM_EMU13_PAD1_DPM_EMU14 << reg_offset_gp) | (1<<pad_indicator_bp))
#define aa3_sys_drm_msecure   (1 | (CONTROL_CORE_PAD0_DPM_EMU13_PAD1_DPM_EMU14 << reg_offset_gp) | (1<<pad_indicator_bp))
#define aa3_uart1_rx   (2 | (CONTROL_CORE_PAD0_DPM_EMU13_PAD1_DPM_EMU14 << reg_offset_gp) | (1<<pad_indicator_bp))
#define aa3_gpio_25   (3 | (CONTROL_CORE_PAD0_DPM_EMU13_PAD1_DPM_EMU14 << reg_offset_gp) | (1<<pad_indicator_bp))
#define aa3_rfbi_data5   (4 | (CONTROL_CORE_PAD0_DPM_EMU13_PAD1_DPM_EMU14 << reg_offset_gp) | (1<<pad_indicator_bp))
#define aa3_dispc2_data5   (5 | (CONTROL_CORE_PAD0_DPM_EMU13_PAD1_DPM_EMU14 << reg_offset_gp) | (1<<pad_indicator_bp))
#define aa3_attila_hw_dbg14   (6 | (CONTROL_CORE_PAD0_DPM_EMU13_PAD1_DPM_EMU14 << reg_offset_gp) | (1<<pad_indicator_bp))
#define aa3_safe_mode   (7 | (CONTROL_CORE_PAD0_DPM_EMU13_PAD1_DPM_EMU14 << reg_offset_gp) | (1<<pad_indicator_bp))

#define aa4_dpm_emu15   (0 | (CONTROL_CORE_PAD0_DPM_EMU15_PAD1_DPM_EMU16 << reg_offset_gp) | (0<<pad_indicator_bp))
#define aa4_sys_drm_indicator   (1 | (CONTROL_CORE_PAD0_DPM_EMU15_PAD1_DPM_EMU16 << reg_offset_gp) | (0<<pad_indicator_bp))
#define aa4_gpio_26   (3 | (CONTROL_CORE_PAD0_DPM_EMU15_PAD1_DPM_EMU16 << reg_offset_gp) | (0<<pad_indicator_bp))
#define aa4_rfbi_data4   (4 | (CONTROL_CORE_PAD0_DPM_EMU15_PAD1_DPM_EMU16 << reg_offset_gp) | (0<<pad_indicator_bp))
#define aa4_dispc2_data4   (5 | (CONTROL_CORE_PAD0_DPM_EMU15_PAD1_DPM_EMU16 << reg_offset_gp) | (0<<pad_indicator_bp))
#define aa4_attila_hw_dbg15   (6 | (CONTROL_CORE_PAD0_DPM_EMU15_PAD1_DPM_EMU16 << reg_offset_gp) | (0<<pad_indicator_bp))
#define aa4_safe_mode   (7 | (CONTROL_CORE_PAD0_DPM_EMU15_PAD1_DPM_EMU16 << reg_offset_gp) | (0<<pad_indicator_bp))

#define ab2_dpm_emu16   (0 | (CONTROL_CORE_PAD0_DPM_EMU15_PAD1_DPM_EMU16 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ab2_dmtimer8_pwm_evt   (1 | (CONTROL_CORE_PAD0_DPM_EMU15_PAD1_DPM_EMU16 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ab2_dsi1_te0   (2 | (CONTROL_CORE_PAD0_DPM_EMU15_PAD1_DPM_EMU16 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ab2_gpio_27   (3 | (CONTROL_CORE_PAD0_DPM_EMU15_PAD1_DPM_EMU16 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ab2_rfbi_data3   (4 | (CONTROL_CORE_PAD0_DPM_EMU15_PAD1_DPM_EMU16 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ab2_dispc2_data3   (5 | (CONTROL_CORE_PAD0_DPM_EMU15_PAD1_DPM_EMU16 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ab2_attila_hw_dbg16   (6 | (CONTROL_CORE_PAD0_DPM_EMU15_PAD1_DPM_EMU16 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ab2_safe_mode   (7 | (CONTROL_CORE_PAD0_DPM_EMU15_PAD1_DPM_EMU16 << reg_offset_gp) | (1<<pad_indicator_bp))

#define ab3_dpm_emu17   (0 | (CONTROL_CORE_PAD0_DPM_EMU17_PAD1_DPM_EMU18 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ab3_dmtimer9_pwm_evt   (1 | (CONTROL_CORE_PAD0_DPM_EMU17_PAD1_DPM_EMU18 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ab3_dsi1_te1   (2 | (CONTROL_CORE_PAD0_DPM_EMU17_PAD1_DPM_EMU18 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ab3_gpio_28   (3 | (CONTROL_CORE_PAD0_DPM_EMU17_PAD1_DPM_EMU18 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ab3_rfbi_data2   (4 | (CONTROL_CORE_PAD0_DPM_EMU17_PAD1_DPM_EMU18 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ab3_dispc2_data2   (5 | (CONTROL_CORE_PAD0_DPM_EMU17_PAD1_DPM_EMU18 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ab3_attila_hw_dbg17   (6 | (CONTROL_CORE_PAD0_DPM_EMU17_PAD1_DPM_EMU18 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ab3_safe_mode   (7 | (CONTROL_CORE_PAD0_DPM_EMU17_PAD1_DPM_EMU18 << reg_offset_gp) | (0<<pad_indicator_bp))

#define ab4_dpm_emu18   (0 | (CONTROL_CORE_PAD0_DPM_EMU17_PAD1_DPM_EMU18 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ab4_dmtimer10_pwm_evt   (1 | (CONTROL_CORE_PAD0_DPM_EMU17_PAD1_DPM_EMU18 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ab4_dsi2_te0   (2 | (CONTROL_CORE_PAD0_DPM_EMU17_PAD1_DPM_EMU18 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ab4_gpio_190   (3 | (CONTROL_CORE_PAD0_DPM_EMU17_PAD1_DPM_EMU18 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ab4_rfbi_data1   (4 | (CONTROL_CORE_PAD0_DPM_EMU17_PAD1_DPM_EMU18 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ab4_dispc2_data1   (5 | (CONTROL_CORE_PAD0_DPM_EMU17_PAD1_DPM_EMU18 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ab4_attila_hw_dbg18   (6 | (CONTROL_CORE_PAD0_DPM_EMU17_PAD1_DPM_EMU18 << reg_offset_gp) | (1<<pad_indicator_bp))
#define ab4_safe_mode   (7 | (CONTROL_CORE_PAD0_DPM_EMU17_PAD1_DPM_EMU18 << reg_offset_gp) | (1<<pad_indicator_bp))

#define ac4_dpm_emu19   (0 | (CONTROL_CORE_PAD0_DPM_EMU19_PAD1_CSI22_DX2 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ac4_dmtimer11_pwm_evt   (1 | (CONTROL_CORE_PAD0_DPM_EMU19_PAD1_CSI22_DX2 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ac4_dsi2_te1   (2 | (CONTROL_CORE_PAD0_DPM_EMU19_PAD1_CSI22_DX2 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ac4_gpio_191   (3 | (CONTROL_CORE_PAD0_DPM_EMU19_PAD1_CSI22_DX2 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ac4_rfbi_data0   (4 | (CONTROL_CORE_PAD0_DPM_EMU19_PAD1_CSI22_DX2 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ac4_dispc2_data0   (5 | (CONTROL_CORE_PAD0_DPM_EMU19_PAD1_CSI22_DX2 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ac4_attila_hw_dbg19   (6 | (CONTROL_CORE_PAD0_DPM_EMU19_PAD1_CSI22_DX2 << reg_offset_gp) | (0<<pad_indicator_bp))
#define ac4_safe_mode   (7 | (CONTROL_CORE_PAD0_DPM_EMU19_PAD1_CSI22_DX2 << reg_offset_gp) | (0<<pad_indicator_bp))

void cm_wakeupevent_set(unsigned int pin_function);
void cm_wakeupevent_clear(unsigned int pin_function);
void cm_wakeupenable_set(unsigned int pin_function);
void cm_wakeupenable_clear(unsigned int pin_function);
void cm_offmodepulltypeselect_set(unsigned int pin_function);
void cm_offmodepulltypeselect_clear(unsigned int pin_function);
void cm_offmodepulludenable_set(unsigned int pin_function);
void cm_offmodepulludenable_clear(unsigned int pin_function);
void cm_offmodeoutvalue_set(unsigned int pin_function);
void cm_offmodeoutvalue_clear(unsigned int pin_function);
void cm_offmodeoutenable_set(unsigned int pin_function);
void cm_offmodeoutenable_clear(unsigned int pin_function);
void cm_offmodeenable_set(unsigned int pin_function);
void cm_offmodeenable_clear(unsigned int pin_function);
void cm_inputenable_set(unsigned int pin_function);
void cm_inputenable_clear(unsigned int pin_function);
void cm_pulltypeselect_set(unsigned int pin_function);
void cm_pulltypeselect_clear(unsigned int pin_function);
void cm_pulludenable_set(unsigned int pin_function);
void cm_pulludenable_clear(unsigned int pin_function);
void cm_muxmode_set(unsigned int pin_function);
void cm_pin_all_set(unsigned int pin_function, signed char pulludenable, signed char pulltypeselect,
		signed char inputenable, signed char offmodeenable, signed char offmodeoutenable, signed char offmodeoutvalue,
			signed char offmodepulludenable, signed char offmodepulltypeselect, signed char wakeupenable, signed char wakeupevent);
void cm_pin_set(unsigned int pin_function, signed char pulludenable, signed char pulltypeselect,
		signed char inputenable);

#ifdef HEADER_INCLUDE_C_FILES
#include "control_module.c"
#endif
#endif /* PIN_MUX_GPIO_DEF_H_ */
