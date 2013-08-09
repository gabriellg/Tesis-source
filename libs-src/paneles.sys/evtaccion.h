// Eventos de accion

struct evtaccion_t;

struct item_datos_accion_t *evtaccion_no_usar_datos(struct evtaccion_t *evtaccion);
    
#define evtaccion_datos(tipo, evtaccion) (tipo *)evtaccion_no_usar_datos(evtaccion)
	
struct dialogo_t *evtaccion_dialogo_padre(struct evtaccion_t *evtaccion);
