/* -----------------------------------------------------------------------------
*  ILog.hpp
*  Author: Gabriel López García (gabriel DOT lg AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#ifndef ILOG_HPP_
#define ILOG_HPP_

class ILog
{
    public:
        ILog();
        virtual ~ILog();

        void setContext(const char *context);

        void line(const char *formato, ...);
        void error(const char *formato, ...);
        void warning(const char *textWarning);
        void errorWithLine(unsigned long line, const char *textError);
        void warningWithLine(unsigned long line, const char *textWarning);

    private:
        virtual void lineText(const char *text) = 0;

        struct prv_dataPrivateILog_t *m_dataPrivate;
};

#endif
