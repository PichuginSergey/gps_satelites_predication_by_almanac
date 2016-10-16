#include <iostream>
#include <cmath>
#include <future>
#include "glonassAlmanac.h"
#include "gpsAlmanac.h"
#include "satellite.h"
#include "service.h"
#include "position.h"

int main(int argc, char* argv[]) 
{
	try
	{
		Position_WGS_84 position;
		get—urrentPosition("position.ini", position);
		Time	currentTime;
		getCurrentTime(currentTime);

		GpsAlmanac almanacGps;
		ReadYumaGpsAlmanac    readYumaGpsAlmanac;
		almanacGps.setStrategy(&readYumaGpsAlmanac);
		almanacGps.readAlmanac("MCCT_160825.agp");
		SatellitesGps satellitesGps;

		GlonassAlmanac almanacGlonass;
		ReadGlonassAlmanac readGlonassnAlmanac;
		almanacGlonass.setStrategy(&readGlonassnAlmanac);
		almanacGlonass.readAlmanac("MCCT_160825.agl");
		SatellitesGlonass satellitesGlonass;

		auto gpsAlmanacPredicatonTask = std::async(Alm_predication_task(position, currentTime, almanacGps, satellitesGps));
		auto glonassAlmanacPredicatonTask = std::async(Alm_predication_task(position, currentTime, almanacGlonass, satellitesGlonass));

		const int ELVATION_THRESHOLD{ 10 };
		gpsAlmanacPredicatonTask.get();
		displaySatellite("GPS satellites: ", satellitesGps, ELVATION_THRESHOLD);
		glonassAlmanacPredicatonTask.get();
		displaySatellite("GLNONASS satellites: ", satellitesGlonass, ELVATION_THRESHOLD);
	}
	catch (std::runtime_error& er) {
		std::cout << er.what() << std::endl;
	}
	return 0;
}