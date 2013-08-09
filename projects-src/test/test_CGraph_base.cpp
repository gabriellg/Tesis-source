/* -----------------------------------------------------------------------------
*  Test_CGraph
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "stdlib.h"
#include "stdio.h"
#include "CGraph.hpp"
#include "CGraphIO.hpp"
#include "CVertex.hpp"
#include "CArray.hpp"
#include "CString.hpp"
#include "CArrayRef.hpp"
#include "CFileOutBin.hpp"
#include "CFileInBin.hpp"
#include "CFileSystem.hpp"
#include "asrtbas.h"

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

typedef class CArrayRef<CVertex> *(*prv_FPtr_getVertex)(const class CGraph<CVertex> *graph, const class CVertex *vertex);

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

    printf("> Creating the test graph\n");

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
static class CArrayRef<CVertex> *prv_getChildren(
                    const class CGraph<CVertex> *graph,
                    const class CVertex *vertex)
{
    assert_no_null(graph);
    return graph->getChildren(vertex);
}

//------------------------------------------------------------------------------
//
static class CArrayRef<CVertex> *prv_getParents(
                    const class CGraph<CVertex> *graph,
                    const class CVertex *vertex)
{
    assert_no_null(graph);
    return graph->getParents(vertex);
}

//------------------------------------------------------------------------------
//
static void prv_recursiveIteration(
                    unsigned long numRecursive,
                    const class CGraph<CVertex> *graph,
                    const class CArrayRef<CVertex> *vertexs,
                    prv_FPtr_getVertex func_vertexs,
                    char **walk)
{
    unsigned long num;

    assert(numRecursive < 100);
    numRecursive++;

    num = vertexs->size();

    for (unsigned long i = 0; i < num; i++)
    {
        const class CVertex *vertex;
        class CArrayRef<CVertex> *vertexsOfVertex;
        char label;

        vertex = vertexs->get(i);
        label = vertex->getLabel();
        CString::concatenateChar(walk, label);
        vertexsOfVertex = func_vertexs(graph, vertex);

        prv_recursiveIteration(numRecursive, graph, vertexsOfVertex, func_vertexs, walk);

        delete vertexsOfVertex;
    }
}

//------------------------------------------------------------------------------
//
static void prv_test_walkOnTheGraphFromInitials(const class CGraph<CVertex> *graph)
{
    class CArrayRef<CVertex> *vertexsInitials;
    char *walk;

    vertexsInitials = graph->getInitials();
    walk = CString::copy("");

    prv_recursiveIteration(0, graph, vertexsInitials, prv_getChildren, &walk);

    assert(CString::equal("013567462567", walk) == true);

    delete vertexsInitials;
}

//------------------------------------------------------------------------------
//
static void prv_test_walkOnTheGraphFromFinals(const class CGraph<CVertex> *graph)
{
    class CArrayRef<CVertex> *vertexsFinals;
    char *walk;

    vertexsFinals = graph->getFinals();
    walk = CString::copy("");

    prv_recursiveIteration(0, graph, vertexsFinals, prv_getParents, &walk);

    assert(CString::equal("64105203107520310", walk) == true);

    delete vertexsFinals;
}

//------------------------------------------------------------------------------
//
static void prv_test_write(const char *nameFile, class CGraph<CVertex> *graph)
{
    class CFileOutBin *out;

    out = new CFileOutBin(nameFile);
    CGraphIO<CVertex>::writeBin(graph, out);

    delete out;
}

//------------------------------------------------------------------------------
//
static class CGraph<CVertex> *prv_test_read(const char *nameFile)
{
    class CGraph<CVertex> *graph;
    class CFileInBin *in;

    in = new CFileInBin(nameFile);
    graph = CGraphIO<CVertex>::readBin(in);

    delete in;

    return graph;
}

//------------------------------------------------------------------------------
//
static void prv_test_readWriteAndCompare(class CGraph<CVertex> *graph)
{
    static const char *PRV_NAME_FILE_TEST = "testGraph.grf";
    class CGraph<CVertex> *graphRead;
    bool isEqual;

    prv_test_write(PRV_NAME_FILE_TEST, graph);
    graphRead = prv_test_read(PRV_NAME_FILE_TEST);
    isEqual = graphRead->equal(graph);
    assert(isEqual == true);

    delete graphRead;

    CFileSystem::removeFile(PRV_NAME_FILE_TEST);
}

//------------------------------------------------------------------------------
//
int main( int argc, char *argv[] )
{
    class CGraph<CVertex> *graph;

    printf("-----------------------------------------------------------\n");

    graph = prv_test_create();

    prv_test_isVertexAndNumParentsAndChildrens(graph);
    prv_test_walkOnTheGraphFromInitials(graph);
    prv_test_walkOnTheGraphFromFinals(graph);
    prv_test_readWriteAndCompare(graph);

    delete graph;

    printf("> All tests have been successful!\n");
    printf("-----------------------------------------------------------\n");

    return (EXIT_SUCCESS);
}
