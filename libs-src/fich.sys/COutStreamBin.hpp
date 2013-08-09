/* -----------------------------------------------------------------------------
*  COutStreamBin
*  Author: Gabriel López García (gabriel.lg AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "IOutStream.hpp"

class COutStreamBin: public IOutStream
{
    public:

        COutStreamBin(const char *nameFile);
        virtual ~COutStreamBin();

        virtual void writeBool(bool b);
        virtual void writeString(const char *string);
        virtual void writeChar(char value);
        virtual void writeByte(unsigned char value);

        virtual void writeUShort(unsigned short value);
        virtual void writeShort(short value);

        virtual void writeLong(long value);
        virtual void writeULong(unsigned long value);
        virtual void writeDouble(double value);

        void close(void);

    private:

        struct prv_dataPrivateOutStreamBin_t *m_dataPrivate;
};
