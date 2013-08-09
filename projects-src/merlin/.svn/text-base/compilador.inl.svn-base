// Definici�n del compilador

struct compilador_t *compilador_crea_rel(const char *directorio_inicio, const char *ext_exes, const char *ext_dlls, const char *prefijo_sistema_operativo);

struct compilador_t *compilador_crea_deb(const char *directorio_inicio, const char *ext_exes, const char *ext_dlls, const char *prefijo_sistema_operativo);

struct compilador_t *compilador_crea_prof(const char *directorio_inicio, const char *ext_exes, const char *ext_dlls, const char *prefijo_sistema_operativo);

struct compilador_t *compilador_crea_lin(const char *directorio_inicio, const char *ext_exes, const char *ext_dlls, const char *prefijo_sistema_operativo);

void compilador_destruye(struct compilador_t **compilador);

void compilador_compilar(
					const struct compilador_t *compilador,
					struct marco_t *marco,
					struct fichsrc_t *fichero,
					const ArrTipo(fichsrc_t) *ficheros_proyecto_include,
					const ArrString *includes_necesarios,
					const ArrString *includes_de_terceros,
					const char *directorio_base,
					const char *subdirectorios_objs,
					const char *subdirectorios_errores);

char *compilador_lineCommandMake(const struct compilador_t *compilador, const ArrString *includes_de_terceros, const ArrString *ficheros_include);

bool compilador_es_compilable(
					const struct compilador_t *compilador, 
					const struct fichsrc_t *fichero,
					const char *directorio_base,
					const char *subdirectorio_objs);

bool compilador_es_compilable_fichero_incluido(
					const struct compilador_t *compilador,
					const struct fichsrc_t *fichero,
					const struct fichsrc_t *fichero_incluido,
					const char *directorio_base,
					const char *subdirectorio_objs);

char *compilador_nombre_ejecutable(
					const struct compilador_t *compilador,
					const struct fichsrc_t *ejecutable);

char *compilador_nombre_obj(
					const struct compilador_t *compilador,
					const struct fichsrc_t *fichero_fuente);

const char *compilador_nombre_compilador(const struct compilador_t *compilador);

const char *compilador_nombre_linker(const struct compilador_t *compilador);

void compilador_linkar(
					const struct compilador_t *compilador,
					const struct fichsrc_t *ejecutable,
					const char *directorio_base,
					const char *directorio_salida_ejecutables,
                    const char *subdirectorio_objs,
                    const char *subdirectorio_dlls,
					const ArrTipo(fichsrc_t) *ficheros_necesarios,
					const ArrString *includes_de_terceros,
					const ArrString *dlls_nombre_base_necesarias,
					struct marco_t *marco);

char *compilador_lineCommandLinkerMake(
					const struct compilador_t *compilador,
					const ArrString *includes_de_terceros);

void compilador_linkado_plugins(
                    const struct compilador_t *compilador,
                    const char *nombre_proyecto,
                    const char *directorio_base,
                    const char *subdirectorio_plugins,
                    const char *subdirectorio_objs,
                    const char *subdirectorio_dlls,
                    const char *subdirectorio_errores,
                    const ArrTipo(fichsrc_t) *ficheros_del_proyecto,
                    const ArrString *includes_de_terceros,
                    const ArrString *dlls_nombre_base_necesarias,
                    struct marco_t *marco);

void compilador_linkado_dinamico(
					const struct compilador_t *compilador, 
					const char *nombre_proyecto,
					const char *directorio_base,
                    const char *subdirectorio_objs,
                    const char *subdirectorio_dlls,
					const ArrTipo(fichsrc_t) *ficheros_del_proyecto,
					struct marco_t *marco);

void compilador_clean(
					const struct compilador_t *compilador,
					const char *directorio_base,
	                const char *subdirectorio_objs,
	                const char *subdirectorio_dlls,
	                const char *subdirectorio_bin_projects,
	                const char *subdirectorio_errores,
					struct marco_t *marco);
