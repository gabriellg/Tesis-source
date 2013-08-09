/* -----------------------------------------------------------------------------
*  CSubGraphVertex
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "CSubGraphVertex.hpp"

#include <iostream>
//#include "CFileBinIn.hpp"
//#include "CFileBinOut.hpp"
#include "asrtbas.h"
using namespace std;

struct prv_subGraphVertex_t
{
    const class IVertex *ptrVertex;
    unsigned long labelIn;
    unsigned long labelOut;
};

//------------------------------------------------------------------------------
//
static struct prv_subGraphVertex_t* prv_create( const class IVertex *vertex,
                                                unsigned long labelIn,
                                                unsigned long labelOut )
{
    assert_no_null( vertex );

    struct prv_subGraphVertex_t *data = new prv_subGraphVertex_t;
    assert_no_null( data );

    data->ptrVertex = vertex;
    data->labelIn = labelIn;
    data->labelOut = labelOut;

    return data;
}

//------------------------------------------------------------------------------
//
CSubGraphVertex :: CSubGraphVertex( const class IVertex *vertex,
                                unsigned long labelIn, unsigned long labelOut )
{
    m_data = prv_create( vertex, labelIn, labelOut );
}

//------------------------------------------------------------------------------
//
CSubGraphVertex :: CSubGraphVertex( const class CSubGraphVertex *subGraphVertex )
{
    assert_no_null( subGraphVertex );

    m_data = prv_create( subGraphVertex->m_data->ptrVertex,
                         subGraphVertex->m_data->labelIn,
                         subGraphVertex->m_data->labelOut );
}

//------------------------------------------------------------------------------
//
CSubGraphVertex :: ~CSubGraphVertex()
{
    delete m_data;
}

//------------------------------------------------------------------------------
//
const class IVertex* CSubGraphVertex :: getReferedVertex() const
{
    return m_data->ptrVertex;
}

//------------------------------------------------------------------------------
//
unsigned long CSubGraphVertex :: getLabelOut() const
{   
    return m_data->labelOut;
}

//------------------------------------------------------------------------------
//
unsigned long CSubGraphVertex :: getLabelIn() const
{   
    return m_data->labelIn;
}
/*
//------------------------------------------------------------------------------
//
class IVertex* CSubGraphVertex :: getCopy() const
{
    assert_no_null( m_data );

    CSubGraphVertex *vertex = new CSubGraphVertex( this );

    return vertex;
}*/

//------------------------------------------------------------------------------
//
bool CSubGraphVertex :: equal( const class IVertex *vertex ) const
{
    assert_no_null( vertex );

    const CSubGraphVertex *auxVertex = dynamic_cast<const CSubGraphVertex*>(vertex);
    assert_with_message( auxVertex != NULL,
                         "Vertex must be of type CSubGraphVertex" );

    if ( m_data->ptrVertex->equal( auxVertex ) == true &&
         m_data->labelOut == auxVertex->m_data->labelOut &&
         m_data->labelIn == auxVertex->m_data->labelIn )
    {
        return true;
    }

    return false;
    
    /*
#ifndef CHECK_ONLY_STRUCT
    if (m_data->label == auxVertex->m_data->label ) {
#endif

        if ( m_data->labelOut == auxVertex->m_data->labelOut &&
             m_data->labelIn == auxVertex->m_data->labelIn )
        {
            return true;
        }

#ifndef CHECK_ONLY_STRUCT
    }
#endif

    return false;
*/
}

//------------------------------------------------------------------------------
//
void CSubGraphVertex :: print() const
{
    cout << "{";

    m_data->ptrVertex->print();

    cout << "}";

//    if ( m_data->labelOut != -1 || m_data->labelIn != -1 )
//    {
        cout << "[" << m_data->labelIn;
        cout << "/" << m_data->labelOut << "] ";
//    }
}


/*
//------------------------------------------------------------------------------
//
class IVertex* CSubGraphVertex :: read( class CFileTxtIn *file ) const
{
    assert_no_null( file );

    char c = file->readChar();
    return new CVertex( c );
}

//------------------------------------------------------------------------------
//
void CSubGraphVertex :: write( class CFileTxtOut *file ) const
{
    assert_no_null( file );
    file->writeChar( m_data->label );
}*/
/*
//------------------------------------------------------------------------------
//
class IVertex* CSubGraphVertex :: read( 
                                class CFileBinIn *file,
                                class IVertex* (*func_readVertex)( class CFileBinIn *file )) const
{
    class IVertex *vertex;
    unsigned long labelIn;
    unsigned long labelOut;

    assert_no_null( file );

    vertex = func_readVertex( file );

    labelIn = file->readULong();
    labelOut = file->readULong();

    return new CSubGraphVertex( vertex, labelIn, labelOut );
}

//------------------------------------------------------------------------------
//
void CSubGraphVertex :: write( class CFileBinOut *file ) const
{
    assert_no_null( file );

    m_data->ptrVertex->write( file );

    file->writeULong( m_data->labelIn );
    file->writeULong( m_data->labelOut );
}
*/
