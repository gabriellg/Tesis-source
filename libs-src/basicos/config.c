// Definici�n estandar para leer ficheros de texto de configuracion

#include "config.h"

#include "variable.inl"
#include "variable.tli"

#include "asrtbas.h"
#include "memory.h"
#include "strbas.h"
#include "arrcad.h"
#include "array.h"
#include "nomfich.h"
#include "filetxt.hpp"

struct config_t
{
	ArrTipo(variable_t) *variables;
};

//---------------------------------------------------------

static void i_integridad(const struct config_t *configuracion)
{
	assert_no_null(configuracion);
	assert_no_null(configuracion->variables);
}

//---------------------------------------------------------

static struct config_t *i_crea(const ArrTipo(variable_t) *variables)
{
	struct config_t *configuracion;
		
	configuracion = MALLOC(struct config_t);
		
	configuracion->variables = array_Copia(variable_t, variables, variable_copia);
	
	return configuracion;
}

//---------------------------------------------------------

static char *i_elimina_espacios(const char *cadena)
{
	char *limpia_a_derecha, *limpia_espacios;
	
	limpia_a_derecha = strbas_RTrim(cadena);
	limpia_espacios = strbas_LTrim(limpia_a_derecha);
	
	strbas_destroy(&limpia_a_derecha);
	
	return limpia_espacios;
}

//---------------------------------------------------------

static bool i_es_variable(const char *texto, char **nombre, char **valor)
{
	char *cadena_sin_espacios;
	bool es_variable;

	assert_no_null(nombre);
	assert_no_null(valor);
	
	cadena_sin_espacios = strbas_LTrim(texto);
	
	if (strbas_length(cadena_sin_espacios) > 0)
	{
		if (cadena_sin_espacios[0] != '#')
		{
			char *nombre_loc, *valor_loc;
		
			nombre_loc = strbas_initialExtractUntilChar(texto, '=');
			valor_loc = strbas_endExtractFromChar(texto, '=');
	
			if (nombre_loc != NULL && valor_loc != NULL)
			{
				*nombre = i_elimina_espacios(nombre_loc);
				*valor = i_elimina_espacios(valor_loc);
				es_variable = true;
			}
			else
				es_variable = false;
			
			if (nombre_loc != NULL)
				strbas_destroy(&nombre_loc);
			
			if (valor_loc != NULL)
				strbas_destroy(&valor_loc);
				
		}
		else
			es_variable = false;
	}
	else
		es_variable = false;

	strbas_destroy(&cadena_sin_espacios);
	
	return es_variable;
}

//---------------------------------------------------------

static void i_lee_variables(ArrString *textos, ArrTipo(variable_t) *variables)
{
	unsigned long i, num_lineas;
	
	num_lineas = arrcad_numElementos(textos);
	
	for (i = 0; i < num_lineas; i++)
	{
		const char *texto;
		char *nombre, *valor;
		
		texto = arrcad_Get(textos, i);
		
		if (i_es_variable(texto, &nombre, &valor) == true)
		{
			struct variable_t *variable;
			unsigned long pos;
			
			if (array_Buscar(variable_t, variables, char, nombre, variable_es_variable, &pos) == true)
				variable = array_Get(variable_t, variables, pos);
			else
			{
				variable = variable_crea_defecto(nombre);
				array_Anyadir(variable_t, variables, variable);
			}
			
			variable_append_valor(variable, valor);
			
			strbas_destroy(&nombre);
			strbas_destroy(&valor);
		}
	}
}

//---------------------------------------------------------

struct config_t *config_lee_configuracion(const char *nombre_fichero)
{
	struct config_t *configuracion;
	ArrTipo(variable_t) *variables;
	
	variables = array_CreaPuntero(0, variable_t);
	
	if (nomfich_existe_fichero(nombre_fichero) == true)
	{
		ArrString *fichero;
			
		fichero = filetxt_leer_fichero_texto(nombre_fichero);
		
		i_lee_variables(fichero, variables);
		
		arrcad_Destruye(&fichero);
	}
	
	configuracion = i_crea(variables);
	
	array_Destruye(variable_t, &variables, variable_destruye);
	
	return configuracion;
}

//---------------------------------------------------------

struct config_t *config_copia(const struct config_t *configuracion)
{
	assert_no_null(configuracion);
	return i_crea(configuracion->variables);
}

//---------------------------------------------------------

void config_destruye(struct config_t **configuracion)
{
	assert_no_null(configuracion);
	i_integridad(*configuracion);
	
	array_Destruye(variable_t, &((*configuracion)->variables), variable_destruye);
	
	FREE_T(configuracion, struct config_t);
}

//---------------------------------------------------------

bool config_existe_variable(
					const struct config_t *configuracion, 
					const char *nombre_variable)
{
	i_integridad(configuracion);
	return array_Buscar(variable_t, configuracion->variables, char, nombre_variable, variable_es_variable, NULL);
}

//---------------------------------------------------------

ArrString *config_obtener_valor(
					const struct config_t *configuracion,
					const char *nombre_variable)
{
	bool existe_variable;
	unsigned long pos;
	struct variable_t *variable;
	
	i_integridad(configuracion);
	
	existe_variable = array_Buscar(variable_t, configuracion->variables, char, nombre_variable, variable_es_variable, &pos);
	assert(existe_variable == true);
	
	variable = array_Get(variable_t, configuracion->variables, pos);
	assert_no_null(variable);
	
	return arrcad_Copia(variable->valores);
}
