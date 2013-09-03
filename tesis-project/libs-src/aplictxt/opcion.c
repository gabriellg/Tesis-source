#include "opcion.h"
#include "opcion.inl"

#include "arrcad.h"
#include "asrtbas.h"
#include "strbas.h"
#include "memory.h"
#include "array.h"
#include "marco.h"

enum i_tipo_opcion_t
{
	i_OPCION_EJECUTAR,
	i_OPCION_CON_PARAMETRO,
	i_SUBMENU
};


struct opcion_t
{
	enum i_tipo_opcion_t tipo;
	
	char *id_ejecutar;
	char *titulo_opcion;
	char *help;
	
	opcion_FPtr_ejecutar funcion_ejecutar;
	opcion_FPtr_ejecutar_con_parametro funcion_ejecutar_con_parametro;

	opcion_FPtr_se_puede_ejecutar funcion_se_puede_ejecutar_opc;

	ArrTipo(opcion_t) *opciones_submenu;
};

//-----------------------------------------------------------------------

static void i_integridad(const struct opcion_t *opcion)
{
	assert_no_null(opcion);
	
	assert_no_null(opcion->id_ejecutar);
	assert_no_null(opcion->titulo_opcion);
	assert_no_null(opcion->help);
	
	switch(opcion->tipo)
	{
		case i_OPCION_EJECUTAR:
			assert_no_null(opcion->funcion_ejecutar);
			break;
		
		case i_SUBMENU:
			assert_no_null(opcion->opciones_submenu);
			assert(array_numElementos(opcion_t, opcion->opciones_submenu) > 0);
			break;
		
		case i_OPCION_CON_PARAMETRO:
			assert_no_null(opcion->funcion_ejecutar_con_parametro);
			break;

		default_error();
	}
}

//-----------------------------------------------------------------------

static struct opcion_t *i_crea(
						enum i_tipo_opcion_t tipo,
						const char *id_ejecutar,
						const char *titulo_opcion,
						const char *help,
						opcion_FPtr_ejecutar funcion_ejecutar,
						opcion_FPtr_ejecutar_con_parametro funcion_ejecutar_con_parametro,
						opcion_FPtr_se_puede_ejecutar funcion_se_puede_ejecutar_opc,
						ArrTipo(opcion_t) **opciones_submenu)
{
	struct opcion_t *opcion;
		
	opcion = MALLOC(struct opcion_t);
	
	opcion->tipo = tipo;
	
	opcion->id_ejecutar = strbas_copyString(id_ejecutar);
	opcion->titulo_opcion = strbas_copyString(titulo_opcion);
	opcion->help = strbas_copyString(help);
	
	opcion->funcion_ejecutar = funcion_ejecutar;
	opcion->funcion_ejecutar_con_parametro = funcion_ejecutar_con_parametro;
	opcion->funcion_se_puede_ejecutar_opc = funcion_se_puede_ejecutar_opc;
	
	if (opciones_submenu != NULL)
		opcion->opciones_submenu = ASSIGN_PP(opciones_submenu, ArrTipo(opcion_t));
	else
		opcion->opciones_submenu = NULL;

	i_integridad(opcion);
	
	return opcion;
}

//-----------------------------------------------------------------------

struct opcion_t *opcion_no_usar_crea_opcion_simple(
						const char *id_ejecutar,
						const char *titulo_opcion,
						const char *help,
						opcion_FPtr_ejecutar funcion_ejecutar,
						opcion_FPtr_se_puede_ejecutar funcion_se_puede_ejecutar_opc)
{
	enum i_tipo_opcion_t tipo;
	ArrTipo(opcion_t) **opciones_submenu;
	opcion_FPtr_ejecutar_con_parametro funcion_ejecutar_con_parametro;
	
	tipo = i_OPCION_EJECUTAR;
	opciones_submenu = NULL;
	funcion_ejecutar_con_parametro = NULL;
	
	return i_crea(tipo, id_ejecutar, titulo_opcion, help,
						funcion_ejecutar, funcion_ejecutar_con_parametro, funcion_se_puede_ejecutar_opc,
						opciones_submenu);
}

//-----------------------------------------------------------------------

struct opcion_t *opcion_no_usar_crea_opcion_con_parametro(
						const char *id_ejecutar,
						const char *titulo_opcion,
						const char *help,
						opcion_FPtr_ejecutar_con_parametro funcion_ejecutar_con_parametro,
						opcion_FPtr_se_puede_ejecutar funcion_se_puede_ejecutar_opc)
{
	enum i_tipo_opcion_t tipo;
	ArrTipo(opcion_t) **opciones_submenu;
	opcion_FPtr_ejecutar funcion_ejecutar;
	
	tipo = i_OPCION_CON_PARAMETRO;
	opciones_submenu = NULL;
	funcion_ejecutar = NULL;
	
	return i_crea(tipo, id_ejecutar, titulo_opcion, help,
						funcion_ejecutar,
						funcion_ejecutar_con_parametro,
						funcion_se_puede_ejecutar_opc, opciones_submenu);
}

//-----------------------------------------------------------------------

void opcion_destruye(struct opcion_t **opcion)
{
	assert_no_null(opcion);
	i_integridad(*opcion);
	
	strbas_destroy(&((*opcion)->id_ejecutar));
	strbas_destroy(&((*opcion)->titulo_opcion));
	strbas_destroy(&((*opcion)->help));
	
	switch((*opcion)->tipo)
	{
		case i_OPCION_EJECUTAR:
		case i_OPCION_CON_PARAMETRO:
			break;
		case i_SUBMENU:
			array_Destruye(opcion_t, &((*opcion)->opciones_submenu), opcion_destruye);
			break;
		default_error();
	}
	
	FREE_T(opcion, struct opcion_t);
}

//-----------------------------------------------------------------------

static bool i_opcion_ejecutar(const struct opcion_t *opcion,const char *ejecutar)
{
	assert_no_null(opcion);
	return strbas_equalsStrings(opcion->id_ejecutar, ejecutar);
}

//-----------------------------------------------------------------------

bool opcion_es_opcion(const ArrTipo(opcion_t) *opciones, const char *ejecutar)
{
	return array_Buscar(opcion_t, opciones, char, ejecutar, i_opcion_ejecutar, NULL);
}

//-----------------------------------------------------------------------

static bool i_se_puede_ejecutar_opcion(
				struct marco_t *marco,
				struct opcion_item_datos_t *datos_opcion,
				opcion_FPtr_se_puede_ejecutar funcion_se_puede_ejecutar)
{
	bool se_puede_ejecutar;

	se_puede_ejecutar = true;

	if (funcion_se_puede_ejecutar != NULL)
	{
		char *mensaje_error;

		mensaje_error = NULL;
		se_puede_ejecutar = funcion_se_puede_ejecutar(datos_opcion, &mensaje_error);

		if (se_puede_ejecutar == false)
		{
			marco_lanza_mensaje(marco, MARCO_MENSAJE_ERROR, mensaje_error);
			strbas_destroy(&mensaje_error);
		}
	}
	else
		se_puede_ejecutar = true;

	return se_puede_ejecutar;
}

//-----------------------------------------------------------------------

void opcion_ejecuta(
				const ArrTipo(opcion_t) *opciones, 
				struct marco_t *marco,
				const char *ejecutar,
				struct opcion_item_datos_t *datos_opcion,
				const char *parametro)
{
	bool encontrado;
	const struct opcion_t *opcion;
	unsigned long ind_opcion;
	
	encontrado = array_Buscar(opcion_t, opciones, char, ejecutar, i_opcion_ejecutar, &ind_opcion);
	assert(encontrado == true);
	
	opcion = array_Get(opcion_t, opciones, ind_opcion);
	i_integridad(opcion);
	
	if (i_se_puede_ejecutar_opcion(
					marco, datos_opcion,
					opcion->funcion_se_puede_ejecutar_opc) == true)
	{
		switch(opcion->tipo)
		{
			case i_OPCION_EJECUTAR:
				opcion->funcion_ejecutar(marco, datos_opcion);
				break;

			case i_OPCION_CON_PARAMETRO:
				assert_no_null(opcion->funcion_ejecutar_con_parametro);
				opcion->funcion_ejecutar_con_parametro(marco, datos_opcion, parametro);
				break;

			case i_SUBMENU:
			default_error();
		}
	}
}

//-----------------------------------------------------------------------

static char *i_texto_help(
						const char *id_ejecutar,
						const char *help)
{
	assert_no_null(id_ejecutar);
	assert_no_null(help);

	return strbas_printf("%s: %s", id_ejecutar, help);
}

//-----------------------------------------------------------------------

ArrString *opcion_obtener_todo_help(const ArrTipo(opcion_t) *opciones)
{
	unsigned long ind, num_opciones;
	ArrString *help_opciones;
	
	num_opciones = array_numElementos(opcion_t, opciones);
	help_opciones = arrcad_CreaString(num_opciones);
	
	for (ind = 0; ind < num_opciones; ind++)
	{
		const struct opcion_t *opcion;
		char *help_con_titulo;
			
		opcion = array_Get(opcion_t, opciones, ind);
		assert_no_null(opcion);
		
		help_con_titulo = i_texto_help(opcion->id_ejecutar, opcion->help);
		arrcad_Set(help_opciones, ind, help_con_titulo);
		strbas_destroy(&help_con_titulo);
	}

	return help_opciones;
}

//-----------------------------------------------------------------------

ArrString *opcion_obtener_help_comandos(
						const ArrTipo(opcion_t) *opciones,
						const ArrString *comandos)
{
	unsigned long i, num_comandos;
	ArrString *help_opciones;
	
	num_comandos = arrcad_numElementos(comandos);
	help_opciones = arrcad_CreaString(0);
	
	for (i = 0; i < num_comandos; i++)
	{
		bool encontrado;
		const char *comando;
		unsigned long ind_opcion;
		const struct opcion_t *opcion;
		char *help_con_titulo;
			
		comando = arrcad_Get(comandos, i);
		encontrado = array_Buscar(opcion_t, opciones, char, comando, i_opcion_ejecutar, &ind_opcion);
		assert(encontrado == true);
		
		opcion = array_Get(opcion_t, opciones, ind_opcion);
		assert_no_null(opcion);
		
		help_con_titulo = i_texto_help(opcion->id_ejecutar, opcion->help);
		arrcad_Anyadir(help_opciones, help_con_titulo);
		strbas_destroy(&help_con_titulo);
	}

	return help_opciones;
}

