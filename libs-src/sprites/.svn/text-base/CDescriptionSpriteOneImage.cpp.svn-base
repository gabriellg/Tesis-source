/* -----------------------------------------------------------------------------
 *  CDescriptionSpriteOneImage.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CDescriptionSpriteOneImage.hpp"

#include "memory.h"
#include "CImg.hpp"
#include "CGraphicsSprite.hpp"

#include "asrtbas.h"
#include "memory.h"

struct prv_dataPrivateDescriptionSpriteOneImage_t
{
    enum CGraphicsSprite::EJustificeHorizontal_t justificeHorizontal;
    enum CGraphicsSprite::EJustificeVertical_t justificeVertical;
    unsigned long scaleX, scaleY;
    long dx, dy;
    class CImg *image;
};

//-----------------------------------------------------------------------

static struct prv_dataPrivateDescriptionSpriteOneImage_t *prv_createDescriptionSpriteOneImage(
                    enum CGraphicsSprite::EJustificeHorizontal_t justificeHorizontal,
                    enum CGraphicsSprite::EJustificeVertical_t justificeVertical,
                    long dx, long dy,
                    class CImg **image)
{
    struct prv_dataPrivateDescriptionSpriteOneImage_t *dataPrivate;

    dataPrivate = MALLOC(struct prv_dataPrivateDescriptionSpriteOneImage_t);

    dataPrivate->justificeHorizontal = justificeHorizontal;
    dataPrivate->justificeVertical = justificeVertical;

    dataPrivate->dx = dx;
    dataPrivate->dy = dy;
    dataPrivate->image = ASSIGN_PP_NO_NULL(image, class CImg);

    return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroyDescriptionSpriteOneImage(struct prv_dataPrivateDescriptionSpriteOneImage_t **dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(*dataPrivate);

    delete (*dataPrivate)->image;

    FREE_T(dataPrivate, struct prv_dataPrivateDescriptionSpriteOneImage_t);
}

//-----------------------------------------------------------------------

CDescriptionSpriteOneImage::CDescriptionSpriteOneImage(class CImg **image)
{
    enum CGraphicsSprite::EJustificeHorizontal_t justificeHorizontal;
    enum CGraphicsSprite::EJustificeVertical_t justificeVertical;
    long dx, dy;

    dx = 0;
    dy = 0;

    justificeHorizontal = CGraphicsSprite::LEFT;
    justificeVertical = CGraphicsSprite::UP;

    m_dataPrivate = prv_createDescriptionSpriteOneImage(justificeHorizontal, justificeVertical, dx, dy, image);
}

//-----------------------------------------------------------------------

CDescriptionSpriteOneImage::CDescriptionSpriteOneImage(long dx, long dy, class CImg **image)
{
    enum CGraphicsSprite::EJustificeHorizontal_t justificeHorizontal;
    enum CGraphicsSprite::EJustificeVertical_t justificeVertical;

    justificeHorizontal = CGraphicsSprite::LEFT;
    justificeVertical = CGraphicsSprite::UP;

    m_dataPrivate = prv_createDescriptionSpriteOneImage(justificeHorizontal, justificeVertical, dx, dy, image);
}

//-----------------------------------------------------------------------

CDescriptionSpriteOneImage::CDescriptionSpriteOneImage(struct prv_dataPrivateDescriptionSpriteOneImage_t **dataPrivate)
{
    m_dataPrivate = ASSIGN_PP_NO_NULL(dataPrivate, struct prv_dataPrivateDescriptionSpriteOneImage_t);
}

//-----------------------------------------------------------------------

class CDescriptionSpriteOneImage *CDescriptionSpriteOneImage::createSpriteOneImageInCenter(class CImg **image)
{
    struct prv_dataPrivateDescriptionSpriteOneImage_t *dataPrivate;
    enum CGraphicsSprite::EJustificeHorizontal_t justificeHorizontal;
    enum CGraphicsSprite::EJustificeVertical_t justificeVertical;
    long dx, dy;

    dx = 0;
    dy = 0;

    justificeHorizontal = CGraphicsSprite::CENTER_HORIZONTAL;
    justificeVertical = CGraphicsSprite::CENTER_VERTICAL;

    dataPrivate = prv_createDescriptionSpriteOneImage(justificeHorizontal, justificeVertical, dx, dy, image);
    return new CDescriptionSpriteOneImage(&dataPrivate);
}

//-----------------------------------------------------------------------

CDescriptionSpriteOneImage::~CDescriptionSpriteOneImage()
{
    prv_destroyDescriptionSpriteOneImage(&m_dataPrivate);
}

//-----------------------------------------------------------------------

void CDescriptionSpriteOneImage::drawSymbol(
                        class CGraphicsSprite *graphicsSprite,
                        const class IDataSymbol *dataFigure)
{
    bool withTraslate;

    assert_no_null(m_dataPrivate);
    assert(dataFigure == NULL);

    if (m_dataPrivate->dx != 0 || m_dataPrivate->dy != 0)
        withTraslate = true;
    else
        withTraslate = false;

    if (withTraslate == true)
    {
        graphicsSprite->pushTransformation();
        graphicsSprite->translate(m_dataPrivate->dx, m_dataPrivate->dy);
    }

    graphicsSprite->drawImageWithJustifice(
                m_dataPrivate->image,
                m_dataPrivate->justificeHorizontal,
                m_dataPrivate->justificeVertical);

    if (withTraslate == true)
        graphicsSprite->popTransformation();
}

//-----------------------------------------------------------------------

void CDescriptionSpriteOneImage::size(const class IDataSymbol *dataFigure, unsigned long *sizeX, unsigned long *sizeY) const
{
    assert_no_null(m_dataPrivate);
    assert(dataFigure == NULL);

    m_dataPrivate->image->size(sizeX, sizeY);
}
