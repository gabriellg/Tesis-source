// Agrupaciones de arrays.

#include "arrgroup.h"

#include "asrtbas.h"
#include "array.h"
#include "memory.h"
#include "strbas.h"

struct i_group_t
{
	struct Array_t *arrayElements;
};

struct ArrGroup_t
{
	char *tipo;
	ArrTipo(i_group_t) *arrayGroup;
};

//---------------------------------------------------------

static struct i_group_t *i_createGroup(struct Array_t **arrayElements)
{
	struct i_group_t *group;
	
	group = MALLOC(struct i_group_t);
	
	group->arrayElements = ASSIGN_PP_NO_NULL(arrayElements, struct Array_t);
	
	return group;
}

//---------------------------------------------------------

static void i_destroyGroup(struct i_group_t **group)
{
	assert_no_null(group);
	assert_no_null(*group);
	
	assert((*group)->arrayElements == NULL);
	
	FREE_T(group, i_group_t);
}

//---------------------------------------------------------

static void i_destroyElementsGroup(const char *tipo, struct i_group_t *group, arrgroup_FuncLiberaPtr func_destroyElement)
{
	assert_no_null(group);
	array_DestruyePrivada(tipo, &(group->arrayElements), func_destroyElement);
}

//---------------------------------------------------------

static struct ArrGroup_t *i_create(const char *tipo, ArrTipo(i_group_t) **arrayGroup)
{
	struct ArrGroup_t *arrGroup;
	
	arrGroup = MALLOC(struct ArrGroup_t);
	
	arrGroup->tipo = strbas_copyString(tipo);
	arrGroup->arrayGroup = ASSIGN_PP_NO_NULL(arrayGroup, ArrTipo(i_group_t));
	
	return arrGroup;
}
		
//---------------------------------------------------------

struct ArrGroup_t *arrgroup_createEmptyPrivate(const char *tipo)
{
	ArrTipo(i_group_t) *arrGroup;
	
	arrGroup = array_CreaPuntero(0, i_group_t);
	
	return i_create(tipo, &arrGroup);
}

//---------------------------------------------------------

static void i_destroyArrayElementsGroup(
							const char *tipo, 
							ArrTipo(i_group_t) *arrayGroup, 
							arrgroup_FuncLiberaPtr func_destroyElement)
{
	unsigned long i, num;
		
	num = array_numElementos(i_group_t, arrayGroup);
		
	for (i = 0; i < num; i++)
	{
		struct i_group_t *group;
			
		group = array_Get(i_group_t, arrayGroup, i);
		i_destroyElementsGroup(tipo, group, func_destroyElement);
	}
}

//---------------------------------------------------------

void arrgroup_destroyPrivate(const char *tipo, struct ArrGroup_t **arrGroup, arrgroup_FuncLiberaPtr func_destroyElement)
{
	assert_no_null(arrGroup);
	assert_no_null(*arrGroup);
	assert(strbas_equalsStrings(tipo, (*arrGroup)->tipo) == true);
			
	i_destroyArrayElementsGroup((*arrGroup)->tipo, (*arrGroup)->arrayGroup, func_destroyElement);
	array_Destruye(i_group_t, &(*arrGroup)->arrayGroup, i_destroyGroup);
	
	FREE_T(arrGroup, struct ArrGroup_t);
}

//---------------------------------------------------------

unsigned long arrgroup_numGroupsPrivate(const char *tipo, const struct ArrGroup_t *arrGroup)
{
	assert_no_null(arrGroup);
	assert(strbas_equalsStrings(tipo, arrGroup->tipo) == true);
	
	return array_numElementos(i_group_t, arrGroup->arrayGroup);
}

//---------------------------------------------------------

void arrgroup_appendGroupPrivate(const char *tipo, struct ArrGroup_t *arrGroup, struct Array_t **arrayElements)
{
	struct i_group_t *group;
	
	assert_no_null(arrGroup);
	assert(strbas_equalsStrings(tipo, arrGroup->tipo) == true);
	
	group = i_createGroup(arrayElements); 
	array_Anyadir(i_group_t, arrGroup->arrayGroup, group);
}

//---------------------------------------------------------

struct Array_t *arrgroup_getPrivate(const char *tipo, const struct ArrGroup_t *arrGroup, unsigned long ind) 
{
	struct i_group_t *group;
	
	assert_no_null(arrGroup);
	assert(strbas_equalsStrings(tipo, arrGroup->tipo) == true);
	
	group = array_Get(i_group_t, arrGroup->arrayGroup, ind);
	
	return group->arrayElements;
}

//---------------------------------------------------------

bool arrgroup_seachGroupAndElementPrivate(
							const char *tipo, 
							const struct ArrGroup_t *arrGroup,
							const void *elem, 
							unsigned long *indGroup, unsigned long *indElement, 
							arrgroup_FuncSearchPtr funcSearch)
{
	unsigned long i, numGroup;
	
	assert_no_null(arrGroup);
	assert(strbas_equalsStrings(tipo, arrGroup->tipo) == true);
	
	numGroup = array_numElementos(i_group_t, arrGroup->arrayGroup);
	
	for (i = 0; i < numGroup; i++)
	{
		const struct i_group_t *group;
		unsigned long ind_pos_loc;
		
		group = array_Get(i_group_t, arrGroup->arrayGroup, i);
		
		if (array_BuscarPrivada(tipo, group->arrayElements, elem, funcSearch, &ind_pos_loc) == true)
		{
			if (indGroup != NULL)
				*indGroup = i;
			
			if (indElement != NULL)
				*indElement = ind_pos_loc;
			
			return true;
		}
	}
	
	return false;
}
