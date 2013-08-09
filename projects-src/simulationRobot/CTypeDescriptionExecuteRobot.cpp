/* -----------------------------------------------------------------------------
 *  CTypeDescriptionExecuteRobot.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CTypeDescriptionExecuteRobot.inl"

#include "CWorldSimulationRobot.inl"

#include "asrtbas.h"
#include "memory.h"
#include "CTransform2D.hpp"
#include "CStackTransformation.hpp"
#include "CMath.hpp"
#include "CArraysTypes.hpp"
#include "IDescriptionCommandRobot.inl"
#include "CDictionaryDescription.hpp"

struct SPrvDataPrivateTypeDescriptionExecuteRobot
{
    class CWorldSimulationRobot *worldRef;
    const class CDictionaryDescription *dictionaryDescription;
};

//-----------------------------------------------------------------------

static void prv_integrityTypeDescriptionExecuteRobot(struct SPrvDataPrivateTypeDescriptionExecuteRobot *dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(dataPrivate->worldRef);
    assert_no_null(dataPrivate->dictionaryDescription);
}

//-----------------------------------------------------------------------

static struct SPrvDataPrivateTypeDescriptionExecuteRobot *prv_createTypeDescriptionExecuteRobot(
                        class CWorldSimulationRobot *worldRef,
                        const class CDictionaryDescription *dictionaryDescription)
{
    struct SPrvDataPrivateTypeDescriptionExecuteRobot *dataPrivate;

    dataPrivate = new SPrvDataPrivateTypeDescriptionExecuteRobot;

    dataPrivate->worldRef = worldRef;
    dataPrivate->dictionaryDescription = dictionaryDescription;

    prv_integrityTypeDescriptionExecuteRobot(dataPrivate);

    return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroyTypeDescriptionExecuteRobot(struct SPrvDataPrivateTypeDescriptionExecuteRobot **dataPrivate)
{
    assert_no_null(dataPrivate);
    prv_integrityTypeDescriptionExecuteRobot(*dataPrivate);

    DELETE_OBJECT(dataPrivate, struct SPrvDataPrivateTypeDescriptionExecuteRobot);
}

//-----------------------------------------------------------------------

CTypeDescriptionExecuteRobot::CTypeDescriptionExecuteRobot(
                    class CWorldSimulationRobot *worldRef,
                    const class CDictionaryDescription *dictionaryDescription):
                    CTypeDescription("CTypeDescriptionExecuteRobot")
{
    m_dataPrivate = prv_createTypeDescriptionExecuteRobot(worldRef, dictionaryDescription);
}

//-----------------------------------------------------------------------

CTypeDescriptionExecuteRobot::~CTypeDescriptionExecuteRobot()
{
    prv_destroyTypeDescriptionExecuteRobot(&m_dataPrivate);
}

//-----------------------------------------------------------------------

static bool prv_existDescriptionSprite(
                        const class CDictionaryDescription *dictionaryDraw,
                        const char *symbol,
                        class IDescriptionCommandRobot **descriptionCommand)
{
    class IDescription *description;
    bool exist;

    assert_no_null(dictionaryDraw);
    assert_no_null(descriptionCommand);

    exist = dictionaryDraw->searchSymbol(symbol, &description);

    if (exist == true)
    {
        *descriptionCommand = dynamic_cast<IDescriptionCommandRobot *>(description);
        assert_no_null(*descriptionCommand);
    }

    return exist;
}

//-----------------------------------------------------------------------

void CTypeDescriptionExecuteRobot::beginSymbol(const char *symbol, const class IDataSymbol *dataFigure)
{
    class IDescriptionCommandRobot *descriptionCommand;

    prv_integrityTypeDescriptionExecuteRobot(m_dataPrivate);

    if (prv_existDescriptionSprite(
                            m_dataPrivate->dictionaryDescription,
                            symbol,
                            &descriptionCommand) == true)
    {
        descriptionCommand->beginSymbol(m_dataPrivate->worldRef, dataFigure);
    }
}

//-----------------------------------------------------------------------

void CTypeDescriptionExecuteRobot::drawSymbol(const char *symbol, const class IDataSymbol *dataFigure)
{
    class IDescriptionCommandRobot *descriptionCommand;

    prv_integrityTypeDescriptionExecuteRobot(m_dataPrivate);

    if (prv_existDescriptionSprite(
                            m_dataPrivate->dictionaryDescription,
                            symbol,
                            &descriptionCommand) == true)
    {
        descriptionCommand->drawSymbol(m_dataPrivate->worldRef, dataFigure);
    }
}

//-----------------------------------------------------------------------

void CTypeDescriptionExecuteRobot::endSymbol(const char *symbol, const class IDataSymbol *dataFigure)
{
    class IDescriptionCommandRobot *descriptionCommand;

    prv_integrityTypeDescriptionExecuteRobot(m_dataPrivate);

    if (prv_existDescriptionSprite(
                            m_dataPrivate->dictionaryDescription,
                            symbol,
                            &descriptionCommand) == true)
    {
        descriptionCommand->endSymbol(m_dataPrivate->worldRef, dataFigure);
    }
}
