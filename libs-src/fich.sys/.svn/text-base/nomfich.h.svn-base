// Operaciones con paths

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

bool nomfich_existe_fichero(const char *nomFich);

bool nomfich_existe_directorio(const char *nomDirectorio);

bool nomfich_es_fichero_ejecutable(const char *nomFich);
	
bool nomfich_es_directorio(const char *nomFichODirectorio);

bool nomfich_isSameExtension(const char *file, const char *ext);

bool nomfich_es_mas_moderno(const char *nombre_1, const char *nombre_2);

void nomfich_renombrar_fichero(const char *nomFich, const char *nuevoNombreFich);
	
char *nomfich_extension(const char *path);
	
char *nomfich_append_a_directorio(const char *directorio, const char *fichero_o_directorio_a_anyadir);

char *nomfich_crea_nombre_fichero_con_directorio_nombre_base_extension(const char *directorio, const char *nombre_base, const char *extension);

char *nomfich_cambia_extension(const char *path, const char *nueva_extension);

char *nomfich_nombre_base(const char *path);

char *nomfich_nombre_sin_extension(const char *path);

char *nomfich_nombre_base_con_extension(const char *path);

char *nomfich_directorio(const char *path);

time_t nomfich_fecha_modificacion(const char *path);

void nomfich_crear_directorios_si_no_existen(const char *directorio);

void nomfich_borrar_fichero(const char *path);

void nomfich_borrar_directorio(const char *path);

void nomfich_copiar_ficheros(const char *path_origen, const char *path_destino);

#ifdef __cplusplus
}
#endif /* __cplusplus */
