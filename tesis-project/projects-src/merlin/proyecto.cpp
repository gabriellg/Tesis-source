// Definici�n del proyecto.

#include "proyecto.inl"

#include "fichsrc.inl"
#include "compilador.inl"
#include "gesenum.inl"

#include "array.h"
#include "asrtbas.h"
#include "arrcad.h"
#include "memory.h"
#include "objsdir.h"
#include "strbas.h"
#include "identif.h"
#include "nomfich.h"
#include "marco.h"

enum i_tipo_proyecto_t
{
	i_LIBRERIA_ESTATICA_O_EJECUTABLE,
	i_LIBRERIA_DINAMICA,
	i_PLUGINS
};

struct proyecto_t
{
	enum i_tipo_proyecto_t tipo_proyecto;

	unsigned long id;

	char *nombre_proyecto;
	ArrTipo(fichsrc_t) *ficheros_del_proyecto;
};

//-----------------------------------------------------------------------

static void i_integridad(const struct proyecto_t *proyecto)
{
	assert_no_null(proyecto);
	assert_no_null(proyecto->nombre_proyecto);
	assert_no_null(proyecto->ficheros_del_proyecto);
}

//-----------------------------------------------------------------------

static struct proyecto_t *i_crea(
							enum i_tipo_proyecto_t tipo_proyecto,
							unsigned long id,
							const char *nombre_proyecto,
							const ArrTipo(fichsrc_t) *ficheros_del_proyecto)
{
	struct proyecto_t *proyecto;

	proyecto = MALLOC(struct proyecto_t);

	proyecto->tipo_proyecto = tipo_proyecto;

	proyecto->id = id;
	proyecto->nombre_proyecto = strbas_copyString(nombre_proyecto);
	proyecto->ficheros_del_proyecto = array_Copia(fichsrc_t, ficheros_del_proyecto, fichsrc_copia);

	i_integridad(proyecto);

	return proyecto;
}

//-----------------------------------------------------------------------

static bool i_esta_en_libreria_dinamica(enum i_tipo_proyecto_t tipo_proyecto)
{
	bool esta_en_libreria_dinamica;

	switch(tipo_proyecto)
	{
		case i_LIBRERIA_DINAMICA:
		case i_PLUGINS:

			esta_en_libreria_dinamica = true;
			break;

		case i_LIBRERIA_ESTATICA_O_EJECUTABLE:

			esta_en_libreria_dinamica = false;
			break;

		default_error();
	}

	return esta_en_libreria_dinamica;
}

//-----------------------------------------------------------------------

static ArrTipo(fichsrc_t) *i_obtener_ficheros_de_proyecto(
							const class CGenerators *generators,
							enum i_tipo_proyecto_t tipo_proyecto, 
							const char *nombre_proyecto, 
							unsigned long *id_nuevo)
{
	ArrTipo(fichsrc_t) *ficheros_fuentes;
	struct objsdir_t *objetos_directorio;
	ArrString *path_ficheros;
	unsigned long num_ficheros, i;
	bool esta_en_libraria_dinamica;

	objetos_directorio = objsdir_lee_path(nombre_proyecto);
	path_ficheros = objsdir_obtener_ficheros(objetos_directorio);
	objsdir_destruye(&objetos_directorio);

	num_ficheros = arrcad_numElementos(path_ficheros);
	ficheros_fuentes = array_CreaPuntero(0, fichsrc_t);
	esta_en_libraria_dinamica = i_esta_en_libreria_dinamica(tipo_proyecto);

	for (i = 0; i < num_ficheros; i++)
	{
		struct fichsrc_t *fichero;
		const char *path_fichero;

		path_fichero = arrcad_Get(path_ficheros, i);

		fichero = fichsrc_crea_defecto(generators, esta_en_libraria_dinamica, path_fichero, id_nuevo);

		if (fichsrc_es_fuente(fichero) == true)
			array_Anyadir(fichsrc_t, ficheros_fuentes, fichero);
		else
			fichsrc_destruye(&fichero);
	}

	arrcad_Destruye(&path_ficheros);

	return ficheros_fuentes;
}

//-----------------------------------------------------------------------

static enum i_tipo_proyecto_t i_tipo_proyecto(const char *nombre_proyecto)
{
	enum i_tipo_proyecto_t tipo_proyecto;
	char *extension;

	extension = nomfich_extension(nombre_proyecto);

	if (strbas_equalsStringWithoutMatchCase(extension,"din") == true)
		tipo_proyecto = i_LIBRERIA_DINAMICA;
	else
		tipo_proyecto = i_LIBRERIA_ESTATICA_O_EJECUTABLE;

	strbas_destroy(&extension);

	return tipo_proyecto;
}

//-----------------------------------------------------------------------

static struct proyecto_t *i_genera_tipo_proyecto(
                            enum i_tipo_proyecto_t tipo_proyecto,
                            const class CGenerators *generators,
                            const char *nombre_proyecto,
                            unsigned long *id_nuevo)
{
    struct proyecto_t *proyecto;
    ArrTipo(fichsrc_t) *ficheros_del_proyecto;
    unsigned long id;

    ficheros_del_proyecto = i_obtener_ficheros_de_proyecto(generators, tipo_proyecto, nombre_proyecto, id_nuevo);
    id = identif_nuevo_identificador(id_nuevo);

    proyecto = i_crea(tipo_proyecto, id, nombre_proyecto, ficheros_del_proyecto);

    array_Destruye(fichsrc_t, &ficheros_del_proyecto, fichsrc_destruye);

    return proyecto;
}

//-----------------------------------------------------------------------

struct proyecto_t *proyecto_genera_proyecto(const class CGenerators *generators, const char *nombre_proyecto, unsigned long *id_nuevo)
{
	enum i_tipo_proyecto_t tipo_proyecto;

	tipo_proyecto = i_tipo_proyecto(nombre_proyecto);
	return i_genera_tipo_proyecto(tipo_proyecto, generators, nombre_proyecto, id_nuevo);
}

//-----------------------------------------------------------------------

struct proyecto_t *proyecto_genera_plugins(const class CGenerators *generators, const char *nombre_proyecto, unsigned long *id_nuevo)
{
    enum i_tipo_proyecto_t tipo_proyecto;

    tipo_proyecto = i_PLUGINS;
    return i_genera_tipo_proyecto(tipo_proyecto, generators, nombre_proyecto, id_nuevo);
}

//-----------------------------------------------------------------------

void proyecto_destruye(struct proyecto_t **proyecto)
{
	assert_no_null(proyecto),
	i_integridad(*proyecto);

	strbas_destroy(&(*proyecto)->nombre_proyecto);
	array_Destruye(fichsrc_t, &(*proyecto)->ficheros_del_proyecto, fichsrc_destruye);

	FREE_T(proyecto, struct proyecto_t);
}

//-----------------------------------------------------------------------

struct proyecto_t *proyecto_copia(const struct proyecto_t *proyecto)
{
	assert_no_null(proyecto);
	return i_crea(proyecto->tipo_proyecto, proyecto->id, proyecto->nombre_proyecto, proyecto->ficheros_del_proyecto);
}

//-----------------------------------------------------------------------

bool proyecto_es_vacio(const struct proyecto_t *proyecto)
{
	i_integridad(proyecto);

	if (array_numElementos(fichsrc_t, proyecto->ficheros_del_proyecto) > 0)
		return false;
	else
		return true;
}

//-----------------------------------------------------------------------

bool proyecto_es_compilable_para_sistema_operatico(
                        const struct proyecto_t *proyecto,
                        enum gesenum_sistema_operativo_t sistema_operativo)
{
    enum gesenum_sistema_operativo_t sistema_operativo_proyecto;

    i_integridad(proyecto);

    if (gesenum_es_dependiente_sistema_operativo(
                        proyecto->nombre_proyecto,
                        &sistema_operativo_proyecto) == true)
    {
        if (sistema_operativo_proyecto == sistema_operativo)
            return true;
        else
            return false;
    }
    else
        return true;
}

//-----------------------------------------------------------------------

bool proyecto_es_libreria_dinamica(const struct proyecto_t *proyecto)
{
	i_integridad(proyecto);
	return i_esta_en_libreria_dinamica(proyecto->tipo_proyecto);
}

//-----------------------------------------------------------------------

void proyecto_calcular_dependencias_ficheros(struct proyecto_t *proyecto)
{
	unsigned long i, num_ficheros;

	i_integridad(proyecto);

	num_ficheros = array_numElementos(fichsrc_t, proyecto->ficheros_del_proyecto);

	for (i = 0; i < num_ficheros; i++)
	{
		struct fichsrc_t *fichero;

		fichero = array_Get(fichsrc_t, proyecto->ficheros_del_proyecto, i);
		fichsrc_calcular_dependencias(fichero);
	}
}

//-----------------------------------------------------------------------

bool proyecto_fichero_fuente_pertenece_a_proyecto(const struct proyecto_t *proyecto, const struct fichsrc_t *fichero)
{
	return array_BuscarUnica(fichsrc_t, proyecto->ficheros_del_proyecto, struct fichsrc_t, fichero, NULL, NULL);
}

//-----------------------------------------------------------------------

void proyecto_debug(const struct proyecto_t *proyecto, struct marco_t *marco)
{
	unsigned long i, num_ficheros;
	char *mensaje;

	i_integridad(proyecto);

	mensaje = strbas_printf("---->%s\n", proyecto->nombre_proyecto);
	marco_lanza_mensaje(marco, MARCO_MENSAJE_INFORMACION, mensaje);
	strbas_destroy(&mensaje);

	num_ficheros = array_numElementos(fichsrc_t, proyecto->ficheros_del_proyecto);

	for (i = 0; i < num_ficheros; i++)
	{
		const struct fichsrc_t *fichero;

		fichero = array_Get(fichsrc_t, proyecto->ficheros_del_proyecto, i);

		fichsrc_debug(fichero, marco);
	}
}

//-----------------------------------------------------------------------

static void i_obtener_ficheros_es_incluido_de_proyecto(
						const struct proyecto_t *proyecto,
						const struct fichsrc_t *fichero,
						ArrTipo(fichsrc_t) *ficheros_donde_es_incluido)
{
	unsigned long num_ficheros, i;

	num_ficheros = array_numElementos(fichsrc_t, proyecto->ficheros_del_proyecto);

	for (i = 0; i < num_ficheros; i++)
	{
		struct fichsrc_t *fichero_proyecto;

		fichero_proyecto = array_Get(fichsrc_t, proyecto->ficheros_del_proyecto, i);

		if (fichsrc_es_incluido(fichero_proyecto, fichero) == true)
			array_Anyadir(fichsrc_t, ficheros_donde_es_incluido, fichero_proyecto);
	}
}

//-----------------------------------------------------------------------

static ArrTipo(fichsrc_t) *i_obtener_ficheros_es_incluido(
						const ArrTipo(proyecto_t) *proyectos,
						const struct fichsrc_t *fichero)
{
	ArrTipo(fichsrc_t) *ficheros_donde_es_incluido;
	unsigned long num_proyectos, i;

	ficheros_donde_es_incluido = array_CreaPuntero(0, fichsrc_t);

	num_proyectos = array_numElementos(proyecto_t, proyectos);

	for (i = 0; i < num_proyectos; i++)
	{
		struct proyecto_t *proyecto;

		proyecto = array_Get(proyecto_t, proyectos, i);

		i_obtener_ficheros_es_incluido_de_proyecto(proyecto, fichero, ficheros_donde_es_incluido);
	}

	return ficheros_donde_es_incluido;
}

//-----------------------------------------------------------------------

ArrString *proyecto_es_incluido(const struct proyecto_t *proyecto, const char *fichero)
{
	unsigned long i, num_ficheros;
	ArrString *mensajes;

	i_integridad(proyecto);

	mensajes = arrcad_CreaString(0);

	num_ficheros = array_numElementos(fichsrc_t, proyecto->ficheros_del_proyecto);

	for (i = 0; i < num_ficheros; i++)
	{
		const struct fichsrc_t *fichero_proyecto;
		char *mensaje;

		fichero_proyecto = array_Get(fichsrc_t, proyecto->ficheros_del_proyecto, i);

		mensaje = NULL;
		if (fichsrc_es_incluido_fichero(fichero_proyecto, fichero, &mensaje) == true)
		{
			arrcad_Anyadir(mensajes, mensaje);
			strbas_destroy(&mensaje);
		}
	}

	return mensajes;
}

//-----------------------------------------------------------------------

bool proyecto_mismo_nombre(const struct proyecto_t *proyecto1, const struct proyecto_t *proyecto2)
{
	i_integridad(proyecto1);
	i_integridad(proyecto2);

	return strbas_equalsStrings(proyecto1->nombre_proyecto, proyecto2->nombre_proyecto);
}

//-----------------------------------------------------------------------

bool proyecto_es_proyecto(const struct proyecto_t *proyecto, const char *nombre_proyecto)
{
	i_integridad(proyecto);

	return strbas_equalsStrings(proyecto->nombre_proyecto, nombre_proyecto);
}

//-----------------------------------------------------------------------

bool proyecto_es_nombre_base_proyecto(const struct proyecto_t *proyecto, const char *nombre_base)
{
	bool es_nombre_base_proyecto;
	char *nombre_base_proyecto;

	i_integridad(proyecto);

	nombre_base_proyecto = nomfich_nombre_base(proyecto->nombre_proyecto);
	es_nombre_base_proyecto = strbas_equalsStringWithoutMatchCase(nombre_base_proyecto, nombre_base);
	strbas_destroy(&nombre_base_proyecto);

	return es_nombre_base_proyecto;
}

//-----------------------------------------------------------------------

bool proyecto_fichero_pertenece_a_proyecto(const struct proyecto_t *proyecto, const char *nombre_fichero)
{
	i_integridad(proyecto);

	return array_BuscarUnica(fichsrc_t, proyecto->ficheros_del_proyecto, char, nombre_fichero, fichsrc_es_fichero, NULL);
}

//-----------------------------------------------------------------------

bool proyecto_existe_fichero(
							const struct proyecto_t *proyecto, 
							const char *nombre_base, 
							struct fichsrc_t **fichero)
{
	bool existe_fichero;
	unsigned long ind_pos;

	i_integridad(proyecto);

	existe_fichero = array_BuscarUnica(
				fichsrc_t,
				proyecto->ficheros_del_proyecto,
				char, nombre_base,
				fichsrc_es_fichero, &ind_pos);
	
	if (existe_fichero == true && fichero != NULL)
		*fichero = array_Get(fichsrc_t, proyecto->ficheros_del_proyecto, ind_pos);

	return existe_fichero;
}

//-----------------------------------------------------------------------

ArrString *proyecto_obtener_nombres(const ArrTipo(proyecto_t) *proyectos)
{
	ArrString *nombres_proyectos;
	unsigned long num_proyectos, i;

	num_proyectos = array_numElementos(proyecto_t, proyectos);

	nombres_proyectos = arrcad_CreaString(num_proyectos);

	for (i = 0; i < num_proyectos; i++)
	{
		const struct proyecto_t *proyecto;

		proyecto = array_Get(proyecto_t, proyectos, i);
		assert_no_null(proyecto);
		arrcad_Set(nombres_proyectos, i, proyecto->nombre_proyecto);
	}

	return nombres_proyectos;
}

//-----------------------------------------------------------------------

ArrString *proyecto_ficheros_incluidos(const struct proyecto_t *proyecto)
{
	ArrString *ficheros_incluidos;
	unsigned long num_ficheros_en_proyecto, i;

	i_integridad(proyecto);

	ficheros_incluidos = arrcad_CreaString(0);
	num_ficheros_en_proyecto = array_numElementos(fichsrc_t, proyecto->ficheros_del_proyecto);

	for (i = 0; i < num_ficheros_en_proyecto; i++)
	{
		struct fichsrc_t *fichero;

		fichero = array_Get(fichsrc_t, proyecto->ficheros_del_proyecto, i);
		fichsrc_anyadir_ficheros_que_incluye(fichero, ficheros_incluidos);
	}

	return ficheros_incluidos;
}

//-----------------------------------------------------------------------

void proyecto_obtener_ficheros_a_generar(
						const struct proyecto_t *proyecto,
						const class CGenerators *generators,
						const ArrTipo(fichsrc_t) *ficheros_a_generar)
{
	unsigned long i, num_ficheros;

	i_integridad(proyecto);

	num_ficheros = array_numElementos(fichsrc_t, proyecto->ficheros_del_proyecto);

	for (i = 0; i < num_ficheros; i++)
	{
		struct fichsrc_t *fichero;

		fichero = array_Get(fichsrc_t, proyecto->ficheros_del_proyecto, i);

		if (fichsrc_es_generable(fichero, generators) == true)
			array_AnyadirConst(fichsrc_t, ficheros_a_generar, fichero);
	}
}

//-----------------------------------------------------------------------

static void i_ficheros_a_compilar_por_inclusion(
						const ArrTipo(fichsrc_t) *ficheros_donde_esta_incluido,
						const ArrTipo(proyecto_t) *proyectos,
						const struct fichsrc_t *fichero,
						ArrTipo(fichsrc_t) *ficheros_a_compilar,
						const struct compilador_t *compilador,
						const char *directorio_base,
						const char *subdirectorio_objs)
{
	unsigned long i, num_ficheros;

	num_ficheros = array_numElementos(fichsrc_t, ficheros_donde_esta_incluido);

	for (i = 0; i < num_ficheros; i++)
	{
		struct fichsrc_t *fichero_incluido;

		fichero_incluido = array_Get(fichsrc_t, ficheros_donde_esta_incluido, i);

		if (fichsrc_tiene_obj(fichero_incluido) == true)
		{
		    if (compilador_es_compilable_fichero_incluido(
		                        compilador,
		                        fichero_incluido, fichero,
		                        directorio_base, subdirectorio_objs) == true)
			{
				if (array_BuscarUnica(
								fichsrc_t, ficheros_a_compilar,
								struct fichsrc_t, fichero_incluido,
								fichsrc_es_igual, NULL) == false)
					array_Anyadir(fichsrc_t, ficheros_a_compilar, fichero_incluido);
			}
		}
		else
		{
			ArrTipo(fichsrc_t) *ficheros_donde_esta_incluido_2;

			ficheros_donde_esta_incluido_2 = i_obtener_ficheros_es_incluido(proyectos, fichero_incluido);

			i_ficheros_a_compilar_por_inclusion(
								ficheros_donde_esta_incluido_2,
								proyectos, fichero, ficheros_a_compilar,
								compilador, directorio_base, subdirectorio_objs);

			array_Destruye(fichsrc_t, &ficheros_donde_esta_incluido_2, NULL);
		}
	}
}

//-----------------------------------------------------------------------

void proyecto_obtener_ficheros_a_compilar(
						const struct proyecto_t *proyecto,
						ArrTipo(proyecto_t) *proyectos,
						ArrTipo(fichsrc_t) *ficheros_a_compilar,
						const struct compilador_t *compilador,
						const char *directorio_base,
						const char *subdirectorio_objs)
{
	unsigned long i, num_ficheros;

	i_integridad(proyecto);

	num_ficheros = array_numElementos(fichsrc_t, proyecto->ficheros_del_proyecto);

	for (i = 0; i < num_ficheros; i++)
	{
		struct fichsrc_t *fichero;

		fichero = array_Get(fichsrc_t, proyecto->ficheros_del_proyecto, i);

		if (compilador_es_compilable(compilador, fichero, directorio_base, subdirectorio_objs) == true)
		{
			if (array_BuscarUnica(
							fichsrc_t, ficheros_a_compilar,
							struct fichsrc_t, fichero,
							fichsrc_es_igual, NULL) == false)
				array_Anyadir(fichsrc_t, ficheros_a_compilar, fichero);
		}
		else if (fichsrc_es_cabecera(fichero) == true)
		{
			ArrTipo(fichsrc_t) *ficheros_donde_esta_incluido;

			ficheros_donde_esta_incluido = i_obtener_ficheros_es_incluido(proyectos, fichero);

			i_ficheros_a_compilar_por_inclusion(
							ficheros_donde_esta_incluido, proyectos,
							fichero, ficheros_a_compilar,
							compilador, directorio_base, subdirectorio_objs);

			array_Destruye(fichsrc_t, &ficheros_donde_esta_incluido, NULL);
		}
	}
}

//-----------------------------------------------------------------------

const ArrTipo(fichsrc_t) *proyecto_fuentes(const struct proyecto_t *proyecto)
{
	i_integridad(proyecto);

	return proyecto->ficheros_del_proyecto;
}

//-----------------------------------------------------------------------

static void i_integridad_sistema_donde_se_incluye(
						const struct fichsrc_t *fichero,
						const ArrTipo(fichsrc_t) *ficheros_donde_se_incluye,
						ArrString *mensajes_error)
{
	unsigned long i, num_ficheros;

	num_ficheros = array_numElementos(fichsrc_t, ficheros_donde_se_incluye);

	for (i = 0; i < num_ficheros; i++)
	{
		const struct fichsrc_t *fichero_donde_se_incluye;
		char *mensaje_error;

		fichero_donde_se_incluye = array_Get(fichsrc_t, ficheros_donde_se_incluye, i);

		mensaje_error = NULL;
		if (fichsrc_integridad_sistema(fichero, fichero_donde_se_incluye, &mensaje_error) == false)
		{
			arrcad_Anyadir(mensajes_error, mensaje_error);
			strbas_destroy(&mensaje_error);
		}
	}
}

//-----------------------------------------------------------------------

void proyecto_integridad_sistema(
						const struct proyecto_t *proyecto,
						const ArrTipo(proyecto_t) *proyectos,
						ArrString *mensajes_error)
{
	unsigned long i, num_ficheros;

    i_integridad(proyecto);

	num_ficheros = array_numElementos(fichsrc_t, proyecto->ficheros_del_proyecto);

	for (i = 0; i < num_ficheros; i++)
	{
		const struct fichsrc_t *fichero;
		ArrTipo(fichsrc_t) *ficheros_donde_se_incluye;

		fichero = array_Get(fichsrc_t, proyecto->ficheros_del_proyecto, i);
		ficheros_donde_se_incluye = i_obtener_ficheros_es_incluido(proyectos, fichero);

		i_integridad_sistema_donde_se_incluye(fichero, ficheros_donde_se_incluye, mensajes_error);

		array_Destruye(fichsrc_t, &ficheros_donde_se_incluye, NULL);
	}
}

//-----------------------------------------------------------------------

const char *proyecto_path_proyecto(const struct proyecto_t *proyecto)
{
    i_integridad(proyecto);
    return proyecto->nombre_proyecto;
}

//-----------------------------------------------------------------------

char *proyecto_nombre_base(const struct proyecto_t *proyecto)
{
	i_integridad(proyecto);
	return nomfich_nombre_base(proyecto->nombre_proyecto);
}

//-----------------------------------------------------------------------

static const ArrTipo(fichsrc_t) *i_ficheros_con_objs(const ArrTipo(fichsrc_t) *ficheros)
{
	const ArrTipo(fichsrc_t) *ficheros_con_objs;
	unsigned long i, num;
		
	num = array_numElementos(fichsrc_t, ficheros);
	ficheros_con_objs = array_CreaPunteroConst(0, fichsrc_t);
		
	for (i = 0; i < num; i++)
	{
		const struct fichsrc_t *fichero;
			
		fichero = array_Get(fichsrc_t, ficheros, i);
		if (fichsrc_tiene_obj(fichero) == true)
			array_AnyadirConst(fichsrc_t, ficheros_con_objs, fichero);
	}
	
	return ficheros_con_objs; 
}

//-----------------------------------------------------------------------

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
                        struct marco_t *marco)
{
    char *nombre_proyecto;

    i_integridad(proyecto);

    nombre_proyecto = nomfich_nombre_base(proyecto->nombre_proyecto);

    compilador_linkado_plugins(
                        compilador,
                        nombre_proyecto,
                        directorio_base,
                        subdirectorio_plugins, subdirectorio_objs,
                        subdirectorio_dlls, subdirectorio_errores,
                        ficheros_necesarios,
                        includes_de_terceros, dlls_nombre_base_necesarias,
                        marco);

    strbas_destroy(&nombre_proyecto);
}

//-----------------------------------------------------------------------

void proyecto_linkado_dinamico(
						const struct proyecto_t *proyecto,
						const struct compilador_t *compilador,
						const char *directorio_base,
	                    const char *subdirectorio_objs,
	                    const char *subdirectorio_dlls,
						struct marco_t *marco)
{
	char *nombre_proyecto;
	const ArrTipo(fichsrc_t) *ficheros_con_objs;
	
	i_integridad(proyecto);
	
	nombre_proyecto = nomfich_nombre_base(proyecto->nombre_proyecto);
	ficheros_con_objs = i_ficheros_con_objs(proyecto->ficheros_del_proyecto); 
	
	compilador_linkado_dinamico(
						compilador, nombre_proyecto,
						directorio_base,
	                    subdirectorio_objs, subdirectorio_dlls,
						ficheros_con_objs, marco);
	
	array_DestruyeConst(fichsrc_t, &ficheros_con_objs);
	strbas_destroy(&nombre_proyecto);
}

//-----------------------------------------------------------------------

static bool i_tiene_include(const ArrTipo(fichsrc_t) *ficheros_proyecto, const struct fichsrc_t *fichero)
{
	unsigned long i, num;

	num = array_numElementos(fichsrc_t, ficheros_proyecto);

	for (i = 0; i < num; i++)
	{
		const struct fichsrc_t *fichero_es_include;

		fichero_es_include = array_Get(fichsrc_t, ficheros_proyecto, i);

		if (fichsrc_es_include_de_fichero(fichero, fichero_es_include) == true)
			return true;
	}

	return false;
}

//-----------------------------------------------------------------------

void proyecto_append_ejecutables_aplicacion(
						const struct proyecto_t *aplicacion,
						const ArrTipo(fichsrc_t) *ficheros_ejecutables)
{
	unsigned long i, num;

	i_integridad(aplicacion);

	num = array_numElementos(fichsrc_t, aplicacion->ficheros_del_proyecto);

	for (i = 0; i < num; i++)
	{
		const struct fichsrc_t *fichero;

		fichero = array_Get(fichsrc_t, aplicacion->ficheros_del_proyecto, i);

		if (fichsrc_tiene_obj(fichero) == true
				&& i_tiene_include(aplicacion->ficheros_del_proyecto, fichero) == false)
			array_AnyadirConst(fichsrc_t, ficheros_ejecutables, fichero);
	}
}
