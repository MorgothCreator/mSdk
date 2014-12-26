/*
 * adc_def.h
 *
 *  Created on: Dec 26, 2014
 *      Author: Iulian Gheorghiu
 */
/*#####################################################*/
#ifndef ADC_DEF_H_
#define ADC_DEF_H_
/*#####################################################*/
typedef enum
{
	ADC_ALIGN_LEFT = 0,
	ADC_ALIGN_RIGHT
}ADC_ALIGN_en;
/*#####################################################*/
typedef enum
{
	ADC_MODE_INDEPENDENT = 0,
	ADC_MODE_DUAL
}ADC_MODE_en;
/*#####################################################*/
typedef enum
{
	ADC_CONV_MODE_SINGLE = 0,
	ADC_CONV_MODE_MULTICHANNEL
}ADC_CONV_MODE_en;
/*#####################################################*/
typedef enum
{
	ADC_CONTINUOUS_MODE_SINGLE = 0,
	ADC_CONTINUOUS_MODE_CONTINUOUS
}ADC_CONTINUOUS_MODE_en;
/*#####################################################*/
typedef enum
{
	ADC_SAMPLE_TIME_1 = 0,
	ADC_SAMPLE_TIME_2 = 1,
	ADC_SAMPLE_TIME_3 = 2,
	ADC_SAMPLE_TIME_4 = 3,
	ADC_SAMPLE_TIME_5 = 4,
	ADC_SAMPLE_TIME_6 = 5,
	ADC_SAMPLE_TIME_7 = 6,
	ADC_SAMPLE_TIME_8 = 7,
}ADC_SAMPLE_TIME_en;
/*#####################################################*/
typedef enum
{
	ADC_INT_REF_DISABLED = 0,
	ADC_INT_REF_Enabled
}ADC_INT_REF_en;
/*#####################################################*/
typedef enum
{
	ADC_INT_TEMP_SENSOR_DISABLED = 0,
	ADC_INT_TEMP_SENSOR_Enabled
}ADC_INT_TEMP_SENSOR_en;
/*#####################################################*/
typedef enum
{
	ADC_EXT_TRIG_NONE = 0,
	ADC_EXT_TRIG_1,
	ADC_EXT_TRIG_2,
	ADC_EXT_TRIG_3,
	ADC_EXT_TRIG_4,
	ADC_EXT_TRIG_5,
	ADC_EXT_TRIG_6,
	ADC_EXT_TRIG_7,
}ADC_EXT_TRIG_en;
/*#####################################################*/
typedef struct
{
	void *BaseAddr;							/*!< Here will be the base address of ADC module. This will be write on ADC initialization  */

	unsigned char UnitNr;					/*!< Specifies the unit number */

	ADC_MODE_en Mode;						/*!< Configures the ADC to operate in independent or dual mode. ADC_MODE_INDEPENDENT and ADC_MODE_DUAL */

	ADC_CONV_MODE_en ConvMode;				/*!< Specifies whether the conversion is performed in Scan (multichannels) or Single (one channel) mode. This parameter can be set to true = multichanel or false = single channel */

	ADC_CONTINUOUS_MODE_en ContinuousMode;	/*!< Specifies whether the conversion is performed in Continuous or Single mode. This parameter can be set to ENABLE or DISABLE. */

	unsigned int ExtTrig;					/*!< Defines the external trigger used to start the analog to digital conversion of regular channels. */

	ADC_ALIGN_en Align;						/*!< Specifies whether the ADC data alignment is ADC_ALIGN_LEFT or ADC_ALIGN_RIGHT. */

	//unsigned char NrOfChannels;				/*!< Specifies the number of ADC channels that will be converted using the sequencer for regular channel group. */

	ADC_SAMPLE_TIME_en SampleTime;			/*!< Sample time, watch the ADC controller documentation */

	char EnCh[32];							/*!< Specifies whether the channel will be enabled. Here is a table with order of registration, the rank will be assigned in order. ChEn[n] = 0 signify that the channel 'n' is disabled, ChEn[n] = 1 signify Channel 0*/

	bool IntRefEn;

	bool TempSensorEn;

	unsigned int DmaChannel;

	unsigned char DmaUnitNr;

	void *DmaChBaseAddr;

	unsigned int ConvResult[32];
}Adc_t;


/*#####################################################*/
#define new_adc Adc_t
#ifndef new_
#define new_(structure) (structure*)calloc(1,sizeof(structure));
#endif
#define free_gpio(address) free(address);
/*#####################################################*/
#ifdef HEADER_INCLUDE_C_FILES
#include "adc_def.c"
#endif
/*#####################################################*/
#endif /* ADC_DEF_H_ */
/*#####################################################*/
