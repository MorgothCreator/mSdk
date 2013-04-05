//#####################################################
//#####################################################
//#####################################################
//#####################################################
//#####################################################
#ifndef __IO_Ac__
#define __IO_Ac__
//#####################################################
#define    _AC_INTMODE_BOTHEDGES_gc (0x00)	/* Interrupt on both edges */
#define    _AC_INTMODE_FALLING_gc (0x02)	/* Interrupt on falling edge */
#define    _AC_INTMODE_RISING_gc (0x03)		/* Interrupt on rising edge */
//-----------------------------------------------------
#define    _AC_INTLVL_OFF_gc (0x00)			/* Interrupt disabled */
#define    _AC_INTLVL_LO_gc (0x01)			/* Low level */
#define    _AC_INTLVL_MED_gc (0x02)			/* Medium level */
#define    _AC_INTLVL_HI_gc (0x03)			/* High level */
//-----------------------------------------------------
#define    _AC_HYSMODE_NO_gc (0x00)			/* No hysteresis */
#define    _AC_HYSMODE_SMALL_gc (0x01)		/* Small hysteresis */
#define    _AC_HYSMODE_LARGE_gc (0x02)		/* Large hysteresis */
//-----------------------------------------------------
#define    _AC_MUXPOS_PIN0_gc (0x00)		/* Pin 0 */
#define    _AC_MUXPOS_PIN1_gc (0x01)		/* Pin 1 */
#define    _AC_MUXPOS_PIN2_gc (0x02)		/* Pin 2 */
#define    _AC_MUXPOS_PIN3_gc (0x03)		/* Pin 3 */
#define    _AC_MUXPOS_PIN4_gc (0x04)		/* Pin 4 */
#define    _AC_MUXPOS_PIN5_gc (0x05)		/* Pin 5 */
#define    _AC_MUXPOS_PIN6_gc (0x06)		/* Pin 6 */
#define    _AC_MUXPOS_DAC_gc (0x07)			/* DAC output */
//-----------------------------------------------------
#define    _AC_MUXNEG_PIN0_gc (0x00)		/* Pin 0 */
#define    _AC_MUXNEG_PIN1_gc (0x01)		/* Pin 1 */
#define    _AC_MUXNEG_PIN3_gc (0x02)		/* Pin 3 */
#define    _AC_MUXNEG_PIN5_gc (0x03)		/* Pin 5 */
#define    _AC_MUXNEG_PIN7_gc (0x04)		/* Pin 7 */
#define    _AC_MUXNEG_DAC_gc (0x05)			/* DAC output */
#define    _AC_MUXNEG_BANDGAP_gc (0x06)		/* Bandgap Reference */
#define    _AC_MUXNEG_SCALER_gc (0x07)		/* Internal voltage scaler */
//-----------------------------------------------------
#define    _AC_WINTMODE_ABOVE_gc (0x00)		/* Interrupt on above window */
#define    _AC_WINTMODE_INSIDE_gc (0x01)	/* Interrupt on inside window */
#define    _AC_WINTMODE_BELOW_gc (0x02)		/* Interrupt on below window */
#define    _AC_WINTMODE_OUTSIDE_gc (0x03)	/* Interrupt on outside window */
//-----------------------------------------------------
#define    _AC_WINTLVL_OFF_gc (0x00)		/* Interrupt disabled */
#define    _AC_WINTLVL_LO_gc (0x01)			/* Low priority */
#define    _AC_WINTLVL_MED_gc (0x02)		/* Medium priority */
#define    _AC_WINTLVL_HI_gc (0x03			/* High priority */
//-----------------------------------------------------
#define    _AC_WSTATE_ABOVE_gc (0x00)		/* Signal above window */
#define    _AC_WSTATE_INSIDE_gc (0x01)		/* Signal inside window */
#define    _AC_WSTATE_BELOW_gc (0x02)		/* Signal below window */
//#####################################################
typedef struct 
{
	void* Ac0RamServiceAddr;
#ifdef ACB
	void* Ac1RamServiceAddr;
#endif
} AcServiceAddresses;
//#####################################################
typedef struct
{
	void* Ac_BaseAddr;				//
	uint8_t Ac_ModuleNr;			//
	uint8_t Ac_ScaleFactor;			//
	uint8_t Ac_WindowEn;			//
	uint8_t Ac_WindowIntMode;		//
	uint8_t Ac_WindowIntLevel;		//
	uint8_t Ac_Ac0OutEn;			//
	uint8_t Ac_Ch0En;				//
	uint8_t Ac_Ch0IntMode;			//
	uint8_t Ac_Ch0IntLevel;			//
	uint8_t Ac_Ch0HighSpeedEn;		//
	uint8_t Ac_Ch0HisterezisSel;	//
	uint8_t Ac_Ch0PosMux;			//
	uint8_t Ac_Ch0NegMux;			//
	uint8_t Ac_Ch1En;				//
	uint8_t Ac_Ch1IntMode;			//
	uint8_t Ac_Ch1IntLevel;			//
	uint8_t Ac_Ch1HighSpeedEn;		//
	uint8_t Ac_Ch1HisterezisSel;	//
	uint8_t Ac_Ch1PosMux;			//
	uint8_t Ac_Ch1NegMux;			//
	void* CallBackCh0Data;			//
	void* CallBackCh1Data;			//
	void(*CallBackCh0)(void*);		//
	void(*CallBackCh1)(void*);		//
} Ac_t;
//#####################################################
#define new_ac Ac_t
#ifndef new_
#define new_(structure) (structure*)calloc(1,sizeof(structure));
#endif
#define free_ac(address) free(address);
//#####################################################
void ac_init(Ac_t *AcStruct);
//#####################################################
#ifdef HEADER_INCLUDE_C_FILES
#include "ac.c"
#endif
//-----------------------------------------------------
extern AcServiceAddresses Ac_ServiceAddresses;
//#####################################################
#endif
