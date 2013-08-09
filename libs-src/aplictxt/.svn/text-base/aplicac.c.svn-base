#include "aplicac.h"

#include "opcion.inl"

#include "marco.syh"
#include "marco.h"
#include "asrtbas.h"
#include "stdio.h"
#include "arrcad.h"
#include "strbas.h"
#include "nomfich.h"

static const char *i_CABECERA_ERROR = "------- ERROR ---------";
static const char *i_FIN_ERROR = "-----------------------";

//-----------------------------------------------------------------------

static void i_lanza_mensaje(struct datos_txt_t *datos, enum marco_tipo_mensaje_t tipo_mensaje, const char *mensaje)
{
	assert(datos == NULL);

	switch(tipo_mensaje)
	{
		case MARCO_MENSAJE_INFORMACION:

			printf("%s\n", mensaje);
			break;

		case MARCO_MENSAJE_ERROR:

			printf("%s\n", i_CABECERA_ERROR);
			printf("%s\n", mensaje);
			printf("%s\n", i_FIN_ERROR);
			break;

		default_error();
	}
}

//-----------------------------------------------------------------------

static void i_lanza_explicacion(struct datos_txt_t *datos, enum marco_tipo_mensaje_t tipo_mensaje, const ArrString *explicacion)
{
	unsigned long i, num_textos;
	
	assert(datos == NULL);

	switch(tipo_mensaje)
	{
		case MARCO_MENSAJE_INFORMACION:

			break;

		case MARCO_MENSAJE_ERROR:

			printf("%s\n", i_CABECERA_ERROR);
			break;

		default_error();
	}

	num_textos = arrcad_numElementos(explicacion);
	
	for (i = 0; i < num_textos; i++)
	{
		const char *mensaje;
		
		mensaje = arrcad_Get(explicacion, i);
		printf("%s\n", mensaje);
	}

	switch(tipo_mensaje)
	{
		case MARCO_MENSAJE_INFORMACION:

			break;

		case MARCO_MENSAJE_ERROR:

			printf("%s\n", i_FIN_ERROR);
			break;

		default_error();
	}
}

//-----------------------------------------------------------------------

static bool i_lanza_pregunta(struct datos_txt_t *datos, const char *pregunta)
{
	char car;

	assert(datos == NULL);

	do
	{
		printf("\n");
		printf("%s [s/n] ", pregunta);

		car = getchar();
	}
	while(car != 's' && car != 'S' && car != 'N' && car != 'n');

	if (car == 's' || car == 'S')
		return true;
	else
		return false;

}

//-----------------------------------------------------------------------

static void i_imprimir_help(
				const char *titulo_aplicacion,
				const char *explicacion,
				const ArrString *help_opciones)
{
	unsigned long num_help, ind;
	
	printf("%s\n", titulo_aplicacion);
	printf("%s\n", explicacion);
	printf("\n");
	
	num_help = arrcad_numElementos(help_opciones);
	
	for (ind = 0; ind < num_help; ind++)
	{
		const char *help;
		
		help = arrcad_Get(help_opciones, ind);
		printf("%s\n", help);
	}
}				

//-----------------------------------------------------------------------

static void i_imprimir_todo_help(
				const char *titulo_aplicacion,
				const char *explicacion,
				const ArrTipo(opcion_t) *opciones)
{
	ArrString *help_opciones;
	
	help_opciones = opcion_obtener_todo_help(opciones);

	i_imprimir_help(titulo_aplicacion, explicacion, help_opciones);
	
	arrcad_Destruye(&help_opciones);
}				

//-----------------------------------------------------------------------

static void i_imprimir_help_selectivo(
				const char *titulo_aplicacion,
				const char *explicacion,
				const ArrTipo(opcion_t) *opciones,
				const ArrString *comandos)
{
	ArrString *help_opciones;
	
	help_opciones = opcion_obtener_help_comandos(opciones, comandos);

	i_imprimir_help(titulo_aplicacion, explicacion, help_opciones);
	
	arrcad_Destruye(&help_opciones);
}

//-----------------------------------------------------------------------

static void i_obtener_solo_comando(
						const char *parametro,
						char **comando,
						char **parametro_comando)
{
	assert(parametro[0] == '-');
	assert_no_null(comando);
	
	if (strbas_getStringBetweenChar(parametro, '-', ':', comando) == true)
	{
		if (parametro_comando != NULL)
			*parametro_comando = strbas_endExtractFromChar(parametro, ':');
	}
	else
	{
		*comando = strbas_copyString(&parametro[1]);
		if (parametro_comando != NULL)
			*parametro_comando = strbas_copyString("");
	}
}

//-----------------------------------------------------------------------

static ArrString *i_obtener_comandos(
							int argc, char *argv[], 
							const ArrTipo(opcion_t) *opciones,
							bool *es_help)
{
	ArrString *comandos;
	int i;
	bool es_help_loc;

	assert_no_null(es_help);
	assert_no_null(argv);
	comandos = arrcad_CreaString(0);
	es_help_loc = false;
	
	for (i = 1; i < argc; i++)
	{
		assert_no_null(argv[i]);
		if (argv[i][0] == '-')
		{
			char *comando;
			
			i_obtener_solo_comando(argv[i], &comando, NULL);
			
			if (strbas_equalsStrings(comando, "help") == true)
				es_help_loc = true;
			else
			{
				if (arrcad_Buscar(comandos, comando, NULL, NULL) == false
						&& opcion_es_opcion(opciones, comando) == true)
				{
					arrcad_Anyadir(comandos, argv[i]);
				}
			}
			
			strbas_destroy(&comando);
		}
	}

	*es_help = es_help_loc;
	
	return comandos;
}

//-----------------------------------------------------------------------

static ArrString *i_obtener_parametros(int argc, char *argv[])
{
	ArrString *parametros;
	int i;

	assert_no_null(argv);
	parametros = arrcad_CreaString(0);
	
	for (i = 1; i < argc; i++)
	{
		assert_no_null(argv[i]);
		if (argv[i][0] != '-')
			arrcad_Anyadir(parametros, argv[i]);
	}
	
	return parametros;
}

//-----------------------------------------------------------------------

static void i_ejecutar_comandos(
				struct marco_t *marco,
				const ArrString *comandos,
				const ArrTipo(opcion_t) *opciones,
				struct opcion_item_datos_t *datos_opcion)
{
	unsigned long i, num_comandos;
	
	num_comandos = arrcad_numElementos(comandos);
	
	for (i = 0; i < num_comandos; i++)
	{
		const char *argumento;
		char *comando, *parametro;
		
		argumento = arrcad_Get(comandos, i);
		
		i_obtener_solo_comando(argumento, &comando, &parametro);
		
		opcion_ejecuta(opciones, marco, comando, datos_opcion, parametro);
		
		printf("---> Fin de %s \n", comando);

		strbas_destroy(&comando);
		strbas_destroy(&parametro);
	}
}

//-----------------------------------------------------------------------

static void i_datos_aplicacion(const char *ejecutable, const char *titulo_aplicacion, const char *txt_version)
{
	char *fichero_ejecutable;

	fichero_ejecutable = nomfich_nombre_base(ejecutable);

	printf("----- %s -----\n", titulo_aplicacion);
	printf("Versión: %s \n", txt_version);
	printf("Para ver la ayuda ponga '%s [-help]'\n", fichero_ejecutable);

	strbas_destroy(&fichero_ejecutable);
}

//-----------------------------------------------------------------------

int aplicac_no_usar_lanzar(
				int argc, 
				char *argv[], 
				const char *titulo_aplicacion,
				const char *explicacion,
				const char *txt_version,
				aplicac_FPtr_lee_datos funcion_lee_datos,
				aplicac_FPtr_destruye funcion_destruye,
				const ArrTipo(opcion_t) *opciones)
{
	ArrString *comandos;
	int retorno;
	bool es_help;
	struct marco_t *marco;
		
	marco = marco_crea(datos_txt_t, NULL, i_lanza_mensaje, i_lanza_explicacion, i_lanza_pregunta);
	
	comandos = i_obtener_comandos(argc, argv, opciones, &es_help);
	
	if (es_help == true)
	{
		i_datos_aplicacion(argv[0], titulo_aplicacion, txt_version);

		if (arrcad_numElementos(comandos) == 0)
			i_imprimir_todo_help(titulo_aplicacion, explicacion, opciones);
		else
			i_imprimir_help_selectivo(titulo_aplicacion, explicacion, opciones, comandos);
		
		retorno = 0;
	}
	else
	{
		if (arrcad_numElementos(comandos) > 0)
		{
			struct opcion_item_datos_t *datos_opcion;
			ArrString *parametros;
					
			parametros = i_obtener_parametros(argc, argv);
            
            if (funcion_lee_datos != NULL)
                datos_opcion = funcion_lee_datos(marco, argc, argv, parametros);
            else
                datos_opcion = NULL;
			
			i_ejecutar_comandos(marco, comandos, opciones, datos_opcion);
			assert_no_null(funcion_destruye);

			if (funcion_destruye != NULL)
				funcion_destruye(&datos_opcion);
			else
				assert(datos_opcion == NULL);

			retorno = 0;
		}
		else
			retorno = 1;
		
		if (retorno == 1)
			i_datos_aplicacion(argv[0], titulo_aplicacion, txt_version);
	}
	
	marco_destruye(&marco);
	
	arrcad_Destruye(&comandos);
	
	return retorno;
}
