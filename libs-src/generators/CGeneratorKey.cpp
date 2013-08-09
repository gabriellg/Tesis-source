//CGeneratorKey.cpp

#include "CGeneratorKey.hpp"

#include "CEventKey.hpp"
#include "CCollectionEventsSystem.hpp"
#include "asrtbas.h"

//-----------------------------------------------------------------------

void CGeneratorKey::beginEvolution(class CCollectionEventsSystem *allEvents)
{
    class CEventSystem *eventSystem;

	assert_no_null(allEvents);
	
	if (allEvents->existEventSystem(CEventKey::ID_EVENT, &eventSystem) == true)
	{
	    class CEventKey *evtKey;

	    evtKey = dynamic_cast<class CEventKey *>(eventSystem);
	    generateEventWithKey(evtKey, allEvents);
	}
}
