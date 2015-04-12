This project can be configured as fallowing:

Modify definition on main.h

#define _USE_MPU60x0_9150	1
#define _USE_AK8975			  1
#define _USE_BMP180			  1
#define _USE_MS5611			  1
#define _USE_MHC5883		  1
#define _USE_SRF02			  1
#define _USE_ADXL345		  1
#define _USE_HIH613x		  1
#define _USE_LEPTON_FLIR	1
#define _USE_SHT11 			  0
#define _USE_INT_ADC		  0

Change 0 or 1 to enable or disable sensor reading.
