// Define el planificador de tareas.

#include "task.hxx"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct SchedulerTask_t *schedulerTask_create(void);

void schedulerTask_destroy(struct SchedulerTask_t **schedulerTask);

void schedulerTask_openTasks(struct SchedulerTask_t *schedulerTask);

void schedulerTask_appendTasks(struct SchedulerTask_t *schedulerTask, struct Task_t *task);

void schedulerTask_dependTasks(struct SchedulerTask_t *schedulerTask, struct Task_t *taskIn, struct Task_t *taskOut);

void schedulerTask_dependIdTasks(struct SchedulerTask_t *schedulerTask, const char *idTaskIn, const char *idTaskOut);

void schedulerTask_closeTasks(struct SchedulerTask_t *schedulerTask);

void schedulerTask_prepareTasks(struct SchedulerTask_t *schedulerTask);

void schedulerTask_executeTask(struct SchedulerTask_t *schedulerTask, struct Table_t *table);

void schedulerTask_executeNextLevelTasks(struct SchedulerTask_t *schedulerTask, struct Table_t *table);

void schedulerTask_drawPrivate(
					const struct SchedulerTask_t *schedulerTask,
					const char *nameLayerNodes,
					const char *nameLayerArrows,
					task_fptr_drawNode func_drawNode,
					class IGraphics *graphics);

#define scheduleTask_draw(\
					typeDataNode, typeResultNode,\
					schedulerTask,\
					nameLayerNodes, nameLayerArrows,\
					func_drawNode, graphics)\
(\
	TASK_VERIFY_DRAW(typeDataNode, typeResultNode, func_drawNode),\
	schedulerTask_drawPrivate(schedulerTask, nameLayerNodes, nameLayerArrows, (task_fptr_drawNode)func_drawNode, graphics)\
)

#define scheduleTask_drawClass(\
					typeDataNode, typeResultNode,\
					schedulerTask,\
					nameLayerNodes, nameLayerArrows,\
					func_drawNode, graphics)\
(\
	TASK_VERIFY_DRAW_CLASS(typeDataNode, typeResultNode, func_drawNode),\
	schedulerTask_drawPrivate(schedulerTask, nameLayerNodes, nameLayerArrows, (task_fptr_drawNode)func_drawNode, graphics)\
)

#ifdef __cplusplus
}
#endif /* __cplusplus */
