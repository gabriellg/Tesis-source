// Definici�n de paneles;

#include "panel.h"
#include "panel.inl"

#include "componente.inl"
#include "rcpaccion.inl"
#include "timeproc.inl"
#include "rcpkey.inl"
#include "evtkey.inl"

#include "asrtbas.h"
#include "memory.h"
#include "array.h"
#include "arrcad.h"
#include "marcogrf.syh"

#include <limits.h>

enum i_tipo_elemento_t
{
	i_PANEL, i_COMPONENTE
};

struct i_datos_panel_t
{
    struct item_datos_panel_t *datos_panel;
    panel_FPtr_destruye_datos_panel func_destruye_datos_panel;
};

struct panel_t
{
	enum i_tipo_elemento_t tipo_elemento;
	
	struct componente_t *componente;
	
    unsigned long num_filas, num_columnas;
	enum mrpanel_tipo_disposicion_t tipo_disposicion;
	bool expand, fill;
    
    ArrTipo(i_datos_panel_t) *datos_panel;
	ArrTipo(panel_t) *hijos;
	
    ArrTipo(timeproc_t) *times_actions;
    ArrTipo(RcpKey_t) *receptores_teclas;

    struct marcogrf_t *marco;
    struct item_implementacion_t *implementacion_panel;
    struct item_implementacion_dialogo_t *dialogo_so;
    
    struct item_datos_panel_t *datos_aceptar;
    panel_FPtr_aceptado func_aceptar;
};

//---------------------------------------------------------

static void i_integridad(const struct panel_t *panel)
{
	assert_no_null(panel);
	
	switch(panel->tipo_elemento)
	{
		case i_PANEL:
		
			assert_no_null(panel->hijos);
            assert(panel->num_filas > 0);
            assert(panel->num_columnas > 0);
			break;
		
		case i_COMPONENTE:
		
			assert_no_null(panel->componente);
			break;
		
		default_error();
	}
}

//---------------------------------------------------------

static struct i_datos_panel_t *i_crea_datos_panel(
                        struct item_datos_panel_t **datos_panel,
                        panel_FPtr_destruye_datos_panel func_destruye_datos_panel)
{
    struct i_datos_panel_t *datos_destruir;
        
    datos_destruir = MALLOC(struct i_datos_panel_t);
        
    datos_destruir->datos_panel = ASSIGN_PP(datos_panel, struct item_datos_panel_t);
    datos_destruir->func_destruye_datos_panel = func_destruye_datos_panel;
        
    return datos_destruir;
}

//---------------------------------------------------------

static void i_destruir_datos_panel(struct i_datos_panel_t **datos_destruir)
{
    assert_no_null(datos_destruir);
    assert_no_null(*datos_destruir);
    assert_no_null((*datos_destruir)->func_destruye_datos_panel);
    
    (*datos_destruir)->func_destruye_datos_panel(&(*datos_destruir)->datos_panel);
    
    FREE_T(datos_destruir, struct i_datos_panel_t);
}

//---------------------------------------------------------

static struct panel_t *i_crea(
					enum i_tipo_elemento_t tipo_elemento,
					struct componente_t **componente,
                    unsigned long num_filas, unsigned long num_columnas,
					enum mrpanel_tipo_disposicion_t tipo_disposicion,
					bool expand, bool fill,
                    ArrTipo(i_datos_panel_t) **datos_panel,
					ArrTipo(panel_t) **hijos,
                    ArrTipo(timeproc_t) **times_actions,
                    ArrTipo(RcpKey_t) **receptores_teclas,
                    struct marcogrf_t *marco,
                    struct item_implementacion_t *implementacion_panel,
                    struct item_implementacion_dialogo_t *dialogo_so,
                    struct item_datos_panel_t *datos_aceptar,
                    panel_FPtr_aceptado func_aceptar)
{
	struct panel_t *panel;
		
	panel = MALLOC(struct panel_t);

	panel->tipo_elemento = tipo_elemento;
	panel->componente = ASSIGN_PP(componente, struct componente_t);
	panel->tipo_disposicion = tipo_disposicion;
	panel->expand = expand; 
	panel->fill = fill;
    panel->num_filas = num_filas;
    panel->num_columnas = num_columnas;
    panel->datos_panel = ASSIGN_PP(datos_panel, ArrTipo(i_datos_panel_t));
	panel->hijos = ASSIGN_PP(hijos, ArrTipo(panel_t));
    panel->times_actions = ASSIGN_PP_NO_NULL(times_actions, ArrTipo(timeproc_t));
    panel->receptores_teclas = ASSIGN_PP_NO_NULL(receptores_teclas, ArrTipo(RcpKey_t));
    
    panel->marco = marco;
    panel->implementacion_panel = implementacion_panel;
    panel->dialogo_so = dialogo_so;
    panel->datos_aceptar = datos_aceptar;
    panel->func_aceptar = func_aceptar;

	return panel;
}

//---------------------------------------------------------

void panel_destruye(struct panel_t **panel)
{
	assert_no_null(panel);
	i_integridad(*panel);
	
	switch((*panel)->tipo_elemento)
	{
		case i_PANEL:

			array_Destruye(panel_t, &((*panel)->hijos), panel_destruye);
			break;

		case i_COMPONENTE:
		
			componente_destruye(&(*panel)->componente);
			break;
		
		default_error();
	}
	
    array_Destruye(timeproc_t, &(*panel)->times_actions, timeproc_destroy);
    array_Destruye(RcpKey_t, &(*panel)->receptores_teclas, rcpkey_destruye);
    array_Destruye(i_datos_panel_t, &(*panel)->datos_panel, i_destruir_datos_panel);
    
	FREE_T(panel, struct panel_t);
}
	
//---------------------------------------------------------

static struct panel_t *i_crea_panel_vacio(
                            enum mrpanel_tipo_disposicion_t tipo_disposicion,
                            unsigned long num_hijos,
                            unsigned long num_filas, unsigned long num_columnas)
{
	struct componente_t *componente;
	enum i_tipo_elemento_t tipo_elemento;
	ArrTipo(panel_t) *hijos;
    ArrTipo(i_datos_panel_t) *datos_panel;
    struct marcogrf_t *marco;
    struct item_implementacion_t *implementacion_panel;
    struct item_implementacion_dialogo_t *dialogo_so;
    struct item_datos_panel_t *datos_aceptar;
    panel_FPtr_aceptado func_aceptar;
    ArrTipo(timeproc_t) *times_actions;
    ArrTipo(RcpKey_t) *receptores_teclas;
	bool expand, fill;

	tipo_elemento = i_PANEL;
	hijos = array_CreaPuntero(num_hijos, panel_t);
    datos_panel = array_CreaPuntero(0, i_datos_panel_t);
	componente = NULL;
    marco = NULL;
    implementacion_panel = NULL;
    dialogo_so = NULL;
    datos_aceptar = NULL;
    func_aceptar = NULL;
    times_actions = array_CreaPuntero(0, timeproc_t);
    receptores_teclas = array_CreaPuntero(0, RcpKey_t);
	expand = false;
	fill = false;
	
	return i_crea(
                tipo_elemento, &componente, 
                num_filas, num_columnas, tipo_disposicion,
                expand, fill,
                &datos_panel, &hijos, &times_actions, &receptores_teclas,
                marco, implementacion_panel, dialogo_so, datos_aceptar, func_aceptar);
}

//---------------------------------------------------------

struct panel_t *panel_crea_con_disposicion_horizontal(void)
{
	return i_crea_panel_vacio(MRPANEL_HORIZONTAL, 0, USHRT_MAX, USHRT_MAX);
}

//---------------------------------------------------------

struct panel_t *panel_crea_con_disposicion_vertical(void)
{
	return i_crea_panel_vacio(MRPANEL_VERTICAL, 0, USHRT_MAX, USHRT_MAX);
}

//---------------------------------------------------------

struct panel_t *panel_crea_con_disposicion_grid(unsigned long num_filas, unsigned long num_columnas)
{
    unsigned long num_componentes;
    
    num_componentes = num_filas * num_columnas;
    
    return i_crea_panel_vacio(MRPANEL_GRID, num_componentes, num_filas, num_columnas);
}

//---------------------------------------------------------

struct panel_t *panel_crea_componente(struct componente_t **componente)
{
	enum mrpanel_tipo_disposicion_t tipo_disposicion;
	enum i_tipo_elemento_t tipo_elemento;
	ArrTipo(panel_t) *hijos;
    ArrTipo(i_datos_panel_t) *datos_panel;
    struct marcogrf_t *marco;
    struct item_implementacion_t *implementacion_panel;
    struct item_implementacion_dialogo_t *dialogo_so;
    struct item_datos_panel_t *datos_aceptar;
    panel_FPtr_aceptado func_aceptar;
    ArrTipo(timeproc_t) *times_actions;
    ArrTipo(RcpKey_t) *receptores_teclas;
    bool fill, expand;
	
	hijos = NULL;
    datos_panel = array_CreaPuntero(0, i_datos_panel_t);
	tipo_elemento = i_COMPONENTE;
	tipo_disposicion = MRPANEL_VERTICAL;
    marco = NULL;
    implementacion_panel = NULL;
    dialogo_so = NULL;
    datos_aceptar = NULL;
    func_aceptar = NULL;
    fill = false;
    expand = false;

    times_actions = array_CreaPuntero(0, timeproc_t);
    receptores_teclas = array_CreaPuntero(0, RcpKey_t);
    
	return i_crea(
                tipo_elemento, componente, 
                USHRT_MAX, USHRT_MAX, tipo_disposicion, expand, fill,  
                &datos_panel, &hijos, &times_actions, &receptores_teclas, marco,
                implementacion_panel, dialogo_so, 
                datos_aceptar, func_aceptar);
}

//---------------------------------------------------------

void panel_append_panel(struct panel_t *padre, struct panel_t **hijo)
{
	i_integridad(padre);
	assert_no_null(hijo);
	assert_no_null(*hijo);

	switch(padre->tipo_elemento)
	{
		case i_PANEL:
		{
            switch(padre->tipo_disposicion)
            {
                case MRPANEL_HORIZONTAL:
                case MRPANEL_VERTICAL:
                    array_Anyadir(panel_t, padre->hijos, *hijo);
                    *hijo = NULL;
                    break;
                case MRPANEL_GRID:
                default_error();
            }
			break;
		}
		case i_COMPONENTE:
		default_error();
	}
}

//---------------------------------------------------------

static void i_append_implementacion(
                            struct marcogrf_t *marco,
                            struct item_implementacion_t *panel,
                            struct item_implementacion_t *panel_hijo,
                            enum i_tipo_elemento_t tipo_elemento_hijo,
                            enum mrpanel_tipo_disposicion_t tipo_disposicion,
                            unsigned long ind_fila, unsigned long ind_columna,
                            bool expand, bool fill)
{
	switch(tipo_elemento_hijo)
	{
		case i_PANEL:
		{
            marcogrf_append_panel(
                            marco, panel, tipo_disposicion, 
                            ind_fila, ind_columna, expand, fill, panel_hijo);
			break;
		}
		case i_COMPONENTE:
        {
            marcogrf_append_componente(
                            marco, panel, tipo_disposicion, 
                            ind_fila, ind_columna, expand, fill, panel_hijo);
			break;
		}	
		default_error();
	}
}

//---------------------------------------------------------

struct item_implementacion_t *i_crea_panel(
							struct marcogrf_t *marco,
                            unsigned long num_filas,
                            unsigned long num_columnas,
                            unsigned long num_hijos,
							enum mrpanel_tipo_disposicion_t tipo_disposicion)
{
    struct item_implementacion_t *implementacion;
        
	switch(tipo_disposicion)
	{
		case MRPANEL_HORIZONTAL:
			
            implementacion = marcogrf_crea_panel(marco, tipo_disposicion, 1, num_hijos, false, 0);
			break;
		
		case MRPANEL_VERTICAL:
			
            implementacion = marcogrf_crea_panel(marco, tipo_disposicion, num_hijos, 1, false, 0);
			break;
		
        case MRPANEL_GRID:
            
            implementacion = marcogrf_crea_panel(marco, tipo_disposicion, num_filas, num_hijos, false, 0);
            break;
        
		default_error();
	}
    
    return implementacion;
}

//---------------------------------------------------------

static void i_implementacion_timesproc(
                    ArrTipo(timeproc_t) *times_actions, 
                    struct componente_t *componente, 
                    const struct marcogrf_t *marco)
{
    unsigned long i, num;
    
    num = array_numElementos(timeproc_t, times_actions);
    
    for (i = 0; i < num; i++)
    {
        struct timeproc_t *time_action;
        
        time_action = array_Get(timeproc_t, times_actions, i);
        timeproc_set_time(time_action, componente, marco);
    }
}

//---------------------------------------------------------

static struct item_implementacion_t *i_implementacion_recursivamente(
							struct marcogrf_t *marco,
							enum i_tipo_elemento_t tipo_elemento,
                            unsigned long num_filas,
                            unsigned long num_columnas,
							struct componente_t *componente,
							enum mrpanel_tipo_disposicion_t tipo_disposicion,
							ArrTipo(panel_t) *hijos,
                            struct item_implementacion_dialogo_t *dialogo_so,
							unsigned long num_recursividad)
{
    struct item_implementacion_t *implementacion;
    
	assert(num_recursividad < 1000);
	num_recursividad++;

	switch(tipo_elemento)
	{
		case i_PANEL:
		{
			unsigned long num_hijos, i;
	
			num_hijos = array_numElementos(panel_t, hijos);
			assert(num_hijos > 0);
            
            implementacion = i_crea_panel(marco, num_filas, num_columnas, num_hijos, tipo_disposicion);
            
			for (i = 0; i < num_hijos; i++)
			{
				struct panel_t *panel_hijo;
                unsigned long ind_fila, ind_columna;
				
				panel_hijo = array_Get(panel_t, hijos, i);
				assert_no_null(panel_hijo);

                panel_hijo->marco = marco;
                panel_hijo->dialogo_so = dialogo_so;
                panel_hijo->implementacion_panel = i_implementacion_recursivamente(
							marco,
							panel_hijo->tipo_elemento,
                            panel_hijo->num_filas,
                            panel_hijo->num_columnas,
							panel_hijo->componente,
							panel_hijo->tipo_disposicion,
							panel_hijo->hijos,
                            dialogo_so,
							num_recursividad);
                
                ind_fila = i / num_columnas;
                ind_columna = i % num_columnas;
                
                i_append_implementacion(
                            marco,
                            implementacion, 
                            panel_hijo->implementacion_panel,
                            panel_hijo->tipo_elemento,
                            tipo_disposicion,
                            ind_fila, ind_columna,
                            panel_hijo->expand, panel_hijo->fill);
                
                i_implementacion_timesproc(panel_hijo->times_actions, panel_hijo->componente, marco);
			}
			break;
		}
		case i_COMPONENTE:
			
			implementacion = componente_generar_implementacion(marco, componente, dialogo_so);
            marcogrf_mostrar(marco, implementacion);
			break;
			
		default_error();
	}
	
	return implementacion;
}

//---------------------------------------------------------

static struct dialogo_t *i_dialogo_padre_primer_componente(
                            unsigned long recursividad,
                            enum i_tipo_elemento_t tipo_elemento,
                            ArrTipo(panel_t) *paneles,
                            struct componente_t *componente)
{
    struct dialogo_t *dialogo;

    assert(recursividad < 1000);
    recursividad++;
    
    switch(tipo_elemento)
    {
        case i_PANEL:
        {
            struct panel_t *panel;
            
            panel = array_Get(panel_t, paneles, 0);
            dialogo = i_dialogo_padre_primer_componente(recursividad, panel->tipo_elemento, panel->hijos, panel->componente);
            break;
        }
        case i_COMPONENTE:
            
            dialogo = componente_get_dialogo_padre(componente);
            break;
        
        default_error();
    }
    
    return dialogo;
}

//---------------------------------------------------------

static void i_set_dialogo_padre(
                        unsigned long num_recursividad,
                        enum i_tipo_elemento_t tipo_elemento,
                        struct componente_t *componente,
                        ArrTipo(panel_t) *hijos,
                        struct dialogo_t *dialogo)
{
	assert(num_recursividad < 1000);
	num_recursividad++;
	
	switch(tipo_elemento)
	{
		case i_PANEL:
		{
			unsigned long num_hijos, i;
			
			num_hijos = array_numElementos(panel_t, hijos);
            
			for (i = 0; i < num_hijos; i++)
			{
				struct panel_t *panel_hijo;
				
				panel_hijo = array_Get(panel_t, hijos, i);
				assert_no_null(panel_hijo);
                
                i_set_dialogo_padre(num_recursividad, panel_hijo->tipo_elemento, panel_hijo->componente, panel_hijo->hijos, dialogo);
			}
            break;
		}
		case i_COMPONENTE:
        {
            componente_set_dialogo_padre(componente, dialogo);
            break;
        }
		default_error();
	}
}

//---------------------------------------------------------

void panel_set_en_grid(struct panel_t *padre, unsigned long fila, unsigned long columna, struct panel_t **hijo)
{
    unsigned long posicion;
    
	i_integridad(padre);
	assert_no_null(hijo);
	assert_no_null(*hijo);
    
    posicion = fila * padre->num_columnas + columna;

    array_Set(panel_t, padre->hijos, posicion, *hijo);
    *hijo = NULL;
}

//---------------------------------------------------------

struct item_implementacion_t *panel_implementacion(
                    struct panel_t *panel, 
                    struct marcogrf_t *marco,
                    struct item_implementacion_dialogo_t *dialogo_so)
{
	unsigned long num_recursividad;

	i_integridad(panel);
    
	num_recursividad = 0;
    
    panel->marco = marco;
    panel->dialogo_so = dialogo_so;
	panel->implementacion_panel = i_implementacion_recursivamente(
							marco,
 							panel->tipo_elemento, 
                            panel->num_filas, 
                            panel->num_columnas,
                            panel->componente,
 							panel->tipo_disposicion, 
                            panel->hijos,
                            dialogo_so, num_recursividad);
            
    i_implementacion_timesproc(panel->times_actions, panel->componente, marco);
	
    return panel->implementacion_panel;
}

//---------------------------------------------------------

static bool i_se_puede_finalizar(
                        const struct marcogrf_t *marco,
                        unsigned long num_recursividad,
                        enum i_tipo_elemento_t tipo_elemento,
                        struct componente_t *componente,
                        ArrTipo(panel_t) *hijos,
                        char **mensaje_error)
{
	assert(num_recursividad < 1000);
	num_recursividad++;
	
	switch(tipo_elemento)
	{
		case i_PANEL:
		{
			unsigned long num_hijos, i;
			
			num_hijos = array_numElementos(panel_t, hijos);
            
			for (i = 0; i < num_hijos; i++)
			{
				struct panel_t *panel_hijo;
				
				panel_hijo = array_Get(panel_t, hijos, i);
				assert_no_null(panel_hijo);
                if (i_se_puede_finalizar(
                                marco, num_recursividad,
                                panel_hijo->tipo_elemento,
                                panel_hijo->componente,
                                panel_hijo->hijos,
                                mensaje_error) == false)
                {
                    return false;
                }
			}
			
            return true;
		}
		case i_COMPONENTE:
			
			return componente_se_puede_finalizar(marco, componente, mensaje_error);
			
		default_error();
	}
    
    assert_return_error(bool);
}

//---------------------------------------------------------

bool panel_se_puede_finalizar(struct panel_t *panel, const struct marcogrf_t *marco, char **mensaje_error)
{
    unsigned long num_recursividad;
    
    i_integridad(panel);
    num_recursividad = 0;
    
    return i_se_puede_finalizar(
                        marco, num_recursividad, panel->tipo_elemento, panel->componente,
                        panel->hijos,  mensaje_error);

}

//---------------------------------------------------------

static void i_acepta_panel(
                        const struct marcogrf_t *marco,
                        unsigned long num_recursividad,
                        enum i_tipo_elemento_t tipo_elemento,
                        struct componente_t *componente,
                        ArrTipo(panel_t) *hijos,
                        struct item_datos_panel_t *datos_aceptar,
                        panel_FPtr_aceptado func_aceptar)
{
	assert(num_recursividad < 1000);
	num_recursividad++;
	
	switch(tipo_elemento)
	{
		case i_PANEL:
		{
			unsigned long num_hijos, i;
			
			num_hijos = array_numElementos(panel_t, hijos);
            
			for (i = 0; i < num_hijos; i++)
			{
				struct panel_t *panel_hijo;
				
				panel_hijo = array_Get(panel_t, hijos, i);
				assert_no_null(panel_hijo);
                i_acepta_panel(
                            marco, num_recursividad,
                            panel_hijo->tipo_elemento,
                            panel_hijo->componente,
                            panel_hijo->hijos,
                            panel_hijo->datos_aceptar,
                            panel_hijo->func_aceptar);
			}
            break;
		}
		case i_COMPONENTE:
			
			componente_aceptar(marco, componente);
            break;
			
		default_error();
	}

    if (func_aceptar != NULL)
        func_aceptar(datos_aceptar);
}

//---------------------------------------------------------

void panel_aceptar(struct panel_t *panel, const struct marcogrf_t *marco)
{
    unsigned long num_recursividad;
    
    i_integridad(panel);
    num_recursividad = 0;
    i_acepta_panel(
                marco, num_recursividad, 
                panel->tipo_elemento, panel->componente, panel->hijos,
                panel->datos_aceptar, panel->func_aceptar);
}

//---------------------------------------------------------

static void i_titulos_o_datos_componentes(
                        ArrString *textos,
                        unsigned long num_recursividad,
                        enum i_tipo_elemento_t tipo_elemento,
                        struct componente_t *componente,
                        ArrTipo(panel_t) *hijos,
                        ArrString *func_texto(const struct componente_t *))
{
	assert(num_recursividad < 1000);
	num_recursividad++;
	
	switch(tipo_elemento)
	{
		case i_PANEL:
		{
			unsigned long num_hijos, i;
			
			num_hijos = array_numElementos(panel_t, hijos);
            
			for (i = 0; i < num_hijos; i++)
			{
				struct panel_t *panel_hijo;
				
				panel_hijo = array_Get(panel_t, hijos, i);
				assert_no_null(panel_hijo);
                i_titulos_o_datos_componentes(textos, num_recursividad, panel_hijo->tipo_elemento, panel_hijo->componente, panel_hijo->hijos, func_texto);
			}
            break;
		}
		case i_COMPONENTE:
        {
            ArrString *cadenas;
            
			cadenas = func_texto(componente);
            
            if (cadenas != NULL)
            {
                arrcad_Concatena(textos, cadenas);
                arrcad_Destruye(&cadenas);
            }
            break;
        }
		default_error();
	}
}

//---------------------------------------------------------

ArrString *panel_titulos_componentes(struct panel_t *panel)
{
    ArrString *titulos;
    unsigned long num_recursividad;
    
    i_integridad(panel);
    num_recursividad = 0;
    titulos = arrcad_CreaString(0);
    
    i_titulos_o_datos_componentes(
                        titulos, num_recursividad,
                        panel->tipo_elemento,
                        panel->componente,
                        panel->hijos,
                        componente_titulo);
    
    return titulos;
}

//---------------------------------------------------------

ArrString *panel_datos_componentes(struct panel_t *panel)
{
    ArrString *datos;
    unsigned long num_recursividad;
    
    i_integridad(panel);
    num_recursividad = 0;
    datos = arrcad_CreaString(0);
    
    i_titulos_o_datos_componentes(
                        datos, num_recursividad,
                        panel->tipo_elemento,
                        panel->componente,
                        panel->hijos,
                        componente_dato);
    
    return datos;
}

//---------------------------------------------------------

void panel_set_dialogo_padre(struct panel_t *panel, struct dialogo_t *dialogo)
{
    unsigned long num_recursividad;
    
    i_integridad(panel);
    num_recursividad = 0;
    
    i_set_dialogo_padre(
                num_recursividad,
                panel->tipo_elemento,
                panel->componente,
                panel->hijos, dialogo);
}

//---------------------------------------------------------

void panel_nousar_append_datos_panel(
                struct panel_t *panel, 
                struct item_datos_panel_t **item_datos_panel, 
                panel_FPtr_destruye_datos_panel func_destruye_datos_panel)
{
    struct i_datos_panel_t *datos_panel;
    
    i_integridad(panel);
    
    datos_panel = i_crea_datos_panel(item_datos_panel, func_destruye_datos_panel);
    array_Anyadir(i_datos_panel_t, panel->datos_panel, datos_panel);
}

//---------------------------------------------------------

void panel_nousar_set_funcion_aceptar(
                struct panel_t *panel, 
                struct item_datos_panel_t *datos_aceptar, 
                panel_FPtr_aceptado func_aceptar)
{
    i_integridad(panel);
    
    assert(panel->datos_aceptar == NULL);
    assert(panel->func_aceptar == NULL);
    
    panel->datos_aceptar = datos_aceptar;
    panel->func_aceptar = func_aceptar;
}

//---------------------------------------------------------

bool panel_acepta_si_se_puede(struct panel_t *panel, char **mensaje_error)
{
    bool aceptado;
    unsigned long num_recursividad;
    
    i_integridad(panel);
    num_recursividad = 0;
    
    aceptado = i_se_puede_finalizar(
                    panel->marco, num_recursividad, panel->tipo_elemento, panel->componente,
                    panel->hijos,  mensaje_error);
    
    if (aceptado == true)
    {
        i_acepta_panel(
                    panel->marco, num_recursividad, 
                    panel->tipo_elemento, panel->componente, panel->hijos,
                    panel->datos_aceptar, panel->func_aceptar);
    }
    
    return aceptado;
}

//---------------------------------------------------------

static void i_appendComponentsOf(
					const ArrTipo(panel_t) *sons,
					const char *type, 
					struct Array_t *elementsOf, 
					unsigned long numRecursivity)
{
	unsigned long i, num;
	
	numRecursivity++;
	assert(numRecursivity < 1000);
	
	num = array_numElementos(panel_t, sons);
	
	for (i = 0; i < num; i++)
	{
		struct panel_t *son;
		
		son = array_Get(panel_t, sons, i);
		assert_no_null(son);
		
		switch(son->tipo_elemento)
		{
			case i_PANEL:
			
				i_appendComponentsOf(son->hijos, type, elementsOf, numRecursivity);
				break;
			 
			case i_COMPONENTE:
			{
				struct componente_datos_item_t *data;
				
				if (componente_isA(son->componente, type, &data) == true)
					array_AnyadirPrivada(type, elementsOf, data);
				break;
			}	
			default_error();
		}
	}
}

//---------------------------------------------------------

struct Array_t *panel_componentsOfPrivate(struct panel_t *panel, const char *type)
{
	struct Array_t *elements;
	unsigned long numRecursivity;
	
	i_integridad(panel);
	
	elements = array_CreaPrivada(type, 0);
	
	numRecursivity = 0;
	
	switch(panel->tipo_elemento)
	{
		case i_PANEL:
		
			i_appendComponentsOf(panel->hijos, type, elements, numRecursivity);
			break;
		
		case i_COMPONENTE:
		{
			struct componente_datos_item_t *data;
			
			if (componente_isA(panel->componente, type, &data) == true)
				array_AnyadirPrivada(type, elements, data);
			break;
		}	
		default_error();
	}
	
	return elements;
}

//---------------------------------------------------------

void panel_append_time(struct panel_t *panel, unsigned long timeout_miliseconds, struct rcpaccion_t **rcp_process_time)  
{
    struct timeproc_t *timeproc;
    
    assert_no_null(panel);
    
    timeproc = timeproc_create_process_time(timeout_miliseconds, rcp_process_time); 
    array_Anyadir(timeproc_t, panel->times_actions, timeproc);
}

//---------------------------------------------------------

void panel_appendReceptorKeys(struct panel_t *panel, struct RcpKey_t **rcpKey)
{
    assert_no_null(panel);
    assert_no_null(rcpKey);
    assert_no_null(*rcpKey);

    array_Anyadir(RcpKey_t, panel->receptores_teclas, *rcpKey);
    *rcpKey = NULL;    
}

//---------------------------------------------------------

static bool i_hay_receptores_teclas_en_hijos(unsigned long num_recursividad, ArrTipo(panel_t) *hijos)
{
	unsigned long num_hijos, i;

	assert(num_recursividad < 1000);
	num_recursividad++;
	
	num_hijos = array_numElementos(panel_t, hijos);
            
	for (i = 0; i < num_hijos; i++)
	{
		struct panel_t *panel_hijo;
				
		panel_hijo = array_Get(panel_t, hijos, i);
	
		if (array_numElementos(RcpKey_t, panel_hijo->receptores_teclas) > 0)
			return true;
		else
		{
			if (panel_hijo->hijos != NULL && i_hay_receptores_teclas_en_hijos(num_recursividad, panel_hijo->hijos) == true)
				return true;
		}
	}

	return false;
}

//---------------------------------------------------------

bool panel_hay_receptores_tecla(struct panel_t *panel)
{
	if (array_numElementos(RcpKey_t, panel->receptores_teclas) > 0)
		return true;
	else if (panel->hijos != NULL)
		return i_hay_receptores_teclas_en_hijos(0, panel->hijos);
	else
		return false;
}

//---------------------------------------------------------

static void i_sendAllReceptors(const ArrTipo(RcpKey_t) *receptores_teclas, struct EvtKey_t *evtKey)
{
    unsigned long i, num_receptores;
    
    num_receptores = array_numElementos(RcpKey_t, receptores_teclas);
    
    for (i = 0; i < num_receptores; i++)
    {
        struct RcpKey_t *receptorKey;
            
        receptorKey = array_Get(RcpKey_t, receptores_teclas, i);
        rcpkey_sendEvent(receptorKey, evtKey);
    }
}

//---------------------------------------------------------

static void i_sendAllSons(unsigned long num_recursividad, ArrTipo(panel_t) *hijos, struct EvtKey_t *evtKey)
{
	unsigned long num_hijos, i;

	assert(num_recursividad < 1000);
	num_recursividad++;
	
	num_hijos = array_numElementos(panel_t, hijos);
            
	for (i = 0; i < num_hijos; i++)
	{
		struct panel_t *panel_hijo;
				
		panel_hijo = array_Get(panel_t, hijos, i);
		
		i_sendAllReceptors(panel_hijo->receptores_teclas, evtKey);
		
		if (evtkey_isEnded(evtKey) == false && panel_hijo->hijos != NULL)
			i_sendAllSons(num_recursividad, panel_hijo->hijos, evtKey);
		else
			break;
	}
}

//---------------------------------------------------------

void panel_sendEventKey(struct panel_t *panel, struct EvtKey_t *evtKey)
{
    assert_no_null(panel);
    
	i_sendAllReceptors(panel->receptores_teclas, evtKey);
	
	if (evtkey_isEnded(evtKey) == false)
		i_sendAllSons(0, panel->hijos, evtKey);
}

//---------------------------------------------------------

void panel_set_expand(struct panel_t *panel)
{
	assert_no_null(panel);
	assert(panel->expand == false);
	
	panel->expand = true;
}

//---------------------------------------------------------

void panel_set_fill(struct panel_t *panel)
{
	assert_no_null(panel);
	assert(panel->fill == false);
	
	panel->fill = true;
}
