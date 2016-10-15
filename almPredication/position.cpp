#include <cmath>
#include "position.h"	
void Position::XYZ2BLH(void) const {
				
		double e, ee, b, p, tetha, stet, ctet,lat, sinLat, numerator, denumenat, N;

		e = get_ecc()*get_ecc();
		ee = e/(1.0 - e);

		b = get_major_radius()*sqrt(1 - e);

		p = sqrt(m_X * m_X + m_Y * m_Y);
		tetha = atan(m_Z/(p*sqrt(1 - e)));
		stet = sin(tetha);
		ctet = cos(tetha);
		numerator = m_Z + ee*b*stet*stet*stet;
		denumenat = p - e * get_major_radius() * ctet*ctet*ctet;
		lat = atan ( numerator/denumenat);
		sinLat = sin(lat);
		N = get_major_radius()/sqrt(1 - e*sinLat*sinLat);

		m_B = lat;
		m_L = atan2(m_Y, m_X);
		m_H = p/cos(lat)- N;

	}

	double Position::getB() const {
		if (m_calc_BLH == false) {
				XYZ2BLH();
			m_calc_BLH = true;
		}
		return m_B;

	}

	double Position::getL() const {
		if (m_calc_BLH == false) {
				XYZ2BLH();
			m_calc_BLH = true;
		}
		return m_L;

	}

	double Position::getH() const {
		if (m_calc_BLH == false) {
				XYZ2BLH();
			m_calc_BLH = true;
		}
		return m_H;

	}