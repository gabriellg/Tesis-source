// Define los datos del mundo virtual.

#include "CDataWorkspaceWorldVirtual.inl"

#include "CDefineWorld.hpp"

#include "asrtbas.h"
#include "memory.h"

#include "areadibujo.hpp"
#include "CScene.hpp"
#include "CTypeDescription.hpp"
#include "CGestorDisplays.hpp"
#include "CEventCamera.hpp"
#include "CEventKey.hpp"

struct SPrvDataWorkspaceWorldVirtual
{
    class CDefineWorld *defineWorld;
    class CGestorDisplays *gestorDisplays;
    class CScene *scene;
};

//---------------------------------------------------------------

static void prv_integrityDataWorkspaceWorlVirtual(const struct SPrvDataWorkspaceWorldVirtual *dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(dataPrivate->defineWorld);
    assert_no_null(dataPrivate->gestorDisplays);
    assert_no_null(dataPrivate->scene);
}

//---------------------------------------------------------------

static struct SPrvDataWorkspaceWorldVirtual *prv_createDataPrivate(
						class CDefineWorld **defineWorld,
                        class CGestorDisplays **gestorDisplays,
                        class CScene **scene)
{   
    struct SPrvDataWorkspaceWorldVirtual *dataPrivate;
    
    dataPrivate = MALLOC(struct SPrvDataWorkspaceWorldVirtual);
         
    dataPrivate->defineWorld = ASSIGN_PP_NO_NULL(defineWorld, class CDefineWorld);
    dataPrivate->gestorDisplays = ASSIGN_PP_NO_NULL(gestorDisplays, class CGestorDisplays);
    dataPrivate->scene = ASSIGN_PP_NO_NULL(scene, class CScene);
    
    prv_integrityDataWorkspaceWorlVirtual(dataPrivate);

    return dataPrivate;
}

//-----------------------------------------------------------------------

CDataWorkspaceWorldVirtual::CDataWorkspaceWorldVirtual(class CDefineWorld **defineWorld)
{
	class CGestorDisplays *gestorDisplays;
    class CScene *scene;
    
    scene = new CScene();
    gestorDisplays = new CGestorDisplays();

    (*defineWorld)->appendElementsToScene(scene);
    (*defineWorld)->appendDisplays(gestorDisplays);
    
    m_dataPrivate = prv_createDataPrivate(defineWorld, &gestorDisplays, &scene);
}

//-----------------------------------------------------------------------

CDataWorkspaceWorldVirtual::~CDataWorkspaceWorldVirtual()
{
    prv_integrityDataWorkspaceWorlVirtual(m_dataPrivate);

    DELETE_OBJECT(&m_dataPrivate->defineWorld, class CDefineWorld);
    DELETE_OBJECT(&m_dataPrivate->gestorDisplays, class CGestorDisplays);
    DELETE_OBJECT(&m_dataPrivate->scene, class CScene);

    FREE_T(&m_dataPrivate, struct SPrvDataWorkspaceWorldVirtual);
}

//-----------------------------------------------------------------------

void CDataWorkspaceWorldVirtual::defineLayers(class IGraphics *graphics) const
{
    prv_integrityDataWorkspaceWorlVirtual(m_dataPrivate);

    m_dataPrivate->defineWorld->defineLayers(graphics);
}

//-----------------------------------------------------------------------

void CDataWorkspaceWorldVirtual::appendKeyToScene(const struct EvtKey_t *evtKey)
{
    class CEventSystem *eventKey;

    prv_integrityDataWorkspaceWorlVirtual(m_dataPrivate);

    eventKey = new CEventKey(evtKey);
    m_dataPrivate->scene->appendEvent(&eventKey);
}

//-----------------------------------------------------------------------

void CDataWorkspaceWorldVirtual::nextFrame(void)
{
    prv_integrityDataWorkspaceWorlVirtual(m_dataPrivate);
    
    m_dataPrivate->scene->nextFrame();
}

//-----------------------------------------------------------------------

void CDataWorkspaceWorldVirtual::stop(void)
{
    prv_integrityDataWorkspaceWorlVirtual(m_dataPrivate);
    
    delete m_dataPrivate->scene;
    
    m_dataPrivate->scene = new CScene();
    m_dataPrivate->defineWorld->appendElementsToScene(m_dataPrivate->scene);
}

//-----------------------------------------------------------------------

void CDataWorkspaceWorldVirtual::setInitialPositionCamera(void)
{
    class CEventSystem *evtCamera;

    prv_integrityDataWorkspaceWorlVirtual(m_dataPrivate);

    evtCamera = CEventCamera::createInitialPositionCamera();
    m_dataPrivate->scene->appendEvent(&evtCamera);
}

//-----------------------------------------------------------------------

void CDataWorkspaceWorldVirtual::setRotationCamera(double rotXDegrees, double rotYDegrees, double rotZDegrees)
{
    class CEventSystem *evtCamera;

    prv_integrityDataWorkspaceWorlVirtual(m_dataPrivate);

    evtCamera = CEventCamera::createAnimationRotation(rotXDegrees, rotYDegrees, rotZDegrees);
    m_dataPrivate->scene->appendEvent(&evtCamera);
}

//-----------------------------------------------------------------------

void CDataWorkspaceWorldVirtual::incrRotateCamera(double incrRotateX, double incrRotateY, double incrRotateZ)
{
    class CEventSystem *evtCamera;

    prv_integrityDataWorkspaceWorlVirtual(m_dataPrivate);

    evtCamera = CEventCamera::createIncrRotateCamera(incrRotateX, incrRotateY, incrRotateZ);
    m_dataPrivate->scene->appendEvent(&evtCamera);
}

//-----------------------------------------------------------------------

void CDataWorkspaceWorldVirtual::frontCamera(double step)
{
    class CEventSystem *evtCamera;

    prv_integrityDataWorkspaceWorlVirtual(m_dataPrivate);

    evtCamera = CEventCamera::createTraslateCamera(step);
    m_dataPrivate->scene->appendEvent(&evtCamera);
}

//-----------------------------------------------------------------------

void CDataWorkspaceWorldVirtual::backCamera(double step)
{
    class CEventSystem *evtCamera;

    prv_integrityDataWorkspaceWorlVirtual(m_dataPrivate);

    evtCamera = CEventCamera::createTraslateCamera(-step);
    m_dataPrivate->scene->appendEvent(&evtCamera);
}

//-----------------------------------------------------------------------

void CDataWorkspaceWorldVirtual::draw(class IGraphics *graphics) const
{
    prv_integrityDataWorkspaceWorlVirtual(m_dataPrivate);
    
    m_dataPrivate->gestorDisplays->drawScene(graphics, m_dataPrivate->scene);
}
