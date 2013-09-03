// Eventos de accion

struct evtaccion_t *evtaccion_crea(struct componente_t *componente, struct dialogo_t *dialogo_padre, struct item_datos_accion_t *datos_accion);
							
void evtaccion_destruye(struct evtaccion_t **evtaccion);

