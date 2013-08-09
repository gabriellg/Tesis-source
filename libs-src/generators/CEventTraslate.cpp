//CEventTraslate.cpp

#include "CEventTraslate.hpp"

#include "asrtbas.h"
#include "memory.h"
#include "strbas.h"

const char *CEventTraslate::ID_EVENT = "evtTraslate";

enum prv_traslate
{
	PRV_RIGHT, PRV_LEFT, PRV_DOWN, PRV_UP  
};

struct prv_dataPrivateTraslate_t
{
	enum prv_traslate typeTraslate;
};

//-----------------------------------------------------------------------

static struct prv_dataPrivateTraslate_t *prv_create(enum prv_traslate typeTraslate)
{
	struct prv_dataPrivateTraslate_t *dataPrivate;
	
	dataPrivate = MALLOC(struct prv_dataPrivateTraslate_t);
	
	dataPrivate->typeTraslate = typeTraslate;
	
	return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroy(struct prv_dataPrivateTraslate_t **dataPrivate)
{
	FREE_T(dataPrivate, struct prv_dataPrivateTraslate_t);
}

//-----------------------------------------------------------------------

CEventTraslate::CEventTraslate(struct prv_dataPrivateTraslate_t **dataPrivate) : CEventSystem(CEventTraslate::ID_EVENT)
{
	m_dataPrivate = ASSIGN_PP_NO_NULL(dataPrivate, struct prv_dataPrivateTraslate_t);
}

//-----------------------------------------------------------------------

CEventTraslate::~CEventTraslate()
{
	prv_destroy(&m_dataPrivate);
}

//-----------------------------------------------------------------------

class CEventTraslate *CEventTraslate::createLeft(void)
{
	struct prv_dataPrivateTraslate_t *dataPrivate;

	dataPrivate = prv_create(PRV_LEFT);
	return new CEventTraslate(&dataPrivate);
}

//-----------------------------------------------------------------------

class CEventTraslate *CEventTraslate::createRight(void)
{
	struct prv_dataPrivateTraslate_t *dataPrivate;

	dataPrivate = prv_create(PRV_RIGHT);
	return new CEventTraslate(&dataPrivate);
}

//-----------------------------------------------------------------------

class CEventTraslate *CEventTraslate::createDown(void)
{
	struct prv_dataPrivateTraslate_t *dataPrivate;

	dataPrivate = prv_create(PRV_DOWN);
	return new CEventTraslate(&dataPrivate);
}

//-----------------------------------------------------------------------

class CEventTraslate *CEventTraslate::createUp(void)
{
	struct prv_dataPrivateTraslate_t *dataPrivate;

	dataPrivate = prv_create(PRV_UP);
	return new CEventTraslate(&dataPrivate);
}

//-----------------------------------------------------------------------

char *CEventTraslate::applyTraslate(
							const char *idDesireRight,
							const char *idDesireLeft,
							const char *idDesireUp,
							const char *idDesireDown) const
{
	char *idDesire;

	assert_no_null(m_dataPrivate);
	
	switch(m_dataPrivate->typeTraslate)
	{
		case PRV_RIGHT:
		{
			idDesire = strbas_copyString(idDesireRight);
			break;
		}
		case PRV_LEFT: 
		{
			idDesire = strbas_copyString(idDesireLeft);
			break;
		}
		case PRV_DOWN: 
		{
			idDesire = strbas_copyString(idDesireDown);
			break;
		}
		case PRV_UP:
		{
			idDesire = strbas_copyString(idDesireUp);
			break;
		}
		default_error();
	}

	return idDesire;
}
