//robots.c

#include "CApplicationWorldVirtual.hpp"
#include "CSimulationRobot.inl"

#include "CString.hpp"

//-----------------------------------------------------------------------

int main(int argc, char *argv[])
{
	class CApplicationWorldVirtual *appSimulationRobot;
    class CSimulationRobot *simulationRobot;
    bool is3d;

    is3d = true;
    if (argc >= 2)
    {
        if (CString::equal(argv[1], "Sprite") == true)
            is3d = false;
    }
    
    simulationRobot = new CSimulationRobot(is3d);
	
    appSimulationRobot = new CApplicationWorldVirtual(simulationRobot, "Simulation Robots");
	
    appSimulationRobot->run(argc, argv);
	
	delete appSimulationRobot;
		
	return 0;
}

