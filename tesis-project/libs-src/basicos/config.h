// Definici�n estandar para leer ficheros de texto de configuraciom

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct config_t *config_lee_configuracion(const char *nombre_fichero);

struct config_t *config_copia(const struct config_t *configuracion);

void config_destruye(struct config_t **configuracion);

bool config_existe_variable(const struct config_t *configuracion, const char *nombre_variable);

ArrString *config_obtener_valor(const struct config_t *configuracion, const char *nombre_variable);
					
#ifdef __cplusplus
}
#endif /* __cplusplus */

