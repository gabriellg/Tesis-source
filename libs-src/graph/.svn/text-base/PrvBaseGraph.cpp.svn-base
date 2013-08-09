/* -----------------------------------------------------------------------------
*  PrvBaseGraph
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "PrvBaseGraph.inl"

#include "IVertex.hpp"
#include "CArray.hpp"
#include "CArrayRef.hpp"
#include "CMath.hpp"
#include "asrtbas.h"
#include "memory.h"

struct PrvVertexData
{
    PrvVertexData(class IVertex *vertex);
    ~PrvVertexData();

    class IVertex *vertex;
    class CArrayRef<PrvVertexData> *parentsList;
    class CArrayRef<PrvVertexData> *childrenList;
};

struct prv_baseGraph_t
{
    class CArray<PrvVertexData> *vertexDataList;
};

//------------------------------------------------------------------------------
//
PrvVertexData::PrvVertexData(class IVertex *ivertex)
{
    vertex = ivertex;
    parentsList = new CArrayRef<PrvVertexData>();
    childrenList = new CArrayRef<PrvVertexData>();
}

//------------------------------------------------------------------------------
//
PrvVertexData::~PrvVertexData()
{
    delete vertex;
    delete parentsList;
    delete childrenList;
}

//------------------------------------------------------------------------------
//
static void prv_integrity(const struct prv_baseGraph_t *dataGraph)
{
    assert_no_null(dataGraph);
    assert_no_null(dataGraph->vertexDataList);
}

//------------------------------------------------------------------------------
//
static struct prv_baseGraph_t *prv_create( 
                        class CArray<PrvVertexData> **vertexDataList)
{
    struct prv_baseGraph_t *data;

    data = MALLOC(prv_baseGraph_t);

    data->vertexDataList = ASSIGN_PP_NO_NULL(vertexDataList, class CArray<PrvVertexData>);

    return data;
}

//------------------------------------------------------------------------------
//
PrvBaseGraph::PrvBaseGraph(void)
{
    class CArray<PrvVertexData> *vertexDataList;

    vertexDataList = new CArray<PrvVertexData>();
    m_data = prv_create(&vertexDataList);
}

//------------------------------------------------------------------------------
//
static bool prv_searchVertex(const class PrvVertexData *dataVertex,
                                const class IVertex *vertex )
{
    //if ( dataVertex->vertex->equal(vertex) == true)
    if( dataVertex->vertex == vertex )
        return true;
    else
        return false;
}

//------------------------------------------------------------------------------
//
static bool prv_existIVertex(
                    const class CArray<PrvVertexData> *vertexDataList,
                    const class IVertex *vertex, unsigned long *ind_optional )
{
    return vertexDataList->exist<IVertex>(vertex, prv_searchVertex, ind_optional);
}

//------------------------------------------------------------------------------
//
static class PrvVertexData *prv_getPrvVertexData(
                                const class CArray<PrvVertexData> *vertexDataList,
                                const class IVertex *vertex)
{
    class PrvVertexData *vertextData;

    assert_no_null(vertexDataList);

    vertextData = vertexDataList->getOnlyOneElement<IVertex>(vertex, prv_searchVertex);
    assert_no_null(vertextData);

    return vertextData;
}

//------------------------------------------------------------------------------
//
static class CArray<PrvVertexData>* prv_copyVertex(
                            const class CArray<PrvVertexData> *originalVertexDataList,
                            CGraph_FPtr_copyVertex func_copyVertex )
{
    unsigned long numVertex;
    class CArray<PrvVertexData> *copiedVertexDataList;

    numVertex = originalVertexDataList->size();
    copiedVertexDataList = new CArray<PrvVertexData>( numVertex );

    for( unsigned long ind = 0; ind < numVertex; ++ind )
    {
        const class IVertex *originalVertex;
        class IVertex *copiedVertex;
        struct PrvVertexData *newVertexData;

        originalVertex = originalVertexDataList->get( ind )->vertex;
        copiedVertex = func_copyVertex(originalVertex);

        newVertexData = new PrvVertexData( copiedVertex );
        copiedVertexDataList->set( ind, newVertexData );
    }

    return copiedVertexDataList;
}

//------------------------------------------------------------------------------
//
static void prv_copyEdges(
                    const class CArray<PrvVertexData> *originalVertexDataList,
                    class CArray<PrvVertexData> *copiedVertexDataList)
{
    unsigned long numParents;

    numParents = originalVertexDataList->size();

    for( unsigned long ind = 0; ind < numParents; ++ind)
    {
        unsigned long numChildren;
        const PrvVertexData *copiedParent;
        const class CArrayRef<PrvVertexData> *originalChildrenList;

        originalChildrenList = originalVertexDataList->get(ind)->childrenList;
        numChildren = originalChildrenList->size();
        copiedParent = copiedVertexDataList->get(ind);

        for( unsigned long nc = 0; nc < numChildren; ++nc )
        {
            bool existChild;
            unsigned long child_ind;
            const PrvVertexData *originalChild;
            const PrvVertexData *copiedChild;

            originalChild = originalChildrenList->get(nc);

            existChild = prv_existIVertex(originalVertexDataList, originalChild->vertex, &child_ind);
            assert( existChild == true );
            copiedChild = copiedVertexDataList->get(child_ind);

            copiedParent->childrenList->add(copiedChild);
            copiedChild->parentsList->add(copiedParent);
        }
    }
}

//------------------------------------------------------------------------------
//
PrvBaseGraph::PrvBaseGraph( const class PrvBaseGraph *graph,
                              CGraph_FPtr_copyVertex func_copyVertex )
{
    class CArray<PrvVertexData> *copiedVertexDataList;

    assert_no_null( graph );
    assert_no_null( graph->m_data );

    copiedVertexDataList = prv_copyVertex(graph->m_data->vertexDataList, func_copyVertex);

    prv_copyEdges(graph->m_data->vertexDataList, copiedVertexDataList);

    m_data = prv_create(&copiedVertexDataList);
}

//------------------------------------------------------------------------------
//
PrvBaseGraph::~PrvBaseGraph(void)
{
    assert_no_null(m_data);

    delete m_data->vertexDataList;

    FREE_T(&m_data, struct prv_baseGraph_t);
}

//------------------------------------------------------------------------------
//
void PrvBaseGraph::addVertex(class IVertex *vertex)
{
    class PrvVertexData *vertexData;

    prv_integrity(m_data);
    //assert(prv_existIVertex(m_data->vertexDataList, vertex, NULL) == false);

    vertexData = new PrvVertexData(vertex);

    m_data->vertexDataList->add(vertexData);
}

//------------------------------------------------------------------------------
//
void PrvBaseGraph::addEdge(
                        const class IVertex *beginVertex,
                        const class IVertex *endVertex)
{
    class PrvVertexData *vertexDataBegin, *vertexDataEnd;

    prv_integrity(m_data);

    vertexDataBegin = prv_getPrvVertexData(m_data->vertexDataList, beginVertex);
    vertexDataEnd = prv_getPrvVertexData(m_data->vertexDataList, endVertex);

    vertexDataBegin->childrenList->add(vertexDataEnd);
    vertexDataEnd->parentsList->add(vertexDataBegin);
}

//------------------------------------------------------------------------------
//
unsigned long PrvBaseGraph::numVertex() const
{
    prv_integrity(m_data);
    return m_data->vertexDataList->size();
}

//------------------------------------------------------------------------------
//
unsigned long PrvBaseGraph::numEdges() const
{
    unsigned long numEdges, numVertex;

    prv_integrity(m_data);

    m_data->vertexDataList->size();
    numEdges = 0;
    numVertex = m_data->vertexDataList->size();

    for( unsigned long ind = 0; ind < numVertex; ind++)
    {
        numEdges += m_data->vertexDataList->get( ind )->childrenList->size();
    }

    return numEdges;
}

//------------------------------------------------------------------------------
//
unsigned long PrvBaseGraph::numInitials() const
{
    unsigned long numInitials, numVertex;

    prv_integrity(m_data);

    numInitials = 0;
    numVertex = m_data->vertexDataList->size();

    for(unsigned long ind = 0; ind < numVertex; ind++)
    {
        const class PrvVertexData *vertextData;

        vertextData = m_data->vertexDataList->get(ind);
        assert_no_null(vertextData);
        assert_no_null(vertextData->parentsList);

        if(vertextData->parentsList->size() == 0 )
            numInitials++;
    }

    return numInitials;
}

//------------------------------------------------------------------------------
//
unsigned long PrvBaseGraph::numFinals() const
{
    unsigned long numFinals, numVertex;

    prv_integrity(m_data);

    numFinals = 0;
    numVertex = m_data->vertexDataList->size();

    for( unsigned long ind = 0; ind < numVertex; ind++)
    {
        const class PrvVertexData *vertextData;

        vertextData = m_data->vertexDataList->get(ind);
        assert_no_null(vertextData);
        assert_no_null(vertextData->childrenList);

        if(vertextData->childrenList->size() == 0 )
            numFinals++;
    }

    return numFinals;
}

//------------------------------------------------------------------------------
//
unsigned long PrvBaseGraph::numParents( const class IVertex *vertex ) const
{
    const class PrvVertexData *vertextData;

    prv_integrity(m_data);

    vertextData = prv_getPrvVertexData(m_data->vertexDataList, vertex);
    return vertextData->parentsList->size();
}

//------------------------------------------------------------------------------
//
unsigned long PrvBaseGraph::numChildren( const class IVertex *vertex ) const
{
    const class PrvVertexData *vertextData;

    prv_integrity(m_data);

    vertextData = prv_getPrvVertexData(m_data->vertexDataList, vertex);
    return vertextData->childrenList->size();
}

//------------------------------------------------------------------------------
//
static unsigned long prv_level_recursive(
                                const class PrvVertexData *vertexData,
                                class CArrayRef<PrvVertexData> *visitedList )
{
    const class CArrayRef<PrvVertexData> *arrayParents;
    unsigned long maxLevel, currentLevel;
    unsigned long numParents;

    maxLevel = CMath::maxULong();
    currentLevel = maxLevel;

    if( visitedList->exist<PrvVertexData>( vertexData, NULL, NULL ) == true )
    {
        return maxLevel;
    }
    visitedList->add( vertexData );

    arrayParents = vertexData->parentsList;

    numParents = arrayParents->size();

    if( numParents == 0 )
    {
        return 0;
    }

    for( unsigned long i = 0; i < numParents; ++i )
    {
        const PrvVertexData *parentVertexData;
        unsigned long level;

        parentVertexData = arrayParents->get( i );

        level = prv_level_recursive( parentVertexData, visitedList );

        if( level < currentLevel )
            currentLevel = level;
    }

    visitedList->eraseLast();

    if( currentLevel == maxLevel )
        return maxLevel;

    return currentLevel + 1;
}

//------------------------------------------------------------------------------
//
unsigned long PrvBaseGraph :: level( const class IVertex *vertex ) const
{
    class CArrayRef<PrvVertexData> *visitedList;
    const class PrvVertexData *vertexData;
    unsigned long level;

    assert_no_null( vertex );
    prv_integrity(m_data);

    vertexData = prv_getPrvVertexData( m_data->vertexDataList, vertex );
    visitedList = new CArrayRef<PrvVertexData>();

    level = prv_level_recursive( vertexData, visitedList );

    delete visitedList;

    if( level == CMath::maxULong() )
        return 0;
    else
        return level;
}

//------------------------------------------------------------------------------
//
unsigned long PrvBaseGraph :: maxLevel() const
{
    unsigned long maxLevel, numVertex;

    maxLevel = 0;
    numVertex = this->numVertex();

    for( unsigned long ind = 0; ind < numVertex; ++ind )
    {
        const class PrvVertexData *vertexData;

        vertexData = m_data->vertexDataList->get( ind );

        if( vertexData->parentsList->size() > 0 )
        {
            unsigned long level;

            level = this->level( vertexData->vertex );

            if( level > maxLevel )
                maxLevel = level;
        }
    }

    return maxLevel;
}

//------------------------------------------------------------------------------
//
class CArrayRef<IVertex> *PrvBaseGraph::getInitials() const
{
    unsigned long numVertex;
    class CArrayRef<IVertex> *arrayInitials;

    prv_integrity(m_data);

    numVertex = m_data->vertexDataList->size();
    arrayInitials = new CArrayRef<IVertex>();

    for( unsigned long ind = 0; ind < numVertex; ++ind )
    {
        const class PrvVertexData *vertexData;

        vertexData = m_data->vertexDataList->get( ind );
        assert_no_null(vertexData);
        assert_no_null(vertexData->parentsList);

        if( vertexData->parentsList->size() == 0 )
            arrayInitials->add( vertexData->vertex );
    }

    return arrayInitials;
}

//------------------------------------------------------------------------------
//
class CArrayRef<IVertex>* PrvBaseGraph::getFinals() const
{
    unsigned long numVertex;
    class CArrayRef<IVertex> *arrayFinals;

    prv_integrity(m_data);

    numVertex = m_data->vertexDataList->size();
    arrayFinals = new CArrayRef<IVertex>();

    for( unsigned long ind = 0; ind < numVertex; ++ind )
    {
        const class PrvVertexData *vertexData;

        vertexData = m_data->vertexDataList->get( ind );
        assert_no_null(vertexData);
        assert_no_null(vertexData->childrenList);

        if( vertexData->childrenList->size() == 0 )
            arrayFinals->add( vertexData->vertex );
    }

    return arrayFinals;
}

//------------------------------------------------------------------------------
//
static class CArrayRef<IVertex> *prv_getArrayIVertex(
                                class CArrayRef<PrvVertexData> *arrayVertexData)
{
    unsigned long numVertex;
    class CArrayRef<IVertex> *ivertexs;

    assert_no_null(arrayVertexData);

    numVertex = arrayVertexData->size();
    ivertexs = new CArrayRef<IVertex>(numVertex);

    for( unsigned long ind = 0; ind < numVertex; ++ind )
    {
        const class PrvVertexData *vertexData;

        vertexData = arrayVertexData->get(ind);
        assert_no_null(vertexData);

        ivertexs->set(ind, vertexData->vertex);
    }

    return ivertexs;
}

//------------------------------------------------------------------------------
//
class CArrayRef<IVertex>* PrvBaseGraph::getVertex(void) const
{
    unsigned long numVertex;
    class CArrayRef<IVertex> *ivertexs;

    prv_integrity(m_data);

    numVertex = m_data->vertexDataList->size();
    ivertexs = new CArrayRef<IVertex>(numVertex);

    for( unsigned long ind = 0; ind < numVertex; ++ind )
    {
        const class PrvVertexData *vertexData;

        vertexData = m_data->vertexDataList->get(ind);
        assert_no_null(vertexData);

        ivertexs->set(ind, vertexData->vertex);
    }

    return ivertexs;
}

//------------------------------------------------------------------------------
//
class CArrayRef<IVertex>* PrvBaseGraph::getParents(const class IVertex *vertex) const
{
    const class PrvVertexData *vertextData;

    prv_integrity(m_data);

    vertextData = prv_getPrvVertexData(m_data->vertexDataList, vertex);

    return prv_getArrayIVertex(vertextData->parentsList);
}

//------------------------------------------------------------------------------
//
class CArrayRef<IVertex>* PrvBaseGraph::getChildren(const class IVertex *vertex) const
{
    const class PrvVertexData *vertextData;

    prv_integrity(m_data);

    vertextData = prv_getPrvVertexData(m_data->vertexDataList, vertex);

    return prv_getArrayIVertex(vertextData->childrenList);
}

//------------------------------------------------------------------------------
//
bool PrvBaseGraph::exist(
                const struct prv_itemSearch_t *itemSearch,
                CGraph_FPtr_search func_search,
                class IVertex **vertex_optional)
{
    unsigned long num;

    prv_integrity(m_data);
    assert_no_null(func_search);

    num = m_data->vertexDataList->size();

    for (unsigned long i = 0; i < num; i++)
    {
        const class PrvVertexData *vertexData;

        vertexData = m_data->vertexDataList->get(i);

        if (func_search(vertexData->vertex, itemSearch) == true)
        {
            if (vertex_optional != NULL)
                *vertex_optional = vertexData->vertex;

            return true;
        }
    }

    return false;
}

//------------------------------------------------------------------------------
//
bool PrvBaseGraph::isVertex(const class IVertex *vertex) const
{
    prv_integrity(m_data);
    return prv_existIVertex(m_data->vertexDataList, vertex, NULL);
}

//------------------------------------------------------------------------------
//
struct prv_equal_mapVertex_t
{
    prv_equal_mapVertex_t( const struct PrvVertexData *vA,
                           const struct PrvVertexData *vB )
    {
        vertexA = vA;
        vertexB = vB;
    }
    const struct PrvVertexData *vertexA;
    const struct PrvVertexData *vertexB;
};

//------------------------------------------------------------------------------
//
static bool prv_equal_searchMap( const struct prv_equal_mapVertex_t *mapVertex,
                                 const struct PrvVertexData *vertexData )
{
    if( mapVertex->vertexA == vertexData )
        return true;
    else
        return false;
}

//------------------------------------------------------------------------------
//
static bool prv_equal_recursive(
                            const class PrvVertexData *vertexDataA,
                            const class PrvVertexData *vertexDataB,
                            class CArray<prv_equal_mapVertex_t> *arrayVisited )
{
    unsigned long numInputsA, numInputsB;
    unsigned long numOutputsA, numOutputsB;

    //DEBUG( tabbing + "a.id:" + longToString(a->GetID()) + ", b.id:" + longToString(b->GetID()) + "\n" );

    numInputsA = vertexDataA->parentsList->size();
    numInputsB = vertexDataB->parentsList->size();
    numOutputsA = vertexDataA->childrenList->size();
    numOutputsB = vertexDataB->childrenList->size();

    if ( vertexDataA->vertex->equal( vertexDataB->vertex ) == false ||
         numInputsA != numInputsB ||
         numOutputsA != numOutputsB )
    {
        //DEBUG( tabbing + "# different aridad in vertex  ->  return false\n");
        return false;
    }

    arrayVisited->add( new prv_equal_mapVertex_t( vertexDataA, vertexDataB ) );

    if ( numOutputsA == 0 )
    {
        //DEBUG( tabbing + "# final vertex  ->  return true\n");
        return true;
    }


    for ( unsigned long indA = 0; indA < numOutputsA; ++indA )
    {
        const class PrvVertexData *childA;
        const struct prv_equal_mapVertex_t *mapVisitedVertex;
        unsigned long ind;
        bool foundMap, equal;

        equal = false;
        mapVisitedVertex = NULL;
        childA = vertexDataA->childrenList->get( indA );

        foundMap = arrayVisited->exist<PrvVertexData>( childA, prv_equal_searchMap, &ind );

        if( foundMap == true )
            mapVisitedVertex = arrayVisited->get( ind );


        for ( unsigned long indB = 0; equal == false && indB < numOutputsB; ++indB )
        {
            const class PrvVertexData *childB;

            childB = vertexDataB->childrenList->get( indB );


            //DEBUG( tabbing + "*itera (" + longToString(a->GetNumOutputs()) + "): ");
            //DEBUG( longToString(a->GetChild( i )->GetID()) + " con " + longToString(b->GetChild( j )->GetID()));
            //DEBUG( " -> isV[a.child:" + longToString(a->GetChild( i )->GetID()) + "]=" + longToString(isVisitedA[a->GetChild( i )->GetID() ]) + "\n");

            if( ( mapVisitedVertex == NULL &&
                  prv_equal_recursive( childA, childB, arrayVisited ) == true )
                ||
                ( mapVisitedVertex != NULL &&
                  mapVisitedVertex->vertexB == childB ) )
            {
                equal = true;
            }

            //			else
            //DEBUG( tabbing + "-> NO\n");
        }

        if( equal == false )
        {
            if( foundMap == true )
            {
                arrayVisited->erase( ind );
                //DEBUG( tabbing + "UNSET\n");
            }

            return false;
        }
    }

    return true;
}

//------------------------------------------------------------------------------
//
static class CArrayRef<PrvVertexData>* prv_getInitialVertexDataList(
                                const class CArray<PrvVertexData> *vertexDataList )
{
    unsigned long numVertex;
    class CArrayRef<PrvVertexData> *arrayInitials;

    numVertex = vertexDataList->size();
    arrayInitials = new CArrayRef<PrvVertexData>();

    for( unsigned long ind = 0; ind < numVertex; ++ind )
    {
        const class PrvVertexData *vertexData;

        vertexData = vertexDataList->get( ind );
        assert_no_null(vertexData);
        assert_no_null(vertexData->parentsList);

        if( vertexData->parentsList->size() == 0 )
            arrayInitials->add( vertexData );
    }

    return arrayInitials;
}

//------------------------------------------------------------------------------
//
bool PrvBaseGraph :: equal( const class PrvBaseGraph *graph ) const
{
    class CArrayRef<PrvVertexData> *arrayInitialsA, *arrayInitialsB;
    const class PrvVertexData *initialVertexDataA, *initialVertexDataB;
    class CArray<prv_equal_mapVertex_t> *arrayVisited;
    unsigned long numVertex, numInitials;
    bool returnValue;

    assert_no_null( graph );

    arrayInitialsA = prv_getInitialVertexDataList( m_data->vertexDataList );
    arrayInitialsB = prv_getInitialVertexDataList( graph->m_data->vertexDataList );

    numVertex = this->numVertex();
    numInitials = arrayInitialsA->size();

    if ( numVertex != graph->numVertex() ||
         numInitials != arrayInitialsB->size() ||
         this->numFinals() != graph->numFinals() ||
         this->numEdges() != graph->numEdges() )
    {
        delete arrayInitialsA;
        delete arrayInitialsB;

        return false;
    }

    assert_with_message( numInitials > 0,
                         "Not allowed graph comparison without initial vertex" );

    if( numInitials == 1 )
    {
        initialVertexDataA = arrayInitialsA->get( 0 );
        initialVertexDataB = arrayInitialsB->get( 0 );
    }
    else
    {
        initialVertexDataA = new PrvVertexData( NULL );
        initialVertexDataB = new PrvVertexData( NULL );

        for( unsigned long ind = 0; ind < numInitials; ++ind )
        {
            initialVertexDataA->childrenList->add( arrayInitialsA->get( ind ) );
            initialVertexDataB->childrenList->add( arrayInitialsB->get( ind ) );
        }
    }

    arrayVisited = new CArray<prv_equal_mapVertex_t>();

    returnValue = prv_equal_recursive( initialVertexDataA, initialVertexDataB,
                                       arrayVisited );


    assert_with_message( returnValue == false || arrayVisited->size() == numVertex,
                         "The graph has isolated or unreachable areas" );


    //if( returnValue == true )
    //{
    //    returnValue = prv_equalCheckForUnreachableAreas( g, checkOnlyStruct, isVisitedA, isVisitedB );
    //}


    if( numInitials > 1 )
    {
        delete initialVertexDataA;
        delete initialVertexDataB;
    }

    delete arrayInitialsA;
    delete arrayInitialsB;
    delete arrayVisited;

    return returnValue;
}

//------------------------------------------------------------------------------
//
static bool prv_isAcyclic_recursive( const class PrvVertexData *vertexData,
                                     class CArrayRef<PrvVertexData> *arrayVisited,
                                     class CArrayRef<PrvVertexData> *arrayCycles)
{
    unsigned long numChildren;

    arrayVisited->add( vertexData );

    numChildren = vertexData->childrenList->size();

    if ( numChildren == 0 )
    {
        return true;
    }

    arrayCycles->add( vertexData );

    for( unsigned long ind = 0; ind < numChildren; ++ind )
    {
        const class PrvVertexData *child;

        child = vertexData->childrenList->get( ind );

        if( arrayCycles->exist<PrvVertexData>( child, NULL, NULL ) == true )
        {
            //DEBUG( " CYCLE !!\n" );
            return false;
        }

        if( arrayVisited->exist<PrvVertexData>( child, NULL, NULL ) == false
            &&
            prv_isAcyclic_recursive( child, arrayVisited, arrayCycles ) == false )
        {
            return false;
        }

    }

    //DEBUG( "UNSET " + IntToString(v->GetID()) + "\n" );
    arrayCycles->eraseLast();

    return true;
}


//------------------------------------------------------------------------------
//
bool PrvBaseGraph :: isAcyclic() const
{
    class CArrayRef<PrvVertexData> *arrayInitials;
    class CArrayRef<PrvVertexData> *arrayVisited;
    class CArrayRef<PrvVertexData> *arrayCycles;
    unsigned long numInitials, numVertex;
    bool isAcyclic;

    prv_integrity(m_data);

    arrayInitials = prv_getInitialVertexDataList( m_data->vertexDataList );
    numInitials = arrayInitials->size();

    numVertex = m_data->vertexDataList->size();

    if( numInitials == 0 && numVertex > 0 )
        return false;

    arrayVisited = new CArrayRef<PrvVertexData>();
    arrayCycles = new CArrayRef<PrvVertexData>();

    isAcyclic = true;

    for ( unsigned long ind = 0; isAcyclic == true && ind < numInitials; ++ind )
    {
        isAcyclic = prv_isAcyclic_recursive( arrayInitials->get( ind ), arrayVisited, arrayCycles );
    }

    if( isAcyclic == true && arrayVisited->size() != numVertex )
        isAcyclic = false;

    delete arrayInitials;
    delete arrayVisited;
    delete arrayCycles;    

    return isAcyclic;
}

/*
//------------------------------------------------------------------------------
// Chequeamos si, aun devolviendo CIERTO, 
// han quedado partes del grafo sin visitar...
static bool prv_equal_checkUnreachableAreas( const class PrvBaseGraph *graph,
        const class CArray<prv_equal_mapVertex_t> *arrayVisited )
{
    bool returnValue = true;

    IVertex *i1 = new IVertex( NULL );
    IVertex *i2 = new IVertex( NULL );

    vector<IVertex*>::iterator itv2 = g->m_data->vertexList.begin();
    long pos = 0;
    for ( vector<IVertex*>::iterator itv1 = m_data->vertexList.begin();
          itv1 != m_data->vertexList.end(); ++itv1, ++itv2, ++pos )
    {
        if ((*isVisitedA)[pos] == -1)
            i1->addAuxiliarOutput(*itv1);

        if ((*isVisitedB)[pos] == -1)
            i2->addAuxiliarOutput(*itv2);
    }

    returnValue = iIsEqualRecursive(i1, i2,
                                    isVisitedA, isVisitedB, "");

    delete i1;
    delete i2;

    return returnValue;
}*/
