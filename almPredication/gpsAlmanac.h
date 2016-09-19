#ifndef GPS_ALMANAC_H_
#define GPS_ALMANAC_H_
#include <vector>
#include "almanac.h"
#include "satellite.h"

struct AlmanacGps{
	AlmanacGps() : vflg(0) {}
	unsigned int	vflg;				
	unsigned int	almHealth;
	int				refweek;
	double			e;					
	double			toa;				              
	double			delta_i;            
	double			omegadot;           
	double			sqrta;              
	double			omega0 ;            
	double			w;					
	double			m0;					
	double			af0;               
	double			af1;                
};

class GpsAlm : public Almanac {

public :
	GpsAlm() : alm(SatGps::NUM_GPS_SAT) {}
	void sv_pos_predication(const Time& cur_time, Satellite& sat) const;
	void read_alm(const std::string& name);
	
	static const double 		GRAV_CONSTANT_GPS;
	static const double			WGS84_OE;
	static const unsigned int	SECONDS_A_WEEK;
private:
	std::vector<AlmanacGps> alm;
};

#endif