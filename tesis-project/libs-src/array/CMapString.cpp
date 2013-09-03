/* -----------------------------------------------------------------------------
*  CMapString
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "CMapString.hpp"

#include "CString.hpp"
#include "CArray.hpp"
#include "CArrayString.hpp"
#include "asrtbas.h"

struct prv_strPair_t
{
    prv_strPair_t( const char* strKey, const char* strValue )
    {
        key = CString::copy( strKey );
        value = CString::copy( strValue );
    }
    prv_strPair_t( const struct prv_strPair_t *pair )
    {
        key = CString::copy( pair->key );
        value = CString::copy( pair->value );
    }
    ~prv_strPair_t()
    {
        CString::free(&key);
        CString::free(&value);
    }
    char *key, *value;
};

struct prv_mapString_t
{
    class CArray<prv_strPair_t> *map;
};


//------------------------------------------------------------------------------
//
static bool prv_equal_map( const struct prv_strPair_t *strPair,
                           const char* strKey )
{
    return CString::equal( strPair->key, strKey );
}

//------------------------------------------------------------------------------
//
static struct prv_mapString_t* prv_create( class CArray<prv_strPair_t> *map )
{
    struct prv_mapString_t *data;

    assert_no_null( map );

    data = new prv_mapString_t;
    assert_no_null( data );

    data->map = map;

    return data;
}

//------------------------------------------------------------------------------
//
CMapString::CMapString()
{
    class CArray<prv_strPair_t> *map;

    map = new CArray<prv_strPair_t>();

    m_data = prv_create( map );
}

//------------------------------------------------------------------------------
//
CMapString::CMapString( const class CMapString *mapString )
{
    class CArray<prv_strPair_t> *newMap;

    assert_no_null( mapString );

    newMap = new CArray<prv_strPair_t>( mapString->m_data->map );

    m_data = prv_create( newMap );
}

//------------------------------------------------------------------------------
//
CMapString :: ~CMapString()
{
    delete m_data->map;
    delete m_data;
}

//------------------------------------------------------------------------------
//
void CMapString::clear()
{
    m_data->map->clear();
}

//------------------------------------------------------------------------------
//
unsigned long CMapString::size() const
{
    return m_data->map->size();
}

//------------------------------------------------------------------------------
//
const char* CMapString::get( const char* key ) const
{
    unsigned long ind;
    bool existKey;

    existKey = m_data->map->exist( key, prv_equal_map, &ind );
    assert( existKey == true );

    return m_data->map->get( ind )->value;
}

//------------------------------------------------------------------------------
//
class CArrayString* CMapString::getArrayKeys() const
{
    class CArrayString *array;
    unsigned long size;

    size = m_data->map->size();
    array = new CArrayString( size );

    for( unsigned long ind = 0; ind < size; ++ind )
    {
        char* strCopy;

        strCopy = CString::copy( m_data->map->get( ind )->key );

        array->set( ind, strCopy );
    }

    return array;
}

//------------------------------------------------------------------------------
//
void CMapString::set( const char* key, const char* value )
{
    struct prv_strPair_t *pair;
    unsigned long ind;
    bool existKey;

    existKey = m_data->map->exist( key, prv_equal_map, &ind );
    assert( existKey == true );

    pair = m_data->map->get( ind );

    CString::free( &pair->value );
    pair->value = CString::copy( value );
}

//------------------------------------------------------------------------------
//
void CMapString::add( const char* key, const char* value )
{
    struct prv_strPair_t *pair;

    assert( m_data->map->exist( key, prv_equal_map, NULL ) == false );

    pair = new prv_strPair_t( key, value );

    m_data->map->add( pair );
}

//------------------------------------------------------------------------------
//
void CMapString::erase( const char* key )
{
    unsigned long ind;
    bool existKey;

    existKey = m_data->map->exist( key, prv_equal_map, &ind );
    assert( existKey == true );

    m_data->map->erase( ind );
}

//------------------------------------------------------------------------------
//
void CMapString::concatenate( const class CMapString *mapString )
{
    assert_no_null( mapString );

    m_data->map->concatenate( mapString->m_data->map );
}

//------------------------------------------------------------------------------
//
bool CMapString::exist( const char* key ) const
{
    return m_data->map->exist( key, prv_equal_map, NULL );
}
