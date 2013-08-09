/* -----------------------------------------------------------------------------
*  CVertex
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "CVertex.hpp"

#include <iostream>
#include <typeinfo>
#include "IFileIn.hpp"
#include "IFileOut.hpp"
#include "asrtbas.h"
using namespace std;

struct prv_vertex_t
{
    char label;

    //float lambda;
    //int prob[2];   // [0] -> numerator, [1] -> denominator
};

//------------------------------------------------------------------------------
//
static struct prv_vertex_t* prv_create( char label )
{
    struct prv_vertex_t *data = new prv_vertex_t;
    assert_no_null( data );

    data->label = label;

    return data;
}

//------------------------------------------------------------------------------
//
CVertex :: CVertex( char label )
{
    m_data = prv_create( label );
}

//------------------------------------------------------------------------------
//
CVertex :: CVertex( const CVertex *vertex )
{
    assert_no_null( vertex );

    m_data = prv_create( vertex->m_data->label );
}

//------------------------------------------------------------------------------
//
CVertex :: ~CVertex()
{
    delete m_data;
}

//------------------------------------------------------------------------------
//
char CVertex :: getLabel() const
{
    return m_data->label;
}

//------------------------------------------------------------------------------
//
bool CVertex :: equal( const class IVertex *vertex ) const
{
    const CVertex *auxVertex;

    assert_no_null( vertex );

    auxVertex = dynamic_cast<const CVertex*>(vertex);
    assert_with_message(auxVertex != NULL, "Vertex must be of type CVertex");

    if (m_data->label == auxVertex->m_data->label)
        return true;
    else
        return false;
}

//------------------------------------------------------------------------------
//
void CVertex::print() const
{
    cout << this->getLabel();
}

//------------------------------------------------------------------------------
//
class CVertex *CVertex::read( class IFileIn *in )
{
    char label;

    assert_no_null(in);

    label = in->readChar();
    return new CVertex(label);
}

//------------------------------------------------------------------------------
//
void CVertex::write(const class CVertex *vertex, class IFileOut *out)
{
    assert_no_null(vertex);
    assert_no_null(out);

    out->writeChar(vertex->m_data->label);
}
