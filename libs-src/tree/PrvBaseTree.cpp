/* -----------------------------------------------------------------------------
*  PrvBaseTree
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "PrvBaseTree.inl"

#include "INode.hpp"
#include "CMath.hpp"
#include "CArray.hpp"
#include "CArrayRef.hpp"
#include "asrtbas.h"

class PrvNodeData
{
    public:
        PrvNodeData( const class INode *node, const class PrvNodeData *parentData )
        {
            this->node = (INode*)node;
            this->parentData = (PrvNodeData*)parentData;
            this->childrenDataList = new CArrayRef<PrvNodeData>();
        };
        ~PrvNodeData()
        {
            delete node;
            //delete parentData; // already deleted with the list
            delete childrenDataList;
        };

        class INode *node;
        class PrvNodeData *parentData;
        class CArrayRef<PrvNodeData> *childrenDataList;
};

struct prv_baseTree_t
{
    class PrvNodeData *rootData;
    CArray<PrvNodeData> *nodeDataList;
};

//------------------------------------------------------------------------------
//
static struct prv_baseTree_t* prv_create(
                                    class PrvNodeData *rootData,
                                    CArray<PrvNodeData> *nodeDataList )
{
    assert_no_null( nodeDataList );

    prv_baseTree_t *data;

    data = new prv_baseTree_t;
    assert_no_null( data );

    data->rootData = rootData;
    data->nodeDataList = nodeDataList;

    return data;
}

//------------------------------------------------------------------------------
//
PrvBaseTree :: PrvBaseTree()
{
    CArray<PrvNodeData> *nodeDataList;

    nodeDataList = new CArray<PrvNodeData>();

    m_data = prv_create( NULL, nodeDataList );
}

//------------------------------------------------------------------------------
//
static void prv_setRoot( const class INode *root,
                       class PrvNodeData **rootData,
                       class CArray<PrvNodeData> **nodeDataList )
{
    assert_no_null( root );
    assert_no_null( *nodeDataList );
    assert( *rootData == NULL );

    *rootData = new PrvNodeData( root, NULL );
    (*nodeDataList)->add( *rootData );
}

//------------------------------------------------------------------------------
//
static class PrvNodeData* prv_addChild(
                        const class PrvNodeData *parentData,
                        const class INode *node,
                        class CArray<PrvNodeData> **nodeDataList )
{
    class PrvNodeData *nodeData;

    assert_no_null( parentData );
    assert_no_null( node );
    assert_no_null( *nodeDataList );

    nodeData = new PrvNodeData( node, parentData );

    parentData->childrenDataList->add( nodeData );

    (*nodeDataList)->add( nodeData );

    return nodeData;
}

//------------------------------------------------------------------------------
//
static void prv_copyNodes_recursive(   const class PrvNodeData *originalNodeData,
                                     const class PrvNodeData *copiedParentData,
                                     CTree_FPtr_copyNode func_copyNode,
                                     CArray<PrvNodeData> **copiedNodeDataList )
{
    unsigned long numChildren;

    assert_no_null( originalNodeData );
    assert_no_null( *copiedNodeDataList );

    numChildren = originalNodeData->childrenDataList->size();

    for( unsigned long ind = 0; ind < numChildren; ++ind )
    {
        const class PrvNodeData *originalChildData;
        class INode *copiedChild;
        const class PrvNodeData *copiedChildData;

        originalChildData = originalNodeData->childrenDataList->get( ind );

        copiedChild = func_copyNode( originalChildData->node );

        copiedChildData = prv_addChild( copiedParentData,
                                      copiedChild,
                                      copiedNodeDataList );

        if( originalChildData->childrenDataList->size() > 0 )
        {
            prv_copyNodes_recursive( originalChildData, copiedChildData,
                                   func_copyNode, copiedNodeDataList );
        }
    }
}

//------------------------------------------------------------------------------
//
PrvBaseTree :: PrvBaseTree( const class PrvBaseTree *tree,
                            CTree_FPtr_copyNode func_copyNode )
{
    assert_no_null( tree );
    assert_no_null( func_copyNode );

    class CArray<PrvNodeData> *nodeDataList;
    class PrvNodeData *rootNodeData;

    nodeDataList = new CArray<PrvNodeData>();
    rootNodeData = NULL;

    if( tree->m_data->rootData != NULL )
    {
        class INode *copiedRoot;

        copiedRoot = func_copyNode( tree->m_data->rootData->node );
        assert( tree->m_data->rootData->parentData == NULL );

        prv_setRoot( copiedRoot,
                   &rootNodeData, &nodeDataList );

        if( tree->m_data->rootData->childrenDataList->size() > 0 )
        {
            prv_copyNodes_recursive( tree->m_data->rootData,
                                   rootNodeData,
                                   func_copyNode,
                                   &nodeDataList );
        }
    }

    m_data = prv_create( rootNodeData, nodeDataList );
}

//------------------------------------------------------------------------------
//
PrvBaseTree :: ~PrvBaseTree()
{
    delete m_data->nodeDataList;
    //delete m_data->rootData; // already deleted with the list

    delete m_data;
}

//------------------------------------------------------------------------------
//
unsigned long PrvBaseTree :: numNodes() const
{
    return m_data->nodeDataList->size();
}

//------------------------------------------------------------------------------
//
static unsigned long prv_numLevels_recursive( const class PrvNodeData *node )
{
    assert_no_null( node );

    unsigned long numChildren, level;
    const PrvNodeData *childData;

    numChildren = node->childrenDataList->size();
    level = 0;

    if( numChildren == 0 )
        return 1;

    for ( unsigned long ind = 0; ind < numChildren; ++ind )
    {
        childData = node->childrenDataList->get( ind );
        if( childData != NULL )
            level = MAX( level, prv_numLevels_recursive( childData ) );
    }

    return level + 1;
}

//------------------------------------------------------------------------------
//
unsigned long PrvBaseTree :: numLevels() const
{
    if( m_data->rootData == NULL )
    {
        return 0;
    }

    return prv_numLevels_recursive( m_data->rootData );
}

//------------------------------------------------------------------------------
//
static bool prv_searchNode( const class PrvNodeData *nodeData,
                            const class INode *node )
{
    if( nodeData->node == node )
        return true;
    else
        return false;
}

//------------------------------------------------------------------------------
//
static bool prv_isAdded( const class CArray<PrvNodeData> *nodeDataList,
                         const class INode *node,
                         unsigned long *ind_optional )
{
    assert_no_null( node );

    return nodeDataList->exist<INode>( node, prv_searchNode, ind_optional );
}

//------------------------------------------------------------------------------
//
unsigned long PrvBaseTree :: numChildren( const class INode *node ) const
{
    assert_no_null( node );

    unsigned long ind;
    bool isAdded;

    isAdded = prv_isAdded( m_data->nodeDataList, node, &ind );
    assert( isAdded == true );

    return m_data->nodeDataList->get( ind )->childrenDataList->size();
}

//------------------------------------------------------------------------------
//
bool PrvBaseTree :: isLeaf( const class INode *node ) const
{
    assert_no_null( node );

    unsigned long ind;
    bool isAdded;

    isAdded = prv_isAdded( m_data->nodeDataList, node, &ind );
    assert( isAdded == true );

    return m_data->nodeDataList->get( ind )->childrenDataList->size() == 0;
}

//------------------------------------------------------------------------------
//
bool PrvBaseTree :: isRoot( const class INode *node ) const
{
    assert_no_null( node );

    unsigned long ind;
    bool isAdded;

    isAdded = prv_isAdded( m_data->nodeDataList, node, &ind );
    assert( isAdded == true );

    return m_data->nodeDataList->get( ind )->parentData == NULL;
}

//------------------------------------------------------------------------------
//
void PrvBaseTree :: setRoot( const class INode *root )
{
    prv_setRoot( root,
                 &m_data->rootData,
                 &m_data->nodeDataList );
}

//------------------------------------------------------------------------------
//
void PrvBaseTree :: addChild( const class INode *parent, const class INode *node )
{
    unsigned long ind_parent;
    const class PrvNodeData *parentData;

    assert( prv_isAdded( m_data->nodeDataList, node, NULL ) == false );
    assert( prv_isAdded( m_data->nodeDataList, parent, &ind_parent ) == true );

    parentData = m_data->nodeDataList->get( ind_parent );

    prv_addChild( parentData, node,
                &m_data->nodeDataList );
}

//------------------------------------------------------------------------------
//
const class INode* PrvBaseTree :: getRoot() const
{
    if( m_data->rootData == NULL )
        return NULL;

    return m_data->rootData->node;
}

//------------------------------------------------------------------------------
//
const class INode* PrvBaseTree :: getParent( const class INode *node ) const
{
    const class PrvNodeData *parentData;
    unsigned long ind;

    assert_no_null( node );
    assert( prv_isAdded( m_data->nodeDataList, node, &ind ) == true );

    parentData = m_data->nodeDataList->get( ind )->parentData;

    if( parentData == NULL )
        return NULL;

    return parentData->node;
}

//------------------------------------------------------------------------------
//
const class INode* PrvBaseTree :: getChild( const class INode *node, unsigned long ind ) const
{
    const class PrvNodeData *nodeData;
    unsigned long indNode;

    assert_no_null( node );
    assert( prv_isAdded( m_data->nodeDataList, node, &indNode ) == true );

    nodeData = m_data->nodeDataList->get( indNode );

    assert( ind < nodeData->childrenDataList->size() );

    return nodeData->childrenDataList->get( ind )->node;
}

//------------------------------------------------------------------------------
//
class CArrayRef<INode>* PrvBaseTree :: getChildren( const class INode *node ) const
{
    unsigned long ind, numChildren;
    const class PrvNodeData *nodeData;
    CArrayRef<INode> *childrenList;

    assert_no_null( node );
    assert( prv_isAdded( m_data->nodeDataList, node, &ind ) == true );

    nodeData = m_data->nodeDataList->get( ind );

    numChildren = nodeData->childrenDataList->size();

    childrenList = new CArrayRef<INode>( numChildren );

    for( unsigned long nc = 0; nc < numChildren; ++nc )
    {
        const class INode *childNode;

        childNode = nodeData->childrenDataList->get( nc )->node;

        childrenList->set( nc, childNode );
    }

    return childrenList;
}

//------------------------------------------------------------------------------
//
static bool prv_comparePtr( const class INode *item1,
                            const struct prv_itemSearch_t *item2 )
{
    if( item1 == (const class INode*)item2 )
        return true;
    else
        return false;
}

//------------------------------------------------------------------------------
//
bool PrvBaseTree :: exist( const struct prv_itemSearch_t *itemSearch,
                                CTree_FPtr_search func_search_optional ) const
{
    unsigned long numNodes;
    CTree_FPtr_search func_search;

    numNodes = this->numNodes();

    if( func_search_optional == NULL )
        func_search = prv_comparePtr;
    else
        func_search = func_search_optional;

    for( unsigned long ind = 0; ind < numNodes; ++ind )
    {
        if ( func_search( m_data->nodeDataList->get( ind )->node, itemSearch ) == true )
        {
            return true;
        }
    }

    return false;
}

//------------------------------------------------------------------------------
//
static bool prv_equal_recursive( const class PrvNodeData *node1,
                                 const class PrvNodeData *node2 )
{
    unsigned long numChildren;
    bool foundEqual;
    bool *usedChildren;
    
    assert_no_null( node1 );
    assert_no_null( node2 );

    if( node1->node->equal( node2->node ) == false ||
        node1->childrenDataList->size() != node2->childrenDataList->size() )
    {
        return false;
    }

    numChildren = node1->childrenDataList->size();
    usedChildren = new bool[ numChildren ];

    for( unsigned long i=0; i < numChildren; ++i )
        usedChildren[ i ] = false;

    for( unsigned long ind1=0; ind1 < numChildren; ++ind1 )
    {
        foundEqual = false;

        for( unsigned long ind2=0; ind2 < numChildren; ++ind2 )
        {
            if( usedChildren[ ind2 ] == false
                &&
                prv_equal_recursive( node1->childrenDataList->get( ind1 ),
                                     node2->childrenDataList->get( ind2 ) ) == true )
            {
                usedChildren[ ind2 ] = true;
                foundEqual = true;
                break;
            }
        }

        if( foundEqual == false )
            return false;
    }

    delete usedChildren;

    return true;
}

//------------------------------------------------------------------------------
//
bool PrvBaseTree :: equal( const class PrvBaseTree *tree ) const
{
    if( this->numNodes() != tree->numNodes() )
        return false;

    return prv_equal_recursive( m_data->rootData, tree->m_data->rootData );
}

//------------------------------------------------------------------------------
//
static void prv_getNodesPreorder_recursive( 
                                const class PrvNodeData *nodeData,
                                class CArrayRef<INode> *list )
{
    unsigned long numChildren;

    numChildren = nodeData->childrenDataList->size();

    list->add( nodeData->node );

    for ( unsigned long ind = 0; ind < numChildren; ++ind )
    {
        const class PrvNodeData *childData;

        childData = nodeData->childrenDataList->get( ind );

        if( childData != NULL )
            prv_getNodesPreorder_recursive( childData, list );
    }
}

//------------------------------------------------------------------------------
//
class CArrayRef<INode>* PrvBaseTree :: getNodesPreorder() const
{
    class CArrayRef<INode> *nodeList;

    nodeList = new CArrayRef<INode>();

    if( m_data->rootData != NULL )
    {
        prv_getNodesPreorder_recursive( m_data->rootData, nodeList );
    }

    return nodeList;
}

//------------------------------------------------------------------------------
//
static void prv_getNodesInorder_recursive( const class PrvNodeData *nodeData,
                                class CArrayRef<INode> *list )
{
    unsigned long numChildren;
    unsigned long middle;

    numChildren = nodeData->childrenDataList->size();
    middle = numChildren / 2;

    for ( unsigned long ind = 0; ind < middle; ++ind )
    {
        const class PrvNodeData *childData;

        childData = nodeData->childrenDataList->get( ind );

        if( childData != NULL )
            prv_getNodesInorder_recursive( childData, list );
    }

    list->add( nodeData->node );

    for ( unsigned long ind = middle; ind < numChildren; ++ind )
    {
        const class PrvNodeData *childData;

        childData = nodeData->childrenDataList->get( ind );

        if( childData != NULL )
            prv_getNodesInorder_recursive( childData, list );
    }
}

//------------------------------------------------------------------------------
//
class CArrayRef<INode>* PrvBaseTree :: getNodesInorder() const
{
    class CArrayRef<INode> *nodeList;

    nodeList = new CArrayRef<INode>();

    if( m_data->rootData != NULL )
    {
        prv_getNodesInorder_recursive( m_data->rootData, nodeList );
    }

    return nodeList;
}

//------------------------------------------------------------------------------
//
static void prv_getNodesPostorder_recursive( const class PrvNodeData *nodeData,
                                class CArrayRef<INode> *list )
{
    unsigned long numChildren;

    numChildren = nodeData->childrenDataList->size();

    for ( unsigned long ind = 0; ind < numChildren; ++ind )
    {
        const class PrvNodeData *childData;

        childData = nodeData->childrenDataList->get( ind );

        if( childData != NULL )
            prv_getNodesPostorder_recursive( childData, list );
    }

    list->add( nodeData->node );
}

//------------------------------------------------------------------------------
//
class CArrayRef<INode>* PrvBaseTree :: getNodesPostorder() const
{
    class CArrayRef<INode> *nodeList;

    nodeList = new CArrayRef<INode>();

    if( m_data->rootData != NULL )
    {
        prv_getNodesPostorder_recursive( m_data->rootData, nodeList );
    }

    return nodeList;
}
