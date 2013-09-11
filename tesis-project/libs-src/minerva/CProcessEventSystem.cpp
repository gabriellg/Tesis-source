// Procesa el evento del sistema si ha sucedido.

#include "CProcessEventSystem.hpp"

#include "CCollectionEventsSystem.hpp"
#include "CEventSystem.hpp"

#include "asrtbas.h"
#include "memory.h"
#include "strbas.h"

struct prv_dataPrivateProcessEventSystem_t
{
    char *nameTypeEvent;
};

//-----------------------------------------------------------------------

static struct prv_dataPrivateProcessEventSystem_t *prv_create(const char *nameTypeEvent)

{
	struct prv_dataPrivateProcessEventSystem_t *dataPrivate;
	
	dataPrivate = MALLOC(struct prv_dataPrivateProcessEventSystem_t);

	dataPrivate->nameTypeEvent = strbas_copyString(nameTypeEvent);
	
	return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroy(struct prv_dataPrivateProcessEventSystem_t **dataPrivate)
{
	assert_no_null(dataPrivate);
	assert_no_null(*dataPrivate);
	
	strbas_destroy(&(*dataPrivate)->nameTypeEvent);
	
	FREE_T(dataPrivate, struct prv_dataPrivateProcessEventSystem_t);
}

//-----------------------------------------------------------------------

CProcessEventSystem::CProcessEventSystem(const char *nameTypeEvent)
{
	m_dataPrivate = prv_create(nameTypeEvent);
}

//-----------------------------------------------------------------------

CProcessEventSystem::~CProcessEventSystem()
{
	prv_destroy(&m_dataPrivate);
}

//---------------------------------------------------------------

bool CProcessEventSystem::canProcessEvent(const class CEventSystem *evt) const
{
    assert_no_null(m_dataPrivate);
	assert_no_null(evt);

	return evt->isTypeEvent(m_dataPrivate->nameTypeEvent);
}
