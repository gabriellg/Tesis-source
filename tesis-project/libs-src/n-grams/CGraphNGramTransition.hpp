/* -----------------------------------------------------------------------------
*  CGraphNGramTransition
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#ifndef CGRAPHNGRAMTRANSITION_HPP_
#define CGRAPHNGRAMTRANSITION_HPP_

typedef class IVertex* (*CGraphNGramTransition_FPtr_loadVertex)(class CFileBinIn *file);
template<class> class CArrayRef;

class CGraphNGramTransition
{
    public:
        /*CGraphNGramTransition(
                        class CFileBinIn *file,
                        CGraphNGramTransition_FPtr_loadVertex func_loadVertex );*/
        CGraphNGramTransition(
                         const class IVertex *initialVertex,
                         unsigned long numInitialParents,
                         unsigned long numInitialChildren,
                         const class CArrayRef<class CSubGraph> *multiset,
                         const class CSubGraph *finalGraph );
        ~CGraphNGramTransition();

        //char initialLabel() const;
        unsigned long numInitialParents() const;
        unsigned long numInitialChildren() const;

        double getProbability()const;
        unsigned long numTimesRepeated() const;
        void incrementNumTimesRepeated();

        bool equal( const class CGraphNGramTransition *transition ) const;

        void print() const;

        //void writeToBinaryStream( class CFileBinOut *file ) const;

    private:
        struct prv_transition_t *m_data;
};

#endif