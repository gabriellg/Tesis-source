/* -----------------------------------------------------------------------------
*  CSubGraph
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "CSubGraph.hpp"

#include <iostream>
#include "CGraph.hpp"
#include "CGraphIO.hpp"
#include "CSubGraphVertex.hpp"
#include "CMath.hpp"
#include "CArray.hpp"
#include "CArrayRef.hpp"
#include "asrtbas.h"
using namespace std;

struct prv_subgraph_t
{
    class CGraph<CSubGraphVertex> *graph;

    unsigned long maxLevel;
    bool isFinal;
    unsigned long numTimesRepeated;
};

//------------------------------------------------------------------------------
//
static struct prv_subgraph_t* prv_create( 
                                class CGraph<CSubGraphVertex> *graph,
                                int maxLevel, bool isFinal,
                                unsigned long numTimesRepeated )
{
    assert_no_null( graph );

    struct prv_subgraph_t *data = new prv_subgraph_t;
    assert_no_null( data );

    data->graph = graph;
    data->maxLevel = maxLevel;
    data->isFinal = isFinal;
    data->numTimesRepeated = numTimesRepeated;

    return data;
}

//------------------------------------------------------------------------------
//
static void prv_addVertex( const class CGraph<class IVertex> *originalGraph,
                           const class IVertex *originalVertex,
                           class CGraph<CSubGraphVertex> **graph,
                           class CArrayRef<IVertex> **addedVertexList )
{
    class CSubGraphVertex *newVertex;

    newVertex = new CSubGraphVertex( originalVertex,
                                     originalGraph->numParents( originalVertex ),
                                     originalGraph->numChildren( originalVertex ) );

    (*graph)->addVertex( newVertex );

    (*addedVertexList)->add( originalVertex );
}

//------------------------------------------------------------------------------
//
static unsigned long prv_copyVertex_recursive(
                                const class CGraph<class IVertex> *originalGraph,
                                const class IVertex *vertex,
                                int level,
                                int lengthK,
                                class CGraph<CSubGraphVertex> **subGraph,
                                class CArrayRef<IVertex> **addedVertexList,
                                bool *isFinal )
{
    class CArrayRef<IVertex> *childrenList;
    unsigned long numChildren;
    long int maxLevel;

    ++level;
    maxLevel = -1;

    childrenList = originalGraph->getChildren( vertex );
    numChildren = childrenList->size();

    if( numChildren == 0 )
    {
        *isFinal = true;
        delete childrenList;
        return level;
    }

    if ( level == lengthK )
    {
        *isFinal = false;
        delete childrenList;
        return level;
    }

    for ( unsigned long nc = 0; nc < numChildren; ++nc )
    {
        unsigned long childLevel;
        const class IVertex *childVertex;

        childVertex = childrenList->get( nc );

        // Si no ha sido añadido todavía -> lo añadimos

        if( (*addedVertexList)->exist<IVertex>( childVertex, NULL, NULL ) == false )
        {
            prv_addVertex( originalGraph,
                           childVertex,
                           subGraph,
                           addedVertexList );
        }

        childLevel = prv_copyVertex_recursive(
                                             originalGraph,
                                             childVertex,
                                             level,
                                             lengthK,
                                             subGraph,
                                             addedVertexList,
                                             isFinal );

        maxLevel = MAX( maxLevel, (long int)childLevel );
                               
    }

    delete childrenList;

    return maxLevel;
}

//------------------------------------------------------------------------------
//
static void prv_copyEdges(
                            const class CGraph<class IVertex> *originalGraph,
                            class CGraph<CSubGraphVertex> *subGraph )
{
    class CArrayRef<CSubGraphVertex> *vertexList;
    unsigned long numVertex;

    vertexList = subGraph->getVertex();
    numVertex = vertexList->size();

    for( unsigned long ind = 0; ind < numVertex; ++ind )
    {
        class CArrayRef<IVertex> *childrenList;
        const class CSubGraphVertex *vertex;
        unsigned long numChildren;

        vertex = vertexList->get( ind );

        childrenList = originalGraph->getChildren( vertex );
        numChildren = childrenList->size();

        for( unsigned long nc = 0; nc < numChildren; ++nc )
        {
            const class IVertex *child;

            child = childrenList->get( nc );

            subGraph->addEdge( vertex, (CSubGraphVertex*)child );
        }

        delete childrenList;
    }
}

//------------------------------------------------------------------------------
//
CSubGraph :: CSubGraph( const class CGraph<class IVertex> *originalGraph,
                        const class IVertex *copyFromVertex,
                        unsigned long lengthK )
{
    assert_no_null( originalGraph );
    assert_no_null( copyFromVertex );

    class CGraph<CSubGraphVertex> *subGraph;
    class CArrayRef<IVertex> *addedVertexList;
    unsigned long maxLevel;
    bool isFinal;

    subGraph = new CGraph<CSubGraphVertex>();
    addedVertexList = new CArrayRef<IVertex>();

    prv_addVertex( originalGraph,
                   copyFromVertex,
                   &subGraph,
                   &addedVertexList );

    maxLevel = prv_copyVertex_recursive(
                                    originalGraph,
                                    copyFromVertex,
                                    0,  // level
                                    lengthK,
                                    &subGraph,
                                    &addedVertexList,
                                    &isFinal );

    prv_copyEdges( originalGraph, subGraph );

    m_data = prv_create( subGraph, maxLevel, isFinal, 1 );

    delete addedVertexList;
}
/*
//------------------------------------------------------------------------------
//
CSubGraph::CSubGraph( class CFileBinIn *file )
{
    assert_no_null( file );

    class CGraph<CSubGraphVertex> *subGraph;
    unsigned long maxLevel, numTimesRepeated;
    bool isFinal;

    maxLevel = file->readULong();
    isFinal = file->readBool();
    numTimesRepeated = file->readULong();

    subGraph = (CGraph<CSubGraphVertex>*)CGraphIO::readFromBinaryStream( file, NULL );

    m_data = prv_create( subGraph, maxLevel, isFinal, numTimesRepeated );
}*/

//------------------------------------------------------------------------------
//
CSubGraph :: ~CSubGraph()
{
    assert_no_null( m_data );
    assert_no_null( m_data->graph );

    delete m_data->graph;
    delete m_data;
}

//------------------------------------------------------------------------------
//
unsigned long CSubGraph :: length() const
{
    return m_data->maxLevel;
}

//------------------------------------------------------------------------------
//
bool CSubGraph :: isFinal() const
{
    return m_data->isFinal;
}

//------------------------------------------------------------------------------
//
unsigned long CSubGraph :: numTimesRepeated() const
{
    return m_data->numTimesRepeated;
}

//------------------------------------------------------------------------------
//
void CSubGraph :: incrementNumTimesRepeated()
{
    ++m_data->numTimesRepeated;
}

//------------------------------------------------------------------------------
//
bool CSubGraph :: equal( const class CSubGraph *subgraph ) const
{
    if( m_data->isFinal != subgraph->m_data->isFinal ||
        m_data->maxLevel != subgraph->m_data->maxLevel )
    {
        return false;
    }

    return m_data->graph->equal( subgraph->m_data->graph );
}

//------------------------------------------------------------------------------
//
void CSubGraph :: print() const
{
    CGraphIO<CSubGraphVertex>::print( m_data->graph );

    cout << "Max Level: " << m_data->maxLevel << endl;
    cout << "is Final: " << m_data->isFinal << endl;
    cout << "Num Times Repeated: " << m_data->numTimesRepeated << endl;
}

/*
//------------------------------------------------------------------------------
//
void CSubGraph :: writeToBinaryStream( class CFileBinOut *file )
{
    assert_no_null( file );

    file->writeULong( m_data->maxLevel );
    file->writeBool( m_data->isFinal );
    file->writeULong( m_data->numTimesRepeated );

    CGraphIO::writeToBinaryStream( file, (CGraph<IVertex>*)m_data->subGraph );
}
*/
