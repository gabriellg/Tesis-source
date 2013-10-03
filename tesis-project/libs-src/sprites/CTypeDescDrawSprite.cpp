//CTypeDescDrawSprite: Type description it is drawed on a bitmap.

#include "CTypeDescDrawSprite.inl"
#include "IDescriptionSprite.hpp"

#include "CTransform2D.hpp"
#include "IWorld.hpp"

#include "asrtbas.h"
#include "memory.h"

#include "IGraphics.hpp"
#include "CString.hpp"
#include "CGraphicsSprite.hpp"
#include "CFigure.hpp"
#include "CArraysTypes.hpp"
#include "CDictionaryDescription.hpp"

const char *CTypeDescDrawSprite::ID_EVENT = "CTypeDescDrawSprite";

struct SPrvDataPrivateTypeDescDrawSprite
{
	class CGraphicsSprite *graphicsSprite;
	class IWorld *world;

	const class CDictionaryDescription *dictionaryDraw;
};

//-----------------------------------------------------------------------

static void prv_integrity(const struct SPrvDataPrivateTypeDescDrawSprite *dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(dataPrivate->graphicsSprite);
    assert_no_null(dataPrivate->dictionaryDraw);
}

//-----------------------------------------------------------------------

static struct SPrvDataPrivateTypeDescDrawSprite *prv_createTypeDescDrawSprite(
                        class CGraphicsSprite *graphicsSprite,
						class IWorld *world,
						const class CDictionaryDescription *dictionaryDraw)
{
	struct SPrvDataPrivateTypeDescDrawSprite *dataPrivate;
	
	dataPrivate = MALLOC(struct SPrvDataPrivateTypeDescDrawSprite);
	
	dataPrivate->graphicsSprite = graphicsSprite;
	
	dataPrivate->world = world;

	dataPrivate->dictionaryDraw = dictionaryDraw;

	return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroyTypeDescDrawSprite(struct SPrvDataPrivateTypeDescDrawSprite **dataPrivate)
{
    DELETE_OBJECT(dataPrivate, struct SPrvDataPrivateTypeDescDrawSprite);
}

//-----------------------------------------------------------------------

CTypeDescDrawSprite::CTypeDescDrawSprite(
                        class CGraphicsSprite *graphicsSprite,
                        class IWorld *world,
                        const class CDictionaryDescription *dictionaryDraw) :
                    CTypeDescrPhysics(CTypeDescDrawSprite::ID_EVENT)
{
	m_dataPrivate = prv_createTypeDescDrawSprite(graphicsSprite, world, dictionaryDraw);
}

//-----------------------------------------------------------------------

CTypeDescDrawSprite::~CTypeDescDrawSprite()
{
	prv_destroyTypeDescDrawSprite(&m_dataPrivate);
}

//-----------------------------------------------------------------------

static bool prv_existDescriptionSprite(
                        const class CDictionaryDescription *dictionaryDraw,
                        const char *symbol,
                        class IDescriptionSprite **descriptionSprite)
{
    class IDescription *description;
    bool exist;

    assert_no_null(dictionaryDraw);
    assert_no_null(descriptionSprite);

    exist = dictionaryDraw->searchSymbol(symbol, &description);
    if (exist == true)
    {
        *descriptionSprite = dynamic_cast<IDescriptionSprite *>(description);
        assert_no_null(*descriptionSprite);
    }

    return exist;
}

//-----------------------------------------------------------------------

void CTypeDescDrawSprite::beginSymbol(const char *symbol, const class IDataSymbol *dataFigure)
{
    class IDescriptionSprite *descriptionSprite;

    prv_integrity(m_dataPrivate);

    if (prv_existDescriptionSprite(
                            m_dataPrivate->dictionaryDraw, symbol,
                            &descriptionSprite) == true)
    {
        descriptionSprite->beginSymbol(m_dataPrivate->graphicsSprite, dataFigure);
    }
}

//-----------------------------------------------------------------------

void CTypeDescDrawSprite::drawSymbol(const char *symbol, const class IDataSymbol *dataFigure)
{
    class IDescriptionSprite *descriptionSprite;

    prv_integrity(m_dataPrivate);

    if (prv_existDescriptionSprite(
                            m_dataPrivate->dictionaryDraw, symbol,
                            &descriptionSprite) == true)
    {
        descriptionSprite->drawSymbol(m_dataPrivate->graphicsSprite, dataFigure);
    }
}

//-----------------------------------------------------------------------

void CTypeDescDrawSprite::endSymbol(const char *symbol, const class IDataSymbol *dataFigure)
{
    class IDescriptionSprite *descriptionSprite;

    prv_integrity(m_dataPrivate);

    if (prv_existDescriptionSprite(
                            m_dataPrivate->dictionaryDraw, symbol,
                            &descriptionSprite) == true)
    {
        descriptionSprite->endSymbol(m_dataPrivate->graphicsSprite, dataFigure);
    }
}

//-----------------------------------------------------------------------

class IObjectDraw *CTypeDescDrawSprite::move(const char *idElement, class IObjectDraw **objMove)
{
    assert_no_null(m_dataPrivate);
    assert_no_null(objMove);

    if (m_dataPrivate->world != NULL)
        return m_dataPrivate->world->move(idElement, objMove);
    else
        return *objMove;
}
