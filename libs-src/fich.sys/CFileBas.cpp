//CFileBas.cpp:

#include "CFileBas.inl"

#include "stdio.h"
#include "stdlib.h"
#include "asrtbas.h"

#include "CArray.hpp"
#include "CString.hpp"

struct PrvFileOpened
{
    PrvFileOpened(const char *nombreFich, FILE *file);
    ~PrvFileOpened(void);

    char *m_nameFile;
    FILE *m_file;
};

static class CArray<PrvFileOpened> *s_filesOpened = NULL;

//------------------------------------------------------------------------------
//
PrvFileOpened::PrvFileOpened(const char *nameFile, FILE *file)
{
    m_nameFile = CString::copy(nameFile);
    m_file = file;
}

//------------------------------------------------------------------------------
//
PrvFileOpened::~PrvFileOpened(void)
{
    CString::free(&m_nameFile);
}

//------------------------------------------------------------------------------
//
static bool prv_isNameFile(const struct PrvFileOpened *fileOpened, const char *nameFile)
{
    assert_no_null(fileOpened);
    return CString::equalWithoutMatchCase(fileOpened->m_nameFile, nameFile);
}

//------------------------------------------------------------------------------
//
static bool prv_isFILE(const struct PrvFileOpened *fileOpened, const FILE *file)
{
    assert_no_null(fileOpened);

    if (fileOpened->m_file == file)
        return true;
    else
        return false;
}

//------------------------------------------------------------------------------
//
static void prv_registerFile(class CArray<PrvFileOpened> *filesOpened, const char *nameFile, FILE *file)
{
    bool existFileOpened;
    PrvFileOpened *fileOpened;

    assert_no_null(filesOpened);

    existFileOpened = filesOpened->existOnlyOneElement<char>(nameFile, prv_isNameFile, NULL);
    assert_with_message(existFileOpened == false, "File is opened");

    fileOpened = new PrvFileOpened(nameFile, file);
    filesOpened->add(fileOpened);
}

//------------------------------------------------------------------------------
//
static void prv_unregisterFile(class CArray<PrvFileOpened> *filesOpened, FILE *file)
{
    bool existFileOpened;
    PrvFileOpened *fileOpened;
    unsigned long ind;

    assert_no_null(filesOpened);

    existFileOpened = filesOpened->existOnlyOneElement<FILE>(file, prv_isFILE, &ind);
    assert_with_message(existFileOpened == true, "File is not opened");

    fileOpened = filesOpened->get(ind);
    assert_no_null(fileOpened);
    filesOpened->erase(ind);
}

//------------------------------------------------------------------------------
//
static void prv_deleteArrayOpened(void)
{
    unsigned long num;

    assert_no_null(s_filesOpened);

    num = s_filesOpened->size();
    assert_with_message(num == 0, "Files opened");

    delete s_filesOpened;
}

//------------------------------------------------------------------------------
//
void *CFileBas::openBin(const char *nameFile, enum TypeOpen_t typeOpen)
{
    FILE *file;
    const char *modeFile;

    if (s_filesOpened == NULL)
    {
        s_filesOpened = new CArray<PrvFileOpened>;
        atexit(prv_deleteArrayOpened);
    }

    switch(typeOpen)
    {
        case CFileBas::READ:

            modeFile = "rb";
            break;

        case CFileBas::WRITE:

            modeFile = "wb";
            break;

        default_error();
    }

    file = fopen(nameFile, modeFile);
    assert_no_null(file);

    prv_registerFile(s_filesOpened, nameFile, file);

    return file;
}

//------------------------------------------------------------------------------
//
void CFileBas::close(void **file)
{
    FILE *fileSO;

    assert_no_null(file);
    assert_no_null(*file);

    fileSO = (FILE *)*file;
    prv_unregisterFile(s_filesOpened, fileSO);
    fclose(fileSO);

    *file = NULL;
}
