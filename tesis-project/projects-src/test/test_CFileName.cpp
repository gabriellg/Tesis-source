/* -----------------------------------------------------------------------------
*  test CFileName
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include <iostream>
#include <stdlib.h>
#include "CFileName.hpp"
#include "CString.hpp"
#include "asrtbas.h"
using namespace std;

//------------------------------------------------------------------------------
//
void test_functions()
{
    char* str;
    static const char* filename = "myfile.txt";
    static const char* fullname = "/home/javi/file.exe";
    static const char* path = "/home/javi";
    
    str = CFileName::appenToFolder( path, filename );
    assert( CString::equal( str, "/home/javi/myfile.txt" ) == true );

    CString::free( &str );
    str = CFileName::appenToFolder( path, fullname );
    assert( CString::equal( str, "/home/javi/home/javi/file.exe" ) == true );

    CString::free( &str );
    str = CString::copy( path );
    CFileName::addFinalSlashBar( &str );
    assert( CString::equal( str, "/home/javi/" ) == true );
    CFileName::addFinalSlashBar( &str );
    assert( CString::equal( str, "/home/javi/" ) == true );

    CString::free( &str );
    str = CFileName::getPathName( filename );
    assert( CString::equal( str, "" ) == true );

    CString::free( &str );
    str = CFileName::getPathName( fullname );
    assert( CString::equal( str, "/home/javi/" ) == true );

    CString::free( &str );
    str = CFileName::getPathName( path );
    assert( CString::equal( str, "/home/" ) == true );


    CString::free( &str );
    str = CFileName::getFileName( filename );
    assert( CString::equal( str, filename ) == true );


    CString::free( &str );
    str = CFileName::getFileName( fullname );
    assert( CString::equal( str, "file.exe" ) == true );


    CString::free( &str );
    str = CFileName::getFileNameWithoutExtension( filename );
    assert( CString::equal( str, "myfile" ) == true );

    CString::free( &str );
    str = CFileName::getFileNameWithoutExtension( fullname );
    assert( CString::equal( str, "file" ) == true );


    CString::free( &str );
    str = CFileName::getExtension( filename );
    assert( CString::equal( str, "txt" ) == true );

    CString::free( &str );
    str = CFileName::getExtension( fullname );
    assert( CString::equal( str, "exe" ) == true );


    CString::free( &str );
    str = CFileName::getExtensionLowercase( "/home/javi/music.MP3" );
    assert( CString::equal( str, "mp3" ) == true );


    assert( CFileName::compareExtension( filename, "txt" ) == true );
    assert( CFileName::compareExtension( fullname, "exe" ) == true );


    CString::free( &str );
    str = CFileName::changeExtension( filename, "mp3" );
    assert( CString::equal( str, "myfile.mp3" ) == true );

    CString::free( &str );
    str = CFileName::changeExtension( fullname, "sh" );
    assert( CString::equal( str, "/home/javi/file.sh" ) == true );


    CString::free( &str );
    str = CFileName::generateNumberedFilename( path, filename,
                                                512, true, // numberAsPrefix
                                                "_" ); // separator
    assert( CString::equal( str, "/home/javi/0000000512_myfile.txt" ) == true );


    CString::free( &str );
    str = CFileName::generateNumberedFilename( path, filename,
                                                512, false, // numberAsPrefix
                                                "_" ); // separator
    assert( CString::equal( str, "/home/javi/myfile_0000000512.txt" ) == true );


    CString::free( &str );
}

//------------------------------------------------------------------------------
//
int main(int argc, char** argv)
{
    cout << "> Testing CFileName: ";

    test_functions();

    cout << "OK" << endl;

    return (EXIT_SUCCESS);
}
