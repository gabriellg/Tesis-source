// Definicion Servidor.

struct servidor_t *servidor_crea(int puerto, ArrTipo(servicio_t) **servicios);

void servidor_destruye(struct servidor_t **servidor);

void servidor_ejecuta(struct servidor_t *servidor);

