#ifndef GPS_ALMANAC_H_
#define GPS_ALMANAC_H_
#include <vector>
#include "almanac.h"
#include "satellite.h"

struct SatelliteGpsAlmanac {
	SatelliteGpsAlmanac() : vflg(0) {}
	unsigned int	vflg;
	unsigned int	almHealth;
	int				refweek;
	double			e;
	double			toa;
	double			delta_i;
	double			omegadot;
	double			sqrta;
	double			omega0;
	double			w;
	double			m0;
	double			af0;
	double			af1;
};

class GpsAlmanac : public Almanac {

public:
	GpsAlmanac() : almanac_(SatellitesGps::NUMBER_GPS_SATELLITE) {}
	void predicationSatellitePosition(const Time& curTime, Satellite& satellite) const override;
	SatelliteGpsAlmanac* getAlmanacByPRN(int prn) { return &almanac_[prn - 1]; }
	static const double 		GRAV_CONSTANT_GPS;
	static const double			WGS84_OE;
	static const unsigned int	SECONDS_A_WEEK;
private:
	std::vector<SatelliteGpsAlmanac> almanac_;
};

#endif
