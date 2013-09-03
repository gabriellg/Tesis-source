/* -----------------------------------------------------------------------------
*  CArrayString
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*          Gabriel López García (gabriellg AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "PrvBaseArray.inl"

class CArrayString : public PrvBaseArray
{
    public:
        CArrayString();
        CArrayString( unsigned long size );
        CArrayString( const class CArrayString *array );

        void clear();
        unsigned long size() const;

        const char* get( unsigned long ind ) const;
        const char* getLast() const;

        void setDestroyingString( unsigned long pos, char **str );
        void set( unsigned long pos, const char* str );

        void add( const char* str );
        void addDestroyingString( char **str );

        void erase( unsigned long ind );
        void eraseLast();

        void concatenate( const class CArrayString *arrayStr );

        bool equal( const CArrayString *arrayStr ) const;
        bool searchString( const char* str, unsigned long *ind ) const;

        char *toString(char separator);
};
