#include "CScene.hpp"

#include "CAgent.hpp"
#include "CEventKey.hpp"
#include "CCollectionEventsSystem.hpp"

#include "asrtbas.h"
#include "memory.h"
#include "CArray.hpp"
#include "CArrayRef.hpp"

struct SPrvDataPrivateScene
{
    class CArrayRef<CAgent> *initialGeneration;
    class CArrayRef<CAgent> *currentGeneration;

    class CCollectionEventsSystem *allEvents;
};

//---------------------------------------------------------------

static struct SPrvDataPrivateScene *prv_createScene(class CArrayRef<CAgent> **initialGeneration,
        class CArrayRef<CAgent> **currentGeneration, class CCollectionEventsSystem **allEvents)
{
    struct SPrvDataPrivateScene *dataPrivate;
    
    dataPrivate = new SPrvDataPrivateScene;

    dataPrivate->initialGeneration = ASSIGN_PP_NO_NULL(initialGeneration, class CArrayRef<CAgent>);
    dataPrivate->currentGeneration = ASSIGN_PP(currentGeneration, class CArrayRef<CAgent>);
    dataPrivate->allEvents = ASSIGN_PP(allEvents, class CCollectionEventsSystem);
    
    return dataPrivate;
}

//---------------------------------------------------------------

static void prv_destroyScene(struct SPrvDataPrivateScene **dataPrivate)
{
    class CArrayRef<CAgent> *agentScene;

    assert_no_null(dataPrivate);
    assert_no_null(*dataPrivate);

    if ((*dataPrivate)->initialGeneration != NULL)
    {
        assert((*dataPrivate)->currentGeneration == NULL);
        agentScene = (*dataPrivate)->initialGeneration;
    }
    else
    {
        assert_no_null((*dataPrivate)->currentGeneration);
        agentScene = (*dataPrivate)->currentGeneration;
    }

    CAgent::destroyAllAgentsNotRepeated(&agentScene);


    DELETE_OBJECT(&(*dataPrivate)->allEvents, class CCollectionEventsSystem);

    DELETE_OBJECT(dataPrivate, SPrvDataPrivateScene);
}

//---------------------------------------------------------------

CScene::CScene()
{
    class CArrayRef<CAgent> *initialGeneration;
    class CArrayRef<CAgent> *currentGeneration;
    class CCollectionEventsSystem *allEvents;

    initialGeneration = new CArrayRef<CAgent>;
    currentGeneration = NULL;
    allEvents = new CCollectionEventsSystem(this);

    m_dataPrivate = prv_createScene(&initialGeneration, &currentGeneration, &allEvents);
}

//---------------------------------------------------------------

CScene::~CScene()
{
    prv_destroyScene(&m_dataPrivate);
}

//---------------------------------------------------------------

void CScene::appendAgent(class CAgent **agent)
{
    assert_no_null(m_dataPrivate);
    assert_no_null(m_dataPrivate->initialGeneration);
    assert_no_null(agent);
    assert_no_null(*agent);
    
    m_dataPrivate->initialGeneration->add(*agent);
    *agent = NULL;
}

//---------------------------------------------------------------

void CScene::appendEvent(class CEventSystem **evtSystem)
{
    assert_no_null(m_dataPrivate);
    assert_no_null(m_dataPrivate->allEvents);
    m_dataPrivate->allEvents->appendEventSystem(evtSystem);
}

//---------------------------------------------------------------

void CScene::nextFrame()
{
    class CArrayRef<CAgent> *currentGeneration;
    
    assert_no_null(m_dataPrivate);

    if (m_dataPrivate->currentGeneration != NULL)
    {
        assert(m_dataPrivate->initialGeneration == NULL);
        currentGeneration = m_dataPrivate->currentGeneration;
        m_dataPrivate->currentGeneration = NULL;
    }
    else
    {
        assert_no_null(m_dataPrivate->initialGeneration);
        currentGeneration = m_dataPrivate->initialGeneration;
        m_dataPrivate->initialGeneration = NULL;
    }

    CAgent::nextGeneration(m_dataPrivate->allEvents, &currentGeneration);

    DELETE_OBJECT(&m_dataPrivate->allEvents, class CCollectionEventsSystem);
    m_dataPrivate->allEvents = new CCollectionEventsSystem(this);

    m_dataPrivate->currentGeneration = currentGeneration;
}

//---------------------------------------------------------------

void CScene::processDraw(class CTypeDescription *evtDraw) const
{
    class CArrayRef<CAgent> *agentsCurrent;

    assert_no_null(m_dataPrivate);

    if (m_dataPrivate->initialGeneration != NULL)
        agentsCurrent = m_dataPrivate->initialGeneration;
    else
        agentsCurrent = m_dataPrivate->currentGeneration;

    if (agentsCurrent != NULL)
    {
        class CArray<IObjectDraw> *stringRepresentation;

        stringRepresentation = new CArray<IObjectDraw>;

        for (unsigned long i = 0, size = agentsCurrent->size(); i < size; i++)
        {
            class CAgent *agent;
            class CArray<IObjectDraw> *stringRepresentationAgent;

            agent = agentsCurrent->get(i);
            stringRepresentationAgent = agent->getRepresentation(evtDraw);
            stringRepresentation->concatenateDestroying(&stringRepresentationAgent);
        }

        for (unsigned long i = 0, size = stringRepresentation->size(); i < size; i++)
        {
            class IObjectDraw *objectDraw;

            objectDraw = stringRepresentation->get(i);
            objectDraw->drawRepresentation(evtDraw);
        }

        DELETE_OBJECT(&stringRepresentation, class CArray<IObjectDraw>);
    }
}
