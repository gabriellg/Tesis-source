//CEventGL.cpp

#include "CTypeDescriptionGL.hpp"

#include "IDescriptionGraphics3D.hpp"

#include "asrtbas.h"
#include "memory.h"
#include "CDictionaryDescription.hpp"
#include "IWorld.hpp"

struct SPrvDataPrivateTypeDescriptionGL
{
    class IWorld *world;
    class IGraphics *graphics;
    const class CDictionaryDescription *dictionaryDraw;
};

const char *CTypeDescriptionGL::ID_EVENT = "GRAPHICS3D";

//-----------------------------------------------------------------------

static void prv_integrity(struct SPrvDataPrivateTypeDescriptionGL *dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(dataPrivate->graphics);
    assert_no_null(dataPrivate->dictionaryDraw);
}

//-----------------------------------------------------------------------

static struct SPrvDataPrivateTypeDescriptionGL *prv_createTypeDescriptionGL(
                    class IWorld *world,
                    class IGraphics *graphics,
                    const class CDictionaryDescription *dictionaryDraw)
{
    struct SPrvDataPrivateTypeDescriptionGL *dataPrivate;

    dataPrivate = MALLOC(struct SPrvDataPrivateTypeDescriptionGL);

    dataPrivate->world = world;
    dataPrivate->graphics = graphics;
    dataPrivate->dictionaryDraw = dictionaryDraw;

    return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroyTypeDescriptionGL(struct SPrvDataPrivateTypeDescriptionGL **dataPrivate)
{
    FREE_T(dataPrivate, struct SPrvDataPrivateTypeDescriptionGL);
}

//-----------------------------------------------------------------------

CTypeDescriptionGL::CTypeDescriptionGL(class IWorld *world, class IGraphics *graphics, const class CDictionaryDescription *dictionaryDraw)
                    : CTypeDescrPhysics(CTypeDescriptionGL::ID_EVENT)
{
    m_dataPrivate = prv_createTypeDescriptionGL(world, graphics, dictionaryDraw);
}

//-----------------------------------------------------------------------

CTypeDescriptionGL::~CTypeDescriptionGL()
{
    prv_destroyTypeDescriptionGL(&m_dataPrivate);
}

//---------------------------------------------------------------

static bool prv_serchSymbol3d(
                        const class CDictionaryDescription *dictionaryDraw,
                        const char *idSymbol,
                        class IDescriptionGraphics3D **description3d)
{
    class IDescription *description;
    bool exist;

    assert_no_null(description3d);

    assert_no_null(dictionaryDraw);
    exist = dictionaryDraw->searchSymbol(idSymbol, &description);

    if (exist == true)
    {
        *description3d = dynamic_cast<IDescriptionGraphics3D *>(description);
        assert_no_null(*description3d);
    }

    return exist;
}

//---------------------------------------------------------------

void CTypeDescriptionGL::beginSymbol(const char *symbol, const class IDataSymbol *dataFigure)
{
    class IDescriptionGraphics3D *description3d;

    prv_integrity(m_dataPrivate);

    if (prv_serchSymbol3d(m_dataPrivate->dictionaryDraw, symbol, &description3d) == true)
        description3d->beginSymbol(m_dataPrivate->graphics, dataFigure);
}

//---------------------------------------------------------------

void CTypeDescriptionGL::drawSymbol(const char *symbol, const class IDataSymbol *dataFigure)
{
    class IDescriptionGraphics3D *description3d;

    prv_integrity(m_dataPrivate);

    if (prv_serchSymbol3d(m_dataPrivate->dictionaryDraw, symbol, &description3d) == true)
        description3d->drawSymbol(m_dataPrivate->graphics, dataFigure);
}

//---------------------------------------------------------------

void CTypeDescriptionGL::endSymbol(const char *symbol, const class IDataSymbol *dataFigure)
{
    class IDescriptionGraphics3D *description3d;

    prv_integrity(m_dataPrivate);

    if (prv_serchSymbol3d(m_dataPrivate->dictionaryDraw, symbol, &description3d) == true)
        description3d->endSymbol(m_dataPrivate->graphics, dataFigure);
}

//-----------------------------------------------------------------------

class IObjectDraw *CTypeDescriptionGL::move(const char *idElement, class IObjectDraw **figure)
{
    prv_integrity(m_dataPrivate);
    assert_no_null(m_dataPrivate->world);

    return m_dataPrivate->world->move(idElement, figure);
}
