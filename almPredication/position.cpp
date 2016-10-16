#include <cmath>
#include "position.h"	
void Position::XYZ2BLH(void) const 
{
	double e = get_ecc()*get_ecc();
	double ee = e/(1.0 - e);

	double b = get_major_radius()*sqrt(1 - e);

	double p = sqrt(m_X * m_X + m_Y * m_Y);
	double tetha = atan(m_Z/(p*sqrt(1 - e)));
	double stet = sin(tetha);
	double ctet = cos(tetha);
	double numerator = m_Z + ee*b*stet*stet*stet;
	double denumenat = p - e * get_major_radius() * ctet*ctet*ctet;
	double lat = atan ( numerator/denumenat);
	double sinLat = sin(lat);
	double N = get_major_radius()/sqrt(1 - e*sinLat*sinLat);

	m_B = lat;
	m_L = atan2(m_Y, m_X);
	m_H = p/cos(lat)- N;

}

double Position::getB() const 
{
	if (m_calc_BLH == false) 
	{
		XYZ2BLH();
		m_calc_BLH = true;
	}
	return m_B;

}

double Position::getL() const 
{
	if (m_calc_BLH == false) 
	{
		XYZ2BLH();
		m_calc_BLH = true;
	}
	return m_L;
}

double Position::getH() const 
{
	if (m_calc_BLH == false) 
	{
		XYZ2BLH();
		m_calc_BLH = true;
	}
	return m_H;

}