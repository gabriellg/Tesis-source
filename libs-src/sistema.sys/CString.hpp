/* -----------------------------------------------------------------------------
*  CString
*  Author: Antonio Javier Gallego Sánchez (ajgallego AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

class CString
{
    public:
        static char* create(unsigned long numChars);
        static char* createWithInitialitation(char car, unsigned long numChars);
        static void free(char **str);
        static char* copy(const char *str);
        static unsigned long size(const char *str);

		static void assignIfNoNull(char **dst, const char *str);

        static void concatenate(char **str, const char *substr);
        static void concatenatePrefix(const char *substr, char **str);
        static void concatenateChar(char **str, char car);
        static void concatenatePrefixChar(char car, char **str);

        static bool equal(const char *str1,const char *str2);
        static bool equalWithoutMatchCase(const char *str1,const char *str2);

        static bool findChar( const char* str, char c, unsigned long *indFirst_optional );
        static bool rFindChar( const char* str, char c, unsigned long *indLast_optional );

        static char* toLower( const char* str );
        static char* toUpper( const char* str );

        static char* substring( const char* str, unsigned long begin, unsigned long length );
        static char* getPrefix( const char* str, unsigned long numChars );
        static char* getSuffix( const char* str, unsigned long numChars );

        static char* initialExtractUntilChar(const char *str, int letter);
        static char* endExtractFromChar(const char *str, char car);

        static void substChar( char **str, char charSearch, char charReplace);

        static char* trim( const char* str );
        static char* leftTrim( const char* str );
        static char* rightTrim( const char* str );

        static bool isInitialSubstring( const char *str, const char *substr );
        static bool isSubstring( const char *str, const char *substr );

        static char *printf(const char *format,...);
        static int scanf(const char *str, const char *format, ...);
};
