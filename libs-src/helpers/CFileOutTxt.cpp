/* -----------------------------------------------------------------------------
*  CFileOutTxt
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "CFileOutTxt.hpp"

#include <iostream>
#include <fstream>
#include <string.h>

#include "CArrayString.hpp"
#include "asrtbas.h"
using namespace std;

struct prv_fileOut_t
{
    ofstream file;
};

//------------------------------------------------------------------------------
//
static void prv_openFile( const char* filename, ofstream *file )
{
    file->open( filename, ios::out );

    assert_with_message( file->fail() == false,
                         "There was an error opening the file" );
}

//------------------------------------------------------------------------------
//
static void prv_closeFile( ofstream *file )
{
    file->close();
}

//------------------------------------------------------------------------------
//
static struct prv_fileOut_t* prv_create( const char* filename )
{
    prv_fileOut_t *data = new prv_fileOut_t;
    assert_no_null( data );

    prv_openFile( filename, &data->file );

    return data;
}

//------------------------------------------------------------------------------
//
CFileOutTxt :: CFileOutTxt( const char* filename )
{
    m_data = prv_create( filename );
}

//------------------------------------------------------------------------------
//
CFileOutTxt :: ~CFileOutTxt()
{
    prv_closeFile( &m_data->file );

    delete m_data;
}

//------------------------------------------------------------------------------
//
void CFileOutTxt :: writeChar( char c )
{
    m_data->file << c;
}

//------------------------------------------------------------------------------
//
void CFileOutTxt :: writeString( const char* str )
{
    m_data->file << str;
}

//------------------------------------------------------------------------------
//
void CFileOutTxt :: writeString_optional( const char* str )
{
    if( str != NULL )
    {
    	this->writeBool( true );
    	this->writeString( str );
    }

    this->writeBool( false );
}

//------------------------------------------------------------------------------
//
void CFileOutTxt :: writeBool( bool value )
{
    m_data->file << value;
}

//------------------------------------------------------------------------------
//
void CFileOutTxt :: writeByte( unsigned char value )
{
    m_data->file << value;
}

//------------------------------------------------------------------------------
//
void CFileOutTxt :: writeShort( short value )
{
    m_data->file << value;
}

//------------------------------------------------------------------------------
//
void CFileOutTxt :: writeUShort( unsigned short value )
{
    m_data->file << value;
}

//------------------------------------------------------------------------------
//
void CFileOutTxt :: writeInt( int value )
{
    m_data->file << value;
}

//------------------------------------------------------------------------------
//
void CFileOutTxt :: writeULong( unsigned long value )
{
    m_data->file << value;
}

//------------------------------------------------------------------------------
//
void CFileOutTxt :: writeDouble( double value )
{
    m_data->file << value;
}

//------------------------------------------------------------------------------
//
void CFileOutTxt :: writeArray( const class CArrayString *array )
{
    unsigned long size;

    assert_no_null( array );

    size = array->size();

    for( unsigned long i = 0; i < size; ++i )
    {
        this->writeString( array->get( i ) );

        if( i < size - 1 )
            this->writeChar( '\n' );
    }
}
