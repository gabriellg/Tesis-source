// Marco de aplicaci�n

#include "marco.tlh"

void marco_lanza_mensaje(const struct marco_t *marco, enum marco_tipo_mensaje_t tipo_mensaje, const char *mensaje);

void marco_lanza_explicacion(const struct marco_t *marco, enum marco_tipo_mensaje_t tipo_mensaje, const ArrString *explicacion);

bool marco_lanza_pregunta(const struct marco_t *marco, const char *pregunta);
