/* -----------------------------------------------------------------------------
 *  CDictionaryDescription.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CDictionaryDescription.hpp"
#include "IDescription.hpp"

#include "asrtbas.h"
#include "memory.h"

#include "CString.hpp"
#include "CArray.hpp"

struct PrvCDescriptionSymbol
{
    PrvCDescriptionSymbol(const char *symbol, class IDescription **descriptionSymbol);
    ~PrvCDescriptionSymbol();

    char *m_symbol;
    class IDescription *m_descriptionSymbol;
};

struct prv_dataPrivateDictioryDescription_t
{
    class CArray<PrvCDescriptionSymbol> *symbols;
};

//-----------------------------------------------------------------------

PrvCDescriptionSymbol::PrvCDescriptionSymbol(const char *symbol, class IDescription **descriptionSymbol)
{
    m_symbol = CString::copy(symbol);
    m_descriptionSymbol = ASSIGN_PP_NO_NULL(descriptionSymbol, class IDescription);
}

//-----------------------------------------------------------------------

PrvCDescriptionSymbol::~PrvCDescriptionSymbol()
{
    CString::free(&m_symbol);
    delete m_descriptionSymbol;
}

//-----------------------------------------------------------------------

static bool prv_isSymbol(const struct PrvCDescriptionSymbol *descriptionSymbol, const char *symbol)
{
    assert_no_null(descriptionSymbol);
    return CString::equal(descriptionSymbol->m_symbol, symbol);
}

//-----------------------------------------------------------------------

static void prv_integrity(const struct prv_dataPrivateDictioryDescription_t *dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(dataPrivate->symbols);
}

//-----------------------------------------------------------------------

static struct prv_dataPrivateDictioryDescription_t *prv_create(class CArray<PrvCDescriptionSymbol> **symbols)
{
    struct prv_dataPrivateDictioryDescription_t *dataPrivate;

    dataPrivate = MALLOC(struct prv_dataPrivateDictioryDescription_t);

    dataPrivate->symbols = ASSIGN_PP_NO_NULL(symbols, class CArray<PrvCDescriptionSymbol>);

    prv_integrity(dataPrivate);

    return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroy(struct prv_dataPrivateDictioryDescription_t **dataPrivate)
{
    assert_no_null(dataPrivate);
    prv_integrity(*dataPrivate);

    delete (*dataPrivate)->symbols;

    FREE_T(dataPrivate, struct prv_dataPrivateDictioryDescription_t);
}

//-----------------------------------------------------------------------

CDictionaryDescription::CDictionaryDescription()
{
    class CArray<PrvCDescriptionSymbol> *symbols;

    symbols = new CArray<PrvCDescriptionSymbol>;
    m_dataPrivate = prv_create(&symbols);
}

//-----------------------------------------------------------------------

CDictionaryDescription::~CDictionaryDescription()
{
    prv_destroy(&m_dataPrivate);
}

//-----------------------------------------------------------------------

void CDictionaryDescription::addDescription(const char *symbol, class IDescription **description)
{
    bool exist;
    struct PrvCDescriptionSymbol *descriptionSymbol;

    prv_integrity(m_dataPrivate);

    exist = m_dataPrivate->symbols->existOnlyOneElement<char>(symbol, prv_isSymbol, NULL);
    assert(exist == false);

    descriptionSymbol = new PrvCDescriptionSymbol(symbol, description);
    m_dataPrivate->symbols->add(descriptionSymbol);
}

//-----------------------------------------------------------------------

bool CDictionaryDescription::searchSymbol(const char *symbol, class IDescription **description) const
{
    unsigned long indPos;
    bool existSymbol;

    prv_integrity(m_dataPrivate);
    assert_no_null(description);

    existSymbol = m_dataPrivate->symbols->existOnlyOneElement<char>(symbol, prv_isSymbol, &indPos);

    if (existSymbol == true)
    {
        const struct PrvCDescriptionSymbol *descriptionSymbol;

        descriptionSymbol = m_dataPrivate->symbols->get(indPos);
        assert_no_null(descriptionSymbol);

        *description = descriptionSymbol->m_descriptionSymbol;
    }

    return existSymbol;
}
