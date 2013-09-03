#ifndef CRCPOPCRUNONELEVEL_HPP_
#define CRCPOPCRUNONELEVEL_HPP_

#include "CRcpOpcAction.hpp"

class CRcpOpcRunOneLevel : public CRcpOpcAction
{
	public:
		
		CRcpOpcRunOneLevel(void) : CRcpOpcAction("Siguiente nivel") {;} 
		
		virtual void action(class CEvtOpcAreaDib *evtAreaDib);
};

#endif /*CRCPOPCRUNONELEVEL_HPP_*/
