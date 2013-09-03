/* -----------------------------------------------------------------------------
*  CFileOutTxt
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#ifndef CFILEOUTTXT_HPP_
#define CFILEOUTTXT_HPP_

#include "IFileOut.hpp"

class CFileOutTxt : public IFileOut
{
    public:
        CFileOutTxt( const char* filename );
        ~CFileOutTxt();

        void writeChar( char c );
        void writeString( const char* str );
        void writeString_optional( const char *cadena );

        void writeBool( bool value );
        void writeByte( unsigned char value );
        void writeShort( short value );
        void writeUShort( unsigned short value );
        void writeInt( int value );
        void writeULong( unsigned long value );
        void writeDouble( double value );
        //#define writeEnum( value ) (writeUShort( (unsigned short)value ))

        void writeArray( const class CArrayString *array );

    private:
        struct prv_fileOut_t *m_data;
};

#endif
