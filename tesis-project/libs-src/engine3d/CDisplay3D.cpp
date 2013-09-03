//CDisplay3D.cpp

#include "CDisplay3D.hpp"

#include "CDescriptionTransformation3D.hpp"
#include "CTransform3D.hpp"

#include "asrtbas.h"
#include "memory.h"
#include "timesys.h"

#include "areadibujo.hpp"
#include "CTypeDescriptionGL.hpp"

#include "CMath.hpp"
#include "CArray.hpp"
#include "CLight.hpp"
#include "CScene.hpp"
#include "CGraphicsGL.hpp"
#include "CPositionCamera.hpp"
#include "CDictionaryDescription.hpp"

struct SPrvDataPrivateDisplay3D
{
    class IWorld *world;
	class CLight *light;
	class CPositionCamera *positionCameraInitial;
	class CPositionCamera *positionCameraCurrent;
	class CDictionaryDescription *dictionarySymbols;
};

//-----------------------------------------------------------------------

static void prv_integrity(const struct SPrvDataPrivateDisplay3D *dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(dataPrivate->light);
    assert_no_null(dataPrivate->positionCameraInitial);
    assert_no_null(dataPrivate->positionCameraCurrent);
    assert_no_null(dataPrivate->dictionarySymbols);
}

//-----------------------------------------------------------------------

static struct SPrvDataPrivateDisplay3D *prv_createDataPrivateDisplay3D(
                                class IWorld *world,
								class CLight **light, 
								class CPositionCamera **positionCameraInitial,
								class CPositionCamera **positionCameraCurrent,
								class CDictionaryDescription **dictionarySymbols)
{
	struct SPrvDataPrivateDisplay3D *dataPrivate;
	
	dataPrivate = MALLOC(struct SPrvDataPrivateDisplay3D);
	
	dataPrivate->world = world;
	dataPrivate->light = ASSIGN_PP_NO_NULL(light, class CLight);
	dataPrivate->positionCameraInitial = ASSIGN_PP_NO_NULL(positionCameraInitial, class CPositionCamera);
	dataPrivate->positionCameraCurrent = ASSIGN_PP_NO_NULL(positionCameraCurrent, class CPositionCamera);
	dataPrivate->dictionarySymbols = ASSIGN_PP_NO_NULL(dictionarySymbols, class CDictionaryDescription);
	
	prv_integrity(dataPrivate);

	return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroyDataPrivateDisplay3D(struct SPrvDataPrivateDisplay3D **dataPrivate)
{
	assert_no_null(dataPrivate);
	prv_integrity(*dataPrivate);
	
	delete (*dataPrivate)->light;
	delete (*dataPrivate)->positionCameraInitial;
	delete (*dataPrivate)->positionCameraCurrent;
	delete (*dataPrivate)->dictionarySymbols;
	
	FREE_T(dataPrivate, struct SPrvDataPrivateDisplay3D);
}

//-----------------------------------------------------------------------

static void prv_appendTransformation(const char *symbolTransformation, class CDictionaryDescription *dictionarySymbols)
{
    class IDescription *descriptionTransformation;

    assert_no_null(dictionarySymbols);

    descriptionTransformation = new CDescriptionTransformation3D();
    dictionarySymbols->addDescription(symbolTransformation, &descriptionTransformation);
}

//-----------------------------------------------------------------------

CDisplay3D::CDisplay3D(class IWorld *world, class CLight **light, class CPositionCamera **positionCameraInitial)
{
	class CPositionCamera *positionCameraCurrent;
	class CDictionaryDescription *dictionarySymbols;
	
	assert_no_null(positionCameraInitial);
	
	dictionarySymbols = new CDictionaryDescription;
	positionCameraCurrent = new CPositionCamera(*positionCameraInitial);

	prv_appendTransformation(CTransform3D::ID_SYMBOL_SCALE3D, dictionarySymbols);
    prv_appendTransformation(CTransform3D::ID_SYMBOL_TRASLATE3D, dictionarySymbols);
    prv_appendTransformation(CTransform3D::ID_SYMBOL_ROTATE3D, dictionarySymbols);
    prv_appendTransformation(CTransform3D::ID_SYMBOL_EXTRUSION3D, dictionarySymbols);
		
	m_dataPrivate = prv_createDataPrivateDisplay3D(world, light, positionCameraInitial, &positionCameraCurrent, &dictionarySymbols);
}

//-----------------------------------------------------------------------

CDisplay3D::~CDisplay3D()
{
	prv_destroyDataPrivateDisplay3D(&m_dataPrivate);
}

//-----------------------------------------------------------------------

void CDisplay3D::appendDescription(const char *symbol, class IDescription **description)
{
    prv_integrity(m_dataPrivate);
    m_dataPrivate->dictionarySymbols->addDescription(symbol, description);
}

//-----------------------------------------------------------------------

void CDisplay3D::drawScene(class IGraphics *graphics, const class CScene *scene)
{
    class CTypeDescription *evtDraw;
    class CGraphicsGL *graphicsGL;

    prv_integrity(m_dataPrivate);

    evtDraw = new CTypeDescriptionGL(m_dataPrivate->world, graphics, m_dataPrivate->dictionarySymbols);

    graphicsGL = dynamic_cast<class CGraphicsGL *>(graphics);

    if (graphicsGL != NULL)
    {
        m_dataPrivate->light->putLight(graphicsGL, IGraphics::LIGHT0);
        scene->processDraw(evtDraw);
    }
    else
        scene->processDraw(evtDraw);

    delete evtDraw;
}

//-----------------------------------------------------------------------

void CDisplay3D::setInitialPositionCamera(void)
{
	assert_no_null(m_dataPrivate);
	assert_no_null(m_dataPrivate->positionCameraInitial);
	assert_no_null(m_dataPrivate->positionCameraCurrent);
	
	delete m_dataPrivate->positionCameraCurrent;
	
	m_dataPrivate->positionCameraCurrent = new CPositionCamera(m_dataPrivate->positionCameraInitial);
}

//-----------------------------------------------------------------------

void CDisplay3D::positionCamera(class IGraphics *graphics) const
{
	assert_no_null(m_dataPrivate);
	assert_no_null(m_dataPrivate->positionCameraCurrent);
	
	m_dataPrivate->positionCameraCurrent->positionCamera(graphics);
}

//-----------------------------------------------------------------------

void CDisplay3D::makeRotationCamera(class IGraphics *graphics) const
{
	assert_no_null(m_dataPrivate);
	assert_no_null(m_dataPrivate->positionCameraCurrent);
	
	m_dataPrivate->positionCameraCurrent->makeRotationCamera(graphics);
}

//-----------------------------------------------------------------------

static double prv_velocityFunction(double x)
{
    return CMath::pow(x - 1, 3) + 1;
}

//-----------------------------------------------------------------------

void CDisplay3D::setRotationCamera(struct areaDibujo_t *areaDibujo, double rotXDegrees, double rotYDegrees, double rotZDegrees)
{
    double d, rotXInitial, rotYInitial, rotZInitial;

    assert_no_null(m_dataPrivate);

    m_dataPrivate->positionCameraCurrent->getRotation(&rotXInitial, &rotYInitial, &rotZInitial);

    for (d = 0.; d < 1.; d += .04)
    {
        double t;

        t = prv_velocityFunction(d);

        m_dataPrivate->positionCameraCurrent->setParametricRotation(
                    t,
                    rotXInitial, rotYInitial, rotZInitial,
                    rotXDegrees, rotYDegrees, rotZDegrees);

        areadibujo_redraw(areaDibujo);
        timesys_sleep(50);
    }

    m_dataPrivate->positionCameraCurrent->setParametricRotation(
                1.,
                rotXInitial, rotYInitial, rotZInitial,
                rotXDegrees, rotYDegrees, rotZDegrees);
    areadibujo_redraw(areaDibujo);
}


//-----------------------------------------------------------------------

void CDisplay3D::incrRotateCamera(struct areaDibujo_t *areaDibujo, double incrRotateX, double incrRotateY, double incrRotateZ)
{
	assert_no_null(m_dataPrivate);
	assert_no_null(m_dataPrivate->positionCameraCurrent);
	
	m_dataPrivate->positionCameraCurrent->incrRotationCamera(incrRotateX, incrRotateY, incrRotateZ);
    areadibujo_redraw(areaDibujo);
}

//-----------------------------------------------------------------------

void CDisplay3D::frontCamera(struct areaDibujo_t *areaDibujo, double step)
{
	assert_no_null(m_dataPrivate);
	assert_no_null(m_dataPrivate->positionCameraCurrent);
	
	m_dataPrivate->positionCameraCurrent->frontCamera(step);
    areadibujo_redraw(areaDibujo);
}

//-----------------------------------------------------------------------

void CDisplay3D::backCamera(struct areaDibujo_t *areaDibujo, double step)
{
	assert_no_null(m_dataPrivate);
	assert_no_null(m_dataPrivate->positionCameraCurrent);
	
	m_dataPrivate->positionCameraCurrent->backCamera(step);
    areadibujo_redraw(areaDibujo);
}
