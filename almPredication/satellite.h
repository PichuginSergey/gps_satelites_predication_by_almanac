#ifndef SATELITE_H_
#define SATELITE_H_
#include <vector>
#include "position.h" 

class Satellite {

public:
	Satellite(int id) : prn(id) {}
	int get_prn() const {return prn;}
	Coordinate		coord;
	Velocity		vel;
	Angle			ang;
private:

	int prn;
};

class GNSS {

public:
	GNSS (size_t size) {
		for (size_t i = 0; i < size; ++i)
			sat.push_back(i+1);
	}
	size_t size() const {return sat.size();}
	Satellite& operator[](size_t idx) {return sat[idx];}
	const Satellite& operator[](size_t idx) const {return sat[idx];}
	virtual std::string name() const = 0;
protected:
	std::vector<Satellite> sat;
};

class SatGlonass : public GNSS {

public:
	SatGlonass() : GNSS(NUM_GLONASS_SAT) {}
	enum {NUM_GLONASS_SAT = 24};
	std::string name() const {return "Glonass";}
};

class SatGps : public GNSS {

public:
	SatGps() : GNSS(NUM_GPS_SAT){}
	enum {NUM_GPS_SAT = 32};
	std::string name() const {return "Gps";}
};

#endif