// Animacion arbol ardiendo.

#include "CTreeBurning.inl"

#include "CMetereoUtils.inl"

#include "asrtbas.h"
#include "memory.h"
#include "CEventTime.hpp"
#include "CTransform.hpp"
#include "CMultiObject3d.hpp"
#include "CCollectionEventsSystem.hpp"

static const unsigned long prv_NUM_FRAMES = 10;

struct SPrvDataPrivateTreeBurning
{
    double xPos, yPos;
    unsigned long indFrame;
};

//---------------------------------------------------------------

static struct SPrvDataPrivateTreeBurning *prv_create(double xPos, double yPos, unsigned long indFrame)
{
    struct SPrvDataPrivateTreeBurning *dataPrivate;
    
    dataPrivate = MALLOC(struct SPrvDataPrivateTreeBurning);
    
    dataPrivate->xPos = xPos;
    dataPrivate->yPos = yPos;
    dataPrivate->indFrame = indFrame;
    
    return dataPrivate;
}

//---------------------------------------------------------------

static void prv_destroy(struct SPrvDataPrivateTreeBurning **dataPrivate)
{
    FREE_T(dataPrivate, struct SPrvDataPrivateTreeBurning);
}

//---------------------------------------------------------------

CTreeBurning::CTreeBurning(double xPos, double yPos)
{
    m_dataPrivate = prv_create(xPos, yPos, 0);
}

//---------------------------------------------------------------

CTreeBurning::~CTreeBurning()
{
    prv_destroy(&m_dataPrivate);
}

//---------------------------------------------------------------

class CAgent *CTreeBurning::evolution(class CCollectionEventsSystem *allEvents)
{
    class CAgent *agentReturned;

    assert_no_null(m_dataPrivate);

    if (allEvents->existEventSystem(CEventTime::ID_EVENT, NULL) == true)
    {
        if (m_dataPrivate->indFrame < prv_NUM_FRAMES)
        {
            m_dataPrivate->indFrame++;
            agentReturned = this;
        }
        else
            agentReturned = NULL;
    }
    else
        agentReturned = this;

    return agentReturned;
}

//---------------------------------------------------------------

class CAgent *CTreeBurning::representation(class CTypeDescription *evtDraw)
{
	double scale;

	assert_no_null(m_dataPrivate);

	scale = (prv_NUM_FRAMES - m_dataPrivate->indFrame)  / prv_NUM_FRAMES;
	return CMetereoUtils::descriptionTree(m_dataPrivate->xPos, m_dataPrivate->yPos, scale);
}
