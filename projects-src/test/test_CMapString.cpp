/* -----------------------------------------------------------------------------
*  test CMapString
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include <iostream>
#include <stdlib.h>
#include "CMapString.hpp"
#include "CString.hpp"
#include "asrtbas.h"
using namespace std;

//------------------------------------------------------------------------------
//
void test_functions()
{
    class CMapString *map, *map2;

    map = new CMapString();
    assert_no_null( map );
    assert( map->size() == 0 );

    map->add( "key1", "value1" );
    assert( map->size() == 1 );

    map->add( "key2", "value2" );
    assert( map->size() == 2 );

    map2 = new CMapString( map );
    assert( map2->size() == 2 );

    map2->clear();
    assert( map2->size() == 0 );
    assert( map->size() == 2 );

    assert( CString::equal( map->get( "key1" ), "value1" ) == true );
    assert( CString::equal( map->get( "key2" ), "value2" ) == true );


    map->set( "key1", "newvalue1" );
    assert( CString::equal( map->get( "key1" ), "newvalue1" ) == true );


    map->erase( "key2" );
    assert( map->size() == 1 );

    assert( map->exist( "key1" ) == true );
    assert( map->exist( "key2" ) == false );


    map2->add( "11", "v11" );
    map2->add( "22", "v22" );
    map2->add( "33", "v33" );
    assert( map2->size() == 3 );

    map->concatenate( map2 );
    assert( map->size() == 4 );

    assert( CString::equal( map->get( "key1" ), "newvalue1" ) == true );
    assert( CString::equal( map->get( "11" ), "v11" ) == true );
    assert( CString::equal( map->get( "22" ), "v22" ) == true );
    assert( CString::equal( map->get( "33" ), "v33" ) == true );


    map2->clear();
    assert( map2->size() == 0 );
    assert( map->size() == 4 );

    assert( CString::equal( map->get( "key1" ), "newvalue1" ) == true );
    assert( CString::equal( map->get( "11" ), "v11" ) == true );
    assert( CString::equal( map->get( "22" ), "v22" ) == true );
    assert( CString::equal( map->get( "33" ), "v33" ) == true );

    delete map2;

    assert( map->size() == 4 );

    assert( CString::equal( map->get( "key1" ), "newvalue1" ) == true );
    assert( CString::equal( map->get( "11" ), "v11" ) == true );
    assert( CString::equal( map->get( "22" ), "v22" ) == true );
    assert( CString::equal( map->get( "33" ), "v33" ) == true );


    delete map;
}

//------------------------------------------------------------------------------
//
int main(int argc, char** argv)
{
    cout << "> Testing CMapString: ";

    test_functions();

    cout << "OK" << endl;

    return (EXIT_SUCCESS);
}
