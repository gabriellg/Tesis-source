// Definici�n de la configuraci�n de la gesti�n de proyectos

#include "gesenum.hxx"

enum tipo_compilacion_t
{
	CFGPROY_RELEASE,
	CFGPROY_DEBUG,
	CFGPROY_PROFILER,
	CFGPROY_LINT
};

struct cfgproy_t *cfgproy_crea_defecto(const char *home, enum gesenum_sistema_operativo_t sistema_operativo);

void cfgproy_destruye(struct cfgproy_t **cfg_proyectos);
	
void cfgproy_generar_proyectos(struct cfgproy_t *cfg_proyectos);

void cfgproy_debug(const struct cfgproy_t *cfg_proyectos, struct marco_t *marco);

ArrString *cfgproy_es_incluido(const struct cfgproy_t *cfg_proyectos, const char *fichero);
	
bool cfgproy_dependencias_proyectos(
						const struct cfgproy_t *cfg_proyectos, 
						const char *nombre_proyecto,
						ArrString **pnombres_proyectos);

void cfgproy_hacer_ejecutables(const struct cfgproy_t *cfg_proyectos, enum tipo_compilacion_t tipo_compilacion, struct marco_t *marco);

void cfgproy_integridad_sistema(const struct cfgproy_t *cfg_proyectos, ArrString **mensajes);

void cfgproy_clean_segun_compilacion(
							const struct cfgproy_t *cfg_proyectos,
							enum tipo_compilacion_t tipo_compilacion,
							struct marco_t *marco);

void cfgproy_crea_directorios_necesarios(struct marco_t *marco, const char *directorio_base);

bool cfgproy_verifica_directorios(const char *directorio_base, char **mensaje_error_opc);

void cfgproy_genera_mak_proyecto(const struct cfgproy_t *cfg_proyectos, enum tipo_compilacion_t tipo_compilacion, const char *nombre_base_proyecto, struct marco_t *marco);
