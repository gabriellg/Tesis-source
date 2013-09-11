//CGestorDisplay.cpp

#include "CGestorDisplays.hpp"

#include "IDisplay.hpp"

#include "asrtbas.h"
#include "memory.h"

#include "CArray.hpp"

struct prv_dataPrivateGestorDisplays_t
{
	class CArray<IDisplay> *displays;
	unsigned long indCurrentDisplay;
};

//-----------------------------------------------------------------------

static void prv_integrity(const struct prv_dataPrivateGestorDisplays_t *dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(dataPrivate->displays);
}

//-----------------------------------------------------------------------

static struct prv_dataPrivateGestorDisplays_t *prv_create(
                    class CArray<IDisplay> **displays,
					unsigned long indCurrentDisplay)
{
	struct prv_dataPrivateGestorDisplays_t *dataPrivate;
	
	dataPrivate = MALLOC(struct prv_dataPrivateGestorDisplays_t);
	
	dataPrivate->displays = ASSIGN_PP_NO_NULL(displays, class CArray<IDisplay>);
	dataPrivate->indCurrentDisplay = indCurrentDisplay;
	
	prv_integrity(dataPrivate);

	return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroy(struct prv_dataPrivateGestorDisplays_t **dataPrivate)
{
	assert_no_null(dataPrivate);
    prv_integrity(*dataPrivate);

	delete (*dataPrivate)->displays;
	
	FREE_T(dataPrivate, struct prv_dataPrivateGestorDisplays_t);
}

//-----------------------------------------------------------------------

CGestorDisplays::CGestorDisplays()
{
	class CArray<IDisplay> *displays;
	unsigned long indCurrentDisplay;
	
	displays = new CArray<IDisplay>;
	indCurrentDisplay = 0;
	
	m_dataPrivate = prv_create(&displays, indCurrentDisplay);
}

//-----------------------------------------------------------------------

CGestorDisplays::~CGestorDisplays()
{
	prv_destroy(&m_dataPrivate);
}

//-----------------------------------------------------------------------

void CGestorDisplays::appendDisplay(class IDisplay **display)
{
	assert_no_null(display);
	prv_integrity(m_dataPrivate);

	m_dataPrivate->displays->add(*display);
	*display = NULL;
}

//-----------------------------------------------------------------------

void CGestorDisplays::setInitialPositionCamera(void)
{
	unsigned long i, num;

	prv_integrity(m_dataPrivate);
		
	num = m_dataPrivate->displays->size();
		
	for (i = 0; i < num; i++)
	{
		class IDisplay *display;
			
		display = m_dataPrivate->displays->get(i);
		display->setInitialPositionCamera();
	}
}

//-----------------------------------------------------------------------

void CGestorDisplays::makeRotationCurrentDisplay(class IGraphics *graphics)
{
	class IDisplay *display;
	
	prv_integrity(m_dataPrivate);
	
	display = m_dataPrivate->displays->get(m_dataPrivate->indCurrentDisplay);
	display->makeRotationCamera(graphics);
}

//-----------------------------------------------------------------------

void CGestorDisplays::setRotationCameraCurrentDisplay(struct areaDibujo_t *areaDibujo, double rotXDegrees, double rotYDegrees, double rotZDegrees)
{
	class IDisplay *display;
	
	prv_integrity(m_dataPrivate);
	
	display = m_dataPrivate->displays->get(m_dataPrivate->indCurrentDisplay);
	display->setRotationCamera(areaDibujo, rotXDegrees, rotYDegrees, rotZDegrees);
}

//-----------------------------------------------------------------------

void CGestorDisplays::incrRotateCameraCurrentDisplay(struct areaDibujo_t *areaDibujo, double incrRotateX, double incrRotateY, double incrRotateZ)
{
	class IDisplay *display;
	
    prv_integrity(m_dataPrivate);

    display = m_dataPrivate->displays->get(m_dataPrivate->indCurrentDisplay);
	display->incrRotateCamera(areaDibujo, incrRotateX, incrRotateY, incrRotateZ);
}

//-----------------------------------------------------------------------

void CGestorDisplays::frontCameraCurrentDisplay(struct areaDibujo_t *areaDibujo, double step)
{
	class IDisplay *display;
	
    prv_integrity(m_dataPrivate);

    display = m_dataPrivate->displays->get(m_dataPrivate->indCurrentDisplay);
	display->frontCamera(areaDibujo, step);
}

//-----------------------------------------------------------------------

void CGestorDisplays::backCameraCurrentDisplay(struct areaDibujo_t *areaDibujo, double step)
{
	class IDisplay *display;
	
    prv_integrity(m_dataPrivate);

    display = m_dataPrivate->displays->get(m_dataPrivate->indCurrentDisplay);
	display->backCamera(areaDibujo, step);
}

//-----------------------------------------------------------------------

void CGestorDisplays::positionCameraCurrentDisplay(class IGraphics *graphics) const
{
	class IDisplay *display;
	
    prv_integrity(m_dataPrivate);

    display = m_dataPrivate->displays->get(m_dataPrivate->indCurrentDisplay);
	display->positionCamera(graphics);
}

//-----------------------------------------------------------------------

void CGestorDisplays::drawScene(class IGraphics *graphics, const class CScene *scene)
{
	class IDisplay *display;
	
    prv_integrity(m_dataPrivate);

    display = m_dataPrivate->displays->get(m_dataPrivate->indCurrentDisplay);
    display->drawScene(graphics, scene);
}

//-----------------------------------------------------------------------

bool CGestorDisplays::isArea3DCurrentDisplay(void) const
{
	class IDisplay *display;
	
    prv_integrity(m_dataPrivate);

    display = m_dataPrivate->displays->get(m_dataPrivate->indCurrentDisplay);
	return display->isArea3D();
}
