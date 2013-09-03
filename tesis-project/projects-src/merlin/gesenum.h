// Definici�n de enums 

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "gesenum.hxx"

bool gesenum_es_cadena_sistema_operativo(const char *cadena_sistema_operativo, enum gesenum_sistema_operativo_t *sistema_operativo);

const char *gesenum_extension_segun_sistema_operativo(enum gesenum_sistema_operativo_t sistema_operativo);

#ifdef __cplusplus
}
#endif /* __cplusplus */
