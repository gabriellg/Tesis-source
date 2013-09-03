#ifndef RCPOPCACTION_HPP_
#define RCPOPCACTION_HPP_

#include "CRcpOpcAreaDib.hpp"

class CRcpOpcAction : public CRcpOpcAreaDib
{
	public:
		
		CRcpOpcAction(const char *nameOption) : CRcpOpcAreaDib(nameOption) {;};
	
	private:

		virtual void action(class CEvtOpcAreaDib *evtAreaDib) = 0;
	
		virtual enum CRcpOpcAreaDib::Result_t processStart(class CEvtOpcAreaDib *evtAreaDib);
};

#endif /*RCPOPCACTION_HPP_*/
