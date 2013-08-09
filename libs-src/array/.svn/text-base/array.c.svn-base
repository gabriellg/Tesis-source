// M�dulo:array.c
// Descripci�n: Define un array de punteros.

#include "array.h"

#include "asrtbas.h"
#include "memory.h"
#include "strbas.h"
#include "limits.h"

struct Array_t 
{
	char *tipo;
	unsigned long numElementos;
	void **array;
};

//-----------------------------------------------------------------------

static void i_integridad(const struct Array_t *arr, const char *tipo)
{
	assert_no_null(arr);
	if (arr->numElementos > 0)
		assert_no_null(arr->array);
	else
		assert(arr->array == NULL);

	assert(strbas_equalsStrings(tipo, arr->tipo) == true);
}

//-----------------------------------------------------------------------

static void i_Destruye(struct Array_t **arr)
{
	assert_no_null(arr);
	assert_no_null(*arr);

	if ((*arr)->numElementos > 0)
		FREE_T(&((*arr)->array), void *);
	else
		assert((*arr)->array == NULL);

	strbas_destroy(&((*arr)->tipo));

	FREE_T(arr, struct Array_t);
}

//-----------------------------------------------------------------------

struct Array_t *array_CreaPrivada(const char *tipo, unsigned long numElementos)
{
	struct Array_t *arr = MALLOC(struct Array_t);	

	arr->numElementos = numElementos;
	if (arr->numElementos > 0)
		arr->array = CALLOC(numElementos, void *);
	else
		arr->array = NULL;

	arr->tipo = strbas_copyString(tipo);

	i_integridad(arr, tipo);

	return arr;
}

//-----------------------------------------------------------------------

void array_DestruyePrivada(const char *tipo, struct Array_t **arr,FuncLiberaPtr func)
{
	unsigned long i;

	i_integridad(*arr, tipo);

	if (func != NULL)
	{
		for (i = 0; i < (*arr)->numElementos; i++)
			func(&((*arr)->array[i]));
	}

	i_Destruye(arr);
}

//-----------------------------------------------------------------------

static void *i_copia_objeto(FuncCopiaPtr func, void *elemento)
{
	if (func != NULL)
		return func(elemento);
	else
		return elemento;
}

//-----------------------------------------------------------------------

struct Array_t *array_CopiaPrivada(const char *tipo, const struct Array_t *arr,FuncCopiaPtr func)
{
	unsigned long i;
	struct Array_t *copia;

	i_integridad(arr, tipo);

	copia = array_CreaPrivada(tipo, arr->numElementos);

	for(i = 0; i < arr->numElementos; i++)
		copia->array[i] = i_copia_objeto(func, arr->array[i]);

	return copia;
}

//-----------------------------------------------------------------------

void *array_ObtenerPrivada(const char *tipo, const struct Array_t *arr,unsigned long pos)
{
	i_integridad(arr, tipo);

	assert(pos < arr->numElementos);

	return arr->array[pos];
}

//-----------------------------------------------------------------------

void *array_UltimoPrivada(const char *tipo, const struct Array_t *arr)
{
	i_integridad(arr, tipo);
	assert(arr->numElementos > 0);
	
	return arr->array[arr->numElementos - 1];
}


//-----------------------------------------------------------------------

void array_PonerPrivada(const char *tipo, struct Array_t *arr,unsigned long pos,void *ptr)
{
	i_integridad(arr, tipo);

	assert_no_null(ptr);
	assert(pos < arr->numElementos);

	assert(arr->array[pos] == NULL);
	arr->array[pos] = ptr;
}

//-----------------------------------------------------------------------

static void **i_recoloca(void ***array, unsigned long numElementos)
{
	if (array != NULL)
    {
		REALLOC(array, numElementos, void *);
        return *array;
    }
	else
		return CALLOC(numElementos, void *);
}

//-----------------------------------------------------------------------

void array_AnyadirPrivada(const char *tipo, struct Array_t *arr,void *ptr)
{
	i_integridad(arr, tipo);

	assert_no_null(ptr);

	arr->array = i_recoloca(&arr->array, arr->numElementos + 1);
	arr->array[arr->numElementos] = ptr;
	arr->numElementos++;
}

//-----------------------------------------------------------------------

void array_AumentarPrivada(const char *tipo, struct Array_t *arr, unsigned long numNuevosElementos)
{
	unsigned long i;
	
	i_integridad(arr, tipo);

	arr->array = i_recoloca(&arr->array, arr->numElementos + numNuevosElementos);
	
	for (i = 0; i < numNuevosElementos; i++)
	{
		unsigned long ind;
		
		ind = arr->numElementos + i;
		arr->array[ind] = NULL; 
	}
	
	arr->numElementos += numNuevosElementos;
}

//-----------------------------------------------------------------------

void array_InsertarPrivada(const char *tipo, struct Array_t *arr, 
						   unsigned long pos, void *ptr)
{
	i_integridad(arr, tipo);

	assert_no_null(ptr);

	arr->array = i_recoloca(&arr->array, arr->numElementos + 1);

	if (arr->numElementos > 0)
	{
		unsigned long i;

		assert(pos < arr->numElementos);

		for (i = arr->numElementos; i > pos; i--)
			arr->array[i] = arr->array[i - 1];
	}
	
	arr->array[pos] = ptr;

	arr->numElementos++;
}

//-----------------------------------------------------------------------

void array_BorrarPrivada(const char *tipo, struct Array_t *arr,unsigned long pos,
						 FuncLiberaPtr func)
{
	void **ptr;

	i_integridad(arr, tipo);

	assert(pos < arr->numElementos);

	if (func != NULL)
		func(&arr->array[pos]);

	if (arr->numElementos > 1)
	{
		unsigned long i;

		ptr = CALLOC(arr->numElementos-1,void *);
		for (i = 0; i < pos; i++)
			ptr[i] = arr->array[i];

		for (i = pos; i < arr->numElementos-1; i++)
			ptr[i] = arr->array[i+1];
	}
	else
		ptr = NULL;

	FREE_T(&(arr->array), void *);
	arr->array = ptr;

	arr->numElementos--;
}

//-----------------------------------------------------------------------

static bool i_lanza_busca(FuncBuscarPtr func, const void *elem1, const void *elem2)
{
	if (func != NULL)
		return func(elem1,elem2);
	else
	{
		if (elem1 != elem2)
			return false;
		else
			return true;
	}
}

//-----------------------------------------------------------------------

bool array_BuscarPrivada(const char *tipo,
						   const struct Array_t *arr,const void *elem,
						   FuncBuscarPtr func, unsigned long *pos)
{
	unsigned long i;

	i_integridad(arr, tipo);

	for (i = 0; i < arr->numElementos; i++)
	{
		if (i_lanza_busca(func, arr->array[i],elem) == true)
			break;
	}

	if (i == arr->numElementos)
		return false;
	else
	{
		if (pos != NULL)
			*pos = i;

		return true;
	}
}

//-----------------------------------------------------------------------

bool array_BuscarUnicaPrivada(const char *tipo, struct Array_t *arr,const void *elem, FuncBuscarPtr func, unsigned long *pos)
{
	unsigned long i, ind_encontrado;
	
	i_integridad(arr, tipo);
	
	ind_encontrado = ULONG_MAX;

	for (i = 0; i < arr->numElementos; i++)
	{
		if (i_lanza_busca(func, arr->array[i],elem) == true)
		{
			assert(ind_encontrado == ULONG_MAX);
			ind_encontrado = i;
		}
	}

	if (ind_encontrado == ULONG_MAX)
		return false;
	else
	{
		if (pos != NULL)
			*pos = ind_encontrado;
            
		return true;
	}
}

//-----------------------------------------------------------------------

bool array_cumplePrivada(const char *tipo, struct Array_t *arr, FuncCumplePtr func, unsigned long *ind)
{
    unsigned long i;
    
    i_integridad(arr, tipo);
    assert_no_null(func);
    
    for (i = 0; i < arr->numElementos; i++)
    {
        if (func(arr->array[i]) == true)
        {
            if (ind != NULL)
                *ind = i;
                
            return true;
        }
    }

    return false;
} 


//-----------------------------------------------------------------------

unsigned long array_numElementosPrivada(const char *tipo, const struct Array_t *arr)
{
	i_integridad(arr, tipo);
	return arr->numElementos;
}

//-----------------------------------------------------------------------

void array_ConcatenarPrivada(const char *tipo, struct Array_t *arr, const struct Array_t *array_a_anyadir, FuncCopiaPtr func)
{
	unsigned long i;
	
	i_integridad(arr, tipo);
	i_integridad(array_a_anyadir, tipo);

	if (array_a_anyadir->numElementos > 0)
	{
		arr->array = i_recoloca(&arr->array, arr->numElementos + array_a_anyadir->numElementos);
		
		for (i = 0; i < array_a_anyadir->numElementos; i++)
			arr->array[i + arr->numElementos] = i_copia_objeto(func, array_a_anyadir->array[i]);
		
		arr->numElementos += array_a_anyadir->numElementos;
	}
}
