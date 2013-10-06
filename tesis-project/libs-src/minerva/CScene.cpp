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

    class CEventKey *evtKeyCurrent;
};

//---------------------------------------------------------------

static struct SPrvDataPrivateScene *prv_createScene(class CArrayRef<CAgent> **initialGeneration,
        class CArrayRef<CAgent> **currentGeneration, class CEventKey **evtKeyCurrent)
{
    struct SPrvDataPrivateScene *dataPrivate;
    
    dataPrivate = new SPrvDataPrivateScene;

    dataPrivate->initialGeneration = ASSIGN_PP_NO_NULL(initialGeneration, class CArrayRef<CAgent>);
    dataPrivate->currentGeneration = ASSIGN_PP(currentGeneration, class CArrayRef<CAgent>);
    dataPrivate->evtKeyCurrent = ASSIGN_PP(evtKeyCurrent, class CEventKey);
    
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

    if ((*dataPrivate)->evtKeyCurrent != NULL)
        DELETE_OBJECT(&(*dataPrivate)->evtKeyCurrent, class CEventKey);

    DELETE_OBJECT(dataPrivate, SPrvDataPrivateScene);
}

//---------------------------------------------------------------

CScene::CScene()
{
    class CArrayRef<CAgent> *initialGeneration;
    class CArrayRef<CAgent> *currentGeneration;
    class CEventKey *evtKey;

    initialGeneration = new CArrayRef<CAgent>;
    currentGeneration = NULL;
    evtKey = NULL;

    m_dataPrivate = prv_createScene(&initialGeneration, &currentGeneration, &evtKey);
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

void CScene::appendKey(const struct EvtKey_t *evtKey)
{
    assert_no_null(m_dataPrivate);

    if (m_dataPrivate->evtKeyCurrent != NULL)
        delete m_dataPrivate->evtKeyCurrent;

    m_dataPrivate->evtKeyCurrent = new CEventKey(evtKey);
}

//---------------------------------------------------------------

static void prv_nextGeneration(class CScene *scene, class CEventKey **evtKeyCurrent,
        class CArrayRef<CAgent> **currentGeneration)
{
    class CCollectionEventsSystem *allEvents;

    assert_no_null(evtKeyCurrent);

    allEvents = new CCollectionEventsSystem(scene);

    if (*evtKeyCurrent != NULL)
    {
        class CEventSystem *evtSystemKey;

        evtSystemKey = ASSIGN_PP_NO_NULL(evtKeyCurrent, class CEventKey);
        allEvents->appendEventSystem(&evtSystemKey);
    }

    CAgent::nextGeneration(allEvents, currentGeneration);

    DELETE_OBJECT(&allEvents, class CCollectionEventsSystem);
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

    prv_nextGeneration(this, &m_dataPrivate->evtKeyCurrent, &currentGeneration);

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
