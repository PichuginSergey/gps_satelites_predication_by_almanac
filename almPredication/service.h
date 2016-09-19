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

#endif