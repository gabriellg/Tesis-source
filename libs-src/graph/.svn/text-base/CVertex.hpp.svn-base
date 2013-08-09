/* -----------------------------------------------------------------------------
*  CVertex
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "IVertex.hpp"

class CVertex : public IVertex
{
    public:
        CVertex( char label );
        CVertex( const CVertex *vertex );
        ~CVertex();

        char getLabel() const;

        bool equal( const class IVertex *vertex ) const;

        void print() const;

        static class CVertex* read(class IFileIn *in);
        static void write(const class CVertex *vertex, class IFileOut *out);

    private:
        struct prv_vertex_t *m_data;
};
