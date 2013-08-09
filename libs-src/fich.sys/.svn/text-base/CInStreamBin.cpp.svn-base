/* -----------------------------------------------------------------------------
*  CInStreamBin
*  Author: Gabriel López García (gabriel.lg AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "CInStreamBin.hpp"
#include "CFileBas.inl"

#include "CString.hpp"
#include "stdio.h"
#include "asrtbas.h"

//------------------------------------------------------------------------------
//
CInStreamBin::CInStreamBin(const char *nameFile)
{
    m_dataPrivate = (struct prv_dataPrivateInStreamBin_t *)CFileBas::openBin(nameFile, CFileBas::READ);
}

//------------------------------------------------------------------------------
//
CInStreamBin::~CInStreamBin()
{
    assert_with_message(m_dataPrivate == NULL, "Read mode file isn't closed");
}

//------------------------------------------------------------------------------
//
static FILE *prv_transformFILE(struct prv_dataPrivateInStreamBin_t *outStream)
{
    assert_no_null(outStream);
    return (FILE *)outStream;
}

//------------------------------------------------------------------------------
//
bool CInStreamBin::readBool(void)
{
    bool value;
    size_t read;
    FILE *file;

    file = prv_transformFILE(m_dataPrivate);
    read = fread(&value, sizeof(bool), 1, file);
    assert(read == 1);

    assert(value == true || value == false);

    return value;
}

//------------------------------------------------------------------------------
//
char *CInStreamBin::readString(void)
{
    char *str;
    unsigned long length;
    size_t leidos;
    FILE *file;

    file = prv_transformFILE(m_dataPrivate);

    leidos = fread(&length, sizeof(unsigned long), 1, file);
    assert(leidos == 1);

    str = CString::create(length+1);

    leidos = fread(str, sizeof(char), length, file);
    assert(leidos == length);

    return str;
}

//------------------------------------------------------------------------------
//
char CInStreamBin::readChar(void)
{
    char value;
    size_t read;
    FILE *file;

    file = prv_transformFILE(m_dataPrivate);
    read = fread(&value, sizeof(char), 1, file);
    assert(read == 1);

    return value;
}

//------------------------------------------------------------------------------
//
unsigned char CInStreamBin::readByte(void)
{
    unsigned char value;
    size_t read;
    FILE *file;

    file = prv_transformFILE(m_dataPrivate);
    read = fread(&value, sizeof(unsigned char), 1, file);
    assert(read == 1);

    return value;
}

//------------------------------------------------------------------------------
//
unsigned short CInStreamBin::readUShort(void)
{
    unsigned char value;
    size_t read;
    FILE *file;

    file = prv_transformFILE(m_dataPrivate);
    read = fread(&value, sizeof(unsigned char), 1, file);
    assert(read == 1);

    return value;
}

//------------------------------------------------------------------------------
//
short CInStreamBin::readShort(void)
{
    short value;
    size_t read;
    FILE *file;

    file = prv_transformFILE(m_dataPrivate);
    read = fread(&value, sizeof(short), 1, file);
    assert(read == 1);

    return value;
}

//------------------------------------------------------------------------------
//
long CInStreamBin::readLong(void)
{
    long value;
    size_t read;
    FILE *file;

    file = prv_transformFILE(m_dataPrivate);
    read = fread(&value, sizeof(long), 1, file);
    assert(read == 1);

    return value;
}

//------------------------------------------------------------------------------
//
unsigned long CInStreamBin::readULong(void)
{
    unsigned long value;
    size_t read;
    FILE *file;

    file = prv_transformFILE(m_dataPrivate);
    read = fread(&value, sizeof(unsigned long), 1, file);
    assert(read == 1);

    return value;
}

//------------------------------------------------------------------------------
//
double CInStreamBin::readDouble(void)
{
    double value;
    size_t read;
    FILE *file;

    file = prv_transformFILE(m_dataPrivate);
    read = fread(&value, sizeof(double), 1, file);
    assert(read == 1);

    return value;
}

//------------------------------------------------------------------------------
//
void CInStreamBin::close(void)
{
    CFileBas::close((void **)&m_dataPrivate);
}
