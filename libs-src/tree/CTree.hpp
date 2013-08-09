/* -----------------------------------------------------------------------------
*  CTree
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "PrvBaseTree.inl"

template<class TNode> class CTree : public PrvBaseTree
{
    public:
        CTree( void );
        CTree( const class CTree *tree );

        unsigned long numNodes() const;
        unsigned long numLevels() const;
        unsigned long numChildren( const TNode *node ) const;

        bool isLeaf( const TNode *node ) const;
        bool isRoot( const TNode *node ) const;

        void setRoot( const TNode *root );
        void addChild( const TNode *parent, const TNode *node );

        const TNode* getRoot() const;
        const TNode* getParent( const TNode *node ) const;
        class CArrayRef<TNode>* getChildren( const TNode *node ) const;

        class CArrayRef<TNode>* getNodesPreorder() const;
        class CArrayRef<TNode>* getNodesInorder() const;
        class CArrayRef<TNode>* getNodesPostorder() const;

        template<class TypeSearch> inline bool exist(
                                const TypeSearch *itemSearch,
                                bool (*func_search_optional)(const TNode*, const TypeSearch*)) const;

        inline bool equal( const class CTree *tree ) const;

    private:
        inline static class INode* prv_copyNode( const class INode *node );
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//
template<class TNode> CTree<TNode>::CTree(void)
: PrvBaseTree()
{
    ;
}

//------------------------------------------------------------------------------
//
template<class TNode> CTree<TNode>::CTree( const class CTree *node )
: PrvBaseTree( node, prv_copyNode )
{
    ;
}

//------------------------------------------------------------------------------
//
template<class TNode>
unsigned long CTree<TNode>::numNodes() const
{
    return PrvBaseTree::numNodes();
}

//------------------------------------------------------------------------------
//
template<class TNode>
unsigned long CTree<TNode>::numLevels() const
{
    return PrvBaseTree::numLevels();
}

//------------------------------------------------------------------------------
//
template<class TNode>
unsigned long CTree<TNode>::numChildren( const TNode *node ) const
{
    return PrvBaseTree::numChildren( node );
}

//------------------------------------------------------------------------------
//
template<class TNode>
bool CTree<TNode>::isLeaf( const TNode *node ) const
{
    return PrvBaseTree::isLeaf( node );
}

//------------------------------------------------------------------------------
//
template<class TNode>
bool CTree<TNode>::isRoot( const TNode *node ) const
{
    return PrvBaseTree::isRoot( node );
}

//------------------------------------------------------------------------------
//
template<class TNode>
void CTree<TNode>::setRoot( const TNode *root )
{
    PrvBaseTree::setRoot( root );
}

//------------------------------------------------------------------------------
//
template<class TNode>
void CTree<TNode>::addChild( const TNode *parent, const TNode *node )
{
    PrvBaseTree::addChild( parent, node );
}

//------------------------------------------------------------------------------
//
template<class TNode>
const TNode* CTree<TNode>::getRoot() const
{
    return (const TNode*)PrvBaseTree::getRoot();
}

//------------------------------------------------------------------------------
//
template<class TNode>
const TNode* CTree<TNode>::getParent( const TNode *node ) const
{
    return (const TNode*)PrvBaseTree::getParent( node );
}

//------------------------------------------------------------------------------
//
template<class TNode>
class CArrayRef<TNode>* CTree<TNode>::getChildren( const TNode *node ) const
{
    return (class CArrayRef<TNode>*)PrvBaseTree::getChildren( node );
}

//------------------------------------------------------------------------------
//
template<class TNode>
class CArrayRef<TNode>* CTree<TNode>::getNodesPreorder() const
{
    return (class CArrayRef<TNode>*)PrvBaseTree::getNodesPreorder();
}

//------------------------------------------------------------------------------
//
template<class TNode>
class CArrayRef<TNode>* CTree<TNode>::getNodesInorder() const
{
    return (class CArrayRef<TNode>*)PrvBaseTree::getNodesInorder();
}

//------------------------------------------------------------------------------
//
template<class TNode>
class CArrayRef<TNode>* CTree<TNode>::getNodesPostorder() const
{
    return (class CArrayRef<TNode>*)PrvBaseTree::getNodesPostorder();
}

//------------------------------------------------------------------------------
//
template<class TNode> template<class TypeSearch>
bool CTree<TNode>::exist(
                            const TypeSearch *itemSearch,
                            bool (*func_search_optional)(const TNode*, const TypeSearch*) ) const
{
    return PrvBaseTree::exist(
                            (const struct prv_itemSearch_t*)itemSearch,
                            (CTree_FPtr_search)func_search_optional ) ;
}

//------------------------------------------------------------------------------
//
template<class TNode>
bool CTree<TNode>::equal( const class CTree *tree ) const
{
    return PrvBaseTree::equal( tree );
}

//------------------------------------------------------------------------------
//
template<class TNode>
class INode* CTree<TNode>::prv_copyNode( const class INode *node )
{
    return (class INode*) new TNode( (const TNode*)node );
}
