/* -----------------------------------------------------------------------------
 *  CDescriptionSpriteRotated.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CDescriptionSpriteRotated.hpp"

#include "asrtbas.h"
#include "CArray.hpp"
#include "CImg.hpp"
#include "CMath.hpp"
#include "CGraphicsSprite.hpp"

//-----------------------------------------------------------------------

CDescriptionSpriteRotated::CDescriptionSpriteRotated(class CArray<CImg> **images)
{
    m_dataPrivate = (struct prv_dataPrivateDescriptionSpriteRotated_t *)*images;
    *images = NULL;
}

//-----------------------------------------------------------------------

CDescriptionSpriteRotated::~CDescriptionSpriteRotated(void)
{
    class CArray<CImg> *images;

    images = (class CArray<CImg> *)m_dataPrivate;
    delete images;
}

//-----------------------------------------------------------------------

static unsigned long prv_getIndexWithAngle(double angleCurrent, unsigned long numImages)
{
    long ind, numTimesTwoPi;
    double twoPi, angleBetweenZeroAndTwoPi;

    twoPi = 2. * CMath::PI;

    numTimesTwoPi = (long)(angleCurrent / (twoPi));
    angleBetweenZeroAndTwoPi = angleCurrent - numTimesTwoPi * twoPi;

    if (angleBetweenZeroAndTwoPi < 0.)
        angleBetweenZeroAndTwoPi += twoPi;

    ind = (long)(angleBetweenZeroAndTwoPi * numImages / twoPi);
    assert(ind >= 0);

    return (unsigned long)ind;
}

//-----------------------------------------------------------------------

void CDescriptionSpriteRotated::drawSymbol(
                        class CGraphicsSprite *graphicsSprite,
                        const class IDataSymbol *dataFigure)
{
    class CImg *image;
    unsigned long ind, numImage;
    class CArray<CImg> *images;
    double angleCurrent;

    assert_no_null(m_dataPrivate);
    assert(dataFigure == NULL);

    images = (class CArray<CImg> *)m_dataPrivate;

    numImage = images->size();

    graphicsSprite->getRotate(&angleCurrent);
    ind = prv_getIndexWithAngle(angleCurrent, numImage);

    image = images->get(ind);
    graphicsSprite->drawImage(image);
}

//-----------------------------------------------------------------------

void CDescriptionSpriteRotated::size(const class IDataSymbol *dataFigure, unsigned long *width, unsigned long *height) const
{
    const class CImg *img;
    class CArray<CImg> *images;

    assert_no_null(m_dataPrivate);
    assert(dataFigure == NULL);

    images = (class CArray<CImg> *)m_dataPrivate;

    img = images->get(0);
    assert_no_null(img);

    img->size(width, height);
}
