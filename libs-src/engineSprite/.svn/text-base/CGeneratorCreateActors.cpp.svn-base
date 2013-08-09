//CGeneratorCreateActors.cpp:

#include "CGeneratorCreateActors.hpp"

#include "CCollectionPositionCreation.hpp"
#include "ICreatorActors.hpp"

#include "asrtbas.h"
#include "memory.h"

struct prv_dataPrivateGeneratorCreateActors_t
{
	class CWorldGeometry2DPixels *worldGeometry2DPixels;
	class CCollectionPositionCreation *collectionPositionCreation;
	class ICreatorActors *createActors;
};

//-----------------------------------------------------------------------

static struct prv_dataPrivateGeneratorCreateActors_t *prv_create(
					class CWorldGeometry2DPixels *worldGeometry2DPixels,
					class CCollectionPositionCreation *collectionPositionCreation,
					class ICreatorActors *createActors)
{
	struct prv_dataPrivateGeneratorCreateActors_t *dataPrivate;

	dataPrivate = MALLOC(struct prv_dataPrivateGeneratorCreateActors_t);

	dataPrivate->worldGeometry2DPixels = worldGeometry2DPixels;
	dataPrivate->collectionPositionCreation = collectionPositionCreation;
	dataPrivate->createActors = createActors;

	return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroy(struct prv_dataPrivateGeneratorCreateActors_t **dataPrivate)
{
	FREE_T(dataPrivate, struct prv_dataPrivateGeneratorCreateActors_t);
}

//-----------------------------------------------------------------------

CGeneratorCreateActors::CGeneratorCreateActors(
							class CWorldGeometry2DPixels *worldGeometry2DPixels,
							class CCollectionPositionCreation *collectionPositionCreation,
							class ICreatorActors *createActors)
{
	m_dataPrivate = prv_create(worldGeometry2DPixels, collectionPositionCreation, createActors);
}

//-----------------------------------------------------------------------

CGeneratorCreateActors::~CGeneratorCreateActors()
{
	prv_destroy(&m_dataPrivate);
}

//-----------------------------------------------------------------------

void CGeneratorCreateActors::beginEvolution(class CCollectionEventsSystem *events)
{
	assert_no_null(m_dataPrivate);
	assert_no_null(m_dataPrivate->collectionPositionCreation);

	m_dataPrivate->collectionPositionCreation->appendEventsCreations(
						events,
						m_dataPrivate->worldGeometry2DPixels,
						m_dataPrivate->createActors);
}
