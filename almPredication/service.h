#ifndef SERVICE_H_
#define SERVICE_H_

#include "almanac.h"
#include "position.h"
#include "satellite.h"
#include "time.h"

extern const double		Pi;

extern void pred_sv_param(const Position& pos, 
				   const Time& cur_time, 
				   const Almanac& alm, 
				   GNSS& sat);
extern void get_current_time(Time& t);
extern void get_cur_pos (const std::string& file, Position& pos) ;
extern void  sv_angles(const Position& pos, Satellite& sat);
extern void display_sat(const std::string& msg, const GNSS& sat, int elv_threshold);
extern unsigned int data_to_julian(unsigned int day, unsigned int month, unsigned int year);

class Alm_predication_task {

public:
	Alm_predication_task(const Position& pos,
		const Time& cur_time,
		const Almanac& alm,
		GNSS& satellite) : pos_(pos), cur_time_(cur_time), alm_(alm), satellite_(satellite) {}
	void operator()() {
		pred_sv_param(pos_, cur_time_, alm_, satellite_);
	}
private:
	const Position& pos_;
	const Time& cur_time_;
	const Almanac& alm_;
	GNSS& satellite_;
};
#endif