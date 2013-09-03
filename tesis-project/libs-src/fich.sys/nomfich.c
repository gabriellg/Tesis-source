// Operaciones con paths

#include "nomfich.h"

#include "stdio.h"
#include "filebas.inl"

#include "asrtbas.h"
#include "memory.h"
#include "strbas.h"

#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>

// TODO: Revisar access para cygwin porque no funciona.

//-----------------------------------------------------------------------

static bool i_es_barra_directorio(char caracter)
{
    if (caracter == '/' || caracter == '\\')
        return true;
    else
        return false;
}

//-----------------------------------------------------------------------

static char *i_extraer_parte(const char *cadena, size_t inicio, size_t fin)
{
	assert_no_null(cadena);
	assert(fin > inicio);
	return strbas_copyNumberChar(&cadena[inicio], fin - inicio);
}

//-----------------------------------------------------------------------

static char *i_copia_cadena_con_null(const char *cadena)
{
	if (cadena != NULL)
		return strbas_copyString(cadena);
	else
		return strbas_copyString("");
}

//-----------------------------------------------------------------------

static void i_splitpath(
					const char *nombre_fichero,
					char **ppath,
					char **pnombre,
					char **pextension)
{
	size_t posicion, parte_fin;
	bool fin;
	unsigned long estado;
	char *path, *nombre, *extension;

	estado = 1;
	parte_fin = strbas_length(nombre_fichero);
	posicion = strbas_length(nombre_fichero);
	path = NULL;
	nombre = NULL;
	extension = NULL;

	if (posicion > 0)
		fin = false;
	else
		fin = true;

	while(fin == false)
	{
		posicion--;
		switch(estado)
		{
			case 1:

				if (posicion > 0 && nombre_fichero[posicion] == '.'
						&& i_es_barra_directorio(nombre_fichero[posicion-1]) == false)
				{
					estado = 2;
					extension = i_extraer_parte(nombre_fichero, posicion + 1, parte_fin);
					parte_fin = posicion;
				}
				else if (i_es_barra_directorio(nombre_fichero[posicion]) == true)
				{
					fin = true;
				}
				break;

			case 2:
			{
				if (i_es_barra_directorio(nombre_fichero[posicion]) == true)
					fin = true;
				break;
			}
			default_error();
		}

		if (posicion == 0)
			fin = true;

		if (fin == true || posicion == 0)
		{
			if (posicion == 0)
				nombre = i_extraer_parte(nombre_fichero, posicion, parte_fin);
			else
				nombre = i_extraer_parte(nombre_fichero, posicion + 1, parte_fin);

			path = strbas_copyNumberChar(nombre_fichero, posicion);
		}
	}

	if (ppath != NULL)
		*ppath = i_copia_cadena_con_null(path);

	if (pnombre != NULL)
		*pnombre = i_copia_cadena_con_null(nombre);

	if (pextension != NULL)
		*pextension = i_copia_cadena_con_null(extension);

	if (path != NULL)
		strbas_destroy(&path);

	if (nombre != NULL)
		strbas_destroy(&nombre);

	if (extension != NULL)
		strbas_destroy(&extension);
}

//-----------------------------------------------------------------------

bool nomfich_existe_fichero(const char *nomFich)
{
    FILE *f;

	assert_no_null(nomFich);

    f = fopen(nomFich, "rb");

	if (f != NULL)
    {
        fclose(f);
		return true;
    }
	else
		return false;
}

//-----------------------------------------------------------------------

bool nomfich_existe_directorio(const char *nomDirectorio)
{
	if (access(nomDirectorio, F_OK) == 0)
		return true;
	else
		return false;
}

//-----------------------------------------------------------------------

bool nomfich_es_fichero_ejecutable(const char *nomFich)
{
	assert_no_null(nomFich);

	if (access(nomFich, X_OK) == 0)
		return true;
	else
		return false;
}

//-----------------------------------------------------------------------

bool nomfich_es_directorio(const char *nomFichODirectorio)
{
	if (access(nomFichODirectorio, F_OK) == 0)
	{
		int res;
		struct stat estado_fichero;

		res = stat(nomFichODirectorio, &estado_fichero);
		assert(res == 0);

		if (S_ISDIR(estado_fichero.st_mode))
			return true;
		else
			return false;
	}
	else
		return false;
}

//---------------------------------------------------------

bool nomfich_isSameExtension(const char *file, const char *ext)
{
	bool isSameExtension;
	char *extFile;

	i_splitpath(file, NULL, NULL, &extFile);

	isSameExtension = strbas_equalsStringWithoutMatchCase(extFile, ext);

	strbas_destroy(&extFile);

	return isSameExtension;
}

//-----------------------------------------------------------------------

bool nomfich_es_mas_moderno(const char *nombre_1, const char *nombre_2)
{
	time_t tiempo_fichero_1, tiempo_fichero_2;

	tiempo_fichero_1 = nomfich_fecha_modificacion(nombre_1);
	tiempo_fichero_2 = nomfich_fecha_modificacion(nombre_2);

	if (tiempo_fichero_1 > tiempo_fichero_2)
		return true;
	else
		return false;
}

//-----------------------------------------------------------------------

void nomfich_renombrar_fichero(const char *nomFich, const char *nuevoNombreFich)
{
	int resp;

	assert_no_null(nomFich);
	assert_no_null(nuevoNombreFich);

	resp = rename(nomFich, nuevoNombreFich);
	assert(resp == 0);
}

//-----------------------------------------------------------------------

char *nomfich_extension(const char *path)
{
	char *extension_fichero;

	i_splitpath(path, NULL, NULL, &extension_fichero);

	return extension_fichero;
}

//-----------------------------------------------------------------------

static char *i_make_nombre_fichero(const char *nombre, const char *extension)
{
	assert(strbas_length(nombre) > 0);

	if (strbas_length(extension) > 0)
		return strbas_printf("%s.%s", nombre, extension);
	else
		return strbas_copyString(nombre);
}

//-----------------------------------------------------------------------

static char *i_makepath(const char *directorio, const char *nombre, const char *extension)
{
	char *nombre_con_extension, *path;

	nombre_con_extension = i_make_nombre_fichero(nombre, extension);

	if (strbas_length(directorio) > 0)
		path = strbas_printf("%s/%s", directorio, nombre_con_extension);
	else
		path = strbas_copyString(nombre_con_extension);

	strbas_destroy(&nombre_con_extension);

	return path;
}

//-----------------------------------------------------------------------

char *nomfich_append_a_directorio(
					const char *directorio,
					const char *fichero_o_directorio_a_anyadir)
{
	size_t longitud;

	longitud = strbas_length(directorio);
	assert(longitud > 0);
    assert(i_es_barra_directorio(directorio[longitud-1]) == false);

	return i_makepath(directorio, fichero_o_directorio_a_anyadir, "");
}

//-----------------------------------------------------------------------

char *nomfich_crea_nombre_fichero_con_directorio_nombre_base_extension(
                    const char *directorio,
                    const char *nombre_base,
                    const char *extension)
{
    size_t longitud;

    longitud = strbas_length(directorio);
    if (longitud > 0);
        assert(i_es_barra_directorio(directorio[longitud-1]) == false);

    return i_makepath(directorio, nombre_base, extension);
}


//-----------------------------------------------------------------------

char *nomfich_cambia_extension(
					const char *path,
					const char *nueva_extension)
{
	char *directorio, *nombre, *nuevo_path;

	i_splitpath(path, &directorio, &nombre, NULL);

	nuevo_path = i_makepath(directorio, nombre, nueva_extension);

	strbas_destroy(&directorio);
	strbas_destroy(&nombre);

	return nuevo_path;
}

//-----------------------------------------------------------------------

char *nomfich_nombre_base(const char *path)
{
	char *nombre;

	i_splitpath(path, NULL, &nombre, NULL);

	return nombre;
}

//-----------------------------------------------------------------------

char *nomfich_nombre_sin_extension(const char *path)
{
	char *directorio, *nombre, *nombre_sin_extension;

	i_splitpath(path, &directorio, &nombre, NULL);

	nombre_sin_extension = i_makepath(directorio, nombre, "");

	strbas_destroy(&directorio);
	strbas_destroy(&nombre);

	return nombre_sin_extension;
}

//-----------------------------------------------------------------------

char *nomfich_nombre_base_con_extension(const char *path)
{
	char *extension, *nombre, *nombre_fichero;;

	i_splitpath(path, NULL, &nombre, &extension);

	nombre_fichero = i_makepath("", nombre, extension);

	strbas_destroy(&extension);
	strbas_destroy(&nombre);

	return nombre_fichero;
}

//-----------------------------------------------------------------------

char *nomfich_directorio(const char *path)
{
	char *directorio;

	i_splitpath(path, &directorio, NULL, NULL);

	return directorio;
}

//-----------------------------------------------------------------------

time_t nomfich_fecha_modificacion(const char *path)
{
	struct stat informacion;
	int ret;

	ret = stat(path, &informacion);
	assert(ret == 0);

	return informacion.st_ctime;
}

//-----------------------------------------------------------------------

static bool i_es_directorio_raiz(const char *directorio)
{
    if (strbas_equalsStrings(directorio, "/") == true || strbas_equalsStrings(directorio, "\\") == true)
        return true;
    else
        return false;
}

//-----------------------------------------------------------------------

void nomfich_crear_directorios_si_no_existen(const char *directorio)
{
    if (i_es_directorio_raiz(directorio) == false && strbas_length(directorio) != 0)
	{
		char *directorio_siguiente;

		i_splitpath(directorio, &directorio_siguiente, NULL, NULL);

		nomfich_crear_directorios_si_no_existen(directorio_siguiente);

		if (access(directorio, F_OK) != 0)
		{
			int ret;

			ret = mkdir(directorio, S_IRWXU | S_IRWXG | S_IRWXO);
			assert(ret == 0);
		}
	}
}

//-----------------------------------------------------------------------

void nomfich_borrar_fichero(const char *path)
{
	int ret;

	assert_no_null(path);

    if (access(path, R_OK) == 0)
    {
	    ret = remove(path);
	    assert(ret == 0);
    }
}

//-----------------------------------------------------------------------

void nomfich_borrar_directorio(const char *path)
{
	int ret;

	assert_no_null(path);
	ret = rmdir(path);
	assert(ret == 0);
}

//-----------------------------------------------------------------------

void nomfich_copiar_ficheros(const char *path_origen, const char *path_destino)
{
	char buffer[1024];
	FILE *file_origen, *file_destino;

	file_origen = filebas_fopen(path_origen, "rb");
	file_destino = filebas_fopen(path_destino, "wb");

	while(feof(file_origen) == 0)
	{
		size_t byteRead;

		byteRead = fread(buffer, sizeof(char), 1024, file_origen);
		fwrite(buffer, sizeof(char), byteRead, file_destino);
	}

	filebas_fclose(&file_origen);
	filebas_fclose(&file_destino);
}
