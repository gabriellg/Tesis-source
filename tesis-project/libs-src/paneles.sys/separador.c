// Implementacion de separadores

#include "separador.h"

#include "panel.inl"
#include "componente.inl"

#include "asrtbas.h"
#include "memory.h"
#include "marcogrf.syh"

struct separador_t
{
    enum marcogrf_tipo_separador_t tipo_separador;
};

//---------------------------------------------------------

static struct separador_t *i_crea(enum marcogrf_tipo_separador_t tipo_separador)
						
{
	struct separador_t *separador;
		
	separador = MALLOC(struct separador_t);
		
	separador->tipo_separador = tipo_separador;
	
	return separador;
}

//---------------------------------------------------------

static void i_destruye(struct separador_t **separador)
{
	FREE_T(separador, struct separador_t);
}

//---------------------------------------------------------

static struct item_implementacion_t *i_crea_implementacion(
						struct marcogrf_t *marco, 
						struct separador_t *separador, 
                        struct item_implementacion_dialogo_t *dialogo_so)
{
	assert_no_null(separador);
	
	return marcogrf_crea_linea_separacion(marco, dialogo_so, separador->tipo_separador);
}

//---------------------------------------------------------
	
struct panel_t *separador_crea_horizontal(void)
{
	struct separador_t *separador;
	struct componente_t *componente;
	
	separador = i_crea(MARCOGRF_SEPARADOR_HORIZONTAL);
	
	componente = componente_crea_defecto(separador_t, &separador, i_crea_implementacion, i_destruye, NULL, NULL, NULL, NULL);
	
	return panel_crea_componente(&componente);
}

//---------------------------------------------------------
	
struct panel_t *separador_crea_vertical(void)
{
	struct separador_t *separador;
	struct componente_t *componente;
	
	separador = i_crea(MARCOGRF_SEPARADOR_HORIZONTAL);
	
	componente = componente_crea_defecto(separador_t, &separador, i_crea_implementacion, i_destruye, NULL, NULL, NULL, NULL);
	
	return panel_crea_componente(&componente);
}
