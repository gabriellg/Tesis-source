//CEventCollision: Event to check if there is a collision with background or other element.

#include "CEventCollision.hpp"

#include "CWorldGeometry2DPixels.hpp"

#include "asrtbas.h"
#include "memory.h"
#include "strbas.h"

enum prv_typeCollision_t
{
	PRV_COLLISION_WITH_BACKGROUND,
	PRV_COLLISION_BETWEEN_BLOCKS
};

struct prv_dataPrivateEventCollision_t
{
	class CWorldGeometry2DPixels *worldGeometry2D;
};

const char *CEventCollision::ID_EVENT = "CEventCollision";

//-----------------------------------------------------------------------

static struct prv_dataPrivateEventCollision_t *prv_create(class CWorldGeometry2DPixels *worldGeometry2D)
{
	struct prv_dataPrivateEventCollision_t *dataPrivate;

	dataPrivate = MALLOC(struct prv_dataPrivateEventCollision_t);

	dataPrivate->worldGeometry2D = worldGeometry2D;

	return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroy(struct prv_dataPrivateEventCollision_t **dataPrivate)
{
	FREE_T(dataPrivate, struct prv_dataPrivateEventCollision_t);
}

//-----------------------------------------------------------------------

CEventCollision::CEventCollision(class CWorldGeometry2DPixels *worldGeometry2D) : CEventSystem(CEventCollision::ID_EVENT)
{
	m_dataPrivate = prv_create(worldGeometry2D);
}

//-----------------------------------------------------------------------

CEventCollision::~CEventCollision()
{
	prv_destroy(&m_dataPrivate);
}

//-----------------------------------------------------------------------

bool CEventCollision::hasCollisionBackground(const char *id)
{
	assert_no_null(m_dataPrivate);
	assert_no_null(m_dataPrivate->worldGeometry2D);

	return m_dataPrivate->worldGeometry2D->hasCollisionWithBackground(id);
}

//-----------------------------------------------------------------------

bool CEventCollision::hasCollisionWithElement(
							const char *id,
							char **idBlockCollisioned_opt)
{
	assert_no_null(m_dataPrivate);
	assert_no_null(m_dataPrivate->worldGeometry2D);

	return m_dataPrivate->worldGeometry2D->hasCollisionWithOtherBlock(id, idBlockCollisioned_opt);
}
