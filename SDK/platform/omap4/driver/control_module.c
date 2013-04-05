/*
 * pin_mux.c
 *
 *  Created on: Jan 12, 2013
 *      Author: XxXx
 */

#include <stdbool.h>
#include "control_module.h"
#include "include/hw.h"

void cm_wakeupevent_set(unsigned int pin_function)
{
	unsigned int base_addr = SYSCTRL_PADCONF_CORE_BASE_ADDR;
	if((pin_function & (pad_padconf_type_gm << pad_padconf_type_gp)) == pad_padconf_wkup_bm) base_addr = SYSCTRL_PADCONF_WKUP_BASE_ADDR;
	if(pin_function & pad_indicator_bm)
	{
		HWREG(base_addr + (pin_function >> reg_offset_gp)) |= ((unsigned)cm_wakeupevent_bm << cm_pad1_gp);
	}
	else
	{
		HWREG(base_addr + (pin_function >> reg_offset_gp)) |= ((unsigned)cm_wakeupevent_bm << cm_pad0_gp);
	}
}

void cm_wakeupevent_clear(unsigned int pin_function)
{
	unsigned int base_addr = SYSCTRL_PADCONF_CORE_BASE_ADDR;
	if((pin_function & (pad_padconf_type_gm << pad_padconf_type_gp)) == pad_padconf_wkup_bm) base_addr = SYSCTRL_PADCONF_WKUP_BASE_ADDR;
	if(pin_function & pad_indicator_bm)
	{
		HWREG(base_addr + (pin_function >> reg_offset_gp)) &= (~((unsigned)cm_wakeupevent_bm << cm_pad1_gp));
	}
	else
	{
		HWREG(base_addr + (pin_function >> reg_offset_gp)) &= (~((unsigned)cm_wakeupevent_bm << cm_pad0_gp));
	}
}

void cm_wakeupenable_set(unsigned int pin_function)
{
	unsigned int base_addr = SYSCTRL_PADCONF_CORE_BASE_ADDR;
	if((pin_function & (pad_padconf_type_gm << pad_padconf_type_gp)) == pad_padconf_wkup_bm) base_addr = SYSCTRL_PADCONF_WKUP_BASE_ADDR;
	if(pin_function & pad_indicator_bm)
	{
		HWREG(base_addr + (pin_function >> reg_offset_gp)) |= ((unsigned)cm_wakeupenable_bm << cm_pad1_gp);
	}
	else
	{
		HWREG(base_addr + (pin_function >> reg_offset_gp)) |= ((unsigned)cm_wakeupenable_bm << cm_pad0_gp);
	}
}

void cm_wakeupenable_clear(unsigned int pin_function)
{
	unsigned int base_addr = SYSCTRL_PADCONF_CORE_BASE_ADDR;
	if((pin_function & (pad_padconf_type_gm << pad_padconf_type_gp)) == pad_padconf_wkup_bm) base_addr = SYSCTRL_PADCONF_WKUP_BASE_ADDR;
	if(pin_function & pad_indicator_bm)
	{
		HWREG(base_addr + (pin_function >> reg_offset_gp)) &= (~((unsigned)cm_wakeupenable_bm << cm_pad1_gp));
	}
	else
	{
		HWREG(base_addr + (pin_function >> reg_offset_gp)) &= (~((unsigned)cm_wakeupenable_bm << cm_pad0_gp));
	}
}

void cm_offmodepulltypeselect_set(unsigned int pin_function)
{
	unsigned int base_addr = SYSCTRL_PADCONF_CORE_BASE_ADDR;
	if((pin_function & (pad_padconf_type_gm << pad_padconf_type_gp)) == pad_padconf_wkup_bm) base_addr = SYSCTRL_PADCONF_WKUP_BASE_ADDR;
	if(pin_function & pad_indicator_bm)
	{
		HWREG(base_addr + (pin_function >> reg_offset_gp)) |= ((unsigned)cm_offmodepulltypeselect_bm << cm_pad1_gp);
	}
	else
	{
		HWREG(base_addr + (pin_function >> reg_offset_gp)) |= ((unsigned)cm_offmodepulltypeselect_bm << cm_pad0_gp);
	}
}

void cm_offmodepulltypeselect_clear(unsigned int pin_function)
{
	unsigned int base_addr = SYSCTRL_PADCONF_CORE_BASE_ADDR;
	if((pin_function & (pad_padconf_type_gm << pad_padconf_type_gp)) == pad_padconf_wkup_bm) base_addr = SYSCTRL_PADCONF_WKUP_BASE_ADDR;
	if(pin_function & pad_indicator_bm)
	{
		HWREG(base_addr + (pin_function >> reg_offset_gp)) &= (~((unsigned)cm_offmodepulltypeselect_bm << cm_pad1_gp));
	}
	else
	{
		HWREG(base_addr + (pin_function >> reg_offset_gp)) &= (~((unsigned)cm_offmodepulltypeselect_bm << cm_pad0_gp));
	}
}

void cm_offmodepulludenable_set(unsigned int pin_function)
{
	unsigned int base_addr = SYSCTRL_PADCONF_CORE_BASE_ADDR;
	if((pin_function & (pad_padconf_type_gm << pad_padconf_type_gp)) == pad_padconf_wkup_bm) base_addr = SYSCTRL_PADCONF_WKUP_BASE_ADDR;
	if(pin_function & pad_indicator_bm)
	{
		HWREG(base_addr + (pin_function >> reg_offset_gp)) |= ((unsigned)cm_offmodepulludenable_bm << cm_pad1_gp);
	}
	else
	{
		HWREG(base_addr + (pin_function >> reg_offset_gp)) |= ((unsigned)cm_offmodepulludenable_bm << cm_pad0_gp);
	}
}

void cm_offmodepulludenable_clear(unsigned int pin_function)
{
	unsigned int base_addr = SYSCTRL_PADCONF_CORE_BASE_ADDR;
	if((pin_function & (pad_padconf_type_gm << pad_padconf_type_gp)) == pad_padconf_wkup_bm) base_addr = SYSCTRL_PADCONF_WKUP_BASE_ADDR;
	if(pin_function & pad_indicator_bm)
	{
		HWREG(base_addr + (pin_function >> reg_offset_gp)) &= (~((unsigned)cm_offmodepulludenable_bm << cm_pad1_gp));
	}
	else
	{
		HWREG(base_addr + (pin_function >> reg_offset_gp)) &= (~((unsigned)cm_offmodepulludenable_bm << cm_pad0_gp));
	}
}

void cm_offmodeoutvalue_set(unsigned int pin_function)
{
	unsigned int base_addr = SYSCTRL_PADCONF_CORE_BASE_ADDR;
	if((pin_function & (pad_padconf_type_gm << pad_padconf_type_gp)) == pad_padconf_wkup_bm) base_addr = SYSCTRL_PADCONF_WKUP_BASE_ADDR;
	if(pin_function & pad_indicator_bm)
	{
		HWREG(base_addr + (pin_function >> reg_offset_gp)) |= ((unsigned)cm_offmodeoutvalue_bm << cm_pad1_gp);
	}
	else
	{
		HWREG(base_addr + (pin_function >> reg_offset_gp)) |= ((unsigned)cm_offmodeoutvalue_bm << cm_pad0_gp);
	}
}

void cm_offmodeoutvalue_clear(unsigned int pin_function)
{
	unsigned int base_addr = SYSCTRL_PADCONF_CORE_BASE_ADDR;
	if((pin_function & (pad_padconf_type_gm << pad_padconf_type_gp)) == pad_padconf_wkup_bm) base_addr = SYSCTRL_PADCONF_WKUP_BASE_ADDR;
	if(pin_function & pad_indicator_bm)
	{
		HWREG(base_addr + (pin_function >> reg_offset_gp)) &= (~((unsigned)cm_offmodeoutvalue_bm << cm_pad1_gp));
	}
	else
	{
		HWREG(base_addr + (pin_function >> reg_offset_gp)) &= (~((unsigned)cm_offmodeoutvalue_bm << cm_pad0_gp));
	}
}

void cm_offmodeoutenable_set(unsigned int pin_function)
{
	unsigned int base_addr = SYSCTRL_PADCONF_CORE_BASE_ADDR;
	if((pin_function & (pad_padconf_type_gm << pad_padconf_type_gp)) == pad_padconf_wkup_bm) base_addr = SYSCTRL_PADCONF_WKUP_BASE_ADDR;
	if(pin_function & pad_indicator_bm)
	{
		HWREG(base_addr + (pin_function >> reg_offset_gp)) |= ((unsigned)cm_offmodeoutenable_bm << cm_pad1_gp);
	}
	else
	{
		HWREG(base_addr + (pin_function >> reg_offset_gp)) |= ((unsigned)cm_offmodeoutenable_bm << cm_pad0_gp);
	}
}

void cm_offmodeoutenable_clear(unsigned int pin_function)
{
	unsigned int base_addr = SYSCTRL_PADCONF_CORE_BASE_ADDR;
	if((pin_function & (pad_padconf_type_gm << pad_padconf_type_gp)) == pad_padconf_wkup_bm) base_addr = SYSCTRL_PADCONF_WKUP_BASE_ADDR;
	if(pin_function & pad_indicator_bm)
	{
		HWREG(base_addr + (pin_function >> reg_offset_gp)) &= (~((unsigned)cm_offmodeoutenable_bm << cm_pad1_gp));
	}
	else
	{
		HWREG(base_addr + (pin_function >> reg_offset_gp)) &= (~((unsigned)cm_offmodeoutenable_bm << cm_pad0_gp));
	}
}

void cm_offmodeenable_set(unsigned int pin_function)
{
	unsigned int base_addr = SYSCTRL_PADCONF_CORE_BASE_ADDR;
	if((pin_function & (pad_padconf_type_gm << pad_padconf_type_gp)) == pad_padconf_wkup_bm) base_addr = SYSCTRL_PADCONF_WKUP_BASE_ADDR;
	if(pin_function & pad_indicator_bm)
	{
		HWREG(base_addr + (pin_function >> reg_offset_gp)) |= ((unsigned)cm_offmodeenable_bm << cm_pad1_gp);
	}
	else
	{
		HWREG(base_addr + (pin_function >> reg_offset_gp)) |= ((unsigned)cm_offmodeenable_bm << cm_pad0_gp);
	}
}

void cm_offmodeenable_clear(unsigned int pin_function)
{
	unsigned int base_addr = SYSCTRL_PADCONF_CORE_BASE_ADDR;
	if((pin_function & (pad_padconf_type_gm << pad_padconf_type_gp)) == pad_padconf_wkup_bm) base_addr = SYSCTRL_PADCONF_WKUP_BASE_ADDR;
	if(pin_function & pad_indicator_bm)
	{
		HWREG(base_addr + (pin_function >> reg_offset_gp)) &= (~((unsigned)cm_offmodeenable_bm << cm_pad1_gp));
	}
	else
	{
		HWREG(base_addr + (pin_function >> reg_offset_gp)) &= (~((unsigned)cm_offmodeenable_bm << cm_pad0_gp));
	}
}

void cm_inputenable_set(unsigned int pin_function)
{
	unsigned int base_addr = SYSCTRL_PADCONF_CORE_BASE_ADDR;
	if((pin_function & (pad_padconf_type_gm << pad_padconf_type_gp)) == pad_padconf_wkup_bm) base_addr = SYSCTRL_PADCONF_WKUP_BASE_ADDR;
	if(pin_function & pad_indicator_bm)
	{
		HWREG(base_addr + (pin_function >> reg_offset_gp)) |= ((unsigned)cm_offmodeenable_bm << cm_pad1_gp);
	}
	else
	{
		HWREG(base_addr + (pin_function >> reg_offset_gp)) |= ((unsigned)cm_offmodeenable_bm << cm_pad0_gp);
	}
}

void cm_inputenable_clear(unsigned int pin_function)
{
	unsigned int base_addr = SYSCTRL_PADCONF_CORE_BASE_ADDR;
	if((pin_function & (pad_padconf_type_gm << pad_padconf_type_gp)) == pad_padconf_wkup_bm) base_addr = SYSCTRL_PADCONF_WKUP_BASE_ADDR;
	if(pin_function & pad_indicator_bm)
	{
		HWREG(base_addr + (pin_function >> reg_offset_gp)) &= (~((unsigned)cm_inputenable_bm << cm_pad1_gp));
	}
	else
	{
		HWREG(base_addr + (pin_function >> reg_offset_gp)) &= (~((unsigned)cm_inputenable_bm << cm_pad0_gp));
	}
}

void cm_pulltypeselect_set(unsigned int pin_function)
{
	unsigned int base_addr = SYSCTRL_PADCONF_CORE_BASE_ADDR;
	if((pin_function & (pad_padconf_type_gm << pad_padconf_type_gp)) == pad_padconf_wkup_bm) base_addr = SYSCTRL_PADCONF_WKUP_BASE_ADDR;
	if(pin_function & pad_indicator_bm)
	{
		HWREG(base_addr + (pin_function >> reg_offset_gp)) |= ((unsigned)cm_pulltypeselect_bm << cm_pad1_gp);
	}
	else
	{
		HWREG(base_addr + (pin_function >> reg_offset_gp)) |= ((unsigned)cm_pulltypeselect_bm << cm_pad0_gp);
	}
}

void cm_pulltypeselect_clear(unsigned int pin_function)
{
	unsigned int base_addr = SYSCTRL_PADCONF_CORE_BASE_ADDR;
	if((pin_function & (pad_padconf_type_gm << pad_padconf_type_gp)) == pad_padconf_wkup_bm) base_addr = SYSCTRL_PADCONF_WKUP_BASE_ADDR;
	if(pin_function & pad_indicator_bm)
	{
		HWREG(base_addr + (pin_function >> reg_offset_gp)) &= (~((unsigned)cm_pulltypeselect_bm << cm_pad1_gp));
	}
	else
	{
		HWREG(base_addr + (pin_function >> reg_offset_gp)) &= (~((unsigned)cm_pulltypeselect_bm << cm_pad0_gp));
	}
}

void cm_pulludenable_set(unsigned int pin_function)
{
	unsigned int base_addr = SYSCTRL_PADCONF_CORE_BASE_ADDR;
	if((pin_function & (pad_padconf_type_gm << pad_padconf_type_gp)) == pad_padconf_wkup_bm) base_addr = SYSCTRL_PADCONF_WKUP_BASE_ADDR;
	if(pin_function & pad_indicator_bm)
	{
		HWREG(base_addr + (pin_function >> reg_offset_gp)) |= ((unsigned)cm_pulludenable_bm << cm_pad1_gp);
	}
	else
	{
		HWREG(base_addr + (pin_function >> reg_offset_gp)) |= ((unsigned)cm_pulludenable_bm << cm_pad0_gp);
	}
}

void cm_pulludenable_clear(unsigned int pin_function)
{
	unsigned int base_addr = SYSCTRL_PADCONF_CORE_BASE_ADDR;
	if((pin_function & (pad_padconf_type_gm << pad_padconf_type_gp)) == pad_padconf_wkup_bm) base_addr = SYSCTRL_PADCONF_WKUP_BASE_ADDR;
	if(pin_function & pad_indicator_bm)
	{
		HWREG(base_addr + (pin_function >> reg_offset_gp)) &= (~((unsigned)cm_pulludenable_bm << cm_pad1_gp));
	}
	else
	{
		HWREG(base_addr + (pin_function >> reg_offset_gp)) &= (~((unsigned)cm_pulludenable_bm << cm_pad0_gp));
	}
}

void cm_muxmode_set(unsigned int pin_function)
{
	unsigned int base_addr = SYSCTRL_PADCONF_CORE_BASE_ADDR;
	if((pin_function & (pad_padconf_type_gm << pad_padconf_type_gp)) == pad_padconf_wkup_bm) base_addr = SYSCTRL_PADCONF_WKUP_BASE_ADDR;
	if(pin_function & pad_indicator_bm)
	{
		HWREG(base_addr + (pin_function >> reg_offset_gp)) = ((HWREG(base_addr + (pin_function >> reg_offset_gp))) & (~(cm_muxmode_gm << cm_pad1_gp))) | ((cm_muxmode_gm & pin_function) << cm_pad1_gp);
	}
	else
	{
		HWREG(base_addr + (pin_function >> reg_offset_gp)) = ((HWREG(base_addr + (pin_function >> reg_offset_gp))) & (~(cm_muxmode_gm << cm_pad0_gp))) | ((cm_muxmode_gm & pin_function) << cm_pad0_gp);
	}
}

void cm_pin_all_set(unsigned int pin_function, signed char pulludenable, signed char pulltypeselect,
		signed char inputenable, signed char offmodeenable, signed char offmodeoutenable, signed char offmodeoutvalue,
		signed char offmodepulludenable, signed char offmodepulltypeselect, signed char wakeupenable, signed char wakeupevent)
{
	cm_muxmode_set(pin_function);

	if(pulludenable == 1) cm_pulludenable_set(pin_function);
	else if(pulludenable == 0) cm_pulludenable_clear(pin_function);

	if(pulltypeselect == 1) cm_pulltypeselect_set(pin_function);
	else if(pulludenable == 0) cm_pulltypeselect_clear(pin_function);

	if(inputenable == 1) cm_inputenable_set(pin_function);
	else if(pulludenable == 0) cm_inputenable_clear(pin_function);

	if(offmodeenable == 1) cm_offmodeenable_set(pin_function);
	else if(pulludenable == 0) cm_offmodeenable_clear(pin_function);

	if(offmodeoutenable == 1) cm_offmodeoutenable_set(pin_function);
	else if(pulludenable == 0) cm_offmodeoutenable_clear(pin_function);

	if(offmodeoutvalue == 1) cm_offmodeoutvalue_set(pin_function);
	else if(pulludenable == 0) cm_offmodeoutvalue_clear(pin_function);

	if(offmodepulludenable == 1) cm_offmodepulludenable_set(pin_function);
	else if(pulludenable == 0) cm_offmodepulludenable_clear(pin_function);

	if(offmodepulltypeselect == 1) cm_offmodepulltypeselect_set(pin_function);
	else if(pulludenable == 0) cm_offmodepulltypeselect_clear(pin_function);

	if(wakeupenable == 1) cm_wakeupenable_set(pin_function);
	else if(pulludenable == 0) cm_wakeupenable_clear(pin_function);

	if(wakeupevent == 1) cm_wakeupevent_set(pin_function);
	else if(pulludenable == 0) cm_wakeupevent_clear(pin_function);
}

void cm_pin_set(unsigned int pin_function, signed char pulludenable, signed char pulltypeselect,
		signed char inputenable)
{
	cm_muxmode_set(pin_function);

	if(pulludenable == 1) cm_pulludenable_set(pin_function);
	else if(pulludenable == 0) cm_pulludenable_clear(pin_function);

	if(pulltypeselect == 1) cm_pulltypeselect_set(pin_function);
	else if(pulludenable == 0) cm_pulltypeselect_clear(pin_function);

	if(inputenable == 1) cm_inputenable_set(pin_function);
	else if(pulludenable == 0) cm_inputenable_clear(pin_function);

	cm_offmodeenable_clear(pin_function);

}


