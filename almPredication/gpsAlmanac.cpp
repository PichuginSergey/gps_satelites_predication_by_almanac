#include <fstream>
#include <string>
#include <cmath>
#include "gpsAlmanac.h"
#include "time.h"
#include "service.h"

	const double 		GpsAlm::GRAV_CONSTANT_GPS = 3.986005E14;
	const double		GpsAlm::WGS84_OE =  7.2921151467E-5;
	const unsigned int	GpsAlm::SECONDS_A_WEEK = 604800;

void GpsAlm::sv_pos_predication(const Time& cur_time, Satellite& sat) const {

	int wk(0);
	double sec(0);

	utc_to_gps(cur_time, &wk, &sec);
	wk = wk&0xFF; 
	sec -= 3 * 3600;

	double a = 0, Tk, Mk, n0, Eprev, Ek, cek, sek, denom, cvk, svk, Vk,Fk,Uk,
	Rk, Ik = 0, cuk, suk, xpk, ypk, omega_k, sik, cik, sok, cok,
	p , mup, Vr, Vu;
    double  e = 0;
	int prn = sat.get_prn();
	const AlmanacGps*	A = &alm[prn-1];
	if (A->vflg == 0)
		return;
    a = A->sqrta;										
    a = a * a;
	e = A->e;
	
	/* Вычисление (в секундах) разности между временем заложения данных альманаха и текущим моментом времени (времени GPS) */
    signed int r = (wk - A->refweek)*SECONDS_A_WEEK;
	Tk = r + sec - A->toa;

    /* Расчет среднего движения (рад/с). */
    n0 = sqrt((double)(GRAV_CONSTANT_GPS/(a*a*a)));


    /* Cредняя аномалия (rads). */
    Mk = A->m0 + n0*Tk;

    /* Итерационный расчет эксцентрической аномалии. */
	Eprev = Mk + e * sin((double)Mk);
    Ek = Mk + e * sin(Eprev);

	while (fabs((double)(Ek - Eprev)) > 1e-13){
        Eprev = Ek;
        Ek = Mk + e * sin((double)Eprev);
	}

    /* Расчет истинной аномали (rads). */
	cek = cos((double)Ek);
    sek = sin((double)Ek);
	denom = (1-e*cek );
	svk = (sqrt((double)(1-e*e))*sek )/denom;
    cvk = (cek-e)/denom;
	Vk = atan2((double)svk,(double)cvk);						

	if (Vk < 0.0E0)
	Vk = Vk + 2.0*Pi;

    /* Аргумент широты (rads) */
    Fk = Vk + A->w;

    /* Скорректированный аргумент широты (рад) */
    Uk = Fk;

    /* Скорректированный радиус орбиты спутника (рад) */
    Rk = denom * a;

    /* Скорректированная наклонная орбита спутника (рад) */
	Ik = A->delta_i;

    cuk = cos((double)Uk);
    suk = sin((double)Uk);

    /* Координаты спутника в орбитальной плоскости */
    xpk = Rk*cuk;
    ypk = Rk*suk;

    /* Скорректированная долгота восходящего узла(рад) */
    omega_k  = A->omega0 + Tk*(A->omegadot - WGS84_OE) - WGS84_OE * A->toa;

    sik = sin((double)Ik);
    cik = cos((double)Ik);
    sok = sin((double)omega_k);
    cok = cos((double)omega_k);


    /* Преобразование координат в СК WGS-90 */
	sat.coord.xs = (double)(xpk*cok - ypk*cik*sok);
    sat.coord.ys = (double)(xpk*sok + ypk*cik*cok);
    sat.coord.zs = (double)(ypk*sik);
	sat.coord.vflg = true;

	/* Расчет скоростей спутников в СК WGS-90 */
	p = a*(1 - e*e);
    mup = sqrt((double)(GRAV_CONSTANT_GPS/p));
	Vr = mup*e*sin(Vk);							// Радиальная составляющая
    Vu = mup*(1 + e*cos(Vk));					// Тангенсальная составляющая

	sat.vel.vxs = (double)(Vr*(cok*cuk - sok*suk*cik) - Vu*(cok*suk + sok*cuk*cik) + WGS84_OE * sat.coord.ys);          // Поправка связанная с вращением Земли
    sat.vel.vys = (double)(Vr*(sok*cuk + cok*suk*cik) - Vu*(sok*suk - cok*cuk*cik) - WGS84_OE * sat.coord.xs);
    sat.vel.vzs = (double)(Vr*suk*sik + Vu*cuk*sik);
	sat.vel.vflg = true;
}

void GpsAlm::read_alm(const std::string& name){
	std::ifstream	in(name.c_str());
	if (in == NULL ) {
		std::string s = "Can not open file " + name;
		throw std::exception(s.c_str());
	}
	
	for (size_t i = 0; i < alm.size(); ++i) {
		int day_recive_alm, month_recive_alm, year_recive_alm, time_recive_alm;
		in >> day_recive_alm >> month_recive_alm >> year_recive_alm >> time_recive_alm;
		
		int prn;
		int sec_int, day, month, year;
		double sec_double;
		in >> prn;
		AlmanacGps*	A = &alm[prn-1];
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
	in.close();
}