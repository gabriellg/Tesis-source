// Define un arbol.

#include "CTreeActor.inl"

#include "CMetereoUtils.inl"
#include "CTreeBurning.inl"
#include "CEventBurningTree.inl"

#include "asrtbas.h"
#include "memory.h"
#include "CCollectionEventsSystem.hpp"
#include "CMultiObject3d.hpp"

struct SPrvDataPrivateTree
{
    double xPos, yPos;
    bool isBurning;
};

//---------------------------------------------------------------

static struct SPrvDataPrivateTree *prv_create(double xPos, double yPos, bool isBurning)
{
    struct SPrvDataPrivateTree *dataPrivate;
    
    dataPrivate = MALLOC(struct SPrvDataPrivateTree);
    
    dataPrivate->xPos = xPos;
    dataPrivate->yPos = yPos;
    dataPrivate->isBurning = isBurning; 
    
    return dataPrivate;
}

//---------------------------------------------------------------

static void prv_destroy(struct SPrvDataPrivateTree **dataPrivate)
{
    FREE_T(dataPrivate, struct SPrvDataPrivateTree);
}

//-----------------------------------------------------------------------

CTreeActor::CTreeActor(double xPos, double yPos)
{
    bool isBurning;
    
    isBurning = false;
    m_dataPrivate = prv_create(xPos, yPos, isBurning);
}

//---------------------------------------------------------------

CTreeActor::~CTreeActor()
{
    prv_destroy(&m_dataPrivate);    
}

//-----------------------------------------------------------------------

class CAgent *CTreeActor::evolution(class CCollectionEventsSystem *allEvents)
{
    class CAgent *agentReturned;
    class CEventSystem *evtSystem;

    assert_no_null(m_dataPrivate);

    if (allEvents->existEventSystem(CEventBurningTree::ID_EVENT, &evtSystem) == true)
    {
        class CEventBurningTree *eventBurning;

        eventBurning = dynamic_cast<class CEventBurningTree *>(evtSystem);
        assert_no_null(eventBurning);

        if (eventBurning->isSamePosition(m_dataPrivate->xPos, m_dataPrivate->yPos) == true)
        {
            assert(m_dataPrivate->isBurning == false);
            m_dataPrivate->isBurning = true;

            agentReturned = new CTreeBurning(m_dataPrivate->xPos, m_dataPrivate->yPos);
        }
        else
            agentReturned = this;
    }
    else
        agentReturned = this;

    return agentReturned;
}

//-----------------------------------------------------------------------

class CAgent *CTreeActor::representation(class CTypeDescription *evtDraw)
{
    return CMetereoUtils::descriptionTree(m_dataPrivate->xPos, m_dataPrivate->yPos, 1.);
}
