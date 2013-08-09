/* -----------------------------------------------------------------------------
*  CFileBinOut
*  Author: Gabriel López García (gabriellg AT gmail DOT com)
*          Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#ifndef IFILEOUT_HPP_
#define IFILEOUT_HPP_

class IFileOut
{
    public:
        virtual ~IFileOut() {;}

        virtual void writeChar( char value ) = 0;
        virtual void writeString( const char *str ) = 0;
        virtual void writeString_optional( const char *cadena ) = 0;

        virtual void writeBool( bool value ) = 0;
        virtual void writeByte( unsigned char value ) = 0;
        virtual void writeShort( short value ) = 0;
        virtual void writeUShort( unsigned short value ) = 0;
        virtual void writeInt( int value ) = 0;
        virtual void writeULong( unsigned long value ) = 0;
        virtual void writeDouble( double value ) = 0;
};

#endif
