/* -----------------------------------------------------------------------------
*  strbas (Layer of string.h)
*  Author: Gabriel López García (gabriellg AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "stddef.h"

#ifdef __cplusplus
extern "C" {
#endif

char *strbas_create(size_t numChars);

char *strbas_createWithInitialitation(char car, size_t numChars);

void strbas_destroy(char **str);

char *strbas_copyString(const char *str);

char *strbas_copyNumberChar(const char *str, size_t numChars);

char *strbas_copyOptionalNull(const char *str);

char *strbas_printf(const char *format,...);

bool strbas_equalsStrings(const char *str1,const char *str2);

bool strbas_equalsStringWithoutMatchCase(const char *str1,const char *str2);

size_t strbas_length(const char *cad);

char *strbas_endExtractFromChar(const char *str, int c);

char *strbas_initialExtractUntilChar(const char *str, int c);

void strbas_replaceChar(char *inputStr, int search, int replace);

char *strbas_replaceString(const char *inputStr, const char *search, const char *replace);

void strbas_destroyStringList(char ***stringLinst, unsigned long numStrings);

char *strbas_LTrim(const char *str);

char *strbas_RTrim(const char *str);

bool strbas_isInitialSubstring(const char *str, const char *substr);

bool strbas_isSubstring(const char *str, const char *substr);

const char *strbas_isBeginSubstring(const char *str, const char *substr);

char *strbas_subtractionSubstring(const char *str, const char *substr);

bool strbas_getStringBetweenChar(
                        const char *str,
                        const char firstChar,
                        const char endChar,
                        char **str_between_chars);

void strbas_assignMessage(char **error_message, const char *message);

void strbas_concatenate(char **str, const char *substr);

bool strbas_charIsInString(const char *str, int character);

char *strbas_nextToken(char *str, const char *delimiters);

const char *strbas_getEnv(const char *systemName);

unsigned long strbas_setEnv(const char *systemName, unsigned long defaultValue);

void strbas_toUppercase(char *str);

#ifdef __cplusplus
}
#endif
