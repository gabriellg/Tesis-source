/* -----------------------------------------------------------------------------
*  test CString
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include <iostream>
#include <stdlib.h>
#include "CString.hpp"
#include "asrtbas.h"
using namespace std;

//------------------------------------------------------------------------------
//
void test_functions()
{
    char *str, *str2;
    unsigned long index;

    str = CString::create(5);
    str[0] = 'a'; str[1] = 'b'; str[2] = 'c'; str[3] = 'd'; str[4] = 'e';
    assert( CString::size( str ) == 5 );
    CString::free( &str );

    str = CString::createWithInitialitation('c', 4);
    assert( CString::size( str ) == 4 );
    assert( CString::equal( str, "cccc" ) == true );

    str2 = CString::copy(str);
    assert( CString::equal( str, str2 ) == true );
    assert( CString::size( str2 ) == 4 );

    CString::concatenate( &str, "bbbb" );
    assert( CString::equal( str, "ccccbbbb" ) == true );

    CString::concatenatePrefix( "dddd", &str2 );
    assert( CString::equal( str2, "ddddcccc" ) == true );

    CString::concatenateChar( &str, 'a' );
    assert( CString::equal( str, "ccccbbbba" ) == true );
    
    CString::concatenatePrefixChar( 'e', &str2 );
    assert( CString::equal( str2, "eddddcccc" ) == true );

    assert( CString::equalWithoutMatchCase( str, "CcCcBbBbA" ) == true );
    assert( CString::equalWithoutMatchCase( str2, "EdDdDcCcC" ) == true );

    assert( CString::findChar( str, 'a', &index ) == true );
    assert( index == 8 );

    assert( CString::rFindChar( str2, 'd', &index ) == true );
    assert( index = 4 );

    CString::free( &str2 );

    str2 = CString::toUpper( str );
    assert( CString::equal( str2, "CCCCBBBBA" ) == true );

    CString::free( &str );

    str = CString::toLower( str2 );
    assert( CString::equal( str, "ccccbbbba" ) == true );

    CString::free( &str2 );

    str2 = CString::substring( str, 5, 4 );
    assert( CString::equal( str2, "bbba" ) == true );
    
    CString::free( &str2 );
    str2 = CString::getPrefix( str, 5 );
    assert( CString::equal( str2, "ccccb" ) == true );

    CString::free( &str2 );
    str2 = CString::getSuffix( str, 4 );
    assert( CString::equal( str2, "bbba" ) == true );

    CString::free( &str2 );
    str2 = CString::endExtractFromChar( str, 'c' );
    assert( CString::equal( str2, "bbbba" ) == true );

    CString::substChar( &str2, 'b', 'e');
    assert( CString::equal( str2, "eeeea" ) == true );

    CString::free( &str );
    CString::free( &str2 );
    str = CString::copy( "   aabbcc    " );
    str2 = CString::trim( str );
    assert( CString::equal( str2, "aabbcc" ) == true );

    CString::free( &str );
    CString::free( &str2 );
    str = CString::copy( "   aabbcc    " );
    str2 = CString::leftTrim( str );
    assert( CString::equal( str2, "aabbcc    " ) == true );

    CString::free( &str );
    CString::free( &str2 );
    str = CString::copy( "   aabbcc    " );
    str2 = CString::rightTrim( str );
    assert( CString::equal( str2, "   aabbcc" ) == true );

    CString::free( &str );
    str = CString::copy( "abcdefghijk" );
    assert( CString::isInitialSubstring( str, "abcde" ) == true );
    assert( CString::isInitialSubstring( str, "bc" ) == false );

    assert( CString::isSubstring( str, "efghi" ) == true );


    CString::free( &str2 );
    str2 = CString::printf( "..%d %c %s", 314, 'z', str );
    assert( CString::equal( str2, "..314 z abcdefghijk" ) == true );

    CString::free( &str );
    float value;
    str = CString::copy( "3.14" );
    CString::scanf(str, "%f", &value );
    assert( value - 3.14 < 0.001 );


    CString::free( &str );
    CString::free( &str2 );
}

//------------------------------------------------------------------------------
//
int main(int argc, char** argv)
{
    cout << "> Testing CString: ";

    test_functions();

    cout << "OK" << endl;

    return (EXIT_SUCCESS);
}
