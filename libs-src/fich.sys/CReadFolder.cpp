/* -----------------------------------------------------------------------------
 *  CReadFolder.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CReadFolder.hpp"

#include "CString.hpp"
#include "CArrayString.hpp"
#include "CFileName.hpp"
#include "CFileSystem.hpp"

#include "unistd.h"
#include "sys/stat.h"
#include "stdio.h"
#include "dirent.h"

#include "asrtbas.h"
#include "memory.h"

struct prv_dataPrivateReadFolder_t
{
    char *pathFolder;

    class CArrayString *files;
    class CArrayString *subFolders;
};

//-----------------------------------------------------------------------

static void prv_integrity(const struct prv_dataPrivateReadFolder_t *dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(dataPrivate->files);
    assert_no_null(dataPrivate->subFolders);
}

//-----------------------------------------------------------------------

static struct prv_dataPrivateReadFolder_t *prv_create(
                        const char *pathFolder,
                        class CArrayString **files,
                        class CArrayString **subFolders)
{
    struct prv_dataPrivateReadFolder_t *dataPrivate;

    dataPrivate = MALLOC(struct prv_dataPrivateReadFolder_t);

    dataPrivate->pathFolder = CString::copy(pathFolder);
    dataPrivate->files = ASSIGN_PP_NO_NULL(files, class CArrayString);
    dataPrivate->subFolders = ASSIGN_PP_NO_NULL(subFolders, class CArrayString);

    prv_integrity(dataPrivate);

    return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroy(struct prv_dataPrivateReadFolder_t **dataPrivate)
{
    assert_no_null(dataPrivate);
    prv_integrity(*dataPrivate);

    CString::free(&(*dataPrivate)->pathFolder);

    delete (*dataPrivate)->files;
    delete (*dataPrivate)->subFolders;

    FREE_T(dataPrivate, struct prv_dataPrivateReadFolder_t);
}

//-----------------------------------------------------------------------

static void prv_getFilesAndSubFolders(
                            const char *pathFolder,
                            class CArrayString **files,
                            class CArrayString **subFolders)
{
    class CArrayString *files_loc;
    class CArrayString *subFolders_loc;
    DIR *folder;
    bool hasEntry;
    int ret;

    assert_no_null(pathFolder);
    assert_no_null(files);
    assert_no_null(subFolders);

    files_loc = new CArrayString();
    subFolders_loc = new CArrayString();

    folder = opendir(pathFolder);
    assert_no_null(folder);

    do
    {
        struct dirent *entry;

        entry = readdir(folder);
        if (entry != NULL)
        {
            char *filename;

            filename = CFileName::appenToFolder(pathFolder, entry->d_name);

            if (CFileSystem::isFolder(filename) == false)
                files_loc->add(entry->d_name);
            else
            {
                if (CString::equal(entry->d_name, ".") == false &&
                        CString::equal(entry->d_name, "..") == false)
                    subFolders_loc->add(entry->d_name);
            }

            CString::free(&filename);

            hasEntry = false;
        }
        else
            hasEntry = true;
    }
    while(hasEntry == false);

    ret = closedir(folder);
    assert(ret == 0);

    *files = files_loc;
    *subFolders = subFolders_loc;
}

//-----------------------------------------------------------------------

CReadFolder::CReadFolder(const char *pathFolder)
{
    class CArrayString *files;
    class CArrayString *subFolders;

    prv_getFilesAndSubFolders(pathFolder, &files, &subFolders);

    m_dataPrivate = prv_create(pathFolder, &files, &subFolders);
}

//-----------------------------------------------------------------------

CReadFolder::~CReadFolder()
{
    prv_destroy(&m_dataPrivate);
}

//-----------------------------------------------------------------------

bool CReadFolder::hasFiles() const
{
    unsigned long num_ficheros;

    prv_integrity(m_dataPrivate);

    num_ficheros = m_dataPrivate->files->size();

    if (num_ficheros > 0)
        return true;
    else
        return false;
}

//-----------------------------------------------------------------------

static class CArrayString *prv_addPathFolder(const char *pathFolder, const class CArrayString *entries)
{
    class CArrayString *pathWithFolder;
    unsigned long numEntries;

    numEntries = entries->size();
    pathWithFolder = new CArrayString(numEntries);

    for (unsigned long i = 0; i < numEntries; i++)
    {
        const char *nameEntry;
        char *entryWithFolder;

        nameEntry = entries->get(i);
        entryWithFolder = CFileName::appenToFolder(pathFolder, nameEntry);
        pathWithFolder->setDestroyingString(i, &entryWithFolder);
    }

    return pathWithFolder;
}

//-----------------------------------------------------------------------

class CArrayString *CReadFolder::files(void) const
{
    prv_integrity(m_dataPrivate);
    return prv_addPathFolder(m_dataPrivate->pathFolder, m_dataPrivate->files);
}

//-----------------------------------------------------------------------

class CArrayString *CReadFolder::subFolder(void) const
{
    prv_integrity(m_dataPrivate);
    return prv_addPathFolder(m_dataPrivate->pathFolder, m_dataPrivate->subFolders);
}
