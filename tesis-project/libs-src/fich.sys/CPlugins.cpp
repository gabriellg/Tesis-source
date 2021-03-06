/* -----------------------------------------------------------------------------
 *  CPlugins.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CPlugins.hpp"

#include "asrtbas.h"
#include "memory.h"
#include "dlfcn.h"

#include "CArray.hpp"
#include "CString.hpp"

struct PrvLibrary
{
    PrvLibrary(const char *nameLibrary, void **handleLibrary);
    ~PrvLibrary(void);

    char *m_nameLibrary;
    void *m_handleLibrary;
};

struct prv_dataPrivatePlugins_t
{
    class CArray<PrvLibrary> *librariesRegister;
};

//-----------------------------------------------------------------------

PrvLibrary::PrvLibrary(const char *nameLibrary, void **handleLibrary)
{
    m_nameLibrary = CString::copy(nameLibrary);
    m_handleLibrary = ASSIGN_PP_NO_NULL(handleLibrary, void);
}

//-----------------------------------------------------------------------

PrvLibrary::~PrvLibrary(void)
{
    assert(m_handleLibrary == NULL);
    CString::free(&m_nameLibrary);
}

//-----------------------------------------------------------------------

static bool prv_isLibrary(const struct PrvLibrary *library, const char *nameLibrary)
{
    assert_no_null(library);
    return CString::equalWithoutMatchCase(library->m_nameLibrary, nameLibrary);
}

//-----------------------------------------------------------------------

static bool prv_searchLibrary(class CArray<PrvLibrary> *librariesRegister, const char *nameLibrary, struct PrvLibrary **library)
{
    bool existLibrary;
    unsigned long indLibrary;
    struct PrvLibrary *library_loc;

    assert_no_null(librariesRegister);
    assert_no_null(library);

    existLibrary = librariesRegister->existOnlyOneElement<char>(nameLibrary, prv_isLibrary, &indLibrary);

    if (existLibrary == true)
        library_loc = librariesRegister->get(indLibrary);
    else
    {
        void *handleLibrary;

        handleLibrary = dlopen(nameLibrary, RTLD_LAZY);

        if (handleLibrary != NULL)
        {
            library_loc = new PrvLibrary(nameLibrary, &handleLibrary);
            librariesRegister->add(library_loc);
            existLibrary = true;
        }
        else
        {
            library_loc = NULL;
            existLibrary = false;
        }
    }

    if (existLibrary == true)
        *library = library_loc;

    return existLibrary;
}

//-----------------------------------------------------------------------

static void prv_closeAllLibraries(class CArray<PrvLibrary> *librariesRegister)
{
    unsigned long numLibraries;

    assert_no_null(librariesRegister);

    numLibraries = librariesRegister->size();

    for (unsigned long i = 0; i < numLibraries; i++)
    {
        struct PrvLibrary *library;
        int res;

        library = librariesRegister->get(i);
        assert_no_null(library);

        res = dlclose(library->m_handleLibrary);
        assert(res == 0);
        library->m_handleLibrary = NULL;
    }
}

//-----------------------------------------------------------------------

static void prv_integrity(const struct prv_dataPrivatePlugins_t *dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(dataPrivate->librariesRegister);
}

//-----------------------------------------------------------------------

static struct prv_dataPrivatePlugins_t *prv_create(class CArray<PrvLibrary> **librariesRegister)
{
    struct prv_dataPrivatePlugins_t *dataPrivate;

    dataPrivate = MALLOC(struct prv_dataPrivatePlugins_t);

    dataPrivate->librariesRegister = ASSIGN_PP_NO_NULL(librariesRegister, class CArray<PrvLibrary>);

    return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroy(struct prv_dataPrivatePlugins_t **dataPrivate)
{
    assert_no_null(dataPrivate);
    prv_integrity(*dataPrivate);

    delete (*dataPrivate)->librariesRegister;

    FREE_T(dataPrivate, struct prv_dataPrivatePlugins_t);
}

//-----------------------------------------------------------------------

CPlugins::CPlugins(void)
{
    class CArray<PrvLibrary> *librariesRegister;

    librariesRegister = new CArray<PrvLibrary>;

    m_dataPrivate = prv_create(&librariesRegister);
}

//-----------------------------------------------------------------------

CPlugins::~CPlugins()
{
    prv_closeAllLibraries(m_dataPrivate->librariesRegister);
    prv_destroy(&m_dataPrivate);
}

//-----------------------------------------------------------------------

bool CPlugins::readFunction(const char *nameLibrary, const char *symbol, void **addressFunction, char **messageError)
{
    bool existSymbol;
    struct PrvLibrary *library;
    void *addressFunction_loc;

    prv_integrity(m_dataPrivate);

    existSymbol = prv_searchLibrary(m_dataPrivate->librariesRegister, nameLibrary, &library);

    if (existSymbol == true)
    {
        addressFunction_loc = dlsym(library->m_handleLibrary, symbol);

        if (addressFunction_loc != NULL)
            existSymbol = true;
        else
        {
            existSymbol = false;

            if (messageError != NULL)
                *messageError = CString::printf("No exist symbol %s in Library %s", symbol, nameLibrary);
        }
    }
    else
    {
        addressFunction_loc = NULL;

        if (messageError != NULL)
            *messageError = CString::printf("No exist library %s", nameLibrary);
    }

    *addressFunction = addressFunction_loc;

    return existSymbol;
}
