// Receptores para las opciones de animacion.

#include "CRcpActionAnimation.inl"

#include "CDataWorkspaceWorldVirtual.inl"

#include "asrtbas.h"
#include "CEvtOpcAreaDib.hpp"

//---------------------------------------------------------------

void CRcpActionStop::action(class CEvtOpcAreaDib *evtAreaDib)
{
    class CDataWorkspaceWorldVirtual *dataWorkspaceWorld;
    
    dataWorkspaceWorld = (class CDataWorkspaceWorldVirtual *)evtAreaDib->getDataWorkspace();
    assert_no_null(dataWorkspaceWorld);
    
    dataWorkspaceWorld->stop();
    evtAreaDib->setRedraw();
}
       
//---------------------------------------------------------------

void CRcpActionPause::action(class CEvtOpcAreaDib *evtAreaDib)
{
	;
}
