// Receptores de acciones

#include "rcpaccion.hxx"

struct rcpaccion_t *rcpaccion_no_usar_crea(
					rcpaccion_FPtr_accion func_accion,
					rcpaccion_FPtr_destruye func_destruye,
					struct item_datos_accion_t **datos_accion);

#define rcpaccion_crea(func_accion, func_destruye, tipo, datos_accion)\
					(\
					(void)((struct tipo **)datos_accion == datos_accion),\
					RCPACCION_COMPROBAR_FUNC_DESTRUYE(tipo, func_destruye),\
					rcpaccion_no_usar_crea(\
						func_accion,\
						(rcpaccion_FPtr_destruye)func_destruye,\
						(struct item_datos_accion_t **)datos_accion)\
					)

#define rcpaccion_crea_class(func_accion, func_destruye, tipo, datos_accion)\
					(\
					(void)((class tipo **)datos_accion == datos_accion),\
					RCPACCION_COMPROBAR_FUNC_DESTRUYE(tipo, func_destruye),\
					rcpaccion_no_usar_crea(\
						func_accion,\
						(rcpaccion_FPtr_destruye)func_destruye,\
						(struct item_datos_accion_t **)datos_accion)\
					)
					
struct rcpaccion_t *rcpaccion_no_usar_crea_sin_destruye(
					rcpaccion_FPtr_accion func_accion,
					struct item_datos_accion_t *datos_accion);
					
#define rcpaccion_crea_sin_destruye(func_accion, tipo, datos_accion)\
					(\
					(void)((const struct tipo *)datos_accion == datos_accion),\
					rcpaccion_no_usar_crea_sin_destruye(\
						func_accion,\
						(struct item_datos_accion_t *)(void *)datos_accion)\
					)

#define rcpaccion_crea_sin_destruye_class(func_accion, tipo, datos_accion)\
					(\
					(void)((const class tipo *)datos_accion == datos_accion),\
					rcpaccion_no_usar_crea_sin_destruye(\
						func_accion,\
						(struct item_datos_accion_t *)(void *)datos_accion)\
					)
