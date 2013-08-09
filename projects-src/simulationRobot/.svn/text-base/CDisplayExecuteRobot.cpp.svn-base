/* -----------------------------------------------------------------------------
 *  CDisplayExecuteRobot.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CDisplayExecuteRobot.inl"

#include "CTypeDescriptionExecuteRobot.inl"
#include "CTransformRobot2D.inl"

#include "asrtbas.h"
#include "memory.h"
#include "CScene.hpp"
#include "CDictionaryDescription.hpp"

struct SPrvDataPrivateDisplayExecuteRobot
{
    class CWorldSimulationRobot *world;
    class CDictionaryDescription *dictionarySymbols;
};

//-----------------------------------------------------------------------

static void prv_integrity(const struct SPrvDataPrivateDisplayExecuteRobot *dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(dataPrivate->dictionarySymbols);
}

//-----------------------------------------------------------------------

static struct SPrvDataPrivateDisplayExecuteRobot *prv_createDisplayExecuteRobot(
                        class CWorldSimulationRobot *world,
                        class CDictionaryDescription **dictionarySymbols)
{
    struct SPrvDataPrivateDisplayExecuteRobot *dataPrivate;

    dataPrivate = MALLOC(struct SPrvDataPrivateDisplayExecuteRobot);

    dataPrivate->world = world;
    dataPrivate->dictionarySymbols = ASSIGN_PP_NO_NULL(dictionarySymbols, class CDictionaryDescription);

    prv_integrity(dataPrivate);

    return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroyDisplayExecuteRobot(struct SPrvDataPrivateDisplayExecuteRobot **dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(*dataPrivate);

    delete (*dataPrivate)->dictionarySymbols;

    FREE_T(dataPrivate, struct SPrvDataPrivateDisplayExecuteRobot);
}

//-----------------------------------------------------------------------

static void prv_appendSymbol(const char *idSymbol, class CDictionaryDescription *dictionarySymbols)
{
    class IDescription *descriptionTransformation;

    descriptionTransformation = new CDescriptionExecuteTransformation();
    dictionarySymbols->addDescription(idSymbol, &descriptionTransformation);
}

//-----------------------------------------------------------------------

CDisplayExecuteRobot::CDisplayExecuteRobot(class CWorldSimulationRobot *world)
{
    class CDictionaryDescription *dictionarySymbols;

    dictionarySymbols = new CDictionaryDescription;

    prv_appendSymbol(CTransformRobot::ID_TRASLATE_ROBOT, dictionarySymbols);
    prv_appendSymbol(CTransformRobot::ID_ROTATE_ROBOT, dictionarySymbols);

    m_dataPrivate = prv_createDisplayExecuteRobot(world, &dictionarySymbols);
}

//-----------------------------------------------------------------------

CDisplayExecuteRobot::~CDisplayExecuteRobot()
{
    prv_destroyDisplayExecuteRobot(&m_dataPrivate);
}

//-----------------------------------------------------------------------

void CDisplayExecuteRobot::appendDescription(const char *symbol, class IDescription **description)
{
    prv_integrity(m_dataPrivate);
    m_dataPrivate->dictionarySymbols->addDescription(symbol, description);
}

//-----------------------------------------------------------------------

void CDisplayExecuteRobot::drawScene(class IGraphics *graphics, const class CScene *scene)
{
    class CTypeDescriptionExecuteRobot *evtDraw;

    evtDraw = new CTypeDescriptionExecuteRobot(m_dataPrivate->world, m_dataPrivate->dictionarySymbols);

    scene->processDraw(evtDraw);

    delete evtDraw;
}
