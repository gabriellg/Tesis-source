// Receptores de acciones

#include "rcpaccion.hxx"

void rcpaccion_destruye(struct rcpaccion_t **receptor_accion);

void rcpaccion_envia(struct rcpaccion_t *receptor_accion, struct componente_t *componente, struct dialogo_t *dialogo_padre);
				
