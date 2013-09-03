//genmak: Generación de .mak para proyectos de merlin.

struct genmak_t *genmak_createDefault(const char *nameProject, const ArrTipo(fichsrc_t) *ficheros_proyecto_include);

void genmak_destroy(struct genmak_t **generator_mak);

void genmak_appendProgram(
					struct genmak_t *generatorMak,
					const struct fichsrc_t *fileMain,
					const ArrTipo(fichsrc_t) **filesSource,
					const ArrTipo(fichsrc_t) **filesIncludes,
					ArrString **includes_de_terceros);

void genmak_copyFiles(
					const struct genmak_t *generatorMak,
					const char *dirMake,
					struct marco_t *marco);

void genmak_generateMak(
					const struct genmak_t *generatorMak,
					const struct compilador_t *compilador,
					const char *dirMake);


