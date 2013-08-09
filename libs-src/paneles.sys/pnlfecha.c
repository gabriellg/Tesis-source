// Edici�n de fecha.

#include "pnlfecha.h"

#include "componente.inl"
#include "panel.inl"

#include "asrtbas.h"
#include "memory.h"
#include "strbas.h"
#include "fecha.h"
#include "arrcad.h"
#include "marcogrf.syh"

struct pnlfecha_t
{
    char *titulo;
    enum marcogrf_tipo_fecha_t tipo_fecha;

    struct fecha_t **fecha;
    
    struct item_implementacion_t *implementacion;
    struct marcogrf_t *marco;
};

//---------------------------------------------------------

static struct pnlfecha_t *i_crea(
                            const char *titulo,
                            enum marcogrf_tipo_fecha_t tipo_fecha,
                            struct fecha_t **fecha,
                            struct item_implementacion_t *implementacion,
                            struct marcogrf_t *marco)
{
    struct pnlfecha_t *pnlfecha;
        
    pnlfecha = MALLOC(struct pnlfecha_t);
        
    pnlfecha->titulo = strbas_copyString(titulo);
    pnlfecha->tipo_fecha = tipo_fecha;
    pnlfecha->fecha = fecha;
    pnlfecha->implementacion = implementacion;
    pnlfecha->marco = marco;
        
    return pnlfecha;
}

//---------------------------------------------------------

static void i_destruye(struct pnlfecha_t **pnlfecha)
{
    assert_no_null(pnlfecha);
    strbas_destroy(&(*pnlfecha)->titulo);
    
    FREE_T(pnlfecha, struct pnlfecha_t);
}

//---------------------------------------------------------

static struct item_implementacion_t *i_crea_implementacion(
                        struct marcogrf_t *marco, 
                        struct pnlfecha_t *pnlfecha, 
                        struct item_implementacion_dialogo_t *dialogo_so)
{
    assert_no_null(pnlfecha);
    assert_no_null(pnlfecha->fecha);
    
    assert(pnlfecha->implementacion == NULL);
    assert(pnlfecha->marco == NULL);
    
    pnlfecha->marco = marco;
    pnlfecha->implementacion = marcogrf_crea_fecha(marco, dialogo_so, pnlfecha->tipo_fecha, *(pnlfecha->fecha));
    
    return pnlfecha->implementacion;
}

//---------------------------------------------------------

static bool i_se_puede_aceptar(
                        const struct marcogrf_t *marco, 
                        struct pnlfecha_t *pnlfecha, 
                        char **mensaje_error)
{
    struct fecha_t *fecha;
    
    fecha = marcogrf_fecha(pnlfecha->marco, pnlfecha->implementacion);
    
    if (fecha != NULL)
    {
        fecha_destruye(&fecha);
        return true;
    }
    else
    {
        if (mensaje_error != NULL)
            *mensaje_error = strbas_printf("%s: %s", pnlfecha->titulo, "Incorrecto");
        
        return false;
    }
}

//---------------------------------------------------------

static void i_aceptar(const struct marcogrf_t *marco, struct pnlfecha_t *pnlfecha)
{
    struct fecha_t *fecha;
        
    assert_no_null(pnlfecha);
    
    fecha = marcogrf_fecha(pnlfecha->marco, pnlfecha->implementacion);
    assert_no_null(fecha);
    
    fecha_destruye(pnlfecha->fecha);
    *(pnlfecha->fecha) = fecha;
}

//---------------------------------------------------------

static ArrString *i_crea_array_textos(const char *texto)
{
    ArrString *textos;
        
    textos = arrcad_CreaString(1);
    arrcad_Set(textos, 0, texto);
    
    return textos;
}

//---------------------------------------------------------

static ArrString *i_get_titulo(const struct pnlfecha_t *pnlfecha)
{
    assert_no_null(pnlfecha);
    return i_crea_array_textos(pnlfecha->titulo);
}

//---------------------------------------------------------

static char *i_texto_fecha_segun_tipo_fecha(
                    enum marcogrf_tipo_fecha_t tipo_fecha, 
                    const struct fecha_t *fecha_actual)
{
    char *texto;
    
    switch(tipo_fecha)
    {
        case MARCOGRF_FECHA:
        
            texto = fecha_texto_dia(fecha_actual);
            break;
        
        case MARCOGRF_HORA:
        
            texto = fecha_texto_hora(fecha_actual);
            break;
        
        case MARCOGRF_FECHA_Y_HORA:
        
            texto = fecha_texto_dia_y_hora(fecha_actual);
            break;

        default_error();
    }
    
    return texto;
}

//---------------------------------------------------------

static ArrString *i_get_valor(const struct pnlfecha_t *pnlfecha)
{
    ArrString *textos;
    char *texto_fecha;
    struct fecha_t *fecha_actual;
    
    assert_no_null(pnlfecha);
    
    if (pnlfecha->implementacion != NULL)
    {
        fecha_actual = marcogrf_fecha(pnlfecha->marco, pnlfecha->implementacion);
    }
    else
    {
        assert_no_null(pnlfecha->fecha);
        fecha_actual = fecha_copia(*(pnlfecha->fecha));
    }
    
    texto_fecha = i_texto_fecha_segun_tipo_fecha(pnlfecha->tipo_fecha, fecha_actual);
    
    textos = i_crea_array_textos(texto_fecha);
    
    fecha_destruye(&fecha_actual);
    
    return textos;
}

//---------------------------------------------------------

static struct panel_t *i_crea_fecha(
                        enum marcogrf_tipo_fecha_t tipo_fecha, 
                        const char *titulo, 
                        struct fecha_t **fecha)
{
    struct pnlfecha_t *pnlfecha;
    struct item_implementacion_t *implementacion;
    struct marcogrf_t *marco;
    struct componente_t *componente;

    implementacion = NULL;
    marco = NULL;
    
    pnlfecha = i_crea(titulo, tipo_fecha, fecha, implementacion, marco); 
    
	componente = componente_crea_defecto(pnlfecha_t, &pnlfecha, i_crea_implementacion, i_destruye, i_se_puede_aceptar, i_aceptar, i_get_titulo, i_get_valor);
	return panel_crea_componente(&componente);
}

//---------------------------------------------------------

struct panel_t *pnlfecha_crea_hora(const char *titulo, struct fecha_t **fecha)
{
    return i_crea_fecha(MARCOGRF_HORA, titulo, fecha);
}

//---------------------------------------------------------

struct panel_t *pnlfecha_crea_dia_y_hora(const char *titulo, struct fecha_t **fecha)
{
    return i_crea_fecha(MARCOGRF_FECHA_Y_HORA, titulo, fecha);
}
