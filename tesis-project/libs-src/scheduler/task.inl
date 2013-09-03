// Define una tarea.

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void task_destroy(struct Task_t **task);

bool task_id_iguales(const struct Task_t *task1, const struct Task_t *task2);

bool task_isEqualId(const struct Task_t *task, const char *id);

void task_resetResult(struct Task_t *taks);

void task_execute(struct Task_t *task, ArrTipo(Task_t) *fathers, struct Table_t *table);

void task_drawNode(const struct Task_t *task, task_fptr_drawNode func_drawNode, class IGraphics *graphics);

#ifdef __cplusplus
}
#endif /* __cplusplus */
