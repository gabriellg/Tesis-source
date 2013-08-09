//arrcad.c
// Array de cadenas.

#include "arrcad.h"

#include "asrtbas.h"
#include "array.h"
#include "strbas.h"

//--------------------------------------------------------------------------

ArrString *arrcad_CreaString(unsigned long numElementos)
{
	return (ArrString *)array_CreaPrivada("char *", numElementos);
}

//--------------------------------------------------------------------------

void arrcad_Destruye(ArrString **arr)
{
	array_DestruyePrivada("char *", (struct Array_t **)arr,
						  (FuncLiberaPtr)strbas_destroy);
}

//--------------------------------------------------------------------------

ArrString *arrcad_Copia(const ArrString *arr)
{
	return (ArrString *)array_CopiaPrivada("char *", (const struct Array_t *)arr,
												  (FuncCopiaPtr)strbas_copyString);
}

//--------------------------------------------------------------------------

const char *arrcad_Get(const ArrString *arr, unsigned long pos)
{
	return (const char *)array_ObtenerPrivada("char *", (const struct Array_t *)arr, pos);
}

//--------------------------------------------------------------------------

char *arrcad_GetCopia(const ArrString *arr, unsigned long pos)
{
	const char *str;

	str = (const char *)array_ObtenerPrivada("char *", (const struct Array_t *)arr, pos);
	return strbas_copyString(str);
}

//--------------------------------------------------------------------------

void arrcad_Set(ArrString *arr, unsigned long pos, const char *cad)
{
	char *cadena;

	cadena = (char *)array_ObtenerPrivada("char *", (const struct Array_t *)arr, pos);
	if (cadena != NULL)
		strbas_destroy(&cadena);

	array_PonerPrivada("char *", (struct Array_t *)arr, pos, strbas_copyString(cad));
}

//--------------------------------------------------------------------------

void arrcad_SetDestruyendoCadena(ArrString *arr, unsigned long pos, char **cad)
{
	char *cadena;
	
	assert_no_null(cad);

	cadena = (char *)array_ObtenerPrivada("char *", (const struct Array_t *)arr, pos);
	if (cadena != NULL)
		strbas_destroy(&cadena);

	array_PonerPrivada("char *", (struct Array_t *)arr, pos, *cad);
	*cad = NULL;
}

//--------------------------------------------------------------------------

void arrcad_Anyadir(ArrString *arr, const char *cad)
{
	array_AnyadirPrivada("char *", (struct Array_t *)arr, strbas_copyString(cad));
}

//--------------------------------------------------------------------------

void arrcad_AnyadirDestruyendoCadena(ArrString *arr, char **cad)
{
	assert_no_null(cad);
	array_AnyadirPrivada("char *", (struct Array_t *)arr, *cad);
	*cad = NULL;
}

//--------------------------------------------------------------------------

void arrcad_Borrar(ArrString *arr, unsigned long pos)
{
	array_BorrarPrivada("char *", (struct Array_t *)arr, pos,
						(FuncLiberaPtr)strbas_destroy);
}

//--------------------------------------------------------------------------

char *arrcad_popCadena(ArrString *arr)
{
	char *strResult;
	unsigned long numElementos, indLast;
	const char *str;

	numElementos = array_numElementosPrivada("char *", (const struct Array_t *)arr);
	assert(numElementos > 0);

	indLast = numElementos - 1;

	str = (const char *)array_ObtenerPrivada("char *", (const struct Array_t *)arr, indLast);
	strResult = strbas_copyString(str);
	array_BorrarPrivada("char *", (struct Array_t *)arr, indLast, (FuncLiberaPtr)strbas_destroy);

	return strResult;
}

//--------------------------------------------------------------------------

const char *arrcad_ultimaCadena(const ArrString *arr)
{
	unsigned long numElementos, indLast;

	numElementos = array_numElementosPrivada("char *", (const struct Array_t *)arr);
	assert(numElementos > 0);

	indLast = numElementos - 1;

	return (const char *)array_ObtenerPrivada("char *", (const struct Array_t *)arr, indLast);
}

//--------------------------------------------------------------------------

bool arrcad_Buscar(const ArrString *arr, const char *cad, arrcad_FPtr_es_cadena func_buscar, unsigned long *pos)
{
	if (func_buscar != NULL)
		return array_BuscarPrivada("char *", (const struct Array_t *)arr, cad,
								   (FuncBuscarPtr)func_buscar, pos);
	else
		return array_BuscarPrivada("char *", (const struct Array_t *)arr, cad,
								   (FuncBuscarPtr)strbas_equalsStrings, pos);
}

//--------------------------------------------------------------------------

unsigned long arrcad_numElementos(const ArrString *arr)
{
	return array_numElementosPrivada("char *", (const struct Array_t *)arr);
}

//--------------------------------------------------------------------------

void arrcad_Concatena(ArrString *arr, const ArrString *array_a_anyadir)
{
	array_ConcatenarPrivada("char *", (struct Array_t *)arr, (const struct Array_t *)array_a_anyadir, (FuncCopiaPtr)strbas_copyString);
}

//--------------------------------------------------------------------------

static char *i_texto_separador_por_separadores(const ArrString *arr, const char *separador)
{
	unsigned long i, num_elementos;
	char *cadena;

	cadena = strbas_copyString("");

	num_elementos = array_numElementosPrivada("char *", (const struct Array_t *)arr);

	for (i = 0; i < num_elementos; i++)
	{
		const char *texto;

		texto = (const char *)array_ObtenerPrivada("char *", (const struct Array_t *)arr, i);
		strbas_concatenate(&cadena, texto);
		strbas_concatenate(&cadena, separador);
	}

	return cadena;
}

//--------------------------------------------------------------------------

char *arrcad_texto_separado_por_espacios(const ArrString *arr)
{
    const char *separador;

    separador = " ";

	return i_texto_separador_por_separadores(arr, separador);
}

//--------------------------------------------------------------------------

char *arrcad_texto_separado_por_comas(const ArrString *arr)
{
    const char *separador;

    separador = ",";

	return i_texto_separador_por_separadores(arr, separador);
}

//--------------------------------------------------------------------------

ArrString *arrcad_separa_palabras_por_comas(char *cadena)
{
    const char *separador;
    char *cadena_siguiente;
    struct Array_t *parametros;

    separador = ",";

    cadena_siguiente = strbas_nextToken(cadena, separador);
    assert_no_null(cadena_siguiente);

    parametros = array_CreaPrivada("char *", 1);
    array_PonerPrivada("char *", (struct Array_t *)parametros, 0, strbas_copyString(cadena_siguiente));

	while (cadena_siguiente != NULL)
    {
        cadena_siguiente = strbas_nextToken(NULL, ",");

        if (cadena_siguiente != NULL)
            array_AnyadirPrivada("char *", (struct Array_t *)parametros, strbas_copyString(cadena_siguiente));
    }

    return (ArrString *)parametros;
}
