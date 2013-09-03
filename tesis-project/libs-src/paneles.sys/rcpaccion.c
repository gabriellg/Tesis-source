// Receptores de acciones

#include "rcpaccion.h"
#include "rcpaccion.inl"

#include "evtaccion.inl"

#include "asrtbas.h"
#include "memory.h"

struct rcpaccion_t
{
	struct item_datos_accion_t *datos_accion;
		
	rcpaccion_FPtr_accion func_accion;
	rcpaccion_FPtr_destruye func_destruye;
};

//---------------------------------------------------------

static void i_integridad(const struct rcpaccion_t *rcpaccion)
{
	assert_no_null(rcpaccion);
	assert_no_null(rcpaccion->func_accion);
}

//---------------------------------------------------------

static struct rcpaccion_t *i_crea(
						struct item_datos_accion_t **datos_accion,
						rcpaccion_FPtr_accion func_accion,
						rcpaccion_FPtr_destruye func_destruye)
{
	struct rcpaccion_t *receptor_accion;
	
	receptor_accion = MALLOC(struct rcpaccion_t);
	
	receptor_accion->datos_accion = ASSIGN_PP(datos_accion, struct item_datos_accion_t);
	receptor_accion->func_accion = func_accion;
	receptor_accion->func_destruye = func_destruye;

	i_integridad(receptor_accion);
	
	return receptor_accion;
}

//---------------------------------------------------------

struct rcpaccion_t *rcpaccion_no_usar_crea(
					rcpaccion_FPtr_accion func_accion,
					rcpaccion_FPtr_destruye func_destruye,
					struct item_datos_accion_t **datos_accion)
{
	return i_crea(datos_accion, func_accion, func_destruye);
}

//---------------------------------------------------------

struct rcpaccion_t *rcpaccion_no_usar_crea_sin_destruye(
					rcpaccion_FPtr_accion func_accion,
					struct item_datos_accion_t *datos_accion)
{
	rcpaccion_FPtr_destruye func_destruye;
	
	func_destruye = NULL;
							
	return i_crea(&datos_accion, func_accion, func_destruye);
}

//---------------------------------------------------------

void rcpaccion_destruye(struct rcpaccion_t **receptor_accion)
{
	assert_no_null(receptor_accion);
	i_integridad(*receptor_accion);
	
	if ((*receptor_accion)->datos_accion != NULL && (*receptor_accion)->func_destruye != NULL)
		(*receptor_accion)->func_destruye(&(*receptor_accion)->datos_accion);
	
	FREE_T(receptor_accion, struct rcpaccion_t);
}

//---------------------------------------------------------

void rcpaccion_envia(
				struct rcpaccion_t *receptor_accion, 
				struct componente_t *componente, 
				struct dialogo_t *dialogo_padre)
{
	struct evtaccion_t *evento_accion;
	
	i_integridad(receptor_accion);
	
	evento_accion = evtaccion_crea(componente, dialogo_padre, (struct item_datos_accion_t *)receptor_accion->datos_accion);
	
	receptor_accion->func_accion(evento_accion);
	
	evtaccion_destruye(&evento_accion);
}
