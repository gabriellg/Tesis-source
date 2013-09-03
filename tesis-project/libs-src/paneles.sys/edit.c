// Definicion de edit

#include "edit.h"

#include "componente.inl"
#include "panel.inl"

#include "asrtbas.h"
#include "memory.h"
#include "array.h"
#include "arrcad.h"
#include "strbas.h"
#include "marcogrf.syh"

#include "stdio.h"

enum i_tipo_dato_t
{
    i_DOUBLE,
    i_TEXTO
};

struct i_filtro_t
{
    struct i_item_datos_t *datos;
    edit_FPtr_filtra func_filtra;
    edit_FPtr_destruye func_destruye;
};

struct edit_t
{
    char *titulo;
    enum i_tipo_dato_t tipo;
    
    double *numero;
    unsigned long num_decimales;
    double inicio, final;
    
    char **texto;
    
    ArrTipo(i_filtro_t) *filtros;
    
    struct item_implementacion_t *implementacion;
    const struct marcogrf_t *marco;
        
    bool es_multilinea, editable;
    unsigned long ancho, alto;
};

//---------------------------------------------------------

static struct i_filtro_t *i_crea_filtro(
                        struct i_item_datos_t **datos,
                        edit_FPtr_filtra func_filtra,
                        edit_FPtr_destruye func_destruye)
{
    struct i_filtro_t *filtro;
        
    filtro = MALLOC(struct i_filtro_t);
        
    filtro->datos = ASSIGN_PP(datos, struct i_item_datos_t);
    filtro->func_filtra = func_filtra;
    filtro->func_destruye = func_destruye;
        
    return filtro;
}

//---------------------------------------------------------

static void i_destruye_filtro(struct i_filtro_t **filtro)
{
    assert_no_null(filtro);
    assert_no_null(*filtro);
    
    if ((*filtro)->func_destruye != NULL)
        (*filtro)->func_destruye(&(*filtro)->datos);
    
    FREE_T(filtro, struct i_filtro_t);
}

//---------------------------------------------------------

static struct edit_t *i_crea(
                        const char *titulo,
                        enum i_tipo_dato_t tipo,
                        double *numero,
                        unsigned long num_decimales,
                        double inicio, double final,
                        char **texto,
                        ArrTipo(i_filtro_t) **filtros,
                        struct item_implementacion_t **implementacion,
                        const struct marcogrf_t *marco,
                        bool es_multilinea, bool editable,
                        unsigned long ancho, unsigned long alto)
{
	struct edit_t *edit;
		
	edit = MALLOC(struct edit_t);
		
    edit->titulo = strbas_copyString(titulo);
    edit->tipo = tipo;
    edit->numero = numero;
    edit->num_decimales = num_decimales;
    edit->inicio = inicio;
    edit->final = final;
    edit->texto = texto;
    edit->filtros = ASSIGN_PP(filtros, ArrTipo(i_filtro_t));
    edit->implementacion = ASSIGN_PP(implementacion, struct item_implementacion_t);
    edit->marco = marco;

    edit->es_multilinea = es_multilinea;
    edit->editable = editable;
    edit->ancho = ancho;
    edit->alto = alto;
	
	return edit;
}

//---------------------------------------------------------

static void i_destruye(struct edit_t **edit)
{
	assert_no_null(edit);
	assert_no_null(*edit);
	
    strbas_destroy(&(*edit)->titulo);
	array_Destruye(i_filtro_t, &(*edit)->filtros, i_destruye_filtro);
	
	FREE_T(edit, struct edit_t);
}

//---------------------------------------------------------

static double i_valor(const char *texto)
{
    double valor;
    int num_campos;
    
    num_campos = sscanf(texto, "%lf", &valor);
    assert(num_campos == 1);
    
    return valor;
}

//---------------------------------------------------------

static bool i_filtra_numero(const char *texto, double inicio, double final, unsigned long numero_decimales, char **mensaje_error)
{
    double valor;
    
    valor = i_valor(texto);
    
    if (valor < inicio || valor > final)
    {
        char *mensaje;
        
        mensaje = strbas_printf("El valor debe estar comprendido entre %.*f y %.*f", numero_decimales, inicio, numero_decimales, final);
        strbas_assignMessage(mensaje_error, mensaje);
        strbas_destroy(&mensaje);
        
        return false;
    }
    
    return true;
}

//---------------------------------------------------------

static bool i_ejecuta_filtro(const struct i_filtro_t *filtro, const char *texto, char **mensaje_error)
{
    assert_no_null(filtro);
    assert_no_null(filtro->func_filtra);
    
    return filtro->func_filtra(filtro->datos, texto, mensaje_error);
}

//---------------------------------------------------------

static bool i_filtra_texto(const char *texto, ArrTipo(i_filtro_t) *filtros, char **mensaje_error)
{
    unsigned long i, num_filtros;
    
    num_filtros = array_numElementos(i_filtro_t, filtros);
    
    for (i = 0; i < num_filtros; i++)
    {
        const struct i_filtro_t *filtro;
            
        filtro = array_Get(i_filtro_t, filtros, i);
        
        if (i_ejecuta_filtro(filtro, texto, mensaje_error) == false)
            return false;
    }
    
    return true;
}

//---------------------------------------------------------

static bool i_filtra(const char *texto, struct edit_t *edit, char **mensaje_error)
{
    bool aceptado;

    assert_no_null(edit);
    
    switch (edit->tipo)
    {
        case i_DOUBLE:

            aceptado = i_filtra_numero(texto, edit->inicio, edit->final, edit->num_decimales, mensaje_error);
            break;
        
        case i_TEXTO:

            aceptado = i_filtra_texto(texto, edit->filtros, mensaje_error);
            break;
        
        default_error();
    }
    
    return aceptado;
}

//---------------------------------------------------------

static struct item_implementacion_t *i_crea_implementacion(struct marcogrf_t *marco, struct edit_t *edit, struct item_implementacion_dialogo_t *dialogo_so)
{
    char *texto;
    const char *filtro_teclas;
    
    assert_no_null(edit);
    assert(edit->implementacion == NULL);
    assert(edit->marco == NULL);
    
    switch(edit->tipo)
    {
        case i_DOUBLE:
    
            assert_no_null(edit->numero);
            texto = strbas_printf("%.*f", edit->num_decimales, *(edit->numero));
            filtro_teclas = "0123456789.";
            break;
        
        case i_TEXTO:

            texto = strbas_copyString(*(edit->texto));
            filtro_teclas = NULL;
            break;
        
        default_error();
    }
    
    edit->implementacion = marcogrf_crea_edit(
                    marco, dialogo_so, texto, filtro_teclas, edit, 
                    edit->es_multilinea, edit->editable, edit->ancho, edit->alto, i_filtra);
    edit->marco = marco;
    
    strbas_destroy(&texto);
    
    return edit->implementacion;
}

//---------------------------------------------------------

static void i_aceptar(const struct marcogrf_t *marco, struct edit_t *edit)
{
    char *texto;
    
    texto = marcogrf_get_texto(marco, edit->implementacion);
    
    assert_no_null(edit);
    
    switch(edit->tipo)
    {
        case i_DOUBLE:

            *(edit->numero) = i_valor(texto);
            break;
        
        case i_TEXTO:
            
            strbas_destroy(edit->texto);
            *(edit->texto) = strbas_copyString(texto);
            break;
        
        default_error();
    }
    
    strbas_destroy(&texto);
}

//---------------------------------------------------------

static ArrString *i_get_titulo(const struct edit_t *edit)
{
    ArrString *textos;

    assert_no_null(edit);
    
    textos = arrcad_CreaString(1);
    arrcad_Set(textos, 0, edit->titulo);
    
    return textos;
}

//---------------------------------------------------------

static ArrString *i_get_valor(const struct edit_t *edit)
{
    char *texto;
    ArrString *textos;

    assert_no_null(edit);
    
    switch(edit->tipo)
    {
        case i_DOUBLE:
        {
            texto = strbas_printf("%.*f", edit->num_decimales, *(edit->numero));
            break;
        }
        case i_TEXTO:
            
            texto = strbas_copyString(*(edit->texto));
            break;
        
        default_error();
    }
    
    textos = arrcad_CreaString(1);
    arrcad_Set(textos, 0, texto);
    
    strbas_destroy(&texto);
    
    return textos;
}

//---------------------------------------------------------
	
struct panel_t *edit_crea_numero(const char *titulo, double *valor, double inicio, double final, unsigned long num_decimales)
{
	struct edit_t *edit;
	struct componente_t *componente;
    struct item_implementacion_t *implementacion;
    ArrTipo(i_filtro_t) *filtros;
    
    filtros = array_CreaPuntero(0, i_filtro_t);
	
    implementacion = NULL;
    edit = i_crea(titulo, i_DOUBLE, valor, num_decimales, inicio, final, NULL, &filtros, &implementacion, NULL, false, true, 0, 0);
	
	componente = componente_crea_defecto(edit_t, &edit, i_crea_implementacion, i_destruye, NULL, i_aceptar, i_get_titulo, i_get_valor);
	
	return panel_crea_componente(&componente);
}

//---------------------------------------------------------

struct panel_t *edit_crea_texto(const char *titulo, char **texto)
{
	struct edit_t *edit;
	struct componente_t *componente;
    struct item_implementacion_t *implementacion;
    ArrTipo(i_filtro_t) *filtros;
    
    filtros = array_CreaPuntero(0, i_filtro_t);
	
    implementacion = NULL;
    edit = i_crea(titulo, i_TEXTO, NULL, 0, 0., 0., texto, &filtros, &implementacion, NULL, false, true, 0, 0);
	
	componente = componente_crea_defecto(edit_t, &edit, i_crea_implementacion, i_destruye, NULL, i_aceptar, i_get_titulo, i_get_valor);
	
	return panel_crea_componente(&componente);
}

//---------------------------------------------------------

struct panel_t *edit_crea_texto_multilinea(
                            const char *titulo, char **texto, 
                            unsigned long ancho, unsigned long alto, bool editable)
{
	struct edit_t *edit;
	struct componente_t *componente;
    struct item_implementacion_t *implementacion;
    ArrTipo(i_filtro_t) *filtros;
    
    filtros = array_CreaPuntero(0, i_filtro_t);
	
    implementacion = NULL;
    edit = i_crea(titulo, i_TEXTO, NULL, 0, 0., 0., texto, &filtros, &implementacion, NULL, true, editable, ancho, alto);
	
	componente = componente_crea_defecto(edit_t, &edit, i_crea_implementacion, i_destruye, NULL, i_aceptar, i_get_titulo, i_get_valor);
	
	return panel_crea_componente(&componente);
}

//---------------------------------------------------------

void edit_nousar_anade_filtro(
                        struct edit_t *edit, 
                        struct i_item_datos_t **datos,
                        edit_FPtr_filtra func_filtra,
                        edit_FPtr_destruye func_destruye)
{
    struct i_filtro_t *filtro;
    
    assert_no_null(edit);

    filtro = i_crea_filtro(datos, func_filtra, func_destruye);
    array_Anyadir(i_filtro_t, edit->filtros, filtro);
}

//---------------------------------------------------------

const char *edit_get_texto(const struct edit_t *edit)
{
    assert_no_null(edit);
    return marcogrf_get_texto(edit->marco, edit->implementacion);
}
