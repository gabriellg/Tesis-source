#include "CTransform.hpp"

#include "asrtbas.h"
#include "memory.h"
#include "CArrayRef.hpp"
#include "IDataSymbol.hpp"

#include "CTypeDescription.hpp"
#include "CString.hpp"

#include "asrtbas.h"
#include "memory.h"

struct SPrvDataPrivateTransform
{
    char *idSymbol;
    class IDataSymbol *dataTransform;
};

//---------------------------------------------------------------

static void prv_integrity(struct SPrvDataPrivateTransform *dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(dataPrivate->idSymbol);
    assert_no_null(dataPrivate->dataTransform);
}

//-----------------------------------------------------------------------

static struct SPrvDataPrivateTransform *prv_createTransform(const char *idSymbol, class IDataSymbol **dataTransform)
{
    struct SPrvDataPrivateTransform *dataPrivate;

    dataPrivate = new SPrvDataPrivateTransform;

    dataPrivate->idSymbol = CString::copy(idSymbol);
    dataPrivate->dataTransform = ASSIGN_PP_NO_NULL(dataTransform, class IDataSymbol);

    return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroyTransform(struct SPrvDataPrivateTransform **dataPrivate)
{
    assert_no_null(dataPrivate);
    prv_integrity(*dataPrivate);

    CString::free(&(*dataPrivate)->idSymbol);

    DELETE_OBJECT(&(*dataPrivate)->dataTransform, class IDataSymbol);

    DELETE_OBJECT(dataPrivate, SPrvDataPrivateTransform);
}

//-----------------------------------------------------------------------

CTransform::CTransform(const char *idSymbol, class IDataSymbol **dataTransform, class CAgent **actorToTransform)
{
    m_dataPrivate = prv_createTransform(idSymbol, dataTransform);
    appendChild(actorToTransform);
}

//-----------------------------------------------------------------------

CTransform::~CTransform()
{
    prv_destroyTransform(&m_dataPrivate);
}

//---------------------------------------------------------------

class CAgent *CTransform::evolution(class CCollectionEventsSystem *allEvents)
{
    return this;
}

//---------------------------------------------------------------

class CAgent *CTransform::representation(class CTypeDescription *evtDescription)
{
    return this;
}

//---------------------------------------------------------------

void CTransform::beginRepresentation(class CTypeDescription *evtDescription)
{
    prv_integrity(m_dataPrivate);
    assert_no_null(evtDescription);

    evtDescription->beginSymbol(m_dataPrivate->idSymbol, m_dataPrivate->dataTransform);
}

//---------------------------------------------------------------

void CTransform::drawRepresentation(class CTypeDescription *evtDescription)
{
    prv_integrity(m_dataPrivate);
    assert_no_null(evtDescription);

    evtDescription->drawSymbol(m_dataPrivate->idSymbol, m_dataPrivate->dataTransform);
}

//---------------------------------------------------------------

void CTransform::endRepresentation(class CTypeDescription *evtDescription)
{
    prv_integrity(m_dataPrivate);
    assert_no_null(evtDescription);

    evtDescription->endSymbol(m_dataPrivate->idSymbol, m_dataPrivate->dataTransform);
}
