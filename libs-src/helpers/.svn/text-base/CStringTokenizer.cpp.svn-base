/* -----------------------------------------------------------------------------
*  CStringTokenizer
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "CStringTokenizer.hpp"
#include <sstream>
#include <string>
#include <string.h>

#include "CArrayString.hpp"
#include "asrtbas.h"
using namespace std;

struct prv_stringTokenizer_t
{
    class CArrayString *arrayTokens;
    unsigned long pos;
};

//------------------------------------------------------------------------------
//
static struct prv_stringTokenizer_t* prv_create()
{
    prv_stringTokenizer_t *data = new prv_stringTokenizer_t;
    assert_no_null( data );

    data->arrayTokens = new CArrayString();
    data->pos = 0;

    return data;
}

//------------------------------------------------------------------------------
//
static void prv_Calculate_Tokens( const char *str, char delimiter,
                                  bool remove_repeated_delimiters,
                                  class CArrayString **tokens )
{
    string buf;
    stringstream ss( str );

    while ( getline( ss, buf, delimiter ) )
    {
        if( !remove_repeated_delimiters || ! buf.empty() )
        {
            (*tokens)->add( strdup( buf.c_str() ) );
        }
    }
}

//------------------------------------------------------------------------------
//
CStringTokenizer :: CStringTokenizer( const char* str )
{
    m_data = prv_create();

    prv_Calculate_Tokens( str, ' ', true, &m_data->arrayTokens );
}

//------------------------------------------------------------------------------
//
CStringTokenizer :: CStringTokenizer( const char* str, char delimiter,
                                      bool remove_repeated_delimiters )
{
    m_data = prv_create();

    prv_Calculate_Tokens( str, delimiter, remove_repeated_delimiters,
                          &m_data->arrayTokens );
}

//------------------------------------------------------------------------------
//
CStringTokenizer :: ~CStringTokenizer()
{
    assert_no_null( m_data );

    delete m_data->arrayTokens;

    delete m_data;
}

//------------------------------------------------------------------------------
//
bool CStringTokenizer :: hasMoreTokens() const
{
    return m_data->pos < m_data->arrayTokens->size();
}

//------------------------------------------------------------------------------
//
unsigned int CStringTokenizer :: numTokens() const
{
    return m_data->arrayTokens->size();
}

//------------------------------------------------------------------------------
//
const char* CStringTokenizer :: getNextToken()
{
    assert( this->hasMoreTokens() == true );

    const char* str = m_data->arrayTokens->get( m_data->pos );

    ++m_data->pos;

    return str;
}

//------------------------------------------------------------------------------
//
const char* CStringTokenizer :: peekNextToken() const
{
    assert( this->hasMoreTokens() == true );

    return m_data->arrayTokens->get( m_data->pos );
}

//------------------------------------------------------------------------------
//
class CArrayString* CStringTokenizer :: getArrayOfTokens() const
{
    return new class CArrayString( m_data->arrayTokens );
}
