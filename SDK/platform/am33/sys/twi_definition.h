/*
 * i2c_definitions.h
 *
 *  Created on: Dec 3, 2012
 *      Author: XxXx
 */

#ifndef I2C_DEFINITIONS_H_
#define I2C_DEFINITIONS_H_

typedef volatile unsigned int I2C_REGISTER;

typedef struct i2c_s
{
	I2C_REGISTER REVNB_LO;			//0x00 Module Revision Register (LOW BYTES)
	I2C_REGISTER REVNB_HI;			//0x04 Module Revision Register (HIGH BYTES)
	I2C_REGISTER RESERVED_0x08_TO_0xC[(0x10 - 0x8) / sizeof(I2C_REGISTER)];
	I2C_REGISTER SYSC;				//0x10 System Configuration Register
	I2C_REGISTER RESERVED_0x14_TO_0x20[(0x24 - 0x14) / sizeof(I2C_REGISTER)];
	I2C_REGISTER IRQSTATUS_RAW;		//0x24 I2C Status Raw Register
	I2C_REGISTER IRQSTATUS;			//0x28 I2C Status Register
	I2C_REGISTER IRQENABLE_SET;		//0x2C I2C Interrupt Enable Set Register
	I2C_REGISTER IRQENABLE_CLR;		//0x30 I2C Interrupt Enable Clear Register
	I2C_REGISTER WE;				//0x34 I2C Wakeup Enable Register
	I2C_REGISTER DMARXENABLE_SET;	//0x38 Receive DMA Enable Set Register
	I2C_REGISTER DMATXENABLE_SET;	//0x3C Transmit DMA Enable Set Register
	I2C_REGISTER DMARXENABLE_CLR;	//0x40 Receive DMA Enable Clear Register
	I2C_REGISTER DMATXENABLE_CLR;	//0x44 Transmit DMA Enable Clear Register
	I2C_REGISTER DMARXWAKE_EN;		//0x48 Receive DMA Wakeup Register
	I2C_REGISTER DMATXWAKE_EN;		//0x5C Transmit DMA Wakeup Register
	I2C_REGISTER RESERVED_0x50_TO_0x8C[(0x90 - 0x50) / sizeof(I2C_REGISTER)];
	I2C_REGISTER SYSS;				//0x90 System Status Register
	I2C_REGISTER BUF;				//0x94 Buffer Configuration Register
	I2C_REGISTER CNT;				//0x98 Data Counter Register
	I2C_REGISTER DATA;				//0x9C Data Access Register
	I2C_REGISTER RESERVED_0xA0;
	I2C_REGISTER CON;				//0xA4 I2C Configuration Register
	I2C_REGISTER OA;				//0xA8 I2C Own Address Register
	I2C_REGISTER SA;				//0xAC I2C Slave Address Register
	I2C_REGISTER PSC;				//0xB0 I2C Clock Prescaler Register
	I2C_REGISTER SCLL;				//0xB4 I2C SCL Low Time Register
	I2C_REGISTER SCLH;				//0xB8 I2C SCL High Time Register
	I2C_REGISTER SYSTEST;			//0xBC System Test Register
	I2C_REGISTER BUFSTAT;			//0xC0 I2C Buffer Status Register
	I2C_REGISTER OA1;				//0xC4 I2C Own Address 1 Register
	I2C_REGISTER OA2;				//0xC8 I2C Own Address 2 Register
	I2C_REGISTER OA3;				//0xCC I2C Own Address 3 Register
	I2C_REGISTER ACTOA;				//0xD0 Active Own Address Register
	I2C_REGISTER SBLOCK;			//0xD4 I2C Clock Blocking Enable Register
}i2c_t;

#define I2C0_addr (0x44E0B000)
#define I2C0 (*(i2c_t *) 0x44E0B000)

#define I2C1_addr (0x4802A000)
#define I2C1 (*(i2c_t *) 0x4802A000)

#define I2C2_addr (0x4819C000)
#define I2C2 (*(i2c_t *) 0x4819C000)

#endif /* I2C_DEFINITIONS_H_ */
