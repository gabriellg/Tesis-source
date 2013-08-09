/* -----------------------------------------------------------------------------
*  IFileIn (Interface FileIn)
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#ifndef IFILEIN_HPP_
#define IFILEIN_HPP_

class IFileIn
{
    public:
        virtual ~IFileIn() {;}

        virtual bool eof() const = 0;

        //char* readBuffer( char *buffer, unsigned long sizeBuffer );

        virtual char readChar() = 0;
        virtual char* readString() = 0;
        virtual char* readString_optional() = 0;

        virtual bool readBool() = 0;
        virtual unsigned char readByte() = 0;
        virtual short readShort() = 0;
        virtual unsigned short readUShort() = 0;
        virtual int readInt() = 0;
        virtual unsigned long readULong() = 0;
        virtual double readDouble() = 0;
};

#endif
