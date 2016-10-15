#ifndef ALMANAC_H_
#define ALMANAC_H_
#include <string> 
#include "satellite.h" 
#include "position.h"
#include "sv_time.h"

class Almanac;

class Strategy {

public:
	virtual ~Strategy() {}
	virtual void read_alm(const std::string& name, Almanac* alm) = 0;
};

class ReadYumaGpsAlm : public Strategy {

public:
	~ReadYumaGpsAlm() {}
	void read_alm(const std::string& name, Almanac* alm) override;
};

class ReadGlnAlm : public Strategy {

public:
	~ReadGlnAlm() {}
	void read_alm(const std::string& name, Almanac* alm) override;
};

class Almanac {

public:
	virtual void sv_pos_predication(const Time& cur_time, Satellite& sat) const = 0;
	void read_alm(const std::string& name) { strategy->read_alm(name, this); }
	void setStrategy(Strategy* s)
	{
		strategy = s;
	}
private:
	Strategy* strategy = nullptr;

};

#endif
