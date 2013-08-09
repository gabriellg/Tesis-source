// Animación de arbol creciendo.

#include "CTreeGrowing.inl"

#include "CMetereoUtils.inl"
#include "CTreeActor.inl"
#include "CEventGrowningTree.inl"

#include "asrtbas.h"
#include "memory.h"
#include "CEventTime.hpp"
#include "CTransform.hpp"
#include "CCollectionEventsSystem.hpp"
#include "CMultiObject3d.hpp"

static const unsigned long i_NUM_FRAMES = 20;

struct SPrvDataPrivateTreeGrowning
{
    double xPos, yPos;
    unsigned long indFrame;
};

//-----------------------------------------------------------------------

static struct SPrvDataPrivateTreeGrowning *prv_createTreeGrowning(double xPos, double yPos, unsigned long indFrame)
{
	struct SPrvDataPrivateTreeGrowning *dataPrivate;

	dataPrivate = MALLOC(struct SPrvDataPrivateTreeGrowning);

	dataPrivate->xPos = xPos;
	dataPrivate->yPos = yPos;
	dataPrivate->indFrame = indFrame;

	return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroyTreeGrowning(struct SPrvDataPrivateTreeGrowning **dataPrivate)
{
	assert_no_null(dataPrivate);
	assert_no_null(*dataPrivate);

	FREE_T(dataPrivate, struct SPrvDataPrivateTreeGrowning);
}

//---------------------------------------------------------------

CTreeGrowing::CTreeGrowing(double xPos, double yPos)
{
    unsigned long indFrame;

    indFrame = 0;
    m_dataPrivate = prv_createTreeGrowning(xPos, yPos, indFrame);
}

//---------------------------------------------------------------

CTreeGrowing::~CTreeGrowing()
{
	prv_destroyTreeGrowning(&m_dataPrivate);
}

//-----------------------------------------------------------------------

class CAgent *CTreeGrowing::evolution(class CCollectionEventsSystem *allEvent)
{
    class CAgent *agentReturned;

    assert_no_null(m_dataPrivate);

    if (allEvent->existEventSystem(CEventTime::ID_EVENT, NULL) == true)
    {
        if (m_dataPrivate->indFrame < i_NUM_FRAMES)
        {
            m_dataPrivate->indFrame++;
            agentReturned = this;
        }
        else
            agentReturned = new CTreeActor(m_dataPrivate->xPos, m_dataPrivate->yPos);
    }
    else
        agentReturned = this;

    return agentReturned;
}

//-----------------------------------------------------------------------

class CAgent *CTreeGrowing::representation(class CTypeDescription *evtDraw)
{
    double scale;

    assert_no_null(m_dataPrivate);

    scale = (double)m_dataPrivate->indFrame / (double)i_NUM_FRAMES;
    return CMetereoUtils::descriptionTree(m_dataPrivate->xPos, m_dataPrivate->yPos, scale);
}
