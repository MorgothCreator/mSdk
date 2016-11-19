/*
 * cc1101.h
 *
 *  Created on: Jun 30, 2016
 *      Author: John Smith
 */

#ifndef LIB_DEVICE_CC1101_H_
#define LIB_DEVICE_CC1101_H_

#include <api/spi_def.h>
#include <stdbool.h>
#include "api/timer_api.h"

#ifndef ST
/**
 *  This macro is for use by other macros to form a fully valid C statement.
 *  Without this, the if/else conditionals could show unexpected behavior.
 *
 *  For example, use...
 *    #define SET_REGS()  st( ioreg1 = 0; ioreg2 = 0; )
 *  instead of ...
 *    #define SET_REGS()  { ioreg1 = 0; ioreg2 = 0; }
 *  or
 *    #define  SET_REGS()    ioreg1 = 0; ioreg2 = 0;
 *  The last macro would not behave as expected in the if/else construct.
 *  The second to last macro will cause a compiler error in certain uses
 *  of if/else construct
 *
 *  It is not necessary, or recommended, to use this macro where there is
 *  already a valid C statement.  For example, the following is redundant...
 *    #define CALL_FUNC()   st(  func();  )
 *  This should simply be...
 *    #define CALL_FUNC()   func()
 *
 * (The while condition below evaluates false without generating a
 *  constant-controlling-loop type of warning on most compilers.)
 */
#define ST(X) do { X } while (0)
#endif

#ifndef ABS
#define ABS(a) ((a) < 0 ? -(a) : (a))
#endif

// Supported chip information
#define CC1101                1
#define CC1101_CHIPPARTNUM    0x00u
#define CC1101_CHIPVERSION    0x04u
#define CC110L                2
#define CC110L_CHIPPARTNUM    0x00u
#define CC110L_CHIPVERSION    0x07u
#define CC2500                3
#define CC2500_CHIPPARTNUM    0x80u
#define CC2500_CHIPVERSION    0x03u

// Command strobes
#define CC1101_SRES           0x30u // Reset chip
#define CC1101_SFSTXON        0x31u // Enable and calibrate frequency synthesizer
#define CC1101_SXOFF          0x32u // Turn off crystal oscillator
#define CC1101_SCAL           0x33u // Calibrate frequency synthesizer and turn it off
#define CC1101_SRX            0x34u // Enable rx
#define CC1101_STX            0x35u // In IDLE state: enable TX
#define CC1101_SIDLE          0x36u // Exit RX/TX, turn off frequency synthesizer
// Note: CC1101 datasheet skips register at 0x37u. Use is unavailable.
#define CC1101_SWOR           0x38u // Start automatic RX polling sequence
#define CC1101_SPWD           0x39u // Enter power down mode when CSn goes high
#define CC1101_SFRX           0x3Au // Flush the RX FIFO buffer
#define CC1101_SFTX           0x3Bu // Flush the TX FIFO buffer
#define CC1101_SWORRST        0x3Cu // Reset real time clock to Event1 value
#define CC1101_SNOP           0x3Du // No operation

// Status registers
#define CC1101_PARTNUM        0x30u // Chip ID
#define CC1101_VERSION        0x31u // Chip ID
#define CC1101_FREQEST        0x32u // Frequency offset estimate from demodulator
#define CC1101_LQI            0x33u // Demodulator estimate for link quality
#define CC1101_RSSI           0x34u // Received signal strength indication
#define CC1101_MARCSTATE      0x35u // Main radio control state machine state
#define CC1101_WORTIME1       0x36u // High byte of WOR time
#define CC1101_WORTIME0       0x37u // Low byte of WOR time
#define CC1101_PKTSTATUS      0x38u // Current GDOx status and packet status
#define CC1101_VCO_VC_DAC     0x39u // Current setting from PLL calibration module
#define CC1101_TXBYTES        0x3Au // Underflow and number of bytes
#define CC1101_RXBYTES        0x3Bu // Overflow and number of bytes
#define CC1101_RCCTRL1_STATUS 0x3Cu // Last RC oscillator calibration result
#define CC1101_RCCTRL0_STATUS 0x3Du // Last RC oscillator calibration result

// PA table register
#define CC1101_PATABLE        0x3Eu // RF output power level

// RX/TX FIFO registers
#define CC1101_RXFIFO         0x3Fu // Receive FIFO buffer (read-only)
#define CC1101_TXFIFO         0x3Fu // Transmit FIFO buffer (write-only)

// -----------------------------------------------------------------------------

// Configuration register addresses
/**
 *  Note: For the CC110L, some of the following registers are reserved. Please
 *  see the CC110L Value Line Transceiver User's Guide (swrs109a) for more
 *  information.
 */
#define CC1101_REG_IOCFG2         0x00u // GDO2 Output Pin Configuration
#define CC1101_REG_IOCFG1         0x01u // GDO1 Output Pin Configuration
#define CC1101_REG_IOCFG0         0x02u // GDO0 Output Pin Configuration
#define CC1101_REG_FIFOTHR        0x03u // RX FIFO and TX FIFO Thresholds
#define CC1101_REG_SYNC1          0x04u // Sync Word, High Byte
#define CC1101_REG_SYNC0          0x05u // Sync Word, Low Byte
#define CC1101_REG_PKTLEN         0x06u // Packet Length
#define CC1101_REG_PKTCTRL1       0x07u // Packet Automation Control
#define CC1101_REG_PKTCTRL0       0x08u // Packet Automation Control
#define CC1101_REG_ADDR           0x09u // Device Address
#define CC1101_REG_CHANNR         0x0Au // Channel Number
#define CC1101_REG_FSCTRL1        0x0Bu // Frequency Synthesizer Control
#define CC1101_REG_FSCTRL0        0x0Cu // Frequency Synthesizer Control
#define CC1101_REG_FREQ2          0x0Du // Frequency Control Word, High Byte
#define CC1101_REG_FREQ1          0x0Eu // Frequency Control Word, Middle Byte
#define CC1101_REG_FREQ0          0x0Fu // Frequency Control Word, Low Byte
#define CC1101_REG_MDMCFG4        0x10u // Modem Configuration
#define CC1101_REG_MDMCFG3        0x11u // Modem Configuration
#define CC1101_REG_MDMCFG2        0x12u // Modem Configuration
#define CC1101_REG_MDMCFG1        0x13u // Modem Configuration
#define CC1101_REG_MDMCFG0        0x14u // Modem Configuration
#define CC1101_REG_DEVIATN        0x15u // Modem Deviation Setting
#define CC1101_REG_MCSM2          0x16u // Main Radio Control State Machine Configuration
#define CC1101_REG_MCSM1          0x17u // Main Radio Control State Machine Configuration
#define CC1101_REG_MCSM0          0x18u // Main Radio Control State Machine Configuration
#define CC1101_REG_FOCCFG         0x19u // Frequency Offset Compensation Configuration
#define CC1101_REG_BSCFG          0x1Au // Bit Synchronization Configuration
#define CC1101_REG_AGCCTRL2       0x1Bu // AGC Control
#define CC1101_REG_AGCCTRL1       0x1Cu // AGC Control
#define CC1101_REG_AGCCTRL0       0x1Du // AGC Control
#define CC1101_REG_WOREVT1        0x1Eu // High Byte Event0 Timeout
#define CC1101_REG_WOREVT0        0x1Fu // Low Byte Event0 Timeout
#define CC1101_REG_WORCTRL        0x20u // Wake On Radio Control
#define CC1101_REG_FREND1         0x21u // Front End RX Configuration
#define CC1101_REG_FREND0         0x22u // Front End TX Configuration
#define CC1101_REG_FSCAL3         0x23u // Frequency Synthesizer Calibration
#define CC1101_REG_FSCAL2         0x24u // Frequency Synthesizer Calibration
#define CC1101_REG_FSCAL1         0x25u // Frequency Synthesizer Calibration
#define CC1101_REG_FSCAL0         0x26u // Frequency Synthesizer Calibration
#define CC1101_REG_RCCTRL1        0x27u // RC Oscillator Configuration
#define CC1101_REG_RCCTRL0        0x28u // RC Oscillator Configuration
#define CC1101_REG_FSTEST         0x29u // Frequency Synthesizer Calibration Control
#define CC1101_REG_PTEST          0x2Au // Production Test
#define CC1101_REG_AGCTEST        0x2Bu // AGC Test
#define CC1101_REG_TEST2          0x2Cu // Various Test Settings
#define CC1101_REG_TEST1          0x2Du // Various Test Settings
#define CC1101_REG_TEST0          0x2Eu // Various Test Settings

// -----------------------------------------------------------------------------

// Configuration register field masks
// IOCFG2
#define CC1101_GDO2_INV                   0x40u
#define CC1101_GDO2_CFG                   0x3Fu
// IOCFG1
#define CC1101_GDO1_DS                    0x80u
#define CC1101_GDO1_INV                   0x40u
#define CC1101_GDO1_CFG                   0x3Fu
// IOCFG0
#define CC1101_GDO0_TEMP_SENSOR_ENABLE    0x80u
#define CC1101_GDO0_INV                   0x40u
#define CC1101_GDO0_CFG                   0x3Fu
// FIFOTHR
#define CC1101_ADC_RETENTION              0x40u
#define CC1101_CLOSE_IN_RX                0x30u
#define CC1101_FIFO_THR                   0x0Fu
// SYNC1
#define CC1101_SYNC_MSB                   0xFFu
// SYNC0
#define CC1101_SYNC_LSB                   0xFFu
// PKTLEN
#define CC1101_PACKET_LENGTH              0xFFu
// PKTCTRL1
#define CC1101_PQT                        0xE0u
#define CC1101_CRC_AUTOFLUSH              0x08u
#define CC1101_APPEND_STATUS              0x04u
#define CC1101_ADR_CHK                    0x03u
// PKTCTRL0
#define CC1101_WHITE_DATA                 0x40u
#define CC1101_PKT_FORMAT                 0x30u
#define CC1101_CRC_EN                     0x04u
#define CC1101_LENGTH_CONFIG              0x03u
// ADDR
#define CC1101_DEVICE_ADDR                0xFFu
// CHANNR
#define CC1101_CHANNR_CHAN                0xFFu
// FSCTRL1
#define CC1101_FREQ_IF                    0x1Fu
// FSCTRL0
#define CC1101_FREQOFF                    0xFFu
// FREQ2
#define CC1101_FREQ_23_22                 0xC0u
#define CC1101_FREQ_21_16                 0x3Fu
// FREQ1
#define CC1101_FREQ_15_8                  0xFFu
// FREQ0
#define CC1101_FREQ_7_0                   0xFFu
// MDMCFG4
#define CC1101_CHANBW_E                   0xC0u
#define CC1101_CHANBW_M                   0x30u
#define CC1101_DRATE_E                    0x0Fu
// MDMCFG3
#define CC1101_DRATE_M                    0xFFu
// MDMCFG2
#define CC1101_DEM_DCFILT_OFF             0x80u
#define CC1101_MOD_FORMAT                 0x70u
#define CC1101_MANCHESTER_EN              0x08u
#define CC1101_SYNC_MODE                  0x07u
// MDMCFG1
#define CC1101_FEC_EN                     0x80u
#define CC1101_NUM_PREAMBLE               0x70u
#define CC1101_CHANSPC_E                  0x03u
// MDMCFG0
#define CC1101_CHANSPC_M                  0xFFu
// DEVIATN
#define CC1101_DEVIATION_E                0x70u
#define CC1101_DEVIATION_M                0x07u
// MCSM2
#define CC1101_RX_TIME_RSSI               0x10u
#define CC1101_RX_TIME_QUAL               0x08u
#define CC1101_RX_TIME                    0x07u
// MCSM1
#define CC1101_CCA_MODE                   0x30u
#define CC1101_RXOFF_MODE                 0x0Cu
#define CC1101_TXOFF_MODE                 0x03u
// MCSM0
#define CC1101_FS_AUTOCAL                 0x30u
#define CC1101_PO_TIMEOUT                 0x0Cu
#define CC1101_PIN_CTRL_EN                0x02u
#define CC1101_XOSC_FORCE_ON              0x01u
// FOCCFG
#define CC1101_FOC_BS_CS_GATE             0x20u
#define CC1101_FOC_PRE_K                  0x18u
#define CC1101_FOC_POST_K                 0x04u
#define CC1101_FOC_LIMIT                  0x03u
// BSCFG
#define CC1101_BS_PRE_K                   0xC0u
#define CC1101_BS_PRE_KP                  0x30u
#define CC1101_BS_POST_K                  0x08u
#define CC1101_BS_POST_KP                 0x04u
#define CC1101_BS_LIMIT                   0x03u
// AGCCTRL2
#define CC1101_MAX_DVGA_GAIN              0xC0u
#define CC1101_MAX_LNA_GAIN               0x38u
#define CC1101_MAGN_TARGET                0x07u
// AGCCTRL1
#define CC1101_AGC_LNA_PRIORITY           0x40u
#define CC1101_CARRIER_SENSR_REL_THR      0x30u
#define CC1101_CARRIER_SENSE_ABS_THR      0x0Fu
// AGCCTRL0
#define CC1101_HYST_LEVEL                 0xC0u
#define CC1101_WAIT_TIME                  0x30u
#define CC1101_AGC_FREEZE                 0x0Cu
#define CC1101_FILTER_LENGTH              0x03u
// WOREVT1
#define CC1101_EVENT0_15_8                0xFFu
// WOREVT0
#define CC1101_EVENT0_7_0                 0xFFu
// WORCTRL
#define CC1101_RC_PD                      0x80u
#define CC1101_EVENT1                     0x70u
#define CC1101_RC_CAL                     0x08u
#define CC1101_WOR_RES                    0x03u
// FREND1
#define CC1101_LNA_CURRENT                0xC0u
#define CC1101_LNA2MIX_CURRENT            0x30u
#define CC1101_LODIV_BUF_CURRENT          0x0Cu
#define CC1101_MIX_CURRENT                0x03u
// FREND0
#define CC1101_LODIV_BUF_CURRENT_TX       0x30u
#define CC1101_PA_POWER                   0x07u
// FSCAL3
#define CC1101_FSCAL3_7_6                 0xC0u
#define CC1101_CHP_CURR_CAL_EN            0x30u
#define CC1101_FSCAL3_3_0                 0x0Fu
// FSCAL2
#define CC1101_VCO_CORE_H_EN              0x20u
#define CC1101_FSCAL2_7_0                 0x1Fu
// FSCAL1
#define CC1101_FSCAL1_7_0                 0x3Fu
// FSCAL0
#define CC1101_FSCAL0_7_0                 0x7Fu
// RCCTRL1
#define CC1101_RCCTRL_1                   0x7Fu
// RCCTRL0
#define CC1101_RCCTRL_0                   0x7Fu
// FSTEST
#define CC1101_FSTEST_7_0                 0xFFu
// PTEST
#define CC1101_PTEST_7_0                  0xFFu
// AGCTEST
#define CC1101_AGCTEST_7_0                0xFFu
// TEST2
#define CC1101_TEST2_7_0                  0xFFu
// TEST1
#define CC1101_TEST1_7_0                  0xFFu
// TEST0
#define CC1101_TEST0_7_2                  0xFCu
#define CC1101_VCO_SEL_CAL_EN             0x02u
#define CC1101_TEST0_0                    0x01u

// Status register field masks
// PARTNUM
#define CC1101_PARTNUM_7_0                0xFFu
// VERSION
#define CC1101_VERSION_7_0                0xFFu
// FREQOFF_EST
#define CC1101_FREQOFF_EST                0xFFu
// LQI
#define CC1101_CRC_OK                     0x80u
#define CC1101_LQI_EST                    0x7Fu
// RSSI
#define CC1101_RSSI_7_0                   0xFFu
// MARC_STATE
#define CC1101_MARC_STATE                 0x1Fu
// WORTIME1
#define CC1101_TIME_15_8                  0xFFu
// WORTIME0
#define CC1101_TIME_7_0                   0xFFu
// PKTSTATUS
#define CC1101_PKTSTATUS_CRC_OK           0x80u
#define CC1101_PKSTATUS_CS                0x40u
#define CC1101_PKTSTATUS_PQT_REACHED      0x20u
#define CC1101_PKTSTATUS_CCA              0x10u
#define CC1101_PKSTATUS_SFD               0x08u
#define CC1101_PKTSTATUS_GDO2             0x04u
#define CC1101_PKTSTATUS_GDO0             0x01u
// VCO_VC_DAC
#define CC1101_VCO_VC_DAC_7_0             0xFFu
// TXBYTES
#define CC1101_TXFIFO_UNDERFLOW           0x80u
#define CC1101_NUM_TXBYTES                0x7Fu
// RXBYTES
#define CC1101_RXFIFO_OVERFLOW            0x80u
#define CC1101_NUM_RXBYTES                0x7Fu
// RCCTRL1_STATUS
#define CC1101_RCCTRL1_STATUS_7_0         0xFFu
// RCCTRL0_STATUS
#define CC1101_RCCTRL0_STATUS_7_0         0xFFu

// Burst/single access masks
#define CC1101_WRITE_SINGLE               0x00u
#define CC1101_WRITE_BURST                0x40u
#define CC1101_READ_SINGLE                0x80u
#define CC1101_READ_BURST                 0xC0u

// -----------------------------------------------------------------------------

// Physical FIFO size
#define CC1101_RXFIFO_SIZE        64 // Receive hardware FIFO absolute size
#define CC1101_TXFIFO_SIZE        64 // Transmit hardware FIFO absolute size

// Maximum timeout error ticks
#define CC1101_MAX_TIMEOUT        2000

// -----------------------------------------------------------------------------

/**
 *  eCC1101Error - error codes issued by the device driver.
 */
enum eCC1101Error
{
  eCC1101ErrorTimeout     = 0x01u,
  eCC1101ErrorSleep       = 0x02u
};

/**
 *  eCC1101Chip - device driver supported chipsets.
 */
enum eCC1101Chip
{
  eCC1101ChipUnknown    = 0,
  eCC1101Chip1101       = CC1101,
  eCC1101Chip110L       = CC110L,
  eCC1101Chip2500       = CC2500
};

/**
 *  eCC1101MarcState - all possible MARCSTATEs that the radio state machine can
 *  traverse.
 */
enum eCC1101MarcState
{
  eCC1101MarcStateSleep             = 0x00u,
  eCC1101MarcStateIdle              = 0x01u,
  eCC1101MarcStateXOff              = 0x02u,
  eCC1101MarcStateVcoon_mc          = 0x03u,
  eCC1101MarcStateRegon_mc          = 0x04u,
  eCC1101MarcStateMancal            = 0x05u,
  eCC1101MarcStateVcoon             = 0x06u,
  eCC1101MarcStateRegon             = 0x07u,
  eCC1101MarcStateStartcal          = 0x08u,
  eCC1101MarcStateBwboost           = 0x09u,
  eCC1101MarcStateFs_lock           = 0x0Au,
  eCC1101MarcStateIfadcon           = 0x0Bu,
  eCC1101MarcStateEndcal            = 0x0Cu,
  eCC1101MarcStateRx                = 0x0Du,
  eCC1101MarcStateRx_end            = 0x0Eu,
  eCC1101MarcStateRx_rst            = 0x0Fu,
  eCC1101MarcStateTxrx_switch       = 0x10u,
  eCC1101MarcStateRxfifo_overflow   = 0x11u,
  eCC1101MarcStateFstxon            = 0x12u,
  eCC1101MarcStateTx                = 0x13u,
  eCC1101MarcStateTx_end            = 0x14u,
  eCC1101MarcStateRxtx_switch       = 0x15u,
  eCC1101MarcStateTxfifo_underflow  = 0x16u,
  eCC1101MarcStateUnknown           = 0xFFu
};

/**
 *  eCC1101GdoState - possible states that the GDOx line can be in.
 */
enum eCC1101GdoState
{
  eCC1101GdoStateWaitForAssert    = 0x00u,
  eCC1101GdoStateWaitForDeassert  = 0x01u
};

// -----------------------------------------------------------------------------

/**
 *  sCC1101 - configuration registers for the CC1101. These registers control
 *  the operation of the transceiver chip and are the same for the 110L and
 *  2500.
 *
 *  Note: The following registers are unavailable in the CC110L, mdmcfg0,
 *  worevt1, worevt0, worctrl1, rcctrl1, rcctrl0, fstest, ptest, agctest. These
 *  registers should only be written to with their appropriate default values.
 *  Please see the CC110L User's Guide (swrs109a) for more information.
 */
typedef struct
{
  unsigned char iocfg2;   // GDO2 output pin configuration
  unsigned char iocfg1;   // GDO1 output pin configuration
  unsigned char iocfg0;   // GDO0 output pin configuration
  unsigned char fifothr;  // RXFIFO and TXFIFO thresholds
  unsigned char sync1;    // Sync word, high byte
  unsigned char sync0;    // Sync word, low byte
  unsigned char pktlen;   // Packet length
  unsigned char pktctrl1; // Packet automation control
  unsigned char pktctrl0; // Packet automation control
  unsigned char addr;     // Device address
  unsigned char channr;   // Channel number
  unsigned char fsctrl1;  // Frequency synthesizer control
  unsigned char fsctrl0;  // Frequency synthesizer control
  unsigned char freq2;    // Frequency control word, high byte
  unsigned char freq1;    // Frequency control word, middle byte
  unsigned char freq0;    // Frequency control word, low byte
  unsigned char mdmcfg4;  // Modem configuration 4
  unsigned char mdmcfg3;  // Modem configuration 3
  unsigned char mdmcfg2;  // Modem configuration 2
  unsigned char mdmcfg1;  // Modem configuration 1
  unsigned char mdmcfg0;  // Modem configuration 0
  unsigned char deviatn;  // Modem deviation setting
  unsigned char mcsm2;    // Main radio control state machine configuration
  unsigned char mcsm1;    // Main radio control state machine configuration
  unsigned char mcsm0;    // Main radio control state machine configuration
  unsigned char foccfg;   // Frequency offset compensation configuration
  unsigned char bscfg;    // Bit synchronization configuration
  unsigned char agcctrl2; // AGC control 2
  unsigned char agcctrl1; // AGC control 1
  unsigned char agcctrl0; // AGC control 0
  unsigned char worevt1;  // High byte event0 timeout
  unsigned char worevt0;  // Low byte event0 timeout
  unsigned char worctrl;  // Wake on radio control
  unsigned char frend1;   // Front end RX configuration
  unsigned char frend0;   // Front end TX configuration
  unsigned char fscal3;   // Frequency synthesizer calibration
  unsigned char fscal2;   // Frequency synthesizer calibration
  unsigned char fscal1;   // Frequency synthesizer calibration
  unsigned char fscal0;   // Frequency synthesizer calibration
  unsigned char rcctrl1;  // RC oscillator configuration
  unsigned char rcctrl0;  // RC oscillator configuration
  unsigned char fstest;   // Frequency synthesizer calibration control
  unsigned char ptest;    // Production test
  unsigned char agctest;  // AGC test
  unsigned char test2;    // Various test settings 2
  unsigned char test1;    // Various test settings 1
  unsigned char test0;    // Various test settings 0
} sCC1101;

/**
 *  sCC1101Gdo - GDOx implementation pointers. These function pointers, when
 *  initialized properly, will be pointing to the addresses of your GDOx
 *  interface. This will allow the CC1101 interface to operate correclty.
 *
 *  Note: It is assumed that this structure is defined in static memory. This is
 *  because the CC1101 interface depends on the integrity of the data stored for
 *  all its operations.
 */
typedef struct
{
	void(*const Init)(void);
  	  bool(*const Event)(volatile const unsigned char);
  	  void(*const WaitForAssert)(void);
  	  void(*const WaitForDeassert)(void);
  	  enum eCC1101GdoState(*const GetState)(void);
  	  void(*const Enable)(bool);
} sCC1101Gdo;

/**
 *  sCC1101PhyInfo - the CC1101 physical device's information. The information
 *  is updated by the CC1101 interface throughout its lifetime of operation. In
 *  order to keep this structure's integrity, it is recommended that the caller
 *  does not change any of the contents in this structure manually. The CC1101
 *  interface provides functions to directly manipulate and see the contents of
 *  this structure.
 *
 *  Note: It is assumed that this structure is defined in static memory. This is
 *  because the CC1101 interface depends on the integrity of the data stored for
 *  all its operations.
 */
typedef struct
{
	Mcspi_t* SpiStruct;     // Interface for SPI
	sCC1101Gdo *gdo[3];  // Interface for GDOx
	volatile bool sleep;        // Chip sleep flag
	STimer_t timer;
	unsigned char spi_instance;
} sCC1101Phy;

bool CC1101_read(sCC1101Phy* phy, unsigned char address, unsigned char *Buff, unsigned int DataLen);
bool CC1101_write(sCC1101Phy* phy, unsigned char address, unsigned char *Buff, unsigned int DataLen);
unsigned char CC1101_ReadSyncProblem(sCC1101Phy* phy, unsigned char address);
bool CC1101_SetVerifyState(sCC1101Phy* phy, unsigned char command, enum eCC1101MarcState state);
void CC1101_GdoIit(sCC1101Phy* phy, sCC1101Gdo *gdo[3]);
unsigned char CC1101_GetReg(sCC1101Phy* phy, unsigned char address);
bool CC1101_SetRag(sCC1101Phy* phy, unsigned char address, unsigned char value);
bool CC1101_ReadRegs(sCC1101Phy* phy, unsigned char address, unsigned char *buffer, unsigned char count);
bool CC1101_WriteRegs(sCC1101Phy* phy, unsigned char address, unsigned char *buffer, unsigned char count);
unsigned char CC1101_ReadFifo(sCC1101Phy* phy, unsigned char *buffer, unsigned char count);
bool CC1101_WriteFifo(sCC1101Phy* phy, unsigned char *buffer, unsigned char count);
bool CC1101_Strobe(sCC1101Phy* phy, unsigned char command);
bool CC1101_GoOffCrystal(sCC1101Phy* phy);
bool CC1101_Calibrate(sCC1101Phy* phy);
bool CC1101Sleep(sCC1101Phy* phy);
void CC1101Wakeup(sCC1101Phy* phy, unsigned char agctest, unsigned char test[3], unsigned char *paTable, unsigned char paTableSize);
/**
 *  CC1101GetRxFifoCount - get the number of bytes available in the RX FIFO.
 *
 *    @param  struct sCC1101PhyInfo*  phyInfo CC1101 interface state information
 *                                            used by the interface for all chip
 *                                            interaction.
 *
 *    @return unsigned char   Number of bytes available in the RX FIFO.
 */
#define CC1101_GetRxFifoCount(phy)\
		CC1101_GetReg(phy, CC1101_RXBYTES)

/**
 *  CC1101GetTxFifoCount - get the number of bytes available in the TX FIFO.
 *
 *    @param  struct sCC1101PhyInfo*  phyInfo CC1101 interface state information
 *                                            used by the interface for all chip
 *                                            interaction.
 *
 *    @return unsigned char   Number of bytes in the TX FIFO.
 */
#define CC1101_GetTxFifoCount(phy)\
		CC1101_GetReg(phy, CC1101_TXBYTES)

/**
 *  CC1101GetMarcState - get the radio state machine's current state.
 *
 *    @param  struct sCC1101PhyInfo*  phyInfo CC1101 interface state information
 *                                            used by the interface for all chip
 *                                            interaction.
 *
 *    @return enum eCC1101MarcState   The last MARCSTATE read from the CC1101.
 */
#define CC1101_GetMarcState(phy)\
  (enum eCC1101MarcState)CC1101_GetReg(phy, CC1101_MARCSTATE)

/**
 *  CC1101GetRssi - get the receive signal strength indicator (RSSI) value.
 *
 *    @param  struct sCC1101PhyInfo*  phyInfo CC1101 interface state information
 *                                            used by the interface for all chip
 *                                            interaction.
 *
 *    @return unsigned char   Last RSSI sample value obtained.
 */
#define CC1101_GetRssi(phy)\
		CC1101_GetReg(phy, CC1101_RSSI)

#endif /* LIB_DEVICE_CC1101_H_ */
