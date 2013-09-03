// Definici�n de enums 

#include "gesenum.hxx"

bool gesenum_es_dependiente_sistema_operativo(const char *nombre_proyecto, enum gesenum_sistema_operativo_t *sistema_operativo);

const char *gesenum_prefijo_segun_sistema_operativo(enum gesenum_sistema_operativo_t sistema_operativo);
