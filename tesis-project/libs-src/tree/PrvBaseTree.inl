/* -----------------------------------------------------------------------------
*  PrvBaseTree
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#ifndef PRVBASETREE_HPP_
#define PRVBASETREE_HPP_

template<class> class CArrayRef;
typedef class INode* (*CTree_FPtr_copyNode)(const class INode*);
typedef bool (*CTree_FPtr_search)(const class INode*, const struct prv_itemSearch_t*);

class PrvBaseTree
{
    protected:
        PrvBaseTree();
        PrvBaseTree( const class PrvBaseTree *tree,
                     CTree_FPtr_copyNode func_copyNode );
        ~PrvBaseTree();

        unsigned long numNodes() const;
        unsigned long numLevels() const;
        unsigned long numChildren( const class INode *node ) const;

        bool isLeaf( const class INode *node ) const;
        bool isRoot( const class INode *node ) const;

        void setRoot( const class INode *root );
        void addChild( const class INode *parent, const class INode *node );

        const class INode* getRoot() const;
        const class INode* getParent( const class INode *node ) const;
        const class INode* getChild( const class INode *node, unsigned long ind ) const;
        class CArrayRef<INode>* getChildren( const class INode *node ) const;

        class CArrayRef<INode>* getNodesPreorder() const;
        class CArrayRef<INode>* getNodesInorder() const;
        class CArrayRef<INode>* getNodesPostorder() const;

        bool exist( const struct prv_itemSearch_t *itemSearch,
                        CTree_FPtr_search func_search_optional ) const;

        bool equal( const class PrvBaseTree *tree ) const;

    private:
        struct prv_baseTree_t *m_data;
};

#endif