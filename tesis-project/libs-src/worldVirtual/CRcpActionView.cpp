// Receptores para situar la cámara en diferentes vistas 

#include "CRcpActionView.inl"

#include "CViewWorldVirtual.inl"

#include "asrtbas.h"
#include "CEvtOpcAreaDib.hpp"

//---------------------------------------------------------------

void CRcpActionInitialPosition::action(class CEvtOpcAreaDib *evtAreaDib)
{
    class CDataWorkspaceWorldVirtual *dataWorkspaceWorld;
    class CViewWorldVirtual *viewWorld;
    
    assert_no_null(evtAreaDib);
    
    dataWorkspaceWorld = (class CDataWorkspaceWorldVirtual *)evtAreaDib->getDataWorkspace();
    assert_no_null(dataWorkspaceWorld);
    
    viewWorld = (class CViewWorldVirtual *)evtAreaDib->getViewCurrent();
    assert_no_null(viewWorld);
    
    viewWorld->setInitialPositionCamera(dataWorkspaceWorld);    
}       
