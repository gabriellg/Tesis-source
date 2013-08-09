/* -----------------------------------------------------------------------------
*  CNode (Tree Node)
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#ifndef CNODE_HPP_
#define CNODE_HPP_

#include "INode.hpp"

class CNode : public INode
{
    public:
        CNode( char label );
        CNode( const CNode *node );
        ~CNode();

        char getLabel() const;

        bool equal( const class INode *node ) const;

        void print() const;

    private:
        struct prv_node_t *m_data;
};

#endif