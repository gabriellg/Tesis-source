#include "CScene.hpp"

#include "CAgent.hpp"
#include "CMultiObject3d.hpp"
#include "CEventKey.hpp"
#include "CCollectionEventsSystem.hpp"

#include "asrtbas.h"
#include "memory.h"
#include "CArray.hpp"


struct SPrvDataPrivateScene
{
    class CAgent *sceneInitial;
    class CAgent *sceneCurrent;

    class CEventKey *evtKeyCurrent;
};

//---------------------------------------------------------------

static struct SPrvDataPrivateScene *prv_createScene(
                        class CAgent **sceneInitial,
                        class CAgent **sceneCurrent,
                        class CEventKey **evtKeyCurrent)
{   
    struct SPrvDataPrivateScene *dataPrivate;
    
    dataPrivate = new SPrvDataPrivateScene;

    dataPrivate->sceneInitial = ASSIGN_PP_NO_NULL(sceneInitial, class CAgent);
    dataPrivate->sceneCurrent = ASSIGN_PP_NO_NULL(sceneCurrent, class CAgent);
    dataPrivate->evtKeyCurrent = ASSIGN_PP(evtKeyCurrent, class CEventKey);
    
    return dataPrivate;
}

//---------------------------------------------------------------

static void prv_destroyScene(struct SPrvDataPrivateScene **dataPrivate)
{
    class CAgent *agentScene;

    assert_no_null(dataPrivate);
    assert_no_null(*dataPrivate);

    if ((*dataPrivate)->sceneInitial != NULL)
    {
    	assert((*dataPrivate)->sceneCurrent == NULL);
    	agentScene = (*dataPrivate)->sceneInitial;
    }
    else
    {
    	assert_no_null((*dataPrivate)->sceneCurrent);
    	agentScene = (*dataPrivate)->sceneCurrent;
    }

    CAgent::deleteAllAgentNotRepeted(&agentScene);

    if ((*dataPrivate)->evtKeyCurrent != NULL)
        DELETE_OBJECT(&(*dataPrivate)->evtKeyCurrent, class CEventKey);

    DELETE_OBJECT(dataPrivate, SPrvDataPrivateScene);
} 

//---------------------------------------------------------------

CScene::CScene()
{
	class CAgent *sceneInitial;
    class CAgent *sceneCurrent;
    class CEventKey *evtKey;

	sceneInitial = new CMultiObject3d();
    sceneCurrent = NULL;
    evtKey = NULL;

    m_dataPrivate = prv_createScene(&sceneInitial, &sceneCurrent, &evtKey);
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
    assert_no_null(m_dataPrivate->sceneInitial);
    
    m_dataPrivate->sceneInitial->appendChild(agent);
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

void CScene::nextFrame()
{
    class CCollectionEventsSystem *allEvents;
    class CAgent *sceneCurrent;
    
    assert_no_null(m_dataPrivate);

    if (m_dataPrivate->sceneCurrent != NULL)
    {
    	assert(m_dataPrivate->sceneInitial == NULL);
    	sceneCurrent = m_dataPrivate->sceneCurrent;
    	m_dataPrivate->sceneCurrent = NULL;
    }
    else
    {
    	assert_no_null(m_dataPrivate->sceneInitial);
    	assert(m_dataPrivate->sceneCurrent == NULL);
    	sceneCurrent = m_dataPrivate->sceneInitial;
    	m_dataPrivate->sceneInitial = NULL;
    }
    
    allEvents = new CCollectionEventsSystem(this);

    if (m_dataPrivate->evtKeyCurrent != NULL)
    {
        class CEventSystem *evtSystemKey;

        evtSystemKey = ASSIGN_PP_NO_NULL(&m_dataPrivate->evtKeyCurrent, class CEventKey);
        allEvents->appendEventSystem(&evtSystemKey);
    }

    m_dataPrivate->sceneCurrent = sceneCurrent->nextGeneration(allEvents);
    CAgent::eraseOldObjects(m_dataPrivate->sceneCurrent, &sceneCurrent);

    delete allEvents;
}

//---------------------------------------------------------------

void CScene::processDraw(class CTypeDescription *evtDraw) const
{
    class CAgent *agentCurrent;
    
    assert_no_null(m_dataPrivate);

    if (m_dataPrivate->sceneInitial != NULL)
        agentCurrent = m_dataPrivate->sceneInitial;
    else
        agentCurrent = m_dataPrivate->sceneCurrent;
    
    if (agentCurrent != NULL)
    {
        class CArrayRef<CAgent> *stringRepresentation;
        class CAgent *representation;

        stringRepresentation = agentCurrent->getRepresentation(evtDraw);

        representation = new CMultiObject3d();
        representation->appendChildren(&stringRepresentation);

        representation->traslateRepresentationToDescription(evtDraw);

        CAgent::eraseOldObjects(agentCurrent, &representation);
    }
}
