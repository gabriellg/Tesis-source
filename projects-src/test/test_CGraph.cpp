/* -----------------------------------------------------------------------------
*  Test CGraph
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include <iostream>
#include <stdlib.h>
#include <string.h>

#include "CGraph.hpp"
#include "CGraphIO.hpp"
#include "CVertex.hpp"
#include "CArray.hpp"
#include "CArrayRef.hpp"
#include "CFileSystem.hpp"
#include "CFileInTxt.hpp"
#include "CFileOutTxt.hpp"
#include "asrtbas.h"
using namespace std;

//#define PRINT_GRAPH

//------------------------------------------------------------------------------
//
static void prv_print_graph( const CGraph<CVertex> *graph )
{
    #ifdef PRINT_GRAPH
        CGraphIO<CVertex>::print( graph );
    #endif
}

//------------------------------------------------------------------------------
//
CGraph<CVertex>* test_create()
{
    class CGraph<CVertex> *graph;
    class CVertex *vertex0, *vertex1, *vertex2, *vertex3;
    class CVertex *vertex4, *vertex5, *vertex6, *vertex7;

    graph = new CGraph<CVertex>();
    assert( graph->numInitials() == 0 );
    assert( graph->numFinals() == 0 );
    assert( graph->numVertex() == 0 );
    assert( graph->numEdges() == 0 );
    prv_print_graph( graph );

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
    vertex0 = new CVertex( '0' );
    vertex1 = new CVertex( '1' );
    vertex2 = new CVertex( '2' );
    vertex3 = new CVertex( '3' );
    vertex4 = new CVertex( '4' );
    vertex5 = new CVertex( '5' );
    vertex6 = new CVertex( '6' );
    vertex7 = new CVertex( '7' );

    graph->addVertex( vertex0 );
    graph->addVertex( vertex1 );
    graph->addVertex( vertex2 );
    graph->addVertex( vertex3 );
    graph->addVertex( vertex4 );
    graph->addVertex( vertex5 );
    graph->addVertex( vertex6 );
    graph->addVertex( vertex7 );
    graph->addEdge( vertex0, vertex1 );
    graph->addEdge( vertex0, vertex2 );
    graph->addEdge( vertex1, vertex3 );
    graph->addEdge( vertex1, vertex4 );
    graph->addEdge( vertex2, vertex5 );
    graph->addEdge( vertex3, vertex5 );
    graph->addEdge( vertex4, vertex6 );
    graph->addEdge( vertex5, vertex6 );
    graph->addEdge( vertex5, vertex7 );

    prv_print_graph( graph );

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

    assert( graph->level( vertex0 ) == 0 );
    assert( graph->level( vertex1 ) == 1 );
    assert( graph->level( vertex2 ) == 1 );
    assert( graph->level( vertex3 ) == 2 );
    assert( graph->level( vertex4 ) == 2 );
    assert( graph->level( vertex5 ) == 2 );
    assert( graph->level( vertex6 ) == 3 );
    assert( graph->level( vertex7 ) == 3 );

    assert( graph->maxLevel() == 3 );

    return graph;
}

//------------------------------------------------------------------------------
//
CGraph<CVertex>* test_add()
{
    class CGraph<CVertex> *graph;
    class CVertex *vertexA, *vertexB, *vertexC;

    /* Graph:
            b<--->a
            ^     ^
             \   /
               c
    */

    graph = new CGraph<CVertex>(); //"test-graph" );
    assert( graph->numInitials() == 0 );
    assert( graph->numFinals() == 0 );
    assert( graph->numVertex() == 0 );
    assert( graph->numEdges() == 0 );
    prv_print_graph( graph );


    vertexA = new CVertex( 'a' );
    vertexB = new CVertex( 'b' );
    vertexC = new CVertex( 'c' );


    graph->addVertex( vertexA );
    assert( graph->numInitials() == 1 );
    assert( graph->numFinals() == 1 );
    assert( graph->numVertex() == 1 );
    assert( graph->numEdges() == 0 );
    prv_print_graph( graph );


    graph->addVertex( vertexB );
    assert( graph->numInitials() == 2 );
    assert( graph->numFinals() == 2 );
    assert( graph->numVertex() == 2 );
    assert( graph->numEdges() == 0 );
    prv_print_graph( graph );


    graph->addEdge( vertexA, vertexB );
    assert( graph->numInitials() == 1 );
    assert( graph->numFinals() == 1 );
    assert( graph->numVertex() == 2 );
    assert( graph->numEdges() == 1 );
    prv_print_graph( graph );


    graph->addEdge( vertexB, vertexA );
    assert( graph->numInitials() == 0 );
    assert( graph->numFinals() == 0 );
    assert( graph->numVertex() == 2 );
    assert( graph->numEdges() == 2 );
    prv_print_graph( graph );


    graph->addVertex( vertexC );
    assert( graph->numInitials() == 1 );
    assert( graph->numFinals() == 1 );
    assert( graph->numVertex() == 3 );
    assert( graph->numEdges() == 2 );
    prv_print_graph( graph );


    graph->addEdge( vertexC, vertexA );
    assert( graph->numInitials() == 1 );
    assert( graph->numFinals() == 0 );
    assert( graph->numVertex() == 3 );
    assert( graph->numEdges() == 3 );
    prv_print_graph( graph );


    graph->addEdge( vertexC, vertexB );
    assert( graph->numInitials() == 1 );
    assert( graph->numFinals() == 0 );
    assert( graph->numVertex() == 3 );
    assert( graph->numEdges() == 4 );
    prv_print_graph( graph );


    assert( graph->level( vertexA ) == 1 );
    assert( graph->level( vertexB ) == 1 );
    assert( graph->level( vertexC ) == 0 );

    assert( graph->maxLevel() == 1 );

    return graph;
}

//------------------------------------------------------------------------------
//
void test_getInitials( const CGraph<CVertex> *graph )
{
    class CArrayRef<CVertex> *array;

    assert_no_null( graph );
    assert( graph->numInitials() == 1 );
    assert( graph->numFinals() == 0 );
    assert( graph->numVertex() == 3 );
    assert( graph->numEdges() == 4 );

    array = (CArrayRef<CVertex>*)graph->getInitials();
    assert( array->size() == 1 );
    assert( array->get( 0 )->getLabel() == 'c' );

    delete array;
}

//------------------------------------------------------------------------------
//
void test_get( const CGraph<CVertex> *graph )
{
    const class CVertex *v;
    class CArrayRef<CVertex> *array;
    class CArrayRef<CVertex> *arrayVertex;


    assert_no_null( graph );
    assert( graph->numInitials() == 1 );
    assert( graph->numFinals() == 0 );
    assert( graph->numVertex() == 3 );
    assert( graph->numEdges() == 4 );

    arrayVertex = graph->getVertex();

    v = arrayVertex->get( 0 );
    assert( v->getLabel() == 'a' );
    assert( graph->level( v ) == 1 );
    assert( graph->numParents( v ) == 2 );
    assert( graph->numChildren( v ) == 1 );

    array = (CArrayRef<CVertex>*)graph->getParents( v );
    assert( array->size() == 2 );
    assert( array->get( 0 )->getLabel() == 'b' );
    assert( array->get( 1 )->getLabel() == 'c' );
    delete array;

    array = (CArrayRef<CVertex>*)graph->getChildren( v );
    assert( array->size() == 1 );
    assert( array->get( 0 )->getLabel() == 'b' );
    delete array;


    v = arrayVertex->get( 1 );
    assert( v->getLabel() == 'b' );
    assert( graph->level( v ) == 1 );
    assert( graph->numParents( v ) == 2 );
    assert( graph->numChildren( v ) == 1 );

    array = (CArrayRef<CVertex>*)graph->getParents( v );
    assert( array->size() == 2 );
    assert( array->get( 0 )->getLabel() == 'a' );
    assert( array->get( 1 )->getLabel() == 'c' );
    delete array;

    array = (CArrayRef<CVertex>*)graph->getChildren( v );
    assert( array->size() == 1 );
    assert( array->get( 0 )->getLabel() == 'a' );
    delete array;


    v = arrayVertex->get( 2 );
    assert( v->getLabel() == 'c' );
    assert( graph->level( v ) == 0 );
    assert( graph->numParents( v ) == 0 );
    assert( graph->numChildren( v ) == 2 );

    array = (CArrayRef<CVertex>*)graph->getParents( v );
    assert( array->size() == 0 );
    delete array;

    array = (CArrayRef<CVertex>*)graph->getChildren( v );
    assert( array->size() == 2 );
    assert( array->get( 0 )->getLabel() == 'a' );
    assert( array->get( 1 )->getLabel() == 'b' );
    delete array;

    delete arrayVertex;
}

//------------------------------------------------------------------------------
//
void test_search( const class CGraph<CVertex> *graph )
{
    assert_no_null( graph );
    assert( graph->numInitials() == 1 );
    assert( graph->numFinals() == 0 );
    assert( graph->numVertex() == 3 );
    assert( graph->numEdges() == 4 );

    class CArrayRef<CVertex> *arrayVertex;
    class CVertex *vertexAux;

    arrayVertex = graph->getVertex();

    const CVertex *vertexA = arrayVertex->get( 0 );
    const CVertex *vertexB = arrayVertex->get( 1 );
    const CVertex *vertexC = arrayVertex->get( 2 );

    assert( graph->isVertex( vertexA ) == true );
    assert( graph->isVertex( vertexB ) == true );
    assert( graph->isVertex( vertexC ) == true );

    vertexAux = new CVertex( 'a' );
    assert( graph->isVertex( vertexAux ) == false );

    delete arrayVertex;
    delete vertexAux;
}

//------------------------------------------------------------------------------
//
void test_copy( const class CGraph<CVertex> *graph )
{
    class CGraph<CVertex> *copiedGraph;
    class CArrayRef<CVertex> *copiedArrayVertex;
    class CArrayRef<CVertex> *arrayVertex;
    unsigned long size;

    assert_no_null( graph );

    copiedGraph = new CGraph<CVertex>( graph );

    assert( graph->numInitials() == copiedGraph->numInitials() );
    assert( graph->numFinals() == copiedGraph->numFinals() );
    assert( graph->numVertex() == copiedGraph->numVertex() );
    assert( graph->numEdges() == copiedGraph->numEdges() );
    assert( graph->maxLevel() == copiedGraph->maxLevel() );


    copiedArrayVertex = copiedGraph->getVertex();
    arrayVertex = graph->getVertex();
    size = arrayVertex->size();

    for( unsigned long ind = 0; ind < size; ++ind )
    {
        const CVertex *vertex1 = arrayVertex->get( ind );
        const CVertex *vertex2 = copiedArrayVertex->get( ind );

        assert( vertex1->equal( vertex2 ) == true );

        assert( vertex1->getLabel() == vertex2->getLabel() );
        assert( graph->level( vertex1 ) == copiedGraph->level( vertex2 ) );
        assert( graph->numParents( vertex1 ) == copiedGraph->numParents( vertex2 ) );
        assert( graph->numChildren( vertex1 ) == copiedGraph->numChildren( vertex2 ) );

        assert( graph->isVertex( vertex1 ) == true );
        assert( graph->isVertex( vertex2 ) == false );
        assert( copiedGraph->isVertex( vertex1 ) == false );
        assert( copiedGraph->isVertex( vertex2 ) == true );
    }

    delete arrayVertex;
    delete copiedArrayVertex;
    delete copiedGraph;
}

//------------------------------------------------------------------------------
//
void test_compareVertex()
{
    class CVertex *vertex1, *vertex2, *vertex3, *vertex4;

    vertex1 = new CVertex( '1' );
    vertex2 = new CVertex( '1' );
    vertex3 = new CVertex( '0' );
    vertex4 = new CVertex( '\0' );

    assert( vertex1->equal( vertex1 ) == true );
    assert( vertex1->equal( vertex2 ) == true );
    assert( vertex1->equal( vertex3 ) == false );
    assert( vertex1->equal( vertex4 ) == false );

    assert( vertex2->equal( vertex1 ) == true );
    assert( vertex2->equal( vertex2 ) == true );
    assert( vertex2->equal( vertex3 ) == false );
    assert( vertex2->equal( vertex4 ) == false );

    assert( vertex3->equal( vertex1 ) == false );
    assert( vertex3->equal( vertex2 ) == false );
    assert( vertex3->equal( vertex3 ) == true );
    assert( vertex3->equal( vertex4 ) == false );

    assert( vertex4->equal( vertex1 ) == false );
    assert( vertex4->equal( vertex2 ) == false );
    assert( vertex4->equal( vertex3 ) == false );
    assert( vertex4->equal( vertex4 ) == true );

    delete vertex1;
    delete vertex2;
    delete vertex3;
    delete vertex4;
}

//------------------------------------------------------------------------------
//
void test_save_and_load( const class CGraph<CVertex> *graph )
{
    class CFileInTxt *fileIn;
    class CFileOutTxt *fileOut;
    class CGraph<CVertex> *graph2;

    assert_no_null( graph );

    const char* tmpFilename1 = "test_CGraph.grf";

    fileOut = new CFileOutTxt( tmpFilename1 );

    CGraphIO<CVertex>::writeGRF( graph, fileOut );

    delete fileOut;

    fileIn = new CFileInTxt( tmpFilename1 );

    graph2 = CGraphIO<CVertex>::readGRF( fileIn );
    //CGraphIO<CVertex>::print( graph2 );

    delete fileIn;
    
    assert( graph->numInitials() == graph2->numInitials() );
    assert( graph->numVertex() == graph2->numVertex() );
    assert( graph->numEdges() == graph2->numEdges() );
    assert( graph->numFinals() == graph2->numFinals() );
    assert( graph->maxLevel() == graph2->maxLevel() );
    assert( graph->equal( graph2 ) == true );



    delete graph2;

    CFileSystem::removeFile( tmpFilename1 );
}


//------------------------------------------------------------------------------
//
void test_compareGraphs( const class CGraph<CVertex> *graph1,
                         const class CGraph<CVertex> *graph2,
                         const class CGraph<CVertex> *graph3 )
{
    class CGraph<CVertex> *graph11, *graph22, *graph33;

    assert( graph1->equal( graph1 ) == true );
    assert( graph2->equal( graph2 ) == true );
    assert( graph3->equal( graph3 ) == true );


    assert( graph1->equal( graph2 ) == false );
    assert( graph2->equal( graph1 ) == false );

    assert( graph1->equal( graph3 ) == true );
    assert( graph3->equal( graph1 ) == true );

    assert( graph2->equal( graph3 ) == false );
    assert( graph3->equal( graph2 ) == false );


    graph11 = new CGraph<CVertex>( graph1 );
    graph22 = new CGraph<CVertex>( graph2 );
    graph33 = new CGraph<CVertex>( graph3 );

    assert( graph1->equal( graph11 ) == true );
    assert( graph11->equal( graph1 ) == true );

    assert( graph2->equal( graph22 ) == true );
    assert( graph22->equal( graph2 ) == true );

    assert( graph3->equal( graph33 ) == true );
    assert( graph33->equal( graph3 ) == true );

    delete graph11;
    delete graph22;
    delete graph33;
}


//------------------------------------------------------------------------------
//
void test_compareGraphs_2( const class CGraph<CVertex> *graph1,
                           const class CGraph<CVertex> *graph2,
                           const class CGraph<CVertex> *graph3 )
{
    class CGraph<CVertex> *graphA, *graphB;
    class CVertex *vertex0, *vertex1, *vertex2, *vertex3;
    class CVertex *vertex4, *vertex5, *vertex6, *vertex7;
    class CVertex *vertexB0, *vertexB1, *vertexB2, *vertexB3;
    class CVertex *vertexB4, *vertexB5, *vertexB6, *vertexB7;

    graphA = new CGraph<CVertex>();
    graphB = new CGraph<CVertex>();
    /* Graph:      0
                /     \
               2       1
               |  \    |
               |   3   |
               |     \ |
               4       5
                 \   / |
                   7   6
    */
    vertex0 = new CVertex( '0' );
    vertex1 = new CVertex( '1' );
    vertex2 = new CVertex( '2' );
    vertex3 = new CVertex( '3' );
    vertex4 = new CVertex( '4' );
    vertex5 = new CVertex( '5' );
    vertex6 = new CVertex( '6' );
    vertex7 = new CVertex( '7' );

    graphA->addVertex( vertex0 );
    graphA->addVertex( vertex1 );
    graphA->addVertex( vertex2 );
    graphA->addVertex( vertex3 );
    graphA->addVertex( vertex4 );
    graphA->addVertex( vertex5 );
    graphA->addVertex( vertex6 );
    graphA->addVertex( vertex7 );
    graphA->addEdge( vertex0, vertex1 );
    graphA->addEdge( vertex0, vertex2 );
    graphA->addEdge( vertex1, vertex5 );
    graphA->addEdge( vertex2, vertex3 );
    graphA->addEdge( vertex2, vertex4 );
    graphA->addEdge( vertex3, vertex5 );
    graphA->addEdge( vertex4, vertex7 );
    graphA->addEdge( vertex5, vertex7 );
    graphA->addEdge( vertex5, vertex6 );


    vertexB0 = new CVertex( '0' );
    vertexB1 = new CVertex( '1' );
    vertexB2 = new CVertex( '2' );
    vertexB3 = new CVertex( '3' );
    vertexB4 = new CVertex( '4' );
    vertexB5 = new CVertex( '5' );
    vertexB6 = new CVertex( '6' );
    vertexB7 = new CVertex( '7' );

    graphB->addVertex( vertexB7 );
    graphB->addVertex( vertexB6 );
    graphB->addVertex( vertexB5 );
    graphB->addVertex( vertexB4 );
    graphB->addVertex( vertexB3 );
    graphB->addVertex( vertexB2 );
    graphB->addVertex( vertexB1 );
    graphB->addVertex( vertexB0 );
    graphB->addEdge( vertexB5, vertexB7 );
    graphB->addEdge( vertexB5, vertexB6 );
    graphB->addEdge( vertexB4, vertexB7 );
    graphB->addEdge( vertexB3, vertexB5 );
    graphB->addEdge( vertexB2, vertexB4 );
    graphB->addEdge( vertexB2, vertexB3 );
    graphB->addEdge( vertexB1, vertexB5 );
    graphB->addEdge( vertexB0, vertexB1 );
    graphB->addEdge( vertexB0, vertexB2 );


    assert( graphA->equal( graphA ) == true );
    assert( graphB->equal( graphB ) == true );

    assert( graphA->equal( graphB ) == true );
    assert( graphB->equal( graphA ) == true );

    assert( graphA->equal( graph1 ) == false );
    assert( graphA->equal( graph2 ) == false );
    assert( graphA->equal( graph3 ) == false );
    assert( graphB->equal( graph1 ) == false );
    assert( graphB->equal( graph2 ) == false );
    assert( graphB->equal( graph3 ) == false );

    assert( graph1->equal( graph2 ) == false );
    assert( graph2->equal( graph1 ) == false );
    assert( graph1->equal( graph3 ) == true );
    assert( graph3->equal( graph1 ) == true );
    assert( graph2->equal( graph3 ) == false );
    assert( graph3->equal( graph2 ) == false );

    delete graphA;
    delete graphB;
}

//------------------------------------------------------------------------------
//
void test_acyclic( const class CGraph<CVertex> *graph1,
                   const class CGraph<CVertex> *graph2 )
{
    class CGraph<CVertex> *graphA, *graphB, *graphC, *graphD, *graphE;
    class CVertex *vertex0, *vertex1, *vertex2, *vertex3;
    class CVertex *vertex4, *vertex5, *vertex6, *vertex7;


    assert( graph1->isAcyclic() == true );

    assert( graph2->isAcyclic() == false );

    ///////////

    graphA = new CGraph<CVertex>();

    assert( graphA->isAcyclic() == true );


    vertex0 = new CVertex( '0' );
    vertex1 = new CVertex( '1' );

    graphA->addVertex( vertex0 );
    graphA->addVertex( vertex1 );

    graphA->addEdge( vertex0, vertex1 );
    graphA->addEdge( vertex1, vertex0 );

    assert( graphA->isAcyclic() == false );

    ///////////

    graphB = new CGraph<CVertex>();

    vertex0 = new CVertex( '0' );

    graphB->addVertex( vertex0 );

    graphB->addEdge( vertex0, vertex0 );

    assert( graphB->isAcyclic() == false );

    ///////////

    graphC = new CGraph<CVertex>();

    vertex0 = new CVertex( '0' );
    vertex1 = new CVertex( '1' );
    vertex2 = new CVertex( '2' );
    vertex3 = new CVertex( '3' );
    vertex4 = new CVertex( '4' );

    graphC->addVertex( vertex0 );
    graphC->addVertex( vertex1 );
    graphC->addVertex( vertex2 );
    graphC->addVertex( vertex3 );
    graphC->addVertex( vertex4 );

    graphC->addEdge( vertex0, vertex1 );
    graphC->addEdge( vertex1, vertex2 );
    graphC->addEdge( vertex2, vertex3 );
    graphC->addEdge( vertex3, vertex4 );
    graphC->addEdge( vertex4, vertex0 );

    assert( graphC->isAcyclic() == false );


    ///////////
    
    graphD = new CGraph<CVertex>();

    vertex0 = new CVertex( '0' );
    vertex1 = new CVertex( '1' );
    vertex2 = new CVertex( '2' );
    vertex3 = new CVertex( '3' );
    vertex4 = new CVertex( '4' );

    graphD->addVertex( vertex0 );
    graphD->addVertex( vertex1 );
    graphD->addVertex( vertex2 );
    graphD->addVertex( vertex3 );
    graphD->addVertex( vertex4 );

    graphD->addEdge( vertex0, vertex1 );
    graphD->addEdge( vertex1, vertex2 );
    graphD->addEdge( vertex2, vertex3 );
    graphD->addEdge( vertex2, vertex4 );
    graphD->addEdge( vertex4, vertex1 );

    assert( graphD->isAcyclic() == false );


    ///////////

    graphE = new CGraph<CVertex>();

    vertex0 = new CVertex( '0' );
    vertex1 = new CVertex( '1' );
    vertex2 = new CVertex( '2' );
    vertex3 = new CVertex( '3' );
    vertex4 = new CVertex( '4' );
    vertex5 = new CVertex( '5' );
    vertex6 = new CVertex( '6' );
    vertex7 = new CVertex( '7' );


    graphE->addVertex( vertex0 );
    graphE->addVertex( vertex1 );
    graphE->addVertex( vertex2 );
    graphE->addVertex( vertex3 );
    graphE->addVertex( vertex4 );
    graphE->addVertex( vertex5 );
    graphE->addVertex( vertex6 );
    graphE->addVertex( vertex7 );

    graphE->addEdge( vertex0, vertex1 );
    graphE->addEdge( vertex0, vertex2 );
    graphE->addEdge( vertex0, vertex3 );
    graphE->addEdge( vertex0, vertex7 );

    graphE->addEdge( vertex1, vertex4 );
    graphE->addEdge( vertex1, vertex3 );
    graphE->addEdge( vertex1, vertex7 );

    graphE->addEdge( vertex2, vertex7 );

    graphE->addEdge( vertex3, vertex2 );
    graphE->addEdge( vertex3, vertex5 );
    graphE->addEdge( vertex3, vertex6 );
    graphE->addEdge( vertex3, vertex7 );

    graphE->addEdge( vertex4, vertex3 );
    graphE->addEdge( vertex4, vertex5 );
    graphE->addEdge( vertex4, vertex7 );

    graphE->addEdge( vertex5, vertex6 );
    graphE->addEdge( vertex5, vertex7 );

    graphE->addEdge( vertex6, vertex2 );
    graphE->addEdge( vertex6, vertex7 );

    assert( graphE->isAcyclic() == true );


    graphE->addEdge( vertex6, vertex0 );


    assert( graphE->isAcyclic() == false );


    delete graphA;
    delete graphB;
    delete graphC;
    delete graphD;
    delete graphE;
}


//------------------------------------------------------------------------------
//
int main()
{
    cout << "-----------------------------------------------------------" << endl;
    cout << "> Testing CGraph" << endl;


    class CGraph<CVertex> *graph1, *graph2, *graph3;

    graph1 = test_create();
    test_copy( graph1 );
    test_copy( graph1 );  // x2 assert no removed original vertex
    //test_save_and_load( graph1 );


    graph2 = test_add();
    test_getInitials( graph2 );
    test_get( graph2 );
    test_search( graph2 );
    test_copy( graph2 );

    test_save_and_load( graph2 );


    test_compareVertex();


    //CGraph *graph3 = Test_CGraph_Loading( tmpFilename );
    graph3 = new CGraph<CVertex>( graph1 );

    test_compareGraphs( graph1, graph2, graph3 );
    test_compareGraphs( graph1, graph2, graph3 );      // x2

    test_compareGraphs_2( graph1, graph2, graph3 );

    test_acyclic( graph1, graph2 );



    delete graph1;
    delete graph2;
    delete graph3;


    cout << "> All tests have been successful!" << endl;
    cout << "-----------------------------------------------------------" << endl;

    return (EXIT_SUCCESS);
}
