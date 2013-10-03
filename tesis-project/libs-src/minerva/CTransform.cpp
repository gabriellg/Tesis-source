#include "CTransform.hpp"

#include "asrtbas.h"
#include "memory.h"
#include "CArray.hpp"
#include "IDataSymbol.hpp"

#include "CTypeDescription.hpp"
#include "CString.hpp"

#include "asrtbas.h"
#include "memory.h"

struct SPrvDataPrivateTransform
{
    char *idSymbol;
    class IDataSymbol *dataTransform;
    class CArray<IObjectDraw> *sons;
};

//---------------------------------------------------------------

static void prv_integrity(struct SPrvDataPrivateTransform *dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(dataPrivate->idSymbol);
    assert_no_null(dataPrivate->dataTransform);
}

//-----------------------------------------------------------------------

static struct SPrvDataPrivateTransform *prv_createTransform(const char *idSymbol, class IDataSymbol **dataTransform,
        class CArray<IObjectDraw> **child)
{
    struct SPrvDataPrivateTransform *dataPrivate;

    dataPrivate = new SPrvDataPrivateTransform;

    dataPrivate->idSymbol = CString::copy(idSymbol);
    dataPrivate->dataTransform = ASSIGN_PP_NO_NULL(dataTransform, class IDataSymbol);
    dataPrivate->sons = ASSIGN_PP_NO_NULL(child, class CArray<IObjectDraw>);

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

CTransform::CTransform(const char *idSymbol, class IDataSymbol **dataTransform, class CArray<IObjectDraw> **sons)
{
    m_dataPrivate = prv_createTransform(idSymbol, dataTransform, sons);
}

//---------------------------------------------------------------

CTransform::CTransform(const char *idSymbol, class IDataSymbol **dataTransform, IObjectDraw **son)
{
    class CArray<IObjectDraw> *sons;

    assert_no_null(son);
    assert_no_null(*son);

    sons = new CArray<IObjectDraw>(1);
    sons->set(0, *son);
    *son = NULL;

    m_dataPrivate = prv_createTransform(idSymbol, dataTransform, &sons);
}


//-----------------------------------------------------------------------

CTransform::~CTransform()
{
    prv_destroyTransform(&m_dataPrivate);
}

//---------------------------------------------------------------

void CTransform::drawRepresentation(class CTypeDescription *evtDescription)
{
    prv_integrity(m_dataPrivate);
    assert_no_null(evtDescription);

    evtDescription->beginSymbol(m_dataPrivate->idSymbol, m_dataPrivate->dataTransform);
    evtDescription->drawSymbol(m_dataPrivate->idSymbol, m_dataPrivate->dataTransform);

    for (unsigned long i = 0, size = m_dataPrivate->sons->size(); i < size; i++)
    {
        class IObjectDraw *objectDraw;

        objectDraw = m_dataPrivate->sons->get(i);
        objectDraw->drawRepresentation(evtDescription);
    }

    evtDescription->endSymbol(m_dataPrivate->idSymbol, m_dataPrivate->dataTransform);
}
