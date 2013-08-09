/* -----------------------------------------------------------------------------
*  CFileBinIn
*  Author: Gabriel López García (gabriellg AT gmail DOT com)
*          Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#ifndef CFILEINBIN_HPP_
#define CFILEINBIN_HPP_

#include "IFileIn.hpp"

class CFileInBin : public IFileIn
{
    public:
        CFileInBin( const char* filename );
        ~CFileInBin();

        bool eof() const;

        char* readBuffer( char *buffer, unsigned long sizeBuffer );

        char readChar();
        char* readString();
        char* readString_optional();

        bool readBool();
        unsigned char readByte();
        short readShort();
        unsigned short readUShort();
        int readInt();
        unsigned long readULong();
        double readDouble();

    private:
        struct prv_fileBinIn_t *m_data;
};

#endif
