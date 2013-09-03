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

class CAgent *CAgentGeneratorEvent::representation(class CTypeDescription *evtDescription)
{
    return NULL;
}

//-----------------------------------------------------------------------

void CAgentGeneratorEvent::beginRepresentation(class CTypeDescription *evtDescription)
{
    assert_message("Not Representation");
}

//-----------------------------------------------------------------------

void CAgentGeneratorEvent::drawRepresentation(class CTypeDescription *evtDescription)
{
    assert_message("Not Representation");
}

//-----------------------------------------------------------------------

void CAgentGeneratorEvent::endRepresentation(class CTypeDescription *evtDescription)
{
    assert_message("Not Representation");
}
