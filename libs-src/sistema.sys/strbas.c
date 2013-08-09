/* -----------------------------------------------------------------------------
*  strbas (Layer of string.h)
*  Author: Gabriel López García (gabriellg AT gmail DOT com)
*  License: GNU Public License
* ----------------------------------------------------------------------------*/

#include "strbas.h"

#include "asrtbas.h"
#include "memory.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdarg.h"
#include "string.h"
#include "ctype.h"

#define MAX_BUFFER 10240


char *strbas_create(size_t numChars)
{
    char *str;

    str = CALLOC(numChars + 1, char);
    memset(str,'\0',numChars);
    str[numChars] = '\0';

    return str;
}

//------------------------------------------------------------------------------

char *strbas_createWithInitialitation(char car, size_t numChars)
{
    char *str;

    assert(numChars > 0);

    str = CALLOC(numChars + 1, char);
    memset(str,car,numChars);

    str[numChars] = '\0';

    return str;
}

//------------------------------------------------------------------------------

void strbas_destroy(char **str)
{
    assert_no_null(str);
    assert_no_null(*str);
    FREE_T(str, char);
}

//------------------------------------------------------------------------------

char *strbas_copyString(const char *str)
{
    char *copia;
    size_t len;

    assert_no_null(str);
    len = strlen(str);

    copia = strbas_create(len);
    strncpy(copia,str,len);

    return copia;
}

//------------------------------------------------------------------------------

char *strbas_copyNumberChar(const char *str, size_t numChars)
{
    char *copia;
    size_t len;

    assert_no_null(str);
    len = strlen(str);
    assert(numChars <= len);

    copia = strbas_create(numChars);
    strncpy(copia, str, numChars);

    return copia;
}

//------------------------------------------------------------------------------

char *strbas_copyOptionalNull(const char *str)
{
    if (str != NULL)
        return strbas_copyString(str);
    else
        return NULL;
}

//------------------------------------------------------------------------------

char *strbas_printf(const char *format,...)
{
    char buffer[MAX_BUFFER];
    va_list args;
    int escritos;

    assert_no_null(format);

    va_start(args,format);

    escritos = vsnprintf(buffer,MAX_BUFFER,format,args);
    assert(escritos >= 0 && escritos < MAX_BUFFER);

    va_end(args);

    return strbas_copyString(buffer);
}

//------------------------------------------------------------------------------

bool strbas_equalsStrings(const char *str1,const char *str2)
{
    assert_no_null(str1);
    assert_no_null(str2);

    if (strcmp(str1,str2) == 0)
        return true;
    else
        return false;
}

//------------------------------------------------------------------------------

bool strbas_equalsStringWithoutMatchCase(const char *str1,const char *str2)
{
    assert_no_null(str1);
    assert_no_null(str2);

    if (strcasecmp(str1,str2) == 0)
        return true;
    else
        return false;
}

//------------------------------------------------------------------------------

size_t strbas_length(const char *str)
{
    assert_no_null(str);
    return strlen(str);
}

//------------------------------------------------------------------------------

char *strbas_endExtractFromChar(const char *str, int c)
{
    const char *foundStr;

    assert_no_null(str);

    foundStr = strrchr(str, c);

    if (foundStr != NULL)
        return strbas_copyString(&foundStr[1]);
    else
        return NULL;
}

//------------------------------------------------------------------------------

char *strbas_initialExtractUntilChar(const char *str, int c)
{
    const char *foundStr;

    assert_no_null(str);

    foundStr = strrchr(str, c);

    if (foundStr != NULL)
    {
        char *initialStr;
        size_t pos;

        initialStr = strbas_copyString(str);
        pos = (size_t)(foundStr - str);
        initialStr[pos] = '\0';

        return initialStr;
    }
    else
        return NULL;
}

//------------------------------------------------------------------------------

void strbas_replaceChar(char *str, int charEnstr, int charASustituir)
{
    char *ptr, *foundStr;

    assert_no_null(str);
    assert(charEnstr != charASustituir);

    ptr = str;

    do
    {
        foundStr = strrchr(ptr, charEnstr);

        if (foundStr != NULL)
        {
            *foundStr = charASustituir;
            ptr = foundStr + 1;
        }
    }
    while(foundStr != NULL);
}

//--------------------------------------------------------------------------------------------

char *strbas_replaceString(const char *entrada, const char *buscar, const char *remplazar)
{
    char *salida_recortada;
    char *salida;
    const char *p;
    size_t long_entrada, long_buscar, long_remplazar;
    size_t long_salida, j;

    long_entrada   = strlen(entrada);
    long_buscar    = strlen(buscar);
    long_remplazar = strlen(remplazar);

    assert(long_buscar > 0);

    if (long_buscar >= long_remplazar)
        long_salida = long_entrada;
    else
        long_salida = (long_entrada / long_buscar) * long_remplazar + long_entrada % long_buscar;

    salida = strbas_createWithInitialitation(0xEE, long_salida + 1);

    j = 0;

    do
    {
        p = strstr(entrada, buscar);

        if (p == NULL)
        {
            j += sprintf(salida + j, "%s", entrada);
        }
        else
        {
            unsigned long numCharsACopiar;

            assert(p >= entrada);
            numCharsACopiar = p - entrada;

            memcpy(salida + j, entrada, numCharsACopiar);

            j += numCharsACopiar;
            j += sprintf(salida + j, "%s", remplazar);

            entrada = p + long_buscar;
        }
    }
    while (p != NULL);

    assert(j < long_salida + 1);

    if (j < long_salida + 1)
        assert(salida[j+1] == (char)0xEE || salida[j+1] == (char)0x00);

    salida[j+1] = '\0';

    salida_recortada = strbas_copyString(salida);

    FREE_T(&salida, char);

    return salida_recortada;
}

//------------------------------------------------------------------------------

void strbas_destroyStringList(char ***lista_strs, unsigned long num_strs)
{
    unsigned long ind;

    assert_no_null(lista_strs);
    assert(num_strs > 0);

    for (ind = 0; ind < num_strs; ind++)
    {
        char *str;

        str = (*lista_strs)[ind];

        strbas_destroy(&str);
    }

    FREE_T(lista_strs, char *);
}

//------------------------------------------------------------------------------

char *strbas_LTrim(const char *str)
{
    size_t longitud, i;

    assert_no_null(str);
    longitud = strlen(str);

    for (i = 0; i < longitud; i++)
    {
        if (str[i] != ' ')
            return strbas_copyString(&str[i]);
    }

    return strbas_copyString("");
}

//------------------------------------------------------------------------------

char *strbas_RTrim(const char *str)
{
    size_t longitud, i;

    assert_no_null(str);
    longitud = strlen(str);

    for (i = 0; i < longitud; i++)
    {
        size_t ind;

        ind = longitud - i - 1;

        if (str[ind] != ' ')
        {
            char *rtrim;

            rtrim = strbas_copyString(str);
            if (ind < longitud - 1)
                rtrim[ind+1] = '\0';

            return rtrim;
        }
    }

    return strbas_copyString("");
}

//------------------------------------------------------------------------------

bool strbas_isInitialSubstring(const char *str, const char *substr)
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

bool strbas_isSubstring(const char *str, const char *substr)
{
    assert_no_null(str);
    assert_no_null(substr);

    if (strstr(str, substr) != NULL)
        return true;
    else
        return false;
}

//------------------------------------------------------------------------------

const char *strbas_isBeginSubstring(const char *str, const char *substr)
{
    assert_no_null(str);
    assert_no_null(substr);

    return strstr(str, substr);
}

//------------------------------------------------------------------------------

char *strbas_subtractionSubstring(const char *str, const char *substr)
{
    const char *foundStr;
    char *principio, *final, *concatenado;
    size_t pos, longitud_inicial, longitud_final,
            longitud_str, longitud_substr, pos_final;

    assert_no_null(str);
    assert_no_null(substr);

    longitud_str = strlen(str);
    longitud_substr = strlen(substr);

    assert(longitud_str >= longitud_substr);

    foundStr = strstr(str, substr);

    pos = (size_t)(foundStr - str);
    pos_final = pos + longitud_substr;

    principio = strdup(str);
    assert_no_null(principio);

    principio[pos] = '\0';

    if (pos_final < longitud_str)
        final = strdup(&str[pos_final]);
    else
        final = strdup("");

    longitud_inicial = strlen(principio);
    longitud_final = strlen(final);

    concatenado = strbas_create(longitud_inicial + longitud_final);
    strcpy(concatenado, principio);
    strcat(concatenado, final);

    strbas_destroy(&principio);
    strbas_destroy(&final);

    return concatenado;
}

//--------------------------------------------------------------------------------------------

bool strbas_getStringBetweenChar(
                        const char *str,
                        const char caracter_inicial,
                        const char caracter_final,
                        char **str_entre_caracteres)
{
    const char *foundStr;
    char *str_loc;
    size_t longitud_str;
    bool resp;

    assert_no_null(str);
    assert_no_null(str_entre_caracteres);

    longitud_str = strbas_length(str);
    str_loc = strbas_create(longitud_str);

    foundStr = strchr(str, caracter_inicial);

    if (foundStr != NULL)
    {
        size_t pos_inicial, i;

        pos_inicial = (size_t)(foundStr - str + 1);

        for (i = pos_inicial; i < longitud_str; i++)
        {
            if (str[i] != caracter_final)
                str_loc[i - pos_inicial] = str[i];
            else
            {
                str_loc[i - pos_inicial] = '\0';
                break;
            }
        }

        if (i < longitud_str)
        {
            *str_entre_caracteres = strbas_copyString(str_loc);
            resp = true;
        }
        else
            resp = false;
    }
    else
        resp = false;

    strbas_destroy(&str_loc);

    return resp;
}

//--------------------------------------------------------------------------------------------

void strbas_assignMessage(char **mensaje_error, const char *mensaje)
{
    if (mensaje_error != NULL)
        *mensaje_error = strbas_copyString(mensaje);
}

//--------------------------------------------------------------------------------------------

void strbas_concatenate(char **str, const char *substr)
{
    assert_no_null(str);
    assert_no_null(substr);

    if (*str != NULL)
    {
        char *str_concatenada;
        size_t num_total;

        num_total = strlen(*str) + strlen(substr);
        str_concatenada = strbas_create(num_total);
        strcpy(str_concatenada, *str);
        strcat(str_concatenada, substr);

        strbas_destroy(str);
        *str = str_concatenada;
    }
    else
        *str = strbas_copyString(substr);
}

//--------------------------------------------------------------------------------------------

bool strbas_charIsInString(const char *str, int caracter)
{
    assert_no_null(str);

    if (strchr(str, caracter) != NULL)
        return true;
    else
        return false;
}

//--------------------------------------------------------------------------------------------

char *strbas_nextToken(char *str, const char *delimitadores)
{
    assert_no_null(delimitadores);
    return strtok(str, delimitadores);
}

//--------------------------------------------------------------------------------------------

const char *strbas_getEnv(const char *nombre_sistema)
{
    return getenv(nombre_sistema);
}

//--------------------------------------------------------------------------------------------

unsigned long strbas_setEnv(const char *nombreSistema, unsigned long valorDefecto)
{
    unsigned long valor;
    const char *txtValorSistema;

    txtValorSistema = getenv(nombreSistema);
    if (txtValorSistema != NULL)
    {
        int res;

        res = sscanf(txtValorSistema, "%ld", &valor);
        if (res != 1)
            valor = valorDefecto;
    }
    else
        valor = valorDefecto;

    return valor;
}

//--------------------------------------------------------------------------------------------

void strbas_toUppercase(char *str)
{
    char *ptr;

    ptr = str;

    while (*ptr != '\0')
    {
        *ptr = (char)toupper(*ptr);
        ptr++;
    }
}
