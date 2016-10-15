#include <stdexcept>
#include <string>
#include <cmath>
#include "glonassAlmanac.h"
#include "service.h"
#include "file.h"

/* AlmanacGLN's static const */
const double GlonassAlm::EARTH_RADIUS_GLON = 6378136.0;
const double GlonassAlm::GRAV_CONSTANT_GLON = 3.986004418E14;
const double GlonassAlm::I0 = 1.09955742875643;
const int	 GlonassAlm::T0 = 43200;
const double GlonassAlm::C20 = -0.00108263;
const double GlonassAlm::PZ90_OE = 7.292115E-5;


void ReadGlnAlm::read_alm(const std::string& name, Almanac* a) {

	InputFile<std::ifstream> in(name.c_str());

	if (!in)
	{
		std::string s = "Can not open file " + name;
		throw std::runtime_error(s.c_str());
	}
	size_t idx = 0;
	GlonassAlm* alm = dynamic_cast<GlonassAlm*>(a);
	if (alm == 0)
		throw std::runtime_error("This is not GlonassAlm in ReadYumaGlnAlm");
	while (!in.eof() && idx < SatGlonass::NUM_GLONASS_SAT)
	{
		int day_recive_alm, month_recive_alm, year_recive_alm, time_recive_alm;
		in >> day_recive_alm >> month_recive_alm >> year_recive_alm >> time_recive_alm;
		int prn;
		int day, month, year;
		in >> prn;
		AlmanacGlon*	A = alm->getAlmByPRN(prn);
		in >> A->H_n >> A->svHealth >> day >> month >> year >> A->tLambda
			>> A->taus >> A->taugps >> A->taun;
		unsigned int jdn = data_to_julian(day, month, year);
		A->Na = jdn % GlonassAlm::DAYS_IN_FOUR_YEARS;
		in >> A->omega0 >> A->delta_i >> A->w >> A->ecc >> A->deltaT >> A->dT;
		// Convert to radius 
		A->omega0 *= Pi;
		A->delta_i *= Pi;
		A->w *= Pi;
		A->vflg = 1;
		++idx;
	}
}

void GlonassAlm::sv_pos_predication(const Time& cur_time, Satellite& sat) const {

	double ik, e, Tdr, n, a, l_, w_, D_t, ww_p, ok, Ep, M_p, dT_p, M, Ek, delta, E,
		sek, cek, x1o, x2o, xo1, xo2, swp, cwp, sok, cok, sik, cik, e0x1, e0y1,
		e0z1, e0x2, e0y2, e0z2;

	/* The current data in days */
	unsigned int jd = data_to_julian(cur_time.get_day(), cur_time.get_month(), cur_time.get_year());
	jd = (jd % 1461);
	/* The current time in sec */
	double sec = (cur_time.get_hour() - 0) * 3600 + cur_time.get_min() * 60 + cur_time.get_sec();

	int prn = sat.get_prn();
	const AlmanacGlon*	Glon = &alm[prn - 1];

	ik = I0 + Glon->delta_i;
	Tdr = T0 + Glon->deltaT;
	n = (double)(2.0*(Pi) / (double)Tdr);
	a = pow(GRAV_CONSTANT_GLON / (double)(n*n), 1.0 / (double)3.0);

	sik = sin(ik);
	cik = cos(ik);

	l_ = -10.0*pow(EARTH_RADIUS_GLON / a, 7.0 / (double)2.0)*cik*(Pi) / (double)(86400 * 180);

	w_ = 5.0*pow(EARTH_RADIUS_GLON / a, 7.0 / (double)2.0)*(5 * (cik*cik) - 1)*(Pi) / (double)(86400 * 180);

	D_t = 86400.0*(jd - Glon->Na) + sec - Glon->tLambda;

	ww_p = Glon->w + w_*D_t;

	ok = Glon->omega0 + (l_ - PZ90_OE)*D_t;

	e = Glon->ecc;
	Ep = 2.0*atan(tan(ww_p / 2.0)*sqrt((1.0 - e) / (double)(1.0 + e)));

	M_p = Ep - e*sin(Ep);
	dT_p = M_p / (double)n;
	if (ww_p > (Pi)) dT_p = dT_p + Tdr;

	M = n*(D_t - dT_p);
	Ek = M;
	delta = 1.0;
	while (delta>3e-13) {
		E = M + e*sin(Ek);
		delta = fabs(E - Ek);
		Ek = E;
	}

	sek = sin(Ek);
	cek = cos(Ek);
	x1o = a*(cek - e);
	x2o = a*sqrt(1.0 - e*e)*sek;
	xo1 = -(n*a*sek / (1.0 - e*cek));
	xo2 = (n*a*sqrt(1 - e*e)*cek / ((1.0 - e*cek)));

	swp = sin(ww_p);
	cwp = cos(ww_p);
	sok = sin(ok);
	cok = cos(ok);

	e0x1 = cwp*cok - swp*sok*cik;
	e0y1 = cwp*sok + swp*cok*cik;
	e0z1 = swp*sik;

	e0x2 = -swp*cok - cwp*sok*cik;
	e0y2 = -swp*sok + cwp*cok*cik;
	e0z2 = cwp*sik;

	sat.coord.xs = x1o*e0x1 + x2o*e0x2;
	sat.coord.ys = x1o*e0y1 + x2o*e0y2;
	sat.coord.zs = x1o*e0z1 + x2o*e0z2;
	sat.coord.vflg = 1;
	sat.vel.vxs = xo1*e0x1 + xo2*e0x2 + PZ90_OE*sat.coord.ys;
	sat.vel.vys = xo1*e0y1 + xo2*e0y2 - PZ90_OE*sat.coord.xs;
	sat.vel.vzs = xo1*e0z1 + xo2*e0z2;
	sat.vel.vflg = 1;

}
