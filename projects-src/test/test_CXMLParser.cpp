/* -----------------------------------------------------------------------------
*  test CXMLParser
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include <iostream>
#include <stdlib.h>
#include "CXMLParser.hpp"
#include "CString.hpp"
#include "CFileSystem.hpp"
#include "CMapString.hpp"
#include "CArrayRef.hpp"
#include "asrtbas.h"
using namespace std;

//------------------------------------------------------------------------------
//
class CNodeXML* prv_createXML()
{
    class CNodeXML *rootXML, *nodeGraph, *node1, *node2, *node3, *node4;

    rootXML = new CNodeXML( "xml", "" );
    nodeGraph = new CNodeXML( "graph", "" );
    node1 = new CNodeXML( "node", "value1" );
    node2 = new CNodeXML( "node", "value2" );
    node3 = new CNodeXML( "edge", "w1" );
    node4 = new CNodeXML( "edge", "w2" );

    nodeGraph->addAttribute( "label", "mygraph" );
    node1->addAttribute( "id", "1" );
    node2->addAttribute( "id", "2" );
    node3->addAttribute( "from", "1" );
    node3->addAttribute( "to", "2" );
    node4->addAttribute( "from", "2" );
    node4->addAttribute( "to", "1" );

    rootXML->addChildNode( nodeGraph );
    nodeGraph->addChildNode( node1 );
    nodeGraph->addChildNode( node2 );
    nodeGraph->addChildNode( node3 );
    nodeGraph->addChildNode( node4 );

    return rootXML;
}

//------------------------------------------------------------------------------
//
void prv_test_readedXML( class CNodeXML *root1, class CNodeXML *root2 )
{
    class CMapString *map1, *map2, *map11, *map22;
    const class CNodeXML *ptr1, *ptr2;
    class CArrayRef<CNodeXML> *arrayChildren1, *arrayChildren2;
    class CArrayRef<CNodeXML> *arrayChildren11, *arrayChildren22;

    assert( CString::equal( root1->name(), root2->name() ) == true );
    assert( CString::equal( root1->value(), root2->value() ) == true );

    map1 = root1->getAttributes();
    map2 = root2->getAttributes();
    arrayChildren1 = root1->getChildren();
    arrayChildren2 = root2->getChildren();

    assert( map1->size() == 0 );
    assert( map2->size() == 0 );
    assert( arrayChildren1->size() == 1 );
    assert( arrayChildren2->size() == 1 );

    // bajamos un nivel <gxl><graph>
    ptr1 = arrayChildren1->get( 0 );
    ptr2 = arrayChildren2->get( 0 );

    assert( CString::equal( ptr1->name(), ptr2->name() ) == true );
    assert( CString::equal( ptr1->value(), ptr2->value() ) == true );

    map11 = ptr1->getAttributes();
    map22 = ptr2->getAttributes();
    arrayChildren11 = ptr1->getChildren();
    arrayChildren22 = ptr2->getChildren();

    assert( map11->size() == map22->size() );
    assert( arrayChildren11->size() == 4 );
    assert( arrayChildren22->size() == 4 );

    for( unsigned long i = 0; i < 4; ++i )
    {
        class CMapString *map111, *map222;
        class CArrayRef<CNodeXML> *arrayChildren111, *arrayChildren222;
        const class CNodeXML *child1, *child2;

        child1 = arrayChildren11->get( i );
        child2 = arrayChildren22->get( i );
        assert( CString::equal( child1->name(), child2->name() ) == true );
        assert( CString::equal( child1->value(), child2->value() ) == true );

        map111 = child1->getAttributes();
        map222 = child2->getAttributes();
        arrayChildren111 = child1->getChildren();
        arrayChildren222 = child2->getChildren();

        assert( map111->size() == map222->size() );
        assert( arrayChildren111->size() == arrayChildren222->size() );

        delete map111;
        delete map222;
        delete arrayChildren111;
        delete arrayChildren222;
    }

    delete map1;
    delete map2;
    delete map11;
    delete map22;
    delete arrayChildren1;
    delete arrayChildren2;
    delete arrayChildren11;
    delete arrayChildren22;
}

//------------------------------------------------------------------------------
//
/*void prv_processNodes( const class CNodeXML *node )
{
    class CMapString *map;
    class CArrayRef<CNodeXML> *arrayChildren;

    map = node->getAttributes();
    arrayChildren = node->getChildren();

    cout << ">" << node->name() << endl;
    cout << "  value: " << node->value() << endl;
    cout << "  att: " << map->size() << endl;
    cout << "  child: " << arrayChildren->size() << endl;

    delete map;
    delete arrayChildren;
}*/

//------------------------------------------------------------------------------
//
void test_functions()
{
    class CNodeXML *rootXML, *nodeReaded;
    const char* tmpFilename = "test_CXMLParser.xml";

    rootXML = prv_createXML();

    CXMLParser::save( tmpFilename, rootXML );

    nodeReaded = CXMLParser::load( tmpFilename );

    prv_test_readedXML( rootXML, nodeReaded );

    //nodeReaded->processChildren_recursive( prv_processNodes );


    CFileSystem::removeFile( tmpFilename );
    delete rootXML;
    delete nodeReaded;
}

//------------------------------------------------------------------------------
//
int main(int argc, char** argv)
{
    cout << "> Testing CXMLParser: ";

    test_functions();

    cout << "OK" << endl;

    return (EXIT_SUCCESS);
}
