// Definici�n de enums 

#include "gesenum.h"
#include "gesenum.inl"

#include "asrtbas.h"
#include "strbas.h"
#include "nomfich.h"

//-----------------------------------------------------------------------

bool gesenum_es_cadena_sistema_operativo(const char *cadena_sistema_operativo, enum gesenum_sistema_operativo_t *sistema_operativo)
{
    bool es_cadena_sistema_operativo;

    assert_no_null(sistema_operativo);

    if (strbas_equalsStringWithoutMatchCase(cadena_sistema_operativo, "win") == true)
    {
        *sistema_operativo = GESENUM_COMPILADOR_WIN32;
        es_cadena_sistema_operativo = true;
    }
    else if (strbas_equalsStringWithoutMatchCase(cadena_sistema_operativo, "linux") == true)
    {
        *sistema_operativo = GESENUM_COMPILADOR_LINUX;
        es_cadena_sistema_operativo = true;
    }
    else
        es_cadena_sistema_operativo = false;

    return es_cadena_sistema_operativo;
}

//-----------------------------------------------------------------------

bool gesenum_es_dependiente_sistema_operativo(const char *nombre_proyecto, enum gesenum_sistema_operativo_t *sistema_operativo)
{
    bool es_dependiente_sistema_operativo;
    char *extension;

    assert_no_null(sistema_operativo);

    extension = nomfich_extension(nombre_proyecto);

    if (strbas_equalsStringWithoutMatchCase(extension, "win") == true)
    {
        *sistema_operativo = GESENUM_COMPILADOR_WIN32;
        es_dependiente_sistema_operativo = true;
    }
    else if (strbas_equalsStringWithoutMatchCase(extension, "lnx") == true)
    {
        *sistema_operativo = GESENUM_COMPILADOR_LINUX;
        es_dependiente_sistema_operativo = true;
    }
    else
        es_dependiente_sistema_operativo = false;

    return es_dependiente_sistema_operativo;
}

//-----------------------------------------------------------------------

const char *gesenum_prefijo_segun_sistema_operativo(enum gesenum_sistema_operativo_t sistema_operativo)
{
    const char *extension_directorio;

    switch (sistema_operativo) 
    { 
        case GESENUM_COMPILADOR_WIN32:
        
            extension_directorio = "win";
            break;

        case GESENUM_COMPILADOR_LINUX:
        
            extension_directorio = "lnx";
            break;

        default_error(); 
    }

    return extension_directorio;
}
