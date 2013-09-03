/* -----------------------------------------------------------------------------
*  CArrayString
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*          Gabriel López García (gabriellg AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "CArrayString.hpp"

#include "CString.hpp"
#include "memory.h"
#include "asrtbas.h"

//------------------------------------------------------------------------------
//
CArrayString::CArrayString()
:PrvBaseArray( 0, (CArray_FPtr_destroyElement)CString::free)
{
    ;
}

//------------------------------------------------------------------------------
//
CArrayString::CArrayString(unsigned long size)
:PrvBaseArray( size, (CArray_FPtr_destroyElement)CString::free)
{
    ;
}

//------------------------------------------------------------------------------
//
CArrayString::CArrayString( const class CArrayString *arrayString )
:PrvBaseArray( arrayString, (CArray_FPtr_copyElement)CString::copy)
{
    ;
}

//------------------------------------------------------------------------------
//
void CArrayString::clear()
{
    return PrvBaseArray::clear();
}

//------------------------------------------------------------------------------
//
unsigned long CArrayString::size() const
{
    return PrvBaseArray::size();
}

//------------------------------------------------------------------------------
//
const char* CArrayString::get( unsigned long ind ) const
{
    return (char *)PrvBaseArray::get( ind );
}

//------------------------------------------------------------------------------
//
const char* CArrayString::getLast() const
{
    return (char *)PrvBaseArray::getLast();
}

//------------------------------------------------------------------------------
//
void CArrayString::setDestroyingString(unsigned long ind, char **str)
{
    assert_no_null(str);
    PrvBaseArray::set( ind, (struct prv_item_t *)str);
    *str = NULL;
}

//------------------------------------------------------------------------------
//
void CArrayString::set( unsigned long ind, const char* str )
{
    char *str_copy;

    str_copy = CString::copy(str);
    PrvBaseArray::set( ind, (struct prv_item_t *)str_copy);
}

//------------------------------------------------------------------------------
//
void CArrayString::addDestroyingString (char **str)
{
    PrvBaseArray::add((struct prv_item_t *)*str);
    *str = NULL;
}

//------------------------------------------------------------------------------
//
void CArrayString::add(const char* str)
{
    char *str_copy;

    str_copy = CString::copy(str);
    PrvBaseArray::add((struct prv_item_t *)str_copy);
}

//------------------------------------------------------------------------------
//
void CArrayString::erase(unsigned long ind)
{
    PrvBaseArray::erase(ind);
}

//------------------------------------------------------------------------------
//
void CArrayString::eraseLast()
{
    PrvBaseArray::eraseLast();
}

//------------------------------------------------------------------------------
//
void CArrayString::concatenate( const class CArrayString *arrayStr )
{
    assert_no_null( arrayStr );
    PrvBaseArray::concatenate(arrayStr, (CArray_FPtr_copyElement)CString::copy);
}

//------------------------------------------------------------------------------
//
bool CArrayString::equal( const CArrayString *arrayStr ) const
{
    unsigned long num;

    num = PrvBaseArray::size();

    if( arrayStr->size() != num )
        return false;

    for( unsigned long i = 0; i < num; ++i )
    {
        const char *str1, *str2;

        str1 = (char *)PrvBaseArray::get(i);
        str2 = (char *)arrayStr->get(i);

        if( CString::equal(str1, str2) == false )
            return false;
    }

    return true;
}

//------------------------------------------------------------------------------
//
bool CArrayString::searchString( const char* str, unsigned long *ind ) const
{
    unsigned long num;

    num = PrvBaseArray::size();

    for( unsigned long i = 0; i < num; ++i)
    {
        const char *str1;

        str1 = (char *)PrvBaseArray::get(i);

        if( CString::equal(str1, str) == true)
        {
            if (ind != NULL)
                *ind = i;

            return true;
        }
    }

    return false;
}

//------------------------------------------------------------------------------
//
char *CArrayString::toString(char separator)
{
    char *str;
    unsigned long num;

    str = CString::copy("");

    num = PrvBaseArray::size();

    for (unsigned long i = 0; i < num; i++)
    {
        const char *strArray;

        strArray = (const char *)PrvBaseArray::get(i);
        CString::concatenate(&str, strArray);

        if (i < num - 1)
            CString::concatenateChar(&str, separator);
    }

    return str;
}

