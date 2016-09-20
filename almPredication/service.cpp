#include <iostream>
#include <ctime>
#include <cmath>
#include <fstream>
#include <memory>
#include "satellite.h"
#include "service.h"
#include "file.h"
#include "time.h"

const double Pi = 3.141592653589793;

void pred_sv_param(const Position& pos, 
				   const Time& cur_time, 
				   const Almanac& alm, 
				   GNSS& sat) {
	
	for (size_t i = 0; i < sat.size(); ++i) {
		alm.sv_pos_predication(cur_time, sat[i]);
		if (sat[i].coord.vflg)
			sv_angles(pos, sat[i]);
	}
}

/* Get current time from system */
void get_current_time(Time& t) {
	time_t lt = time(NULL);
	tm* tm = localtime(&lt);
		
	t.set_week_day(tm->tm_wday);
	t.set_day(tm->tm_mday);
	t.set_month(tm->tm_mon + 1);
	t.set_year(tm->tm_year + 1900);

	t.set_hour(tm->tm_hour);
	t.set_min(tm->tm_min);
	t.set_sec(tm->tm_sec);
}

/* Read current position of the users from file */
void get_cur_pos (const std::string& file, Position& pos) { 		
	
	InputFile<std::ifstream> in(file.c_str());
	if (!in) {
		std::string s = "Can not open file " + file;
		throw std::exception(s.c_str());
	}

	signed int temp (0);
	in >> temp; pos.setX(temp);
	in >> temp; pos.setY(temp);
	in >> temp; pos.setZ(temp);



}

void  sv_angles(const Position& pos, Satellite& sat){

		double dNEU[] = {0, 0, 0};
		double T[3][3];
		double temp;
		double dx = sat.coord.xs - pos.getX();
		double dy = sat.coord.ys - pos.getY();
		double dz = sat.coord.zs - pos.getZ();

	// Get the transition matrix from XYZ to NEU
	double clat, slat, clon, slon;
		clat = cos(pos.getB());
        slat = sin(pos.getB());
        clon = cos(pos.getL());
        slon = sin(pos.getL());

		T[0][0] = -slat*clon;	T[0][1] = -slat*slon;	T[0][2] = clat;
        T[1][0] = -slon;		T[1][1] = clon;			T[1][2] = 0;
        T[2][0] = clat*clon;	T[2][1] = clat*slon;	T[2][2] = slat;

	for(unsigned int i = 0; i < 3; i++)
		 dNEU[i] = T[i][0] * dx + T[i][1] * dy + T[i][2] * dz;

	temp =  sqrt((dNEU[0] * dNEU[0]) + (dNEU[1] * dNEU[1]));

	sat.ang.elv = atan2(dNEU[2], temp);

	sat.ang.azi = atan2(dNEU[1], dNEU[0]);
	if (sat.ang.azi<0)
		sat.ang.azi += (double)2*Pi;
	sat.ang.vflg = true;
}

void display_sat(const std::string& msg, const GNSS& sat, int elv_threshold) {

	std::cout << msg << '\n';
	for (size_t i = 0; i < sat.size(); ++i) {
	
		double elv = floor(sat[i].ang.elv*180/Pi + 0.5);
		double azi = floor(sat[i].ang.azi*180/Pi + 0.5);
		
		if (sat[i].ang.vflg && 
			sat[i].ang.elv > 0 && elv >= elv_threshold)
				std::cout << (i + 1) << "  " << azi << " " << elv << std::endl;
	}
	std::cout << std::endl;
}

/* Convert current date to julian date*/
unsigned int data_to_julian(unsigned int day, unsigned int month, unsigned int year) {
		
	unsigned a, y, m, jdn;

	a = (unsigned)(14 - month)/12;
	y = year + 4800 - a;
	m = month + 12*a - 3;

	jdn = day + (unsigned)(153*m + 2)/5 +365*y + (unsigned)y/4 - 32082;
	return jdn;

}