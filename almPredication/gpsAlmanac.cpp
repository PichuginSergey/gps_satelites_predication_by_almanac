#include <stdexcept>
#include <cmath>
#include "gpsAlmanac.h"
#include "service.h"
#include "file.h"

const double 		GpsAlmanac::GRAV_CONSTANT_GPS = 3.986005E14;
const double		GpsAlmanac::WGS84_OE = 7.2921151467E-5;
const unsigned int	GpsAlmanac::SECONDS_A_WEEK = 604800;

void GpsAlmanac::predicationSatellitePosition(const Time& curTime, Satellite& satellite) const 
{

	int wk{0};
	double sec{0.0};

	fromUtcToGpsTime(curTime, &wk, &sec);
	wk = wk & 0xFF;
	sec -= 3 * 3600;

	int prn = satellite.get_prn();
	const SatelliteGpsAlmanac*	oneGpsSatAlmanac = &almanac[prn - 1];
	if (oneGpsSatAlmanac->vflg == 0)
		return;
	double a = oneGpsSatAlmanac->sqrta;
	a = a * a;
	double e = oneGpsSatAlmanac->e;

	signed int r = (wk - oneGpsSatAlmanac->refweek)*SECONDS_A_WEEK;
	double Tk = r + sec - oneGpsSatAlmanac->toa;

	double n0 = sqrt((double)(GRAV_CONSTANT_GPS / (a*a*a)));

	double Mk = oneGpsSatAlmanac->m0 + n0*Tk;

	double Eprev = Mk + e * sin((double)Mk);
	double Ek = Mk + e * sin(Eprev);

	while (fabs((double)(Ek - Eprev)) > 1e-13) 
	{
		Eprev = Ek;
		Ek = Mk + e * sin((double)Eprev);
	}

	double cek = cos((double)Ek);
	double sek = sin((double)Ek);
	double denom = (1 - e*cek);
	double svk = (sqrt((double)(1 - e*e))*sek) / denom;
	double cvk = (cek - e) / denom;
	double Vk = atan2((double)svk, (double)cvk);

	if (Vk < 0.0E0)
		Vk = Vk + 2.0*Pi;

	double Fk = Vk + oneGpsSatAlmanac->w;

	double Uk = Fk;

	double Rk = denom * a;

	double Ik = oneGpsSatAlmanac->delta_i;

	double cuk = cos((double)Uk);
	double suk = sin((double)Uk);

	double xpk = Rk*cuk;
	double ypk = Rk*suk;

	double omega_k = oneGpsSatAlmanac->omega0 + Tk*(oneGpsSatAlmanac->omegadot - WGS84_OE) - WGS84_OE * oneGpsSatAlmanac->toa;

	double sik = sin((double)Ik);
	double cik = cos((double)Ik);
	double sok = sin((double)omega_k);
	double cok = cos((double)omega_k);

	satellite.coord.xs = (double)(xpk*cok - ypk*cik*sok);
	satellite.coord.ys = (double)(xpk*sok + ypk*cik*cok);
	satellite.coord.zs = (double)(ypk*sik);
	satellite.coord.vflg = true;

	double p = a*(1 - e*e);
	double mup = sqrt((double)(GRAV_CONSTANT_GPS / p));
	double Vr = mup*e*sin(Vk);
	double Vu = mup*(1 + e*cos(Vk));

	satellite.vel.vxs = (double)(Vr*(cok*cuk - sok*suk*cik) - Vu*(cok*suk + sok*cuk*cik) + WGS84_OE * satellite.coord.ys);
	satellite.vel.vys = (double)(Vr*(sok*cuk + cok*suk*cik) - Vu*(sok*suk - cok*cuk*cik) - WGS84_OE * satellite.coord.xs);
	satellite.vel.vzs = (double)(Vr*suk*sik + Vu*cuk*sik);
	satellite.vel.vflg = true;
}

void ReadYumaGpsAlmanac::readAlmanac(const std::string& fileName, Almanac* almanac) 
{

	InputFile<std::ifstream> in(fileName.c_str());

	if (!in) {
		std::string s = "Can not open file " + fileName;
		throw std::runtime_error(s.c_str());
	}

	GpsAlmanac* alm = dynamic_cast<GpsAlmanac*>(almanac);
	if (alm == nullptr)
		throw std::runtime_error("This is not GpsAlm in ReadYumaGpsAlm");
	for (size_t i = 0; i < SatGps::NUM_GPS_SAT; ++i) {
		int day_recive_alm, month_recive_alm, year_recive_alm, time_recive_alm;
		in >> day_recive_alm >> month_recive_alm >> year_recive_alm >> time_recive_alm;

		int prn;
		int sec_int, day, month, year;
		double sec_double;
		in >> prn;
		SatelliteGpsAlmanac*	A = alm->getAlmanacByPRN(prn);
		in >> A->almHealth >> A->refweek >> sec_int >> day >> month >> year >> sec_double
			>> A->af0 >> A->af1 >> A->omegadot;
		in >> A->omega0 >> A->delta_i >> A->w >> A->e >> A->sqrta >> A->m0;
		A->toa = sec_int;
		A->refweek &= 0xFF;
		/* Convert to radius */
		A->omega0 *= Pi;
		A->omegadot *= Pi;
		A->delta_i *= Pi;
		A->w *= Pi;
		A->m0 *= Pi;
		A->vflg = 1;
	}
}
