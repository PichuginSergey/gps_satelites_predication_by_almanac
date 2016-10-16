#ifndef SATELITE_H_
#define SATELITE_H_
#include <vector>
#include "position.h" 

class Satellite {

public:
	explicit Satellite(int id) : prn_(id) {}
	int getPrn() const {return prn_;}
	Coordinate		coordinate;
	Velocity		velocity;
	Angle			angle;

private:
	int prn_;
};

class GNSS {

public:
	explicit GNSS (size_t size) {
		for (size_t i = 0; i < size; ++i)
			satellites_.push_back(Satellite(i+1));
	}
	size_t size() const {return satellites_.size();}
	Satellite& operator[](size_t idx) {return satellites_[idx];}
	const Satellite& operator[](size_t idx) const {return satellites_[idx];}
	virtual std::string name() const = 0;
	auto begin() { return satellites_.begin() ;}
	auto end() { return satellites_.end(); }
	auto cbegin() const { return satellites_.cbegin(); }
	auto cend() const { return satellites_.cend(); }

protected:
	std::vector<Satellite> satellites_;
};

class SatellitesGlonass : public GNSS {

public:
	SatellitesGlonass() : GNSS(NUMBER_GLONASS_SATELLITE) {}
	enum { NUMBER_GLONASS_SATELLITE = 24};
	std::string name() const override {return "Glonass";}
};

class SatellitesGps : public GNSS {

public:
	SatellitesGps() : GNSS(NUMBER_GPS_SATELLITE){}
	enum { NUMBER_GPS_SATELLITE = 32};
	std::string name()  const override {return "Gps";}
};

#endif