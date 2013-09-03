//CEventCreateActor: Event to create actors.

#include "CEventCreateActor.inl"

#include "CWorldGeometry2DPixels.hpp"

#include "ICreatorActors.hpp"
#include "asrtbas.h"
#include "memory.h"
#include "CString.hpp"

const char *CEventCreateActor::ID_EVENT = "CREATE";

struct SPrvDataPrivateEventCreateActor
{
	char *typeActor;
	long x, y;
	class ICreatorActors *createActors;
	class CWorldGeometry2DPixels *worldGeometry2DPixels;
};

//-----------------------------------------------------------------------

static struct SPrvDataPrivateEventCreateActor *prv_createEventCreateActor(
				const char *typeActor,
				long x, long y,
				class ICreatorActors *createActors,
				class CWorldGeometry2DPixels *worldGeometry2DPixels)
{
    struct SPrvDataPrivateEventCreateActor *dataPrivate;

	dataPrivate = new SPrvDataPrivateEventCreateActor;

	dataPrivate->typeActor = CString::copy(typeActor);

	dataPrivate->x = x;
	dataPrivate->y = y;

	dataPrivate->createActors = createActors;
	dataPrivate->worldGeometry2DPixels = worldGeometry2DPixels;

	return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroyEventCreateActor(struct SPrvDataPrivateEventCreateActor **dataPrivate)
{
	assert_no_null(dataPrivate);
	assert_no_null(*dataPrivate);

	CString::free(&(*dataPrivate)->typeActor);

	DELETE_OBJECT(dataPrivate, struct SPrvDataPrivateEventCreateActor);
}

//-----------------------------------------------------------------------

CEventCreateActor::CEventCreateActor(
						const char *typeActor,
						long x, long y,
						class ICreatorActors *createActors,
						class CWorldGeometry2DPixels *worldGeometry2DPixels) : CEventSystem(ID_EVENT)
{
	m_dataPrivate = prv_createEventCreateActor(typeActor, x, y, createActors, worldGeometry2DPixels);
}

//-----------------------------------------------------------------------

CEventCreateActor::~CEventCreateActor()
{
	prv_destroyEventCreateActor(&m_dataPrivate);
}

//-----------------------------------------------------------------------

bool CEventCreateActor::createActor(class CAgent **actorCreate)
{
	bool isCreated;
	char *idActor;

	assert_no_null(m_dataPrivate);
	assert_no_null(m_dataPrivate->createActors);
	assert_no_null(m_dataPrivate->worldGeometry2DPixels);

	isCreated = m_dataPrivate->createActors->createActor(m_dataPrivate->typeActor, &idActor, actorCreate);

	if (isCreated == true)
	{
		m_dataPrivate->worldGeometry2DPixels->createBlockSprite(idActor, m_dataPrivate->x, m_dataPrivate->y);
		CString::free(&idActor);
	}

	return isCreated;
}
