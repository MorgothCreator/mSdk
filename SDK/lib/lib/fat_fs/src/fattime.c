

#include "../inc/integer.h"
#include "../inc/fattime.h"
//#include "rtc.h" //RPi
#include "api/rtc_def.h"
#include "api/rtc_api.h"

DWORD get_fattime (void)
{
	rtc_t calendar;
	rtc_get_calendar(&calendar);
	//String.SetF(PasswordWindowNumeric->Text, "%02d:%02d:%02d-%02d:%02d:%02d", calendar.Year + 2000, calendar.Month, calendar.Date, calendar.Hours, calendar.Minutes, calendar.Seconds);
  return ((DWORD)((((calendar.Year + 2000) - 1980) & 0x7F)) << 25 | (DWORD)(calendar.Month & 0x0F) << 21 | (DWORD)(calendar.Date & 0x1F) << 16) | (DWORD)(calendar.Hours & 0x1F) << 11 | (DWORD)(calendar.Minutes & 0x3F) << 5 | (DWORD)(calendar.Seconds & 0x3F);
}

