// Control lista.c

#include "lista.h"

#include "panel.inl"
#include "componente.inl"

#include "asrtbas.h"
#include "array.h"
#include "arrcad.h"
#include "memory.h"
#include "strbas.h"
#include "marcogrf.syh"
#include "limits.h"

struct lista_t
{
    bool forzar_seleccion;
    unsigned long *ind_elemento_seleccionado;
    
    unsigned long *id_nuevo;
    char *tipo;
    
    unsigned long ancho, alto;
    
    bool trata_con_datos_originales;
    struct Array_t **datos_editando;
    struct Array_t *copia_de_datos;
    
    lista_FPtr_crea_defecto func_crea_defecto;
    lista_FPtr_panel func_panel;
    lista_FPtr_copia func_copia;
    lista_FPtr_destruye func_destruye;

    bool con_datos_extra;
    const struct lista_item_datos_extra_t *datos_extra;
    lista_FPtr_panel_con_datos_extra func_panel_con_datos_extra;
    
    struct marcogrf_t *marco;
    struct item_implementacion_t *implementacion;
};

//---------------------------------------------------------

static void i_integridad(const struct lista_t *lista)
{
    assert_no_null(lista->datos_editando);
    assert_no_null(lista->copia_de_datos);
    
    if (lista->con_datos_extra == true)
        assert_no_null(lista->func_panel_con_datos_extra);
    else
        assert_no_null(lista->func_panel);
    
    if (lista->forzar_seleccion == false)
    {
        assert_no_null(lista->func_crea_defecto);
        assert_no_null(lista->func_copia);
        assert_no_null(lista->func_destruye);
    }
}

//---------------------------------------------------------

struct lista_t *i_crea(
                    bool forzar_seleccion,
                    unsigned long *ind_elemento_seleccionado,
                    unsigned long *id_nuevo,
                    const char *tipo,
                    unsigned long ancho, unsigned long alto,
                    bool trata_con_datos_originales,
                    struct Array_t **datos_editando,
                    struct Array_t **copia_de_datos,
                    lista_FPtr_crea_defecto func_crea_defecto,
                    lista_FPtr_panel func_panel,
                    lista_FPtr_copia func_copia,
                    lista_FPtr_destruye func_destruye,
                    bool con_datos_extra,
                    const struct lista_item_datos_extra_t *datos_extra,
                    lista_FPtr_panel_con_datos_extra func_panel_con_datos_extra,
                    struct marcogrf_t *marco,
                    struct item_implementacion_t **implementacion)
{
    struct lista_t *lista;
        
    lista = MALLOC(struct lista_t);
        
    lista->forzar_seleccion = forzar_seleccion;
    lista->ind_elemento_seleccionado = ind_elemento_seleccionado;
    
    lista->id_nuevo = id_nuevo;
    lista->tipo = strbas_copyString(tipo);
    lista->datos_editando = datos_editando;
    lista->copia_de_datos = ASSIGN_PP(copia_de_datos, struct Array_t);

    lista->trata_con_datos_originales = trata_con_datos_originales;
    lista->ancho = ancho;
    lista->alto = alto;
    
    lista->func_crea_defecto = func_crea_defecto;
    lista->func_panel = func_panel;
    lista->func_copia = func_copia;
    lista->func_destruye = func_destruye;
    
    lista->con_datos_extra = con_datos_extra;
    lista->datos_extra = datos_extra;
    lista->func_panel_con_datos_extra = func_panel_con_datos_extra;
    
    lista->marco = marco;
    lista->implementacion = ASSIGN_PP(implementacion, struct item_implementacion_t);

    i_integridad(lista);
    
    return lista;
}
    
//---------------------------------------------------------

static void i_destruye(struct lista_t **lista)
{
    assert_no_null(lista);
    assert_no_null(*lista);

    if ((*lista)->trata_con_datos_originales == false)
        array_DestruyePrivada((*lista)->tipo, &(*lista)->copia_de_datos, (FuncLiberaPtr)(*lista)->func_destruye);
    
    strbas_destroy(&(*lista)->tipo);
    
    FREE_T(lista, struct lista_t);
}

//---------------------------------------------------------

static void i_aceptar(const struct marcogrf_t *marco, struct lista_t *lista)
{
    assert_no_null(lista);
    
    if (lista->trata_con_datos_originales == false)
    {
        array_DestruyePrivada(lista->tipo, lista->datos_editando, (FuncLiberaPtr)lista->func_destruye);
        *(lista->datos_editando) = array_CopiaPrivada(lista->tipo, lista->copia_de_datos, (FuncCopiaPtr)lista->func_copia);
    }
    
    if (lista->ind_elemento_seleccionado != NULL)
    {
        unsigned long ind_elemento_activo;
        
        if (marcogrf_get_fila_activa((struct marcogrf_t *)marco, lista->implementacion, &ind_elemento_activo) == true)
            *(lista->ind_elemento_seleccionado) = ind_elemento_activo;
    }
}

//---------------------------------------------------------

static bool i_se_puede_aceptar(const struct marcogrf_t *marco, struct lista_t *lista, char **mensaje_error)
{
    bool hay_seleccion;
    
    assert_no_null(lista);
    
    hay_seleccion = marcogrf_get_fila_activa((struct marcogrf_t *)marco, lista->implementacion, NULL);

    if (lista->forzar_seleccion == true && hay_seleccion == false)
    {
        strbas_assignMessage(mensaje_error, "Debemos seleccionar un elemento de la lista.");
        return false;
    }
    else
        return true;
}


//---------------------------------------------------------

static ArrString *i_crea_datos_titulos(
                            struct lista_item_dato_t *dato, 
                            lista_FPtr_panel func_panel,
                            bool con_datos_extra,
                            const struct lista_item_datos_extra_t *datos_extra,
                            lista_FPtr_panel_con_datos_extra func_panel_con_datos_extra,
                            ArrString *func_textos_panel(struct panel_t *))
{
    ArrString *textos;
    struct panel_t *panel;
        
    assert_no_null(func_textos_panel);
    
    if (con_datos_extra == true)
    {
        assert_no_null(func_panel_con_datos_extra);
        panel = func_panel_con_datos_extra(datos_extra, dato);
    }
    else
    {
        assert_no_null(func_panel);
        panel = func_panel(dato);
    }
    
    textos = func_textos_panel(panel);
    
    panel_destruye(&panel);
    
    return textos;
}

//---------------------------------------------------------

static ArrString *i_crea_titulos(
                            const char *tipo,
                            struct Array_t *elementos,
                            lista_FPtr_crea_defecto func_crea_defecto, 
                            lista_FPtr_destruye func_destruye,
                            lista_FPtr_panel func_panel, 
                            bool con_datos_extra,
                            const struct lista_item_datos_extra_t *datos_extra,
                            lista_FPtr_panel_con_datos_extra func_panel_con_datos_extra)
{
    ArrString *titulos;
    bool destruye_dato;
    struct lista_item_dato_t *dato;
        
    if (array_numElementosPrivada(tipo, elementos) == 0)
    {
        unsigned long id_nuevo;
        
        destruye_dato = true;
        assert_no_null(func_crea_defecto);
        id_nuevo = 0;
        dato = func_crea_defecto(&id_nuevo);
    }
    else
    {
        destruye_dato = false;
        dato = (struct lista_item_dato_t *)array_ObtenerPrivada(tipo, elementos, 0);
    }

    titulos = i_crea_datos_titulos(
                            dato, func_panel, 
                            con_datos_extra, datos_extra, 
                            func_panel_con_datos_extra,
                            panel_titulos_componentes);
    
    if (destruye_dato == true)
    {
        assert_no_null(func_destruye);
        func_destruye(&dato);
    }
    
    return titulos;
}

//---------------------------------------------------------

static ArrString *i_texto_dato(
                            const char *tipo, 
                            struct Array_t *elementos, 
                            unsigned long fila, 
                            lista_FPtr_panel func_panel,
                            bool con_datos_extra,
                            const struct lista_item_datos_extra_t *datos_extra,
                            lista_FPtr_panel_con_datos_extra func_panel_con_datos_extra)
{
    struct lista_item_dato_t *dato;
        
    dato = (struct lista_item_dato_t *)array_ObtenerPrivada(tipo, elementos, fila);
        
    return i_crea_datos_titulos(
                            dato, func_panel, 
                            con_datos_extra, datos_extra, func_panel_con_datos_extra,
                            panel_datos_componentes);
}

//---------------------------------------------------------

static ArrPointer(ArrString) *i_crea_texto_datos(
                            const char *tipo, 
                            struct Array_t *elementos, 
                            lista_FPtr_panel func_panel,
                            bool con_datos_extra,
                            const struct lista_item_datos_extra_t *datos_extra,
                            lista_FPtr_panel_con_datos_extra func_panel_con_datos_extra)
{
    ArrPointer(ArrString) *texto_datos;
    unsigned long i, num_datos;
    
    num_datos = array_numElementosPrivada(tipo, elementos);
    
    texto_datos = array_CreaArrayPuntero(num_datos, ArrString);
    
    for (i = 0; i < num_datos; i++)
    {
        ArrString *texto_dato;
        
        texto_dato = i_texto_dato(tipo, elementos, i, func_panel, con_datos_extra, datos_extra, func_panel_con_datos_extra);
        array_SetPointer(ArrString, texto_datos, i, texto_dato);
    }
    
    return texto_datos;
}

//---------------------------------------------------------

static struct item_implementacion_t *i_crea_implementacion(struct marcogrf_t *marco, struct lista_t *lista, struct item_implementacion_dialogo_t *dialogo_so)
{
    ArrString *titulos;
    ArrPointer(ArrString) *texto_datos;
    bool hay_seleccion;
    unsigned long indice_seleccion;
    
    assert_no_null(lista);
    assert(lista->marco == NULL);
    assert(lista->implementacion == NULL);
    
    titulos = i_crea_titulos(
                            lista->tipo,
                            lista->copia_de_datos,
                            lista->func_crea_defecto, 
                            lista->func_destruye,
                            lista->func_panel, 
                            lista->con_datos_extra,
                            lista->datos_extra,
                            lista->func_panel_con_datos_extra);
    
    texto_datos = i_crea_texto_datos(
                            lista->tipo, lista->copia_de_datos, lista->func_panel,
                            lista->con_datos_extra,
                            lista->datos_extra,
                            lista->func_panel_con_datos_extra);

    if (lista->ind_elemento_seleccionado != NULL)
    {
        indice_seleccion = *(lista->ind_elemento_seleccionado);
        hay_seleccion = true;
    }
    else
    {
        indice_seleccion = USHRT_MAX;
        hay_seleccion = false;
    }
    
    lista->implementacion = marcogrf_crea_lista(marco, dialogo_so, titulos, texto_datos, hay_seleccion, indice_seleccion);
    
    array_DestruyePointer(ArrString, &texto_datos, arrcad_Destruye);
    arrcad_Destruye(&titulos);
    
    lista->marco = marco;
    return lista->implementacion;
}

//---------------------------------------------------------

static ArrString *i_titulos(const struct lista_t *lista)
{
    assert_no_null(lista);
    return i_crea_titulos(
                        lista->tipo,
                        lista->copia_de_datos,
                        lista->func_crea_defecto, 
                        lista->func_destruye,
                        lista->func_panel, 
                        lista->con_datos_extra,
                        lista->datos_extra,
                        lista->func_panel_con_datos_extra);
}

//---------------------------------------------------------

static bool i_hay_elemento_seleccionado(
                    unsigned long *ind_elemento_seleccionado_por_defecto,
                    struct marcogrf_t *marco,
                    struct item_implementacion_t *implementacion,
                    unsigned long *ind_elemento)
{
    bool hay_seleccion;
    
    assert_no_null(ind_elemento);
    
    if (marco == NULL)
    {
        if (ind_elemento_seleccionado_por_defecto != NULL)
        {
            hay_seleccion = true;
            *ind_elemento = *ind_elemento_seleccionado_por_defecto;
        }
        else
            hay_seleccion = false;
    }
    else
        hay_seleccion = marcogrf_get_fila_activa(marco, implementacion, ind_elemento);
    
    return hay_seleccion;
}

//---------------------------------------------------------

static ArrString *i_datos(const struct lista_t *lista)
{
    bool hay_elementos;
    unsigned long ind_elemento;
    
    hay_elementos = i_hay_elemento_seleccionado(
                            lista->ind_elemento_seleccionado,
                            lista->marco,
                            lista->implementacion,
                            &ind_elemento);
    assert(hay_elementos == true);
    
    return i_texto_dato(
                    lista->tipo, 
                    lista->copia_de_datos,
                    ind_elemento, 
                    lista->func_panel,
                    lista->con_datos_extra,
                    lista->datos_extra,
                    lista->func_panel_con_datos_extra);
}

//---------------------------------------------------------
	
struct panel_t *lista_nousar_crea(
                    bool forzar_seleccion,
                    unsigned long *ind_elemento_seleccionado,
                    const char *tipo,
                    unsigned long *id_nuevo,
                    unsigned long ancho, unsigned long alto,
                    bool trata_con_datos_originales,
                    struct Array_t **datos,
                    lista_FPtr_crea_defecto func_crea_defecto,
                    lista_FPtr_panel func_panel,
                    lista_FPtr_copia func_copia,
                    lista_FPtr_destruye func_destruye,
                    bool con_datos_extra,
                    const struct lista_item_datos_extra_t *datos_extra,
                    lista_FPtr_panel_con_datos_extra func_panel_con_datos_extra,
                    struct lista_t **plista)
{
    struct componente_t *componente;
    struct lista_t *lista;
    struct Array_t *copia_datos;
    struct item_implementacion_t *implementacion;
    struct marcogrf_t *marco;

	assert_no_null(datos);
    assert_no_null(*datos);
	
    implementacion = NULL;
    marco = NULL;
    
    if (trata_con_datos_originales == true)
        copia_datos = *datos;
    else
        copia_datos = array_CopiaPrivada(tipo, *datos, (FuncCopiaPtr)func_copia);
    
    lista = i_crea(
                forzar_seleccion, ind_elemento_seleccionado,
                id_nuevo, tipo, ancho, alto, trata_con_datos_originales, datos, 
                &copia_datos, 
                func_crea_defecto, func_panel, func_copia, func_destruye, 
                con_datos_extra, datos_extra, func_panel_con_datos_extra,
                marco, &implementacion);
	
    if (plista != NULL)
        *plista = lista;
	
	componente = componente_crea_defecto(lista_t, &lista, i_crea_implementacion, i_destruye, i_se_puede_aceptar, i_aceptar, i_titulos, i_datos);
    
	return panel_crea_componente(&componente);
}

//---------------------------------------------------------

struct lista_item_dato_t *lista_nousar_nuevo_elemento(struct lista_t *lista, const char *tipo)
{
    assert_no_null(lista);
    assert_no_null(lista->func_crea_defecto);
    assert(strbas_equalsStrings(lista->tipo, tipo) == true);
    
    return lista->func_crea_defecto(lista->id_nuevo);
}

//---------------------------------------------------------

void lista_nousar_append_elemento(struct lista_t *lista, const char *tipo, struct lista_item_dato_t **dato)
{
    ArrString *datos_fila;
    unsigned long num_elementos, ind_fila;
    
    assert_no_null(lista);
    assert(strbas_equalsStrings(lista->tipo, tipo) == true);
    assert_no_null(dato);
    
    array_AnyadirPrivada(lista->tipo, lista->copia_de_datos, *dato);
    *dato = NULL;
    
    num_elementos = array_numElementosPrivada(lista->tipo, lista->copia_de_datos);
    assert(num_elementos > 0);
    ind_fila = num_elementos - 1;
    
    datos_fila = i_texto_dato(
                            lista->tipo, 
                            lista->copia_de_datos,
                            ind_fila, 
                            lista->func_panel,
                            lista->con_datos_extra,
                            lista->datos_extra,
                            lista->func_panel_con_datos_extra);
    
    marcogrf_append_lista(lista->marco, lista->implementacion, datos_fila);
    marcogrf_set_fila_activa(lista->marco, lista->implementacion, ind_fila);
    
    arrcad_Destruye(&datos_fila);
}

//---------------------------------------------------------

void lista_nousar_borra_elemento_seleccionado(struct lista_t *lista, const char *tipo)
{
    unsigned long ind_elemento_activo;
    
    assert_no_null(lista);
    assert(strbas_equalsStrings(lista->tipo, tipo) == true);
    
    if (marcogrf_get_fila_activa(lista->marco, lista->implementacion, &ind_elemento_activo) == true)
    {
        marcogrf_remove_lista(lista->marco, lista->implementacion, ind_elemento_activo);
        array_BorrarPrivada(lista->tipo, lista->copia_de_datos, ind_elemento_activo, (FuncLiberaPtr)lista->func_destruye);
    }
}

//---------------------------------------------------------

bool lista_nousar_hay_elementos_seleccionados(struct lista_t *lista, const char *tipo)
{
    assert_no_null(lista);
    assert(strbas_equalsStrings(lista->tipo, tipo) == true);
    return marcogrf_get_fila_activa(lista->marco, lista->implementacion, NULL);
}

//---------------------------------------------------------

struct panel_t *lista_nousar_panel_elemento_activo(struct lista_t *lista, const char *tipo)
{
    unsigned long ind_elemento_activo;
    bool hay_seleccionados;
    struct lista_item_dato_t *dato_seleccionado;
    
    assert_no_null(lista);
    assert(strbas_equalsStrings(lista->tipo, tipo) == true);
    assert_no_null(lista->func_panel);
    
    hay_seleccionados = marcogrf_get_fila_activa(lista->marco, lista->implementacion, &ind_elemento_activo);
    assert(hay_seleccionados == true);

    dato_seleccionado = (struct lista_item_dato_t *)array_ObtenerPrivada(lista->tipo, lista->copia_de_datos, ind_elemento_activo);
    
    return lista->func_panel(dato_seleccionado);
}

//---------------------------------------------------------

struct lista_item_dato_t *lista_nousar_elemento_activo(struct lista_t *lista, const char *tipo)
{
    unsigned long ind_elemento_activo;
    bool hay_seleccionados;
    
    assert_no_null(lista);
    assert(strbas_equalsStrings(lista->tipo, tipo) == true);
    
    hay_seleccionados = marcogrf_get_fila_activa(lista->marco, lista->implementacion, &ind_elemento_activo);
    assert(hay_seleccionados == true);

    return (struct lista_item_dato_t *)array_ObtenerPrivada(lista->tipo, lista->copia_de_datos, ind_elemento_activo);
}

//---------------------------------------------------------

void lista_nousar_actualiza_activo(struct lista_t *lista, const char *tipo)
{
    bool hay_seleccion;
    unsigned long ind_elemento_activo;
    ArrString *datos_fila;
    
    assert_no_null(lista);
    assert(strbas_equalsStrings(lista->tipo, tipo) == true);
    
    hay_seleccion = marcogrf_get_fila_activa(lista->marco, lista->implementacion, &ind_elemento_activo);
    assert(hay_seleccion == true);
 
    datos_fila = i_texto_dato(
                            lista->tipo, 
                            lista->copia_de_datos,
                            ind_elemento_activo, 
                            lista->func_panel,
                            lista->con_datos_extra,
                            lista->datos_extra,
                            lista->func_panel_con_datos_extra);
    
    marcogrf_cambia_fila(lista->marco, lista->implementacion, ind_elemento_activo, datos_fila);
    
    arrcad_Destruye(&datos_fila);
}
