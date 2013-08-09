/* -----------------------------------------------------------------------------
*  CGraphNGram (N-Grams for graphs)
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#ifndef CGRAPHNGRAM_HPP_
#define CGRAPHNGRAM_HPP_

template<class> class CGraph;

class CGraphNGram
{
    public:
        //CGraphNGram( const char *filename );
        CGraphNGram( unsigned long lengthK );
        ~CGraphNGram();

        unsigned long lengthK() const;
        unsigned long numOfTransitions() const;
        unsigned long numGraphsLoaded() const;
        //unsigned long numClasses() const;

        void extractTransitionRules( const class CGraph<class IVertex> *graph );

        bool isAcceptedGraph( const class CGraph<class IVertex> *graph, double *prob_optional ) const;

        void print() const;

        void testProbabilityIsEqualTo1() const;

        //void writeToFile( const char* filename ) const;

    private:
        struct prv_graphNGram_t *m_data;
};

#endif
