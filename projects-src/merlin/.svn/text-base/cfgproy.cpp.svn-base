// Configuraci�n del sistema de gesti�n de proyectos.

#include "cfgproy.inl"

#include "proyecto.inl"
#include "fichsrc.inl"
#include "compilador.inl"
#include "gesenum.inl"
#include "genmak.inl"
#include "CGenFileImage.inl"
#include "CGenerators.inl"

#include "asrtbas.h"
#include "memory.h"
#include "nomfich.h"
#include "strbas.h"
#include "array.h"
#include "arrcad.h"
#include "objsdir.h"
#include "marco.h"

struct cfgproy_t
{
	unsigned long id_nuevo;
	ArrTipo(proyecto_t) *proyectos;
	ArrTipo(proyecto_t) *aplicaciones;
	ArrTipo(proyecto_t) *plugins;
		
    enum gesenum_sistema_operativo_t sistema_operativo;
	char *directorio_base;
	struct compilador_t *compilador_rel, *compilador_deb, *compilador_prof, *compilador_lin;
	
	class CGenerators *generators;
};

static const char *I_LIBS_SRC = "libs-src";
static const char *I_INCLUDE = "include";
static const char *I_ERRORES = "errors";
static const char *I_PROJECTS = "projects-src";
static const char *I_BIN_PROYECTOS = "projects-bin";
static const char *I_PLUGINS = "plugins-src";
static const char *I_BIN_PLUGINS = "plugins-bin";
static const char *I_MAKE = "make";
static const char *I_OBJS = "objs";
static const char *I_DLLS = "dlls";

//-----------------------------------------------------------------------

static void i_integridad(const struct cfgproy_t *cfg_proyectos)
{
	assert_no_null(cfg_proyectos);
	
	assert_no_null(cfg_proyectos->directorio_base);
	assert_no_null(cfg_proyectos->proyectos);
	assert_no_null(cfg_proyectos->aplicaciones);
	assert_no_null(cfg_proyectos->plugins);
	assert_no_null(cfg_proyectos->compilador_rel);
	assert_no_null(cfg_proyectos->compilador_deb);
	assert_no_null(cfg_proyectos->compilador_lin);
	assert_no_null(cfg_proyectos->compilador_prof);
}

//-----------------------------------------------------------------------

static struct cfgproy_t *i_crea(
                            enum gesenum_sistema_operativo_t sistema_operativo,
							unsigned long id_nuevo,
							const char * directorio_base,
							ArrTipo(proyecto_t) **proyectos,
							ArrTipo(proyecto_t) **aplicaciones,
							ArrTipo(proyecto_t) **plugins,
							struct compilador_t **compilador_rel,
							struct compilador_t **compilador_deb,
							struct compilador_t **compilador_prof,
							struct compilador_t **compilador_lin,
							class CGenerators **generators)
{
	struct cfgproy_t *cfg_proyectos;
		
	cfg_proyectos = MALLOC(struct cfgproy_t);
		
    cfg_proyectos->sistema_operativo = sistema_operativo;

	cfg_proyectos->id_nuevo = id_nuevo;
	cfg_proyectos->directorio_base = strbas_copyString(directorio_base);
	cfg_proyectos->proyectos = ASSIGN_PP_NO_NULL(proyectos, ArrTipo(proyecto_t));
	cfg_proyectos->aplicaciones = ASSIGN_PP_NO_NULL(aplicaciones, ArrTipo(proyecto_t));
	cfg_proyectos->plugins = ASSIGN_PP_NO_NULL(plugins, ArrTipo(proyecto_t));

	cfg_proyectos->compilador_rel = ASSIGN_PP_NO_NULL(compilador_rel, struct compilador_t);
	cfg_proyectos->compilador_deb = ASSIGN_PP_NO_NULL(compilador_deb, struct compilador_t);
	cfg_proyectos->compilador_prof = ASSIGN_PP_NO_NULL(compilador_prof, struct compilador_t);
	cfg_proyectos->compilador_lin = ASSIGN_PP_NO_NULL(compilador_lin, struct compilador_t);
	cfg_proyectos->generators = ASSIGN_PP_NO_NULL(generators, class CGenerators);
	
	i_integridad(cfg_proyectos);

	return cfg_proyectos;
}

//-----------------------------------------------------------------------

static const char *i_extension_exe_segun_so(enum gesenum_sistema_operativo_t sistema_operativo)
{
    switch (sistema_operativo) 
    { 
        case GESENUM_COMPILADOR_WIN32:

            return "exe";

        case GESENUM_COMPILADOR_LINUX:

            return "";

        default_error(); 
    }

    return NULL;
}

//-----------------------------------------------------------------------

static const char *i_extension_dll_segun_so(enum gesenum_sistema_operativo_t sistema_operativo)
{
    switch (sistema_operativo) 
    { 
        case GESENUM_COMPILADOR_WIN32:

            return "dll";

        case GESENUM_COMPILADOR_LINUX:

            return "so";

        default_error(); 
    }

    return NULL;
}

//-----------------------------------------------------------------------

static class CGenerators *i_createGenerators(void)
{
	class CGenerators *generators;
	
	generators = new CGenerators();
	
	CGenFileImage::appendGeneratorImage(generators);

	return generators;
}

//-----------------------------------------------------------------------

struct cfgproy_t *cfgproy_crea_defecto(const char *home, enum gesenum_sistema_operativo_t sistema_operativo)
{
	ArrTipo(proyecto_t) *proyectos, *aplicaciones, *plugins;
	unsigned long id_nuevo;
    const char *ext_exes, *ext_dlls;
    const char *prefijo_sistema_operativo;
	struct compilador_t *compilador_rel, *compilador_deb, *compilador_prof, *compilador_lin;
	class CGenerators *generators;
	
	id_nuevo = 0;

    prefijo_sistema_operativo = gesenum_prefijo_segun_sistema_operativo(sistema_operativo);

    ext_exes = i_extension_exe_segun_so(sistema_operativo);
    ext_dlls = i_extension_dll_segun_so(sistema_operativo);
	
	compilador_rel = compilador_crea_rel(home, ext_exes, ext_dlls, prefijo_sistema_operativo);
	compilador_deb = compilador_crea_deb(home, ext_exes, ext_dlls, prefijo_sistema_operativo);
	compilador_prof = compilador_crea_prof(home, ext_exes, ext_dlls, prefijo_sistema_operativo);
	compilador_lin = compilador_crea_lin(home, ext_exes, ext_dlls, prefijo_sistema_operativo);
	
	proyectos = array_CreaPuntero(0, proyecto_t);
	aplicaciones = array_CreaPuntero(0, proyecto_t);
	plugins = array_CreaPuntero(0, proyecto_t);
	
	generators = i_createGenerators();
	
	return i_crea(sistema_operativo, id_nuevo, home, &proyectos, &aplicaciones, &plugins,
						&compilador_rel, &compilador_deb, &compilador_prof, &compilador_lin, &generators);
}

//-----------------------------------------------------------------------

void cfgproy_destruye(struct cfgproy_t **cfg_proyectos)
{
	assert_no_null(cfg_proyectos);
	i_integridad(*cfg_proyectos);
	
	strbas_destroy(&(*cfg_proyectos)->directorio_base);
	array_Destruye(proyecto_t, &(*cfg_proyectos)->proyectos, proyecto_destruye);
	array_Destruye(proyecto_t, &(*cfg_proyectos)->aplicaciones, proyecto_destruye);
	array_Destruye(proyecto_t, &(*cfg_proyectos)->plugins, proyecto_destruye);

	compilador_destruye(&(*cfg_proyectos)->compilador_rel);
	compilador_destruye(&(*cfg_proyectos)->compilador_deb);
	compilador_destruye(&(*cfg_proyectos)->compilador_prof);
	compilador_destruye(&(*cfg_proyectos)->compilador_lin);
		
	FREE_T(cfg_proyectos, struct cfgproy_t);
}

//-----------------------------------------------------------------------

static ArrTipo(proyecto_t) *i_obtener_proyectos(
							const char *directorio_base,
							unsigned long *id_nuevo,
							bool isPlugins,
							const class CGenerators *generators)
{
	ArrTipo(proyecto_t) *proyectos;
	struct objsdir_t *objetos_directorio;
	ArrString *path_proyectos;
	unsigned long num_proyectos, i;
	
	objetos_directorio = objsdir_lee_path(directorio_base);
	path_proyectos = objsdir_obtener_directorios(objetos_directorio);
	objsdir_destruye(&objetos_directorio);
	
	num_proyectos = arrcad_numElementos(path_proyectos);
	
	proyectos = array_CreaPuntero(num_proyectos, proyecto_t);
		
	for (i = 0; i < num_proyectos; i++)
	{
		struct proyecto_t *proyecto;
		const char *path_proyecto;
		
		path_proyecto = arrcad_Get(path_proyectos, i);

		if (isPlugins == true)
		    proyecto = proyecto_genera_plugins(generators, path_proyecto, id_nuevo);
		else
		    proyecto = proyecto_genera_proyecto(generators, path_proyecto, id_nuevo);

		array_Set(proyecto_t, proyectos, i, proyecto);
	}
	
	return proyectos;
}

//-----------------------------------------------------------------------

static void i_eliminar_proyectos_vacios(ArrTipo(proyecto_t) *proyectos)
{
	unsigned long i, num_proyectos, num_borrados;
	
	num_borrados = 0;
	num_proyectos = array_numElementos(proyecto_t, proyectos);
	
	for (i = 0; i < num_proyectos; i++)
	{
		const struct proyecto_t *proyecto;
		unsigned long pos;
		
		pos = i - num_borrados;
		
		proyecto = array_Get(proyecto_t, proyectos, pos);
		
		if (proyecto_es_vacio(proyecto) == true)
		{
			array_Borrar(proyecto_t, proyectos, pos, proyecto_destruye);
			num_borrados++;
		}
	}
}

//-----------------------------------------------------------------------

static void i_calcular_dependencias(ArrTipo(proyecto_t) *proyectos)
{
	unsigned long i, num_proyectos;
	
	num_proyectos = array_numElementos(proyecto_t, proyectos);
	
	for (i = 0; i < num_proyectos; i++)
	{
		struct proyecto_t *proyecto;
		
		proyecto = array_Get(proyecto_t, proyectos, i);
		proyecto_calcular_dependencias_ficheros(proyecto);
	}
}

//-----------------------------------------------------------------------

static void i_eliminar_proyectos_segun_sistema_operativo(
                        ArrTipo(proyecto_t) *proyectos,
                        enum gesenum_sistema_operativo_t sistema_operativo)
{
	unsigned long i, num_proyectos, num_borrados;
	
	num_borrados = 0;
	num_proyectos = array_numElementos(proyecto_t, proyectos);
	
	for (i = 0; i < num_proyectos; i++)
	{
		const struct proyecto_t *proyecto;
		unsigned long pos;
		
		pos = i - num_borrados;
		
		proyecto = array_Get(proyecto_t, proyectos, pos);
		
		if (proyecto_es_compilable_para_sistema_operatico(proyecto, sistema_operativo) == false)
		{
			array_Borrar(proyecto_t, proyectos, pos, proyecto_destruye);
			num_borrados++;
		}
	}
}

//-----------------------------------------------------------------------

static ArrTipo(proyecto_t) *i_generar_proyectos(
                            const char *directorio_base_proyectos,
                            unsigned long *id_nuevo,
                            bool isPlugins,
                            const class CGenerators *generators,
                            enum gesenum_sistema_operativo_t sistema_operativo)
{
    ArrTipo(proyecto_t) *nuevos_proyectos;

    nuevos_proyectos = i_obtener_proyectos(directorio_base_proyectos, id_nuevo, isPlugins, generators);

    i_eliminar_proyectos_vacios(nuevos_proyectos);
    i_eliminar_proyectos_segun_sistema_operativo(nuevos_proyectos, sistema_operativo);
    i_calcular_dependencias(nuevos_proyectos);

    return nuevos_proyectos;
}

//-----------------------------------------------------------------------

static ArrTipo(proyecto_t) *i_generar_proyectos_que_deben_existir(
                            const char *directorio_base,
                            const char *directorio_base_tipo_proyectos,
                            unsigned long *id_nuevo,
                            const class CGenerators *generators,
                            enum gesenum_sistema_operativo_t sistema_operativo)
{
    char *directorio_base_proyectos;
    ArrTipo(proyecto_t) *nuevos_proyectos;
    bool isPlugins;

    directorio_base_proyectos = nomfich_append_a_directorio(directorio_base, directorio_base_tipo_proyectos);

    isPlugins = false;
    nuevos_proyectos = i_generar_proyectos(directorio_base_proyectos, id_nuevo, isPlugins, generators, sistema_operativo);

    strbas_destroy(&directorio_base_proyectos);

    return nuevos_proyectos;
}

//-----------------------------------------------------------------------

void cfgproy_generar_proyectos(struct cfgproy_t *cfg_proyectos)
{
	ArrTipo(proyecto_t) *nuevos_proyectos, *nuevas_aplicaciones, *nuevos_plugins;
	char *directorio_fuente_plugins;
	
	i_integridad(cfg_proyectos);
	
    nuevos_proyectos = i_generar_proyectos_que_deben_existir(
                                cfg_proyectos->directorio_base,
                                I_LIBS_SRC,
                                &cfg_proyectos->id_nuevo,
                                cfg_proyectos->generators,
                                cfg_proyectos->sistema_operativo);

    nuevas_aplicaciones = i_generar_proyectos_que_deben_existir(
                                cfg_proyectos->directorio_base,
                                I_PROJECTS,
                                &cfg_proyectos->id_nuevo,
                                cfg_proyectos->generators,
                                cfg_proyectos->sistema_operativo);

    directorio_fuente_plugins = nomfich_append_a_directorio(cfg_proyectos->directorio_base, I_PLUGINS);

    if (nomfich_existe_directorio(directorio_fuente_plugins) == true)
    {
        bool isPlugins;

        isPlugins = true;
        nuevos_plugins = i_generar_proyectos(
                                directorio_fuente_plugins,
                                &cfg_proyectos->id_nuevo,
                                isPlugins,
                                cfg_proyectos->generators,
                                cfg_proyectos->sistema_operativo);
    }
    else
        nuevos_plugins = array_CreaPuntero(0, proyecto_t);

    strbas_destroy(&directorio_fuente_plugins);

	array_Destruye(proyecto_t, &(cfg_proyectos->proyectos), proyecto_destruye);
	cfg_proyectos->proyectos = nuevos_proyectos;

	array_Destruye(proyecto_t, &(cfg_proyectos->aplicaciones), proyecto_destruye);
	cfg_proyectos->aplicaciones = nuevas_aplicaciones;

    array_Destruye(proyecto_t, &(cfg_proyectos->plugins), proyecto_destruye);
    cfg_proyectos->plugins = nuevos_plugins;
}

//-----------------------------------------------------------------------

void cfgproy_debug(const struct cfgproy_t *cfg_proyectos, struct marco_t *marco)
{
	unsigned long num_proyectos, i;
	
	i_integridad(cfg_proyectos);
	
	num_proyectos = array_numElementos(proyecto_t, cfg_proyectos->proyectos);
	
	marco_lanza_mensaje(marco, MARCO_MENSAJE_INFORMACION, "Imprimiendo proyectos....");
	
	for (i = 0; i < num_proyectos; i++)
	{
		const struct proyecto_t *proyecto;
			
		proyecto = array_Get(proyecto_t, cfg_proyectos->proyectos, i);
		
		proyecto_debug(proyecto, marco);
	}
	
	marco_lanza_mensaje(marco, MARCO_MENSAJE_INFORMACION, "Fin proyectos");
}

//-----------------------------------------------------------------------

ArrString *cfgproy_es_incluido(const struct cfgproy_t *cfg_proyectos, const char *fichero)
{
	unsigned long num_proyectos, i;
	ArrString *ficheros;

	i_integridad(cfg_proyectos);
	
	num_proyectos = array_numElementos(proyecto_t, cfg_proyectos->proyectos);
	ficheros = arrcad_CreaString(0);
	
	for (i = 0; i < num_proyectos; i++)
	{
		const struct proyecto_t *proyecto;
		ArrString *ficheros_en_proyecto;
			
		proyecto = array_Get(proyecto_t, cfg_proyectos->proyectos, i);
		
		ficheros_en_proyecto = proyecto_es_incluido(proyecto, fichero);
		arrcad_Concatena(ficheros, ficheros_en_proyecto);
		arrcad_Destruye(&ficheros_en_proyecto);
	}
	
	return ficheros;
}

//-----------------------------------------------------------------------

static bool i_buscar_proyecto(const ArrTipo(proyecto_t) *proyectos,
								const char *fichero_incluido,
								struct proyecto_t **proyecto_buscado)
{
	bool se_ha_encontrado;
	unsigned long ind_proyecto;
	
	se_ha_encontrado = array_BuscarUnica(
						proyecto_t, proyectos, 
						char, fichero_incluido, 
						proyecto_fichero_pertenece_a_proyecto, 
						&ind_proyecto);
	
	if (se_ha_encontrado == true && proyecto_buscado != NULL)
		*proyecto_buscado = array_Get(proyecto_t, proyectos, ind_proyecto);
	
	return se_ha_encontrado;
}

//-----------------------------------------------------------------------

static void i_anyadir_proyecto_si_no_existe(
								const struct proyecto_t *proyecto,
								const ArrTipo(proyecto_t) *proyectos_anyadidos)
{
	if (array_BuscarUnica(
				proyecto_t, proyectos_anyadidos, 
				struct proyecto_t, proyecto, 
				proyecto_mismo_nombre, NULL) == false)
		array_AnyadirConst(proyecto_t, proyectos_anyadidos, proyecto);
}

//-----------------------------------------------------------------------

static void i_anyadir_proyecto_dependiente(
				const ArrTipo(proyecto_t) *proyectos,
				const ArrTipo(proyecto_t) *proyectos_dependientes,
				const char *fichero_incluido)
{
	struct proyecto_t *proyecto_donde_esta;
	
	if (i_buscar_proyecto(proyectos, fichero_incluido, &proyecto_donde_esta) == true)
		i_anyadir_proyecto_si_no_existe(proyecto_donde_esta, proyectos_dependientes); 
}

//-----------------------------------------------------------------------

static ArrTipo(proyecto_t) *i_obtener_dependencias_proyecto(
								const struct proyecto_t *proyecto,
								ArrTipo(proyecto_t) *proyectos)
{
	ArrString *ficheros_incluidos;
	ArrTipo(proyecto_t) *proyectos_dependientes;		
	unsigned long num_ficheros_incluidos, i;
			
	ficheros_incluidos = proyecto_ficheros_incluidos(proyecto);
	num_ficheros_incluidos = arrcad_numElementos(ficheros_incluidos);
	proyectos_dependientes = array_CreaPuntero(0, proyecto_t);
	
	for (i = 0; i < num_ficheros_incluidos; i++)
	{
		const char *fichero_incluido;
		
		fichero_incluido = arrcad_Get(ficheros_incluidos, i);
		i_anyadir_proyecto_dependiente(proyectos, proyectos_dependientes, fichero_incluido);
	}
	
	return proyectos_dependientes;
}

//-----------------------------------------------------------------------

bool cfgproy_dependencias_proyectos(
						const struct cfgproy_t *cfg_proyectos, 
						const char *nombre_proyecto,
						ArrString **pnombres_proyectos)
{
	bool existe;
	unsigned long pos;
	char *nombre_completo_proyecto, *directorio_fuente;
		
	i_integridad(cfg_proyectos);
	
	directorio_fuente = nomfich_append_a_directorio(cfg_proyectos->directorio_base, I_LIBS_SRC);

	nombre_completo_proyecto = nomfich_append_a_directorio(directorio_fuente, nombre_proyecto);
	
	assert_no_null(pnombres_proyectos);
	
	existe = array_Buscar(
						proyecto_t, cfg_proyectos->proyectos, char, nombre_completo_proyecto, 
						proyecto_es_proyecto, &pos);
	
	if (existe == true)
	{
		static ArrTipo(proyecto_t) *proyectos_dependientes;
		struct proyecto_t *proyecto;
		
		proyecto = array_Get(proyecto_t, cfg_proyectos->proyectos, pos);
		
		proyectos_dependientes = i_obtener_dependencias_proyecto(proyecto, cfg_proyectos->proyectos);
			
		*pnombres_proyectos = proyecto_obtener_nombres(proyectos_dependientes);
		
		array_Destruye(proyecto_t, &proyectos_dependientes, NULL);
	}
	
	strbas_destroy(&nombre_completo_proyecto);
	strbas_destroy(&directorio_fuente);
	
	return existe;
}

//-----------------------------------------------------------------------

static ArrTipo(fichsrc_t) *i_obtener_ficheros_a_compilar(
						ArrTipo(proyecto_t) *proyectos,
						const char *directorio_base,
						const char *subdirectorio_objs,
						const struct compilador_t *compilador)
{
	ArrTipo(fichsrc_t) *ficheros_a_compilar;
	unsigned long num_proyectos, i;
	
	ficheros_a_compilar = array_CreaPuntero(0, fichsrc_t);
	
	num_proyectos = array_numElementos(proyecto_t, proyectos);
	
	for (i = 0; i < num_proyectos; i++)
	{
		const struct proyecto_t *proyecto;
			
		proyecto = array_Get(proyecto_t, proyectos, i);
		
		proyecto_obtener_ficheros_a_compilar(
						proyecto, proyectos,
						ficheros_a_compilar,
						compilador,
						directorio_base, subdirectorio_objs);
	}
	
	return ficheros_a_compilar;
}

//-----------------------------------------------------------------------

static void i_obtener_proyectos_necesarios(
						ArrTipo(proyecto_t) *proyectos, 
						const struct fichsrc_t *fichero,
						ArrTipo(proyecto_t) *proyectos_necesarios,
						ArrString *includes_de_terceros)
{
	const ArrString *incluidos;
	unsigned long num_incluidos, i;
		
	incluidos = fichsrc_incluidos(fichero);
	num_incluidos = arrcad_numElementos(incluidos);
	
	for (i = 0; i < num_incluidos; i++)
	{
		const char *include;
		struct proyecto_t *proyecto_al_que_pertenece;
		
		include = arrcad_Get(incluidos, i);
		
		if (i_buscar_proyecto(proyectos, include, &proyecto_al_que_pertenece) == true)
		{
			struct fichsrc_t *fichero_include;
			bool existe_fichero;

			existe_fichero = proyecto_existe_fichero(proyecto_al_que_pertenece, include, &fichero_include);
			assert(existe_fichero == true);

			i_obtener_proyectos_necesarios(proyectos, fichero_include, proyectos_necesarios, includes_de_terceros);
			i_anyadir_proyecto_si_no_existe(proyecto_al_que_pertenece, proyectos_necesarios); 
		}

		if (arrcad_Buscar(includes_de_terceros, include, NULL, NULL) == false)
			arrcad_Anyadir(includes_de_terceros, include);
	}
}

//-----------------------------------------------------------------------

static void i_compilar_ficheros(
						struct marco_t *marco,
						ArrTipo(fichsrc_t) *ficheros_a_compilar,
						ArrTipo(proyecto_t) *proyectos,
						const ArrTipo(fichsrc_t) *ficheros_proyecto_include,
						const struct compilador_t *compilador,
						const char *directorio_base)
{
	unsigned long num_ficheros, i;
	
	num_ficheros = array_numElementos(fichsrc_t, ficheros_a_compilar);
	
	for (i = 0; i < num_ficheros; i++)
	{
		struct fichsrc_t *fichero;
		ArrTipo(proyecto_t) *proyectos_necesarios;
		ArrString *includes_necesarios, *includes_de_terceros;
			
		fichero = array_Get(fichsrc_t, ficheros_a_compilar, i);
		
		proyectos_necesarios = array_CreaPuntero(0, proyecto_t);
		includes_de_terceros = arrcad_CreaString(0);
		
		i_obtener_proyectos_necesarios(proyectos, fichero, proyectos_necesarios, includes_de_terceros);
		includes_necesarios = proyecto_obtener_nombres(proyectos_necesarios);

		compilador_compilar(
						compilador, marco, fichero, ficheros_proyecto_include,
						includes_necesarios, includes_de_terceros,
						directorio_base, I_OBJS, I_ERRORES);
		
		array_Destruye(proyecto_t, &proyectos_necesarios, NULL);
		arrcad_Destruye(&includes_de_terceros);
		arrcad_Destruye(&includes_necesarios);
	}
}

//-----------------------------------------------------------------------

static const struct compilador_t *i_obtener_compilador(
						enum tipo_compilacion_t tipo_compilacion,
						const struct compilador_t *compilador_rel, 
						const struct compilador_t *compilador_deb, 
						const struct compilador_t *compilador_prof, 
						const struct compilador_t *compilador_lin)
{
	switch(tipo_compilacion)
	{
		case CFGPROY_RELEASE:
		
			return compilador_rel;
		
		case CFGPROY_DEBUG:
		
			return compilador_deb;
		
		case CFGPROY_PROFILER:
		
			return compilador_prof;
		
		case CFGPROY_LINT:
		
			return compilador_lin;
		
		default_error();
	}
	
	assert_return_error(const struct compilador_t *);
}

//-----------------------------------------------------------------------

static const ArrTipo(fichsrc_t) *i_obtener_ficheros_proyecto(
								const char *directorio_base,
								const ArrTipo(proyecto_t) *proyectos,
								const char *nombre_base_proyecto)
{
	char *directorio_fuente, *nombre_completo_proyecto;
	unsigned long ind_proyecto;
	
	directorio_fuente = nomfich_append_a_directorio(directorio_base, I_LIBS_SRC);
	nombre_completo_proyecto = nomfich_append_a_directorio(directorio_fuente, nombre_base_proyecto);
	
	if (array_Buscar(
				proyecto_t, proyectos, char, nombre_completo_proyecto, 
				proyecto_es_proyecto, &ind_proyecto) == true)
	{
		struct proyecto_t *proyecto;
		
		proyecto = array_Get(proyecto_t, proyectos, ind_proyecto);
		return proyecto_fuentes(proyecto);
	}
	else
		return NULL;
}

//-----------------------------------------------------------------------

static const ArrTipo(fichsrc_t) *i_obtener_ficheros_a_generar(
					const ArrTipo(proyecto_t) *proyectos,
					const class CGenerators *generators)
{
	unsigned long i, num;
	const ArrTipo(fichsrc_t) *fichsrc_a_generar;
		
	fichsrc_a_generar = array_CreaPunteroConst(0, fichsrc_t); 
	
	num = array_numElementos(proyecto_t, proyectos);
		
	for (i = 0; i < num; i++)
	{
		const struct proyecto_t *proyecto;
			
		proyecto = array_Get(proyecto_t, proyectos, i);
		proyecto_obtener_ficheros_a_generar(proyecto, generators, fichsrc_a_generar);
	}
	
	return fichsrc_a_generar;
}

//-----------------------------------------------------------------------

static void i_generar_fuentes(
					const ArrTipo(proyecto_t) *proyectos,
					const class CGenerators *generators,
					struct marco_t *marco)
{
	unsigned long i, num;
	const ArrTipo(fichsrc_t) *ficheros_a_generar;
		
	ficheros_a_generar = i_obtener_ficheros_a_generar(proyectos, generators);
	
	num = array_numElementos(fichsrc_t, ficheros_a_generar);
		
	for (i = 0; i < num; i++)
	{
		const struct fichsrc_t *fichero_a_generar;
			
		fichero_a_generar = array_Get(fichsrc_t, ficheros_a_generar, i);
		fichsrc_generar(fichero_a_generar, generators, marco);
	}
	
	array_DestruyeConst(fichsrc_t, &ficheros_a_generar);
}

//-----------------------------------------------------------------------

static void i_compilar(
				ArrTipo(proyecto_t) *proyectos,
				ArrTipo(proyecto_t) *proyectos_dependientes,
				const char *directorio_base,
				const char *subdirectorio_include,
				const char *subdirectorio_objs,
				const struct compilador_t *compilador,
				struct marco_t *marco)
{
	ArrTipo(fichsrc_t) *ficheros_a_compilar;
	const ArrTipo(fichsrc_t) *ficheros_proyecto_include;
	
	ficheros_a_compilar = i_obtener_ficheros_a_compilar(proyectos, directorio_base, subdirectorio_objs, compilador);

	ficheros_proyecto_include = i_obtener_ficheros_proyecto(directorio_base, proyectos_dependientes, subdirectorio_include);

	i_compilar_ficheros(
				marco, ficheros_a_compilar, proyectos_dependientes,
				ficheros_proyecto_include, compilador,
				directorio_base);
								
	array_Destruye(fichsrc_t, &ficheros_a_compilar, NULL);
}

//-----------------------------------------------------------------------

static void i_obtener_ficheros_necesarios_para_linkar(
					const struct fichsrc_t *fichero_base,
					const struct fichsrc_t *fichero_include,
					const class CGenerators *generators,
					const ArrTipo(proyecto_t) *proyectos,
					const ArrTipo(fichsrc_t) *ficheros_necesarios,
					bool se_incluyen_fuentes_de_dinamica,
					const ArrTipo(proyecto_t) *dlls_necesarias_opc,
					ArrString *includes_de_terceros)
{
	if (se_incluyen_fuentes_de_dinamica == true)
		assert(dlls_necesarias_opc == NULL);
	else
		assert_no_null(dlls_necesarias_opc);

	if (array_BuscarUnica(
					fichsrc_t, ficheros_necesarios,
					struct fichsrc_t, fichero_base,
					fichsrc_es_igual, NULL) == false)
    {
        ArrString *ficheros_incluidos;
        unsigned long num_ficheros_incluidos, i;

        if (fichsrc_es_cabecera(fichero_base) == false)
            array_AnyadirConst(fichsrc_t, ficheros_necesarios, fichero_base);
							
        ficheros_incluidos = arrcad_CreaString(0);
        fichsrc_anyadir_ficheros_que_incluye(fichero_base, ficheros_incluidos);
        
        if (fichero_include != NULL)
        	fichsrc_anyadir_ficheros_que_incluye(fichero_include, ficheros_incluidos);
        
        num_ficheros_incluidos = arrcad_numElementos(ficheros_incluidos);
        
        for (i = 0; i < num_ficheros_incluidos; i++)
        {
            const char *include;
            
            include = arrcad_Get(ficheros_incluidos, i);
            
            if (fichsrc_es_include(fichero_base, generators, include) == false)
            {
                struct proyecto_t *proyecto_donde_esta;
                
                if (i_buscar_proyecto(proyectos, include, &proyecto_donde_esta) == true)
                {
                	if (se_incluyen_fuentes_de_dinamica == true
                			|| proyecto_es_libreria_dinamica(proyecto_donde_esta) == false)
                	{
						struct fichsrc_t *fich_fuente;
						char *nombre_fichero_fuente;
						const char *directorio;
                        struct fichsrc_t *fich_include;
						
						directorio = proyecto_path_proyecto(proyecto_donde_esta);
						nombre_fichero_fuente = fichsrc_obtener_fuente(directorio, include);
						
						if (proyecto_existe_fichero(proyecto_donde_esta, nombre_fichero_fuente, &fich_fuente) == false)
						    fich_fuente = NULL;

                        if (proyecto_existe_fichero(proyecto_donde_esta, include, &fich_include) == true)
                        {
                            if (fich_fuente == NULL)
                            {
                                fich_fuente = fich_include;
                                fich_include = NULL;
                            }

                            i_obtener_ficheros_necesarios_para_linkar(
                                                fich_fuente,
                                                fich_include,
                                                generators,
                                                proyectos,
                                                ficheros_necesarios,
                                                se_incluyen_fuentes_de_dinamica,
                                                dlls_necesarias_opc,
                                                includes_de_terceros);
                        }
						
						strbas_destroy(&nombre_fichero_fuente);
                	}
                	else
                	{
                		assert_no_null(dlls_necesarias_opc);
                		array_AnyadirConst(proyecto_t, dlls_necesarias_opc, proyecto_donde_esta);
                	}
                }
                
				if (arrcad_Buscar(includes_de_terceros, include, NULL, NULL) == false)
					arrcad_Anyadir(includes_de_terceros, include);
            }
        }
    }
}

//-----------------------------------------------------------------------

static ArrString *i_nombres_bases_dlls(const ArrTipo(proyecto_t) *dlls_necesarias)
{
	ArrString *dlls_nombre_base_necesarias;
	unsigned long i, num;
		
	num = array_numElementos(proyecto_t, dlls_necesarias);
	dlls_nombre_base_necesarias = arrcad_CreaString(num);
		
	for (i = 0; i < num; i++)
	{
		const struct proyecto_t *proyecto;
		char *nombre_base_proyecto;
			
		proyecto = array_Get(proyecto_t, dlls_necesarias, i);
		nombre_base_proyecto = proyecto_nombre_base(proyecto);
		arrcad_SetDestruyendoCadena(dlls_nombre_base_necesarias, i, &nombre_base_proyecto);
	}
	
	return dlls_nombre_base_necesarias;
}

//-----------------------------------------------------------------------

static char *i_nombre_base_proyecto_aplicacion(
					const ArrTipo(proyecto_t) *proyectos,
					const struct fichsrc_t *aplicacion)
{
	unsigned long ind_fichero;
	bool existe;
	const struct proyecto_t *proyecto_aplicacion;

	existe = array_Buscar(
					proyecto_t, proyectos, struct fichsrc_t, aplicacion,
					proyecto_fichero_fuente_pertenece_a_proyecto, &ind_fichero);
	assert(existe == true);

	proyecto_aplicacion = array_Get(proyecto_t, proyectos, ind_fichero);

	return proyecto_nombre_base(proyecto_aplicacion);
}

//-----------------------------------------------------------------------

static void i_linkar_ejecutables_aplicaciones(
					const ArrTipo(fichsrc_t) *ficheros_aplicaciones,
					const ArrTipo(proyecto_t) *proyectos,
                    const class CGenerators *generators,
					const struct compilador_t *compilador,
					const char *directorio_base,
					const char *subdirectorio_ejecutables,
                    const char *subdirectorio_objs,
                    const char *subdirectorio_dlls,
					struct marco_t *marco)
{
	unsigned long num_ejecutables, i;

	num_ejecutables = array_numElementos(fichsrc_t, ficheros_aplicaciones);

	for (i = 0; i < num_ejecutables; i++)
	{
		const struct fichsrc_t *aplicacion;
		ArrString *includes_de_terceros, *dlls_nombre_base_necesarias;
		const ArrTipo(fichsrc_t) *ficheros_necesarios;
		const ArrTipo(proyecto_t) *dlls_necesarias;
		char *nombre_base_proyecto, *subdirectorio_ejecutable_aplicacion;

		aplicacion = array_Get(fichsrc_t, ficheros_aplicaciones, i);
		nombre_base_proyecto = i_nombre_base_proyecto_aplicacion(proyectos, aplicacion);

		ficheros_necesarios = array_CreaPunteroConst(0, fichsrc_t);
		dlls_necesarias = array_CreaPunteroConst(0, proyecto_t);
		includes_de_terceros = arrcad_CreaString(0);

		i_obtener_ficheros_necesarios_para_linkar(
					aplicacion,
					NULL, //fichero_include
					generators,
					proyectos, ficheros_necesarios,
					false, //se_incluyen_fuentes_de_dinamica
					dlls_necesarias, includes_de_terceros);

		dlls_nombre_base_necesarias = i_nombres_bases_dlls(dlls_necesarias);

		subdirectorio_ejecutable_aplicacion = nomfich_append_a_directorio(
					subdirectorio_ejecutables,
					nombre_base_proyecto);

		compilador_linkar(
                    compilador, aplicacion,
                    directorio_base,
                    subdirectorio_ejecutable_aplicacion,
                    subdirectorio_objs, subdirectorio_dlls,
                    ficheros_necesarios,
                    includes_de_terceros, dlls_nombre_base_necesarias,
                    marco);

		strbas_destroy(&subdirectorio_ejecutable_aplicacion);
		strbas_destroy(&nombre_base_proyecto);

		arrcad_Destruye(&includes_de_terceros);
		arrcad_Destruye(&dlls_nombre_base_necesarias);
		array_DestruyeConst(fichsrc_t, &ficheros_necesarios);
		array_DestruyeConst(proyecto_t, &dlls_necesarias);
	}
}

//-----------------------------------------------------------------------

const ArrTipo(proyecto_t) *i_proyectos_librerias_dinamicas(const ArrTipo(proyecto_t) *proyectos)
{
	const ArrTipo(proyecto_t) *proyectos_libreria_dinamica;
	unsigned long i, num;
		
	num = array_numElementos(proyecto_t, proyectos);
	proyectos_libreria_dinamica = array_CreaPunteroConst(0, proyecto_t);

	for (i = 0; i < num; i++)
	{
		const struct proyecto_t *proyecto;
			
		proyecto = array_Get(proyecto_t, proyectos, i);
		if (proyecto_es_libreria_dinamica(proyecto) == true)
			array_AnyadirConst(proyecto_t, proyectos_libreria_dinamica, proyecto);
	}

	return proyectos_libreria_dinamica;
}

//-----------------------------------------------------------------------

static void i_linkar_librerias_dinamicas(
							const ArrTipo(proyecto_t) *proyectos, 
							const struct compilador_t *compilador,
							const char *directorio_base,
							const char *subdirectorio_objs,
							const char *subdirectorio_dlls,
							struct marco_t *marco)
{
	const ArrTipo(proyecto_t) *proyectos_librearias_dinamicas;
	unsigned long i, num;

	proyectos_librearias_dinamicas = i_proyectos_librerias_dinamicas(proyectos);

	num = array_numElementos(proyecto_t, proyectos_librearias_dinamicas);

	for (i = 0; i < num; i++)
	{
		const struct proyecto_t *proyecto;
			
		proyecto = array_Get(proyecto_t, proyectos_librearias_dinamicas, i);
		proyecto_linkado_dinamico(proyecto, compilador, directorio_base, subdirectorio_objs, subdirectorio_dlls, marco);
	}

	array_DestruyeConst(proyecto_t, &proyectos_librearias_dinamicas);
}

//-----------------------------------------------------------------------

static const ArrTipo(fichsrc_t) *i_obtener_ejecutables(const ArrTipo(proyecto_t) *aplicaciones)
{
	unsigned long i, num;
	const ArrTipo(fichsrc_t) *ficheros_ejecutables;

	ficheros_ejecutables = array_CreaPunteroConst(0, fichsrc_t);

	num = array_numElementos(proyecto_t, aplicaciones);

	for (i = 0; i < num; i++)
	{
		const struct proyecto_t *aplicacion;

		aplicacion = array_Get(proyecto_t, aplicaciones, i);
		proyecto_append_ejecutables_aplicacion(aplicacion, ficheros_ejecutables);
	}

	return ficheros_ejecutables;
}

//-----------------------------------------------------------------------

static void i_linkar_aplicaciones(
				const ArrTipo(proyecto_t) *aplicaciones,
				const ArrTipo(proyecto_t) *proyectos_dependencias,
                const class CGenerators *generators,
				const char *directorio_base,
				const struct compilador_t *compilador,
				struct marco_t *marco)
{
	const ArrTipo(fichsrc_t) *ficheros_aplicaciones;

	ficheros_aplicaciones = i_obtener_ejecutables(aplicaciones);

	if (array_numElementos(fichsrc_t, ficheros_aplicaciones) > 0)
		i_linkar_ejecutables_aplicaciones(
				ficheros_aplicaciones,
				proyectos_dependencias,
				generators,
				compilador, directorio_base,
				I_BIN_PROYECTOS, I_OBJS, I_DLLS,
				marco);

	array_DestruyeConst(fichsrc_t, &ficheros_aplicaciones);
}

//-----------------------------------------------------------------------

static void i_linkar_proyecto_plugins(
                const struct proyecto_t *proyecto,
                const ArrTipo(proyecto_t) *todos_los_proyectos_plugins,
                const class CGenerators *generators,
                const char *directorio_base,
                const char *subdirectorio_plugins,
                const char *subdirectorio_objs,
                const char *subdirectorio_dlls,
                const char *subdirectorio_errores,
                const struct compilador_t *compilador,
                struct marco_t *marco)
{
    const ArrTipo(fichsrc_t) *ficheros_proyecto;
    ArrString *includes_de_terceros, *dlls_nombre_base_necesarias;
    const ArrTipo(fichsrc_t) *ficheros_necesarios;
    const ArrTipo(proyecto_t) *dlls_necesarias;
    unsigned long i, num;

    ficheros_necesarios = array_CreaPunteroConst(0, fichsrc_t);
    dlls_necesarias = array_CreaPunteroConst(0, proyecto_t);
    includes_de_terceros = arrcad_CreaString(0);

    ficheros_proyecto = proyecto_fuentes(proyecto);
    num = array_numElementos(fichsrc_t, ficheros_proyecto);

    for (i = 0; i < num; i++)
    {
        const struct fichsrc_t *fichero_proyecto;

        fichero_proyecto = array_Get(fichsrc_t, ficheros_proyecto, i);

        i_obtener_ficheros_necesarios_para_linkar(
                fichero_proyecto,
                NULL, //fichero_include
                generators,
                todos_los_proyectos_plugins,
                ficheros_necesarios,
                false, //se_incluyen_fuentes_de_dinamica
                dlls_necesarias, includes_de_terceros);
    }

    dlls_nombre_base_necesarias = i_nombres_bases_dlls(dlls_necesarias);

    proyecto_linkar_plugins(
                proyecto, compilador,
                directorio_base,
                subdirectorio_plugins, subdirectorio_objs,
                subdirectorio_dlls, subdirectorio_errores,
                ficheros_necesarios,
                includes_de_terceros, dlls_nombre_base_necesarias, marco);

    arrcad_Destruye(&includes_de_terceros);
    arrcad_Destruye(&dlls_nombre_base_necesarias);
    array_DestruyeConst(fichsrc_t, &ficheros_necesarios);
    array_DestruyeConst(proyecto_t, &dlls_necesarias);
}

//-----------------------------------------------------------------------

static void i_linkar_plugins(
                const ArrTipo(proyecto_t) *plugins,
                const ArrTipo(proyecto_t) *todos_los_proyectos_plugins,
                const class CGenerators *generators,
                const char *directorio_base,
                const char *subdirectorio_plugins,
                const char *subdirectorio_objs,
                const char *subdirectorio_dlls,
                const char *subdirectorio_errores,
                const struct compilador_t *compilador,
                struct marco_t *marco)
{
    unsigned long i, num;

    num = array_numElementos(proyecto_t, plugins);

    for (i = 0; i < num; i++)
    {
        const struct proyecto_t *proyecto;

        proyecto = array_Get(proyecto_t, plugins, i);
        i_linkar_proyecto_plugins(
                        proyecto,
                        todos_los_proyectos_plugins,
                        generators,
                        directorio_base,
                        subdirectorio_plugins, subdirectorio_objs,
                        subdirectorio_dlls, subdirectorio_errores,
                        compilador, marco);
    }
}

//-----------------------------------------------------------------------

static void i_borra_los_directorios_vacios(unsigned long num_recursividad, const char *directorio)
{
	struct objsdir_t *objs_directorio;
	ArrString *nombres_subdirectorios;
	unsigned long i, num;

	num_recursividad++;
	assert(num_recursividad < 100);

	objs_directorio = objsdir_lee_path(directorio);

	nombres_subdirectorios = objsdir_obtener_directorios(objs_directorio);

	num = arrcad_numElementos(nombres_subdirectorios);

	for (i = 0; i < num; i++)
	{
		const char *nombre_subdirectorio;

		nombre_subdirectorio = arrcad_Get(nombres_subdirectorios, i);

		i_borra_los_directorios_vacios(num_recursividad, nombre_subdirectorio);

		if (objsdir_es_directorio_vacio(nombre_subdirectorio) == true)
			nomfich_borrar_directorio(nombre_subdirectorio);
	}

	objsdir_destruye(&objs_directorio);
	arrcad_Destruye(&nombres_subdirectorios);
}

//-----------------------------------------------------------------------

static void i_limpiar_directorios_vacios(const char *directorio_base)
{
	char *directorio_errores;

	directorio_errores = nomfich_append_a_directorio(directorio_base, I_ERRORES);

	if (nomfich_existe_directorio(directorio_errores) == true)
		i_borra_los_directorios_vacios(0, directorio_errores);

	strbas_destroy(&directorio_errores);
}

//-----------------------------------------------------------------------

void cfgproy_hacer_ejecutables(
					const struct cfgproy_t *cfg_proyectos,
					enum tipo_compilacion_t tipo_compilacion,
					struct marco_t *marco)
{
	const struct compilador_t *compilador;
	ArrTipo(proyecto_t) *todos_los_proyectos_aplicaciones, *todos_los_proyectos_plugins;

	i_integridad(cfg_proyectos);

	compilador = i_obtener_compilador(
					tipo_compilacion,
					cfg_proyectos->compilador_rel,
					cfg_proyectos->compilador_deb,
					cfg_proyectos->compilador_prof,
					cfg_proyectos->compilador_lin);

    todos_los_proyectos_aplicaciones = array_Copia(proyecto_t, cfg_proyectos->aplicaciones, NULL);
    array_ConcatenaConst(proyecto_t, todos_los_proyectos_aplicaciones, cfg_proyectos->proyectos);

    todos_los_proyectos_plugins = array_Copia(proyecto_t, cfg_proyectos->plugins, NULL);
    array_ConcatenaConst(proyecto_t, todos_los_proyectos_plugins, cfg_proyectos->proyectos);

	i_generar_fuentes(cfg_proyectos->proyectos, cfg_proyectos->generators, marco);
	i_generar_fuentes(cfg_proyectos->aplicaciones, cfg_proyectos->generators, marco);
	i_generar_fuentes(cfg_proyectos->plugins, cfg_proyectos->generators, marco);

	i_compilar(cfg_proyectos->proyectos, cfg_proyectos->proyectos, cfg_proyectos->directorio_base, I_INCLUDE, I_OBJS, compilador, marco);
    i_compilar(cfg_proyectos->aplicaciones, todos_los_proyectos_aplicaciones, cfg_proyectos->directorio_base, I_INCLUDE, I_OBJS, compilador, marco);
    i_compilar(cfg_proyectos->plugins, todos_los_proyectos_aplicaciones, cfg_proyectos->directorio_base, I_INCLUDE, I_OBJS, compilador, marco);

	switch(tipo_compilacion)
	{
		case CFGPROY_RELEASE:
		case CFGPROY_DEBUG:
		case CFGPROY_PROFILER:
		{
			i_linkar_librerias_dinamicas(
                        cfg_proyectos->proyectos,
                        compilador,
                        cfg_proyectos->directorio_base,
                        I_OBJS, I_DLLS, marco);

			i_linkar_aplicaciones(
                        cfg_proyectos->aplicaciones,
                        todos_los_proyectos_aplicaciones,
                        cfg_proyectos->generators,
						cfg_proyectos->directorio_base,
						compilador, marco);

			i_linkar_plugins(
                        cfg_proyectos->plugins,
                        todos_los_proyectos_plugins,
                        cfg_proyectos->generators,
                        cfg_proyectos->directorio_base,
                        I_BIN_PLUGINS, I_OBJS, I_DLLS, I_ERRORES,
                        compilador, marco);
			break;
		}

		case CFGPROY_LINT:

			break;

		default_error();
	}

	i_limpiar_directorios_vacios(cfg_proyectos->directorio_base);

	array_DestruyeConst(proyecto_t, &todos_los_proyectos_aplicaciones);
	array_DestruyeConst(proyecto_t, &todos_los_proyectos_plugins);
}

//-----------------------------------------------------------------------

void cfgproy_integridad_sistema(
					const struct cfgproy_t *cfg_proyectos, 
					ArrString **mensajes)
{
	ArrString *mensajes_errores;
	unsigned long num_proyectos, i;

	i_integridad(cfg_proyectos);
	assert_no_null(mensajes);
	
	mensajes_errores = arrcad_CreaString(0);
	
	num_proyectos = array_numElementos(proyecto_t, cfg_proyectos->proyectos);
	
	for (i = 0; i < num_proyectos; i++)
	{
		const struct proyecto_t *proyecto;
			
		proyecto = array_Get(proyecto_t, cfg_proyectos->proyectos, i);
		proyecto_integridad_sistema(proyecto, cfg_proyectos->proyectos, mensajes_errores);
	}
	
	*mensajes = mensajes_errores;
}

//-----------------------------------------------------------------------

void cfgproy_clean_segun_compilacion(
							const struct cfgproy_t *cfg_proyectos,
							enum tipo_compilacion_t tipo_compilacion,
							struct marco_t *marco)
{
	const struct compilador_t *compilador;

	i_integridad(cfg_proyectos);

	compilador = i_obtener_compilador(
					tipo_compilacion,
					cfg_proyectos->compilador_rel,
					cfg_proyectos->compilador_deb,
					cfg_proyectos->compilador_prof,
					cfg_proyectos->compilador_lin);

	compilador_clean(
					compilador,
					cfg_proyectos->directorio_base,
					I_OBJS, I_DLLS, I_BIN_PROYECTOS, I_ERRORES,
					marco);
}

//-----------------------------------------------------------------------

static void i_directorios_completos_necesarios(const char *directorio_base, char **fuentes, char **projects)
{
	assert_no_null(fuentes);
	assert_no_null(projects);

	*fuentes = nomfich_append_a_directorio(directorio_base, I_LIBS_SRC);
	*projects = nomfich_append_a_directorio(directorio_base, I_PROJECTS);
}

//-----------------------------------------------------------------------

static bool i_verifica_directorios(const char *directorio_base)
{
	bool existen_directorios;
	char *fuentes, *projects;

	i_directorios_completos_necesarios(directorio_base, &fuentes, &projects);

	if (nomfich_es_directorio(directorio_base) == false
			|| nomfich_es_directorio(fuentes) == false
			|| nomfich_es_directorio(projects) == false)
	{
		existen_directorios = false;
	}
	else
		existen_directorios = true;

	strbas_destroy(&fuentes);
	strbas_destroy(&projects);

	return existen_directorios;
}

//-----------------------------------------------------------------------

void cfgproy_crea_directorios_necesarios(struct marco_t *marco, const char *directorio_base)
{
	bool existen_directorios;

	existen_directorios = i_verifica_directorios(directorio_base);

	if (existen_directorios == true)
	{
		marco_lanza_mensaje(marco, MARCO_MENSAJE_ERROR, "Ya existen todos los directorios necesarios.");
	}
	else
	{
		char *fuentes, *projects;

		i_directorios_completos_necesarios(directorio_base, &fuentes, &projects);

		nomfich_crear_directorios_si_no_existen(projects);
		nomfich_crear_directorios_si_no_existen(fuentes);

		strbas_destroy(&fuentes);
		strbas_destroy(&projects);
	}
}

//-----------------------------------------------------------------------

bool cfgproy_verifica_directorios(const char *directorio_base, char **mensaje_error_opc)
{
	bool existen_directorios;

	existen_directorios = i_verifica_directorios(directorio_base);

	if (existen_directorios == false)
	{
		if (mensaje_error_opc != NULL)
			*mensaje_error_opc = strbas_printf(
						"Faltan los directorios %s y %s para que merlin puede trabajar.\n"
						"Si desea que merlin los genere utilice la opcion -generar.",
						I_LIBS_SRC, I_PROJECTS);
	}

	return existen_directorios;
}

//-----------------------------------------------------------------------

static void i_anyade_incluidos(
						unsigned long num_recursividad,
						const struct fichsrc_t *fichero,
						const ArrTipo(proyecto_t) *todos_los_proyectos,
						const ArrTipo(fichsrc_t) *fichero_incluidos,
						ArrString *includes_no_encontrados_en_proyectos)
{
	const ArrString *ficheros_incluidos;
	unsigned long i, num;

	num_recursividad++;
	assert(num_recursividad > 0);

	ficheros_incluidos = fichsrc_incluidos(fichero);

	num = arrcad_numElementos(ficheros_incluidos);

	for (i = 0; i < num; i++)
	{
		const char *fichero_incluido;
		bool se_ha_encontrado;
		unsigned long ind_proyecto;

		fichero_incluido = arrcad_Get(ficheros_incluidos, i);

		se_ha_encontrado = array_BuscarUnica(
							proyecto_t, todos_los_proyectos,
							char, fichero_incluido,
							proyecto_fichero_pertenece_a_proyecto, &ind_proyecto);

		if (se_ha_encontrado == true)
		{
			bool existe_fichero;
			const struct proyecto_t *proyecto;
			struct fichsrc_t *fichero_encontrado;

			proyecto = array_Get(proyecto_t, todos_los_proyectos, ind_proyecto);

			existe_fichero = proyecto_existe_fichero(proyecto, fichero_incluido, &fichero_encontrado);
			assert(existe_fichero == true);

			if (array_BuscarUnica(
					fichsrc_t, fichero_incluidos,
					struct fichsrc_t, fichero_encontrado, NULL, NULL) == false)
			{
				array_AnyadirConst(fichsrc_t, fichero_incluidos, fichero_encontrado);

				i_anyade_incluidos(
								num_recursividad,
								fichero_encontrado,
								todos_los_proyectos,
								fichero_incluidos,
								includes_no_encontrados_en_proyectos);
			}
		}
		else
		{
			if (arrcad_Buscar(
						includes_no_encontrados_en_proyectos,
						fichero_incluido, strbas_equalsStrings, NULL) == false)
				arrcad_Anyadir(includes_no_encontrados_en_proyectos, fichero_incluido);
		}
	}
}

//-----------------------------------------------------------------------

static void i_ficheros_incluidos(
						const ArrTipo(fichsrc_t) *ficheros,
						const ArrTipo(proyecto_t) *todos_los_proyectos,
						const ArrTipo(fichsrc_t) **ficheros_incluidos,
						ArrString **includes_no_encontrados_en_proyectos)
{
	const ArrTipo(fichsrc_t) *ficheros_incluidos_loc;
	ArrString *includes_no_encontrados_en_proyectos_loc;
	unsigned long i, num;

	ficheros_incluidos_loc = array_CreaPunteroConst(0, fichsrc_t);
	includes_no_encontrados_en_proyectos_loc = arrcad_CreaString(0);

	num = array_numElementos(fichsrc_t, ficheros);

	for (i = 0; i < num; i++)
	{
		const struct fichsrc_t *fichero;

		fichero = array_Get(fichsrc_t, ficheros, i);
		i_anyade_incluidos(
						0, fichero, todos_los_proyectos, ficheros_incluidos_loc,
						includes_no_encontrados_en_proyectos_loc);
	}

	*ficheros_incluidos = ficheros_incluidos_loc;
	*includes_no_encontrados_en_proyectos = includes_no_encontrados_en_proyectos_loc;
}

//-----------------------------------------------------------------------

static struct genmak_t *i_crea_generator_mak(
						const char *dirBase,
						const ArrTipo(fichsrc_t) *ficheros_ejecutables,
						const ArrTipo(proyecto_t) *todos_los_proyectos,
						const class CGenerators *generatos,
						const char *nombre_base_proyecto)
{
	const ArrTipo(fichsrc_t) *ficheros_proyecto_include;
	struct genmak_t *generator_mak;
	unsigned long i, num;

	ficheros_proyecto_include = i_obtener_ficheros_proyecto(dirBase, todos_los_proyectos, I_INCLUDE);

	generator_mak = genmak_createDefault(nombre_base_proyecto, ficheros_proyecto_include);

	num = array_numElementos(fichsrc_t, ficheros_ejecutables);

	for (i = 0; i < num; i++)
	{
		const struct fichsrc_t *fileMain;
		const ArrTipo(fichsrc_t) *filesSource, *ficheros_incluidos;
		ArrString *includes_de_terceros, *includes_no_encontrados_en_proyectos;

		filesSource = array_CreaPunteroConst(0, fichsrc_t);
		includes_de_terceros = arrcad_CreaString(0);

		fileMain = array_Get(fichsrc_t, ficheros_ejecutables, i);

		i_obtener_ficheros_necesarios_para_linkar(
						fileMain,
						NULL, //fichero_include,
						generatos,
						todos_los_proyectos,
						filesSource,
						true, //se_incluyen_fuentes_de_dinamica
						NULL, //dlls_necesarias_opc
						includes_de_terceros);

		i_ficheros_incluidos(
						filesSource,
						todos_los_proyectos,
						&ficheros_incluidos,
						&includes_no_encontrados_en_proyectos);

		arrcad_Concatena(includes_de_terceros, includes_no_encontrados_en_proyectos);

		genmak_appendProgram(
						generator_mak,
						fileMain,
						&filesSource,
						&ficheros_incluidos,
						&includes_de_terceros);

		arrcad_Destruye(&includes_no_encontrados_en_proyectos);
	}

	return generator_mak;
}

//-----------------------------------------------------------------------

void cfgproy_genera_mak_proyecto(
						const struct cfgproy_t *cfg_proyectos,
						enum tipo_compilacion_t tipo_compilacion,
						const char *nombre_base_proyecto,
						struct marco_t *marco)
{
	unsigned long ind_proyecto;

	if (array_BuscarUnica(
						proyecto_t, cfg_proyectos->aplicaciones,
						char, nombre_base_proyecto,
						proyecto_es_nombre_base_proyecto, &ind_proyecto) == true)
	{
		const struct proyecto_t *aplicacion;
		const ArrTipo(fichsrc_t) *ficheros_ejecutables;
		const ArrTipo(proyecto_t) *todos_los_proyectos;
		const struct compilador_t *compilador;
		struct genmak_t *generator_mak;
		char *directorio_make;

		todos_los_proyectos = array_Copia(proyecto_t, cfg_proyectos->aplicaciones, NULL);
		array_ConcatenaConst(proyecto_t, todos_los_proyectos, cfg_proyectos->proyectos);

		aplicacion = array_Get(proyecto_t, cfg_proyectos->aplicaciones, ind_proyecto);

		compilador = i_obtener_compilador(
						tipo_compilacion,
						cfg_proyectos->compilador_rel,
						cfg_proyectos->compilador_deb,
						cfg_proyectos->compilador_prof,
						cfg_proyectos->compilador_lin);

		ficheros_ejecutables = array_CreaPunteroConst(0, fichsrc_t);
		proyecto_append_ejecutables_aplicacion(aplicacion, ficheros_ejecutables);

		generator_mak = i_crea_generator_mak(
                        cfg_proyectos->directorio_base,
                        ficheros_ejecutables, todos_los_proyectos,
                        cfg_proyectos->generators, nombre_base_proyecto);

		directorio_make = nomfich_append_a_directorio(cfg_proyectos->directorio_base, I_MAKE);

		genmak_copyFiles(generator_mak, directorio_make, marco);
		genmak_generateMak(generator_mak, compilador, directorio_make);

		strbas_destroy(&directorio_make);

		array_DestruyeConst(fichsrc_t, &ficheros_ejecutables);
		array_DestruyeConst(proyecto_t, &todos_los_proyectos);
		genmak_destroy(&generator_mak);
	}
	else
	{
		char *mensaje_error;

		mensaje_error = strbas_printf("No existe el proyecto %s", nombre_base_proyecto);
		marco_lanza_mensaje(marco, MARCO_MENSAJE_ERROR, mensaje_error);
		strbas_destroy(&mensaje_error);
	}
}
