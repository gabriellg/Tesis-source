// Manejamos datos para enviar por cliente-servidor.

struct filesocket_t *filesocket_abre_lectura(ArrString **cadenas);

struct filesocket_t *filesocket_abre_escritura(void);

void filesocket_destruye(struct filesocket_t **file);

const ArrString *filesocket_contenido(const struct filesocket_t *file);
