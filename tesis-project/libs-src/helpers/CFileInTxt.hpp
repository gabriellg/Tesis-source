/* -----------------------------------------------------------------------------
*  CFileInTxt
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#ifndef CFILEINTXT_HPP_
#define CFILEINTXT_HPP_

#include "IFileIn.hpp"

class CFileInTxt : public IFileIn
{
    public:
        CFileInTxt( const char* filename );
        ~CFileInTxt();

        bool eof() const;
        unsigned long totalLines() const;
        unsigned long totalBytes() const;

        bool ungetChar();
        void ungetAll();

        char readChar();
        char* readLine();
        char* readString();
        char* readString_optional();

        unsigned char readByte();
        bool readBool();
        short readShort();
        unsigned short readUShort();
        int readInt();
        unsigned long readULong();
        double readDouble();

        class CArrayString* readVector();
        //class CArrayString* readVector( unsigned long numLine ) const;

    private:
        struct prv_fileTxtIn_t *m_data;
};

#endif