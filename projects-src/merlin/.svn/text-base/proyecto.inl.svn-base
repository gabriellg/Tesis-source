// Definici�n del proyecto.

#include "gesenum.hxx"

struct proyecto_t *proyecto_genera_proyecto(const class CGenerators *generators, const char *nombre_proyecto, unsigned long *id_nuevo);

struct proyecto_t *proyecto_genera_plugins(const class CGenerators *generators, const char *nombre_proyecto, unsigned long *id_nuevo);

void proyecto_destruye(struct proyecto_t **proyecto);

struct proyecto_t *proyecto_copia(const struct proyecto_t *proyecto);

bool proyecto_es_vacio(const struct proyecto_t *proyecto);

bool proyecto_es_compilable_para_sistema_operatico(const struct proyecto_t *proyecto, enum gesenum_sistema_operativo_t sistema_operativo);

bool proyecto_es_libreria_dinamica(const struct proyecto_t *proyecto);

void proyecto_calcular_dependencias_ficheros(struct proyecto_t *proyecto);

bool proyecto_fichero_fuente_pertenece_a_proyecto(const struct proyecto_t *proyecto, const struct fichsrc_t *fichero);

void proyecto_debug(const struct proyecto_t *proyecto, struct marco_t *marco);

ArrString *proyecto_es_incluido(const struct proyecto_t *proyecto, const char *fichero);

bool proyecto_es_proyecto(const struct proyecto_t *proyecto, const char *nombre_proyecto);

bool proyecto_es_nombre_base_proyecto(const struct proyecto_t *proyecto, const char *nombre_base);

bool proyecto_mismo_nombre(const struct proyecto_t *proyecto1, const struct proyecto_t *proyecto2);

bool proyecto_fichero_pertenece_a_proyecto(const struct proyecto_t *proyecto, const char *nombre_fichero);

bool proyecto_existe_fichero(const struct proyecto_t *proyecto, const char *nombre_base, struct fichsrc_t **fichero);

ArrString *proyecto_obtener_nombres(const ArrTipo(proyecto_t) *proyectos);

ArrString *proyecto_ficheros_incluidos(const struct proyecto_t *proyecto);

void proyecto_obtener_ficheros_a_generar(
						const struct proyecto_t *proyecto,
						const class CGenerators *generators,
						const ArrTipo(fichsrc_t) *ficheros_a_generar);

void proyecto_obtener_ficheros_a_compilar(
						const struct proyecto_t *proyecto, 
						ArrTipo(proyecto_t) *proyectos, 
						ArrTipo(fichsrc_t) *ficheros_a_compilar,
						const struct compilador_t *compilador,
						const char *directorio_base,
	                    const char *subdirectorio_objs);

const ArrTipo(fichsrc_t) *proyecto_fuentes(const struct proyecto_t *proyecto);

void proyecto_integridad_sistema(
						const struct proyecto_t *proyecto, 
						const ArrTipo(proyecto_t) *proyectos, 
						ArrString *mensajes_error);

const char *proyecto_path_proyecto(const struct proyecto_t *proyecto);

char *proyecto_nombre_base(const struct proyecto_t *proyecto);

void proyecto_linkar_plugins(
                        const struct proyecto_t *proyecto,
                        const struct compilador_t *compilador,
                        const char *directorio_base,
                        const char *subdirectorio_plugins,
                        const char *subdirectorio_objs,
                        const char *subdirectorio_dlls,
                        const char *subdirectorio_errores,
                        const ArrTipo(fichsrc_t) *ficheros_necesarios,
                        const ArrString *includes_de_terceros,
                        const ArrString *dlls_nombre_base_necesarias,
                        struct marco_t *marco);

void proyecto_linkado_dinamico(
						const struct proyecto_t *proyecto,
						const struct compilador_t *compilador,
						const char *directorio_base,
                        const char *subdirectorio_objs,
                        const char *subdirectorio_dlls,
						struct marco_t *marco);

void proyecto_append_ejecutables_aplicacion(
						const struct proyecto_t *aplicacion,
						const ArrTipo(fichsrc_t) *ficheros_ejecutables);
