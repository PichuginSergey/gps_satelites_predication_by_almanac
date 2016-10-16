#include <iostream>
#include <ctime>
#include <cmath>
#include <fstream>
#include <stdexcept>
#include <memory>
#include "satellite.h"
#include "service.h"
#include "file.h"
#include "time.h"

const double Pi = 3.141592653589793;

void predicationSatelliteParametrs(const Position& position, 
				   const Time& currentTime, 
				   const Almanac& almanac, 
				   GNSS& satellites) 
{
	
	for (auto& satellite : satellites) 
	{
		almanac.predicationSatellitePosition(currentTime, satellite);
		if (satellite.coordinate.vflg)
			calculationSatelliteAngles(position, satellite);
	}
}

/* Get current time from system */
void getCurrentTime(Time& currentTime)
{
	time_t lt = time(NULL);
	tm* tm = localtime(&lt);
		
	currentTime.setWeekOfDay(tm->tm_wday);
	currentTime.setDay(tm->tm_mday);
	currentTime.setMonth(tm->tm_mon + 1);
	currentTime.setYear(tm->tm_year + 1900);

	currentTime.setHour(tm->tm_hour);
	currentTime.setMinute(tm->tm_min);
	currentTime.setSecond(tm->tm_sec);
}

/* Read current position of the users from file */
void get—urrentPosition(const std::string& fileName, Position& position) 
{
	InputFile<std::ifstream> in(fileName.c_str());
	if (!in) 
	{
		std::string s = "Can not open file " + fileName;
		throw std::runtime_error(s.c_str());
	}

	signed int temp{0};
	in >> temp; position.setX(temp);
	in >> temp; position.setY(temp);
	in >> temp; position.setZ(temp);

}

void  calculationSatelliteAngles(const Position& position, Satellite& satellite)
{
	double dNEU[] = {0, 0, 0};
	double T[3][3];
	double temp;
	double dx = satellite.coordinate.xs - position.getX();
	double dy = satellite.coordinate.ys - position.getY();
	double dz = satellite.coordinate.zs - position.getZ();

	// Get the transition matrix from XYZ to NEU
	double clat, slat, clon, slon;
	clat = cos(position.getB());
    slat = sin(position.getB());
    clon = cos(position.getL());
    slon = sin(position.getL());

	T[0][0] = -slat*clon;	T[0][1] = -slat*slon;	T[0][2] = clat;
    T[1][0] = -slon;		T[1][1] = clon;			T[1][2] = 0;
    T[2][0] = clat*clon;	T[2][1] = clat*slon;	T[2][2] = slat;

	for(unsigned int i = 0; i < 3; i++)
		 dNEU[i] = T[i][0] * dx + T[i][1] * dy + T[i][2] * dz;

	temp =  sqrt((dNEU[0] * dNEU[0]) + (dNEU[1] * dNEU[1]));

	satellite.angle.elv = atan2(dNEU[2], temp);

	satellite.angle.azi = atan2(dNEU[1], dNEU[0]);
	if (satellite.angle.azi<0)
		satellite.angle.azi += (double)2*Pi;
	satellite.angle.vflg = true;
}

void displaySatellite(const std::string&  title, GNSS& satellites, int elavationTreshold)
{
	std::cout << title << '\n';
	for (const auto& satellite: satellites) 
	{
		double elv = floor(satellite.angle.elv*180/Pi + 0.5);
		double azi = floor(satellite.angle.azi*180/Pi + 0.5);
		
		if (satellite.angle.vflg &&
			satellite.angle.elv > 0 && elv >= elavationTreshold)
				std::cout << satellite.getPrn() << "  " << azi << " " << elv << std::endl;
	}
	std::cout << std::endl;
}

/* Convert current date to julian date*/
unsigned int convertDateToJulianFormat(unsigned int day, unsigned int month, unsigned int year) noexcept
{
	unsigned int a = (unsigned)(14 - month)/12;
	unsigned int y = year + 4800 - a;
	unsigned int m = month + 12*a - 3;

	unsigned int jdn = day + (unsigned)(153*m + 2)/5 +365*y + (unsigned)y/4 - 32082;
	return jdn;
}