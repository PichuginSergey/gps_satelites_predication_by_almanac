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
	virtual void readAlmanac(const std::string& fileName, Almanac* almanac) = 0;
};

class ReadYumaGpsAlmanac : public Strategy {

public:
	~ReadYumaGpsAlmanac() override {}
	void readAlmanac(const std::string& fileName, Almanac* almanac) override;
};

class ReadGlonassAlmanac : public Strategy {

public:
	~ReadGlonassAlmanac() override {}
	void readAlmanac(const std::string& fileName, Almanac* almanac) override;
};

class Almanac {

public:
	virtual void predicationSatellitePosition(const Time& curTime, Satellite& satellite) const = 0;
	void readAlmanac(const std::string& fileName) 
	{ 
		strategy_->readAlmanac(fileName, this); 
	}
	void setStrategy(Strategy* strategy)
	{
		strategy_ = strategy;
	}

private:
	Strategy* strategy_ = nullptr;
};

#endif
