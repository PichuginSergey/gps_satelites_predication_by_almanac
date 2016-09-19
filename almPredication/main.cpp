#include <iostream>
#include <cmath>
#include "glonassAlmanac.h"
#include "gpsAlmanac.h"
#include "satellite.h"
#include "service.h"
#include "position.h"

int main(int argc, char* argv[]) {
	
	Position_WGS_84 pos;
	get_cur_pos("position.ini", pos);
	Time	cur_time;
	get_current_time(cur_time);

	GpsAlm almGps;
	almGps.read_alm("MCCT_160825.agp");
	SatGps sat_gps;

	pred_sv_param(pos, cur_time, almGps, sat_gps);
	display_sat("GPS sat: ", sat_gps, 10);
	
	GlonassAlm almGln;
	almGln.read_alm("MCCT_160825.agl");
	SatGlonass sat_gln;

	pred_sv_param(pos, cur_time, almGln, sat_gln);
	display_sat("GLN sat: ", sat_gln, 10);

	return 0;
}