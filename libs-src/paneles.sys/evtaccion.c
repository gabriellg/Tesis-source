// Eventos de accion

#include "evtaccion.h"
#include "evtaccion.inl"

#include "componente.inl"

#include "asrtbas.h"
#include "memory.h"

struct evtaccion_t
{
    struct componente_t *componente;
	struct dialogo_t *dialogo_padre;
	struct item_datos_accion_t *datos_accion;
};

//---------------------------------------------------------

struct evtaccion_t *evtaccion_crea(struct componente_t *componente, struct dialogo_t *dialogo_padre, struct item_datos_accion_t *datos_accion)
{
	struct evtaccion_t *evento_accion;
		
	evento_accion = MALLOC(struct evtaccion_t);
		
	evento_accion->datos_accion = datos_accion;
	evento_accion->dialogo_padre = dialogo_padre;
    evento_accion->componente = componente;
	
	return evento_accion;
}
	
//---------------------------------------------------------

void evtaccion_destruye(struct evtaccion_t **evento_accion)
{
	FREE_T(evento_accion, struct evtaccion_t);
}

//---------------------------------------------------------

struct item_datos_accion_t *evtaccion_no_usar_datos(struct evtaccion_t *evento_accion)
{
	assert_no_null(evento_accion);
	return evento_accion->datos_accion;
}

//---------------------------------------------------------

struct dialogo_t *evtaccion_dialogo_padre(struct evtaccion_t *evtaccion)
{
    assert_no_null(evtaccion);
    return evtaccion->dialogo_padre;
}
