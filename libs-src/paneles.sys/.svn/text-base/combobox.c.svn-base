// Definicion de combobox

#include "combobox.h"

#include "componente.inl"
#include "panel.inl"

#include "asrtbas.h"
#include "memory.h"
#include "array.h"
#include "arrcad.h"
#include "strbas.h"
#include "marcogrf.syh"

struct i_opcion_t
{
    unsigned long id;
    char *texto;
};

struct combobox_t
{
    char *titulo;
    unsigned long *valor;
    ArrTipo(i_opcion_t) *opciones;

    const struct marcogrf_t *marco;
    struct item_implementacion_t *implementacion;
        
    struct item_datos_cambio_t *datos_cambio;
    combobox_ha_cambiado_opcion func_ha_cambiado_opcion;
};

//---------------------------------------------------------

static struct i_opcion_t *i_crea_opcion(unsigned long id, const char *texto)
{
    struct i_opcion_t *opcion;
        
    opcion = MALLOC(struct i_opcion_t);
    
    opcion->id = id;
    opcion->texto = strbas_copyString(texto);
    
    return opcion;
}

//---------------------------------------------------------

static bool i_busca_opcion(const struct i_opcion_t *opcion, const unsigned long *valor)
{
    assert_no_null(opcion);
    assert_no_null(valor);
    
    if (opcion->id == *valor)
        return true;
    else
        return false;
}

//---------------------------------------------------------

static void i_destruye_opcion(struct i_opcion_t **opcion)
{
    assert_no_null(opcion);
    assert_no_null(*opcion);
    
    strbas_destroy(&(*opcion)->texto);
    
    FREE_T(opcion, struct i_opcion_t);
}

//---------------------------------------------------------

static struct combobox_t *i_crea(
                        const char *titulo,
                        unsigned long *valor,
                        ArrTipo(i_opcion_t) **opciones,
                        struct marcogrf_t *marco,
                        struct item_implementacion_t **implementacion,
                        struct item_datos_cambio_t *datos_cambio,
                        combobox_ha_cambiado_opcion  func_ha_cambiado_opcion)
{
	struct combobox_t *combobox;
		
	combobox = MALLOC(struct combobox_t);
		
    combobox->titulo = strbas_copyString(titulo);
	combobox->valor = valor;
	combobox->opciones = ASSIGN_PP(opciones, ArrTipo(i_opcion_t));
    combobox->marco = marco;
    combobox->implementacion = ASSIGN_PP(implementacion, struct item_implementacion_t);
    combobox->datos_cambio = datos_cambio;
    combobox->func_ha_cambiado_opcion = func_ha_cambiado_opcion;
	
	return combobox;
}

//---------------------------------------------------------

static void i_destruye(struct combobox_t **combobox)
{
	assert_no_null(combobox);
	assert_no_null(*combobox);
	
    strbas_destroy(&(*combobox)->titulo);
	array_Destruye(i_opcion_t, &(*combobox)->opciones, i_destruye_opcion);
	
	FREE_T(combobox, struct combobox_t);
}

//---------------------------------------------------------

static ArrString *i_crea_opciones(const ArrTipo(i_opcion_t) *opciones)
{
    ArrString *texto_opciones;
    unsigned long i, num_opciones;
    
    num_opciones = array_numElementos(i_opcion_t, opciones);
    
    texto_opciones = arrcad_CreaString(num_opciones);
    
    for (i = 0; i < num_opciones; i++)
    {
        const struct i_opcion_t *opcion;
            
        opcion = array_Get(i_opcion_t, opciones, i);
        arrcad_Set(texto_opciones, i, opcion->texto);
    }
    
    return texto_opciones;
}

//---------------------------------------------------------

static void i_ha_cambiado_opcion(struct combobox_t *combobox, unsigned long ind_opcion_activada)
{
    assert_no_null(combobox);
    
    if (combobox->datos_cambio != NULL)
    {
        assert_no_null(combobox->func_ha_cambiado_opcion);
        combobox->func_ha_cambiado_opcion(combobox->datos_cambio, ind_opcion_activada);
    }
    else
        assert(combobox->func_ha_cambiado_opcion == NULL);
}

//---------------------------------------------------------

static struct item_implementacion_t *i_crea_implementacion(
						struct marcogrf_t *marco, 
						struct combobox_t *combobox,
                        struct item_implementacion_dialogo_t *dialogo_so)
{
    ArrString *texto_opciones;
    unsigned long ind_valor;
    bool existe;
    
	assert_no_null(combobox);
    assert(combobox->implementacion == NULL);
    assert(combobox->marco == NULL);
	
    existe = array_BuscarUnica(i_opcion_t, combobox->opciones, unsigned long, combobox->valor, i_busca_opcion, &ind_valor);
    assert(existe == true);
    
    texto_opciones = i_crea_opciones(combobox->opciones);
    
	combobox->implementacion = marcogrf_crea_combobox(marco, dialogo_so, combobox, i_ha_cambiado_opcion, texto_opciones, ind_valor);
    combobox->marco = marco;
    
    arrcad_Destruye(&texto_opciones);
    
    return combobox->implementacion;
}

//---------------------------------------------------------

static void i_aceptar(const struct marcogrf_t *marco, struct combobox_t *combobox)
{
    unsigned long ind_opcion;
    struct i_opcion_t *opcion;
    
    assert_no_null(combobox);
    assert_no_null(combobox->valor);

    ind_opcion = marcogrf_get_opcion_combobox(marco, combobox->implementacion);
    opcion = array_Get(i_opcion_t, combobox->opciones, ind_opcion);
    
    assert_no_null(opcion);
    *(combobox->valor) = opcion->id;
}

//---------------------------------------------------------

static ArrString *i_get_titulo(const struct combobox_t *combobox)
{
    ArrString *titulos;
    
    assert_no_null(combobox);
    
    titulos = arrcad_CreaString(1);
    arrcad_Set(titulos, 0, combobox->titulo);
    
    return titulos;
}

//---------------------------------------------------------

static ArrString *i_get_valor(const struct combobox_t *combobox)
{
    unsigned long ind_opcion;
    struct i_opcion_t *opcion;
    bool existe;
    ArrString *datos;
    
    assert_no_null(combobox);
    assert_no_null(combobox->valor);

    existe = array_BuscarUnica(i_opcion_t, combobox->opciones, unsigned long, combobox->valor, i_busca_opcion, &ind_opcion);
    assert(existe == true);
    opcion = array_Get(i_opcion_t, combobox->opciones, ind_opcion);

    datos = arrcad_CreaString(1);
    arrcad_Set(datos, 0, opcion->texto);
    
    return datos;
}

//---------------------------------------------------------
	
struct panel_t *combobox_crea(const char *titulo, unsigned long *valor, struct combobox_t **ptr_combobox)
{
	ArrTipo(i_opcion_t) *opciones;
	struct combobox_t *combobox;
	struct componente_t *componente;
    struct item_implementacion_t *implementacion;
    struct marcogrf_t *marco;
    struct item_datos_cambio_t *datos_cambio;
    combobox_ha_cambiado_opcion func_ha_cambiado_opcion;
	
	opciones = array_CreaPuntero(0, i_opcion_t);
    implementacion = NULL;
    marco = NULL;
    datos_cambio = NULL;
    func_ha_cambiado_opcion = NULL;
	
	combobox = i_crea(titulo, valor, &opciones, marco, &implementacion, datos_cambio, func_ha_cambiado_opcion);

	if (ptr_combobox != NULL)
        *ptr_combobox = combobox;
	
	componente = componente_crea_defecto(combobox_t, &combobox, i_crea_implementacion, i_destruye, NULL, i_aceptar, i_get_titulo, i_get_valor);

	return panel_crea_componente(&componente);
}

//---------------------------------------------------------

void combobox_anade_opcion(struct combobox_t *combobox, unsigned long id_valor, const char *texto_opcion)
{
    struct i_opcion_t *opcion;
    
    assert_no_null(combobox);
    assert(combobox->implementacion == NULL);
    
    opcion = i_crea_opcion(id_valor, texto_opcion);
    array_Anyadir(i_opcion_t, combobox->opciones, opcion);
}

//---------------------------------------------------------

void combobox_nousar_set_ha_cambiado_opcion(
                    struct combobox_t *combobox, 
                    struct item_datos_cambio_t *datos_cambio,
                    combobox_ha_cambiado_opcion func_ha_cambiado_opcion)
{
    assert_no_null(combobox);
    assert(combobox->datos_cambio == NULL);
    assert(combobox->func_ha_cambiado_opcion == NULL);
    
    combobox->datos_cambio = datos_cambio;
    combobox->func_ha_cambiado_opcion = func_ha_cambiado_opcion;
}
