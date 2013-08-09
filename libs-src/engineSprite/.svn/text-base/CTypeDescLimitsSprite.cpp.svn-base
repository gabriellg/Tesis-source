//CTypeDescLimitsSprite: Type description it is calculated sprites's size to look for collision.

#include "CTypeDescLimitsSprite.inl"

#include "CTransform2D.hpp"
#include "IDescriptionSprite.hpp"
#include "CWorldGeometry2DPixels.hpp"

#include "asrtbas.h"
#include "memory.h"
#include "CImg.hpp"
#include "CFigure.hpp"
#include "CString.hpp"
#include "CTransform.hpp"
#include "CArrayString.hpp"
#include "CDictionaryDescription.hpp"
#include "IDataSymbol.hpp"

const char *CTypeDescLimitsSprite::ID_EVENT = "CTypeDescLimitsSprite";

static const char *prv_ROOT = "root";
static const char *PRV_DATA_ID = "DATA_ID";

class CDataID : public IDataSymbol
{
    public:

        CDataID(const char *idElement);
        virtual ~CDataID();

        void beginId(class CArrayString *stackFigures, char **idElementCurrent) const;
        void endId(class CArrayString *stackFigures, char **idElementCurrent) const;

    private:

        char *m_idElement;
};

struct SPrvDataPrivateTypeDescLimits
{
	class CWorldGeometry2DPixels *worldGeometry2D;

	char *idElementCurrent;
    class CArrayString *stackFigures;

    const class CDictionaryDescription *dictionaryDraw;
};

//-----------------------------------------------------------------------

CDataID::CDataID(const char *idElement)
{
    m_idElement = CString::copy(idElement);
}

//-----------------------------------------------------------------------

CDataID::~CDataID()
{
    CString::free(&m_idElement);
}

//-----------------------------------------------------------------------

void CDataID::beginId(class CArrayString *stackFigures, char **idElementCurrent) const
{
    stackFigures->addDestroyingString(idElementCurrent);
    *idElementCurrent = CString::copy(m_idElement);
}

//-----------------------------------------------------------------------

void CDataID::endId(class CArrayString *stackFigures, char **idElementCurrent) const
{
    const char *idLast;

    assert(CString::equal(m_idElement, *idElementCurrent) == true);

    idLast = stackFigures->getLast();
    CString::free(idElementCurrent);

    *idElementCurrent = CString::copy(idLast);
    stackFigures->eraseLast();
}

//-----------------------------------------------------------------------

static void prv_integrity(struct SPrvDataPrivateTypeDescLimits *dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(dataPrivate->idElementCurrent);
    assert_no_null(dataPrivate->stackFigures);
    assert_no_null(dataPrivate->worldGeometry2D);
}

//-----------------------------------------------------------------------

static struct SPrvDataPrivateTypeDescLimits *prv_createTypeDescLimits(
					class CWorldGeometry2DPixels *worldGeometry2D,
					const class CDictionaryDescription *dictionaryDraw,
					const char *idElementCurrent,
					class CArrayString **stackFigures)
{
	struct SPrvDataPrivateTypeDescLimits *dataPrivate;

	dataPrivate = MALLOC(struct SPrvDataPrivateTypeDescLimits);

	dataPrivate->idElementCurrent = CString::copy(idElementCurrent);
	dataPrivate->stackFigures = ASSIGN_PP_NO_NULL(stackFigures, class CArrayString);
	dataPrivate->worldGeometry2D = worldGeometry2D;
	dataPrivate->dictionaryDraw = dictionaryDraw;

	prv_integrity(dataPrivate);

	return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroyTypeDescLimits(struct SPrvDataPrivateTypeDescLimits **dataPrivate)
{
    assert_no_null(dataPrivate);
    prv_integrity(*dataPrivate);
    assert((*dataPrivate)->stackFigures->size() == 0);
    assert(CString::equal((*dataPrivate)->idElementCurrent, prv_ROOT) == true);

    delete (*dataPrivate)->stackFigures;

	FREE_T(dataPrivate, struct SPrvDataPrivateTypeDescLimits);
}

//-----------------------------------------------------------------------

CTypeDescLimitsSprite::CTypeDescLimitsSprite(
                        class CWorldGeometry2DPixels *worldGeometry2D,
                        const class CDictionaryDescription *dictionaryDraw)
						: CTypeDescrPhysics(CTypeDescLimitsSprite::ID_EVENT)
{
    class CArrayString *stackFigures;

    stackFigures = new CArrayString();
	m_dataPrivate = prv_createTypeDescLimits(worldGeometry2D, dictionaryDraw, prv_ROOT, &stackFigures);
}

//-----------------------------------------------------------------------

CTypeDescLimitsSprite::~CTypeDescLimitsSprite()
{
    prv_destroyTypeDescLimits(&m_dataPrivate);
}

//-----------------------------------------------------------------------

class CAgent *CTypeDescLimitsSprite::move(const char *idElement, class CAgent **actorToTransform)
{
    class IDataSymbol *dataId;

    dataId = new CDataID(idElement);
    return new CTransform(PRV_DATA_ID, &dataId, actorToTransform);
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

void CTypeDescLimitsSprite::beginSymbol(const char *symbol, const class IDataSymbol *dataFigure)
{
    prv_integrity(m_dataPrivate);

    if (CString::equal(symbol, PRV_DATA_ID) == true)
    {
        const class CDataID *dataId;

        dataId = dynamic_cast<const class CDataID *>(dataFigure);
        assert_no_null(dataId);
        dataId->beginId(m_dataPrivate->stackFigures, &m_dataPrivate->idElementCurrent);
    }
}

//-----------------------------------------------------------------------

void CTypeDescLimitsSprite::drawSymbol(const char *symbol, const class IDataSymbol *dataFigure)
{
    class IDescriptionSprite *descriptionSprite;

    prv_integrity(m_dataPrivate);

    if (prv_existDescriptionSprite(
                            m_dataPrivate->dictionaryDraw,
                            symbol,
                            &descriptionSprite) == true)
    {
        unsigned long width, heigth;

        descriptionSprite->size(dataFigure, &width, &heigth);
        m_dataPrivate->worldGeometry2D->setSizeFigure(m_dataPrivate->idElementCurrent, width, heigth);
    }
}

//-----------------------------------------------------------------------

void CTypeDescLimitsSprite::endSymbol(const char *symbol, const class IDataSymbol *dataFigure)
{
    prv_integrity(m_dataPrivate);

    if (CString::equal(symbol, PRV_DATA_ID) == true)
    {
        const class CDataID *dataId;

        dataId = dynamic_cast<const class CDataID *>(dataFigure);
        assert_no_null(dataId);
        dataId->endId(m_dataPrivate->stackFigures, &m_dataPrivate->idElementCurrent);
    }
}
