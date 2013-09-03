/* -----------------------------------------------------------------------------
*  INode (Tree Node Interface)
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#ifndef INODE_HPP_
#define INODE_HPP_

class INode
{
    public:
        virtual ~INode() {;}

        virtual bool equal( const class INode *node ) const = 0;

        virtual void print() const = 0;
};

#endif
