/* -----------------------------------------------------------------------------
*  CFileName
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "CFileName.hpp"

#include "stdio.h"
#include "CString.hpp"
#include "asrtbas.h"

#define SLASHBAR    '/'

//-----------------------------------------------------------------------
//
char *CFileName :: appenToFolder(
                    const char *foldername,
                    const char *nameFileOrFolderToAppend)
{
    unsigned long lenght;

    assert_no_null( foldername );
    assert_no_null( nameFileOrFolderToAppend );

    lenght = CString::size(foldername);
    assert(lenght > 0);

    if( foldername[ lenght - 1 ] == SLASHBAR ||
        nameFileOrFolderToAppend[0] == SLASHBAR )
        return CString::printf("%s%s", foldername, nameFileOrFolderToAppend);
    else
        return CString::printf("%s%c%s", foldername, SLASHBAR, nameFileOrFolderToAppend);
}

//------------------------------------------------------------------------------
//
void CFileName :: addFinalSlashBar( char** foldername )
{
    unsigned long lenght;

    assert_no_null(foldername);

    lenght = CString::size(*foldername);
    assert(lenght > 0);

    if ((*foldername)[lenght - 1] != SLASHBAR)
    {
        CString::concatenateChar(foldername, SLASHBAR);
    }
}

//------------------------------------------------------------------------------
//
char* CFileName :: getPathName( const char* filename )
{
    unsigned long index;
    bool found;

    if( CString::size( filename ) == 0 )
        return CString::copy( "" );

    found = CString::rFindChar( filename, SLASHBAR, &index );

    if( found == true )
        return CString::substring( filename, 0, index + 1 );
    else
        return CString::copy( "" );
}

//------------------------------------------------------------------------------
//
char* CFileName :: getFileName( const char* filename )
{
    unsigned long length, index;
    bool found;

    length = CString::size( filename );

    if( length == 0 )
        return CString::copy( "" );

    found = CString::rFindChar( filename, SLASHBAR, &index );

    if( found == true )
        return CString::substring( filename, index + 1, length - index - 1 );
    else
        return CString::copy( filename );
}

//------------------------------------------------------------------------------
//
char* CFileName :: getFileNameWithoutExtension( const char* filename )
{
    char* onlyFilename;
    unsigned long index;
    bool found;

    onlyFilename = CFileName::getFileName( filename );

    found = CString::rFindChar( onlyFilename, '.', &index );

    if( found == true )
    {
        char* str;

        str = CString::substring( onlyFilename, 0, index );

        CString::free( &onlyFilename );

        return str;
    }

    return onlyFilename;
}

//------------------------------------------------------------------------------
//
char* CFileName :: getExtension( const char* filename )
{
    unsigned long index;
    bool found;

    found = CString::rFindChar( filename, '.', &index );

    if( found == true )
        return CString::substring( filename, index + 1, CString::size( filename ) - index - 1 );
    else
        return CString::copy( "" );
}

//------------------------------------------------------------------------------
//
char* CFileName :: getExtensionLowercase( const char* filename )
{
    char* fileExtension;
    char* lowerExtension;

    fileExtension = CFileName::getExtension( filename );
    lowerExtension = CString::toLower( fileExtension );

    CString::free( &fileExtension );

    return lowerExtension;
}

//------------------------------------------------------------------------------
//
bool CFileName :: compareExtension( const char* filename, const char* extension )
{
    char *fileExtension;
    bool equalExtensions;

    fileExtension = CFileName::getExtension( filename );

    equalExtensions = CString::equalWithoutMatchCase(fileExtension, extension);

    CString::free(&fileExtension);

    return equalExtensions;
}

//-----------------------------------------------------------------------
//
char *CFileName::changeExtension(
                    const char *filename,
                    const char *newExtension)
{
    char *newFilename;

    assert( CString::size( newExtension ) > 0 );
    assert( newExtension[0] != '.' );

    newFilename = CString::printf( "%s%s.%s",
                                  CFileName::getPathName( filename ),
                                  CFileName::getFileNameWithoutExtension( filename ),
                                  newExtension );

    return newFilename;
}

//------------------------------------------------------------------------------
//
char* CFileName :: generateNumberedFilename( const char* path,
                        const char* filename,
                        unsigned int numeration, bool numberAsPrefix,
                        const char* separatorOfNumberAndName )
{
    char *generatedName, *fullGeneratedName;

    if(numberAsPrefix == true)
    {
        generatedName = CString::printf("%010d%s%s", numeration, separatorOfNumberAndName, filename);
    }
    else
    {
        char* filenameWithoutExtension;
        char* extension;

        filenameWithoutExtension = CFileName::getFileNameWithoutExtension( filename );
        extension = CFileName::getExtension( filename );

        generatedName = CString::printf("%s%s%010d.%s", filenameWithoutExtension, separatorOfNumberAndName, numeration, extension);

        CString::free(&filenameWithoutExtension);
        CString::free(&extension);
    }

    fullGeneratedName = CFileName::appenToFolder( path, generatedName );

    CString::free(&generatedName);

    return fullGeneratedName;
}
