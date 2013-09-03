/* -----------------------------------------------------------------------------
*  CFileOutBin
*  Author: Gabriel López García (gabriellg AT gmail DOT com)
*          Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#ifndef CFILEOUTBIN_HPP_
#define CFILEOUTBIN_HPP_

#include "IFileOut.hpp"

class CFileOutBin : public IFileOut
{
    public:
        CFileOutBin( const char* filename );
        ~CFileOutBin();

        void writeChar( char value );
        void writeString( const char *str );
        void writeString_optional( const char *cadena );

        void writeBool( bool value );
        void writeByte( unsigned char value );
        void writeShort( short value );
        void writeUShort( unsigned short value );
        void writeInt( int value );
        void writeDouble( double value );
        void writeUChar( unsigned char value );
        void writeULong( unsigned long value );

        //#define fileout_escr_enum(f, tipo_enum, car) ((void)((tipo_enum)car == car), fileout_escr_ushort(f, (unsigned long)car))
    private:
        struct prv_fileBinOut_t *m_data;
};

#endif