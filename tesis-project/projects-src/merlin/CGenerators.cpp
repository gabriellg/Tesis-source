/* -----------------------------------------------------------------------------
 *  CGenerators.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CGenerators.inl"

#include "IGenerator.hpp"
#include "CGenUtil.inl"

#include "asrtbas.h"
#include "memory.h"
#include "marco.h"

#include "CArray.hpp"
#include "filetxt.hpp"
#include "CArrayString.hpp"
#include "CString.hpp"
#include "CFileSystem.hpp"
#include "CFileName.hpp"

struct prv_dataPrivateGenerators_t
{
    class CArray<IGenerator> *generators;
};

static const char *PRV_VERSION = "//VERSION_MERLIN:";
static const unsigned long NUM_HEADER = 2;

//-----------------------------------------------------------------------
//
static void prv_integrity(const struct prv_dataPrivateGenerators_t *dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(dataPrivate->generators);
}

//-----------------------------------------------------------------------
//
static struct prv_dataPrivateGenerators_t *prv_create(class CArray<IGenerator> **generators)
{
    struct prv_dataPrivateGenerators_t *dataPrivate;

    dataPrivate = MALLOC(struct prv_dataPrivateGenerators_t);

    dataPrivate->generators = ASSIGN_PP_NO_NULL(generators, class CArray<IGenerator>);

    prv_integrity(dataPrivate);

    return dataPrivate;
}

//-----------------------------------------------------------------------
//
static void prv_destroy(struct prv_dataPrivateGenerators_t **dataPrivate)
{
    assert_no_null(dataPrivate);
    prv_integrity(*dataPrivate);

    delete (*dataPrivate)->generators;

    FREE_T(dataPrivate, struct prv_dataPrivateGenerators_t);
}

//-----------------------------------------------------------------------
//
CGenerators::CGenerators()
{
    class CArray<IGenerator> *generators;

    generators = new CArray<IGenerator>;
    m_dataPrivate = prv_create(&generators);
}

//-----------------------------------------------------------------------
//
CGenerators::~CGenerators()
{
    prv_destroy(&m_dataPrivate);
}

//-----------------------------------------------------------------------
//
void CGenerators::add(class IGenerator *generator)
{
    prv_integrity(m_dataPrivate);
    assert_no_null(generator);

    m_dataPrivate->generators->add(generator);
}

//-----------------------------------------------------------------------
//
static bool prv_isFileSource(const struct IGenerator *generator, const char *fileSource)
{
    assert_no_null(generator);
    return generator->isFileSource(fileSource);
}

//-----------------------------------------------------------------------
//
bool CGenerators::isResourceSource(const char *fileSource) const
{
    prv_integrity(m_dataPrivate);
    return m_dataPrivate->generators->existOnlyOneElement<char>(fileSource, prv_isFileSource, NULL);
}

//-----------------------------------------------------------------------
//
static bool prv_isFileGenerated(const struct IGenerator *generator, const char *fileGenerated)
{
    assert_no_null(generator);
    return generator->isFileGenerated(fileGenerated);
}

//-----------------------------------------------------------------------
//
bool CGenerators::isResourceGenerated(const char *fileGenerated) const
{
    prv_integrity(m_dataPrivate);
    return m_dataPrivate->generators->existOnlyOneElement<char>(fileGenerated, prv_isFileGenerated, NULL);
}

//-----------------------------------------------------------------------
//
static bool prv_getVersion(const char *txtLineVersion, const unsigned short *version)
{
    int numFieldsRead;
    char *mascara;

    assert_no_null(version);

    mascara = CString::printf("%s%%d", PRV_VERSION);
    numFieldsRead = CString::scanf(txtLineVersion, mascara, version);
    CString::free(&mascara);

    if (numFieldsRead == 1)
        return true;
    else
        return false;
}

//-----------------------------------------------------------------------
//
static bool prv_mustBeGeneratedByVersion(const char *fileGenerated, unsigned short version)
{
    bool mustBeGenerated;
    class CArrayString *codeString;
    unsigned long numLines;

    numLines = NUM_HEADER;
    codeString = filetxt_readFileTxtNumLines(fileGenerated, numLines);

    if (codeString->size() >= 2)
    {
        const char *txtLineVersion;
        unsigned short versionRead;

        txtLineVersion = codeString->get(1);

        if (prv_getVersion(txtLineVersion, &versionRead) == true)
        {
            if (versionRead < version)
                mustBeGenerated = true;
            else
                mustBeGenerated = false;
        }
        else
            mustBeGenerated = true;
    }
    else
        mustBeGenerated = true;

    delete codeString;

    return mustBeGenerated;
}

//-----------------------------------------------------------------------
//
static bool prv_mustGenerated(const class IGenerator *generator, const char *fileSource)
{
    bool mustGenerated;
    char *fileGenerated;
    unsigned short version;

    assert_no_null(generator);

    fileGenerated = generator->nameFileGenerated(fileSource);
    version = generator->versionGeneration();

    if (CFileSystem::existFile(fileGenerated) == true)
    {
        mustGenerated = CFileSystem::firstIsMoreModern(fileSource, fileGenerated);
        if (mustGenerated == false)
            mustGenerated = prv_mustBeGeneratedByVersion(fileGenerated, version);
    }
    else
        mustGenerated = true;

    CString::free(&fileGenerated);

    return mustGenerated;
}

//-----------------------------------------------------------------------
//
bool CGenerators::mustGenerate(const char *fileSource) const
{
    unsigned long num;

    prv_integrity(m_dataPrivate);

    num = m_dataPrivate->generators->size();

    for (unsigned long i = 0; i < num; i++)
    {
        const class IGenerator *generator;

        generator = m_dataPrivate->generators->get(i);

        if (generator->isFileSource(fileSource) == true && prv_mustGenerated(generator, fileSource) == true)
            return true;
    }

    return false;
}

//-----------------------------------------------------------------------
//
static class CArrayString *prv_initializeCode(const char *fileGenerated, const char *fileSource, unsigned char version)
{
    class CArrayString *codeSource;
    char *textComentary, *nameSource, *nameGenerated;

    codeSource = new CArrayString;

    nameGenerated = CFileName::getFileName(fileGenerated);
    nameSource = CFileName::getFileName(fileSource);

    textComentary = CString::printf("%s: File generates with %s", nameGenerated, nameSource);
    CGenUtil::appendComentaryTextDestroying(codeSource, &textComentary);

    textComentary = CString::printf("%s%d", PRV_VERSION, version);
    codeSource->addDestroyingString(&textComentary);

    CString::free(&nameSource);
    CString::free(&nameGenerated);

    return codeSource;
}

//-----------------------------------------------------------------------
//
void CGenerators::generate(const char *fileSource, struct marco_t *marco) const
{
    const class IGenerator *generator;
    char *message, *fileGenerated;
    class CArrayString *codeSource;
    unsigned short version;

    prv_integrity(m_dataPrivate);

    generator = m_dataPrivate->generators->getOnlyOneElement<char>(fileSource, prv_isFileSource);

    message = CString::printf("Generating %s", fileSource);
    marco_lanza_mensaje(marco, MARCO_MENSAJE_INFORMACION, message);
    CString::free(&message);

    fileGenerated = generator->nameFileGenerated(fileSource);
    version = generator->versionGeneration();

    codeSource = prv_initializeCode(fileGenerated, fileSource, version);
    generator->createSource(fileSource, codeSource);

    filetxt_writeFileTxt(fileGenerated, codeSource);

    delete codeSource;
    CString::free(&fileGenerated);
}
