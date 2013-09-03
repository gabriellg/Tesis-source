// Lectura de ficheros y directorios de un directorio.
#include "objsdir.h"

#include "asrtbas.h"
#include "memory.h"
#include "arrcad.h"
#include "nomfich.h"
#include "strbas.h"

#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <dirent.h>

struct objsdir_t
{
	char *path_directorio;
	
	ArrString *ficheros;
	ArrString *directorios;
};

//-----------------------------------------------------------------------

static void i_integridad(const struct objsdir_t *objetos_directorio)
{
	assert_no_null(objetos_directorio);
	assert_no_null(objetos_directorio->ficheros);
	assert_no_null(objetos_directorio->directorios);
}

//-----------------------------------------------------------------------

static struct objsdir_t *i_crea(
							const char *path_directorio, 
							const ArrString *ficheros,
							const ArrString *directorios)
{
	struct objsdir_t *objetos_directorio;
		
	objetos_directorio = MALLOC(struct objsdir_t);
		
	objetos_directorio->path_directorio = strbas_copyString(path_directorio);
	objetos_directorio->ficheros = arrcad_Copia(ficheros);
	objetos_directorio->directorios = arrcad_Copia(directorios);
	
	i_integridad(objetos_directorio);
	
	return objetos_directorio;
}

//-----------------------------------------------------------------------

static void i_obtener_fichero_y_directorios(
							const char *path,
							ArrString **ficheros,
							ArrString **directorios)
{
	DIR *directorio;
	bool salir;
	ArrString *ficheros_loc;
	ArrString *directorios_loc;
	int ret;
	
	assert_no_null(path);
	assert_no_null(ficheros);
	assert_no_null(directorios);

	ficheros_loc = arrcad_CreaString(0);
	directorios_loc = arrcad_CreaString(0);
	
	directorio = opendir(path);
	assert_no_null(directorio);
	
	do
	{
		struct dirent *entrada;
		
		entrada = readdir(directorio);
		if (entrada != NULL)
		{
			char *path_fichero;
		
			path_fichero = nomfich_append_a_directorio(path, entrada->d_name);
		
			if (nomfich_es_directorio(path_fichero) == false)
				arrcad_Anyadir(ficheros_loc, entrada->d_name);
			else
			{
				if (strbas_equalsStrings(entrada->d_name, ".") == false &&
						strbas_equalsStrings(entrada->d_name, "..") == false)
					arrcad_Anyadir(directorios_loc, entrada->d_name);
			}
			
			strbas_destroy(&path_fichero);
			
			salir = false;
		}
		else
			salir = true;
	}
	while(salir == false);
		
	ret = closedir(directorio);
	assert(ret == 0);
	
	*ficheros = ficheros_loc;
	*directorios = directorios_loc;
}
							
//-----------------------------------------------------------------------

struct objsdir_t *objsdir_lee_path(const char *path)
{
	struct objsdir_t *objetos_directorio;
	
	ArrString *ficheros;
	ArrString *directorios;
	
	assert_no_null(path);

	i_obtener_fichero_y_directorios(path, &ficheros, &directorios);
	
	objetos_directorio = i_crea(path, ficheros, directorios);
	
	arrcad_Destruye(&ficheros);
	arrcad_Destruye(&directorios);
	
	return objetos_directorio;
}

//-----------------------------------------------------------------------

void objsdir_destruye(struct objsdir_t **objetos_directorio)
{
	assert_no_null(objetos_directorio);
	i_integridad(*objetos_directorio);

	strbas_destroy(&(*objetos_directorio)->path_directorio);
	arrcad_Destruye(&(*objetos_directorio)->ficheros);
	arrcad_Destruye(&(*objetos_directorio)->directorios);

	FREE_T(objetos_directorio, struct objsdir_t);
}

//-----------------------------------------------------------------------

bool objsdir_es_directorio_vacio(const char *path)
{
	unsigned long num;
	ArrString *ficheros, *directorios;

	i_obtener_fichero_y_directorios(path, &ficheros, &directorios);

	num = arrcad_numElementos(ficheros);
	num += arrcad_numElementos(directorios);

	arrcad_Destruye(&ficheros);
	arrcad_Destruye(&directorios);

	if (num == 0)
		return true;
	else
		return false;
}

//-----------------------------------------------------------------------

bool objsdir_hay_ficheros(const struct objsdir_t *objetos_directorio)
{
	unsigned long num_ficheros;

	num_ficheros = arrcad_numElementos(objetos_directorio->ficheros);

	if (num_ficheros > 0)
		return true;
	else
		return false;
}

//-----------------------------------------------------------------------

ArrString *objsdir_obtener_ficheros(const struct objsdir_t *objetos_directorio)
{
	ArrString *path_ficheros;
	unsigned long num_ficheros, i;
		
	i_integridad(objetos_directorio);
	
	num_ficheros = arrcad_numElementos(objetos_directorio->ficheros);
	path_ficheros = arrcad_CreaString(num_ficheros);
	
	for (i = 0; i < num_ficheros; i++)
	{
		const char *fichero;
		char *path_fichero;
		
		fichero = arrcad_Get(objetos_directorio->ficheros, i);
			
		path_fichero = nomfich_append_a_directorio(objetos_directorio->path_directorio, fichero);
		arrcad_Set(path_ficheros, i, path_fichero);
		strbas_destroy(&path_fichero);
	}

	return path_ficheros;
}

//-----------------------------------------------------------------------

ArrString *objsdir_obtener_directorios(const struct objsdir_t *objetos_directorio)
{
	ArrString *path_directorios;
	unsigned long num_directorios, i;
		
	i_integridad(objetos_directorio);
	
	num_directorios = arrcad_numElementos(objetos_directorio->directorios);
	path_directorios = arrcad_CreaString(num_directorios);
	
	for (i = 0; i < num_directorios; i++)
	{
		const char *directorio;
		char *path_directorio;
		
		directorio = arrcad_Get(objetos_directorio->directorios, i);
		
		path_directorio = nomfich_append_a_directorio(objetos_directorio->path_directorio, directorio);
		arrcad_Set(path_directorios, i, path_directorio);
		strbas_destroy(&path_directorio);
	}

	return path_directorios;
}
