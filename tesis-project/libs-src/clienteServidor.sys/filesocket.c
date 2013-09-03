// Manejamos datos para enviar por cliente-servidor.

#include "filesocket.h"

#include "asrtbas.h"
#include "memory.h"
#include "strbas.h"
#include "hilo.h"
#include "arrcad.h"
#include <stdio.h>

enum i_tipo_apertura_t
{
    i_ESCRIBIENDO,
    i_LEYENDO
};

struct filesocket_t
{
    enum i_tipo_apertura_t tipo_apertura;
    
    unsigned long posicion_actual;
    ArrString *cadenas;
};

//---------------------------------------------------------

static struct filesocket_t *i_crea(
                    enum i_tipo_apertura_t tipo_apertura,
                    unsigned long posicion_actual,
                    ArrString **cadenas)
{
    struct filesocket_t *filesocket;
        
    filesocket = MALLOC(struct filesocket_t);
        
    filesocket->tipo_apertura = tipo_apertura;
    filesocket->posicion_actual = posicion_actual;
    filesocket->cadenas = ASSIGN_PP_NO_NULL(cadenas, ArrString);
        
    return filesocket;
}
 
//---------------------------------------------------------

struct filesocket_t *filesocket_abre_lectura(ArrString **cadenas)
{
    enum i_tipo_apertura_t tipo_apertura;
    unsigned long posicion_actual;

    tipo_apertura = i_LEYENDO;
    posicion_actual = 0;
    
    return i_crea(tipo_apertura, posicion_actual, cadenas);
}

//---------------------------------------------------------

struct filesocket_t *filesocket_abre_escritura(void)
{
    enum i_tipo_apertura_t tipo_apertura;
    unsigned long posicion_actual;
    ArrString *cadenas;

    tipo_apertura = i_ESCRIBIENDO;
    posicion_actual = 0;
    cadenas = arrcad_CreaString(0);
    
    return i_crea(tipo_apertura, posicion_actual, &cadenas);
}

//---------------------------------------------------------

void filesocket_destruye(struct filesocket_t **file)
{
    assert_no_null(file);
    assert_no_null(*file);
    
    arrcad_Destruye(&(*file)->cadenas);
    FREE_T(file, struct filesocket_t);
}

//---------------------------------------------------------

const ArrString *filesocket_contenido(const struct filesocket_t *file)
{
    assert_no_null(file);
    return file->cadenas;
}

//---------------------------------------------------------

void filesocket_escr_cadena(struct filesocketOut_t *fileOut, const char *cadena)
{
    struct filesocket_t *file;
        
    assert_no_null(fileOut);
    file = (struct filesocket_t *)fileOut;
    assert(file->tipo_apertura == i_ESCRIBIENDO);
    
    arrcad_Anyadir(file->cadenas, cadena);
    file->posicion_actual++;
}

//---------------------------------------------------------

void filesocket_escr_ushort(struct filesocketOut_t *fileOut, unsigned long id_codigo)
{
    char *cadena;
    struct filesocket_t *file;

    assert_no_null(fileOut);
    file = (struct filesocket_t *)fileOut;
    assert(file->tipo_apertura == i_ESCRIBIENDO);
    
    cadena = strbas_printf("%u", id_codigo);
    
    arrcad_Anyadir(file->cadenas, cadena);
    file->posicion_actual++;
    
    strbas_destroy(&cadena);
}

//---------------------------------------------------------

char *filesocket_lee_cadena(struct filesocketIn_t *fileIn)
{
    struct filesocket_t *file;
    const char *cadena;
    
    assert_no_null(fileIn);
    file = (struct filesocket_t *)fileIn;
    assert(file->tipo_apertura == i_LEYENDO);
    
    cadena = arrcad_Get(file->cadenas, file->posicion_actual);
    
    file->posicion_actual++;
    
    return strbas_copyString(cadena);
}

//---------------------------------------------------------

unsigned long filesocket_lee_ushort(struct filesocketIn_t *fileIn)
{
    struct filesocket_t *file;
    const char *cadena;
    unsigned int valor;
    int res;
    
    assert_no_null(fileIn);
    file = (struct filesocket_t *)fileIn;
        
    assert(file->tipo_apertura == i_LEYENDO);
    
    cadena = arrcad_Get(file->cadenas, file->posicion_actual);
    res = sscanf(cadena, "%u", &valor);
    hilo_assert(res == 1);
    
    file->posicion_actual++;
    
    return (unsigned long)valor;
}

//---------------------------------------------------------

