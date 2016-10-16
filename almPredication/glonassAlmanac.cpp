#include <stdexcept>
#include <string>
#include <cmath>
#include "glonassAlmanac.h"
#include "service.h"
#include "file.h"

/* AlmanacGLN's static const */
const double GlonassAlmanac::EARTH_RADIUS_GLON = 6378136.0;
const double GlonassAlmanac::GRAV_CONSTANT_GLON = 3.986004418E14;
const double GlonassAlmanac::I0 = 1.09955742875643;
const int	 GlonassAlmanac::T0 = 43200;
const double GlonassAlmanac::C20 = -0.00108263;
const double GlonassAlmanac::PZ90_OE = 7.292115E-5;

void GlonassAlmanac::predicationSatellitePosition(const Time& curTime, Satellite& satellite) const 
{
	/* The current data in days */
	unsigned int jd = convertDateToJulianFormat(curTime.getDay(), curTime.getMonth(), curTime.getYear());
	jd = (jd % 1461);
	/* The current time in sec */
	double sec = (curTime.getHour() - 0) * 3600 + curTime.getMinute() * 60 + curTime.getSecond();

	int prn = satellite.getPrn();
	const SatelliteGlonassAlmanac*	oneGlnSatAlmanac = &almanac_[prn - 1];

	double ik = I0 + oneGlnSatAlmanac->delta_i;
	double Tdr = T0 + oneGlnSatAlmanac->deltaT;
	double n = (double)(2.0*(Pi) / (double)Tdr);
	double a = pow(GRAV_CONSTANT_GLON / (double)(n*n), 1.0 / (double)3.0);

	double sik = sin(ik);
	double cik = cos(ik);

	double l_ = -10.0*pow(EARTH_RADIUS_GLON / a, 7.0 / (double)2.0)*cik*(Pi) / (double)(86400 * 180);

	double w_ = 5.0*pow(EARTH_RADIUS_GLON / a, 7.0 / (double)2.0)*(5 * (cik*cik) - 1)*(Pi) / (double)(86400 * 180);

	double D_t = 86400.0*(jd - oneGlnSatAlmanac->Na) + sec - oneGlnSatAlmanac->tLambda;

	double ww_p = oneGlnSatAlmanac->w + w_*D_t;

	double ok = oneGlnSatAlmanac->omega0 + (l_ - PZ90_OE)*D_t;

	double e = oneGlnSatAlmanac->ecc;
	double Ep = 2.0*atan(tan(ww_p / 2.0)*sqrt((1.0 - e) / (double)(1.0 + e)));

	double M_p = Ep - e*sin(Ep);
	double dT_p = M_p / (double)n;
	if (ww_p > (Pi))
		dT_p = dT_p + Tdr;

	double M = n*(D_t - dT_p);
	double Ek = M;
	double delta{ 1.0 };
	while (delta>3e-8) 
	{
		double E = M + e*sin(Ek);
		delta = fabs(E - Ek);
		Ek = E;
	}

	double sek = sin(Ek);
	double cek = cos(Ek);
	double x1o = a*(cek - e);
	double x2o = a*sqrt(1.0 - e*e)*sek;
	double xo1 = -(n*a*sek / (1.0 - e*cek));
	double xo2 = (n*a*sqrt(1 - e*e)*cek / ((1.0 - e*cek)));

	double swp = sin(ww_p);
	double cwp = cos(ww_p);
	double sok = sin(ok);
	double cok = cos(ok);

	double e0x1 = cwp*cok - swp*sok*cik;
	double e0y1 = cwp*sok + swp*cok*cik;
	double e0z1 = swp*sik;

	double e0x2 = -swp*cok - cwp*sok*cik;
	double e0y2 = -swp*sok + cwp*cok*cik;
	double e0z2 = cwp*sik;

	satellite.coordinate.xs = x1o*e0x1 + x2o*e0x2;
	satellite.coordinate.ys = x1o*e0y1 + x2o*e0y2;
	satellite.coordinate.zs = x1o*e0z1 + x2o*e0z2;
	satellite.coordinate.vflg = 1;
	satellite.velocity.vxs = xo1*e0x1 + xo2*e0x2 + PZ90_OE*satellite.coordinate.ys;
	satellite.velocity.vys = xo1*e0y1 + xo2*e0y2 - PZ90_OE*satellite.coordinate.xs;
	satellite.velocity.vzs = xo1*e0z1 + xo2*e0z2;
	satellite.velocity.vflg = 1;

}

void ReadGlonassAlmanac::readAlmanac(const std::string& fileName, Almanac* almanac)
{
	InputFile<std::ifstream> in(fileName.c_str());

	if (!in)
	{
		std::string s = "Can not open file " + fileName;
		throw std::runtime_error(s.c_str());
	}
	size_t idx = 0;
	GlonassAlmanac* alm = dynamic_cast<GlonassAlmanac*>(almanac);
	if (alm == nullptr)
		throw std::runtime_error("This is not GlonassAlm in ReadYumaGlnAlm");
	while (!in.eof() && idx < SatellitesGlonass::NUMBER_GLONASS_SATELLITE)
	{
		int day_recive_alm, month_recive_alm, year_recive_alm, time_recive_alm;
		in >> day_recive_alm >> month_recive_alm >> year_recive_alm >> time_recive_alm;
		int prn;
		int day, month, year;
		in >> prn;
		SatelliteGlonassAlmanac*	A = alm->getAlmanacByPRN(prn);
		in >> A->H_n >> A->svHealth >> day >> month >> year >> A->tLambda
			>> A->taus >> A->taugps >> A->taun;
		unsigned int jdn = convertDateToJulianFormat(day, month, year);
		A->Na = jdn % GlonassAlmanac::DAYS_IN_FOUR_YEARS;
		in >> A->omega0 >> A->delta_i >> A->w >> A->ecc >> A->deltaT >> A->dT;
		// Convert to radius 
		A->omega0 *= Pi;
		A->delta_i *= Pi;
		A->w *= Pi;
		A->vflg = 1;
		++idx;
	}
}
