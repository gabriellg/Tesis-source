// Define una tarea.

#include "task.hxx"

#ifdef __cplusplus
extern "C" {
#endif

struct Task_t *task_createTaskPrivate(
					const char *id,
					const char *typeResult,
					struct task_item_data_t *data,
					task_fptr_executeTask func_executeTask,
					task_fptr_destroyDataResult func_destroyDataResult);

#define task_createTask(typeData, typeResult, id, data, func_executeTask, func_destroyDataResult)\
(\
	(void)((struct typeData *)data == data),\
	TASK_VERIFY_EXECUTE_TASK(typeData, typeResult, func_executeTask),\
	TASK_VERIFY_DESTROY_DATA_RESULT(typeResult, func_destroyDataResult),\
	task_createTaskPrivate(\
					 id, #typeResult, (struct task_item_data_t *)data,\
					 (task_fptr_executeTask)func_executeTask,\
					 (task_fptr_destroyDataResult)func_destroyDataResult)\
)

#define task_createTaskClass(typeData, typeResult, id, data, func_executeTask, func_destroyDataResult)\
(\
	(void)((class typeData *)data == data),\
	TASK_VERIFY_DESTROY_DATA_RESULT_CLASS(typeResult, func_destroyDataResult),\
	TASK_VERIFY_EXECUTE_TASK_CLASS(typeData, typeResult, func_executeTask),\
	task_createTaskPrivate(\
					 id, #typeResult, (struct task_item_data_t *)data,\
					 (task_fptr_executeTask)func_executeTask,\
					 (task_fptr_destroyDataResult)func_destroyDataResult)\
)

#ifdef __cplusplus
}
#endif
