/* -----------------------------------------------------------------------------
*  CSubGraphVertex
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#ifndef CSUBGRAPHVERTEX_HPP_
#define CSUBGRAPHVERTEX_HPP_

#include "IVertex.hpp"

class CSubGraphVertex : public IVertex
{
    public:
        CSubGraphVertex( const class IVertex *vertex,
                         unsigned long labelIn, unsigned long labelOut );
        CSubGraphVertex( const class CSubGraphVertex *vertex );
        ~CSubGraphVertex();

        const class IVertex* getReferedVertex() const;
        unsigned long getLabelOut() const;
        unsigned long getLabelIn() const;

        bool equal( const class IVertex *vertex ) const;

        void print() const;

        //class IVertex* read( class CFileTxtIn **file ) const;
        //void write( class CFileTxtOut **file ) const;
        //class IVertex* read( class CFileBinIn *file ) const;
        //void write( class CFileBinOut *file ) const;

    private:
        struct prv_subGraphVertex_t *m_data;
};

#endif