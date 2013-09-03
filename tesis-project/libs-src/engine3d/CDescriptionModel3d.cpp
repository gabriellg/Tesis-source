/* -----------------------------------------------------------------------------
 *  CDescriptionModel3d.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CDescriptionModel3d.hpp"

#include "asrtbas.h"
#include "memory.h"

#include "CModel3d.hpp"
#include "IGraphics.hpp"

struct SDataPrivateDescriptionModel3d
{
    bool isFirstTime;
    class CModel3d *model;
};

//-----------------------------------------------------------------------

static void prv_integrityDescriptionModel3d(struct SDataPrivateDescriptionModel3d *dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(dataPrivate->model);
}

//-----------------------------------------------------------------------

static struct SDataPrivateDescriptionModel3d *prv_createDescriptionModel3d(
                            bool isFirstTime,
                            class CModel3d **model)
{
    struct SDataPrivateDescriptionModel3d *dataPrivate;

    dataPrivate = MALLOC(struct SDataPrivateDescriptionModel3d);

    dataPrivate->isFirstTime = isFirstTime;
    dataPrivate->model = ASSIGN_PP_NO_NULL(model, class CModel3d);

    prv_integrityDescriptionModel3d(dataPrivate);

    return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroyDescriptionModel3d(struct SDataPrivateDescriptionModel3d **dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(*dataPrivate);

    delete (*dataPrivate)->model;

    FREE_T(dataPrivate, struct SDataPrivateDescriptionModel3d);
}

//-----------------------------------------------------------------------

CDescriptionModel3d::CDescriptionModel3d(class CModel3d **model3d)
{
    bool isFirstTime;

    isFirstTime = true;
    m_dataPrivate = prv_createDescriptionModel3d(isFirstTime, model3d);
}

//-----------------------------------------------------------------------

CDescriptionModel3d::~CDescriptionModel3d()
{
    prv_destroyDescriptionModel3d(&m_dataPrivate);
}

//-----------------------------------------------------------------------

void CDescriptionModel3d::drawSymbol(class IGraphics *graphics, const class IDataSymbol *dataFigure)
{
    assert_no_null(graphics);

    if (m_dataPrivate->isFirstTime == true && graphics->hasManagerBlocks() == true)
    {
        m_dataPrivate->model->defineMaterialsInGraphics(graphics);
        m_dataPrivate->model->defineBlock(graphics);
        m_dataPrivate->isFirstTime = false;
    }

    m_dataPrivate->model->drawIdBlock(graphics);
}
