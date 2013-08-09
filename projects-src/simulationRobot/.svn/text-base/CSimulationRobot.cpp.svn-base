//CSimulationRobots.cpp

#include "CSimulationRobot.inl"

#include "CWorldSimulationRobot.inl"
#include "CRobotSprites.inl"
#include "CDisplaySimulationRobot.inl"
#include "CGeneratorsSimulation.inl"
#include "CRobot.inl"

#include "asrtbas.h"
#include "memory.h"

#include "CFigure.hpp"
#include "CGestorDisplays.hpp"
#include "CScene.hpp"

struct prv_dataPrivateSimulationRobot_t
{
    bool is3d;
    class CWorldSimulationRobot *worldSimulationRobot;
};

//-----------------------------------------------------------------------

static struct prv_dataPrivateSimulationRobot_t *prv_createSimulationRobot(bool is3d, class CWorldSimulationRobot **worldSimulationRobot)
{
    struct prv_dataPrivateSimulationRobot_t *dataPrivate;

    dataPrivate = MALLOC(struct prv_dataPrivateSimulationRobot_t);

    dataPrivate->is3d = is3d;
    dataPrivate->worldSimulationRobot = ASSIGN_PP_NO_NULL(worldSimulationRobot, class CWorldSimulationRobot);

    return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroySimulationRobot(struct prv_dataPrivateSimulationRobot_t **dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(*dataPrivate);

    delete (*dataPrivate)->worldSimulationRobot;
    (*dataPrivate)->worldSimulationRobot = NULL;

    FREE_T(dataPrivate, struct prv_dataPrivateSimulationRobot_t);
}

//-----------------------------------------------------------------------

CSimulationRobot::CSimulationRobot(bool is3d)
{
    class CWorldSimulationRobot *worldSimulationRobot;
    class CImg *map;

    map = CRobotSprites::createMap();

    worldSimulationRobot = new CWorldSimulationRobot(is3d, &map);
    m_dataPrivate = prv_createSimulationRobot(is3d, &worldSimulationRobot);
}

//-----------------------------------------------------------------------

CSimulationRobot::~CSimulationRobot()
{
    prv_destroySimulationRobot(&m_dataPrivate);
}

//-----------------------------------------------------------------------

void CSimulationRobot::appendElementsToScene(class CScene *scene)
{
	class CAgent *mapSprite, *robot;
	class CAgent *generatorObjective, *generatorLaser, *generatorVideo, *generatorDecide;
	
	assert_no_null(scene);

	mapSprite = new CFigure(CDisplaySimulationRobot::SYMBOL_SCENARIO);
	scene->appendAgent(&mapSprite);

	robot = new CRobot();

    generatorDecide = new CGeneratorEventsDecide();
    generatorDecide->appendChild(&robot);

    generatorObjective = new CGeneratorEventsObjectiveSimulation();
    generatorObjective->appendChild(&generatorDecide);

    generatorLaser = new CGeneratorEventsLaserSimulation(m_dataPrivate->worldSimulationRobot);
    generatorLaser->appendChild(&generatorObjective);

    generatorVideo = new CGeneratorEventsVideoSimulation(m_dataPrivate->worldSimulationRobot);
    generatorVideo->appendChild(&generatorLaser);

    scene->appendAgent(&generatorVideo);
}

//-----------------------------------------------------------------------

void CSimulationRobot::appendDisplays(class CGestorDisplays *displays)
{
    class IDisplay *display;

    assert_no_null(m_dataPrivate);

    if (m_dataPrivate->is3d == true)
        display = CDisplaySimulationRobot::createDisplayGL(m_dataPrivate->worldSimulationRobot);
    else
        display = CDisplaySimulationRobot::createDisplaySprite(m_dataPrivate->worldSimulationRobot);

	displays->appendDisplay(&display);
}

//-----------------------------------------------------------------------

void CSimulationRobot::defineLayers(class IGraphics *graphics)
{
	//TODO: Esta función debería estar en CDisplay3D
}
