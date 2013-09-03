// Definici�n del compilador

#include "compilador.inl"

#include "fichsrc.inl"

#include "memory.h"
#include "strbas.h"
#include "asrtbas.h"
#include "nomfich.h"
#include "config.h"
#include "arrcad.h"
#include "objsdir.h"
#include "marco.h"

struct compilador_t
{
	unsigned long nivel_salida;
    
    bool soporta_cpp;
	
	char *ext_objs, *ext_exes, *ext_dlls;
	char *directorio_base;
	char *flag_objs;
	char *flag_include;
	char *flags;
	char *flags_linker;
	char *compilador;
	char *linker;
	struct config_t *config_compilador;
	struct config_t *config_linker;
};

//-----------------------------------------------------------------------

static void i_integridad(const struct compilador_t *compilador)
{
	assert_no_null(compilador);
	
	assert(compilador->nivel_salida > 0);
	assert_no_null(compilador->ext_objs);
	assert_no_null(compilador->ext_exes);
	assert_no_null(compilador->ext_dlls);
	assert_no_null(compilador->directorio_base);
	assert_no_null(compilador->flag_objs);
	assert_no_null(compilador->flag_include);
	assert_no_null(compilador->flags);
	assert_no_null(compilador->flags_linker);
	assert_no_null(compilador->compilador);
	assert_no_null(compilador->linker);
	assert_no_null(compilador->config_compilador);
	assert_no_null(compilador->config_linker);
}

//-----------------------------------------------------------------------

static struct compilador_t *i_crea(
				unsigned long nivel_salida,
                bool soporta_cpp,
				const char *ext_objs,
				const char *ext_exes,
				const char *ext_dlls,
				const char *directorio_base,
				const char *flag_objs,
				const char *flag_include,
				const char *flags,
				const char *flags_linker,
				const char *comando_compilador,
				const char *linker,
				const struct config_t *config_compilador,
				const struct config_t *config_linker)
{
	struct compilador_t *compilador;
		
	compilador = MALLOC(struct compilador_t);

	compilador->nivel_salida = nivel_salida;
    compilador->soporta_cpp = soporta_cpp;
	compilador->ext_exes = strbas_copyString(ext_exes);
	compilador->ext_objs = strbas_copyString(ext_objs);
	compilador->ext_dlls = strbas_copyString(ext_dlls);
	compilador->directorio_base = strbas_copyString(directorio_base);
	compilador->flag_objs = strbas_copyString(flag_objs);
	compilador->flag_include = strbas_copyString(flag_include);
	compilador->flags = strbas_copyString(flags);
	compilador->flags_linker = strbas_copyString(flags_linker);
	compilador->compilador = strbas_copyString(comando_compilador);
	compilador->linker = strbas_copyString(linker);
	compilador->config_compilador = config_copia(config_compilador);
	compilador->config_linker = config_copia(config_linker);
	
	i_integridad(compilador);
	
	return compilador;
}

//-----------------------------------------------------------------------

static void i_nombres_ficheros_configuracion(
                        const char *prefijo_compilador, 
                        const char *prefijo_sistema_operativo,
                        char **nom_fichero_conf_compiler,
                        char **nom_fichero_conf_linker)
{
    assert_no_null(nom_fichero_conf_compiler);
    assert_no_null(nom_fichero_conf_linker);

    *nom_fichero_conf_compiler = strbas_printf("compiler.%s.%s", prefijo_compilador, prefijo_sistema_operativo);
    *nom_fichero_conf_linker = strbas_printf("linker.%s.%s", prefijo_compilador, prefijo_sistema_operativo);
}

//-----------------------------------------------------------------------

static void i_lee_configuraciones(
						const char *directorio_inicio,
                        const char *prefijo_compilador, 
                        const char *prefijo_sistema_operativo,
						struct config_t **config_compilador,
						struct config_t **config_linker)
{
	char *nombre_configuracion_compilador, *nombre_configuracion_linker;
    char *nombre_compiler, *nombre_linker;
	
	assert_no_null(config_compilador);
	assert_no_null(config_linker);

    i_nombres_ficheros_configuracion(prefijo_compilador, prefijo_sistema_operativo, &nombre_compiler, &nombre_linker);
	
	nombre_configuracion_compilador = nomfich_append_a_directorio(directorio_inicio, nombre_compiler);
	nombre_configuracion_linker = nomfich_append_a_directorio(directorio_inicio, nombre_linker);

	*config_compilador = config_lee_configuracion(nombre_configuracion_compilador);
	*config_linker = config_lee_configuracion(nombre_configuracion_linker);
	
	strbas_destroy(&nombre_configuracion_compilador);
	strbas_destroy(&nombre_configuracion_linker);
	strbas_destroy(&nombre_compiler);
	strbas_destroy(&nombre_linker);
}

//-----------------------------------------------------------------------

struct compilador_t *compilador_crea_rel(
                        const char *directorio_inicio, 
                        const char *ext_exes,
                        const char *ext_dlls,
                        const char *prefijo_sistema_operativo)
{
	struct compilador_t *compilador_rel;
	
	const char *flag_objs, *flag_include, *flags, *flags_linker, *compilador, *directorio_base, *ext_objs, *linker;
	unsigned long nivel_salida;
    bool soporta_cpp;
	struct config_t *config_compilador;
	struct config_t *config_linker;
	
	nivel_salida = 2;
    soporta_cpp = true;
	ext_objs = "o";
	directorio_base = "rel";
	flag_objs = "-o";
	flag_include = "-include"; 
	flags = "-c -Wall -fmessage-length=0";
	flags_linker = "";
	compilador = "g++";
	linker = "g++";
	
	i_lee_configuraciones(directorio_inicio, "rel", prefijo_sistema_operativo, &config_compilador, &config_linker);
	
	compilador_rel = i_crea(
				nivel_salida, 
				soporta_cpp, 
				ext_objs, 
				ext_exes,
				ext_dlls,
				directorio_base, flag_objs, flag_include, 
				flags, flags_linker, compilador, linker, config_compilador, config_linker);
	
	config_destruye(&config_compilador);
	config_destruye(&config_linker);
	
	return compilador_rel;
}

//-----------------------------------------------------------------------

struct compilador_t *compilador_crea_deb(
                        const char *directorio_inicio, 
                        const char *ext_exes, 
        				const char *ext_dlls,
                        const char *prefijo_sistema_operativo)
{
	struct compilador_t *compilador_deb;
	
	const char *flag_objs, *flag_include, *flags, *flags_linker, *compilador, *directorio_base, *ext_objs, *linker;
	unsigned long nivel_salida;
	struct config_t *config_compilador;
	struct config_t *config_linker;
    bool soporta_cpp;
	
	nivel_salida = 2;
    soporta_cpp = true;
	ext_objs = "o";
	flag_objs = "-o";
	directorio_base = "deb";
	flag_include = "-include"; 
	flags = "-c -g -Wall -fmessage-length=0";
	flags_linker = "";
	compilador = "g++";
	linker = "g++";

	i_lee_configuraciones(directorio_inicio, "deb", prefijo_sistema_operativo, &config_compilador, &config_linker);

	compilador_deb = i_crea(
				nivel_salida, soporta_cpp, ext_objs, ext_exes, ext_dlls,
				directorio_base, flag_objs, flag_include, 
				flags, flags_linker, compilador, linker,
				config_compilador, config_linker);
	
	config_destruye(&config_compilador);
	config_destruye(&config_linker);
	
	return compilador_deb;
}

//-----------------------------------------------------------------------

struct compilador_t *compilador_crea_prof(
                        const char *directorio_inicio, 
                        const char *ext_exes, 
        				const char *ext_dlls,
                        const char *prefijo_sistema_operativo)
{
	struct compilador_t *compilador_deb;
	
	const char *flag_objs, *flag_include, *flags, *flags_linker, *compilador, *directorio_base, *ext_objs, *linker;
	unsigned long nivel_salida;
	struct config_t *config_compilador;
	struct config_t *config_linker;
    bool soporta_cpp;
	
	nivel_salida = 2;
    soporta_cpp = true;
	ext_objs = "o";
	flag_objs = "-o";
	directorio_base = "prof";
	flag_include = "-include"; 
	flags = "-c -g -pg -Wall -fmessage-length=0";
	flags_linker = "-pg";
	compilador = "g++";
	linker = "g++";

	i_lee_configuraciones(directorio_inicio, "deb", prefijo_sistema_operativo, &config_compilador, &config_linker);

	compilador_deb = i_crea(
				nivel_salida, soporta_cpp, ext_objs, ext_exes, ext_dlls,
				directorio_base, flag_objs, flag_include, 
				flags, flags_linker, compilador, linker,
				config_compilador, config_linker);
	
	config_destruye(&config_compilador);
	config_destruye(&config_linker);
	
	return compilador_deb;
}

//-----------------------------------------------------------------------

struct compilador_t *compilador_crea_lin(
                        const char *directorio_inicio, 
                        const char *ext_exes, 
        				const char *ext_dlls,
                        const char *prefijo_sistema_operativo)
{
	struct compilador_t *compilador_lin;
	
	const char *flag_objs, *flag_include, *flags_linker, *compilador, *directorio_base, *ext_objs, *linker;
	unsigned long nivel_salida;
	char *flags, *path_cfg;
	struct config_t *config_compilador;
	struct config_t *config_linker;
    bool soporta_cpp;
	
	nivel_salida = 1;
    soporta_cpp = false;
	flag_objs = "-dump";
	ext_objs = "o";
	directorio_base = "lin";
	flag_include = ""; 
	flags_linker = "";
	
	path_cfg = nomfich_append_a_directorio(directorio_inicio, "lin.cfg");
	
	if (nomfich_existe_fichero(path_cfg) == true)
		flags = strbas_printf("-syntax -f %s", path_cfg);
	else
		flags = strbas_copyString("-syntax -warnunixlib +unix-lib");
	
	compilador = "splint";
	linker = "splint";

	i_lee_configuraciones(directorio_inicio, "lin", prefijo_sistema_operativo, &config_compilador, &config_linker);

	compilador_lin = i_crea(
				nivel_salida, soporta_cpp, ext_objs, ext_exes, ext_dlls, directorio_base, 
				flag_objs, flag_include, flags, flags_linker, compilador, linker,
				config_compilador, config_linker);
	
	strbas_destroy(&flags);
	strbas_destroy(&path_cfg);

	config_destruye(&config_compilador);
	config_destruye(&config_linker);
	
	return compilador_lin;
}

//-----------------------------------------------------------------------

void compilador_destruye(struct compilador_t **compilador)
{
	assert_no_null(compilador);
	i_integridad(*compilador);
	
	strbas_destroy(&(*compilador)->ext_objs);
	strbas_destroy(&(*compilador)->ext_exes);
	strbas_destroy(&(*compilador)->ext_dlls);
	strbas_destroy(&(*compilador)->directorio_base);
	strbas_destroy(&(*compilador)->flag_include);
	strbas_destroy(&(*compilador)->flags);
	strbas_destroy(&(*compilador)->flags_linker);
	strbas_destroy(&(*compilador)->flag_objs);
	strbas_destroy(&(*compilador)->compilador);
	strbas_destroy(&(*compilador)->linker);

	config_destruye(&(*compilador)->config_compilador);
	config_destruye(&(*compilador)->config_linker);
	
	FREE_T(compilador, struct compilador_t);
}

//-----------------------------------------------------------------------

static char *i_directorio_salida(
						const char *directorio_base, 
						const char *directorio_base_compilacion,
						const char *directorio_salida)
{
	char *nombre_tmp, *nombre_salida;

	nombre_tmp = nomfich_append_a_directorio(directorio_base, directorio_salida);
	nombre_salida = nomfich_append_a_directorio(nombre_tmp, directorio_base_compilacion);
	strbas_destroy(&nombre_tmp);
	
	return nombre_salida;
}

//-----------------------------------------------------------------------

static ArrString *i_obtener_flags_adicionales(
						const struct config_t *configuracion,
						const ArrString *includes_de_terceros)
{
	unsigned long num_includes, i;
	ArrString *flags_adicionales;
	
	num_includes = arrcad_numElementos(includes_de_terceros);
	flags_adicionales = arrcad_CreaString(0);
	
	for (i = 0; i < num_includes; i++)
	{
		const char *include;
		char *nombre_base;
		
		include = arrcad_Get(includes_de_terceros, i);
		nombre_base = nomfich_nombre_base_con_extension(include);
		
		if (config_existe_variable(configuracion, nombre_base) == true)
		{
			ArrString *valores;
				
			valores = config_obtener_valor(configuracion, nombre_base);
			arrcad_Concatena(flags_adicionales, valores);
			arrcad_Destruye(&valores);
		}
		
		strbas_destroy(&nombre_base);
	}
	
	return flags_adicionales;
}

//-----------------------------------------------------------------------

void compilador_compilar(
						const struct compilador_t *compilador,
						struct marco_t *marco,
						struct fichsrc_t *fichero,
						const ArrTipo(fichsrc_t) *ficheros_proyecto_include,
						const ArrString *includes_necesarios,
						const ArrString *includes_de_terceros,
						const char *directorio_base,
						const char *subdirectorio_objs,
                        const char *subdirectorio_errores)
{
	char *directorio_objs, *directorio_errs;
	ArrString *flags_adicionales;
    bool compilar;
	
	i_integridad(compilador);

	directorio_objs = i_directorio_salida(directorio_base, compilador->directorio_base, subdirectorio_objs);
	directorio_errs = i_directorio_salida(directorio_base, compilador->directorio_base, subdirectorio_errores);
	
	flags_adicionales = i_obtener_flags_adicionales(compilador->config_compilador, includes_de_terceros);
    
    if (fichscr_es_cpp(fichero) == true && compilador->soporta_cpp == false)
        compilar = false;
    else
        compilar = true;
        
    if (compilar == true)
        fichsrc_compilar(
                        fichero, marco, ficheros_proyecto_include, 
                        compilador->nivel_salida,
                        compilador->compilador, 
                        compilador->flag_objs,
                        compilador->flags, 
                        compilador->flag_include, 
                        includes_necesarios, 
                        directorio_errs, directorio_objs, 
                        compilador->ext_objs, flags_adicionales);
	
	arrcad_Destruye(&flags_adicionales);
	strbas_destroy(&directorio_objs);
	strbas_destroy(&directorio_errs);
}

//-----------------------------------------------------------------------

static char *i_obtener_flags_proyecto_include(
						const ArrString *ficheros_include,
						const char *flag_include)
{
	char *flags;
	unsigned long i, num_ficheros;

	flags = strbas_copyString("");

	num_ficheros = arrcad_numElementos(ficheros_include);

	for (i = 0; i < num_ficheros; i++)
	{
		const char *fichero_include;
		char *flag;

		fichero_include = arrcad_Get(ficheros_include, i);
		if (i > 0)
			flag = strbas_printf(" %s %s", flag_include, fichero_include);
		else
			flag = strbas_printf("%s %s", flag_include, fichero_include);

		strbas_concatenate(&flags, flag);

		strbas_destroy(&flag);
	}

	return flags;
}

//-----------------------------------------------------------------------

char *compilador_lineCommandMake(
						const struct compilador_t *compilador,
						const ArrString *includes_de_terceros,
						const ArrString *ficheros_include)
{
	char *lineCommand, *strFlagsAdicionales, *strInclude;
	ArrString *flags_adicionales;

	i_integridad(compilador);

	flags_adicionales = i_obtener_flags_adicionales(compilador->config_compilador, includes_de_terceros);
	strInclude = i_obtener_flags_proyecto_include(ficheros_include, compilador->flag_include);
	strFlagsAdicionales = arrcad_texto_separado_por_espacios(flags_adicionales);

	lineCommand = strbas_printf("%s %s %s", compilador->flags, strFlagsAdicionales, strInclude);

	arrcad_Destruye(&flags_adicionales);
	strbas_destroy(&strFlagsAdicionales);
	strbas_destroy(&strInclude);

	return lineCommand;
}

//-----------------------------------------------------------------------

bool compilador_es_compilable(
					const struct compilador_t *compilador,
					const struct fichsrc_t *fichero,
					const char *directorio_base,
					const char *subdirectorio_objs)
{
	bool es_compilable;
	char *directorio_objs;
	
	i_integridad(compilador);
	
	directorio_objs = i_directorio_salida(directorio_base, compilador->directorio_base, subdirectorio_objs);
	
	if (fichsrc_tiene_obj(fichero) == true
		&& fichsrc_es_compilable(fichero, directorio_objs, compilador->ext_objs) == true)
		es_compilable = true;
	else
		es_compilable = false;
	
	strbas_destroy(&directorio_objs);
	
	return es_compilable;
}

//-----------------------------------------------------------------------

bool compilador_es_compilable_fichero_incluido(
					const struct compilador_t *compilador,
					const struct fichsrc_t *fichero,
					const struct fichsrc_t *fichero_include,
					const char *directorio_base,
					const char *subdirectorio_objs)
{
	bool es_compilable;
	char *directorio_objs;
	
	i_integridad(compilador);
	
	directorio_objs = i_directorio_salida(directorio_base, compilador->directorio_base, subdirectorio_objs);
	
	if (fichsrc_tiene_obj(fichero) == true)
	{
		es_compilable = fichsrc_es_compilable_fichero_incluido(
					fichero, fichero_include, 
					directorio_objs, compilador->ext_objs);
	}
	else
		es_compilable = false;
	
	strbas_destroy(&directorio_objs);
	
	return es_compilable;
}

//-----------------------------------------------------------------------

char *compilador_nombre_ejecutable(
					const struct compilador_t *compilador,
					const struct fichsrc_t *ejecutable)
{
	i_integridad(compilador);
	return fichsrc_nombre_ejecutable(ejecutable, compilador->ext_exes);
}

//-----------------------------------------------------------------------

char *compilador_nombre_obj(
					const struct compilador_t *compilador,
					const struct fichsrc_t *fichero_fuente)
{
	i_integridad(compilador);
	return fichsrc_nombre_obj(fichero_fuente, compilador->ext_objs);
}


//-----------------------------------------------------------------------

const char *compilador_nombre_compilador(const struct compilador_t *compilador)
{
	i_integridad(compilador);
	return compilador->compilador;
}

//-----------------------------------------------------------------------

const char *compilador_nombre_linker(const struct compilador_t *compilador)
{
	i_integridad(compilador);
	return compilador->linker;
}

//-----------------------------------------------------------------------

static char *i_nombre_dll(const char *nombre_proyecto, const char *ext_dlls)
{
	return strbas_printf("lib%s.%s", nombre_proyecto, ext_dlls);
}

//-----------------------------------------------------------------------

static ArrString *i_paths_dlls(
							const char *directorio_dlls,
							const char *ext_dlls,
							const ArrString *dlls_nombre_base_necesarias)
{
	ArrString *dlls_paths;
	unsigned long i, num;
	
	num = arrcad_numElementos(dlls_nombre_base_necesarias);
	dlls_paths = arrcad_CreaString(num);
	
	for (i = 0; i < num; i++)
	{
		const char *nombre_base_dll;
		char *nombre_libreria, *path_libreria;
		
		nombre_base_dll = arrcad_Get(dlls_nombre_base_necesarias, i);
		
		nombre_libreria = i_nombre_dll(nombre_base_dll, ext_dlls);
		path_libreria = nomfich_append_a_directorio(directorio_dlls, nombre_libreria);
		
		arrcad_SetDestruyendoCadena(dlls_paths, i, &path_libreria);
		
		strbas_destroy(&nombre_libreria);
	}
	
	return dlls_paths;
}

//-----------------------------------------------------------------------

static ArrString *i_dlls_con_flags(const ArrString *dlls_nombre_base_necesarias)
{
	ArrString *dlls_con_flags;
	unsigned long i, num;
	
	num = arrcad_numElementos(dlls_nombre_base_necesarias);
	dlls_con_flags = arrcad_CreaString(num);
	
	for (i = 0; i < num; i++)
	{
		const char *nombre_base_dll;
		char *dll_con_flag;
		
		nombre_base_dll = arrcad_Get(dlls_nombre_base_necesarias, i);
		dll_con_flag = strbas_printf("-l%s", nombre_base_dll);
		arrcad_SetDestruyendoCadena(dlls_con_flags, i, &dll_con_flag);
	}
	
	return dlls_con_flags;
}

//-----------------------------------------------------------------------

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
					struct marco_t *marco)
{
	char *directorio_objs, *directorio_exes, *directorio_dlls;
	ArrString *flags_adicionales, *dlls_path, *dlls_con_flags;

	i_integridad(compilador);

	directorio_objs = i_directorio_salida(directorio_base, compilador->directorio_base, subdirectorio_objs);
	directorio_dlls = i_directorio_salida(directorio_base, compilador->directorio_base, subdirectorio_dlls);
	directorio_exes = i_directorio_salida(directorio_base, compilador->directorio_base, directorio_salida_ejecutables);

	flags_adicionales = i_obtener_flags_adicionales(compilador->config_linker, includes_de_terceros);
	dlls_path = i_paths_dlls(directorio_dlls, compilador->ext_dlls, dlls_nombre_base_necesarias);
	dlls_con_flags = i_dlls_con_flags(dlls_nombre_base_necesarias);

	fichsrc_linkar(
				ejecutable, ficheros_necesarios,
				directorio_objs, compilador->ext_objs,
				directorio_exes,
				directorio_dlls,
				compilador->ext_exes,
				compilador->linker,
				compilador->flags_linker,
				flags_adicionales, dlls_con_flags,
				dlls_path, marco);

	arrcad_Destruye(&flags_adicionales);
	arrcad_Destruye(&dlls_path);
	arrcad_Destruye(&dlls_con_flags);

	strbas_destroy(&directorio_dlls);
	strbas_destroy(&directorio_objs);
	strbas_destroy(&directorio_exes);
}

//-----------------------------------------------------------------------

char *compilador_lineCommandLinkerMake(
					const struct compilador_t *compilador,
					const ArrString *includes_de_terceros)
{
	char *commandLinker;
	ArrString *flags_adicionales;
	char *strFlagsAdicionales;

	i_integridad(compilador);

	flags_adicionales = i_obtener_flags_adicionales(compilador->config_linker, includes_de_terceros);
	strFlagsAdicionales = arrcad_texto_separado_por_espacios(flags_adicionales);

	commandLinker = strbas_printf("%s %s", compilador->flags_linker, strFlagsAdicionales);

	arrcad_Destruye(&flags_adicionales);
	strbas_destroy(&strFlagsAdicionales);

	return commandLinker;
}

//-----------------------------------------------------------------------

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
                    struct marco_t *marco)
{
    char *directorio_objs, *directorio_dlls, *directorio_plugins, *directorio_errores;
    ArrString *flags_adicionales, *dlls_path, *dlls_con_flags;

    i_integridad(compilador);

    directorio_objs = i_directorio_salida(directorio_base, compilador->directorio_base, subdirectorio_objs);
    directorio_plugins = i_directorio_salida(directorio_base, compilador->directorio_base, subdirectorio_plugins);
    directorio_errores = i_directorio_salida(directorio_base, compilador->directorio_base, subdirectorio_errores);
    directorio_dlls = i_directorio_salida(directorio_base, compilador->directorio_base, subdirectorio_dlls);

    flags_adicionales = i_obtener_flags_adicionales(compilador->config_linker, includes_de_terceros);
    dlls_path = i_paths_dlls(directorio_dlls, compilador->ext_dlls, dlls_nombre_base_necesarias);
    dlls_con_flags = i_dlls_con_flags(dlls_nombre_base_necesarias);

    fichsrc_linkar_plugins(
                    ficheros_del_proyecto,
                    nombre_proyecto,
                    compilador->ext_objs,
                    directorio_objs,
                    directorio_plugins,
                    directorio_errores,
                    compilador->ext_dlls,
                    compilador->linker,
                    "-fPIC -shared", //flags_linker
                    flags_adicionales,
                    dlls_con_flags, dlls_path, marco);

    arrcad_Destruye(&flags_adicionales);
    arrcad_Destruye(&dlls_path);
    arrcad_Destruye(&dlls_con_flags);

    strbas_destroy(&directorio_objs);
    strbas_destroy(&directorio_dlls);
    strbas_destroy(&directorio_plugins);
    strbas_destroy(&directorio_errores);
}

//-----------------------------------------------------------------------

void compilador_linkado_dinamico(
					const struct compilador_t *compilador, 
					const char *nombre_proyecto,
					const char *directorio_base,
					const char *subdirectorio_objs,
                    const char *subdirectorio_dlls,
					const ArrTipo(fichsrc_t) *ficheros_del_proyecto,
					struct marco_t *marco)
{
	char *nombre_libreria, *directorio_objs, *directorio_dlls;
	
	i_integridad(compilador);
	
	nombre_libreria = i_nombre_dll(nombre_proyecto, compilador->ext_dlls); 
	directorio_objs = i_directorio_salida(directorio_base, compilador->directorio_base, subdirectorio_objs);
	directorio_dlls = i_directorio_salida(directorio_base, compilador->directorio_base, subdirectorio_dlls);
	
	fichsrc_linkar_dll(
					ficheros_del_proyecto,
					nombre_libreria,
					compilador->ext_objs,
					directorio_objs,
					directorio_dlls,
					compilador->ext_dlls,
					compilador->linker,
					"-fPIC -shared", //flags_linker
					marco);
	
	strbas_destroy(&nombre_libreria);
	strbas_destroy(&directorio_objs);
	strbas_destroy(&directorio_dlls);
}


//-----------------------------------------------------------------------

static void i_borra_fichero(const ArrString *nombres_ficheros)
{
	unsigned long i, num;

	num = arrcad_numElementos(nombres_ficheros);

	for (i = 0; i < num; i++)
	{
		const char *nombre_fichero;

		nombre_fichero = arrcad_Get(nombres_ficheros, i);
		nomfich_borrar_fichero(nombre_fichero);
	}
}

//-----------------------------------------------------------------------

static void i_borra_los_directorios_con_ficheros(unsigned long num_recursividad, const char *directorio)
{
	struct objsdir_t *objs_directorio;
	ArrString *nombres_subdirectorios, *nombre_ficheros;
	unsigned long i, num;

	num_recursividad++;
	assert(num_recursividad < 100);

	objs_directorio = objsdir_lee_path(directorio);

	nombre_ficheros = objsdir_obtener_ficheros(objs_directorio);
	i_borra_fichero(nombre_ficheros);
	arrcad_Destruye(&nombre_ficheros);

	nombres_subdirectorios = objsdir_obtener_directorios(objs_directorio);

	num = arrcad_numElementos(nombres_subdirectorios);

	for (i = 0; i < num; i++)
	{
		const char *nombre_subdirectorio;

		nombre_subdirectorio = arrcad_Get(nombres_subdirectorios, i);
		i_borra_los_directorios_con_ficheros(num_recursividad, nombre_subdirectorio);
	}

	objsdir_destruye(&objs_directorio);
	arrcad_Destruye(&nombres_subdirectorios);

	nomfich_borrar_directorio(directorio);
}

//-----------------------------------------------------------------------

static void i_mensaje_limpiando(const char *directorio, struct marco_t *marco)
{
	char *txt_mensaje;

	txt_mensaje = strbas_printf("Limpiando %s....", directorio);
	marco_lanza_mensaje(marco, MARCO_MENSAJE_INFORMACION, txt_mensaje);
	strbas_destroy(&txt_mensaje);
}

//-----------------------------------------------------------------------

static void i_limpiar_directorio(
							const char *directorio_base,
							const char *directorio_base_compilacion,
							const char *directorio_salida,
							struct marco_t *marco)
{
	char *directorio_compilacion, *directorio_recurso;

	directorio_compilacion = i_directorio_salida(directorio_base, directorio_base_compilacion, directorio_salida);

	if (nomfich_existe_directorio(directorio_compilacion) == true)
	{
		i_mensaje_limpiando(directorio_compilacion, marco);
		i_borra_los_directorios_con_ficheros(0, directorio_compilacion);
	}

	directorio_recurso = nomfich_append_a_directorio(directorio_base, directorio_salida);

	if (nomfich_existe_directorio(directorio_recurso) == true && objsdir_es_directorio_vacio(directorio_recurso) == true)
		nomfich_borrar_directorio(directorio_recurso);

	strbas_destroy(&directorio_recurso);
	strbas_destroy(&directorio_compilacion);
}

//-----------------------------------------------------------------------

static void i_limpiar_directorio_aplicaciones(
							const char *directorio_base,
							const char *directorio_base_compilacion,
							const char *directorio_salida,
							struct marco_t *marco)
{
	struct objsdir_t *objs_directorio;
	ArrString *nombre_directorios_aplicaciones;
	unsigned long i, num;
	char *directorio_aplicaciones;

	directorio_aplicaciones = nomfich_append_a_directorio(directorio_base, directorio_salida);

	if (nomfich_existe_directorio(directorio_aplicaciones) == true)
	{
		objs_directorio = objsdir_lee_path(directorio_aplicaciones);

		nombre_directorios_aplicaciones = objsdir_obtener_directorios(objs_directorio);

		num = arrcad_numElementos(nombre_directorios_aplicaciones);

		for (i = 0; i < num; i++)
		{
			const char *nombre_aplicaciones;
			char *nombre_directorio_con_compilacion;

			nombre_aplicaciones = arrcad_Get(nombre_directorios_aplicaciones, i);
			nombre_directorio_con_compilacion = nomfich_append_a_directorio(nombre_aplicaciones, directorio_base_compilacion);

			i_mensaje_limpiando(nombre_directorio_con_compilacion, marco);

			i_borra_los_directorios_con_ficheros(0, nombre_directorio_con_compilacion);

			if (objsdir_es_directorio_vacio(nombre_aplicaciones) == true)
				nomfich_borrar_directorio(nombre_aplicaciones);

			strbas_destroy(&nombre_directorio_con_compilacion);
		}

		objsdir_destruye(&objs_directorio);
		arrcad_Destruye(&nombre_directorios_aplicaciones);

		if (nomfich_existe_directorio(directorio_aplicaciones) == true
				&& objsdir_es_directorio_vacio(directorio_aplicaciones) == true)
			nomfich_borrar_directorio(directorio_aplicaciones);
	}

	strbas_destroy(&directorio_aplicaciones);
}

//-----------------------------------------------------------------------

void compilador_clean(
				const struct compilador_t *compilador,
				const char *directorio_base,
                const char *subdirectorio_objs,
                const char *subdirectorio_dlls,
                const char *subdirectorio_bin_projects,
                const char *subdirectorio_errores,
				struct marco_t *marco)
{
	i_integridad(compilador);

	i_limpiar_directorio(directorio_base, compilador->directorio_base, subdirectorio_objs, marco);
	i_limpiar_directorio(directorio_base, compilador->directorio_base, subdirectorio_errores, marco);
	i_limpiar_directorio_aplicaciones(directorio_base, compilador->directorio_base, subdirectorio_bin_projects, marco);
	i_limpiar_directorio(directorio_base, compilador->directorio_base, subdirectorio_dlls, marco);
}
