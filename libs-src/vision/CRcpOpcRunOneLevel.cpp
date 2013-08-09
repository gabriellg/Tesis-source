#include "CRcpOpcRunOneLevel.hpp"

#include "CVisionDataWorkspace.hpp"
#include "CEvtOpcAreaDib.hpp"

#include "asrtbas.h"

//-----------------------------------------------------------------------

void CRcpOpcRunOneLevel::action(class CEvtOpcAreaDib *evtAreaDib)
{
	CVisionDataWorkspace *dataWorkspaceVision;
	
	dataWorkspaceVision = (CVisionDataWorkspace *)evtAreaDib->getDataWorkspace();
	assert_no_null(dataWorkspaceVision);
	
	dataWorkspaceVision->runNextLevel();
	evtAreaDib->setRedraw();
}
