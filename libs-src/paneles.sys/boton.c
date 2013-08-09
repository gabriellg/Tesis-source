// Definicion de boton

#include "boton.h"

#include "rcpaccion.inl"
#include "componente.inl"
#include "panel.inl"

#include "asrtbas.h"
#include "memory.h"
#include "array.h"
#include "strbas.h"
#include "marcogrf.syh"
#include "callback.syh"

struct boton_t
{
	char *texto;
	ArrTipo(rcpaccion_t) *receptores;
	struct callback_t *callback;
    struct componente_t *componente;
};

//---------------------------------------------------------

static void i_pulsar(struct boton_t *boton)
{
	unsigned long i, num_receptores;
	struct dialogo_t *dialogo_padre;
	
	assert_no_null(boton);
	
	dialogo_padre = componente_get_dialogo_padre(boton->componente);
	
	num_receptores = array_numElementos(rcpaccion_t, boton->receptores);
	
	for (i = 0; i < num_receptores; i++)
	{
		struct rcpaccion_t *receptor_accion;
			
		receptor_accion = array_Get(rcpaccion_t, boton->receptores, i);
		rcpaccion_envia(receptor_accion, boton->componente, dialogo_padre);
	}
}

//---------------------------------------------------------

static struct boton_t *i_crea(
						const char *texto, 
						ArrTipo(rcpaccion_t) **receptores,
						struct callback_t **callback,
                        struct componente_t **componente)
{
	struct boton_t *boton;
		
	boton = MALLOC(struct boton_t);
		
	boton->texto = strbas_copyString(texto);
	boton->receptores = ASSIGN_PP(receptores, ArrTipo(rcpaccion_t));
	boton->callback = ASSIGN_PP(callback, struct callback_t);
    boton->componente = ASSIGN_PP(componente, struct componente_t);
	
	return boton;
}

//---------------------------------------------------------

static void i_destruye(struct boton_t **boton)
{
	assert_no_null(boton);
	assert_no_null(*boton);
	
	strbas_destroy(&(*boton)->texto);
	array_Destruye(rcpaccion_t, &(*boton)->receptores, rcpaccion_destruye);
	callback_destruye(&(*boton)->callback);
	
	FREE_T(boton, struct boton_t);
}

//---------------------------------------------------------

static struct item_implementacion_t *i_crea_implementacion(
						struct marcogrf_t *marco, 
						struct boton_t *boton,
                        struct item_implementacion_dialogo_t *dialogo_so)
{
	assert_no_null(boton);
	assert(boton->callback == NULL);
	
	boton->callback = callback_crea(struct boton_t, boton, i_pulsar, NULL);
	
    return marcogrf_crea_boton(marco, dialogo_so, boton->texto, boton->callback);
}

//---------------------------------------------------------
	
struct panel_t *boton_crea(const char *texto, struct rcpaccion_t *receptor_accion)
{
	ArrTipo(rcpaccion_t) *receptores;
	struct boton_t *boton, *boton_aux;
	struct componente_t *componente;
	struct callback_t *callback;
	
	assert_no_null(receptor_accion);
	receptores = array_CreaPuntero(1, rcpaccion_t);
	array_Set(rcpaccion_t, receptores, 0, receptor_accion);
	callback = NULL;
    componente = NULL;
	
	boton = i_crea(texto, &receptores, &callback, &componente);
    boton_aux = boton;
	
	componente = componente_crea_defecto(boton_t, &boton_aux, i_crea_implementacion, i_destruye, NULL, NULL, NULL, NULL);
    boton->componente = componente;
	
	return panel_crea_componente(&componente);
}
