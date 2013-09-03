/* -----------------------------------------------------------------------------
*  CFileInTxt
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "CFileInTxt.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <algorithm>

#include "CArrayString.hpp"
#include "CString.hpp"
#include "asrtbas.h"
using namespace std;

struct prv_fileTxtIn_t
{
    ifstream file;
    char* filename;
};

//------------------------------------------------------------------------------
//
static void prv_openFile( const char* filename, ifstream *file )
{
    file->open( filename, ios::in );

    assert_with_message( file->fail() == false,
                         "There was an error opening the file" );
}

//------------------------------------------------------------------------------
//
static void prv_closeFile( ifstream *file )
{
    file->close();
}

//------------------------------------------------------------------------------
//
static struct prv_fileTxtIn_t* prv_create( const char* filename )
{
    struct prv_fileTxtIn_t *data;

    assert_no_null( filename );

    data = new prv_fileTxtIn_t;
    assert_no_null( data );

    data->filename = CString::copy( filename );

    prv_openFile( filename, &data->file );

    return data;
}

//------------------------------------------------------------------------------
//
CFileInTxt :: CFileInTxt( const char* filename )
{
    m_data = prv_create( filename );
}

//------------------------------------------------------------------------------
//
CFileInTxt :: ~CFileInTxt()
{
    prv_closeFile( &m_data->file );

    CString::free( &m_data->filename );

    delete m_data;
}

//------------------------------------------------------------------------------
//
bool CFileInTxt :: eof () const
{
    return m_data->file.eof();
}

//------------------------------------------------------------------------------
//
unsigned long CFileInTxt :: totalLines() const
{
    ifstream file;
    unsigned long numLines;

    prv_openFile( m_data->filename, &file );

    numLines = count( istreambuf_iterator<char>( file ),
                      istreambuf_iterator<char>(),'\n');

    prv_closeFile( &file );

    return numLines + 1;
}

//------------------------------------------------------------------------------
//
static unsigned int prv_getCurrentPos( const ifstream &file )
{
    const std::streampos currentPos = ((ifstream*)&file)->tellg();
    assert( currentPos >= 0 );

    return currentPos;
}

//------------------------------------------------------------------------------
//
static void prv_setFileToEnd( ifstream *file )
{
    assert_no_null( file );

    file->seekg( 0, ios::end );
}

//------------------------------------------------------------------------------
//
unsigned long CFileInTxt :: totalBytes() const
{
    ifstream file;
    unsigned long beginningPos, endPos, totalBytes;

    prv_openFile( m_data->filename, &file );

    beginningPos = prv_getCurrentPos( file );

    prv_setFileToEnd( &file );

    endPos = prv_getCurrentPos( file );

    totalBytes = endPos - beginningPos;

    prv_closeFile( &file );

    return totalBytes;
}

//------------------------------------------------------------------------------
//
bool CFileInTxt :: ungetChar()
{
    m_data->file.unget();

    if( m_data->file.fail() )
        return false;

    return true;
}

//------------------------------------------------------------------------------
//
void CFileInTxt :: ungetAll()
{
    prv_closeFile( &m_data->file );
    prv_openFile( m_data->filename, &m_data->file );
}

//------------------------------------------------------------------------------
//
char CFileInTxt :: readChar()
{
    char c;

    assert( m_data->file.eof() == false );

    c = m_data->file.get();

//    assert( m_data->file.gcount() == 1 );

    assert( m_data->file.fail() == false );

    return c;
}

//------------------------------------------------------------------------------
//
char* CFileInTxt :: readLine()
{
    string str;

    assert( m_data->file.eof() == false );

    getline( m_data->file, str );
    //assert( m_data->file.gcount() >= 1 );

    return strdup( str.c_str() );
}

//------------------------------------------------------------------------------
//
char* CFileInTxt :: readString()
{
    string str;

    assert( m_data->file.eof() == false );

    m_data->file >> str;
    assert( m_data->file.fail() == false );

    return CString::copy( str.c_str() );
}

//------------------------------------------------------------------------------
//
char* CFileInTxt :: readString_optional()
{
    if ( this->readBool() == true )
    {
        return this->readString();
    }

    return NULL;
}

//------------------------------------------------------------------------------
//
unsigned char CFileInTxt :: readByte()
{
    unsigned char car;

    assert( m_data->file.eof() == false );

    m_data->file >> car;
    assert( m_data->file.fail() == false );

    return car;
}

//------------------------------------------------------------------------------
//
bool CFileInTxt :: readBool()
{
    char c;

    c = this->readChar();
    assert( c == '0' || c == '1' );

    if( c == '0' )
        return false;

    return true;
}

//------------------------------------------------------------------------------
//
int CFileInTxt :: readInt()
{
    int value;

    assert( m_data->file.eof() == false );

    m_data->file >> value;
    assert( m_data->file.fail() == false );

    return value;
}

//------------------------------------------------------------------------------
//
short CFileInTxt :: readShort()
{
    short value;

    assert( m_data->file.eof() == false );

    m_data->file >> value;
    assert( m_data->file.fail() == false );

    return value;
}

//------------------------------------------------------------------------------
//
unsigned short CFileInTxt :: readUShort()
{
    unsigned short value;

    assert( m_data->file.eof() == false );

    m_data->file >> value;
    assert( m_data->file.fail() == false );

    return value;
}

//------------------------------------------------------------------------------
//
unsigned long CFileInTxt :: readULong()
{
    unsigned long value;

    assert( m_data->file.eof() == false );

    m_data->file >> value;
    assert( m_data->file.fail() == false );

    return value;
}

//------------------------------------------------------------------------------
//
double CFileInTxt :: readDouble()
{
    double value;

    assert( m_data->file.eof() == false );

    m_data->file >> value;
    assert( m_data->file.fail() == false );

    return value;
}

//------------------------------------------------------------------------------
//
class CArrayString* CFileInTxt :: readVector()
{
    class CArrayString *content;

    assert( m_data->file.eof() == false );

    content = new CArrayString();

    while( this->eof() == false )
    {
        const char* str;

        str = this->readLine();

        content->add( str );
    }

    return content;
}
