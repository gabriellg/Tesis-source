/* -----------------------------------------------------------------------------
 *  CAgentGeneratorEvent.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CAgentGeneratorEvent.hpp"

#include "asrtbas.h"

//-----------------------------------------------------------------------

class CAgent *CAgentGeneratorEvent::evolution(class CCollectionEventsSystem *allEvents)
{
    return this;
}

//-----------------------------------------------------------------------

class CArray<IObjectDraw> *CAgentGeneratorEvent::createRepresentation(class CTypeDescription *evtDescription,
        class CArray<IObjectDraw> **childsOpt)
{
    assert_no_null(childsOpt);

    return *childsOpt;
}
