/* -----------------------------------------------------------------------------
*  CArgumentParser
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#ifndef CARGUMENTPARSER_HPP_
#define CARGUMENTPARSER_HPP_

class CArgumentParser
{
    public:
        CArgumentParser( const char* programDescription );
        ~CArgumentParser();

        void addArgFlag( const char* option, const char* description );
        void addArgChar( const char* option, const char* description );
        void addArgChar_optional( const char* option, const char* description, char defaultChar, bool showDefault );
        void addArgStr( const char* option, const char* description );
        void addArgStr_optional( const char* option, const char* description, const char* defaultStr, bool showDefault );
        void addArgInt( const char* option, const char* description );
        void addArgInt_optional( const char* option, const char* description, int defaultValue, bool showDefault );
        void addArgULong( const char* option, const char* description );
        void addArgULong_optional( const char* option, const char* description, unsigned long defaultValue, bool showDefault );
        void addArgDouble( const char* option, const char* description );
        void addArgDouble_optional( const char* option, const char* description, double defaultValue, bool showDefault );
        void addDescription( const char* description );

        bool validateParse( int argc, const char *argv[], class ILog *log_optional );

        bool isProcessed( const char* option ) const;

        bool getArgFlag( const char* option ) const;
        char getArgChar( const char* option ) const;
        char* getArgStr( const char* option ) const;
        int getArgInt( const char* option ) const;
        unsigned long getArgULong( const char* option ) const;
        double getArgDouble( const char* option ) const;

    private:
        struct prv_argumentParser_t *m_data;
};

#endif
