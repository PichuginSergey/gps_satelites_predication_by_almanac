#include "sv_time.h"

/* Conversion from UTC to GPS time */
void fromUtcToGpsTime(const Time& currentTime, int *wk, double *seconds) noexcept
{
	const unsigned int doy[12] ={ 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
	const int mounth = currentTime.getMonth();
	const int ye = (currentTime.getYear())-1980;
	int lpdays = ye/4 + 1;
	if ( ((ye%4) == 0) && ((mounth) <= 2) ) --lpdays;
	int de = ye*365 + doy[(mounth)-1] + currentTime.getDay() + lpdays - 6;
	/* GPS weeks */
	*wk = de / Time::NUM_DAYS_IN_WEEK;
	/* GPS seconds */
	*seconds = (de%Time::NUM_DAYS_IN_WEEK)*Time::SECONDS_IN_DAY + (currentTime.getHour())*Time::SECONDS_IN_HOUR +
			(currentTime.getMinute())*Time::SECONDS_IN_MINUTE + (currentTime.getSecond());

	/* GPS seconds must be in the range 0-SECONDS_IN_WEEK */
	while(*seconds<0.0)
	{
		*wk -= 1;
		*seconds += Time::SECONDS_IN_WEEK;
	}
	while(*seconds >= Time::SECONDS_IN_WEEK)
	{
		*wk += 1;
		*seconds -= Time::SECONDS_IN_WEEK;
	}	
}