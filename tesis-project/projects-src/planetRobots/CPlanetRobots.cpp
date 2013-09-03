//CPlanetRobots.cpp

#include "CPlanetRobots.hpp"

#include "CRobotUser.inl"
#include "CPlanetFigure.inl"

#include "asrtbas.h"
#include "memory.h"
#include "strbas.h"

#include "CMapSprite.hpp"
#include "CWorldGeometry2DPixels.hpp"
#include "CScene.hpp"
#include "CGeneratorTraslate.hpp"
#include "CWorldSprite.hpp"
#include "ICreatorActors.hpp"
#include "CCollectionPositionCreation.hpp"
#include "CGeneratorCreateActors.hpp"
#include "CGeneratorCollisionSprite.hpp"
#include "CDisplaySprite.hpp"
#include "CDescriptionSpriteOneImage.hpp"
#include "CGestorDisplays.hpp"

static const char *PRV_SCENARIO = "PLANET_ROBOT";

struct prv_dataPrivatePlanetRobots_t
{
	class CWorldGeometry2DPixels *world;
	class ICreatorActors *createActors;
	class CCollectionPositionCreation *collectionPositionCreation;
};

//-----------------------------------------------------------------------

static struct prv_dataPrivatePlanetRobots_t *prv_create(
                        class CWorldGeometry2DPixels **world,
                        class ICreatorActors **createActors,
                        class CCollectionPositionCreation **collectionPositionCreation)
{
	struct prv_dataPrivatePlanetRobots_t *dataPrivate;

	dataPrivate = MALLOC(struct prv_dataPrivatePlanetRobots_t);

	dataPrivate->world = ASSIGN_PP_NO_NULL(world, class CWorldGeometry2DPixels);
	dataPrivate->createActors = ASSIGN_PP_NO_NULL(createActors, class ICreatorActors);
	dataPrivate->collectionPositionCreation = ASSIGN_PP_NO_NULL(collectionPositionCreation, class CCollectionPositionCreation);

	return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroy(struct prv_dataPrivatePlanetRobots_t **dataPrivate)
{
	assert_no_null(dataPrivate);
	assert_no_null(*dataPrivate);

	delete (*dataPrivate)->world;
	delete (*dataPrivate)->createActors;
	delete (*dataPrivate)->collectionPositionCreation;

	FREE_T(dataPrivate, struct prv_dataPrivatePlanetRobots_t);
}

//-----------------------------------------------------------------------

CPlanetRobots::CPlanetRobots()
{
    class CWorldGeometry2DPixels *world;
    class ICreatorActors *createActors;
    class CCollectionPositionCreation *collectionPositionCreation;

    collectionPositionCreation = CPlanetFigure::collectionPositionCreationPlane();
    createActors = CPlanetFigure::createActors();
	world = CPlanetFigure::createWorld();

	m_dataPrivate = prv_create(&world, &createActors, &collectionPositionCreation);
}

//-----------------------------------------------------------------------

CPlanetRobots::~CPlanetRobots()
{
	prv_destroy(&m_dataPrivate);
}

//-----------------------------------------------------------------------

void CPlanetRobots::appendElementsToScene(class CScene *scene)
{
    class CGeneratorCollisionSprite *generatorCollision;
	class CAgent *mapSprite, *creatorActors, *generatorTraslate, *agentGeneratorCollision;
	
	assert_no_null(scene);
	assert_no_null(m_dataPrivate);

	mapSprite = new CMapSprite(PRV_SCENARIO);

	creatorActors = new CGeneratorCreateActors(
                            m_dataPrivate->world,
                            m_dataPrivate->collectionPositionCreation,
                            m_dataPrivate->createActors);

    generatorCollision = new CGeneratorCollisionSprite(m_dataPrivate->world);
    CRobotUser::appendRobotToCollision(generatorCollision);
    agentGeneratorCollision = generatorCollision;

    generatorTraslate = new CGeneratorTraslate();

    generatorTraslate->appendChild(&mapSprite);
    generatorTraslate->appendChild(&creatorActors);
    agentGeneratorCollision->appendChild(&generatorTraslate);

    scene->appendAgent(&agentGeneratorCollision);
}

//-----------------------------------------------------------------------

void CPlanetRobots::appendDisplays(class CGestorDisplays *displays)
{
    class IDisplay *displaySprite;
    class CImg *imageScenario;
    class IDescription *description;

	assert_no_null(m_dataPrivate);
	
	displaySprite = new CDisplaySprite(400, 400, m_dataPrivate->world);
	imageScenario = CPlanetFigure::createImageScenario();
	description = new CDescriptionSpriteOneImage(&imageScenario);
	displaySprite->appendDescription(PRV_SCENARIO, &description);
    CRobotUser::appendSymbolRobot(displaySprite);

	displays->appendDisplay(&displaySprite);
}

//-----------------------------------------------------------------------

void CPlanetRobots::defineLayers(class IGraphics *graphics)
{
	//TODO: Esta función debería estar en CDisplay3D
}
