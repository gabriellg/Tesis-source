/* -----------------------------------------------------------------------------
*  CFileInBin
*  Author: Gabriel López García (gabriellg AT gmail DOT com)
*          Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "CFileInBin.hpp"

#include <stdio.h>
#include "CString.hpp"
#include "asrtbas.h"

struct prv_fileBinIn_t
{
    FILE *file;
    const char* filename;
};

//------------------------------------------------------------------------------
//
static void prv_openFile( const char *filename, FILE **file )
{
    assert_no_null( filename );

    *file = fopen( filename, "rb" );
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
static struct prv_fileBinIn_t* prv_create( const char* filename )
{
    prv_fileBinIn_t *data = new prv_fileBinIn_t;
    assert_no_null( data );

    data->filename = CString::copy( filename );

    prv_openFile( filename, &data->file );

    return data;
}

//------------------------------------------------------------------------------
//
CFileInBin :: CFileInBin( const char* filename )
{
    m_data = prv_create( filename );
}

//------------------------------------------------------------------------------
//
CFileInBin :: ~CFileInBin()
{
    prv_closeFile( &m_data->file );

    delete m_data->filename;

    delete m_data;
}

//------------------------------------------------------------------------------
//
bool CFileInBin :: eof() const
{
    if( feof( m_data->file ) == 0 )
        return false;

    return true;
}

//------------------------------------------------------------------------------
//
char* CFileInBin :: readBuffer( char *buffer, unsigned long sizeBuffer )
{
    return fgets( buffer, sizeBuffer, m_data->file );
}

//------------------------------------------------------------------------------
//
char CFileInBin :: readChar()
{
    char car;
    size_t leidos;

    leidos = fread( &car, sizeof(char), 1, m_data->file );
    assert(leidos == 1);

    return car;
}

//------------------------------------------------------------------------------
//
char* CFileInBin :: readString()
{
    size_t leidos, tam;
    char *cadena;

    leidos = fread( &tam, sizeof(size_t), 1, m_data->file );
    assert(leidos == 1);

    cadena = CString::create(tam+1);

    leidos = fread( cadena, sizeof(char), tam, m_data->file );
    assert(leidos == tam);

    return cadena;
}

//------------------------------------------------------------------------------
//
char* CFileInBin :: readString_optional()
{
    if ( this->readBool() == true )
    {
        return this->readString();
    }

    return NULL;
}

//------------------------------------------------------------------------------
//
bool CFileInBin :: readBool()
{
    bool dato;
    size_t leidos;

    leidos = fread( &dato, sizeof(bool), 1, m_data->file );
    assert(leidos == 1);
    assert(dato == true || dato == false);

    return dato;
}

//------------------------------------------------------------------------------
//
unsigned char CFileInBin :: readByte()
{
    unsigned char car;
    size_t leidos;

    leidos = fread( &car, sizeof(unsigned char), 1, m_data->file );
    assert(leidos == 1);

    return car;
}

//------------------------------------------------------------------------------
//
short CFileInBin :: readShort()
{
    short shrt;
    size_t leidos;

    leidos = fread( &shrt, sizeof(short), 1, m_data->file );
    assert(leidos == 1);

    return shrt;
}

//------------------------------------------------------------------------------
//
unsigned short CFileInBin :: readUShort()
{
    unsigned long ushort;
    size_t leidos;

    leidos = fread(&ushort, sizeof(unsigned long), 1, m_data->file);
    assert(leidos == 1);

    return ushort;
}

//------------------------------------------------------------------------------
//
int CFileInBin :: readInt()
{
    int value;
    size_t size;

    size = fread( &value, sizeof(int), 1, m_data->file );
    assert(size == 1);

    return value;
}

//------------------------------------------------------------------------------
//
unsigned long CFileInBin :: readULong()
{
    unsigned long dato;
    size_t leidos;

    leidos = fread( &dato, sizeof(unsigned long), 1, m_data->file );
    assert(leidos == 1);

    return dato;
}

//------------------------------------------------------------------------------
//
double CFileInBin :: readDouble()
{
    double d;
    size_t leidos;

    leidos = fread( &d, sizeof(double), 1, m_data->file );
    assert(leidos == 1);

    return d;
}
