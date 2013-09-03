/* -----------------------------------------------------------------------------
 *  CFileSystem.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CFileSystem.hpp"

#include "asrtbas.h"
#include "stdio.h"
#include "unistd.h"
#include "sys/stat.h"

//-----------------------------------------------------------------------
//
bool CFileSystem::existFile(const char *path)
{
    FILE *f;

    assert_no_null(path);

    f = fopen(path, "rb");

    if (f != NULL)
    {
        fclose(f);
        return true;
    }
    else
        return false;
}

//-----------------------------------------------------------------------
//
bool CFileSystem::existFolder(const char *pathFolder)
{
    if (access(pathFolder, F_OK) == 0)
        return true;
    else
        return false;
}

//-----------------------------------------------------------------------

void CFileSystem::rename(const char *filename, const char *newFilename)
{
    int resp;

    assert_no_null(filename);
    assert_no_null(newFilename);

    resp = ::rename(filename, newFilename);
    assert(resp == 0);
}

//-----------------------------------------------------------------------

void CFileSystem::removeFile(const char *filename)
{
    int ret;

    assert_no_null(filename);

    if (access(filename, R_OK) == 0)
    {
        ret = remove(filename);
        assert(ret == 0);
    }
}

//-----------------------------------------------------------------------

void CFileSystem::copyFiles(const char *pathOrigin, const char *pathDestiny)
{
    char buffer[1024];
    FILE *fileOrigin, *fileDestiny;

    fileOrigin = fopen(pathOrigin, "rb");
    fileDestiny = fopen(pathDestiny, "wb");
    assert_no_null(fileOrigin);
    assert_no_null(fileDestiny);

    while(feof(fileOrigin) == 0)
    {
        size_t byteRead;

        byteRead = fread(buffer, sizeof(char), 1024, fileOrigin);
        fwrite(buffer, sizeof(char), byteRead, fileDestiny);
    }

    fclose(fileOrigin);
    fclose(fileDestiny);
}

//-----------------------------------------------------------------------
//
bool CFileSystem::isExecutableFile(const char *filename)
{
    assert_no_null(filename);

    if (access(filename, X_OK) == 0)
        return true;
    else
        return false;
}

//-----------------------------------------------------------------------
//
bool CFileSystem::isFolder(const char *path)
{
    if (access(path, F_OK) == 0)
    {
        int res;
        struct stat statePath;

        res = stat(path, &statePath);
        assert(res == 0);

        if (S_ISDIR(statePath.st_mode))
            return true;
        else
            return false;
    }
    else
        return false;
}

//-----------------------------------------------------------------------

unsigned long CFileSystem::modificationDate(const char *filename)
{
    struct stat information;
    int ret;

    ret = stat(filename, &information);
    assert(ret == 0);

    return information.st_ctime;
}

//-----------------------------------------------------------------------

bool CFileSystem::firstIsMoreModern(const char *filename1, const char *filename2)
{
    struct stat information1, information2;
    int ret;

    ret = stat(filename1, &information1);
    assert(ret == 0);

    ret = stat(filename2, &information2);
    assert(ret == 0);

    if (information1.st_ctime > information2.st_ctime)
        return true;
    else
        return false;
}
