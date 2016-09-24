#include <iostream>
#include <cmath>
#include <future>
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

	GlonassAlm almGln;
	almGln.read_alm("MCCT_160825.agl");
	SatGlonass sat_gln;

	auto alm_pred_gps = std::async(Alm_predication_task(pos, cur_time, almGps, sat_gps));
	auto alm_pred_gln = std::async(Alm_predication_task(pos, cur_time, almGln, sat_gln));

	alm_pred_gps.get();
	display_sat("GPS sat: ", sat_gps, 10);
	alm_pred_gln.get();
	display_sat("GLN sat: ", sat_gln, 10);

	return 0;
}