/* -----------------------------------------------------------------------------
*  CSubGraph
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#ifndef CSUBGRAPH_HPP_
#define CSUBGRAPH_HPP_

template<class> class CGraph;

class CSubGraph
{
    public:
        CSubGraph( const class CGraph<class IVertex> *graph,
                   const class IVertex *copyFromVertex,
                   unsigned long lengthK );

        //CSubGraph( class CFileBinIn *file );

        ~CSubGraph();

        unsigned long length() const;
        bool isFinal() const;

        unsigned long numTimesRepeated() const;
        void incrementNumTimesRepeated();

        bool equal( const class CSubGraph *subgraph ) const;

        void print() const;

        //void writeToBinaryStream( class CFileBinOut *file );

    private:
        struct prv_subgraph_t *m_data;
};

#endif