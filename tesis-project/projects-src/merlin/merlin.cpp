#include "cfgproy.inl"

#include "aplicac.h"
#include "opcion.h"
#include "memory.h"
#include "array.h"
#include "arrcad.h"
#include "strbas.h"
#include "gesenum.h"
#include "asrtbas.h"
#include "marco.h"
#include "nomfich.h"
#include "stdio.h"

#include "filetxt.hpp"
#include "marcogtk.hpp"

static const char *i_VERSION_MERLIN = "v 1.5";

enum i_hacer_t
{
	i_HACER_ALL,
	i_HACER_CLEAN,
	i_HACER_NORMAL
};

struct i_datos_aplicacion_t
{
	ArrString *parametros;
	char *directorio_base;

	enum i_hacer_t tipo_hacer;
};

//-----------------------------------------------------------------------

static struct cfgproy_t *i_genera_proyectos(const char *directorio_base)
{
	struct cfgproy_t *cfg_proyectos;
    enum gesenum_sistema_operativo_t sistema_operativo;

#ifdef _WINDOWS_
    sistema_operativo = GESENUM_COMPILADOR_WINDOWS;
#else
    sistema_operativo = GESENUM_COMPILADOR_LINUX;
#endif

	cfg_proyectos = cfgproy_crea_defecto(directorio_base, sistema_operativo);
	cfgproy_generar_proyectos(cfg_proyectos);

	return cfg_proyectos;
}

//-----------------------------------------------------------------------

static struct i_datos_aplicacion_t *i_crea(
							ArrString **parametros,
							char **directorio_base,
							enum i_hacer_t tipo_hacer)
{
	struct i_datos_aplicacion_t *datos_aplicacion;
		
	datos_aplicacion = MALLOC(struct i_datos_aplicacion_t);
		
	datos_aplicacion->parametros = ASSIGN_PP_NO_NULL(parametros,  ArrString);
	datos_aplicacion->directorio_base = ASSIGN_PP_NO_NULL(directorio_base,  char);
	datos_aplicacion->tipo_hacer = tipo_hacer;
		
	return datos_aplicacion;
}

//-----------------------------------------------------------------------

static void i_destruye(struct i_datos_aplicacion_t **datos_aplicacion)
{
	assert_no_null(datos_aplicacion);
	assert_no_null(*datos_aplicacion);
	
	arrcad_Destruye(&(*datos_aplicacion)->parametros);
	strbas_destroy(&(*datos_aplicacion)->directorio_base);
	
	FREE_T(datos_aplicacion, struct i_datos_aplicacion_t);
}

//-----------------------------------------------------------------------

static bool i_hay_directorio_base(const struct i_datos_aplicacion_t *datos_aplicacion, char **mensaje_error)
{
	assert_no_null(datos_aplicacion);

	if (strbas_equalsStrings(datos_aplicacion->directorio_base, "") == true)
	{
		*mensaje_error = strbas_copyString(
								"Debemos definir la variable del sistema MERLIN_PROJECTS\n"
								"como el directorio donde están los proyectos a compilar o\n"
								"poner el directorio como parámetro");
		return false;
	}
	else
		return true;
}

//-----------------------------------------------------------------------

static bool i_hay_componentes_necesarios(const struct i_datos_aplicacion_t *datos_aplicacion, char **mensaje_error)
{
	assert_no_null(datos_aplicacion);

	if (i_hay_directorio_base(datos_aplicacion, mensaje_error) == true)
		return cfgproy_verifica_directorios(datos_aplicacion->directorio_base, mensaje_error);
	else
		return false;
}

//-----------------------------------------------------------------------

static void i_debug(struct marco_t *marco, struct i_datos_aplicacion_t *datos_aplicacion)
{
	struct cfgproy_t *cfg_proyectos;

	assert_no_null(datos_aplicacion);

	cfg_proyectos = i_genera_proyectos(datos_aplicacion->directorio_base);
	cfgproy_debug(cfg_proyectos, marco);
	cfgproy_destruye(&cfg_proyectos);
}

//-----------------------------------------------------------------------

static void i_es_incluido(struct marco_t *marco, struct i_datos_aplicacion_t *datos_aplicacion, const char *fichero)
{
	ArrString *mensajes;
	struct cfgproy_t *cfg_proyectos;
	
	assert_no_null(datos_aplicacion);
	
	cfg_proyectos = i_genera_proyectos(datos_aplicacion->directorio_base);

	mensajes = cfgproy_es_incluido(cfg_proyectos, fichero);
	marco_lanza_explicacion(marco, MARCO_MENSAJE_INFORMACION, mensajes);

	arrcad_Destruye(&mensajes);
	cfgproy_destruye(&cfg_proyectos);
}

//-----------------------------------------------------------------------

static void i_dependencias_proyectos(struct marco_t *marco,  struct i_datos_aplicacion_t *datos_aplicacion,  const char *nombre_proyecto)
{
	ArrString *nombres_proyectos;
	struct cfgproy_t *cfg_proyectos;
	
	assert_no_null(datos_aplicacion);
	
	cfg_proyectos = i_genera_proyectos(datos_aplicacion->directorio_base);
    nombres_proyectos = arrcad_CreaString(0);
		
	if (cfgproy_dependencias_proyectos(cfg_proyectos, nombre_proyecto, &nombres_proyectos) == false)
	{
		char *mensaje;
		
		mensaje = strbas_printf("No existe el proyecto %s", nombre_proyecto);
		arrcad_Anyadir(nombres_proyectos, mensaje);
		strbas_destroy(&mensaje);
	}
	
	marco_lanza_explicacion(marco, MARCO_MENSAJE_INFORMACION, nombres_proyectos);
	
	arrcad_Destruye(&nombres_proyectos);
	cfgproy_destruye(&cfg_proyectos);
}

//-----------------------------------------------------------------------

static bool i_es_version_compilacion(const char *parametro, enum tipo_compilacion_t *tipo_compilacion, char **mensaje_error)
{
	assert_no_null(tipo_compilacion);
	
	if (strbas_equalsStringWithoutMatchCase(parametro, "REL") == true)
	{
		*tipo_compilacion = CFGPROY_RELEASE;
		return true;
	}
	else if (strbas_equalsStringWithoutMatchCase(parametro, "DEB") == true)
	{
		*tipo_compilacion = CFGPROY_DEBUG;
		return true;
	}
	else if (strbas_equalsStringWithoutMatchCase(parametro, "PROF") == true)
	{
		*tipo_compilacion = CFGPROY_PROFILER;
		return true;
	}
	else if (strbas_equalsStringWithoutMatchCase(parametro, "LIN") == true)
	{
		*tipo_compilacion = CFGPROY_LINT;
		return true;
	}
	else
	{
	    strbas_assignMessage(mensaje_error, "El parámetro de -hacer debe ser [REL,DEB,PROF,LIN)");
		return false;
	}
}

//-----------------------------------------------------------------------

static void i_hacer(struct marco_t *marco, struct i_datos_aplicacion_t *datos_aplicacion, const char *parametro)
{
	enum tipo_compilacion_t tipo_compilacion;
	char *mensaje_error;
	
	assert_no_null(datos_aplicacion);
	
	if (i_es_version_compilacion(parametro, &tipo_compilacion, &mensaje_error) == true)
	{
		struct cfgproy_t *cfg_proyectos;

		cfg_proyectos = i_genera_proyectos(datos_aplicacion->directorio_base);

		switch(datos_aplicacion->tipo_hacer)
		{
			case i_HACER_NORMAL:
			case i_HACER_ALL: //TODO: De momento no hacemos nada con el ALL sólo por compatibilidad con Eclipse.
				cfgproy_hacer_ejecutables(cfg_proyectos, tipo_compilacion, marco);
				break;
			case i_HACER_CLEAN:
				cfgproy_clean_segun_compilacion(cfg_proyectos, tipo_compilacion, marco);
				break;
			default_error();
		}

		cfgproy_destruye(&cfg_proyectos);
	}
	else
	{
		marco_lanza_mensaje(marco, MARCO_MENSAJE_ERROR, mensaje_error);
		strbas_destroy(&mensaje_error);
	}
}

//-----------------------------------------------------------------------

static void i_integridad_sistema(struct marco_t *marco, struct i_datos_aplicacion_t *datos_aplicacion)
{
	ArrString *mensajes;
	struct cfgproy_t *cfg_proyectos;

	assert_no_null(datos_aplicacion);

	cfg_proyectos = i_genera_proyectos(datos_aplicacion->directorio_base);

	cfgproy_integridad_sistema(cfg_proyectos, &mensajes);
	marco_lanza_explicacion(marco, MARCO_MENSAJE_INFORMACION, mensajes);

	arrcad_Destruye(&mensajes);
	cfgproy_destruye(&cfg_proyectos);
}

//-----------------------------------------------------------------------

static void i_genera_mak(struct marco_t *marco, struct i_datos_aplicacion_t *datos_aplicacion, const char *parametro)
{
	enum tipo_compilacion_t tipo_compilacion;
	struct cfgproy_t *cfg_proyectos;

	tipo_compilacion = CFGPROY_DEBUG;

	cfg_proyectos = i_genera_proyectos(datos_aplicacion->directorio_base);
	cfgproy_genera_mak_proyecto(cfg_proyectos, tipo_compilacion, parametro, marco);
	cfgproy_destruye(&cfg_proyectos);
}

//-----------------------------------------------------------------------

static void i_genera_directorios(struct marco_t *marco, struct i_datos_aplicacion_t *datos_aplicacion)
{
	char *mensaje;

	assert_no_null(datos_aplicacion);
	cfgproy_crea_directorios_necesarios(marco, datos_aplicacion->directorio_base);

	mensaje = strbas_printf("Proyecto merlin generado en %s", datos_aplicacion->directorio_base);
	marco_lanza_mensaje(marco, MARCO_MENSAJE_INFORMACION, mensaje);
	strbas_destroy(&mensaje);
}

//-----------------------------------------------------------------------

static ArrTipo(opcion_t) *i_crea_opciones(void) 
{
	ArrTipo(opcion_t) *opciones;
	struct opcion_t *opcion;
	
	opciones = array_CreaPuntero(0, opcion_t);
	
	opcion = opcion_crea_opcion_simple(
						"debug",
						"Información de debug",
						"Información de debug",
						i_datos_aplicacion_t,
						i_debug, i_hay_componentes_necesarios);
	array_Anyadir(opcion_t, opciones, opcion);

	opcion = opcion_crea_opcion_con_parametro(
						"incluido",
						"Ficheros donde es incluido",
						"Ficheros donde es incluido",
						i_datos_aplicacion_t,
						i_es_incluido, i_hay_componentes_necesarios);
	array_Anyadir(opcion_t, opciones, opcion);

	opcion = opcion_crea_opcion_con_parametro(
						"depproy",
						"Dependencias entre proyectos",
						"Dependencias entre proyectos",
						i_datos_aplicacion_t,
						i_dependencias_proyectos, i_hay_componentes_necesarios);
	array_Anyadir(opcion_t, opciones, opcion);

	opcion = opcion_crea_opcion_con_parametro(
						"hacer",
						"Hacer ejecutables",
						"Hacer ejecutables",
						i_datos_aplicacion_t,
						i_hacer, i_hay_componentes_necesarios);
	array_Anyadir(opcion_t, opciones, opcion);

	opcion = opcion_crea_opcion_simple(
						"integridad",
						"Revisa la integridad del sistema",
						"Revisa la integridad del sistema",
						i_datos_aplicacion_t,
						i_integridad_sistema,
						i_hay_componentes_necesarios);
	array_Anyadir(opcion_t, opciones, opcion);

	opcion = opcion_crea_opcion_con_parametro(
						"make",
						"Genera proyectos .mak",
						"Genera proyectos .mak",
						i_datos_aplicacion_t,
						i_genera_mak, i_hay_componentes_necesarios);
	array_Anyadir(opcion_t, opciones, opcion);

	opcion = opcion_crea_opcion_simple(
						"generar",
						"Genera los directorios necesarios para que merlin procese los fuentes",
						"Genera los directorios necesarios para que merlin procese los fuentes",
						i_datos_aplicacion_t,
						i_genera_directorios, i_hay_directorio_base);
	array_Anyadir(opcion_t, opciones, opcion);

	return opciones;
}

//-----------------------------------------------------------------------

static const char *i_directorio_proyectos(const ArrString *parametros, enum i_hacer_t *tipo_hacer)
{
	const char *directorio_base;
	enum i_hacer_t tipo_hacer_loc;
	unsigned long i, num;

	assert_no_null(tipo_hacer);

	num = arrcad_numElementos(parametros);

	directorio_base = NULL;
	tipo_hacer_loc = i_HACER_NORMAL;

	for (i = 0; i < num; i++)
	{
		const char *cadena;

		cadena = arrcad_Get(parametros, i);

		if (strbas_equalsStringWithoutMatchCase(cadena, "all") == true)
		{
			assert_with_message(tipo_hacer_loc == i_HACER_NORMAL, "Sólo all o clean, pero no ambos");
			tipo_hacer_loc = i_HACER_ALL;
		}
		else if (strbas_equalsStringWithoutMatchCase(cadena, "clean") == true)
		{
		    assert_with_message(tipo_hacer_loc == i_HACER_NORMAL, "Sólo all o clean, pero no ambos");
			tipo_hacer_loc = i_HACER_CLEAN;
		}
		else
		{
		    assert_with_message(directorio_base == NULL, "Sólo un directorio base");
			directorio_base = cadena;
		}
	}

	*tipo_hacer = tipo_hacer_loc;

	if (directorio_base == NULL)
		return strbas_getEnv("MERLIN_PROJECTS");
	else
		return directorio_base;
}

//-----------------------------------------------------------------------

static struct i_datos_aplicacion_t *i_leer_configuracion(struct marco_t *marco, int argc, char *argv[], const ArrString *parametros)
{
	const char *directorio_base;
	ArrString *copia_parametros;
	char *directorio_base_copia;
	enum i_hacer_t tipo_hacer;
	
	directorio_base = i_directorio_proyectos(parametros, &tipo_hacer);

	copia_parametros = arrcad_Copia(parametros);

	if (directorio_base != NULL)
		directorio_base_copia = strbas_copyString(directorio_base);
	else
		directorio_base_copia = strbas_copyString("");

	return i_crea(&copia_parametros, &directorio_base_copia, tipo_hacer);
}

//-----------------------------------------------------------------------

int main(int argc, char *argv[])
{
	int codigo_salida;
	const char *titulo_aplicacion, *explicacion;
	ArrTipo(opcion_t) *opciones;

	titulo_aplicacion = "Merlin";
	explicacion = "Sintaxis: merlin [opciones] [directorio_proyectos]";

	opciones = i_crea_opciones();
	
	codigo_salida = aplicac_lanzar(
							argc, argv, 
							titulo_aplicacion, explicacion, i_VERSION_MERLIN,
							i_leer_configuracion,
							i_destruye,
							opciones, i_datos_aplicacion_t);
	
	array_Destruye(opcion_t, &opciones, opcion_destruye);

	return codigo_salida;
}
