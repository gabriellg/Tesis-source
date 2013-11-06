//CLight3d.hpp

#include "CLight3d.hpp"

#include "asrtbas.h"
#include "memory.h"

#include "CLight.hpp"

struct SPrvLight3d
{
    class CLight *light;
};

//-----------------------------------------------------------------------

static struct SPrvLight3d *prv_createLight3d(class CLight **light)
{
    struct SPrvLight3d *dataPrivate;

    dataPrivate = new struct SPrvLight3d;

    dataPrivate->light = ASSIGN_PP_NO_NULL(light, class CLight);

    return dataPrivate;
}

//---------------------------------------------------------------

CLight3d::CLight3d(class CLight **light)
{
    m_dataPrivate = prv_createLight3d(light);
}

//---------------------------------------------------------------

CLight3d::~CLight3d()
{
    assert_no_null(m_dataPrivate);

    DELETE_OBJECT(&m_dataPrivate->light, class CLight);

    DELETE_OBJECT(&m_dataPrivate, struct SPrvLight3d);
}

//---------------------------------------------------------------

void CLight3d::drawSymbol(class IGraphics *graphics, const class IDataSymbol *dataFigure)
{
    assert_no_null(m_dataPrivate);
    m_dataPrivate->light->putLight(graphics, IGraphics::LIGHT0);
}
