/* -----------------------------------------------------------------------------
*  CGraphNGram
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "CGraphNGram.hpp"

#include <iostream>
//#include <stdlib.h>

#include "CGraphNGramTransition.hpp"
#include "IVertex.hpp"
#include "CGraph.hpp"
#include "CSubGraph.hpp"
#include "CArray.hpp"
#include "CArrayRef.hpp"
//#include "CFileBinIn.hpp"
//#include "CFileNameHelper.hpp"
#include "asrtbas.h"
using namespace std;


struct prv_graphNGram_t
{
    unsigned long lengthK;
    unsigned long numGraphsLoaded;
    //vector<string> mClasses;

    // mMapTransitions y mTransitions contienen la misma información:
    // la lista de transiciones del autómata, pero ordenada de diferente forma
    // para acelerar el proceso de búsqueda
    // map [ label ] [ nInputs ] [ nOutputs ] = vector<Transition*>
    //map< char, map< int, map< int, vector<Transition*> > > > mMapTransitions;

    class CArray<CGraphNGramTransition> *transitionList;

    // mSubGraphList: contiene la lista de todos subGrafos que se usan en las
    // transiciones. Cuando se añade una transicion hay que comprobar si sus
    // subgrafos están en esta lista, y si es así se han de cambiar los punteros.
    // De esta forma se ahorra mucho espacio (hay muchos subgrafos repetidos)
    // y además es necesario para calcular las probabilidades.
    class CArray<CSubGraph> *subGraphList;
};

//------------------------------------------------------------------------------
//
static struct prv_graphNGram_t* prv_create(
                                unsigned long lengthK,
                                unsigned long numGraphsLoaded,
                                class CArray<CGraphNGramTransition> *transitionList,
                                class CArray<CSubGraph> *subGraphList )
{
    struct prv_graphNGram_t *data;
    
    data = new prv_graphNGram_t;
    assert_no_null( data );

    data->lengthK = lengthK;
    data->numGraphsLoaded = numGraphsLoaded;
    data->transitionList = transitionList;
    data->subGraphList = subGraphList;

    return data;
}

//------------------------------------------------------------------------------
//
static void prv_addTransition( const class CGraphNGramTransition *transition,
                               class CArray<CGraphNGramTransition> *transitionList )
{
    /*mMapTransitions[ t->GetInitialLabel() ]
                   [ t->GetInitialInputs() ]
                   [ t->GetInitialOutputs() ].push_back( t );
*/
    assert_no_null( transition );
    assert_no_null( transitionList );

    transitionList->add( (class CGraphNGramTransition *)(void *)transition );
}
/*
//------------------------------------------------------------------------------
//
static bool prv_isRepeatedSubGraph(
                                const class CArray<CSubGraph> *array,
                                const class CSubGraph *subgraph,
                                unsigned long *ind_subgraph )
{
    unsigned long numElements;

    numElements = array->size();

    for( unsigned long ind = 0; ind < numElements; ++ind )
    {
        if ( subgraph->equal( array->get( ind ) ) )
        {
            *ind_subgraph = ind;
            return true;
        }
    }

    return false;
}*/
/*
//------------------------------------------------------------------------------
//
static void prv_addSubgraph( class CArray<CSubGraph> *subGraphList,
                             class CSubGraph **subgraph,
                             bool incrementRepeated )
{
    unsigned long ind_subgraph;
    bool isRepeated;

    assert_no_null( subgraph );

    isRepeated = prv_isRepeatedSubGraph( subGraphList, *subgraph, &ind_subgraph );

    if( isRepeated == false )
    {
        subGraphList->add( *subgraph );
    }
    else
    {
        delete *subgraph;

        *subgraph = subGraphList->get( ind_subgraph );

        if( incrementRepeated == true )
            (*subgraph)->incrementNumTimesRepeated();
    }
}*/
/*
//------------------------------------------------------------------------------
//
static void prv_addSubGraphList( const class CGraphNGramTransition *transition )
{
    prv_addSubgraph( transition->mFinalState, false );

    for( unsigned int i = 0; i < transition->mMultiset.size(); ++i )
    {
        prv_addSubgraph( transition->mMultiset[i], false );
    }
}*/
/*
//------------------------------------------------------------------------------
//
static void prv_loadAutomatonFromFile( const char* filename )
{
    assert( CFileNameHelper::compareExtension( filename, "atm" ) == true );

    CFileBinIn *file;
    unsigned long lenghtK;
    unsigned long numGraphsLoaded;
    unsigned long numTransitions;

    file = new CFileBinIn( filename );

    lenghtK = file->readULong();
    numGraphsLoaded = file->readULong();
    numTransitions = file->readULong();

    //cout << "Automata: " << mKvalue << " " << mNumGraphsLoaded << " " << numTransitions << endl;

    for ( unsigned long i = 0; i < numTransitions; ++i )
    {
        class CTransition *transition;

        transition = new CTransition( file );

        prv_addSubGraphList( transition );

        prv_addTransition( transition );
    }

    delete file;
}

//------------------------------------------------------------------------------
//
CGraphNGram :: CGraphNGram( const char* filename )
{
    prv_loadAutomatonFromFile( filename );
}*/

//------------------------------------------------------------------------------
//
CGraphNGram::CGraphNGram( unsigned long lengthK )
{
    class CArray<CGraphNGramTransition> *transitionList;
    class CArray<CSubGraph> *subGraphList;

    assert( lengthK >= 2 );

    transitionList = new CArray<CGraphNGramTransition>();
    subGraphList = new CArray<CSubGraph>();

    m_data = prv_create( lengthK,
                         0, // numGraphsLoaded
                         transitionList,
                         subGraphList );
}

//------------------------------------------------------------------------------
//
CGraphNGram::~CGraphNGram()
{
    assert_no_null( m_data );

    delete m_data->transitionList;
    delete m_data->subGraphList;
    delete m_data;
}

//------------------------------------------------------------------------------
//
unsigned long CGraphNGram::lengthK() const
{
    return m_data->lengthK;
}

//------------------------------------------------------------------------------
//
unsigned long CGraphNGram::numOfTransitions() const
{
    return m_data->transitionList->size();
}

//------------------------------------------------------------------------------
//
unsigned long CGraphNGram::numGraphsLoaded() const
{
    return m_data->numGraphsLoaded;
}
/*
//------------------------------------------------------------------------------
//
unsigned long CGraphNGram::numClasses() const
{
    return m_data->classesList->size();
}*/

//------------------------------------------------------------------------------
//
static class CGraphNGramTransition* prv_isRepeatedTransition(
                                const class CArray<CGraphNGramTransition> *transitionList,
                                const class CGraphNGramTransition *newTransition )
{
    unsigned long numTransitions;
/*
    vector<Transition*> &aux = mMapTransitions[ t->GetInitialLabel() ]
                                              [ t->GetInitialInputs() ]
                                              [ t->GetInitialOutputs() ];
*/
    numTransitions = transitionList->size();

    for ( unsigned long nt = 0; nt < numTransitions; ++nt )
    {
        class CGraphNGramTransition *transition;

        transition = transitionList->get( nt );

        if( newTransition->equal( transition ) )
        {
            return transition;
        }
    }

    return NULL;
}

//------------------------------------------------------------------------------
//
static class CSubGraph* prv_extractTransitionRules_recursive(
                                const class CGraph<IVertex> *graph,
                                const class IVertex *fromVertex,
                                unsigned long lengthK,
                                class CArrayRef<IVertex> *visitedList,
                                class CArray<CGraphNGramTransition> *transitionList )
{
    class CArrayRef<IVertex> *childrenList;
    class CArrayRef<CSubGraph> *multiset;
    unsigned long numChildren;

    visitedList->add( fromVertex );

    multiset = new CArrayRef<CSubGraph>();
    childrenList = graph->getChildren( fromVertex );
    numChildren = childrenList->size();

    // 1 - CREATE THE MULTISET OF THIS TRANSITION
    for( unsigned long ind = 0; ind < numChildren; ++ind )
    {
        const class IVertex* child;
        const class CSubGraph* subgraph;

        child = childrenList->get( ind );

        if( visitedList->exist<IVertex>( child, NULL, NULL ) == false )
        {
            subgraph = prv_extractTransitionRules_recursive( graph, child, lengthK, 
                                                             visitedList, transitionList );
        }
        else
        {
            subgraph = new CSubGraph( graph, child, lengthK - 1 );
        }

        multiset->add( subgraph );
    }


    // 2 - CALCULATE THE CURRENT SUB-GRAPH
    CSubGraph *sg = new CSubGraph( graph, fromVertex, lengthK - 1 );
//    prv_addSubgraph( sg, true );  // incrementRepeated = true


    // 3 - CREATE A NEW TRANSITION
    class CGraphNGramTransition *t;
    class CGraphNGramTransition *tRepeat;

    t = new CGraphNGramTransition(  fromVertex,
                                    graph->numParents( fromVertex ),
                                    graph->numChildren( fromVertex ),
                                    multiset, sg );

    tRepeat = prv_isRepeatedTransition ( transitionList, t );

    if( tRepeat == NULL )
    {
            prv_addTransition( t, transitionList );

/*
cout << "******************************************************************" << endl;
cout << "\nNEW TRANSITION" << endl;
t->Print();
*/
    }
    else
    {
            tRepeat->incrementNumTimesRepeated();

/*
cout << "******************************************************************" << endl;
cout << "\nREPEATED TRANSITION!!!!!!!!!" << endl;
t->Print();
cout << "-" << endl;
tRepeat->Print();
*/
            delete t;
    }

    delete childrenList;

    return sg;
}

/*//------------------------------------------------------------------------------
//
static bool prv_existClass( const char* className )
{
    for ( vector<string>::const_iterator it = mClasses.begin();
          it != mClasses.end(); ++it)
    {
        if( *it == className )
                return true;
    }
    return false;
}

//------------------------------------------------------------------------------
//
static void prv_addNonRepeatedClasses( const char* className )
{
    assert( className.length() < LABEL_SIZE );

    if( prv_existClass( className ) == false )
    {
        mClasses.push_back( className );
    }
}*/

//------------------------------------------------------------------------------
//
void CGraphNGram::extractTransitionRules( const class CGraph<IVertex>* graph )
{
    class CArrayRef<IVertex> *visitedList;
    class CArrayRef<IVertex> *initialVertexList;
    const class IVertex *initialVertex;

    assert_no_null( graph );
    //g->CheckIntegrity();

    visitedList = new CArrayRef<IVertex>();

    ++m_data->numGraphsLoaded;
    //iAddNonRepeatedClasses( g->GetLabel() );

    initialVertexList = graph->getInitials();
    assert( initialVertexList->size() == 1 );

    initialVertex = initialVertexList->get( 0 );

    prv_extractTransitionRules_recursive( graph,
                                          initialVertex,
                                          m_data->lengthK,
                                          visitedList,
                                          m_data->transitionList );

/*
    map<int, bool> isVisited;
    for ( vector<CVertex*>::const_iterator it = g->mVertex.begin();
          it != g->mVertex.end(); ++it )
    {
            isVisited[ (*it)->GetID() ] = false;
    }


    for ( vector<CVertex*>::const_iterator it = g->mVertex.begin();
          it != g->mVertex.end(); ++it )
//    for( int i = g->GetNumInitials() - 1; i >= 0; --i )
    {
        if( (*it)->GetNumInputs() == 0 )
            prv_extractTransitionRules_recursive( g, *it, visitedList );
    }*/

    delete visitedList;
    delete initialVertexList;
}

//------------------------------------------------------------------------------
//
bool CGraphNGram::isAcceptedGraph( const class CGraph<IVertex>* graph,
                                double *prob_optional ) const
{
    class CGraphNGram *newAutomaton;
    unsigned long numNewTransitions;
    bool isAccepted;
    double calculatedProb;

    assert_no_null( graph );
    //g->CheckIntegrity();

    if( graph->numInitials() == 0 )
    {
        if( prob_optional != NULL )
            *prob_optional = 0.0;

        return false;
    }

    newAutomaton = new CGraphNGram( m_data->lengthK );

    newAutomaton->extractTransitionRules( graph );

    isAccepted = true;
    calculatedProb = 1.0;

    numNewTransitions = newAutomaton->m_data->transitionList->size();

    for ( unsigned long nt = 0; nt < numNewTransitions; ++nt )
    {
        const class CGraphNGramTransition *newTransition;
        const class CGraphNGramTransition *transition;

        newTransition = newAutomaton->m_data->transitionList->get( nt );

        transition = prv_isRepeatedTransition( m_data->transitionList,
                                               newTransition );

        if( transition == NULL )
        {
            isAccepted = false;
            calculatedProb = 0.0;
            break;
        }

        calculatedProb *= (double)newTransition->numTimesRepeated()
                          *
                          transition->getProbability();

//cout << newTransition->numTimesRepeated() << " * ";
//cout << transition->getProbability() << " = " << calculatedProb << endl;
    }

    *prob_optional = calculatedProb;

    delete newAutomaton;

    return isAccepted;
}

//------------------------------------------------------------------------------
//
void CGraphNGram::print( ) const
{
    unsigned long numTransitions;

    cout << "K value: " << this->lengthK() << endl;
//    cout << "Num Clases: " << this->numClasses() << endl;
    cout << "Num Graphs loaded: " << this->numGraphsLoaded() << endl;
    cout << "Num of Transitions Rules: " << this->numOfTransitions() << endl;
    cout << "List of Transition Rules: " << endl;

    numTransitions = m_data->transitionList->size();

    for ( unsigned long nt = 0; nt < numTransitions; ++nt )
    {
        m_data->transitionList->get( nt )->print();
        cout << endl;
    }
}

//------------------------------------------------------------------------------
//
void CGraphNGram::testProbabilityIsEqualTo1() const
{
    unsigned long numTransitions;
    double prob;

    prob = 1.0;
    numTransitions = m_data->transitionList->size();

    for ( unsigned long nt = 0; nt < numTransitions; ++nt )
    {
	prob *= m_data->transitionList->get( nt )->getProbability();
    }

    cout << "Probability: " << prob << endl;
}

/*
//------------------------------------------------------------------------------
//
void CGraphNGram::writeToFile( const char* filename ) const
{
    assert( CFileNameHelper::compareExtension( filename, "atm" ) == true );

    ofstream fout( file.c_str(), ios::binary);
    if( !fout.is_open() ) {
            cerr << "ERROR: The file '" << file << "' can not be opened." << endl;
            exit(-1);
    }


    fout.write((char *)(&mKvalue), sizeof( mKvalue ));
    fout.write((char *)(&mNumGraphsLoaded), sizeof( mNumGraphsLoaded ));

    int numTransitions = mTransitions.size();
    fout.write((char *)(&numTransitions), sizeof( numTransitions ));

    for (vector<Transition*>::iterator it = mTransitions.begin();
             it != mTransitions.end(); ++it)
    {
            (*it)->WriteToBinaryStream( fout );
    }


    fout.close();
}*/
