/* -----------------------------------------------------------------------------
 *  CDescriptionSpriteWithDataFigure.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CDescriptionSpriteWithDataFigure.hpp"

#include "asrtbas.h"
#include "memory.h"

#include "CImg.hpp"
#include "CGraphicsSprite.hpp"

struct prv_dataPrivateDataImageFigure_t
{
    const class CImg *imageConst;
    class CImg *imageMustDestroy;
};

struct prv_dataPrivateDescriptionSpriteWithDataFigure_t
{
    long dx, dy;
};

//-----------------------------------------------------------------------

static struct prv_dataPrivateDataImageFigure_t *prv_createDataImageFigure(
                            const class CImg *imageConst,
                            class CImg **imageMustDestroy)
{
    struct prv_dataPrivateDataImageFigure_t *dataPrivate;

    dataPrivate = MALLOC(struct prv_dataPrivateDataImageFigure_t);

    dataPrivate->imageConst = imageConst;
    dataPrivate->imageMustDestroy = ASSIGN_PP(imageMustDestroy, class CImg);

    return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroyDataImageFigure(struct prv_dataPrivateDataImageFigure_t **dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(*dataPrivate);

    if ((*dataPrivate)->imageMustDestroy != NULL)
        delete (*dataPrivate)->imageMustDestroy;

    FREE_T(dataPrivate, struct prv_dataPrivateDataImageFigure_t);
}

//-----------------------------------------------------------------------

static const class CImg *prv_getImage(const struct prv_dataPrivateDataImageFigure_t *dataPrivate)
{
    assert_no_null(dataPrivate);

    if (dataPrivate->imageConst != NULL)
        return dataPrivate->imageConst;
    else
    {
        assert_no_null(dataPrivate->imageMustDestroy);
        return dataPrivate->imageMustDestroy;
    }
}

//-----------------------------------------------------------------------

CDataImageFigure::CDataImageFigure(class CImg **image)
{
    m_dataPrivate = prv_createDataImageFigure(NULL, image);
}

//-----------------------------------------------------------------------

CDataImageFigure::CDataImageFigure(const class CImg *image)
{
    class CImg *imageMustDestroy;

    imageMustDestroy = NULL;
    m_dataPrivate = prv_createDataImageFigure(image, &imageMustDestroy);
}

//-----------------------------------------------------------------------

CDataImageFigure::~CDataImageFigure()
{
    prv_destroyDataImageFigure(&m_dataPrivate);
}

//-----------------------------------------------------------------------

void CDataImageFigure::drawImage(class CGraphicsSprite *graphicsSprite) const
{
    const class CImg *image;

    assert_no_null(m_dataPrivate);

    image = prv_getImage(m_dataPrivate);
    graphicsSprite->drawImage(image);
}

//-----------------------------------------------------------------------

void CDataImageFigure::size(unsigned long *sizeX, unsigned long *sizeY) const
{
    const class CImg *image;

    assert_no_null(m_dataPrivate);

    image = prv_getImage(m_dataPrivate);
    image->size(sizeX, sizeY);
}

//-----------------------------------------------------------------------

static struct prv_dataPrivateDescriptionSpriteWithDataFigure_t *prv_createDescriptionSpriteWithDataFigure(long dx, long dy)
{
    struct prv_dataPrivateDescriptionSpriteWithDataFigure_t *dataPrivate;

    dataPrivate = MALLOC(struct prv_dataPrivateDescriptionSpriteWithDataFigure_t);

    dataPrivate->dx = dx;
    dataPrivate->dy = dy;

    return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroyDescriptionSpriteWithDataFigure(struct prv_dataPrivateDescriptionSpriteWithDataFigure_t **dataPrivate)
{
    FREE_T(dataPrivate, struct prv_dataPrivateDescriptionSpriteWithDataFigure_t);
}

//-----------------------------------------------------------------------

CDescriptionSpriteWithDataFigure::CDescriptionSpriteWithDataFigure(long dx, long dy)
{
    m_dataPrivate = prv_createDescriptionSpriteWithDataFigure(dx, dy);
}

//-----------------------------------------------------------------------

CDescriptionSpriteWithDataFigure::~CDescriptionSpriteWithDataFigure()
{
    prv_destroyDescriptionSpriteWithDataFigure(&m_dataPrivate);
}

//-----------------------------------------------------------------------

void CDescriptionSpriteWithDataFigure::drawSymbol(class CGraphicsSprite *graphicsSprite, const class IDataSymbol *dataFigure)
{
    bool withTraslate;
    const class CDataImageFigure *dataImageFigure;

    assert_no_null(m_dataPrivate);

    dataImageFigure = dynamic_cast<const class CDataImageFigure *>(dataFigure);
    assert_no_null(dataImageFigure);

    if (m_dataPrivate->dx != 0 || m_dataPrivate->dy != 0)
        withTraslate = true;
    else
        withTraslate = false;

    if (withTraslate == true)
    {
        graphicsSprite->pushTransformation();
        graphicsSprite->translate(m_dataPrivate->dx, m_dataPrivate->dy);
    }

    dataImageFigure->drawImage(graphicsSprite);

    if (withTraslate == true)
        graphicsSprite->popTransformation();
}

//-----------------------------------------------------------------------

void CDescriptionSpriteWithDataFigure::size(const class IDataSymbol *dataFigure, unsigned long *sizeX, unsigned long *sizeY) const
{
    const class CDataImageFigure *dataImageFigure;

    dataImageFigure = dynamic_cast<const class CDataImageFigure *>(dataFigure);
    assert_no_null(dataImageFigure);

    dataImageFigure->size(sizeX, sizeY);
}
