#include "CCollectionEventsSystem.hpp"

#include "CEventSystem.hpp"

#include "asrtbas.h"
#include "memory.h"

#include "CArray.hpp"

struct SPrvDataPrivateCollectionEventSystem
{
	const class CScene *scene;
    class CArray<CEventSystem> *allEvents;
};

//---------------------------------------------------------------

static void prv_integrityCollectionEventSystem(const struct SPrvDataPrivateCollectionEventSystem *dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(dataPrivate->allEvents);
}

//---------------------------------------------------------------

static struct SPrvDataPrivateCollectionEventSystem *prv_create(
							const class CScene *scene,
							class CArray<CEventSystem> **allEvents)
{
    struct SPrvDataPrivateCollectionEventSystem *dataPrivate;
    
    dataPrivate = MALLOC(struct SPrvDataPrivateCollectionEventSystem);
    
    dataPrivate->scene = scene;
    dataPrivate->allEvents = ASSIGN_PP_NO_NULL(allEvents, class CArray<CEventSystem>);
    
    prv_integrityCollectionEventSystem(dataPrivate);

    return dataPrivate;
}

//---------------------------------------------------------------

static void prv_destroy(struct SPrvDataPrivateCollectionEventSystem **dataPrivate)
{
    assert_no_null(dataPrivate);
    prv_integrityCollectionEventSystem(*dataPrivate);
    
    DELETE_OBJECT(&(*dataPrivate)->allEvents, class CArray<CEventSystem>);
    DELETE_OBJECT(dataPrivate, struct SPrvDataPrivateCollectionEventSystem);
}

//---------------------------------------------------------------

CCollectionEventsSystem::CCollectionEventsSystem(const class CScene *scene)
{
    class CArray<CEventSystem> *allEvents;
    
    allEvents = new CArray<CEventSystem>;
    m_dataPrivate = prv_create(scene, &allEvents);
}

//---------------------------------------------------------------

CCollectionEventsSystem::~CCollectionEventsSystem()
{
    prv_destroy(&m_dataPrivate);
}

//---------------------------------------------------------------

void CCollectionEventsSystem::appendEventSystem(class CEventSystem **eventSystem)
{
    assert_no_null(m_dataPrivate);
    assert_no_null(eventSystem);
    
    m_dataPrivate->allEvents->add(*eventSystem);
    *eventSystem = NULL;
}

//---------------------------------------------------------------

static bool prv_isEvent(const class CEventSystem *evtSystem, const char *idSystem)
{
    assert_no_null(evtSystem);
    return evtSystem->isTypeEvent(idSystem);
}

//---------------------------------------------------------------

bool CCollectionEventsSystem::existEventSystem(const char *idEvent, class CEventSystem **evtSystem_opt)
{
    bool exist;
    unsigned long indPos;

    assert_no_null(m_dataPrivate);

    exist = m_dataPrivate->allEvents->existOnlyOneElement<char>(idEvent, prv_isEvent, &indPos);

    if (exist == true && evtSystem_opt != NULL)
        *evtSystem_opt = m_dataPrivate->allEvents->get(indPos);

    return exist;
}

//---------------------------------------------------------------

class CArrayRef<CEventSystem> *CCollectionEventsSystem::allTypeEvent(const char *idEvent) const
{
    assert_no_null(m_dataPrivate);
    return m_dataPrivate->allEvents->search<char>(idEvent, prv_isEvent);
}

//---------------------------------------------------------------

const class CArray<CEventSystem> *CCollectionEventsSystem::getEvents(void) const
{
    return m_dataPrivate->allEvents;
}

//---------------------------------------------------------------

const class CScene *CCollectionEventsSystem::getScene() const
{
	return m_dataPrivate->scene;
}
