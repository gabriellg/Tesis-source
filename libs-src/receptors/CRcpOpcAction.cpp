// Receptor de ejecution y termina.

#include "CRcpOpcAction.hpp"

#include "CEvtOpcAreaDib.hpp"
#include "asrtbas.h"

//-----------------------------------------------------------------------

enum CRcpOpcAreaDib::Result_t CRcpOpcAction::processStart(class CEvtOpcAreaDib *evtAreaDib)
{
	assert_no_null(evtAreaDib);
	assert(evtAreaDib->typeEvent() == CEvtOpcAreaDib::START);
	
	action(evtAreaDib);

	return CRcpOpcAreaDib::END;
}
