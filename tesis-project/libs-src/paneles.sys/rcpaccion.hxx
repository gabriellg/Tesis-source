// Receptores de acciones

#ifndef __RECEPTOR_ACCION__
#define __RECEPTOR_ACCION__

struct evtaccion_t;
struct item_datos_accion_t;

typedef void (*rcpaccion_FPtr_accion)(struct evtaccion_t *evt);
typedef void (*rcpaccion_FPtr_destruye)(struct item_datos_accion_t **);
	
#define RCPACCION_COMPROBAR_FUNC_DESTRUYE(tipo, funcion) (void)((void (*)(struct tipo**))funcion == funcion)

#endif
