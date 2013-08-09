// Define una tarea.

#include "task.hpp"
#include "task.inl"

#include "asrtbas.h"
#include "memory.h"
#include "strbas.h"
#include "array.h"

#include "stdio.h"

struct Task_t
{
	char *id;
	char *typeResult;

	struct task_item_data_t *data;
	struct task_item_result_t *result; 
	
	task_fptr_executeTask func_executeTask;
	task_fptr_destroyDataResult func_destroyDataResult;
};

//---------------------------------------------------------
	
static struct Task_t *i_create(
					const char *id,
					const char *typeResult,
					struct task_item_data_t *data,
					struct task_item_result_t **result,
					task_fptr_executeTask func_executeTask,
					task_fptr_destroyDataResult func_destroyDataResult)
{
	struct Task_t *task;
	
	task = MALLOC(struct Task_t);
	
	task->id = strbas_copyString(id);
	task->typeResult = strbas_copyString(typeResult);
	
	task->data = data;
	task->result = ASSIGN_PP(result, struct task_item_result_t);
	
	task->func_executeTask = func_executeTask;
	task->func_destroyDataResult = func_destroyDataResult;
	
	return task;
}

//---------------------------------------------------------
	
struct Task_t *task_createTaskPrivate(
					const char *id,
					const char *typeResult,
					struct task_item_data_t *data,
					task_fptr_executeTask func_executeTask,
					task_fptr_destroyDataResult func_destroyDataResult)
{
	struct task_item_result_t *result;
	
	result = NULL;
	
	return i_create(
					id, typeResult, data, &result,
					func_executeTask, func_destroyDataResult);
}

//---------------------------------------------------------
	
void task_destroy(struct Task_t **task)
{
	assert_no_null(task);
	assert_no_null(*task);
	
	if ((*task)->result != NULL)
	{
		assert_no_null((*task)->func_destroyDataResult);
		(*task)->func_destroyDataResult(&(*task)->result);
	}
	
	strbas_destroy(&(*task)->id);
	strbas_destroy(&(*task)->typeResult);
	
	FREE_T(task, struct Task_t);
}

//---------------------------------------------------------

bool task_id_iguales(const struct Task_t *task1, const struct Task_t *task2)
{
	assert_no_null(task1);
	assert_no_null(task2);
	
	return strbas_equalsStrings(task1->id, task2->id);
}

//---------------------------------------------------------

bool task_isEqualId(const struct Task_t *task, const char *id)
{
	assert_no_null(task);
	return strbas_equalsStrings(task->id, id);
}

//---------------------------------------------------------

void task_resetResult(struct Task_t *task)
{
	assert_no_null(task);
	
	if (task->result != NULL)
	{
		assert_no_null(task->func_destroyDataResult);
		task->func_destroyDataResult(&task->result);
	}
}

//---------------------------------------------------------

static ArrTipo(task_item_result_t) *i_createResult(
						const ArrTipo(Task_t) *fathers, 
						const char *typeResult)
{
	struct Array_t *result_fathers;
	unsigned long i, numFathers;
	
	numFathers = array_numElementos(Task_t, fathers);
	
	result_fathers = array_CreaPrivada(typeResult, numFathers);
	
	for (i = 0; i < numFathers; ++i) 
	{
		struct Task_t *father;
		
		father = array_Get(Task_t, fathers, i);
		assert(strbas_equalsStrings(father->typeResult, typeResult) == true);
		array_PonerPrivada(typeResult, result_fathers, i, father->result);	
	}
	
	return (ArrTipo(task_item_result_t) *)result_fathers;
}

//---------------------------------------------------------

void task_execute(struct Task_t *task, ArrTipo(Task_t) *fathers, struct Table_t *table)
{
	ArrTipo(task_item_result_t) *result_fathers; 
	
	assert_no_null(task);
	
	result_fathers = i_createResult(fathers, task->typeResult); 
	
	task->result = task->func_executeTask(task->data, result_fathers, table);
	
	array_DestruyePrivada(task->typeResult, (struct Array_t **)&result_fathers, NULL);
}

//---------------------------------------------------------

void task_drawNode(
				const struct Task_t *task,
				task_fptr_drawNode func_drawNode, 
				class IGraphics *graphics)
{
	assert_no_null(task);
	assert_no_null(func_drawNode);
	
	func_drawNode(task->data, task->result, graphics);
}
