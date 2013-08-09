#ifndef TASK_HXX
#define TASK_HXX

typedef struct task_item_result_t *(*task_fptr_executeTask)(struct task_item_data_t *data, const ArrTipo(task_item_result_t) *resultFathers, struct Table_t *table);
#define TASK_VERIFY_EXECUTE_TASK(typeData, typeResult, function)\
(\
	(void)((struct typeResult *(*)(\
					struct typeData *,\
					const ArrTipo(typeResult) *,\
					struct Table_t *))function == function)\
)

#define TASK_VERIFY_EXECUTE_TASK_CLASS(typeData, typeResult, function)\
(\
	(void)((class typeResult * (*)(\
					class typeData *,\
					const ArrClass(typeResult) *,\
					struct Table_t *))function == function)\
)

typedef void (*task_fptr_destroyDataResult)(struct task_item_result_t **);
#define TASK_VERIFY_DESTROY_DATA_RESULT(typeResult, function) (void)((void (*)(struct typeResult **))function == function) 
#define TASK_VERIFY_DESTROY_DATA_RESULT_CLASS(typeResult, function) (void)((void (*)(class typeResult **))function == function) 

typedef void (*task_fptr_drawNode)(const struct task_item_data_t *data, const struct task_item_result_t *dataResult, class IGraphics *graphics);
#define TASK_VERIFY_DRAW(typeData, typeResult, function)\
(\
	(void)((void (*)(\
					const struct typeData *,\
					const struct typeResult *,\
					class IGraphics *))function == function)\
)

#define TASK_VERIFY_DRAW_CLASS(typeData, typeResult, function)\
(\
	(void)((void (*)(\
					const class typeData *,\
					const class typeResult *,\
					class IGraphics *))function == function)\
)

#endif
