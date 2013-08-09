//CGeneratorMove.cpp

#include "CGeneratorTraslate.hpp"

#include "asrtbas.h"
#include "evtkey.tlh"

#include "CEventKey.hpp"
#include "CEventTraslate.hpp"
#include "CCollectionEventsSystem.hpp"

//---------------------------------------------------------------

void CGeneratorTraslate::generateEventWithKey(const class CEventKey *evtKey, class CCollectionEventsSystem *events)
{
	unsigned int key;
	class CEventSystem *evtTraslate;
	
	assert_no_null(evtKey);

	key = evtKey->key();
	
	switch(key)
	{
		case EVT_Left:
			
			evtTraslate = CEventTraslate::createLeft();
			break;
			
		case EVT_Up:
			
			evtTraslate = CEventTraslate::createUp();
			break;
			
		case EVT_Right:
			
			evtTraslate = CEventTraslate::createRight();
			break;
			
		case EVT_Down:
			
			evtTraslate = CEventTraslate::createDown();
			break;
			
		default:
			
			evtTraslate = NULL;
			break;
	}

	if (evtTraslate != NULL)
		events->appendEventSystem(&evtTraslate);
}
