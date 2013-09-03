// Definición de fichero fuente.

#include "fichsrc.inl"

#include "CGenerators.inl"

#include "asrtbas.h"
#include "memory.h"
#include "strbas.h"
#include "nomfich.h"
#include "identif.h"
#include "arrcad.h"
#include "array.h"
#include "proceso.h"
#include "marco.h"
#include "stdlib.h"

#include "filetxt.hpp"

enum i_tipo_fichero_t
{
	I_NO_IDENTIFICADO,
	I_DECLARACION_PURA_HXX,
	I_DECLARACION_INTERFACE_PUBLICO_H,
	I_DECLARACION_INTERFACE_PUBLICO_HPP,
	I_DECLARACION_ESTRUCTURA_PUBLICA_TLH,
	I_DECLARACION_INTERFACE_SISTEMA_SYH,
	I_DECLARACION_INTERFACE_PRIVADO_INL,
	I_DECLARACION_ESTRUCTURA_PRIVADA_TLI,
	I_FUENTE_C,
    I_FUENTE_CPP,
    I_FUENTE_RECURSO_PRIVADO,
    I_RECURSO_PRIVADO_GENERADO
};

struct fichsrc_t
{
	bool esta_en_libreria_dinamica;

	unsigned long id;

	char *nombre_fichero;
	enum i_tipo_fichero_t tipo_fichero;
	ArrString *includes;
};

static const char *i_TOKEN_INCLUDE = "#include";

static const bool i_INTERFACE_PUBLICO = true;
static const bool i_INTERFACE_PRIVADO = false;


//-----------------------------------------------------------------------

static void i_integridad(const struct fichsrc_t *fichero)
{
	assert_no_null(fichero);
	assert_no_null(fichero->nombre_fichero);
	assert_no_null(fichero->includes);
}

//-----------------------------------------------------------------------

static struct fichsrc_t *i_crea(
							bool esta_en_libreria_dinamica,
							unsigned long id,
							const char *nombre_fichero,
							enum i_tipo_fichero_t tipo_fichero,
							const ArrString *includes)
{
	struct fichsrc_t *fichero;

	fichero = MALLOC(struct fichsrc_t);

	fichero->esta_en_libreria_dinamica = esta_en_libreria_dinamica;
	fichero->id = id;
	fichero->nombre_fichero = strbas_copyString(nombre_fichero);
	fichero->tipo_fichero = tipo_fichero;
	fichero->includes = arrcad_Copia(includes);

	i_integridad(fichero);

	return fichero;
}

//-----------------------------------------------------------------------

static enum i_tipo_fichero_t i_obtener_tipo_fichero(
						const char *nombre_fichero, 
						const class CGenerators *generators)
{
	char *extension;
	enum i_tipo_fichero_t tipo_fichero;

	extension = nomfich_extension(nombre_fichero);

	if (strbas_equalsStringWithoutMatchCase(extension, "hxx") == true)
	{
		tipo_fichero = I_DECLARACION_PURA_HXX;
	}
	else if (strbas_equalsStrings(extension, "h") == true)
	{
		tipo_fichero = I_DECLARACION_INTERFACE_PUBLICO_H;
	}
	else if (strbas_equalsStrings(extension, "tlh") == true)
	{
		tipo_fichero = I_DECLARACION_ESTRUCTURA_PUBLICA_TLH;
	}
	else if (strbas_equalsStrings(extension, "syh") == true)
	{
		tipo_fichero = I_DECLARACION_INTERFACE_SISTEMA_SYH;
	}
	else if (strbas_equalsStrings(extension, "inl") == true)
	{
		tipo_fichero = I_DECLARACION_INTERFACE_PRIVADO_INL;
	}
	else if (strbas_equalsStrings(extension, "tli") == true)
	{
		tipo_fichero = I_DECLARACION_ESTRUCTURA_PRIVADA_TLI;
	}
	else if (strbas_equalsStrings(extension, "c") == true)
	{
		tipo_fichero = I_FUENTE_C;
	}
	else if (strbas_equalsStrings(extension, "cpp") == true)
	{
		tipo_fichero = I_FUENTE_CPP;
	}
	else if (strbas_equalsStrings(extension, "hpp") == true)
	{
		tipo_fichero = I_DECLARACION_INTERFACE_PUBLICO_HPP;
	}
    else if (generators->isResourceSource(nombre_fichero) == true)
    {
    	tipo_fichero = I_FUENTE_RECURSO_PRIVADO;
    }
    else if (generators->isResourceGenerated(nombre_fichero) == true)
    {
        tipo_fichero = I_RECURSO_PRIVADO_GENERADO;
    }
    else
    {
		tipo_fichero = I_NO_IDENTIFICADO;
    }

	strbas_destroy(&extension);

	return tipo_fichero;
}

//-----------------------------------------------------------------------

static bool i_es_linea_include(const char *linea, char **nombre_include)
{
	char *linea_sin_blancos;
	bool respuesta;

	linea_sin_blancos = strbas_LTrim(linea);

	if (strbas_isInitialSubstring(linea_sin_blancos, i_TOKEN_INCLUDE) == true)
	{
		char *sin_include, *sin_include_y_sin_blancos;

		sin_include = strbas_subtractionSubstring(linea_sin_blancos, i_TOKEN_INCLUDE);
		sin_include_y_sin_blancos = strbas_LTrim(sin_include);
		switch(sin_include_y_sin_blancos[0])
		{
			case '"':
			{
				respuesta = strbas_getStringBetweenChar(
											sin_include_y_sin_blancos,
											'"', '"',
											nombre_include);
				break;
			}
			case '<':
			{
				respuesta = strbas_getStringBetweenChar(
											sin_include_y_sin_blancos,
											'<', '>',
											nombre_include);
				break;
			}
			default:
				respuesta = false;
				break;
		}
		strbas_destroy(&sin_include);
		strbas_destroy(&sin_include_y_sin_blancos);
	}
	else
		respuesta = false;

	strbas_destroy(&linea_sin_blancos);

	return respuesta;
}

//-----------------------------------------------------------------------

struct fichsrc_t *fichsrc_crea_defecto(
							const class CGenerators *generadores,
							bool esta_en_libreria_dinamica,
							const char *nombre_fichero, 
							unsigned long *id_nuevo)
{
	struct fichsrc_t *fichero;
	enum i_tipo_fichero_t tipo_fichero;
	unsigned long id;
	ArrString *includes;

	id = identif_nuevo_identificador(id_nuevo);

	tipo_fichero = i_obtener_tipo_fichero(nombre_fichero, generadores);
	includes = arrcad_CreaString(0);

	fichero = i_crea(esta_en_libreria_dinamica, id, nombre_fichero, tipo_fichero, includes);

	arrcad_Destruye(&includes);

	return fichero;
}

//-----------------------------------------------------------------------

void fichsrc_destruye(struct fichsrc_t **fichero)
{
	assert_no_null(fichero);
	i_integridad(*fichero);

	strbas_destroy(&(*fichero)->nombre_fichero);
	arrcad_Destruye(&(*fichero)->includes);

	FREE_T(fichero, struct fichsrc_t);
}

//-----------------------------------------------------------------------

struct fichsrc_t *fichsrc_copia(const struct fichsrc_t *fichero)
{
	assert_no_null(fichero);
	return i_crea(fichero->esta_en_libreria_dinamica, fichero->id, fichero->nombre_fichero, fichero->tipo_fichero, fichero->includes);
}

//-----------------------------------------------------------------------

bool fichsrc_es_fuente(const struct fichsrc_t *fichero)
{
	i_integridad(fichero);

	switch(fichero->tipo_fichero)
	{
		case I_NO_IDENTIFICADO:

			return false;

		case I_DECLARACION_PURA_HXX:
		case I_DECLARACION_INTERFACE_PUBLICO_H:
		case I_DECLARACION_INTERFACE_PUBLICO_HPP:
		case I_DECLARACION_ESTRUCTURA_PUBLICA_TLH:
		case I_DECLARACION_INTERFACE_SISTEMA_SYH:
		case I_DECLARACION_INTERFACE_PRIVADO_INL:
		case I_DECLARACION_ESTRUCTURA_PRIVADA_TLI:
		case I_FUENTE_C:
		case I_FUENTE_CPP:
		case I_FUENTE_RECURSO_PRIVADO:
		case I_RECURSO_PRIVADO_GENERADO:
			
			return true;

		default_error();
	}

	assert_return_error(bool);
}

//-----------------------------------------------------------------------

static bool i_es_fuente(enum i_tipo_fichero_t tipo_fichero)
{
	bool es_fuente;

	switch(tipo_fichero)
	{
		case I_DECLARACION_PURA_HXX:
		case I_DECLARACION_INTERFACE_PUBLICO_H:
		case I_DECLARACION_INTERFACE_PUBLICO_HPP:
		case I_DECLARACION_ESTRUCTURA_PUBLICA_TLH:
		case I_DECLARACION_INTERFACE_SISTEMA_SYH:
		case I_DECLARACION_INTERFACE_PRIVADO_INL:
		case I_DECLARACION_ESTRUCTURA_PRIVADA_TLI:
		case I_FUENTE_RECURSO_PRIVADO:
		case I_RECURSO_PRIVADO_GENERADO:

			es_fuente = false;
			break;

		case I_FUENTE_C:
		case I_FUENTE_CPP:

			es_fuente = true;
			break;

		case I_NO_IDENTIFICADO:
		default_error();
	}

	return es_fuente;
}

//-----------------------------------------------------------------------

bool fichsrc_tiene_obj(const struct fichsrc_t *fichero)
{
	i_integridad(fichero);
	return i_es_fuente(fichero->tipo_fichero);
}

//-----------------------------------------------------------------------

static const char *i_texto_tipo_fichero(enum i_tipo_fichero_t tipo_fichero)
{
	switch(tipo_fichero)
	{
		case I_DECLARACION_PURA_HXX:

			return "fichero hxx";

		case I_DECLARACION_INTERFACE_PUBLICO_H:

			return "fichero h";

		case I_DECLARACION_INTERFACE_PUBLICO_HPP:

			return "fichero hpp";

		case I_DECLARACION_ESTRUCTURA_PUBLICA_TLH:

			return "fichero tlh";

		case I_DECLARACION_INTERFACE_SISTEMA_SYH:

			return "fichero syh";

		case I_DECLARACION_INTERFACE_PRIVADO_INL:

			return "fichero inl";

		case I_DECLARACION_ESTRUCTURA_PRIVADA_TLI:

			return "fichero tli";

		case I_FUENTE_C:

			return "fichero c";

		case I_FUENTE_CPP:

			return "fichero cpp";
			
		case I_FUENTE_RECURSO_PRIVADO:
			
			return "fichero recurso privado";
			
		case I_RECURSO_PRIVADO_GENERADO:

		    return "fichero generado";

		case I_NO_IDENTIFICADO:
		default_error();
	}

	assert_return_error(const char *);
}

//-----------------------------------------------------------------------

void fichsrc_calcular_dependencias(struct fichsrc_t *fichero)
{
	ArrString *lineas_fichero;
	ArrString *includes;
	unsigned long num_lineas, i;

	i_integridad(fichero);

	includes = arrcad_CreaString(0);
	lineas_fichero = filetxt_leer_fichero_texto(fichero->nombre_fichero);

	num_lineas = arrcad_numElementos(lineas_fichero);

	for (i = 0; i < num_lineas; i++)
	{
		const char *linea;
		char *nombre_include;

		linea = arrcad_Get(lineas_fichero, i);
		if (i_es_linea_include(linea, &nombre_include) == true)
		{
			arrcad_Anyadir(includes, nombre_include);
			strbas_destroy(&nombre_include);
		}
	}

	arrcad_Destruye(&lineas_fichero);
	arrcad_Destruye(&fichero->includes);
	fichero->includes = includes;
}

//-----------------------------------------------------------------------

void fichsrc_debug(const struct fichsrc_t *fichero, struct marco_t *marco)
{
	unsigned long num_dependencias, i;
	char *mensaje;

	i_integridad(fichero);

	mensaje = strbas_printf("Nombre fichero: %s Tipo: %s", fichero->nombre_fichero, i_texto_tipo_fichero(fichero->tipo_fichero));
	marco_lanza_mensaje(marco, MARCO_MENSAJE_INFORMACION, mensaje);
	strbas_destroy(&mensaje);

	num_dependencias = arrcad_numElementos(fichero->includes);

	for (i = 0; i < num_dependencias; i++)
	{
		const char *dependencia;

		dependencia = arrcad_Get(fichero->includes, i);
		mensaje = strbas_printf("\t%s", dependencia);
		marco_lanza_mensaje(marco, MARCO_MENSAJE_INFORMACION, mensaje);
		strbas_destroy(&mensaje);
	}
}

//-----------------------------------------------------------------------

bool fichsrc_es_incluido_fichero(
						const struct fichsrc_t *fichero_proyecto,
						const char *fichero,
						char **mensaje)
{
	bool es_incluido;

	i_integridad(fichero_proyecto);

	es_incluido = arrcad_Buscar(fichero_proyecto->includes, fichero, NULL, NULL);

	if (es_incluido == true)
	    strbas_assignMessage(mensaje, fichero_proyecto->nombre_fichero);

	return es_incluido;
}

//-----------------------------------------------------------------------

bool fichsrc_es_incluido(
						const struct fichsrc_t *fichero,
						const struct fichsrc_t *fichero_incluido)
{
	char *nombre_base_con_extension;
	bool es_incluido;

	i_integridad(fichero);
	i_integridad(fichero_incluido);

	nombre_base_con_extension = nomfich_nombre_base_con_extension(fichero_incluido->nombre_fichero);
	es_incluido = arrcad_Buscar(fichero->includes, nombre_base_con_extension, NULL, NULL);
	strbas_destroy(&nombre_base_con_extension);

	return es_incluido;
}


//-----------------------------------------------------------------------

bool fichsrc_es_fichero(const struct fichsrc_t *fichero_proyecto, const char *nombre_fichero)
{
	bool es_fichero;
	char *nombre_fichero_con_extension;

	i_integridad(fichero_proyecto);

	nombre_fichero_con_extension = nomfich_nombre_base_con_extension(fichero_proyecto->nombre_fichero);

	es_fichero = strbas_equalsStrings(nombre_fichero, nombre_fichero_con_extension);

	strbas_destroy(&nombre_fichero_con_extension);

	return es_fichero;
}

//-----------------------------------------------------------------------

bool fichsrc_es_igual(const struct fichsrc_t *fichero_1, const struct fichsrc_t *fichero_2)
{
	i_integridad(fichero_1);
	i_integridad(fichero_2);

	return strbas_equalsStrings(fichero_1->nombre_fichero, fichero_2->nombre_fichero);
}

//-----------------------------------------------------------------------

void fichsrc_anyadir_ficheros_que_incluye(const struct fichsrc_t *fichero, ArrString *ficheros_incluidos)
{
	unsigned long num_ficheros_incluidos, i;

	i_integridad(fichero);

	num_ficheros_incluidos = arrcad_numElementos(fichero->includes);

	for (i = 0; i < num_ficheros_incluidos; i++)
	{
		const char *nombre_fichero;

		nombre_fichero = arrcad_Get(fichero->includes, i);
		if (arrcad_Buscar(ficheros_incluidos, nombre_fichero, NULL, NULL) == false)
			arrcad_Anyadir(ficheros_incluidos, nombre_fichero);
	}
}

//-----------------------------------------------------------------------

static char *i_obtener_nombre_proyecto(const char *nombre_fichero)
{
	char *directorio_proyecto, *nombre_proyecto;

	directorio_proyecto = nomfich_directorio(nombre_fichero);
	nombre_proyecto = nomfich_nombre_base_con_extension(directorio_proyecto);
	strbas_destroy(&directorio_proyecto);

	return nombre_proyecto;
}

//-----------------------------------------------------------------------

static char *i_nombre_fichero_ext(
						const char *nombre_fichero,
						enum i_tipo_fichero_t tipo_fichero,
						const char *dirs_base,
						const char *ext)
{
	switch(tipo_fichero)
	{
		case I_FUENTE_C:
		case I_FUENTE_CPP:
		{
			char *nombre_base, *path, *nombre_base_con_extension, *nombre_proyecto, *path_proyecto;

			nombre_proyecto = i_obtener_nombre_proyecto(nombre_fichero);
			nombre_base = nomfich_nombre_base_con_extension(nombre_fichero);
			nombre_base_con_extension = nomfich_cambia_extension(nombre_base, ext);

			path_proyecto = nomfich_append_a_directorio(dirs_base, nombre_proyecto);
			if (nomfich_existe_fichero(path_proyecto) == false)
				nomfich_crear_directorios_si_no_existen(path_proyecto);
			path = nomfich_append_a_directorio(path_proyecto, nombre_base_con_extension);

			strbas_destroy(&path_proyecto);
			strbas_destroy(&nombre_proyecto);
			strbas_destroy(&nombre_base);
			strbas_destroy(&nombre_base_con_extension);

			return path;
		}
		case I_DECLARACION_PURA_HXX:
		case I_DECLARACION_INTERFACE_PUBLICO_H:
		case I_DECLARACION_INTERFACE_PUBLICO_HPP:
		case I_DECLARACION_ESTRUCTURA_PUBLICA_TLH:
		case I_DECLARACION_INTERFACE_SISTEMA_SYH:
		case I_DECLARACION_INTERFACE_PRIVADO_INL:
		case I_DECLARACION_ESTRUCTURA_PRIVADA_TLI:
		case I_NO_IDENTIFICADO:
		case I_FUENTE_RECURSO_PRIVADO:
		case I_RECURSO_PRIVADO_GENERADO:
		default_error();
	}

	assert_return_error(char *);
}

//-----------------------------------------------------------------------

bool fichsrc_es_compilable(
						const struct fichsrc_t *fichero,
						const char *dirs_objs,
						const char *ext_objs)
{
	char *nombre_obj;
	bool es_compilable;

	i_integridad(fichero);

	nombre_obj = i_nombre_fichero_ext(fichero->nombre_fichero, fichero->tipo_fichero, dirs_objs, ext_objs);

	if (nomfich_existe_fichero(nombre_obj) == true)
		es_compilable = nomfich_es_mas_moderno(fichero->nombre_fichero, nombre_obj);
	else
		es_compilable = true;

	strbas_destroy(&nombre_obj);

	return es_compilable;
}

//-----------------------------------------------------------------------

bool fichsrc_es_generable(const struct fichsrc_t *fichero, const class CGenerators *generators)
{
	bool es_generable;
	
	i_integridad(fichero);
	assert_no_null(generators);

	switch(fichero->tipo_fichero)
	{
		case I_DECLARACION_PURA_HXX:
		case I_DECLARACION_INTERFACE_PUBLICO_H:
		case I_DECLARACION_INTERFACE_PUBLICO_HPP:
		case I_DECLARACION_ESTRUCTURA_PUBLICA_TLH:
		case I_DECLARACION_INTERFACE_SISTEMA_SYH:
		case I_DECLARACION_INTERFACE_PRIVADO_INL:
		case I_DECLARACION_ESTRUCTURA_PRIVADA_TLI:
		case I_NO_IDENTIFICADO:
		case I_FUENTE_C:
		case I_FUENTE_CPP:
        case I_RECURSO_PRIVADO_GENERADO:

			es_generable = false;
			break;

		case I_FUENTE_RECURSO_PRIVADO:

			es_generable = generators->mustGenerate(fichero->nombre_fichero);
			break;
			
		default_error();
	}
	
	return es_generable;
}
	
//-----------------------------------------------------------------------

bool fichsrc_es_compilable_fichero_incluido(
						const struct fichsrc_t *fichero,
						const struct fichsrc_t *fichero_include,
						const char *dirs_objs,
						const char *ext_objs)
{
	char *nombre_obj;
	bool es_compilable;

	i_integridad(fichero);

	nombre_obj = i_nombre_fichero_ext(fichero->nombre_fichero, fichero->tipo_fichero, dirs_objs, ext_objs);

	if (nomfich_existe_fichero(nombre_obj) == true)
		es_compilable = nomfich_es_mas_moderno(fichero_include->nombre_fichero, nombre_obj);
	else
		es_compilable = true;

	strbas_destroy(&nombre_obj);

	return es_compilable;
}

//-----------------------------------------------------------------------

static char *i_obtener_flags_includes(const ArrString *includes_necesarios)
{
	char *flags;
	unsigned long i, num_includes;

	num_includes = arrcad_numElementos(includes_necesarios);

	flags = strbas_copyString("");

	for (i = 0; i < num_includes; i++)
	{
		const char *include;
		char *flag_include;

		include = arrcad_Get(includes_necesarios, i);
		flag_include = strbas_printf(" -I%s", include);
		strbas_concatenate(&flags, flag_include),
		strbas_destroy(&flag_include);
	}

	return flags;
}

//-----------------------------------------------------------------------


static char *i_obtener_flags_proyecto_include(
						const ArrTipo(fichsrc_t) *ficheros_proyecto_include,
						const char *flag_include)
{
	char *flags;

	flags = strbas_copyString("");

	if (ficheros_proyecto_include != NULL)
	{
		unsigned long i, num_ficheros;

		num_ficheros = array_numElementos(fichsrc_t, ficheros_proyecto_include);

		for (i = 0; i < num_ficheros; i++)
		{
			const struct fichsrc_t *fichero;
			char *flag;

			fichero = array_Get(fichsrc_t, ficheros_proyecto_include, i);
			assert_no_null(fichero);
			flag = strbas_printf("%s %s", flag_include, fichero->nombre_fichero);
			strbas_concatenate(&flags, flag),
			strbas_destroy(&flag);
		}
	}

	return flags;
}

//-----------------------------------------------------------------------

static void i_muestra_errores(struct marco_t *marco, const char *path_err)
{
	ArrString *fichero_error;
	unsigned long i, num_lineas;

	fichero_error = filetxt_leer_fichero_texto(path_err);

	num_lineas = arrcad_numElementos(fichero_error);

	for (i = 0; i < num_lineas; i++)
	{
		const char *linea;

		linea = arrcad_Get(fichero_error, i);
		marco_lanza_mensaje(marco, MARCO_MENSAJE_INFORMACION, linea);
	}

	arrcad_Destruye(&fichero_error);
	
	if (num_lineas == 0)
		nomfich_borrar_fichero(path_err);
}

//-----------------------------------------------------------------------

void fichsrc_generar(
				const struct fichsrc_t *fichero,
				const class CGenerators *generators,
				struct marco_t *marco)
{
    i_integridad(fichero);
    assert_no_null(generators);

	assert(fichero->tipo_fichero == I_FUENTE_RECURSO_PRIVADO);
	generators->generate(fichero->nombre_fichero, marco);
}

//-----------------------------------------------------------------------

static void prv_anyadirALineaComando(char **linea_comando, const char *textoAAnyadir)
{
    strbas_concatenate(linea_comando, " ");
    strbas_concatenate(linea_comando, textoAAnyadir);
}

//-----------------------------------------------------------------------

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
				const ArrString *flags_adicionales)
{
	char *nombre_err, *nombre_obj, *linea_comando,
		 *flags_includes, *flags_ficheros_proyectos_include,
		 *mensaje, *texto_flags_adicionales, *str_nivel_salida;
	const char *flag_compilacion_dinamica;

	i_integridad(fichero);

	nombre_obj = i_nombre_fichero_ext(fichero->nombre_fichero, fichero->tipo_fichero, directorio_objs, ext_objs);
	nombre_err = i_nombre_fichero_ext(fichero->nombre_fichero, fichero->tipo_fichero, directorio_err, "err");
	flags_includes = i_obtener_flags_includes(includes_necesarios);
	flags_ficheros_proyectos_include = i_obtener_flags_proyecto_include(ficheros_proyecto_include, flag_include);
	texto_flags_adicionales = arrcad_texto_separado_por_espacios(flags_adicionales);

	if (nomfich_existe_fichero(nombre_obj) == true)
		nomfich_borrar_fichero(nombre_obj);

	if (fichero->esta_en_libreria_dinamica == true)
		flag_compilacion_dinamica = "-fPIC";
	else
		flag_compilacion_dinamica = "";

	str_nivel_salida = strbas_printf("%u>", nivel_salida);

	linea_comando = strbas_copyString(compilador);
	prv_anyadirALineaComando(&linea_comando, flag_compilacion_dinamica);
	prv_anyadirALineaComando(&linea_comando, flags_compilador);
	prv_anyadirALineaComando(&linea_comando, texto_flags_adicionales);
	prv_anyadirALineaComando(&linea_comando, flags_includes);
	prv_anyadirALineaComando(&linea_comando, flags_ficheros_proyectos_include);
	prv_anyadirALineaComando(&linea_comando, flags_objs);
	prv_anyadirALineaComando(&linea_comando, nombre_obj);
	prv_anyadirALineaComando(&linea_comando, fichero->nombre_fichero);
	prv_anyadirALineaComando(&linea_comando, str_nivel_salida);
	prv_anyadirALineaComando(&linea_comando, nombre_err);

    if (getenv("MOSTRAR") != NULL)
	    marco_lanza_mensaje(marco, MARCO_MENSAJE_INFORMACION, linea_comando);

	mensaje = strbas_printf("Compilando %s", fichero->nombre_fichero);
	marco_lanza_mensaje(marco, MARCO_MENSAJE_INFORMACION, mensaje);
	strbas_destroy(&mensaje);
	strbas_destroy(&texto_flags_adicionales);

	proceso_lanzar_proceso(linea_comando);
	i_muestra_errores(marco, nombre_err);

	strbas_destroy(&str_nivel_salida);
	strbas_destroy(&nombre_err);
	strbas_destroy(&nombre_obj);
	strbas_destroy(&linea_comando);
	strbas_destroy(&flags_includes);
	strbas_destroy(&flags_ficheros_proyectos_include);
}

//-----------------------------------------------------------------------

const ArrString *fichsrc_incluidos(const struct fichsrc_t *fichero)
{
	i_integridad(fichero);

	return fichero->includes;
}

//-----------------------------------------------------------------------

static ArrString *i_obtener_ficheros_objs(
				const ArrTipo(fichsrc_t) *ficheros_necesarios,
				const char *directorio_objs,
				const char *ext_objs)
{
	unsigned long num_ficheros, i;
	ArrString *ficheros_objs;

	num_ficheros = array_numElementos(fichsrc_t, ficheros_necesarios);
	ficheros_objs = arrcad_CreaString(num_ficheros);

	for (i = 0; i < num_ficheros; i++)
	{
		char *nombre_obj;
		const struct fichsrc_t *fichero;

		fichero = array_Get(fichsrc_t, ficheros_necesarios, i);

		nombre_obj = i_nombre_fichero_ext(fichero->nombre_fichero, fichero->tipo_fichero, directorio_objs, ext_objs);
		arrcad_Set(ficheros_objs, i, nombre_obj);
		strbas_destroy(&nombre_obj);
	}

	return ficheros_objs;
}

//-----------------------------------------------------------------------

static char *i_obtener_cadena_ficheros(const ArrString *ficheros)
{
	char *cadena_objs;
	unsigned long i, num_includes;

	num_includes = arrcad_numElementos(ficheros);

	cadena_objs = strbas_copyString("");

	for (i = 0; i < num_includes; i++)
	{
		const char *fichero;
		char *fichero_con_espacio;

		fichero = arrcad_Get(ficheros, i);
		fichero_con_espacio = strbas_printf(" %s", fichero);
		strbas_concatenate(&cadena_objs, fichero_con_espacio),
		strbas_destroy(&fichero_con_espacio);
	}

	return cadena_objs;
}

//-----------------------------------------------------------------------

static bool i_se_debe_linkar(
					const char *path_exe, 
					const ArrString *ficheros_dependientes)
{
	if (nomfich_existe_fichero(path_exe) == true)
	{
		unsigned long num_ficheros_objs, i;

		num_ficheros_objs = arrcad_numElementos(ficheros_dependientes);

		for (i = 0; i < num_ficheros_objs; i++)
		{
			const char *fichero;

			fichero = arrcad_Get(ficheros_dependientes, i);
			if (nomfich_existe_fichero(fichero) == true)
			{
				if (nomfich_es_mas_moderno(fichero, path_exe) == true)
					return true;
			}
			else
				return false;
		}

		return false;
	}
	else
		return true;
}

//-----------------------------------------------------------------------

static void i_nombres_ficheros_salida(
							const char *nombre_fichero,
							const char *ext_exes,
							char **nombre_fichero_ejecutable,
							char **nombre_fichero_err_opc)
{
	char *nombre_fichero_base;

	assert_no_null(nombre_fichero_ejecutable);

	nombre_fichero_base = nomfich_nombre_base_con_extension(nombre_fichero);

	*nombre_fichero_ejecutable = nomfich_cambia_extension(nombre_fichero_base, ext_exes);

	if (nombre_fichero_err_opc != NULL)
		*nombre_fichero_err_opc = nomfich_cambia_extension(nombre_fichero_base, "err");

	strbas_destroy(&nombre_fichero_base);
}

//-----------------------------------------------------------------------

char *fichsrc_nombre_ejecutable(const struct fichsrc_t *ejecutable, const char *ext_exes)
{
	char *nombre_ejecutable;

	i_integridad(ejecutable);
	i_nombres_ficheros_salida(ejecutable->nombre_fichero, ext_exes, &nombre_ejecutable, NULL);

	return nombre_ejecutable;
}

//-----------------------------------------------------------------------

char *fichsrc_nombre_obj(const struct fichsrc_t *fichero_fuente, const char *ext_objs)
{
	char *nombre_obj, *nombre_fichero_base;

	i_integridad(fichero_fuente);

	nombre_fichero_base = nomfich_nombre_base_con_extension(fichero_fuente->nombre_fichero);
	nombre_obj = nomfich_cambia_extension(nombre_fichero_base, ext_objs);
	strbas_destroy(&nombre_fichero_base);

	return nombre_obj;
}

//-----------------------------------------------------------------------

char *fichsrc_nombre_base_con_extension(const struct fichsrc_t *fichero)
{
	i_integridad(fichero);
	return nomfich_nombre_base_con_extension(fichero->nombre_fichero);
}

//-----------------------------------------------------------------------

char *fichsrc_nombre_base(const struct fichsrc_t *fichero)
{
	i_integridad(fichero);
	return nomfich_nombre_base(fichero->nombre_fichero);
}

//-----------------------------------------------------------------------

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
				struct marco_t *marco)
{
	ArrString *ficheros_objs;
	char *nombre_base_exe, *path_exe, *linea_comando, *nombre_base_err, *path_err, *cadena_objs;

	i_nombres_ficheros_salida(ejecutable->nombre_fichero, ext_exes, &nombre_base_exe, &nombre_base_err);
	path_exe = nomfich_append_a_directorio(directorio_exes, nombre_base_exe);
	path_err = nomfich_append_a_directorio(directorio_exes, nombre_base_err);

	ficheros_objs = i_obtener_ficheros_objs(ficheros_necesarios, directorio_objs, exts_objs);

	if (i_se_debe_linkar(path_exe, ficheros_objs) == true
			|| i_se_debe_linkar(path_exe, dlls_path) == true)
	{
		char *mensaje, *texto_flags_adicionales, *cadena_dlls;

		if (nomfich_existe_directorio(directorio_exes) == false)
			nomfich_crear_directorios_si_no_existen(directorio_exes);

		if (nomfich_existe_fichero(path_exe) == true)
			nomfich_borrar_fichero(path_exe);
		
		texto_flags_adicionales = arrcad_texto_separado_por_espacios(flags_adicionales);
		cadena_objs = i_obtener_cadena_ficheros(ficheros_objs);
		cadena_dlls = i_obtener_cadena_ficheros(dlls_flags);

		linea_comando = strbas_copyString(linker);
		prv_anyadirALineaComando(&linea_comando, cadena_objs);
        prv_anyadirALineaComando(&linea_comando, texto_flags_adicionales);
        prv_anyadirALineaComando(&linea_comando, flags_linker);

        if (strbas_length(cadena_dlls) > 0)
        {
            prv_anyadirALineaComando(&linea_comando, "-L");
            strbas_concatenate(&linea_comando, directorio_dlls);
            prv_anyadirALineaComando(&linea_comando, cadena_dlls);
        }

        prv_anyadirALineaComando(&linea_comando, "-o");
        prv_anyadirALineaComando(&linea_comando, path_exe);
        prv_anyadirALineaComando(&linea_comando, "2>");
        prv_anyadirALineaComando(&linea_comando, path_err);

		/*if (strbas_length(cadena_dlls) > 0)
		{
			linea_comando = strbas_printf("%s %s %s %s -L%s %s -o %s 2> %s", 
								linker, cadena_objs, texto_flags_adicionales, 
								flags_linker,
								directorio_dlls, cadena_dlls,
								path_exe, path_err);
		}
		else
		{
			linea_comando = strbas_printf("%s %s %s %s -o %s 2> %s", 
								linker, cadena_objs, texto_flags_adicionales, 
								flags_linker,
								path_exe, path_err);
		}*/

		if (getenv("MOSTRAR") != NULL)
			marco_lanza_mensaje(marco, MARCO_MENSAJE_INFORMACION, linea_comando);

		mensaje = strbas_printf("Linkando %s", nombre_base_exe);
		marco_lanza_mensaje(marco, MARCO_MENSAJE_INFORMACION, mensaje);
		strbas_destroy(&mensaje);

		proceso_lanzar_proceso(linea_comando);
		i_muestra_errores(marco, path_err);

		strbas_destroy(&texto_flags_adicionales);
		strbas_destroy(&cadena_dlls);
	}

	strbas_destroy(&nombre_base_exe);
	strbas_destroy(&nombre_base_err);
	strbas_destroy(&path_exe);
	strbas_destroy(&path_err);

	arrcad_Destruye(&ficheros_objs);
}

//-----------------------------------------------------------------------

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
                struct marco_t *marco)
{
    ArrString *ficheros_objs;
    char *nombreBasePlugins, *nombreBaseErr;
    char *nombreProyectoErr, *nombreProyectoPlugins;

    nombreBasePlugins = nomfich_append_a_directorio(directorio_plugins, nombre_proyecto);
    nombreProyectoPlugins = nomfich_cambia_extension(nombreBasePlugins, ext_dlls);

    nombreBaseErr = nomfich_append_a_directorio(directorio_errores, nombre_proyecto);
    nombreProyectoErr = nomfich_cambia_extension(nombreBaseErr, "err");

    ficheros_objs = i_obtener_ficheros_objs(ficheros_necesarios, directorio_objs, exts_objs);

    if (i_se_debe_linkar(nombreProyectoPlugins, ficheros_objs) == true)
    {
        char *mensaje, *linea_comando, *cadena_objs;

        if (nomfich_existe_directorio(directorio_plugins) == false)
            nomfich_crear_directorios_si_no_existen(directorio_plugins);

        if (nomfich_existe_directorio(directorio_errores) == false)
            nomfich_crear_directorios_si_no_existen(directorio_errores);

        cadena_objs = i_obtener_cadena_ficheros(ficheros_objs);

        if (nomfich_existe_fichero(nombreProyectoPlugins) == true)
            nomfich_borrar_fichero(nombreProyectoPlugins);

        linea_comando = strbas_printf("%s %s -o %s %s 2> %s", linker, flags_linker, nombreProyectoPlugins, cadena_objs, nombreProyectoErr);

        mensaje = strbas_printf("Linkando %s", nombre_proyecto);
        marco_lanza_mensaje(marco, MARCO_MENSAJE_INFORMACION, mensaje);

        proceso_lanzar_proceso(linea_comando);

        i_muestra_errores(marco, nombreProyectoErr);

        strbas_destroy(&mensaje);
        strbas_destroy(&linea_comando);
        strbas_destroy(&cadena_objs);
    }

    arrcad_Destruye(&ficheros_objs);
    strbas_destroy(&nombreBasePlugins);
    strbas_destroy(&nombreBaseErr);
    strbas_destroy(&nombreProyectoErr);
    strbas_destroy(&nombreProyectoPlugins);
}

//-----------------------------------------------------------------------

void fichsrc_linkar_dll(
				const ArrTipo(fichsrc_t) *ficheros_necesarios,
				const char *nombre_libreria_con_extension,
				const char *exts_objs,
				const char *directorio_objs,
				const char *directorio_exes,
				const char *ext_dlls,
				const char *linker,
				const char *flags_linker,
				struct marco_t *marco)
{
	ArrString *ficheros_objs;
	char *path_dll, *nombre_base_err, *path_err;

	nombre_base_err = nomfich_cambia_extension(nombre_libreria_con_extension, "err");
	path_dll = nomfich_append_a_directorio(directorio_exes, nombre_libreria_con_extension);
	path_err = nomfich_append_a_directorio(directorio_exes, nombre_base_err);

	ficheros_objs = i_obtener_ficheros_objs(ficheros_necesarios, directorio_objs, exts_objs);
	
	if (i_se_debe_linkar(path_dll, ficheros_objs) == true)
	{
		char *mensaje, *linea_comando, *cadena_objs; 

		if (nomfich_existe_fichero(directorio_exes) == false)
			nomfich_crear_directorios_si_no_existen(directorio_exes);

		cadena_objs = i_obtener_cadena_ficheros(ficheros_objs);

		if (nomfich_existe_fichero(path_dll) == true)
			nomfich_borrar_fichero(path_dll);

		linea_comando = strbas_printf("%s %s -o %s %s 2> %s", linker, flags_linker, path_dll, cadena_objs, path_err);

		mensaje = strbas_printf("Linkando %s", nombre_libreria_con_extension);
		marco_lanza_mensaje(marco, MARCO_MENSAJE_INFORMACION, mensaje);

		proceso_lanzar_proceso(linea_comando);
		
		i_muestra_errores(marco, path_err);
		
		strbas_destroy(&mensaje);
		strbas_destroy(&linea_comando);
		strbas_destroy(&cadena_objs);
	}
	
	strbas_destroy(&nombre_base_err);
	strbas_destroy(&path_dll);
	strbas_destroy(&path_err);

	arrcad_Destruye(&ficheros_objs);
}

//-----------------------------------------------------------------------

static bool i_es_extension_include(const char *nombre_fichero)
{
    bool es_extension_include;
    char *extension;

    extension = nomfich_extension(nombre_fichero);

    if (strbas_equalsStrings(extension, "hxx") == true
            || strbas_equalsStrings(extension, "h") == true
            || strbas_equalsStrings(extension, "tlh") == true
            || strbas_equalsStrings(extension, "syh") == true
            || strbas_equalsStrings(extension, "inl") == true
            || strbas_equalsStrings(extension, "tli") == true
            || strbas_equalsStrings(extension, "hpp") == true)
    {
        es_extension_include = true;
    }
    else
        es_extension_include = false;

    strbas_destroy(&extension);

    return es_extension_include;
}

//-----------------------------------------------------------------------

bool fichsrc_es_include(const struct fichsrc_t *fichero_base, const class CGenerators *generators, const char *include)
{
    bool es_include;

    if (generators->isResourceGenerated(include) == true)
        es_include = true;
    else if (i_es_extension_include(include) == true)
    {
        char *nombre_sin_extension, *include_sin_extension;

        nombre_sin_extension = nomfich_nombre_base(fichero_base->nombre_fichero);
        include_sin_extension = nomfich_nombre_base(include);

        es_include = strbas_equalsStringWithoutMatchCase(include_sin_extension, nombre_sin_extension);

        strbas_destroy(&nombre_sin_extension);
        strbas_destroy(&include_sin_extension);
    }
    else
        es_include = false;

    return es_include;
}

//-----------------------------------------------------------------------

bool fichsrc_es_cabecera(const struct fichsrc_t *fichero)
{
    bool es_cabecera;

    assert_no_null(fichero);

    switch(fichero->tipo_fichero)
    {
        case I_DECLARACION_PURA_HXX:
        case I_DECLARACION_INTERFACE_PUBLICO_H:
        case I_DECLARACION_INTERFACE_PUBLICO_HPP:
        case I_DECLARACION_ESTRUCTURA_PUBLICA_TLH:
        case I_DECLARACION_INTERFACE_SISTEMA_SYH:
        case I_DECLARACION_INTERFACE_PRIVADO_INL:
        case I_DECLARACION_ESTRUCTURA_PRIVADA_TLI:
        case I_RECURSO_PRIVADO_GENERADO:

            es_cabecera = true;
            break;

        case I_NO_IDENTIFICADO:
        case I_FUENTE_C:
        case I_FUENTE_CPP:
        case I_FUENTE_RECURSO_PRIVADO:

            es_cabecera = false;
            break;

        default_error();
    }

    return es_cabecera;
}

//-----------------------------------------------------------------------

static bool i_perteneces_al_mismo_proyecto(
						const char *nombre_fichero_1,
						const char *nombre_fichero_2)
{
	char *nombre_proyecto_1, *nombre_proyecto_2;
	bool iguales;

	nombre_proyecto_1 = nomfich_directorio(nombre_fichero_1);
	nombre_proyecto_2 = nomfich_directorio(nombre_fichero_2);

	iguales = strbas_equalsStringWithoutMatchCase(nombre_proyecto_1, nombre_proyecto_2);

	strbas_destroy(&nombre_proyecto_1);
	strbas_destroy(&nombre_proyecto_2);

	return iguales;
}

//-----------------------------------------------------------------------

static bool i_integridad_sistema_incluido_en_fuente(
						const char *nombre_fichero,
						const char *nombre_fichero_donde_se_incluye,
						enum i_tipo_fichero_t tipo_fichero_donde_se_incluye,
						char **mensaje_error)
{
	bool esCorrecto;
	
	switch(tipo_fichero_donde_se_incluye)
	{
		case I_NO_IDENTIFICADO:
		case I_DECLARACION_PURA_HXX:
		case I_DECLARACION_INTERFACE_PUBLICO_H:
		case I_DECLARACION_INTERFACE_PUBLICO_HPP:
		case I_DECLARACION_ESTRUCTURA_PUBLICA_TLH:
		case I_DECLARACION_INTERFACE_SISTEMA_SYH:
		case I_DECLARACION_INTERFACE_PRIVADO_INL:
		case I_DECLARACION_ESTRUCTURA_PRIVADA_TLI:
		case I_FUENTE_RECURSO_PRIVADO:
        case I_RECURSO_PRIVADO_GENERADO:
		{
			char *mensaje;

			mensaje = strbas_printf("Atención: sólo se puede incluir en fuentes (%s-%s)",
							nombre_fichero,
							nombre_fichero_donde_se_incluye);

			strbas_assignMessage(mensaje_error, mensaje);

			strbas_destroy(&mensaje);

			esCorrecto = false;
			break;
		}
		case I_FUENTE_C:
		case I_FUENTE_CPP:
			
			esCorrecto = true;
			break;
			
		default_error();
	}
	
	return esCorrecto;
}

//-----------------------------------------------------------------------

static bool i_integridad_sistema_doble_ambito(
						const char *nombre_fichero,
						const char *nombre_fichero_donde_se_incluye,
						enum i_tipo_fichero_t tipo_fichero_donde_se_incluye,
						bool interface_publico,
						char **mensaje_error)
{
	if (i_perteneces_al_mismo_proyecto(
						nombre_fichero,
						nombre_fichero_donde_se_incluye) == interface_publico)
	{
		char *mensaje;

		mensaje = strbas_printf("Atención: doble ámbito (%s - %s)",
						nombre_fichero,
						nombre_fichero_donde_se_incluye);

		strbas_assignMessage(mensaje_error, mensaje);

		strbas_destroy(&mensaje);

		return false;
	}
	else
		return i_integridad_sistema_incluido_en_fuente(
						nombre_fichero,
						nombre_fichero_donde_se_incluye,
						tipo_fichero_donde_se_incluye,
						mensaje_error);
}

//-----------------------------------------------------------------------

static bool i_es_proyecto_sistema(const char *nombre_fichero)
{
{
	char *nombre_proyecto, *extension;
	bool es_sistema;

	nombre_proyecto = nomfich_directorio(nombre_fichero);
	extension = nomfich_extension(nombre_proyecto);

	es_sistema = strbas_equalsStringWithoutMatchCase(extension, "sys");

	strbas_destroy(&nombre_proyecto);
	strbas_destroy(&extension);

	return es_sistema;
}
}

//-----------------------------------------------------------------------

static bool i_integridad_sistema_fuente_sistema(
						const char *nombre_fichero,
						const char *nombre_fichero_donde_se_incluye,
						enum i_tipo_fichero_t tipo_fichero_donde_se_incluye,
						char **mensaje_error)
{
	if (i_integridad_sistema_doble_ambito(
						nombre_fichero, nombre_fichero_donde_se_incluye,
						tipo_fichero_donde_se_incluye,
						i_INTERFACE_PUBLICO,
						mensaje_error) == true)
	{
		if (i_es_proyecto_sistema(nombre_fichero_donde_se_incluye) == false)
		{
			char *mensaje;

			mensaje = strbas_printf("Atención: fichero de sistema que no se incluye en libreria de sistema (%s - %s)",
							nombre_fichero,
							nombre_fichero_donde_se_incluye);

			strbas_assignMessage(mensaje_error, mensaje);

			strbas_destroy(&mensaje);

			return false;
		}
	}

	return true;
}

//-----------------------------------------------------------------------

static bool i_es_mismo_nombre_base(
						const char *nombre_fichero_1,
						const char *nombre_fichero_2)
{
	char *nombre_base_1, *nombre_base_2;
	bool iguales;

	nombre_base_1 = nomfich_nombre_base(nombre_fichero_1);
	nombre_base_2 = nomfich_nombre_base(nombre_fichero_2);

	iguales = strbas_equalsStringWithoutMatchCase(nombre_base_1, nombre_base_2);

	strbas_destroy(&nombre_base_1);
	strbas_destroy(&nombre_base_2);

	return iguales;
}

//-----------------------------------------------------------------------

bool fichsrc_integridad_sistema(
						const struct fichsrc_t *fichero,
						const struct fichsrc_t *fichero_donde_se_incluye,
						char **mensaje_error)
{
	i_integridad(fichero);
	i_integridad(fichero_donde_se_incluye);

	if (i_es_mismo_nombre_base(
						fichero->nombre_fichero,
						fichero_donde_se_incluye->nombre_fichero) == true)
	{
		if (i_perteneces_al_mismo_proyecto(
						fichero->nombre_fichero,
						fichero_donde_se_incluye->nombre_fichero) == true)
			return true;
		else
		{
			char *mensaje;

			mensaje = strbas_printf("Atención: el fichero está en dos proyectos (%s - %s)",
						fichero->nombre_fichero,
						fichero_donde_se_incluye->nombre_fichero);

			strbas_assignMessage(mensaje_error, mensaje);

			strbas_destroy(&mensaje);
			return false;
		}
	}
	else
	{
		switch(fichero->tipo_fichero)
		{
			case I_DECLARACION_PURA_HXX:

				return true;

			case I_DECLARACION_INTERFACE_PUBLICO_H:
			case I_DECLARACION_INTERFACE_PUBLICO_HPP:
			case I_DECLARACION_ESTRUCTURA_PUBLICA_TLH:

				return i_integridad_sistema_doble_ambito(
							fichero->nombre_fichero,
							fichero_donde_se_incluye->nombre_fichero,
							fichero_donde_se_incluye->tipo_fichero,
							i_INTERFACE_PUBLICO,
							mensaje_error);

			case I_DECLARACION_INTERFACE_PRIVADO_INL:
			case I_DECLARACION_ESTRUCTURA_PRIVADA_TLI:
			case I_FUENTE_RECURSO_PRIVADO:
			case I_RECURSO_PRIVADO_GENERADO:
				return i_integridad_sistema_doble_ambito(
							fichero->nombre_fichero,
							fichero_donde_se_incluye->nombre_fichero,
							fichero_donde_se_incluye->tipo_fichero,
							i_INTERFACE_PRIVADO,
							mensaje_error);

			case I_DECLARACION_INTERFACE_SISTEMA_SYH:

				return i_integridad_sistema_fuente_sistema(
							fichero->nombre_fichero,
							fichero_donde_se_incluye->nombre_fichero,
							fichero_donde_se_incluye->tipo_fichero,
							mensaje_error);

			case I_FUENTE_C:
			case I_FUENTE_CPP:
			{
				char *mensaje;

				mensaje = strbas_printf("Atención: un fuente no puede incluirse en ningún sitio (%s)",
							fichero_donde_se_incluye->nombre_fichero);

				strbas_assignMessage(mensaje_error, mensaje);

				strbas_destroy(&mensaje);

				return false;
			}
			case I_NO_IDENTIFICADO:
			default_error();
		}

		assert_return_error(bool);
	}
}

//-----------------------------------------------------------------------

bool fichscr_es_cpp(const struct fichsrc_t *fichero)
{
    i_integridad(fichero);

    switch(fichero->tipo_fichero)
    {
        case I_DECLARACION_PURA_HXX:
        case I_DECLARACION_INTERFACE_PUBLICO_H:
        case I_DECLARACION_ESTRUCTURA_PUBLICA_TLH:
        case I_DECLARACION_INTERFACE_SISTEMA_SYH:
        case I_DECLARACION_INTERFACE_PRIVADO_INL:
        case I_DECLARACION_ESTRUCTURA_PRIVADA_TLI:
        case I_FUENTE_C:
        case I_FUENTE_RECURSO_PRIVADO:
        case I_RECURSO_PRIVADO_GENERADO:

            return false;

        case I_FUENTE_CPP:
        case I_DECLARACION_INTERFACE_PUBLICO_HPP:

            return true;

        case I_NO_IDENTIFICADO:
        default_error();
   }

   assert_return_error(bool);
}

//-----------------------------------------------------------------------

static char *i_miramos_tanto_c_como_cpp(const char *directorio, const char *include)
{
    bool existe;
    char *nombre, *path_completo;

    nombre = nomfich_cambia_extension(include, "cpp");

    path_completo = nomfich_append_a_directorio(directorio, nombre);
    existe = nomfich_existe_fichero(path_completo);

    strbas_destroy(&path_completo);

    if (existe == true)
        return nombre;
    else
        return nomfich_cambia_extension(include, "c");
}

//-----------------------------------------------------------------------

char *fichsrc_obtener_fuente(const char *directorio, const char *include)
{
	char *extension;

	extension = nomfich_extension(include);

	if (strbas_equalsStringWithoutMatchCase(extension, "hxx") == true)
	{
		return i_miramos_tanto_c_como_cpp(directorio, include);
	}
	else if (strbas_equalsStrings(extension, "h") == true)
	{
		return i_miramos_tanto_c_como_cpp(directorio, include);
	}
	else if (strbas_equalsStrings(extension, "tlh") == true)
	{
		return nomfich_cambia_extension(include, "c");
	}
	else if (strbas_equalsStrings(extension, "syh") == true)
	{
		return nomfich_cambia_extension(include, "c");
	}
	else if (strbas_equalsStrings(extension, "inl") == true)
	{
        return i_miramos_tanto_c_como_cpp(directorio, include);
	}
	else if (strbas_equalsStrings(extension, "tli") == true)
	{
		return nomfich_cambia_extension(include, "c");
	}
	else if (strbas_equalsStrings(extension, "hpp") == true)
	{
		return nomfich_cambia_extension(include, "cpp");
	}

	strbas_destroy(&extension);

    assert_return_error(char *);
}

//-----------------------------------------------------------------------

static bool i_es_tipo_fichero_cabecera(enum i_tipo_fichero_t tipo_fichero)
{
	bool es_fichero_cabecera;

	switch(tipo_fichero)
	{
		case I_DECLARACION_INTERFACE_PUBLICO_H:
		case I_DECLARACION_INTERFACE_PUBLICO_HPP:
		case I_DECLARACION_INTERFACE_PRIVADO_INL:
        case I_DECLARACION_INTERFACE_SISTEMA_SYH:
        case I_DECLARACION_ESTRUCTURA_PRIVADA_TLI:
        case I_DECLARACION_PURA_HXX:
        case I_DECLARACION_ESTRUCTURA_PUBLICA_TLH:

			es_fichero_cabecera = true;
			break;

		case I_NO_IDENTIFICADO:
		case I_FUENTE_C:
		case I_FUENTE_CPP:
		case I_FUENTE_RECURSO_PRIVADO:
		case I_RECURSO_PRIVADO_GENERADO:

			es_fichero_cabecera = false;
			break;

		default_error();
	}

	return es_fichero_cabecera;
}

//-----------------------------------------------------------------------

bool fichsrc_es_include_de_fichero(const struct fichsrc_t *fichero, const struct fichsrc_t *fichero_es_include)
{
	i_integridad(fichero);
	i_integridad(fichero_es_include);

	if (i_es_tipo_fichero_cabecera(fichero_es_include->tipo_fichero) == false
			|| i_es_fuente(fichero->tipo_fichero) == false)
	{
		return false;
	}
	else
	{
		bool es_include;
		char *nombre_fichero_sin_extension, *nombre_fichero_es_include_sin_extension;

		nombre_fichero_sin_extension = nomfich_nombre_sin_extension(fichero->nombre_fichero);
		nombre_fichero_es_include_sin_extension = nomfich_nombre_sin_extension(fichero_es_include->nombre_fichero);

		es_include = strbas_equalsStrings(nombre_fichero_sin_extension, nombre_fichero_es_include_sin_extension);

		strbas_destroy(&nombre_fichero_sin_extension);
		strbas_destroy(&nombre_fichero_es_include_sin_extension);

		return es_include;
	}
}

//-----------------------------------------------------------------------

void fichsrc_copia_a_directorio(
						const struct fichsrc_t *fichero_src,
						const char *directorio_fuentes_make_source,
						struct marco_t *marco)
{
	char *fichero_base, *mensaje;
	char *nombre_fichero_en_directorio_mak;

	fichero_base = nomfich_nombre_base_con_extension(fichero_src->nombre_fichero);
	nombre_fichero_en_directorio_mak = nomfich_append_a_directorio(directorio_fuentes_make_source, fichero_base);

	mensaje = strbas_printf("Copiando %s ...", fichero_src->nombre_fichero);
	marco_lanza_mensaje(marco, MARCO_MENSAJE_INFORMACION, mensaje);
	strbas_destroy(&mensaje);

	nomfich_copiar_ficheros(fichero_src->nombre_fichero, nombre_fichero_en_directorio_mak);

	strbas_destroy(&fichero_base);
	strbas_destroy(&nombre_fichero_en_directorio_mak);
}
