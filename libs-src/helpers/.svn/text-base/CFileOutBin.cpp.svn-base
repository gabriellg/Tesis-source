/* -----------------------------------------------------------------------------
*  CFileOutBin
*  Author: Gabriel López García (gabriellg AT gmail DOT com)
*          Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "CFileOutBin.hpp"

#include <stdio.h>
#include "asrtbas.h"
#include "strbas.h"

struct prv_fileBinOut_t
{
    FILE *file;
    const char* filename;
};

//------------------------------------------------------------------------------
//
static void prv_openFile( const char *filename, FILE **file )
{
    assert_no_null( filename );

    *file = fopen( filename, "wb" );
}

//------------------------------------------------------------------------------
//
static void prv_closeFile( FILE **file )
{
    int ret = fclose( *file );
    assert(ret == 0);

    *file = NULL;
}

//------------------------------------------------------------------------------
//
static struct prv_fileBinOut_t* prv_create( const char* filename )
{
    prv_fileBinOut_t *data = new prv_fileBinOut_t;
    assert_no_null( data );

    data->filename = strbas_copyString( filename );

    prv_openFile( filename, &data->file );

    return data;
}

//------------------------------------------------------------------------------
//
CFileOutBin :: CFileOutBin( const char* filename )
{
    m_data = prv_create( filename );
}

//------------------------------------------------------------------------------
//
CFileOutBin :: ~CFileOutBin()
{
    prv_closeFile( &m_data->file );

    delete m_data->filename;

    delete m_data;
}

//------------------------------------------------------------------------------
//
void CFileOutBin :: writeChar( char value )
{
    size_t escrito;

    escrito = fwrite( &value, sizeof(char), 1, m_data->file );
    assert(escrito == 1);
}

//------------------------------------------------------------------------------
//
void CFileOutBin :: writeString( const char *str )
{
    size_t escrito, tam_cadena;

    tam_cadena = strbas_length( str );

    escrito = fwrite( &tam_cadena, sizeof(size_t), 1, m_data->file);
    assert(escrito == 1);

    escrito = fwrite( str, sizeof(char), tam_cadena, m_data->file);
    assert(escrito == tam_cadena);
}

//------------------------------------------------------------------------------
//
void CFileOutBin :: writeString_optional( const char *str )
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
void CFileOutBin :: writeBool( bool value )
{
    size_t escrito;

    escrito = fwrite( &value, sizeof(bool), 1, m_data->file );
    assert( escrito == 1 );
    assert( value == false || value == true );
}

//------------------------------------------------------------------------------
//
void CFileOutBin :: writeByte( unsigned char value )
{
    size_t escrito;

    escrito = fwrite( &value, sizeof(unsigned char), 1, m_data->file );
    assert(escrito == 1);
}

//------------------------------------------------------------------------------
//
void CFileOutBin :: writeShort( short value )
{
    size_t escrito;

    escrito = fwrite( &value, sizeof(short), 1, m_data->file );
    assert(escrito == 1);
}

//------------------------------------------------------------------------------
//
void CFileOutBin :: writeUShort( unsigned short value )
{
    size_t escrito;

    escrito = fwrite( &value, sizeof(unsigned short), 1, m_data->file );
    assert(escrito == 1);
}

//------------------------------------------------------------------------------
//
void CFileOutBin :: writeInt( int value )
{
    size_t escrito;

    escrito = fwrite( &value, sizeof(int), 1, m_data->file );
    assert(escrito == 1);
}

//------------------------------------------------------------------------------
//
void CFileOutBin :: writeDouble( double value )
{
    size_t escrito;

    escrito = fwrite( &value, sizeof(double), 1, m_data->file );
    assert(escrito == 1);
}

//------------------------------------------------------------------------------
//
void CFileOutBin :: writeUChar( unsigned char value )
{
    size_t escrito;

    escrito = fwrite( &value, sizeof(unsigned char), 1, m_data->file );
    assert(escrito == 1);
}

//------------------------------------------------------------------------------
//
void CFileOutBin :: writeULong( unsigned long value )
{
    size_t escrito;

    escrito = fwrite( &value, sizeof(unsigned long), 1, m_data->file );
    assert(escrito == 1);
}
