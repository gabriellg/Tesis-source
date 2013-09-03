// Figura

#include "CFigure.hpp"

#include "CTypeDescription.hpp"
#include "IDataSymbol.hpp"

#include "asrtbas.h"
#include "memory.h"

#include "CString.hpp"

#include "asrtbas.h"
#include "memory.h"

struct SPrvDataPrivateFigure
{
    char *symbol;
    class IDataSymbol *dataFigure;
};

//---------------------------------------------------------------

static void prv_integrity(const struct SPrvDataPrivateFigure *dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(dataPrivate->symbol);
}

//-----------------------------------------------------------------------

static struct SPrvDataPrivateFigure *prv_createFigure(const char *symbol, class IDataSymbol **dataFigure)
{
    struct SPrvDataPrivateFigure *dataPrivate;
    
    dataPrivate = new SPrvDataPrivateFigure;

    dataPrivate->symbol = CString::copy(symbol);
    dataPrivate->dataFigure = ASSIGN_PP(dataFigure, class IDataSymbol);
    
    prv_integrity(dataPrivate);

    return dataPrivate;
} 

//-----------------------------------------------------------------------

static void prv_destroyFigure(struct SPrvDataPrivateFigure **dataPrivate)
{
    assert_no_null(dataPrivate);
    prv_integrity(*dataPrivate);
    
    CString::free(&(*dataPrivate)->symbol);

    if ((*dataPrivate)->dataFigure != NULL)
        delete (*dataPrivate)->dataFigure;
    
    DELETE_OBJECT(dataPrivate, struct SPrvDataPrivateFigure);
} 

//---------------------------------------------------------------

CFigure::CFigure(const char *symbol)
{
    class IDataSymbol *dataFigure;

    dataFigure = NULL;
	m_dataPrivate = prv_createFigure(symbol, &dataFigure);
}

//---------------------------------------------------------------

CFigure::CFigure(const char *symbol, class IDataSymbol **dataFigure)
{
    m_dataPrivate = prv_createFigure(symbol, dataFigure);
}

//---------------------------------------------------------------

CFigure::~CFigure()
{
    prv_destroyFigure(&m_dataPrivate);
}

//---------------------------------------------------------------

class CAgent *CFigure::evolution(class CCollectionEventsSystem *allEvents)
{
    return this;
}

//---------------------------------------------------------------

class CAgent *CFigure::representation(class CTypeDescription *evtDescription)
{
	return this;
}

//---------------------------------------------------------------

void CFigure::beginRepresentation(class CTypeDescription *evtDescription)
{
    prv_integrity(m_dataPrivate);
    evtDescription->beginSymbol(m_dataPrivate->symbol, m_dataPrivate->dataFigure);
}

//---------------------------------------------------------------

void CFigure::drawRepresentation(class CTypeDescription *evtDescription)
{
    assert_no_null(m_dataPrivate);
    assert_no_null(evtDescription);

    evtDescription->drawSymbol(m_dataPrivate->symbol, m_dataPrivate->dataFigure);
}

//---------------------------------------------------------------

void CFigure::endRepresentation(class CTypeDescription *evtDescription)
{
    prv_integrity(m_dataPrivate);
    evtDescription->endSymbol(m_dataPrivate->symbol, m_dataPrivate->dataFigure);
}

