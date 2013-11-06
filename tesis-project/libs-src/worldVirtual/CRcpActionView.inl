// Receptor que situa la acción en el Plano XZ.

#include "CRcpOpcAction.hpp"

class CRcpActionInitialPosition : public CRcpOpcAction
{
    public:
    
    	CRcpActionInitialPosition(void) : CRcpOpcAction("Posicion inicial") {;} 
        
        virtual void action(class CEvtOpcAreaDib *evtAreaDib);
      
};
