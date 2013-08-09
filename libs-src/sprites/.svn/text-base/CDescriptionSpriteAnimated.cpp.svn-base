/* -----------------------------------------------------------------------------
 *  CDescriptionSprite.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CDescriptionSpriteAnimated.hpp"

#include "asrtbas.h"
#include "memory.h"

#include "CArray.hpp"
#include "CImg.hpp"
#include "CGraphicsSprite.hpp"

struct prv_dataPrivateDescriptionSpriteAnimated_t
{
    unsigned long indSprite;
    class CArray<CImg> *images;
};

//-----------------------------------------------------------------------

static struct prv_dataPrivateDescriptionSpriteAnimated_t *prv_createDescriptionSpriteAnimated(
                    class CArray<CImg> **images,
                    unsigned long indSprite)
{
    struct prv_dataPrivateDescriptionSpriteAnimated_t *dataPrivate;

    dataPrivate = MALLOC(struct prv_dataPrivateDescriptionSpriteAnimated_t);

    dataPrivate->indSprite = indSprite;
    dataPrivate->images = ASSIGN_PP_NO_NULL(images, class CArray<CImg>);

    return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroyDescriptionSpriteAnimated(struct prv_dataPrivateDescriptionSpriteAnimated_t **dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(*dataPrivate);

    delete (*dataPrivate)->images;

    FREE_T(dataPrivate, struct prv_dataPrivateDescriptionSpriteAnimated_t);
}

//-----------------------------------------------------------------------

CDescriptionSpriteAnimated::CDescriptionSpriteAnimated(class CArray<CImg> **images)
{
    unsigned long indSprite;

    indSprite = 0;
    m_dataPrivate = prv_createDescriptionSpriteAnimated(images, indSprite);
}

//-----------------------------------------------------------------------

CDescriptionSpriteAnimated::~CDescriptionSpriteAnimated(void)
{
    prv_destroyDescriptionSpriteAnimated(&m_dataPrivate);
}

//-----------------------------------------------------------------------

void CDescriptionSpriteAnimated::drawSymbol(
                        class CGraphicsSprite *graphicsSprite,
                        const class IDataSymbol *dataFigure)
{
    class CImg *image;
    unsigned long num;

    assert_no_null(m_dataPrivate);
    assert(dataFigure == NULL);

    image = m_dataPrivate->images->get(m_dataPrivate->indSprite);
    graphicsSprite->drawImage(image);

    num = m_dataPrivate->images->size();
    assert(num > 0);
    m_dataPrivate->indSprite = (m_dataPrivate->indSprite + 1) % num;
}

//-----------------------------------------------------------------------

void CDescriptionSpriteAnimated::size(const class IDataSymbol *dataFigure, unsigned long *width, unsigned long *height) const
{
    const class CImg *img;

    assert(dataFigure == NULL);

    img = m_dataPrivate->images->get(0);
    assert_no_null(img);

    img->size(width, height);
}
