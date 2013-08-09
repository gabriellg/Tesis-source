// Teteamos aplicacion.

#include "asrtbas.h"
#include "memory.h"
#include "array.h"

#include "schedulerTask.hpp"
#include "task.hpp"
#include "table.h"

struct i_tarea_t
{
	int id;
};

//-----------------------------------------------------------------------

static struct i_tarea_t *i_creaTask(int id)
{
	struct i_tarea_t *tarea;
	
	tarea = MALLOC(struct i_tarea_t);
	
	tarea->id = id;
	
	return tarea;
}

//-----------------------------------------------------------------------

static void i_destruyeTarea(struct i_tarea_t **tarea)
{
	FREE_T(tarea, struct i_tarea_t);
}

//-----------------------------------------------------------------------

static struct i_nulo_t *i_ejecutaTarea(struct i_tarea_t *tarea, const ArrTipo(i_nulo_t) *padres, struct Table_t *table)
{
	ArrTipo(i_tarea_t) *tareas;

	tareas = table_get(table, "tareas", ArrTipo(i_tarea_t));
	array_Anyadir(i_tarea_t, tareas, tarea);
	
	return NULL;
}

//-----------------------------------------------------------------------
	
static void i_appendTasks(struct SchedulerTask_t *schedulerTask)
{
	struct i_tarea_t *tarea1, *tarea2, *tarea3, *tarea4, *tarea5, *tarea6;
	struct Task_t *t1, *t2, *t3, *t4, *t5, *t6;

	tarea1 = i_creaTask(1);
	t1 = task_createTask(i_tarea_t, i_nulo_t, "1", tarea1, i_ejecutaTarea, NULL);
	schedulerTask_appendTasks(schedulerTask, t1);

	tarea2 = i_creaTask(2); 
	t2 = task_createTask(i_tarea_t, i_nulo_t, "2", tarea2, i_ejecutaTarea, NULL);
	schedulerTask_appendTasks(schedulerTask, t2);

	tarea3 = i_creaTask(3); 
	t3 = task_createTask(i_tarea_t, i_nulo_t, "3", tarea3, i_ejecutaTarea, NULL);
	schedulerTask_appendTasks(schedulerTask, t3);

	tarea4 = i_creaTask(4); 
	t4 = task_createTask(i_tarea_t, i_nulo_t, "4", tarea4, i_ejecutaTarea, NULL);
	schedulerTask_appendTasks(schedulerTask, t4);

	tarea5 = i_creaTask(5); 
	t5 = task_createTask(i_tarea_t, i_nulo_t, "5", tarea5, i_ejecutaTarea, NULL);
	schedulerTask_appendTasks(schedulerTask, t5);

	tarea6 = i_creaTask(6); 
	t6 = task_createTask(i_tarea_t, i_nulo_t, "6", tarea6, i_ejecutaTarea, NULL);
	schedulerTask_appendTasks(schedulerTask, t6);

	schedulerTask_dependTasks(schedulerTask, t1, t2);
	schedulerTask_dependTasks(schedulerTask, t1, t3);
	schedulerTask_dependTasks(schedulerTask, t2, t4);
	schedulerTask_dependTasks(schedulerTask, t3, t5);
	schedulerTask_dependTasks(schedulerTask, t4, t6);
	schedulerTask_dependTasks(schedulerTask, t5, t6);
}

//-----------------------------------------------------------------------

static void i_esCorrecto(struct Table_t *table)
{
	unsigned long i, num;
	struct i_tarea_t *tarea_anterior;
	ArrTipo(i_tarea_t) *tareas;

	tareas = table_get(table, "tareas", ArrTipo(i_tarea_t));

	tarea_anterior = NULL;	
	num = array_numElementos(i_tarea_t, tareas);

	for (i = 0; i < num; i++) 
	{
		struct i_tarea_t *tarea;
		
		tarea = array_Get(i_tarea_t, tareas, i);
		assert_no_null(tarea);
		
		if (tarea_anterior != NULL)
		{
			int dif;
			
			dif = tarea->id - tarea_anterior->id;
			assert(dif == 1);
		}
	}
}

//-----------------------------------------------------------------------

static void i_destroyTareas(ArrTipo(i_tarea_t) **tareas)
{
	array_Destruye(i_tarea_t, tareas, i_destruyeTarea);
}

//-----------------------------------------------------------------------

static Table_t *i_createTable(void)
{
	struct Table_t *table;
	ArrTipo(i_tarea_t) *tareas;

	tareas = array_CreaPuntero(0, i_tarea_t);
	table = table_create();
	
	table_set(table, "tareas", &tareas, i_destroyTareas, ArrTipo(i_tarea_t));
	
	return table;
}

//-----------------------------------------------------------------------

int main(int argc, char *argv[])
{
	struct SchedulerTask_t *schedulerTask;
	struct Table_t *table;

	table = i_createTable();
	
	schedulerTask = schedulerTask_create();

	schedulerTask_openTasks(schedulerTask);
	
	i_appendTasks(schedulerTask);
	
	schedulerTask_closeTasks(schedulerTask);
	
	schedulerTask_prepareTasks(schedulerTask);
	schedulerTask_executeTask(schedulerTask, table);
	
	i_esCorrecto(table);

	table_destroy(&table);
	schedulerTask_destroy(&schedulerTask);
}
