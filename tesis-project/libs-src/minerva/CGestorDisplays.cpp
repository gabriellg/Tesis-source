//CGestorDisplay.cpp

#include "CGestorDisplays.hpp"

#include "ITraslatorDisplay.hpp"

#include "asrtbas.h"
#include "memory.h"

#include "CArray.hpp"

struct prv_dataPrivateGestorDisplays_t
{
	class CArray<ITraslatorDisplay> *displays;
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
                    class CArray<ITraslatorDisplay> **displays,
					unsigned long indCurrentDisplay)
{
	struct prv_dataPrivateGestorDisplays_t *dataPrivate;
	
	dataPrivate = MALLOC(struct prv_dataPrivateGestorDisplays_t);
	
	dataPrivate->displays = ASSIGN_PP_NO_NULL(displays, class CArray<ITraslatorDisplay>);
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
	class CArray<ITraslatorDisplay> *displays;
	unsigned long indCurrentDisplay;
	
	displays = new CArray<ITraslatorDisplay>;
	indCurrentDisplay = 0;
	
	m_dataPrivate = prv_create(&displays, indCurrentDisplay);
}

//-----------------------------------------------------------------------

CGestorDisplays::~CGestorDisplays()
{
	prv_destroy(&m_dataPrivate);
}

//-----------------------------------------------------------------------

void CGestorDisplays::appendDisplay(class ITraslatorDisplay **display)
{
	assert_no_null(display);
	prv_integrity(m_dataPrivate);

	m_dataPrivate->displays->add(*display);
	*display = NULL;
}

//-----------------------------------------------------------------------

void CGestorDisplays::drawScene(class IGraphics *graphics, const class CScene *scene)
{
	class ITraslatorDisplay *display;
	
    prv_integrity(m_dataPrivate);

    display = m_dataPrivate->displays->get(m_dataPrivate->indCurrentDisplay);
    display->drawScene(graphics, scene);
}
