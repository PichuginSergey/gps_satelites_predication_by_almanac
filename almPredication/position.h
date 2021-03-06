#ifndef POSITION_H_
#define POSITION_H_

struct Coordinate
{
	bool		vflg{ false };
	double		xs{ 0.0 };					// The X coordinate of satellite,		m. 
	double		ys{ 0.0 };					// The Y coordinate of satellite,		m.
	double		zs{ 0.0 };					// The Z coordinate of satellite,		m.
};

struct Velocity
{
	bool		vflg{ false };
	double		vxs{ 0.0 };				// The Vx velocity of satellite,		m/s. 
	double		vys{ 0.0 };				// The Vy velocity of satellite,		m/s.  
	double		vzs{ 0.0 };				// The Vz velocity of satellite,		m/s.
};

struct Angle
{
	bool		vflg{ false };
	double		elv{ 0.0 };				// Elevation angle, rad.				
	double		azi{ 0.0 };				// Azimuth, rad.	
};

class Position {
public:
	Position() : m_X(0), m_Y(0), m_Z(0),
		m_B(0), m_L(0), m_H(0), m_calc_BLH(false) {}
	Position(double X, double Y, double Z) : m_X(X), m_Y(Y), m_Z(Z),
		m_B(0), m_L(0), m_H(0), m_calc_BLH(false) {}
	double getX(void) const { return m_X; }	void setX(double X) { m_calc_BLH = false; m_X = X; }
	double getY(void) const { return m_Y; }	void setY(double Y) { m_calc_BLH = false; m_Y = Y; }
	double getZ(void) const { return m_Z; }	void setZ(double Z) { m_calc_BLH = false; m_Z = Z; }
	double getB(void) const;
	double getL(void) const;
	double getH(void) const;

private:

	/* Conversion from XYZ to BLH */
	void XYZ2BLH(void) const;
	virtual double get_ecc() const = 0;
	virtual double get_major_radius() const = 0;
	double	m_X, m_Y, m_Z;
	mutable double	m_B, m_L, m_H;
	mutable bool	m_calc_BLH;
};

/* Position of the user in WGS-84 */
class Position_WGS_84 : public Position {

public:
	Position_WGS_84() : Position(0, 0, 0) {}
	Position_WGS_84(double X, double Y, double Z) : Position(X, Y, Z) {}

private:
	double get_ecc() const { return 0.0818191908426E0; }
	double get_major_radius() const { return 6378137.0; }
};
#endif
