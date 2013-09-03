// Define los datos del mundo virtual.

#include "CDataWorkspaceWorldVirtual.inl"

#include "CDefineWorld.hpp"

#include "asrtbas.h"
#include "memory.h"

#include "areadibujo.hpp"
#include "CScene.hpp"
#include "CTypeDescription.hpp"
#include "CGestorDisplays.hpp"

struct prv_dataPrivate_t
{
    class CDefineWorld *defineWorld;
    class CGestorDisplays *gestorDisplays;
    class CScene *scene;
};

//---------------------------------------------------------------

static struct prv_dataPrivate_t *prv_createDataPrivate(
						class CDefineWorld **defineWorld,
                        class CGestorDisplays **gestorDisplays,
                        class CScene **scene)
{   
    struct prv_dataPrivate_t *data_private;
    
    data_private = MALLOC(struct prv_dataPrivate_t);
         
    data_private->defineWorld = ASSIGN_PP_NO_NULL(defineWorld, class CDefineWorld);
    data_private->gestorDisplays = ASSIGN_PP_NO_NULL(gestorDisplays, class CGestorDisplays);
    data_private->scene = ASSIGN_PP_NO_NULL(scene, class CScene);
    
    return data_private;
}

//---------------------------------------------------------------

static void prv_destroyDataPrivate(struct prv_dataPrivate_t **dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(*dataPrivate);

    delete (*dataPrivate)->defineWorld;
    delete (*dataPrivate)->scene;
    delete (*dataPrivate)->gestorDisplays;
    
    FREE_T(dataPrivate, struct prv_dataPrivate_t);
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
    assert_no_null(m_dataPrivate);
    prv_destroyDataPrivate(&m_dataPrivate);
}

//-----------------------------------------------------------------------

void CDataWorkspaceWorldVirtual::defineLayers(class IGraphics *graphics) const
{
    assert_no_null(m_dataPrivate);
    assert_no_null(m_dataPrivate->defineWorld);
    m_dataPrivate->defineWorld->defineLayers(graphics);
}

//-----------------------------------------------------------------------

void CDataWorkspaceWorldVirtual::appendKeyToScene(const struct EvtKey_t *evtKey)
{
    assert_no_null(m_dataPrivate);
    assert_no_null(m_dataPrivate->scene);
    
    m_dataPrivate->scene->appendKey(evtKey);
}

//-----------------------------------------------------------------------

void CDataWorkspaceWorldVirtual::nextFrame(void)
{
    assert_no_null(m_dataPrivate);
    assert_no_null(m_dataPrivate->scene);
    
    m_dataPrivate->scene->nextFrame();
}

//-----------------------------------------------------------------------

void CDataWorkspaceWorldVirtual::stop(void)
{
    assert_no_null(m_dataPrivate);
    assert_no_null(m_dataPrivate->scene);
    
    delete m_dataPrivate->scene;
    
    m_dataPrivate->scene = new CScene();
    m_dataPrivate->defineWorld->appendElementsToScene(m_dataPrivate->scene);
}

//-----------------------------------------------------------------------

void CDataWorkspaceWorldVirtual::setInitialPositionCamera(void)
{
    assert_no_null(m_dataPrivate);
    assert_no_null(m_dataPrivate->gestorDisplays);

    m_dataPrivate->gestorDisplays->setInitialPositionCamera();
}

//-----------------------------------------------------------------------

void CDataWorkspaceWorldVirtual::positionCamera(class IGraphics *graphics) const
{
    assert_no_null(m_dataPrivate);
    assert_no_null(m_dataPrivate->gestorDisplays);
    
    m_dataPrivate->gestorDisplays->positionCameraCurrentDisplay(graphics);
}

//-----------------------------------------------------------------------

void CDataWorkspaceWorldVirtual::makeRotationCamera(class IGraphics *graphics) const
{
    assert_no_null(m_dataPrivate);
    assert_no_null(m_dataPrivate->gestorDisplays);
    
    m_dataPrivate->gestorDisplays->makeRotationCurrentDisplay(graphics);
}

//-----------------------------------------------------------------------

void CDataWorkspaceWorldVirtual::setRotationCamera(struct areaDibujo_t *areaDibujo, double rotXDegrees, double rotYDegrees, double rotZDegrees)
{
	assert_no_null(m_dataPrivate);
	assert_no_null(m_dataPrivate->gestorDisplays);
	m_dataPrivate->gestorDisplays->setRotationCameraCurrentDisplay(areaDibujo, rotXDegrees, rotYDegrees, rotZDegrees);
}

//-----------------------------------------------------------------------

void CDataWorkspaceWorldVirtual::incrRotateCamera(struct areaDibujo_t *areaDibujo, double incrRotateX, double incrRotateY, double incrRotateZ)
{
    assert_no_null(m_dataPrivate);
    assert_no_null(m_dataPrivate->gestorDisplays);
    m_dataPrivate->gestorDisplays->incrRotateCameraCurrentDisplay(areaDibujo, incrRotateX, incrRotateY, incrRotateZ);
}

//-----------------------------------------------------------------------

void CDataWorkspaceWorldVirtual::frontCamera(struct areaDibujo_t *areaDibujo, double step)
{
    assert_no_null(m_dataPrivate);
    assert_no_null(m_dataPrivate->gestorDisplays);
    m_dataPrivate->gestorDisplays->frontCameraCurrentDisplay(areaDibujo, step);
}

//-----------------------------------------------------------------------

void CDataWorkspaceWorldVirtual::backCamera(struct areaDibujo_t *areaDibujo, double step)
{
    assert_no_null(m_dataPrivate);
    assert_no_null(m_dataPrivate->gestorDisplays);
    m_dataPrivate->gestorDisplays->backCameraCurrentDisplay(areaDibujo, step);
}

//-----------------------------------------------------------------------

bool CDataWorkspaceWorldVirtual::isArea3D(void) const
{
    assert_no_null(m_dataPrivate);
    assert_no_null(m_dataPrivate->gestorDisplays);
    return m_dataPrivate->gestorDisplays->isArea3DCurrentDisplay();
}

//-----------------------------------------------------------------------

void CDataWorkspaceWorldVirtual::draw(class IGraphics *graphics) const
{
    assert_no_null(m_dataPrivate);
    assert_no_null(m_dataPrivate->gestorDisplays);
    
    m_dataPrivate->gestorDisplays->drawScene(graphics, m_dataPrivate->scene);
}
