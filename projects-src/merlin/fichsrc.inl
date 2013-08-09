// Definición de fichero fuente.

struct fichsrc_t *fichsrc_crea_defecto(const class CGenerators *generadores, bool esta_en_libreria_dinamica, const char *nombre_fichero, unsigned long *id_nuevo);

void fichsrc_destruye(struct fichsrc_t **fichero);

struct fichsrc_t *fichsrc_copia(const struct fichsrc_t *fichero);

bool fichsrc_es_fuente(const struct fichsrc_t *fichero);

bool fichsrc_tiene_obj(const struct fichsrc_t *fichero);

void fichsrc_calcular_dependencias(struct fichsrc_t *fichero);

void fichsrc_debug(const struct fichsrc_t *fichero, struct marco_t *marco);

bool fichsrc_es_incluido_fichero(const struct fichsrc_t *fichero_proyecto, const char *fichero, char **mensaje);

bool fichsrc_es_incluido(const struct fichsrc_t *fichero, const struct fichsrc_t *fichero_incluido);

bool fichsrc_es_fichero(const struct fichsrc_t *fichero_proyecto, const char *nombre_fichero);

bool fichsrc_es_igual(const struct fichsrc_t *fichero_1, const struct fichsrc_t *fichero_2);

void fichsrc_anyadir_ficheros_que_incluye(const struct fichsrc_t *fichero, ArrString *ficheros_incluidos);

bool fichsrc_es_compilable(const struct fichsrc_t *fichero, const char *dirs_objs, const char *ext_objs);

bool fichsrc_es_generable(const struct fichsrc_t *fichero, const class CGenerators *generators);

bool fichsrc_es_compilable_fichero_incluido(
				const struct fichsrc_t *fichero,
				const struct fichsrc_t *fichero_include,
				const char *dirs_objs,
				const char *ext_objs);

void fichsrc_generar(
				const struct fichsrc_t *fichero,
				const class CGenerators *generators,
				struct marco_t *marco);

void fichsrc_compilar(
				struct fichsrc_t *fichero,
				struct marco_t *marco,
				const ArrTipo(fichsrc_t) *ficheros_proyecto_include,
				unsigned long nivel_salida,
				const char *compilador,
				const char *flags_objs,
				const char *flags_compilador,
				const char *flag_include,
				const ArrString *includes_necesarios,
				const char *directorio_err,
				const char *directorio_objs,
				const char *ext_objs,
				const ArrString *flags_adicionales);

const ArrString *fichsrc_incluidos(const struct fichsrc_t *fichero);

char *fichsrc_nombre_ejecutable(const struct fichsrc_t *ejecutable, const char *ext_exes);

char *fichsrc_nombre_obj(const struct fichsrc_t *fichero_fuente, const char *ext_objs);

char *fichsrc_nombre_base_con_extension(const struct fichsrc_t *fichero);

char *fichsrc_nombre_base(const struct fichsrc_t *fichero);

void fichsrc_linkar(
				const struct fichsrc_t *ejecutable,
				const ArrTipo(fichsrc_t) *ficheros_necesarios,
				const char *directorio_objs,
				const char *exts_objs,
				const char *directorio_exes,
				const char *directorio_dlls,
				const char *ext_exes,
				const char *linker,
				const char *flags_linker,
				const ArrString *flags_adicionales,
				const ArrString *dlls_flags,
				const ArrString *dlls_path,
				struct marco_t *marco);

void fichsrc_linkar_plugins(
                const ArrTipo(fichsrc_t) *ficheros_necesarios,
                const char *nombre_proyecto,
                const char *exts_objs,
                const char *directorio_objs,
                const char *directorio_plugins,
                const char *directorio_errores,
                const char *ext_dlls,
                const char *linker,
                const char *flags_linker,
                const ArrString *flags_adicionales,
                const ArrString *dlls_flags,
                const ArrString *dlls_path,
                struct marco_t *marco);

void fichsrc_linkar_dll(
                const ArrTipo(fichsrc_t) *ficheros_necesarios,
                const char *nombre_proyecto,
                const char *exts_objs,
                const char *directorio_objs,
                const char *directorio_plugins,
                const char *ext_dlls,
                const char *linker,
                const char *flags_linker,
                struct marco_t *marco);

bool fichsrc_es_include(const struct fichsrc_t *fichero_base, const class CGenerators *generators, const char *include);

bool fichsrc_es_cabecera(const struct fichsrc_t *fichero);

bool fichsrc_integridad_sistema(
				const struct fichsrc_t *fichero,
				const struct fichsrc_t *fichero_donde_se_incluye,
				char **mensaje_error);

bool fichscr_es_cpp(const struct fichsrc_t *fichero);

char *fichsrc_obtener_fuente(const char *directorio, const char *include);

bool fichsrc_es_include_de_fichero(const struct fichsrc_t *fichero, const struct fichsrc_t *fichero_es_include);

void fichsrc_copia_a_directorio(const struct fichsrc_t *fichero_src, const char *directorio_fuentes_make_source, struct marco_t *marco);


