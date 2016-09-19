#include "time.h"

/* Conversion from UTC to GPS time */
void utc_to_gps(const Time& t, int *wk, double *sec) {
	
	const unsigned int doy[12] ={ 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
	const int mounth = t.get_month();
	const int ye = (t.get_year())-1980;
	int lpdays = ye/4 + 1;
	if ( ((ye%4) == 0) && ((mounth) <= 2) ) --lpdays;
	int de = ye*365 + doy[(mounth)-1] + t.get_day() + lpdays - 6;
	/* GPS weeks */
	const int NUM_DAYS_IN_WEEK = 7;
	*wk = de / NUM_DAYS_IN_WEEK;
	/* GPS seconds */
	*sec = (de%NUM_DAYS_IN_WEEK)*86400.0 + (t.get_hour())*3600.0 + (t.get_min())*60.0 + (t.get_sec());

	/* GPS seconds must be in the range 0-604800.0 */
	while(*sec<0.0){
		*wk -= 1;
		*sec += 604800.0;
	}
	while(*sec>=604800.0){
		*wk += 1;
		*sec -= 604800.0;
	}	
}