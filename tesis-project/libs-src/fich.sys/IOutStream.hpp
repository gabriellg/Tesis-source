/* -----------------------------------------------------------------------------
*  IOutStream
*  Author: Gabriel López García (gabriel.lg AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#ifndef IOUTSTREAM_HPP_
#define IOUTSTREAM_HPP_

class IOutStream
{
    public:

        virtual ~IOutStream() {;}

        virtual void writeBool(bool b) = 0;
        virtual void writeString(const char *string) = 0;
        virtual void writeChar(char value) = 0;
        virtual void writeByte(unsigned char value) = 0;

        virtual void writeUShort(unsigned short value) = 0;
        virtual void writeShort(short value) = 0;

        virtual void writeLong(long value) = 0;
        virtual void writeULong(unsigned long value) = 0;
        virtual void writeDouble(double value) = 0;
};

#endif /* IOUTSTREAM_HPP_ */
