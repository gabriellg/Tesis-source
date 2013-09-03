// Definicion de un servicio.

void servicio_destruye(struct servicio_t **servicio);

void servicio_ejecuta(const struct servicio_t *servicio, int hand_socket);

bool servicio_es_nombre_servicio(const struct servicio_t *servicio, const char *nombre_servicio);

void servicio_envia_nombre_servicio(const struct servicio_t *servicio, int hand_socket);

