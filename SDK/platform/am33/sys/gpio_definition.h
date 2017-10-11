/*
 * gpio_definition.h
 *
 *  Created on: Dec 2, 2012
 *      Author: XxXx
 */

#ifndef GPIO_DEFINITION_H_
#define GPIO_DEFINITION_H_


typedef volatile unsigned int GPIO_REGISTER;

typedef enum gpio_pins_bp_s
{
	GPIO_0_bp = 0,
	GPIO_1_bp,
	GPIO_2_bp,
	GPIO_3_bp,
	GPIO_4_bp,
	GPIO_5_bp,
	GPIO_6_bp,
	GPIO_7_bp,
	GPIO_8_bp,
	GPIO_9_bp,
	GPIO_10_bp,
	GPIO_11_bp,
	GPIO_12_bp,
	GPIO_13_bp,
	GPIO_14_bp,
	GPIO_15_bp,
	GPIO_16_bp,
	GPIO_17_bp,
	GPIO_18_bp,
	GPIO_19_bp,
	GPIO_20_bp,
	GPIO_21_bp,
	GPIO_22_bp,
	GPIO_23_bp,
	GPIO_24_bp,
	GPIO_25_bp,
	GPIO_26_bp,
	GPIO_27_bp,
	GPIO_28_bp,
	GPIO_29_bp,
	GPIO_30_bp,
	GPIO_31_bp
}gpio_pins_bp_t;

typedef enum gpio_pins_bm_s
{
	GPIO_0_bm = 1<<GPIO_0_bp,
	GPIO_1_bm = 1<<GPIO_1_bp,
	GPIO_2_bm = 1<<GPIO_2_bp,
	GPIO_3_bm = 1<<GPIO_3_bp,
	GPIO_4_bm = 1<<GPIO_4_bp,
	GPIO_5_bm = 1<<GPIO_5_bp,
	GPIO_6_bm = 1<<GPIO_6_bp,
	GPIO_7_bm = 1<<GPIO_7_bp,
	GPIO_8_bm = 1<<GPIO_8_bp,
	GPIO_9_bm = 1<<GPIO_9_bp,
	GPIO_10_bm = 1<<GPIO_10_bp,
	GPIO_11_bm = 1<<GPIO_11_bp,
	GPIO_12_bm = 1<<GPIO_12_bp,
	GPIO_13_bm = 1<<GPIO_13_bp,
	GPIO_14_bm = 1<<GPIO_14_bp,
	GPIO_15_bm = 1<<GPIO_15_bp,
	GPIO_16_bm = 1<<GPIO_16_bp,
	GPIO_17_bm = 1<<GPIO_17_bp,
	GPIO_18_bm = 1<<GPIO_18_bp,
	GPIO_19_bm = 1<<GPIO_19_bp,
	GPIO_20_bm = 1<<GPIO_20_bp,
	GPIO_21_bm = 1<<GPIO_21_bp,
	GPIO_22_bm = 1<<GPIO_22_bp,
	GPIO_23_bm = 1<<GPIO_23_bp,
	GPIO_24_bm = 1<<GPIO_24_bp,
	GPIO_25_bm = 1<<GPIO_25_bp,
	GPIO_26_bm = 1<<GPIO_26_bp,
	GPIO_27_bm = 1<<GPIO_27_bp,
	GPIO_28_bm = 1<<GPIO_28_bp,
	GPIO_29_bm = 1<<GPIO_29_bp,
	GPIO_30_bm = 1<<GPIO_30_bp,
	GPIO_31_bm = 1<<GPIO_31_bp
}gpio_pins_bm_t;

typedef struct _gpio_s
{
	GPIO_REGISTER REVISION;			/* 0x00  Module Revision Register */
	GPIO_REGISTER RESERVED_0x4_TO_0x0C[(0x10 - 0x4) / sizeof(GPIO_REGISTER)];
	GPIO_REGISTER SYSCONFIG;		/* 0x10  */
	GPIO_REGISTER RESERVED_0x14_TO_0x20[(0x24 - 0x14) / sizeof(GPIO_REGISTER)];
	GPIO_REGISTER IRQSTATUS_RAW_0;	/* 0x24  */
	GPIO_REGISTER IRQSTATUS_RAW_1;	/* 0x28  */
	GPIO_REGISTER IRQSTATUS_0;		/* 0x2C  */
	GPIO_REGISTER IRQSTATUS_1;		/* 0x30  */
	GPIO_REGISTER IRQSTATUS_SET_0;	/* 0x34  */
	GPIO_REGISTER IRQSTATUS_SET_1;	/* 0x38  */
	GPIO_REGISTER IRQSTATUS_CLR_0;	/* 0x3C  */
	GPIO_REGISTER IRQSTATUS_CLR_1;	/* 0x40  */
	GPIO_REGISTER IRQWAKEN_0;		/* 0x44  */
	GPIO_REGISTER RQWAKEN_1;		/* 0x48  */
	GPIO_REGISTER RESERVED_0x4C_TO_0x110[(0x114 - 0x4C) / sizeof(GPIO_REGISTER)];
	GPIO_REGISTER SYSSTATUS;		/* 0x114 */
	GPIO_REGISTER RESERVED_0x130_TO_0x114[(0x140 - 0x118) / sizeof(GPIO_REGISTER)];
	GPIO_REGISTER CTRL;				/* 0x130 */
	GPIO_REGISTER OE;				/* 0x134 */
	GPIO_REGISTER DATAIN;			/* 0x138 */
	GPIO_REGISTER DATAOUT;			/* 0x13C */
	GPIO_REGISTER LEVELDETECT0;		/* 0x140 */
	GPIO_REGISTER LEVELDETECT1;		/* 0x144 */
	GPIO_REGISTER RISINGDETECT;		/* 0x148 */
	GPIO_REGISTER FALLINGDETECT;	/* 0x14C */
	GPIO_REGISTER DEBOUNCENABLE;	/* 0x150 */
	GPIO_REGISTER DEBOUNCINGTIME;	/* 0x154 */
	GPIO_REGISTER RESERVED_0x158_TO_0x18C[(0x190 - 0x158) / sizeof(GPIO_REGISTER)];
	GPIO_REGISTER CLEARDATAOUT;		/* 0x190 */
	GPIO_REGISTER SETDATAOUT;		/* 0x194 */
}_gpio_t;

#define GPIO0_addr (0x44E07000)
#define GPIO0 (*(gpio_t *) 0x44E07000)

#define GPIO1_addr (0x4804C000)
#define GPIO1 (*(gpio_t *) 0x4804C000)

#define GPIO2_addr (0x481AC000)
#define GPIO2 (*(gpio_t *) 0x481AC000)

#define GPIO3_addr (0x481AE000)
#define GPIO3 (*(gpio_t *) 0x481AE000)


#endif /* GPIO_DEFINITION_H_ */
