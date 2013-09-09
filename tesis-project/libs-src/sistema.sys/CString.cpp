/* -----------------------------------------------------------------------------
*  CString
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "CString.hpp"

#include "stdio.h"
#include "stdlib.h"
#include "stdarg.h"
#include "string.h"
#include "ctype.h"

#include "asrtbas.h"
#include "memory.h"

#define MAX_BUFFER 10240

//------------------------------------------------------------------------------
//
char* CString::create(unsigned long numChars)
{
    char *str;

    str = CALLOC(numChars + 1, char);
    memset(str,'\0',numChars + 1);

    return str;
}

//------------------------------------------------------------------------------
//
char* CString::createWithInitialitation(char car, unsigned long numChars)
{
    char *str;

    assert(numChars > 0);

    str = CALLOC(numChars + 1, char);
    memset(str, car, numChars);

    str[numChars] = '\0';

    return str;
}

//------------------------------------------------------------------------------
//
void CString::free(char **str)
{
    assert_no_null(str);
    assert_no_null(*str);
    FREE_T(str, char);
}

//------------------------------------------------------------------------------
//
void CString::deleteArrayStrings(char ***strings, unsigned long numStrings)
{
    char **string_loc;

    assert_no_null(strings);
    assert_no_null(*strings);

    string_loc = *strings;

    for (unsigned long i = 0; i < numStrings; i++)
        CString::free(&string_loc[i]);

    FREE_T(strings, char *);
}

//------------------------------------------------------------------------------
//
char* CString::copy(const char *str)
{
    char *strCopy;
    size_t len;

    assert_no_null(str);
    len = strlen(str);

    strCopy = CALLOC(len + 1, char);
    strncpy(strCopy, str, len);

    return strCopy;
}

//------------------------------------------------------------------------------
//
unsigned long CString::size(const char *str)
{
    assert_no_null(str);
    return strlen(str);
}

//------------------------------------------------------------------------------
//
void CString::assignIfNoNull(char **dst, const char *str)
{
    if (dst != NULL)
        *dst = CString::copy(str);
    else
        *dst = NULL;
}

//------------------------------------------------------------------------------
//
void CString::concatenate( char **str, const char *substr )
{
    assert_no_null(str);
    assert_no_null(substr);

    if (*str != NULL)
    {
        char *str_concatenada;
        size_t num_total;

        num_total = strlen(*str) + strlen(substr);
        str_concatenada = CString::create(num_total);

        strcpy(str_concatenada, *str);
        strcat(str_concatenada, substr);

        CString::free(str);
        *str = str_concatenada;
    }
    else
        *str = CString::copy(substr);
}

//------------------------------------------------------------------------------
//
void CString::concatenatePrefix( const char *substr, char **str )
{
    assert_no_null(str);
    assert_no_null(substr);

    if (*str != NULL)
    {
        char *str_concatenada;
        size_t num_total;

        num_total = strlen(*str) + strlen(substr);
        str_concatenada = CString::create(num_total);

        strcpy(str_concatenada, substr);
        strcat(str_concatenada, *str);

        CString::free(str);
        *str = str_concatenada;
    }
    else
        *str = CString::copy(substr);
}

//------------------------------------------------------------------------------
//
void CString::concatenateChar(char **str, char car)
{
    CString::concatenate( str, CString::printf( "%c", car ) );
}

//------------------------------------------------------------------------------
//
void CString::concatenatePrefixChar(char car, char **str)
{
    CString::concatenatePrefix( CString::printf( "%c", car ), str );
}

//------------------------------------------------------------------------------
//
bool CString::equal(const char *str1,const char *str2)
{
    assert_no_null(str1);
    assert_no_null(str2);

    if (strcmp(str1,str2) == 0)
        return true;
    else
        return false;
}

//------------------------------------------------------------------------------
//
bool CString::equalWithoutMatchCase(const char *str1,const char *str2)
{
    assert_no_null(str1);
    assert_no_null(str2);

    if (strcasecmp(str1,str2) == 0)
        return true;
    else
        return false;
}

//------------------------------------------------------------------------------
//
bool CString::findChar( const char* str, char c, unsigned long *indFirst_optional )
{
    unsigned long len;

    len = CString::size( str );

    for( unsigned long ind = 0; ind < len; ++ind )
    {
        if( str[ ind ] == c )
        {
            if( indFirst_optional != NULL )
                *indFirst_optional = ind;

            return true;
        }
    }

    return false;
}

//------------------------------------------------------------------------------
//
bool CString::rFindChar( const char* str, char c, unsigned long *indLast_optional )
{
    unsigned long len;

    len = CString::size( str );

    for( long int ind = len - 1; ind >= 0; --ind ) // IMPORTANT: DONT USE UNSIGNED
    {
        if( str[ ind ] == c )
        {
            if( indLast_optional != NULL )
                *indLast_optional = ind;

            return true;
        }
    }

    return false;
}

//------------------------------------------------------------------------------
//
static void prv_transformCharacters(char *str, int (*func_transformation)(int car))
{
    char *ptr;

    assert_no_null(func_transformation);

    ptr = str;

    while (*ptr != '\0')
    {
        *ptr = (char)func_transformation(*ptr);
        ptr++;
    }
}

//------------------------------------------------------------------------------
//
char* CString::toLower( const char* str )
{
    char *strLower;

    strLower = CString::copy(str);
    prv_transformCharacters(strLower, tolower);

    return strLower;
}

//------------------------------------------------------------------------------
//
char* CString::toUpper(const char* str)
{
    char *strUpper;

    strUpper = CString::copy(str);
    prv_transformCharacters(strUpper, toupper);

    return strUpper;
}

//------------------------------------------------------------------------------
//
char* CString::substring( const char* str, unsigned long begin, unsigned long length )
{
    assert_no_null( str );
    assert( CString::size( str ) >= begin + length );

    return strndup( str + begin, length );
}

//------------------------------------------------------------------------------
//
char* CString :: getPrefix( const char* str, unsigned long numChars )
{
    assert(numChars < strlen(str));
    return strndup(str, numChars);
}

//------------------------------------------------------------------------------
//
char* CString :: getSuffix( const char* str, unsigned long numChars )
{
    unsigned long len, indPosBegin;

    len = CString::size(str);
    assert(numChars < len);
    indPosBegin = len - numChars;

    return strndup(str + indPosBegin, numChars);
}

//------------------------------------------------------------------------------
//
char *CString::initialExtractUntilChar(const char *str, int letter)
{
    const char *foundStr;

    assert_no_null(str);

    foundStr = strrchr(str, letter);

    if (foundStr != NULL)
    {
        char *initialStr;
        size_t pos;

        initialStr = CString::copy(str);
        pos = (size_t)(foundStr - str);
        initialStr[pos] = '\0';

        return initialStr;
    }
    else
        return NULL;
}

//------------------------------------------------------------------------------
//
char* CString::endExtractFromChar(const char *str, char car)
{
    const char *foundStr;

    assert_no_null(str);

    foundStr = strrchr(str, car);

    if (foundStr != NULL)
        return CString::copy(&foundStr[1]);
    else
        return NULL;
}

//------------------------------------------------------------------------------
//
void CString :: substChar( char **str, char charSearch, char charReplace)
{
    char *ptr;

    assert_no_null(str);
    assert(charSearch != charReplace);

    ptr = *str;

    while (*ptr != '\0')
    {
        if( *ptr == charSearch )
            *ptr = charReplace;
        ptr++;
    }
}

//------------------------------------------------------------------------------
//
char* CString::leftTrim( const char* str )
{
    size_t len, i;

    assert_no_null(str);
    len = CString::size(str);

    for (i = 0; i < len; ++i)
    {
        if (str[i] != ' ')
            return CString::copy(&str[i]);
    }

    return CString::copy("");
}

//------------------------------------------------------------------------------
//
char* CString::rightTrim(const char* str)
{
    size_t len, i;

    assert_no_null(str);
    len = CString::size(str);

    for (i = 0; i < len; ++i)
    {
        size_t ind;

        ind = len - i - 1;

        if (str[ind] != ' ')
        {
            char *rtrim;

            rtrim = CString::copy(str);

            if (ind < len - 1)
                rtrim[ind+1] = '\0';

            return rtrim;
        }
    }

    return CString::copy("");
}

//------------------------------------------------------------------------------
//
char* CString::trim(const char* str)
{
    char *leftTrim, *trim;

    leftTrim = CString::leftTrim(str);
    trim = CString::rightTrim(leftTrim);

    CString::free(&leftTrim);

    return trim;
}


//------------------------------------------------------------------------------
//
bool CString :: isInitialSubstring( const char *str, const char *substr )
{
    const char *foundStr;

    assert_no_null(str);
    assert_no_null(substr);

    foundStr = strstr(str, substr);

    if (foundStr != NULL)
    {
        if (foundStr == str)
            return true;
        else
            return false;
    }
    else
        return false;
}

//------------------------------------------------------------------------------
//
bool CString :: isSubstring( const char *str, const char *substr )
{
    assert_no_null(str);
    assert_no_null(substr);

    if (strstr(str, substr) != NULL)
        return true;
    else
        return false;
}

//------------------------------------------------------------------------------
//
char *CString::printf(const char *format,...)
{
    char buffer[MAX_BUFFER];
    va_list args;
    int written;

    assert_no_null(format);

    va_start(args, format);

    written = vsnprintf(buffer,MAX_BUFFER,format,args);
    assert(written >= 0 && written < MAX_BUFFER);

    va_end(args);

    return CString::copy(buffer);
}

//------------------------------------------------------------------------------
//
int CString::scanf(const char *str, const char *format, ...)
{
    va_list args;
    int numRead;

    assert_no_null(format);

    va_start(args, format);
    numRead = vsscanf(str, format, args);
    va_end(args);

    return numRead;
}

