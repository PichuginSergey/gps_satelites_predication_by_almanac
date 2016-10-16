#ifndef SERVICE_H_
#define SERVICE_H_

#include "almanac.h"
#include "position.h"
#include "satellite.h"
#include "time.h"

extern const double		Pi;

extern void predicationSatelliteParametrs(const Position& position,
				   const Time& currentTime,
				   const Almanac& almanac,
				   GNSS& satellites);
extern void getCurrentTime(Time& currentTime);
extern void get—urrentPosition(const std::string& file, Position& pos) ;
extern void calculationSatelliteAngles(const Position& pos, Satellite& sat);
extern void displaySatellite(const std::string& msg, GNSS& sat, int elv_threshold);
extern unsigned int convertDateToJulianFormat(unsigned int day, unsigned int month, unsigned int year) noexcept;

class Alm_predication_task {

public:
	Alm_predication_task(const Position& pos,
		const Time& cur_time,
		const Almanac& alm,
		GNSS& satellite) : pos_(pos), cur_time_(cur_time), alm_(alm), satellite_(satellite) {}
	void operator()() {
		predicationSatelliteParametrs(pos_, cur_time_, alm_, satellite_);
	}
private:
	const Position& pos_;
	const Time& cur_time_;
	const Almanac& alm_;
	GNSS& satellite_;
};
#endif