/* -----------------------------------------------------------------------------
*  CNode
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "CNode.hpp"

#include <iostream>
#include <typeinfo>
#include "asrtbas.h"
using namespace std;

struct prv_node_t
{
    char label;
};

//------------------------------------------------------------------------------
//
static struct prv_node_t* prv_create( char label )
{
    struct prv_node_t *data;

    data = new prv_node_t;
    assert_no_null( data );

    data->label = label;

    return data;
}

//------------------------------------------------------------------------------
//
CNode :: CNode( char label )
{
    m_data = prv_create( label );
}

//------------------------------------------------------------------------------
//
CNode :: CNode( const CNode *node )
{
    assert_no_null( node );

    m_data = prv_create( node->m_data->label );
}

//------------------------------------------------------------------------------
//
CNode :: ~CNode()
{
    //cout << "delete CNode" << endl;
    delete m_data;
}

//------------------------------------------------------------------------------
//
char CNode :: getLabel() const
{   
    return m_data->label;
}

//------------------------------------------------------------------------------
//
bool CNode :: equal( const class INode *node ) const
{
    assert_no_null( node );

    const CNode *auxnode = dynamic_cast<const CNode*>(node);
    assert_with_message( auxnode != NULL,
                         "node must be of type CNode" );

    return ( m_data->label == auxnode->m_data->label );
}

//------------------------------------------------------------------------------
//
void CNode :: print() const
{
    cout << m_data->label;
}
