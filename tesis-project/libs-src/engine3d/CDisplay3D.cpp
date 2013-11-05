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
	class CDictionaryDescription *dictionarySymbols;
};

//-----------------------------------------------------------------------

static void prv_integrity(const struct SPrvDataPrivateDisplay3D *dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(dataPrivate->light);
    assert_no_null(dataPrivate->dictionarySymbols);
}

//-----------------------------------------------------------------------

static struct SPrvDataPrivateDisplay3D *prv_createDataPrivateDisplay3D(
                                class IWorld *world,
								class CLight **light, 
								class CDictionaryDescription **dictionarySymbols)
{
	struct SPrvDataPrivateDisplay3D *dataPrivate;
	
	dataPrivate = MALLOC(struct SPrvDataPrivateDisplay3D);
	
	dataPrivate->world = world;
	dataPrivate->light = ASSIGN_PP_NO_NULL(light, class CLight);
	dataPrivate->dictionarySymbols = ASSIGN_PP_NO_NULL(dictionarySymbols, class CDictionaryDescription);
	
	prv_integrity(dataPrivate);

	return dataPrivate;
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

CDisplay3D::CDisplay3D(class IWorld *world, class CLight **light)
{
	class CDictionaryDescription *dictionarySymbols;
	
	dictionarySymbols = new CDictionaryDescription;

	prv_appendTransformation(CTransform3D::ID_SYMBOL_SCALE3D, dictionarySymbols);
    prv_appendTransformation(CTransform3D::ID_SYMBOL_TRASLATE3D, dictionarySymbols);
    prv_appendTransformation(CTransform3D::ID_SYMBOL_ROTATE3D, dictionarySymbols);
    prv_appendTransformation(CTransform3D::ID_SYMBOL_EXTRUSION3D, dictionarySymbols);
		
	m_dataPrivate = prv_createDataPrivateDisplay3D(world, light, &dictionarySymbols);
}

//-----------------------------------------------------------------------

CDisplay3D::~CDisplay3D()
{
    prv_integrity(m_dataPrivate);

    delete m_dataPrivate->light;
    delete m_dataPrivate->dictionarySymbols;

    FREE_T(&m_dataPrivate, struct SPrvDataPrivateDisplay3D);
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
