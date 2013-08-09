//robots.c

#include "CApplicationWorldVirtual.hpp"
#include "CPlanetRobots.hpp"

//-----------------------------------------------------------------------

int main(int argc, char *argv[])
{
	CApplicationWorldVirtual *appPlanetRobots;
    CPlanetRobots *planetRobots;
    
    planetRobots = new CPlanetRobots();
	
    appPlanetRobots = new CApplicationWorldVirtual(planetRobots, "Planet Robots");
	
    appPlanetRobots->run(argc, argv);
	
	delete appPlanetRobots;
		
	return 0;
}

