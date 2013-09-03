//CCollectionPositionCreation: positions Collection where it is been able to create Actors.

#include "CCollectionPositionCreation.hpp"

#include "ICreatorActors.hpp"
#include "CEventCreateActor.inl"

#include "asrtbas.h"
#include "memory.h"
#include "CArrayString.hpp"
#include "CArrPixel.hpp"

#include "CCollectionEventsSystem.hpp"

struct prv_dataPrivateCollectionPositionCreation_t
{
	class CArrayString *typePositions;
	class CArrPixel *pixels;

	unsigned long indCurrentPosition;
};

//-----------------------------------------------------------------------

static struct prv_dataPrivateCollectionPositionCreation_t *prv_create(
				class CArrayString **typePositions,
				class CArrPixel **pixels,
				unsigned long indCurrentPosition)
{
	struct prv_dataPrivateCollectionPositionCreation_t *dataPrivate;

	dataPrivate = MALLOC(struct prv_dataPrivateCollectionPositionCreation_t);

	dataPrivate->typePositions = ASSIGN_PP_NO_NULL(typePositions, class CArrayString);
	dataPrivate->pixels = ASSIGN_PP_NO_NULL(pixels, class CArrPixel);
	dataPrivate->indCurrentPosition = indCurrentPosition;

	return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroy(struct prv_dataPrivateCollectionPositionCreation_t **dataPrivate)
{
	assert_no_null(dataPrivate);
	assert_no_null(*dataPrivate);

	delete (*dataPrivate)->typePositions;
	delete (*dataPrivate)->pixels;

	FREE_T(dataPrivate, struct prv_dataPrivateCollectionPositionCreation_t);
}

//-----------------------------------------------------------------------

CCollectionPositionCreation::CCollectionPositionCreation()
{
	class CArrayString *idPositions;
	class CArrPixel *pixels;
	unsigned long indCurrentPosition;

	idPositions = new CArrayString();
	pixels = new CArrPixel();
	indCurrentPosition = 0;

	m_dataPrivate = prv_create(&idPositions, &pixels, indCurrentPosition);
}

//-----------------------------------------------------------------------

CCollectionPositionCreation::~CCollectionPositionCreation()
{
	prv_destroy(&m_dataPrivate);
}

//-----------------------------------------------------------------------

void CCollectionPositionCreation::appendPosition(char **typePosition, long x, long y)
{
	assert_no_null(m_dataPrivate);
    assert_no_null(m_dataPrivate->typePositions);

	m_dataPrivate->typePositions->addDestroyingString(typePosition);
	m_dataPrivate->pixels->add(x, y);
}

//-----------------------------------------------------------------------

void CCollectionPositionCreation::appendEventsCreations(
					class CCollectionEventsSystem *events,
					class CWorldGeometry2DPixels *worldGeometry2DPixels,
					class ICreatorActors *createActors)
{
	unsigned long i, numElements;

	assert_no_null(m_dataPrivate);
    assert_no_null(m_dataPrivate->typePositions);
    assert_no_null(m_dataPrivate->pixels);
	assert_no_null(createActors);

	numElements = m_dataPrivate->typePositions->size();
	assert(numElements == m_dataPrivate->pixels->size());

	for (i = 0; i < numElements; i++)
	{
		const char *typePosition;

		typePosition = m_dataPrivate->typePositions->get(i);

		if (createActors->isGoingToCreate(typePosition) == true)
		{
			long xPixel, yPixel;
			class CEventSystem *evtCreateActor;

			m_dataPrivate->pixels->get(i, &xPixel, &yPixel);

			evtCreateActor = new CEventCreateActor(typePosition, xPixel, yPixel, createActors, worldGeometry2DPixels);
			events->appendEventSystem(&evtCreateActor);
		}
	}
}
