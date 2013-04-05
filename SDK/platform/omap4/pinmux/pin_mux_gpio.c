/*
 * pin_mux_gpio.c
 *
 *  Created on: Jan 13, 2013
 *      Author: XxXx
 */

#include <stdbool.h>
#include "pin_mux_gpio.h"
#include "driver/control_module.h"

bool gpio_mux_setup(unsigned char PortNr, unsigned char PinNr, unsigned char Mode)
{
	return gpio_mux_setup_complete(PortNr, PinNr, Mode, false, 0, true);
}

bool gpio_mux_setup_complete(unsigned char PortNr, unsigned char PinNr, unsigned char Mode,
		signed char pulludenable, signed char pulltypeselect, signed char inputenable)
{
	switch(PortNr)
	{
	case 0:
		switch(PinNr)
		{
		case 0:
			cm_pin_set(((((unsigned int)j27_gpio_0) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
			return true;
		case 1:
			cm_pin_set(((((unsigned int)h27_gpio_1) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
			return true;
		case 2:
			cm_pin_set(((((unsigned int)l27_gpio_2) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
			return true;
		case 3:
			cm_pin_set(((((unsigned int)k27_gpio_3) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
			return true;
		case 4:
			cm_pin_set(((((unsigned int)k1_gpio_wk4) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
			return true;
		//case 5:
		//	cm_pin_set(((((unsigned int)ad2_gpio_wk5) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
		//	return true;
		case 6:
			cm_pin_set(((((unsigned int)ad2_gpio_wk6) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
			return true;
		case 7:
			cm_pin_set(((((unsigned int)ac2_gpio_wk7) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
			return true;
		case 8:
			cm_pin_set(((((unsigned int)ac3_gpio_wk8) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
			return true;
		case 9:
			cm_pin_set(((((unsigned int)af8_gpio_wk9) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
			return true;
		case 10:
			cm_pin_set(((((unsigned int)ae8_gpio_wk10) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
			return true;
		case 11:
			cm_pin_set(((((unsigned int)m2_gpio_11) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
			return true;
		case 12:
			cm_pin_set(((((unsigned int)n2_gpio_12) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
			return true;
		case 13:
			cm_pin_set(((((unsigned int)p2_gpio_13) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
			return true;
		case 14:
			cm_pin_set(((((unsigned int)v1_gpio_14) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
			return true;
		case 15:
			cm_pin_set(((((unsigned int)v2_gpio_15) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
			return true;
		case 16:
			cm_pin_set(((((unsigned int)w1_gpio_16) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
			return true;
		case 17:
			cm_pin_set(((((unsigned int)w2_gpio_17) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
			return true;
		case 18:
			cm_pin_set(((((unsigned int)w3_gpio_18) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
			return true;
		case 19:
			cm_pin_set(((((unsigned int)w4_gpio_19) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
			return true;
		case 20:
			cm_pin_set(((((unsigned int)y2_gpio_20) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
			return true;
		case 21:
			cm_pin_set(((((unsigned int)y3_gpio_21) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
			return true;
		case 22:
			cm_pin_set(((((unsigned int)y4_gpio_22) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
			return true;
		case 23:
			cm_pin_set(((((unsigned int)aa1_gpio_23) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
			return true;
		case 24:
			cm_pin_set(((((unsigned int)aa2_gpio_24) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
			return true;
		case 25:
			cm_pin_set(((((unsigned int)aa3_gpio_25) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
			return true;
		case 26:
			cm_pin_set(((((unsigned int)aa4_gpio_26) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
			return true;
		case 27:
			cm_pin_set(((((unsigned int)ab2_gpio_27) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
			return true;
		case 28:
			cm_pin_set(((((unsigned int)ab3_gpio_28) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
			return true;
		case 29:
			cm_pin_set(((((unsigned int)ag6_gpio_wk29) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
			return true;
		case 30:
			cm_pin_set(((((unsigned int)ad3_gpio_wk30) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
			return true;
		case 31:
			cm_pin_set(((((unsigned int)ad4_gpio_wk31) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
			return true;
		}
		case 1:
			switch(PinNr)
			{
			case 0:
				cm_pin_set(((((unsigned int)c16_gpio_32) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 1:
				cm_pin_set(((((unsigned int)d16_gpio_33) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 2:
				cm_pin_set(((((unsigned int)c17_gpio_34) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 3:
				cm_pin_set(((((unsigned int)d17_gpio_35) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 4:
				cm_pin_set(((((unsigned int)c18_gpio_36) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 5:
				cm_pin_set(((((unsigned int)d18_gpio_37) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 6:
				cm_pin_set(((((unsigned int)c19_gpio_38) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 7:
				cm_pin_set(((((unsigned int)d19_gpio_39) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 8:
				cm_pin_set(((((unsigned int)b17_gpio_40) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 9:
				cm_pin_set(((((unsigned int)a18_gpio_41) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 10:
				cm_pin_set(((((unsigned int)b18_gpio_42) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 11:
				cm_pin_set(((((unsigned int)a19_gpio_43) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 12:
				cm_pin_set(((((unsigned int)b19_gpio_44) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 13:
				cm_pin_set(((((unsigned int)b20_gpio_45) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 14:
				cm_pin_set(((((unsigned int)a21_gpio_46) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 15:
				cm_pin_set(((((unsigned int)b21_gpio_47) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 16:
				cm_pin_set(((((unsigned int)c20_gpio_48) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 17:
				cm_pin_set(((((unsigned int)d20_gpio_49) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 18:
				cm_pin_set(((((unsigned int)b25_gpio_50) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 19:
				cm_pin_set(((((unsigned int)c21_gpio_51) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 20:
				cm_pin_set(((((unsigned int)d21_gpio_52) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 21:
				cm_pin_set(((((unsigned int)c22_gpio_53) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 22:
				cm_pin_set(((((unsigned int)c25_gpio_54) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 23:
				cm_pin_set(((((unsigned int)b22_gpio_55) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 24:
				cm_pin_set(((((unsigned int)d25_gpio_56) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			//case 25:
			//	cm_pin_set(((((unsigned int)aa3_gpio_57) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
			//	return true;
			//case 26:
			//	cm_pin_set(((((unsigned int)aa4_gpio_58) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
			//	return true;
			case 27:
				cm_pin_set(((((unsigned int)c23_gpio_59) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 28:
				cm_pin_set(((((unsigned int)d22_gpio_60) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 29:
				cm_pin_set(((((unsigned int)b26_gpio_61) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 30:
				cm_pin_set(((((unsigned int)b23_gpio_62) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 31:
				cm_pin_set(((((unsigned int)b9_gpio_63) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			}
		case 2:
			switch(PinNr)
			{
			case 0:
				cm_pin_set(((((unsigned int)b10_gpio_64) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 1:
				cm_pin_set(((((unsigned int)a8_gpio_65) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 2:
				cm_pin_set(((((unsigned int)b8_gpio_66) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 3:
				cm_pin_set(((((unsigned int)r26_gpio_67) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 4:
				cm_pin_set(((((unsigned int)r25_gpio_68) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 5:
				cm_pin_set(((((unsigned int)t26_gpio_69) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 6:
				cm_pin_set(((((unsigned int)t25_gpio_70) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 7:
				cm_pin_set(((((unsigned int)u26_gpio_71) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 8:
				cm_pin_set(((((unsigned int)u25_gpio_72) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 9:
				cm_pin_set(((((unsigned int)v26_gpio_73) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 10:
				cm_pin_set(((((unsigned int)v25_gpio_74) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 11:
				cm_pin_set(((((unsigned int)w26_gpio_75) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 12:
				cm_pin_set(((((unsigned int)w25_gpio_76) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 13:
				cm_pin_set(((((unsigned int)m26_gpio_77) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 14:
				cm_pin_set(((((unsigned int)m25_gpio_78) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 15:
				cm_pin_set(((((unsigned int)n26_gpio_79) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 16:
				cm_pin_set(((((unsigned int)n25_gpio_80) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 17:
				cm_pin_set(((((unsigned int)t27_gpio_81) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 18:
				cm_pin_set(((((unsigned int)u27_gpio_82) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 19:
				cm_pin_set(((((unsigned int)v27_gpio_83) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 20:
				cm_pin_set(((((unsigned int)ae18_gpio_84) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 21:
				cm_pin_set(((((unsigned int)ag19_gpio_85) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 22:
				cm_pin_set(((((unsigned int)af19_gpio_86) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 23:
				cm_pin_set(((((unsigned int)ae19_gpio_87) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 24:
				cm_pin_set(((((unsigned int)af18_gpio_88) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 25:
				cm_pin_set(((((unsigned int)ag18_gpio_89) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 26:
				cm_pin_set(((((unsigned int)ae17_gpio_90) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 27:
				cm_pin_set(((((unsigned int)af17_gpio_91) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 28:
				cm_pin_set(((((unsigned int)ah17_gpio_92) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 29:
				cm_pin_set(((((unsigned int)ae16_gpio_93) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 30:
				cm_pin_set(((((unsigned int)af16_gpio_94) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 31:
				cm_pin_set(((((unsigned int)ag16_gpio_95) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			}
		case 3:
			switch(PinNr)
			{
			case 0:
				cm_pin_set(((((unsigned int)af14_gpio_96) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 1:
				cm_pin_set(((((unsigned int)ae14_gpio_97) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 2:
				cm_pin_set(((((unsigned int)h2_gpio_98) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 3:
				cm_pin_set(((((unsigned int)h3_gpio_99) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 4:
				cm_pin_set(((((unsigned int)d2_gpio_100) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 5:
				cm_pin_set(((((unsigned int)e3_gpio_101) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 6:
				cm_pin_set(((((unsigned int)e4_gpio_102) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 7:
				cm_pin_set(((((unsigned int)e2_gpio_103) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 8:
				cm_pin_set(((((unsigned int)e1_gpio_104) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 9:
				cm_pin_set(((((unsigned int)f4_gpio_105) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 10:
				cm_pin_set(((((unsigned int)f3_gpio_106) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 11:
				cm_pin_set(((((unsigned int)f1_gpio_107) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 12:
				cm_pin_set(((((unsigned int)g4_gpio_108) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 13:
				cm_pin_set(((((unsigned int)g3_gpio_109) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 14:
				cm_pin_set(((((unsigned int)ad27_gpio_110) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 15:
				cm_pin_set(((((unsigned int)ad26_gpio_111) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 16:
				cm_pin_set(((((unsigned int)ad25_gpio_112) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 17:
				cm_pin_set(((((unsigned int)ac28_gpio_113) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 18:
				cm_pin_set(((((unsigned int)ac26_gpio_114) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 19:
				cm_pin_set(((((unsigned int)ac25_gpio_115) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 20:
				cm_pin_set(((((unsigned int)ab25_gpio_116) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 21:
				cm_pin_set(((((unsigned int)ac27_gpio_117) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 22:
				cm_pin_set(((((unsigned int)ah26_gpio_118) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 23:
				cm_pin_set(((((unsigned int)ae24_gpio_119) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 24:
				cm_pin_set(((((unsigned int)af24_gpio_120) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 25:
				cm_pin_set(((((unsigned int)ag24_gpio_121) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 26:
				cm_pin_set(((((unsigned int)ah24_gpio_122) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 27:
				cm_pin_set(((((unsigned int)ab26_gpio_123) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 28:
				cm_pin_set(((((unsigned int)ab27_gpio_124) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 29:
				cm_pin_set(((((unsigned int)aa25_gpio_125) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 30:
				cm_pin_set(((((unsigned int)aa26_gpio_126) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 31:
				cm_pin_set(((((unsigned int)aa27_gpio_127) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			}
		case 4:
			switch(PinNr)
			{
			case 0:
				cm_pin_set(((((unsigned int)c26_gpio_128) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 1:
				cm_pin_set(((((unsigned int)d26_gpio_129) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 2:
				cm_pin_set(((((unsigned int)w27_gpio_130) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 3:
				cm_pin_set(((((unsigned int)y27_gpio_131) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 4:
				cm_pin_set(((((unsigned int)ag21_gpio_132) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 5:
				cm_pin_set(((((unsigned int)ah22_gpio_133) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 6:
				cm_pin_set(((((unsigned int)af22_gpio_134) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 7:
				cm_pin_set(((((unsigned int)ae22_gpio_135) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 8:
				cm_pin_set(((((unsigned int)ag22_gpio_136) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 9:
				cm_pin_set(((((unsigned int)ae23_gpio_137) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 10:
				cm_pin_set(((((unsigned int)af23_gpio_138) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 11:
				cm_pin_set(((((unsigned int)ag23_gpio_139) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 12:
				cm_pin_set(((((unsigned int)ah23_gpio_140) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 13:
				cm_pin_set(((((unsigned int)f27_gpio_141) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 14:
				cm_pin_set(((((unsigned int)f28_gpio_142) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 15:
				cm_pin_set(((((unsigned int)g27_gpio_143) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 16:
				cm_pin_set(((((unsigned int)g28_gpio_144) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 17:
				cm_pin_set(((((unsigned int)ae5_gpio_145) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 18:
				cm_pin_set(((((unsigned int)af5_gpio_146) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 19:
				cm_pin_set(((((unsigned int)ae4_gpio_147) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 20:
				cm_pin_set(((((unsigned int)af4_gpio_148) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 21:
				cm_pin_set(((((unsigned int)ag3_gpio_149) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 22:
				cm_pin_set(((((unsigned int)af3_gpio_150) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 23:
				cm_pin_set(((((unsigned int)ae21_gpio_151) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 24:
				cm_pin_set(((((unsigned int)af20_gpio_152) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 25:
				cm_pin_set(((((unsigned int)af21_gpio_153) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 26:
				cm_pin_set(((((unsigned int)ae20_gpio_154) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 27:
				cm_pin_set(((((unsigned int)ag20_gpio_155) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 28:
				cm_pin_set(((((unsigned int)ah19_gpio_156) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 29:
				cm_pin_set(((((unsigned int)ag12_gpio_157) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 30:
				cm_pin_set(((((unsigned int)af12_gpio_158) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 31:
				cm_pin_set(((((unsigned int)ae12_gpio_159) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			}
		case 5:
			switch(PinNr)
			{
			case 0:
				cm_pin_set(((((unsigned int)ag13_gpio_160) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 1:
				cm_pin_set(((((unsigned int)ae11_gpio_161) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 2:
				cm_pin_set(((((unsigned int)af11_gpio_162) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 3:
				cm_pin_set(((((unsigned int)ag11_gpio_163) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 4:
				cm_pin_set(((((unsigned int)ah11_gpio_164) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 5:
				cm_pin_set(((((unsigned int)ae10_gpio_165) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 6:
				cm_pin_set(((((unsigned int)af10_gpio_166) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 7:
				cm_pin_set(((((unsigned int)ag10_gpio_167) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 8:
				cm_pin_set(((((unsigned int)ae9_gpio_168) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 9:
				cm_pin_set(((((unsigned int)af13_gpio_169) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 10:
				cm_pin_set(((((unsigned int)ae13_gpio_170) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 11:
				cm_pin_set(((((unsigned int)g26_gpio_171) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 12:
				cm_pin_set(((((unsigned int)g25_gpio_172) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 13:
				cm_pin_set(((((unsigned int)h26_gpio_173) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 14:
				cm_pin_set(((((unsigned int)h25_gpio_174) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 15:
				cm_pin_set(((((unsigned int)j26_gpio_175) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 16:
				cm_pin_set(((((unsigned int)j25_gpio_176) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 17:
				cm_pin_set(((((unsigned int)k26_gpio_177) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 18:
				cm_pin_set(((((unsigned int)k25_gpio_178) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			//case 19:
			//	cm_pin_set(((((unsigned int)w4_gpio_179) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
			//	return true;
			//case 20:
			//	cm_pin_set(((((unsigned int)y2_gpio_180) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
			//	return true;
			case 21:
				cm_pin_set(((((unsigned int)aa28_gpio_181) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 22:
				cm_pin_set(((((unsigned int)y28_gpio_182) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 23:
				cm_pin_set(((((unsigned int)af6_gpio_183) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 24:
				cm_pin_set(((((unsigned int)f26_gpio_184) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 25:
				cm_pin_set(((((unsigned int)e27_gpio_185) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 26:
				cm_pin_set(((((unsigned int)e26_gpio_186) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 27:
				cm_pin_set(((((unsigned int)e25_gpio_187) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 28:
				cm_pin_set(((((unsigned int)d28_gpio_188) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 29:
				cm_pin_set(((((unsigned int)d27_gpio_189) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 30:
				cm_pin_set(((((unsigned int)ab4_gpio_190) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			case 31:
				cm_pin_set(((((unsigned int)ac4_gpio_191) & (~pin_function_gm)) | Mode), pulludenable, pulltypeselect, inputenable);
				return true;
			}
	}
	return false;
}
