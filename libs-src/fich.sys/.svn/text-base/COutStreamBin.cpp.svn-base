/* -----------------------------------------------------------------------------
*  COutStreamBin
*  Author: Gabriel López García (gabriel.lg AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "COutStreamBin.hpp"

#include "CFileBas.inl"

#include "CString.hpp"
#include "stdio.h"
#include "asrtbas.h"

//------------------------------------------------------------------------------
//
COutStreamBin::COutStreamBin(const char *nameFile)
{
    m_dataPrivate = (struct prv_dataPrivateOutStreamBin_t *)CFileBas::openBin(nameFile, CFileBas::WRITE);
}

//------------------------------------------------------------------------------
//
COutStreamBin::~COutStreamBin()
{
    assert_with_message(m_dataPrivate == NULL, "Write mode file isn't closed");
}

//------------------------------------------------------------------------------
//
static FILE *prv_transformFILE(struct prv_dataPrivateOutStreamBin_t *outStream)
{
    assert_no_null(outStream);
    return (FILE *)outStream;
}

//------------------------------------------------------------------------------
//
void COutStreamBin::writeBool(bool b)
{
    size_t written;
    FILE *file;

    file = prv_transformFILE(m_dataPrivate);
    written = fwrite(&b, sizeof(bool), 1, file);
    assert(written == 1);
}

//------------------------------------------------------------------------------
//
void COutStreamBin::writeString(const char *string)
{
    unsigned long lengthString;
    size_t written;
    FILE *file;

    file = prv_transformFILE(m_dataPrivate);

    lengthString = CString::size(string);

    written = fwrite(&lengthString, sizeof(unsigned long), 1, file);
    assert(written == 1);

    written = fwrite(string, sizeof(char), lengthString, file);
    assert(written == lengthString);
}

//------------------------------------------------------------------------------
//
void COutStreamBin::writeChar(char value)
{
    size_t written;
    FILE *file;

    file = prv_transformFILE(m_dataPrivate);
    written = fwrite(&value, sizeof(char), 1, file);
    assert(written == 1);
}

//------------------------------------------------------------------------------
//
void COutStreamBin::writeByte(unsigned char value)
{
    size_t written;
    FILE *file;

    file = prv_transformFILE(m_dataPrivate);
    written = fwrite(&value, sizeof(unsigned char), 1, file);
    assert(written == 1);
}

//------------------------------------------------------------------------------
//
void COutStreamBin::writeUShort(unsigned short value)
{
    size_t written;
    FILE *file;

    file = prv_transformFILE(m_dataPrivate);
    written = fwrite(&value, sizeof(unsigned short), 1, file);
    assert(written == 1);
}

//------------------------------------------------------------------------------
//
void COutStreamBin::writeShort(short value)
{
    size_t written;
    FILE *file;

    file = prv_transformFILE(m_dataPrivate);
    written = fwrite(&value, sizeof(short), 1, file);
    assert(written == 1);
}

//------------------------------------------------------------------------------
//
void COutStreamBin::writeLong(long value)
{
    size_t written;
    FILE *file;

    file = prv_transformFILE(m_dataPrivate);
    written = fwrite(&value, sizeof(long), 1, file);
    assert(written == 1);
}

//------------------------------------------------------------------------------
//
void COutStreamBin::writeULong(unsigned long value)
{
    size_t written;
    FILE *file;

    file = prv_transformFILE(m_dataPrivate);
    written = fwrite(&value, sizeof(unsigned long), 1, file);
    assert(written == 1);
}

//------------------------------------------------------------------------------
//
void COutStreamBin::writeDouble(double value)
{
    size_t written;
    FILE *file;

    file = prv_transformFILE(m_dataPrivate);
    written = fwrite(&value, sizeof(double), 1, file);
    assert(written == 1);
}

//------------------------------------------------------------------------------
//
void COutStreamBin::close(void)
{
    CFileBas::close((void **)&m_dataPrivate);
}
