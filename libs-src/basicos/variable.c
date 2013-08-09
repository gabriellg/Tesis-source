// Definici�n de una variable

#include "variable.inl"
#include "variable.tli"

#include "asrtbas.h"
#include "memory.h"
#include "arrcad.h"
#include "strbas.h"


//---------------------------------------------------------

static void i_integridad(const struct variable_t *variable)
{
	assert_no_null(variable);
	assert_no_null(variable->nombre);
	assert_no_null(variable->valores);
}

//---------------------------------------------------------

static struct variable_t *i_crea(
						const char *nombre_variable, 
						const ArrString *valores)
{
	struct variable_t *variable;
		
	variable = MALLOC(struct variable_t);
	
	variable->nombre = strbas_copyString(nombre_variable);
	variable->valores = arrcad_Copia(valores);
		
	i_integridad(variable);
		
	return variable;
}

//---------------------------------------------------------

struct variable_t *variable_crea_defecto(const char *nombre_variable)
{
	struct variable_t *variable;
	ArrString *valores;
		
	valores = arrcad_CreaString(0);
	
	variable = i_crea(nombre_variable, valores);
	
	arrcad_Destruye(&valores);
	
	return variable;
}

//---------------------------------------------------------

void variable_destruye(struct variable_t **variable)
{
	assert_no_null(variable);
	i_integridad(*variable);
	
	strbas_destroy(&(*variable)->nombre);
	arrcad_Destruye(&(*variable)->valores);
	
	FREE_T(variable, struct variable_t);
}

//---------------------------------------------------------

struct variable_t *variable_copia(const struct variable_t *variable)
{
	assert_no_null(variable);
	return i_crea(variable->nombre, variable->valores);
}

//---------------------------------------------------------

bool variable_es_variable(const struct variable_t *variable, const char *nombre)
{
	i_integridad(variable);
	return strbas_equalsStringWithoutMatchCase(variable->nombre, nombre);
}

//---------------------------------------------------------

void variable_append_valor(const struct variable_t *variable, const char *nuevo_valor)
{
	i_integridad(variable);
	
	if (arrcad_Buscar(variable->valores, nuevo_valor, NULL, NULL) == false)
		arrcad_Anyadir(variable->valores, nuevo_valor);
}

//---------------------------------------------------------

