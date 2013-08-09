/* -----------------------------------------------------------------------------
*  IVertex (Vertex Interface)
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#ifndef IVERTEX_HPP_
#define IVERTEX_HPP_

class IVertex
{
    public:
        virtual ~IVertex() {;}

        virtual bool equal( const class IVertex *vertex ) const = 0;

        virtual void print() const = 0;

        //static class IVertex* read(class CFileIn*) = 0;
        //static void write(const class IVertex *vertex, class CFileOut*) = 0;
};

#endif
