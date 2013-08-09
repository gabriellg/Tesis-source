#include "CRcpOpcRunGraph.hpp"

#include "CEvtOpcAreaDib.hpp"
#include "CVisionDataWorkspace.hpp"

#include "asrtbas.h"

//-----------------------------------------------------------------------

void CRcpOpcRunGraph::action(class CEvtOpcAreaDib *evtAreaDib)
{
	CVisionDataWorkspace *dataWorkspaceVision;
	
	dataWorkspaceVision = (CVisionDataWorkspace *)evtAreaDib->getDataWorkspace();
	assert_no_null(dataWorkspaceVision);
	
	dataWorkspaceVision->runProcessImages();
	evtAreaDib->setRedraw();
}
