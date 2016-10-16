#ifndef GLONASS_ALMANAC_H_
#define GLONASS_ALMANAC_H_
#include <vector>
#include "almanac.h"
#include "satellite.h"

struct SatelliteGlonassAlmanac {
	SatelliteGlonassAlmanac() : vflg(0) {}
	unsigned int	vflg;
	unsigned int	svHealth;
	unsigned int	Na;
	signed	 int	H_n;
	double			taun;
	double			taus;
	double			taugps;
	double			tLambda;
	double 			omega0;
	double			delta_i;
	double			w;
	double			ecc;
	double			dT;
	double			deltaT;
};

class GlonassAlmanac : public Almanac {

public:
	GlonassAlmanac() : almanac(SatGlonass::NUM_GLONASS_SAT) {}
	void predicationSatellitePosition(const Time& curTime, Satellite& satellite) const override;
	SatelliteGlonassAlmanac* getAlmanacByPRN(int prn) { return &almanac[prn - 1]; }
	static const double	EARTH_RADIUS_GLON;	// m
	static const double GRAV_CONSTANT_GLON;	// m^3/s^2
	static const double	I0;					// rad		
	static const int	T0;					// s
	static const double C20;
	static const double	PZ90_OE;			// rad/s
	enum { DAYS_IN_FOUR_YEARS = 1461 };

private:

	std::vector<SatelliteGlonassAlmanac> almanac;
};

#endif
