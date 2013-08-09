/* -----------------------------------------------------------------------------
*  CInStreamBin
*  Author: Gabriel López García (gabriel.lg AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "IInStream.hpp"

class CInStreamBin: public IInStream
{
    public:

        CInStreamBin(const char *nameFile);
        virtual ~CInStreamBin();

        virtual bool readBool(void);
        virtual char *readString(void);
        virtual char readChar(void);
        virtual unsigned char readByte(void);

        virtual unsigned short readUShort(void);
        virtual short readShort(void);

        virtual long readLong(void);
        virtual unsigned long readULong(void);

        virtual double readDouble(void);

        void close(void);

    private:

        struct prv_dataPrivateInStreamBin_t *m_dataPrivate;
};
