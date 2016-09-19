#ifndef ALMANAC_H_
#define ALMANAC_H_
#include <string> 
#include "satellite.h" 
#include "position.h"
#include "time.h"

class Almanac {

public:
	virtual void sv_pos_predication(const Time& cur_time, Satellite& sat) const = 0;
	virtual void read_alm(const std::string& name) = 0;

};

#endif