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
#include "CAgentCamera.hpp"
#include "CPositionCamera.hpp"
#include "CModel3d.hpp"
#include "CGraphicsRect.hpp"

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

static class CPositionCamera *prv_createPositionCameraDefault(void)
{
    double eyeX, eyeY, eyeZ;
    double pointReferenceX, pointReferenceY, pointReferenceZ;
    double upX, upY, upZ;

    eyeX = 0.;
    eyeY = 0.;
    eyeZ = 15.;

    pointReferenceX = 0.;
    pointReferenceY = 0.;
    pointReferenceZ = 0.;

    upX = 0.;
    upY = 1.;
    upZ = 0.;

    return new CPositionCamera(eyeX, eyeY, eyeZ, pointReferenceX, pointReferenceY, pointReferenceZ, upX, upY, upZ);
}

//-----------------------------------------------------------------------

static void prv_calculateCenterCircuit(class CDataCircuit *dataCircuit, double *xCenterCircuit, double *yCenterCircuit, double *zCenterCircuit)
{
    class CModel3d *model;
    class CGraphicsRect *graphicsLimits;
    double xmin, ymin, zmin, xmax, ymax, zmax;
    bool existLimits;

    assert_no_null(dataCircuit);

    model = dataCircuit->model3d();

    graphicsLimits = new CGraphicsRect();

    model->draw(graphicsLimits);

    existLimits = graphicsLimits->getLimits(&xmin, &ymin, &zmin, &xmax, &ymax, &zmax);
    assert(existLimits == true);

    *xCenterCircuit = (xmin + xmax) * 0.5;
    *yCenterCircuit = (ymin + ymax) * 0.5;
    *zCenterCircuit = (zmin + zmax) * 0.5;

    DELETE_OBJECT(&model, class CModel3d);
    DELETE_OBJECT(&graphicsLimits, class CGraphicsRect);
}

//-----------------------------------------------------------------------

void CEscalextric::appendElementsToScene(class CScene *scene)
{
    class CAgent *circuit, *generatorAgent, *agentWorld, *agentCamera, *light;
    class CGeneratorAccelerationKey *generatorAccelerationKey;
    class CPositionCamera *positionCamera;
    double xCenterCircuit, yCenterCircuit, zCenterCircuit;

    assert_no_null(scene);
    assert_no_null(m_dataPrivate);

    m_dataPrivate->worldEscalextric->resetCars();

    prv_calculateCenterCircuit(m_dataPrivate->dataCircuit, &xCenterCircuit, &yCenterCircuit, &zCenterCircuit);

    generatorAccelerationKey = new CGeneratorAccelerationKey();
    circuit = new CCircuit(m_dataPrivate->worldEscalextric, xCenterCircuit, yCenterCircuit, zCenterCircuit);

    prv_appendCarWithKey("A-Z", 'a', 'z', true, m_dataPrivate->worldEscalextric, generatorAccelerationKey);
    prv_appendCarWithKey("J-M", 'j', 'm', false, m_dataPrivate->worldEscalextric, generatorAccelerationKey);

    generatorAgent = generatorAccelerationKey;
    circuit->appendChild(&generatorAgent);

    agentWorld = new CAgentPrimitive(CDisplayEscalextric::SYMBOL_WORLD);
    light = new CAgentPrimitive(CDisplayEscalextric::SYMBOL_LIGHT_AMBIENT);

    positionCamera = prv_createPositionCameraDefault();

    agentCamera = new CAgentCamera(&positionCamera);
    agentCamera->appendChild(&agentWorld);
    agentCamera->appendChild(&circuit);

    scene->appendAgent(&light);
    scene->appendAgent(&agentCamera);
}

//-----------------------------------------------------------------------

void CEscalextric::appendDisplays(class CGestorDisplays *displays)
{
    class ITraslatorDisplay *display;

    assert_no_null(m_dataPrivate);

    if (m_dataPrivate->is3d == true)
        display = CDisplayEscalextric::createDisplay3D(m_dataPrivate->dataCircuit, m_dataPrivate->worldEscalextric);
    else
        display = CDisplayEscalextric::createDisplaySprite(m_dataPrivate->dataCircuit, m_dataPrivate->worldEscalextric);

    displays->appendDisplay(&display);
}

//-----------------------------------------------------------------------

void CEscalextric::defineLayers(class IGraphics *graphics)
{
    //TODO: Esta función debería estar en CDisplay3D
}
