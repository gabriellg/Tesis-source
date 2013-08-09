/* -----------------------------------------------------------------------------
*  CStringTokenizer
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

class CStringTokenizer
{
    public:
        CStringTokenizer( const char* str );
        CStringTokenizer( const char* str, char delimiter,
                          bool remove_repeated_delimiters );
        ~CStringTokenizer();

        bool hasMoreTokens() const;

        unsigned int numTokens() const;

        const char* getNextToken();
        const char* peekNextToken() const;

        class CArrayString* getArrayOfTokens() const;

    private:
        struct prv_stringTokenizer_t *m_data;
};
