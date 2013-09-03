// Implementacion de Etiqueta.

#include "etiqueta.h"

#include "panel.inl"
#include "componente.inl"

#include "asrtbas.h"
#include "strbas.h"
#include "arrcad.h"
#include "memory.h"
#include "marcogrf.syh"

struct etiqueta_t
{
	enum marcogrf_justificacion_horizontal_t tipo_justificacion;
	
    bool tiene_nombre;
    char *nombre_etiqueta;
    
    char *texto;
    
    struct item_implementacion_t *implementacion;
    const struct marcogrf_t *marco;
};

//---------------------------------------------------------

static struct etiqueta_t *i_crea(
				enum marcogrf_justificacion_horizontal_t tipo_justificacion,
                bool tiene_nombre,
                const char *nombre_etiqueta,
                const char *texto,
                struct item_implementacion_t **implementacion,
                const struct marcogrf_t *marco)
{
	struct etiqueta_t *etiqueta;
		
	etiqueta = MALLOC(struct etiqueta_t);
	
	etiqueta->tipo_justificacion = tipo_justificacion;
		
    etiqueta->tiene_nombre = tiene_nombre;
    etiqueta->nombre_etiqueta = strbas_copyString(nombre_etiqueta);
	etiqueta->texto = strbas_copyString(texto);

	etiqueta->implementacion = ASSIGN_PP(implementacion, struct item_implementacion_t); 
	etiqueta->marco = marco;
	
	return etiqueta;
}

//---------------------------------------------------------

static void i_destruye(struct etiqueta_t **etiqueta)
{
	assert_no_null(etiqueta);
	assert_no_null(*etiqueta);
	
    strbas_destroy(&(*etiqueta)->nombre_etiqueta);
	strbas_destroy(&(*etiqueta)->texto);
	
	FREE_T(etiqueta, struct etiqueta_t);
}

//---------------------------------------------------------

static struct item_implementacion_t *i_crea_implementacion(
						struct marcogrf_t *marco, 
						struct etiqueta_t *etiqueta,
                        struct item_implementacion_dialogo_t *dialogo_so)
{
	assert_no_null(etiqueta);
	assert(etiqueta->marco == NULL);
	assert(etiqueta->implementacion == NULL);
	
	etiqueta->marco = marco;
	etiqueta->implementacion = marcogrf_crea_etiqueta(marco, dialogo_so, etiqueta->texto, etiqueta->tipo_justificacion);
	
	return etiqueta->implementacion;
}

//---------------------------------------------------------
	
static struct panel_t *i_creaEtiqueta(enum marcogrf_justificacion_horizontal_t tipo_justificacion, const char *texto, struct etiqueta_t **etiqueta_opc)
{
    bool tiene_nombre;
    const char *nombre_etiqueta;
	struct etiqueta_t *etiqueta;
	struct componente_t *componente;
	struct item_implementacion_t *implementacion;
	
    tiene_nombre = false;
    nombre_etiqueta = "";
    
    implementacion = NULL;
	etiqueta = i_crea(tipo_justificacion, tiene_nombre, nombre_etiqueta, texto, &implementacion, NULL);
	
	if (etiqueta_opc != NULL)
		*etiqueta_opc = etiqueta;
	
	componente = componente_crea_defecto(etiqueta_t, &etiqueta, i_crea_implementacion, i_destruye, NULL, NULL, NULL, NULL);
	
	return panel_crea_componente(&componente);
}

//---------------------------------------------------------
	
struct panel_t *etiqueta_creaJustificacionCentro(const char *texto, struct etiqueta_t **etiqueta_opc)
{
	return i_creaEtiqueta(MARCOGRF_CENTRO, texto, etiqueta_opc);
}

//---------------------------------------------------------
	
struct panel_t *etiqueta_creaJustificacionIzquierda(const char *texto, struct etiqueta_t **etiqueta_opc)
{
	return i_creaEtiqueta(MARCOGRF_IZQUIERDA, texto, etiqueta_opc);
}

//---------------------------------------------------------
	
struct panel_t *etiqueta_creaJustificacionDerecha(const char *texto, struct etiqueta_t **etiqueta_opc)
{
	return i_creaEtiqueta(MARCOGRF_DERECHA, texto, etiqueta_opc);
}

//---------------------------------------------------------

static ArrString *i_titulo(const struct etiqueta_t *etiqueta)
{
    ArrString *textos;
        
    assert_no_null(etiqueta);
    assert(etiqueta->tiene_nombre == true);
    
    textos = arrcad_CreaString(1);
    arrcad_Set(textos, 0, etiqueta->nombre_etiqueta);

    return textos;
}
    
//---------------------------------------------------------

static ArrString *i_dato(const struct etiqueta_t *etiqueta)
{
    ArrString *textos;
        
    assert_no_null(etiqueta);
    assert(etiqueta->tiene_nombre == true);
    
    textos = arrcad_CreaString(1);
    arrcad_Set(textos, 0, etiqueta->texto);

    return textos;
}

//---------------------------------------------------------
	
struct panel_t *etiqueta_crea_dato(const char *nombre_etiqueta, const char *texto)
{
    bool tiene_nombre;
	struct etiqueta_t *etiqueta;
	struct componente_t *componente;
	struct item_implementacion_t *implementacion;
	
    tiene_nombre = true;
    
    implementacion = NULL;
	etiqueta = i_crea(MARCOGRF_CENTRO, tiene_nombre, nombre_etiqueta, texto, &implementacion, NULL);
	
	componente = componente_crea_defecto(etiqueta_t, &etiqueta, i_crea_implementacion, i_destruye, NULL, NULL, i_titulo, i_dato);
	
	return panel_crea_componente(&componente);
}

//---------------------------------------------------------
	
void etiqueta_set_texto(struct etiqueta_t *etiqueta, const char *texto)
{
	assert_no_null(etiqueta);
	
	strbas_destroy(&etiqueta->texto);
	etiqueta->texto = strbas_copyString(texto);

	if (etiqueta->marco != NULL)
		marcogrf_set_texto(etiqueta->marco, etiqueta->implementacion, texto);
}
