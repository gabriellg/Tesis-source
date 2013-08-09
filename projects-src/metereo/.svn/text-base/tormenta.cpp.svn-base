// Define la aplicación tormenta.

#include "CApplicationWorldVirtual.hpp"

#include "CWorldStorm.hpp"

//-----------------------------------------------------------------------

int main(int argc, char *argv[])
{
	CApplicationWorldVirtual *tormenta;
    CWorldStorm *worldStorm;
    
    worldStorm = new CWorldStorm();
	
	tormenta = new CApplicationWorldVirtual(worldStorm, "Simulacion de tormentas");
	
	tormenta->run(argc, argv);
	
	delete tormenta;
		
	return 0;
}
