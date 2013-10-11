//CEscalextric.cpp

#include "CEscalextric.inl"

#include "CWorldEscalextric.inl"
#include "CDisplayEscalextric.inl"
#include "CDataCircuit.inl"
#include "CCar.inl"
#include "CCircuit.inl"
#include "CGeneratorAccelerationKey.inl"

#include "asrtbas.h"
#include "memory.h"

#include "CDocXML.hpp"
#include "CString.hpp"
#include "CScene.hpp"
#include "CFigure.hpp"
#include "CAgentPrimitive.hpp"
#include "CGestorDisplays.hpp"

struct SPrvDataPrivateEscalextric
{
    bool is3d;
    class CDataCircuit *dataCircuit;
    class CWorldEscalextric *worldEscalextric;
};

//-----------------------------------------------------------------------

static struct SPrvDataPrivateEscalextric *prv_createDataPrivateEscalextric(bool is3d, class CDataCircuit **dataCircuit,
        class CWorldEscalextric **worldEscalextric)
{
    struct SPrvDataPrivateEscalextric *dataPrivate;

    dataPrivate = MALLOC(struct SPrvDataPrivateEscalextric);

    dataPrivate->is3d = is3d;
    dataPrivate->dataCircuit = ASSIGN_PP_NO_NULL(dataCircuit, class CDataCircuit);
    dataPrivate->worldEscalextric = ASSIGN_PP_NO_NULL(worldEscalextric, class CWorldEscalextric);

    return dataPrivate;
}

//-----------------------------------------------------------------------

CEscalextric::CEscalextric(bool is3d, const char *fileNameXML)
{
    class CWorldEscalextric *worldEscalextric;
    class CDataCircuit *dataCircuit;

    dataCircuit = new CDataCircuit(fileNameXML);

    worldEscalextric = new CWorldEscalextric(dataCircuit);
    m_dataPrivate = prv_createDataPrivateEscalextric(is3d, &dataCircuit, &worldEscalextric);
}

//-----------------------------------------------------------------------

CEscalextric::~CEscalextric()
{
    assert_no_null(m_dataPrivate);

    delete m_dataPrivate->dataCircuit;
    delete m_dataPrivate->worldEscalextric;

    FREE_T(&m_dataPrivate, struct SPrvDataPrivateEscalextric);
}

//-----------------------------------------------------------------------

static void prv_appendCarWithKey(const char *id, char keyAccelerator, char keyDesaccelerator,
        bool isLeft,
        class CWorldEscalextric *worldEscalextric, class CGeneratorAccelerationKey *generatorKey)
{
    class CCar *carKey;
    class CAgent *agentCar;

    agentCar = carKey = new CCar(id);

    if (isLeft == true)
        worldEscalextric->appendCarLaneLeft(id);
    else
        worldEscalextric->appendCarLaneRight(id);

    generatorKey->appendCarKey(id, keyAccelerator, keyDesaccelerator);
    generatorKey->appendChild(&agentCar);
}

//-----------------------------------------------------------------------

void CEscalextric::appendElementsToScene(class CScene *scene)
{
    class CAgent *circuit, *generatorAgent, *agentSky;
    class CGeneratorAccelerationKey *generatorAccelerationKey;

    assert_no_null(scene);
    assert_no_null(m_dataPrivate);

    m_dataPrivate->worldEscalextric->resetCars();

    generatorAccelerationKey = new CGeneratorAccelerationKey();
    circuit = new CCircuit(m_dataPrivate->worldEscalextric);

    prv_appendCarWithKey("A-Z", 'a', 'z', true, m_dataPrivate->worldEscalextric, generatorAccelerationKey);
    prv_appendCarWithKey("J-M", 'j', 'm', false, m_dataPrivate->worldEscalextric, generatorAccelerationKey);

    generatorAgent = generatorAccelerationKey;
    circuit->appendChild(&generatorAgent);

    agentSky = new CAgentPrimitive(CDisplayEscalextric::SYMBOL_SKY);

    scene->appendAgent(&agentSky);
    scene->appendAgent(&circuit);
}

//-----------------------------------------------------------------------

void CEscalextric::appendDisplays(class CGestorDisplays *displays)
{
    class ITraslatorDisplay *display;

    assert_no_null(m_dataPrivate);

    if (m_dataPrivate->is3d == true)
        display = CDisplayEscalextric::createDisplayGL(m_dataPrivate->dataCircuit, m_dataPrivate->worldEscalextric);
    else
        display = CDisplayEscalextric::createDisplaySprite(m_dataPrivate->dataCircuit, m_dataPrivate->worldEscalextric);

    displays->appendDisplay(&display);
}

//-----------------------------------------------------------------------

void CEscalextric::defineLayers(class IGraphics *graphics)
{
    //TODO: Esta función debería estar en CDisplay3D
}
