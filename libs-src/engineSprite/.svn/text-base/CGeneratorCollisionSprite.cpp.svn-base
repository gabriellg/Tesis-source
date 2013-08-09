//CGeneratorCollisionSprite.cpp:

#include "CGeneratorCollisionSprite.hpp"

#include "CWorldGeometry2DPixels.hpp"
#include "CDisplayLimitsSprite.inl"

#include "CEventDesire.hpp"
#include "CEventCollision.hpp"
#include "CDescriptionSpriteOneImage.hpp"
#include "CScene.hpp"

#include "CCollectionEventsSystem.hpp"

#include "asrtbas.h"
#include "memory.h"
#include "strbas.h"

struct prv_dataPrivateGeneratorCollisionSprite_t
{
    class CDisplayLimitsSprite *displayLimits;
	class CWorldGeometry2DPixels *worldGeometry2D;
};

//-----------------------------------------------------------------------

static struct prv_dataPrivateGeneratorCollisionSprite_t *prv_create(
                            class CDisplayLimitsSprite **displayLimits,
							class CWorldGeometry2DPixels *worldGeometry2D)
{
	struct prv_dataPrivateGeneratorCollisionSprite_t *dataPrivate;

	dataPrivate = MALLOC(struct prv_dataPrivateGeneratorCollisionSprite_t);

	dataPrivate->displayLimits = ASSIGN_PP_NO_NULL(displayLimits, class CDisplayLimitsSprite);
	dataPrivate->worldGeometry2D = worldGeometry2D;

	return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroy(struct prv_dataPrivateGeneratorCollisionSprite_t **dataPrivate)
{
	assert_no_null(dataPrivate);
	assert_no_null(*dataPrivate);

	delete (*dataPrivate)->displayLimits;
	(*dataPrivate)->worldGeometry2D = NULL;

	FREE_T(dataPrivate, struct prv_dataPrivateGeneratorCollisionSprite_t);
}

//-----------------------------------------------------------------------

CGeneratorCollisionSprite::CGeneratorCollisionSprite(class CWorldGeometry2DPixels *worldGeometry2D)
{
    class CDisplayLimitsSprite *displayLimits;

    displayLimits = new CDisplayLimitsSprite(worldGeometry2D);
    m_dataPrivate = prv_create(&displayLimits, worldGeometry2D);
}

//-----------------------------------------------------------------------

CGeneratorCollisionSprite::~CGeneratorCollisionSprite()
{
	prv_destroy(&m_dataPrivate);
}

//-----------------------------------------------------------------------

void CGeneratorCollisionSprite::appendSymbolFigureToConsider(const char *symbol, class CImg **img)
{
    class IDescription *description;

    description = new CDescriptionSpriteOneImage(img);
    m_dataPrivate->displayLimits->appendDescription(symbol, &description);
}

//-----------------------------------------------------------------------

void CGeneratorCollisionSprite::beginEvolution(class CCollectionEventsSystem *events)
{
	class CEventSystem *event;
	const class CScene *scene;

	assert_no_null(events);
	assert_no_null(m_dataPrivate);
	assert_no_null(m_dataPrivate->worldGeometry2D);

    scene = events->getScene();

	m_dataPrivate->worldGeometry2D->desactiveAllBlocks();
	m_dataPrivate->displayLimits->drawScene(NULL, scene);
	m_dataPrivate->worldGeometry2D->deleteBlocksDesactived();

	event = new CEventDesire(m_dataPrivate->worldGeometry2D);
	events->appendEventSystem(&event);

	event = new CEventCollision(m_dataPrivate->worldGeometry2D);
	events->appendEventSystem(&event);
}
