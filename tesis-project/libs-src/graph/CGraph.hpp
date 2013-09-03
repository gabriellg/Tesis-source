/* -----------------------------------------------------------------------------
*  CGraph
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "PrvBaseGraph.inl"

template<class TVertex> class CGraph : public PrvBaseGraph
{
    public:
        CGraph(void);
        CGraph(const class CGraph *graph);

        inline void addVertex(TVertex *vertex);
        inline void addEdge(const TVertex *initialVertex, const TVertex *endVertex);

        inline unsigned long numVertex(void) const;
        inline unsigned long numEdges(void) const;
        inline unsigned long numInitials(void) const;
        inline unsigned long numFinals(void) const;
        inline unsigned long numParents(const TVertex *vertex) const;
        inline unsigned long numChildren(const TVertex *vertex) const;
        inline unsigned long level( const TVertex *vertex ) const;
        inline unsigned long maxLevel() const;

        inline class CArrayRef<TVertex>* getInitials(void) const;
        inline class CArrayRef<TVertex>* getFinals(void) const;
        inline class CArrayRef<TVertex>* getVertex(void) const;
        inline class CArrayRef<TVertex>* getParents(const TVertex *vertex) const;
        inline class CArrayRef<TVertex>* getChildren(const TVertex *vertex) const;

        template<class TypeSearch> inline bool exist(
                                const TypeSearch *itemSearch,
                                bool (*func_search)(const TVertex*, const TypeSearch*),
                                TVertex **vertex_optional) const;
        inline bool isVertex(const TVertex *vertex) const;

        inline bool equal(const class CGraph *graph) const;

        inline bool isAcyclic() const;

    private:
        inline static class IVertex* prv_copyVertex( const class IVertex *vertex );
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//
template<class TVertex>
CGraph<TVertex>::CGraph(void)
: PrvBaseGraph()
{
    ;
}

//------------------------------------------------------------------------------
//
template<class TVertex>
CGraph<TVertex>::CGraph( const class CGraph *graph )
: PrvBaseGraph( graph, prv_copyVertex )
{
    ;
}

//------------------------------------------------------------------------------
//
template<class TVertex>
void CGraph<TVertex>::addVertex(TVertex *vertex )
{
    PrvBaseGraph::addVertex(vertex);
}

//------------------------------------------------------------------------------
//
template<class TVertex>
void CGraph<TVertex>::addEdge(const TVertex *initialVertex, const TVertex *endVertex )
{
    PrvBaseGraph::addEdge(initialVertex, endVertex);
}

//------------------------------------------------------------------------------
//
template<class TVertex>
unsigned long CGraph<TVertex>::numVertex() const
{
    return PrvBaseGraph::numVertex();
}

//------------------------------------------------------------------------------
//
template<class TVertex>
unsigned long CGraph<TVertex>::numEdges() const
{
    return PrvBaseGraph::numEdges();
}

//------------------------------------------------------------------------------
//
template<class TVertex>
unsigned long CGraph<TVertex>::numInitials() const
{
    return PrvBaseGraph::numInitials();
}

//------------------------------------------------------------------------------
//
template<class TVertex>
unsigned long CGraph<TVertex>::numFinals() const
{
    return PrvBaseGraph::numFinals();
}

//------------------------------------------------------------------------------
//
template<class TVertex>
unsigned long CGraph<TVertex>::numParents( const TVertex *vertex ) const
{
    return PrvBaseGraph::numParents( vertex );
}

//------------------------------------------------------------------------------
//
template<class TVertex>
unsigned long CGraph<TVertex>::numChildren( const TVertex *vertex ) const
{
    return PrvBaseGraph::numChildren( vertex );
}

//------------------------------------------------------------------------------
//
template<class TVertex>
unsigned long CGraph<TVertex>::level( const TVertex *vertex ) const
{
    return PrvBaseGraph::level( vertex );
}

//------------------------------------------------------------------------------
//
template<class TVertex>
unsigned long CGraph<TVertex>::maxLevel() const
{
    return PrvBaseGraph::maxLevel();
}

//------------------------------------------------------------------------------
//
template<class TVertex>
class CArrayRef<TVertex>* CGraph<TVertex>::getInitials(void) const
{
    return (class CArrayRef<TVertex>*)PrvBaseGraph::getInitials();
}

//------------------------------------------------------------------------------
//
template<class TVertex>
class CArrayRef<TVertex>* CGraph<TVertex>::getFinals(void) const
{
    return (class CArrayRef<TVertex>*)PrvBaseGraph::getFinals();
}

//------------------------------------------------------------------------------
//
template<class TVertex>
class CArrayRef<TVertex>* CGraph<TVertex>::getVertex(void) const
{
    return (class CArrayRef<TVertex>*)PrvBaseGraph::getVertex();
}

//------------------------------------------------------------------------------
//
template<class TVertex>
class CArrayRef<TVertex>* CGraph<TVertex>::getParents(const TVertex *vertex) const
{
    return (class CArrayRef<TVertex>*)PrvBaseGraph::getParents(vertex);
}

//------------------------------------------------------------------------------
//
template<class TVertex>
class CArrayRef<TVertex>* CGraph<TVertex>::getChildren( const TVertex *vertex ) const
{
    return (class CArrayRef<TVertex>*)PrvBaseGraph::getChildren(vertex);
}

//------------------------------------------------------------------------------
//
template<class TVertex> template<class TypeSearch>
bool CGraph<TVertex>::exist(
                            const TypeSearch *itemSearch,
                            bool (*func_search)(const TVertex*, const TypeSearch*),
                            TVertex **vertex_optional) const
{
    return PrvBaseGraph::exist(
                            (const struct prv_itemSearch_t*)itemSearch,
                            (CGraph_FPtr_search)func_search,
                            vertex_optional);
}

//------------------------------------------------------------------------------
//
template<class TVertex>
inline bool CGraph<TVertex>::isVertex(const TVertex *vertex) const
{
    return PrvBaseGraph::isVertex(vertex);
}

//------------------------------------------------------------------------------
//
template<class TVertex>
bool CGraph<TVertex>::equal(const class CGraph *graph) const
{
    return PrvBaseGraph::equal(graph);
}

//------------------------------------------------------------------------------
//
template<class TVertex>
bool CGraph<TVertex>::isAcyclic() const
{
    return PrvBaseGraph::isAcyclic();
}

//------------------------------------------------------------------------------
//
template<class TVertex>
class IVertex* CGraph<TVertex>::prv_copyVertex( const class IVertex *vertex )
{
    return (class IVertex*) new TVertex( (const TVertex*)vertex );
}
