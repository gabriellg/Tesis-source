/* -----------------------------------------------------------------------------
*  Teste CGraph
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include <iostream>
#include "stdio.h"
#include "stdlib.h"
#include "CGraph.hpp"
#include "CVertex.hpp"
#include "CSubGraph.hpp"
#include "CArray.hpp"
#include "CString.hpp"
#include "CArrayRef.hpp"
#include "asrtbas.h"
using namespace std;

/* Graph:      0
            /     \
           1       2
           |  \    |
           |   3   |
           |     \ |
           4       5
             \   / |
               6   7
*/

//------------------------------------------------------------------------------
//
static CGraph<CVertex>* prv_test_create()
{
    class CGraph<CVertex> *graph;
    class CVertex *vertex0, *vertex1, *vertex2, *vertex3;
    class CVertex *vertex4, *vertex5, *vertex6, *vertex7;

    graph = new CGraph<CVertex>();

    assert( graph->numInitials() == 0 );
    assert( graph->numFinals() == 0 );
    assert( graph->numVertex() == 0 );
    assert( graph->numEdges() == 0 );

    vertex0 = new CVertex( '0' );
    vertex1 = new CVertex( '1' );
    vertex2 = new CVertex( '2' );
    vertex3 = new CVertex( '3' );
    vertex4 = new CVertex( '4' );
    vertex5 = new CVertex( '5' );
    vertex6 = new CVertex( '6' );
    vertex7 = new CVertex( '7' );

    graph->addVertex(vertex0);
    graph->addVertex(vertex1);
    graph->addVertex(vertex2);
    graph->addVertex(vertex3);
    graph->addVertex(vertex4);
    graph->addVertex(vertex5);
    graph->addVertex(vertex6);
    graph->addVertex(vertex7);

    graph->addEdge( vertex0, vertex1 );
    graph->addEdge( vertex0, vertex2 );
    graph->addEdge( vertex1, vertex3 );
    graph->addEdge( vertex1, vertex4 );
    graph->addEdge( vertex2, vertex5 );
    graph->addEdge( vertex3, vertex5 );
    graph->addEdge( vertex4, vertex6 );
    graph->addEdge( vertex5, vertex6 );
    graph->addEdge( vertex5, vertex7 );

    assert( graph->numVertex() == 8 );
    assert( graph->numEdges() == 9 );
    assert( graph->numInitials() == 1 );
    assert( graph->numFinals() == 2 );

    assert( graph->numParents( vertex0 ) == 0);
    assert( graph->numParents( vertex1 ) == 1 );
    assert( graph->numParents( vertex2 ) == 1 );
    assert( graph->numParents( vertex3 ) == 1 );
    assert( graph->numParents( vertex4 ) == 1 );
    assert( graph->numParents( vertex5 ) == 2 );
    assert( graph->numParents( vertex6 ) == 2 );
    assert( graph->numParents( vertex7 ) == 1 );

    assert( graph->numChildren( vertex0 ) == 2 );
    assert( graph->numChildren( vertex1 ) == 2 );
    assert( graph->numChildren( vertex2 ) == 1 );
    assert( graph->numChildren( vertex3 ) == 1 );
    assert( graph->numChildren( vertex4 ) == 1 );
    assert( graph->numChildren( vertex5 ) == 2 );
    assert( graph->numChildren( vertex6 ) == 0 );
    assert( graph->numChildren( vertex7 ) == 0 );

    return graph;
}

//------------------------------------------------------------------------------
//
static void prv_test_isVertexAndNumParentsAndChildrens(const class CGraph<CVertex> *graph)
{
    class CVertex *vertex0, *vertex1, *vertex3, *vertex6;

    vertex0 = new CVertex('0');
    vertex1 = new CVertex('1');
    vertex3 = new CVertex('3');
    vertex6 = new CVertex('6');

    assert(graph->isVertex(vertex0) == true);
    assert(graph->numParents(vertex0) == 0);
    assert(graph->numChildren(vertex0) == 2);

    assert(graph->isVertex(vertex1) == true);
    assert(graph->numParents(vertex1) == 1);
    assert(graph->numChildren(vertex1) == 2);

    assert(graph->isVertex(vertex3) == true);
    assert(graph->numParents(vertex3) == 1);
    assert(graph->numChildren(vertex3) == 1);

    assert(graph->isVertex(vertex6) == true);
    assert(graph->numParents(vertex6) == 2);
    assert(graph->numChildren(vertex6) == 0);

    delete vertex0;
    delete vertex1;
    delete vertex3;
    delete vertex6;
}

//------------------------------------------------------------------------------
//
static void prv_test_createSubGraphs(const class CGraph<CVertex> *graph)
{
    class CArrayRef<CVertex> *vertexList;
    unsigned long lengthK, numVertex;

    assert_no_null( graph );

    lengthK = 3;
    vertexList = graph->getVertex();
    numVertex = vertexList->size();

    for( unsigned long ind = 0; ind < numVertex; ++ind )
    {
        class CSubGraph *subgraph;
        const class CVertex *vertex;


    cout << "n " << numVertex << endl;


        vertex = vertexList->get( ind );

        subgraph = new CSubGraph( (CGraph<IVertex>*)graph, vertex, lengthK );

        subgraph->print();

        delete subgraph;
    }

    delete vertexList;
}

//------------------------------------------------------------------------------
//
int main( int argc, char *argv[] )
{
    class CGraph<CVertex> *graph;

    printf("-----------------------------------------------------------\n");

    graph = prv_test_create();

    prv_test_isVertexAndNumParentsAndChildrens(graph);

    prv_test_createSubGraphs( graph );
    

    delete graph;

    printf("> All tests have been successful!\n");
    printf("-----------------------------------------------------------\n");

    return (EXIT_SUCCESS);
}
