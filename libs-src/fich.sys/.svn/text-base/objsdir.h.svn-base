// Lectura de ficheros y directorios de un directorio.

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct objsdir_t *objsdir_lee_path(const char *path);

void objsdir_destruye(struct objsdir_t **objetos_directorio);

bool objsdir_es_directorio_vacio(const char *path);

bool objsdir_hay_ficheros(const struct objsdir_t *objetos_directorio);

ArrString *objsdir_obtener_ficheros(const struct objsdir_t *objetos_directorio);

ArrString *objsdir_obtener_directorios(const struct objsdir_t *objetos_directorios);

#ifdef __cplusplus
}
#endif /* __cplusplus */
