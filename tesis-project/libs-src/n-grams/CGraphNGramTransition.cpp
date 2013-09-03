/* -----------------------------------------------------------------------------
* CGraphNGramTransition
* Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
* License: GNU Public License
* Description: A transition in graph's n-grams is composed by: an initial state,
*              a multiset of subgraphs, and a final state (subgraph).
* ----------------------------------------------------------------------------*/

#include "CGraphNGramTransition.hpp"

#include <iostream>
#include "IVertex.hpp"
//#include "CGraph.h"
#include "CSubGraph.hpp"
//#include "CFileBinIn.hpp"
//#include "CFileBinOut.hpp"
#include "CArrayRef.hpp"
#include "asrtbas.h"
using namespace std;

struct prv_transition_t
{
    //char initialLabel;

    const class IVertex* initialVertex;
    unsigned long numInitialParents;
    unsigned long numInitialChildren;
    const class CArrayRef<CSubGraph> *multiset;
    const class CSubGraph *finalState;

    unsigned long numTimesRepeated;
};

//------------------------------------------------------------------------------
//
static struct prv_transition_t* prv_create(
                                    const class IVertex* initialVertex,
                                    unsigned long numInitialParents,
                                    unsigned long numInitialChildren,
                                    const class CArrayRef<CSubGraph> *multiset,
                                    const class CSubGraph *finalState,
                                    unsigned long numTimesRepeated )
{
    assert_no_null( initialVertex );
    assert_no_null( multiset );
    assert_no_null( finalState );

    struct prv_transition_t *data;

    data = new prv_transition_t;
    assert_no_null( data );

    data->initialVertex = initialVertex;
    data->numInitialParents = numInitialParents;
    data->numInitialChildren = numInitialChildren;
    data->multiset = multiset;
    data->finalState = finalState;
    data->numTimesRepeated = numTimesRepeated;

    return data;
}
/*
//------------------------------------------------------------------------------
//
CGraphNGramTransition::CGraphNGramTransition(
                            class CFileBinIn *file,
                            CGraphNGramTransition_FPtr_loadVertex func_loadVertex )
{
    assert_no_null( file );

    class IVertex* initialVertex;
    unsigned long numInitialParents;
    unsigned long numInitialChildren;
    unsigned long multisetSize;
    class CArrayRef<CSubGraph> *multiset;
    class CSubGraph *finalState;
    unsigned long numTimesRepeated;

    initialVertex = func_loadVertex( file );
    numInitialParents = file->readULong();
    numInitialChildren = file->readULong();
    numTimesRepeated = file->readULong();

    multisetSize = file->readULong();

    //cout << "  CGraphNGramTransition: " << mInitialLabel << " " << mInitialInputs << " ";
    //cout << mInitialOutputs << " " << mNumTimesRepeated << " " << multisetSize << endl;

    multiset = new CArrayRef<CSubGraph>( multisetSize );

    for( unsigned long ind = 0; ind < multisetSize; ++ind )
    {
        class CSubGraph *subgraph;

        subgraph = new CSubGraph( file );

        multiset->set( ind, subgraph );
    }

    finalState = new CSubGraph( file );


    m_data = prv_create(   initialVertex,
                           numInitialParents,
                           numInitialChildren,
                           multiset,
                           finalState,
                           numTimesRepeated );
}*/

//------------------------------------------------------------------------------
//
CGraphNGramTransition::CGraphNGramTransition(
                         const class IVertex *initialVertex,
                         unsigned long numInitialParents,
                         unsigned long numInitialChildren,
                         const class CArrayRef<class CSubGraph> *multiset,
                         const class CSubGraph *finalGraph )
{
/*	mInitialVertex = new Vertex( v->GetID(), 
			   v->GetLabel(),
			   v->GetNumInputs(), 
			   v->GetNumOutputs());
*/
    m_data = prv_create( initialVertex,
                        numInitialParents,
                        numInitialChildren,
                        multiset,
                        finalGraph,
                        1  //numTimesRepeated
                       );

    /*mInitialLabel = v->GetLabel();
    mInitialInputs = v->GetNumInputs();
    mInitialOutputs = v->GetNumOutputs();

    mNumTimesRepeated = 1;

    mMultiset = m;
    mFinalState = f;*/
}


//------------------------------------------------------------------------------
//
CGraphNGramTransition::~CGraphNGramTransition()
{
    assert_no_null( m_data );

    //delete initialVertex;
    //delete finalState;
				
    delete m_data->multiset;
    delete m_data;
}

/*//------------------------------------------------------------------------------
//
char CGraphNGramTransition::initialLabel() const
{
    return m_data->initialLabel;
}*/

//------------------------------------------------------------------------------
//
unsigned long CGraphNGramTransition::numInitialParents() const
{
    return m_data->numInitialParents;
}

//------------------------------------------------------------------------------
//
unsigned long CGraphNGramTransition::numInitialChildren() const
{
    return m_data->numInitialChildren;
}

//------------------------------------------------------------------------------
//
double CGraphNGramTransition::getProbability() const
{
    double prob;

    prob = (double)m_data->numTimesRepeated
           /
           (double)m_data->finalState->numTimesRepeated();

    return prob;
}

//------------------------------------------------------------------------------
//
unsigned long CGraphNGramTransition::numTimesRepeated() const
{
    return m_data->numTimesRepeated;
}

//------------------------------------------------------------------------------
//
void CGraphNGramTransition::incrementNumTimesRepeated()
{
    ++m_data->numTimesRepeated;
}

//------------------------------------------------------------------------------
//
bool CGraphNGramTransition::equal( const class CGraphNGramTransition *transition ) const
{
    assert_no_null( transition );

    unsigned long multisetSize;

    if( //m_data->initialLabel != transition->m_data->initialLabel ||
        m_data->numInitialParents != transition->m_data->numInitialParents ||
        m_data->numInitialChildren != transition->m_data->numInitialChildren )
    {
        return false;
    }

    if( m_data->finalState->equal( transition->m_data->finalState ) == false )
    {
        return false;
    }

    multisetSize = m_data->multiset->size();

    if( multisetSize != transition->m_data->multiset->size() )
    {
        return false;
    }

    if( multisetSize > 0 )
    {
        bool equal;
        bool *used;

        used = new bool[ multisetSize ];
        for( unsigned long i = multisetSize - 1; i >= 0; --i )
            used[i] = false;

        for( unsigned long ind1 = 0; ind1 < multisetSize; ++ind1 )
        {
            equal = false;

            for( unsigned long ind2 = 0;
                 equal == false && ind2 < multisetSize; ++ind2 )
            {
                const class CSubGraph *subgraph1, *subgraph2;

                subgraph1 = m_data->multiset->get( ind1 );
                subgraph2 = transition->m_data->multiset->get( ind2 );

                if( used[ ind2 ] == false &&
                    subgraph1->equal( subgraph2 ) )
                {
                    equal = true;
                    used[ ind2 ] = true;
                }
            }

            if( equal == false )
                return false;
        }

        delete [] used;
    }

    return true;
}

//------------------------------------------------------------------------------
//
void CGraphNGramTransition::print() const
{
    unsigned long multisetSize;

    cout << "CGraphNGramTransition Rule:   ";

    cout << "(Prob: " << m_data->numTimesRepeated << "/";
    cout << m_data->finalState->numTimesRepeated() << ")" << endl;

    cout << "> Initial: ";
    //mInitialVertex->Print();
//    cout << "Label(" << m_data->initialLabel << ") ";
    cout << "[" << m_data->numInitialParents;
    cout << "/" << m_data->numInitialChildren << "] ";
    cout << endl;

    multisetSize = m_data->multiset->size();

    cout << "> Multiset (" << multisetSize << "): " << endl;

    for( unsigned long ind = 0; ind < multisetSize; ++ind )
    {
        cout << " ";
        m_data->multiset->get( ind )->print();
    }

    cout << "> Final: ";
    m_data->finalState->print();
}
/*
//------------------------------------------------------------------------------
//
void CGraphNGramTransition::writeToBinaryStream( class CFileBinOut *file ) const
{
    assert_no_null( file );

    unsigned long multisetSize;

    multisetSize = m_data->multiset->size();

    file->writeChar( m_data->initialLabel );
    file->writeULong( m_data->numInitialParents );
    file->writeULong( m_data->numInitialChildren );
    file->writeULong( m_data->numTimesRepeated );

    file->writeULong( multisetSize );

    for( unsigned long ind = 0; ind < multisetSize; ++ind )
    {
        m_data->multiset->get( ind )->WriteToBinaryStream( file );
    }

    m_data->finalState->write( file );
}*/
