// Definici�n de una variable

struct variable_t *variable_crea_defecto(const char *nombre_variable);

void variable_destruye(struct variable_t **variable);

struct variable_t *variable_copia(const struct variable_t *variable);

bool variable_es_variable(const struct variable_t *variable, const char *nombre);

void variable_append_valor(const struct variable_t *variable, const char *nuevo_valor);
