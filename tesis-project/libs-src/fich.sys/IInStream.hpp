/* -----------------------------------------------------------------------------
*  IInStream
*  Author: Gabriel López García (gabriel.lg AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#ifndef IINSTREAM_HPP_
#define IINSTREAM_HPP_

class IInStream
{
    public:

        virtual ~IInStream() {;}

        virtual bool readBool(void) = 0;
        virtual char *readString(void) = 0;
        virtual char readChar(void) = 0;
        virtual unsigned char readByte(void) = 0;

        virtual unsigned short readUShort(void) = 0;
        virtual short readShort(void) = 0;

        virtual long readLong(void) = 0;
        virtual unsigned long readULong(void) = 0;

        virtual double readDouble(void) = 0;
};

#endif /* IINSTREAM_HPP_ */
