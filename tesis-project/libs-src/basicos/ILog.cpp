/* -----------------------------------------------------------------------------
*  ILog.cpp
*  Author: Gabriel López García (gabriel DOT lg AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "ILog.hpp"

#include "stdio.h"
#include "stdarg.h"
#include "CString.hpp"
#include "asrtbas.h"
#include "memory.h"

#define MAX_BUFFER 4096

static const char *prv_ERROR = "error";
static const char *prv_WARNING = "warning";

struct prv_dataPrivateILog_t
{
    char *currentContext;
};

//-----------------------------------------------------------------------

static struct prv_dataPrivateILog_t *prv_create(const char *currentContext)
{
    struct prv_dataPrivateILog_t *dataPrivate;

    dataPrivate = MALLOC(struct prv_dataPrivateILog_t);

    dataPrivate->currentContext = CString::copy(currentContext);

    return dataPrivate;
}

//-----------------------------------------------------------------------

static void prv_destroy(struct prv_dataPrivateILog_t **dataPrivate)
{
    assert_no_null(dataPrivate);
    assert_no_null(*dataPrivate);

    CString::free(&(*dataPrivate)->currentContext);

    FREE_T(dataPrivate, struct prv_dataPrivateILog_t);
}

//-----------------------------------------------------------------------

ILog::ILog()
{
    m_dataPrivate = prv_create("");
}

//-----------------------------------------------------------------------

ILog::~ILog()
{
    prv_destroy(&m_dataPrivate);
}

//------------------------------------------------------------------------------
//
static char *prv_stringMessage(const char *type, const char *context,
                                unsigned long line, const char *textError)
{
    char *message;

    message = CString::copy(context);

    if (CString::size(context) > 0)
        CString::concatenate(&message, ": ");

    CString::concatenate(&message, type);
    CString::concatenate(&message, ": ");

    if (line > 0)
    {
        char *txtLine;

        txtLine = CString::printf(": (%d) %s ", line, textError);
        CString::concatenate(&message, txtLine);
        CString::free(&txtLine);
    }
    else
        CString::concatenate(&message, textError);

    return message;
}

//------------------------------------------------------------------------------
//
void ILog::setContext(const char *context)
{
    assert_no_null(m_dataPrivate);

    CString::free(&m_dataPrivate->currentContext);
    m_dataPrivate->currentContext = CString::copy(context);
}

//------------------------------------------------------------------------------
//
void ILog::line(const char *formato, ...)
{
    char buffer[MAX_BUFFER];
    va_list args;
    int escritos;

    assert_no_null(formato);

    va_start(args, formato);

    escritos = vsnprintf(buffer, MAX_BUFFER, formato, args);
    assert(escritos > 0);
    lineText(buffer);

    va_end(args);
}

//------------------------------------------------------------------------------
//
void ILog::error(const char *formato, ...)
{
    char *stringError;
    char buffer[MAX_BUFFER];
    va_list args;
    int escritos;

    assert_no_null(formato);
    assert_no_null(m_dataPrivate);

    va_start(args, formato);

    escritos = vsnprintf(buffer, MAX_BUFFER, formato, args);
    assert(escritos > 0);

    stringError = prv_stringMessage(prv_ERROR, m_dataPrivate->currentContext, 0, buffer);
    lineText(stringError);

    va_end(args);
    CString::free(&stringError);
}

//------------------------------------------------------------------------------
//
void ILog::warning(const char *textError)
{
    char *stringWarning;

    assert_no_null(m_dataPrivate);

    stringWarning = prv_stringMessage(prv_WARNING, m_dataPrivate->currentContext, 0, textError);
    lineText(stringWarning);
    CString::free(&stringWarning);
}

//------------------------------------------------------------------------------
//
void ILog::errorWithLine(unsigned long line, const char *textError)
{
    char *stringError;

    stringError = prv_stringMessage(prv_ERROR, m_dataPrivate->currentContext, line, textError);
    lineText(stringError);
    CString::free(&stringError);
}

//------------------------------------------------------------------------------
//
void ILog::warningWithLine(unsigned long line, const char *textError)
{
    char *stringWarning;

    stringWarning = prv_stringMessage(prv_WARNING, m_dataPrivate->currentContext, line, textError);
    lineText(stringWarning);
    CString::free(&stringWarning);
}
