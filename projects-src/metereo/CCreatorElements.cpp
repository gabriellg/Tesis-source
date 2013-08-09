// Define el actor que genera árboles.

#include "CCreatorElements.inl"

#include "CEventGrowningTree.inl"
#include "CEventStorm.inl"
#include "CTreeGrowing.inl"
#include "CLightning.inl"
#include "CProbabilityTable.inl"

#include "asrtbas.h"
#include "memory.h"
#include "CCollectionEventsSystem.hpp"
#include "CMultiObject3d.hpp"

//---------------------------------------------------------------

static void prv_getPosition(const class CEventSystem *evtSystem, double *xPos, double *yPos)
{
    const class CEventPosition *evtPosition;

    evtPosition = dynamic_cast<const class CEventPosition *>(evtSystem);
    assert_no_null(evtPosition);

    evtPosition->getPosition(xPos, yPos);
}

//---------------------------------------------------------------

class CAgent *CCreatorElements::evolution(class CCollectionEventsSystem *allEvents)
{
    class CAgent *agentReturned;
    class CAgent *agentTree, *agentLightning;
    class CEventSystem *evtSystem;

    if (allEvents->existEventSystem(CEventGrowningTree::ID_EVENT, &evtSystem) == true)
    {
        double xPos, yPos;

        prv_getPosition(evtSystem, &xPos, &yPos);
        agentTree = new CTreeGrowing(xPos, yPos);
    }
    else
        agentTree = NULL;

    if (allEvents->existEventSystem(CEventStorm::ID_EVENT, &evtSystem) == true)
    {
        double xPos, yPos;

        prv_getPosition(evtSystem, &xPos, &yPos);
        agentLightning = new CLightning(xPos, yPos);
    }
    else
        agentLightning = NULL;

    if (agentLightning == NULL && agentTree == NULL)
        agentReturned = this;
    else
    {
        class CAgent *agentThis;

        agentThis = this;
        agentReturned = new CMultiObject3d();

        if (agentLightning != NULL)
            agentReturned->appendChild(&agentLightning);

        if (agentTree != NULL)
            agentReturned->appendChild(&agentTree);

        agentReturned->appendChild(&agentThis);
    }

    return agentReturned;
}

//---------------------------------------------------------------

class CAgent *CCreatorElements::representation(class CTypeDescription *evtDraw)
{
	return NULL;
}
