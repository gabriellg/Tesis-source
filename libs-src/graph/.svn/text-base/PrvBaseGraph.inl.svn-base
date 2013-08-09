/* -----------------------------------------------------------------------------
*  PrvBaseGraph
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#ifndef PRVBASEGRAPH_HPP_
#define PRVBASEGRAPH_HPP_

typedef bool (*CGraph_FPtr_search)(const class IVertex *vertex, const class prv_itemSearch_t *dataSearch);
typedef class IVertex* (*CGraph_FPtr_copyVertex)(const class IVertex *vertex);

template<class> class CArrayRef;

class PrvBaseGraph
{
    protected:
        PrvBaseGraph(void);
        PrvBaseGraph(const class PrvBaseGraph *graph, CGraph_FPtr_copyVertex func_copyVertex);

        ~PrvBaseGraph(void);

        void addVertex(class IVertex *vertex);
        void addEdge(const class IVertex *initialVertex, const class IVertex *endVertex);

        unsigned long numVertex(void) const;
        unsigned long numEdges(void) const;
        unsigned long numInitials(void) const;
        unsigned long numFinals(void) const;
        unsigned long numParents(const class IVertex *vertex) const;
        unsigned long numChildren(const class IVertex *vertex) const;
        unsigned long level(const class IVertex *vertex) const;
        unsigned long maxLevel() const;

        class CArrayRef<IVertex>* getInitials(void) const;
        class CArrayRef<IVertex>* getFinals(void) const;
        class CArrayRef<IVertex>* getVertex(void) const;
        class CArrayRef<IVertex>* getParents(const class IVertex *vertex) const;
        class CArrayRef<IVertex>* getChildren(const class IVertex *vertex) const;

        bool exist(const struct prv_itemSearch_t *itemSearch,
                        CGraph_FPtr_search func_search,
                        class IVertex **vertex_optional);
        bool isVertex(const class IVertex *vertex) const;

        bool equal(const class PrvBaseGraph *graph) const;

        bool isAcyclic() const;

    private:
        struct prv_baseGraph_t *m_data;
};

#endif
