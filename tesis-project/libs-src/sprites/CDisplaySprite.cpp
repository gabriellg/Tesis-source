//CDisplaySprite.cpp

#include "CDisplaySprite.hpp"

#include "IWorld.hpp"
#include "CTypeDescDrawSprite.inl"

#include "asrtbas.h"
#include "memory.h"
#include "CImg.hpp"
#include "CDictionaryDescription.hpp"
#include "CTransform2D.hpp"
#include "CScene.hpp"
#include "IGraphics.hpp"
#include "CGraphicsSprite.hpp"

struct prv_dataPrivateDisplaySprite_t
{
    bool originCartesian;
    unsigned long scaleX, scaleY;
    class CImg *imageBackground;
	unsigned long width, height;
	class IWorld *world;
    class CDictionaryDescription *dictionarySymbols;
};

//-----------------------------------------------------------------------

static void prv_integrity(const struct prv_dataPrivateDisplaySprite_t *data)
{
    assert_no_null(data);
    assert_no_null(data->dictionarySymbols);
}

//-----------------------------------------------------------------------

static struct prv_dataPrivateDisplaySprite_t *prv_create(
                        bool originCartesian,
                        unsigned long scaleX, unsigned long scaleY,
                        class CImg **imageBackground,
						unsigned long width,
						unsigned long height,
						class IWorld *world,
						class CDictionaryDescription **dictionarySymbols)
{
	struct prv_dataPrivateDisplaySprite_t *dataPrivate;
	
	dataPrivate = MALLOC(struct prv_dataPrivateDisplaySprite_t);
	
	dataPrivate->originCartesian = originCartesian;

	dataPrivate->scaleX = scaleX;
	dataPrivate->scaleY = scaleY;

	dataPrivate->imageBackground = ASSIGN_PP(imageBackground, class CImg);

	dataPrivate->width = width;
	dataPrivate->height = height;

	dataPrivate->world = world;
	dataPrivate->dictionarySymbols = ASSIGN_PP_NO_NULL(dictionarySymbols, class CDictionaryDescription);
	
	prv_integrity(dataPrivate);

	return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_descriptionTransformation(
                            const char *idSymbolTransformation,
                            class CDictionaryDescription *dictionarySymbols)
{
    class IDescription *descriptionTransformation;

    descriptionTransformation = new CDescriptionTransformation();
    dictionarySymbols->addDescription(idSymbolTransformation, &descriptionTransformation);
}

//-----------------------------------------------------------------------

static struct prv_dataPrivateDisplaySprite_t *prv_createWithDictionary(
                        bool originCartesian,
                        unsigned long scaleX, unsigned long scaleY,
                        class CImg **imageBackground,
                        unsigned long width,
                        unsigned long height,
                        class IWorld *world)
{
    class CDictionaryDescription *dictionarySymbols;

    dictionarySymbols = new CDictionaryDescription();

    prv_descriptionTransformation(CTransform2D::ID_SYMBOL_TRASLATE2D, dictionarySymbols);
    prv_descriptionTransformation(CTransform2D::ID_SYMBOL_ROTATE2D, dictionarySymbols);

    return prv_create(originCartesian,scaleX, scaleY, imageBackground, width, height, world, &dictionarySymbols);
}

//-----------------------------------------------------------------------

static void prv_destroy(struct prv_dataPrivateDisplaySprite_t **dataPrivate)
{
    assert_no_null(dataPrivate);
    prv_integrity(*dataPrivate);

    if ((*dataPrivate)->imageBackground != NULL)
        delete (*dataPrivate)->imageBackground;

    delete (*dataPrivate)->dictionarySymbols;

	FREE_T(dataPrivate, struct prv_dataPrivateDisplaySprite_t);
}

//-----------------------------------------------------------------------

CDisplaySprite::CDisplaySprite(
					unsigned long width, unsigned long height,
					class IWorld *world)
{
    bool originCartesian;
    class CImg *imageBackground;
    unsigned long scaleX, scaleY;

    originCartesian = false;
    scaleX = 1;
    scaleY = 1;
    imageBackground = NULL;

	m_dataPrivate = prv_createWithDictionary(originCartesian, scaleX, scaleY, &imageBackground, width, height, world);
}

//-----------------------------------------------------------------------

CDisplaySprite::CDisplaySprite(
                    class CImg **imageBackground,
                    class IWorld *world)
{
    bool originCartesian;
    unsigned long width, height;
    unsigned long scaleX, scaleY;

    originCartesian = false;
    scaleX = 1;
    scaleY = 1;

    assert_no_null(imageBackground);
    assert_no_null(*imageBackground);

    (*imageBackground)->size(&width, &height);

    m_dataPrivate = prv_createWithDictionary(originCartesian, scaleX, scaleY, imageBackground, width, height, world);
}

//-----------------------------------------------------------------------

CDisplaySprite::CDisplaySprite(
                    bool originCartesian,
                    unsigned long scaleX, unsigned long scaleY,
                    unsigned long width, unsigned long height,
                    class IWorld *world)
{
    class CImg *imageBackground;

    imageBackground = NULL;
    m_dataPrivate = prv_createWithDictionary(originCartesian, scaleX, scaleY, &imageBackground, width, height, world);
}

//-----------------------------------------------------------------------

CDisplaySprite::~CDisplaySprite()
{
	prv_destroy(&m_dataPrivate);
}

//-----------------------------------------------------------------------

void CDisplaySprite::appendDescription(const char *symbol, class IDescription **description)
{
    prv_integrity(m_dataPrivate);
    m_dataPrivate->dictionarySymbols->addDescription(symbol, description);
}

//-----------------------------------------------------------------------

static void prv_setGraphicsSprite(
                        class CGraphicsSprite *graphicsSprite,
                        bool originCartesian,
                        unsigned long scaleX, unsigned long scaleY)
{
    assert_no_null(graphicsSprite);

    if (originCartesian == true)
        graphicsSprite->setOriginCartesian();

    graphicsSprite->scale(scaleX, scaleY);
}

//-----------------------------------------------------------------------

void CDisplaySprite::drawScene(class IGraphics *graphics, const class CScene *scene)
{
    class CTypeDescription *evtDraw;
    class CGraphicsSprite *graphicsSprite;
    class CImg *imageResult;
    unsigned long width, height;

    prv_integrity(m_dataPrivate);

    if (m_dataPrivate->imageBackground != NULL)
        graphicsSprite = new CGraphicsSprite(m_dataPrivate->imageBackground);
    else
        graphicsSprite = new CGraphicsSprite(m_dataPrivate->width, m_dataPrivate->height, 128, 128, 128);

    prv_setGraphicsSprite(graphicsSprite, m_dataPrivate->originCartesian, m_dataPrivate->scaleX, m_dataPrivate->scaleY);

    evtDraw = new CTypeDescDrawSprite(graphicsSprite, m_dataPrivate->world, m_dataPrivate->dictionarySymbols);

    scene->processDraw(evtDraw);

    delete evtDraw;

    imageResult = graphicsSprite->getImage();
    imageResult->size(&width, &height);
    graphics->drawImageRGB(0., 0., width, height, imageResult);

    delete imageResult;
    delete graphicsSprite;
}

//-----------------------------------------------------------------------

bool CDisplaySprite::withSize(unsigned long *width, unsigned long *height) const
{
    assert_no_null(width);
    assert_no_null(height);

    *width = m_dataPrivate->width;
    *height = m_dataPrivate->height;

    return true;
}
