/* -----------------------------------------------------------------------------
 *  CDisplayLimitsSprite.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CDisplayLimitsSprite.inl"

#include "CTypeDescLimitsSprite.inl"

#include "asrtbas.h"
#include "memory.h"
#include "CArray.hpp"
#include "CDictionaryDescription.hpp"
#include "CScene.hpp"

struct SPrvDataPrivateDisplayLimitsSprite
{
    class CWorldGeometry2DPixels *worldGeometry2D;
    class CDictionaryDescription *dictionarySymbols;
};

//-----------------------------------------------------------------------

static void prv_integrity(const struct SPrvDataPrivateDisplayLimitsSprite *dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(dataPrivate->dictionarySymbols);
}

//-----------------------------------------------------------------------

static struct SPrvDataPrivateDisplayLimitsSprite *prv_createDisplayLimitsSprite(
                        class CWorldGeometry2DPixels *worldGeometry2D,
                        class CDictionaryDescription **dictionarySymbols)
{
    struct SPrvDataPrivateDisplayLimitsSprite *dataPrivate;

    dataPrivate = MALLOC(struct SPrvDataPrivateDisplayLimitsSprite);

    dataPrivate->worldGeometry2D = worldGeometry2D;
    dataPrivate->dictionarySymbols = ASSIGN_PP_NO_NULL(dictionarySymbols, class CDictionaryDescription);

    return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroyDisplayLimitsSprite(struct SPrvDataPrivateDisplayLimitsSprite **dataPrivate)
{
    assert_no_null(dataPrivate);
    prv_integrity(*dataPrivate);

    delete (*dataPrivate)->dictionarySymbols;

    FREE_T(dataPrivate, struct SPrvDataPrivateDisplayLimitsSprite);
}

//-----------------------------------------------------------------------

CDisplayLimitsSprite::CDisplayLimitsSprite(class CWorldGeometry2DPixels *worldGeometry2D)
{
    class CDictionaryDescription *dictionarySymbols;

    dictionarySymbols = new CDictionaryDescription;



    m_dataPrivate = prv_createDisplayLimitsSprite(worldGeometry2D, &dictionarySymbols);
}

//-----------------------------------------------------------------------

CDisplayLimitsSprite::~CDisplayLimitsSprite()
{
    prv_destroyDisplayLimitsSprite(&m_dataPrivate);
}

//-----------------------------------------------------------------------

void CDisplayLimitsSprite::appendDescription(const char *symbol, class IDescription **description)
{
    prv_integrity(m_dataPrivate);
    m_dataPrivate->dictionarySymbols->addDescription(symbol, description);
}

//-----------------------------------------------------------------------

void CDisplayLimitsSprite::drawScene(class IGraphics *graphics, const class CScene *scene)
{
    class CTypeDescription *evtDraw;

    prv_integrity(m_dataPrivate);

    evtDraw = new CTypeDescLimitsSprite(m_dataPrivate->worldGeometry2D, m_dataPrivate->dictionarySymbols);
    scene->processDraw(evtDraw);
    delete evtDraw;
}
