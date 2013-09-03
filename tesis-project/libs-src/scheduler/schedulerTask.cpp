// Define el planificador de tareas.

#include "schedulerTask.hpp"

#include "task.inl"

#include "asrtbas.h"
#include "memory.h"
#include "graph.h"
#include "array.h"
#include "arrgroup.h"

#include "sys/param.h"

#include "CGraphicsRect.hpp"
#include "CGeometry.hpp"

struct SchedulerTask_t
{
	Graph(Task_t) *graphTask;
	ArrGroup(Task_t) *levelsTasks;

	unsigned long indLevelRunningCurrent;	
	bool canAppendTask;
};

static const double i_HEIGTH_NODE = 1.5;
static const double i_WIDTH_NODE = 1.5;

//---------------------------------------------------------

static struct SchedulerTask_t *i_create(
						Graph(Task_t) **graphTask, 
						ArrGroup(Task_t) **levelsTasks,
						unsigned long indLevelRunningCurrent,
						bool canAppendTask)
{
	struct SchedulerTask_t *schedulerTask;	
	
	schedulerTask = MALLOC(struct SchedulerTask_t);
	
	schedulerTask->graphTask = ASSIGN_PP_NO_NULL(graphTask, Graph(Task_t));
	schedulerTask->levelsTasks = ASSIGN_PP(levelsTasks, ArrGroup(Task_t));
	
	schedulerTask->canAppendTask = canAppendTask;
	schedulerTask->indLevelRunningCurrent = indLevelRunningCurrent;
	
	return schedulerTask;
}

//---------------------------------------------------------

struct SchedulerTask_t *schedulerTask_create(void)
{
	Graph(Task_t) *graphTask;
	ArrGroup(Task_t) *levelsTasks;
	unsigned long indLevelRunningCurrent;
	bool canAppendTask;
	
	graphTask = graph_create(Task_t, task_id_iguales, task_destroy);
	levelsTasks = NULL;
	canAppendTask = false;
	
	indLevelRunningCurrent = 0;
	return i_create(&graphTask, &levelsTasks, indLevelRunningCurrent, canAppendTask);
}

//---------------------------------------------------------

void schedulerTask_destroy(struct SchedulerTask_t **schedulerTask)
{
	assert_no_null(schedulerTask);
	assert_no_null(*schedulerTask);	
	
	graph_destroy(Task_t, &(*schedulerTask)->graphTask);
	
	if ((*schedulerTask)->levelsTasks != NULL)
		arrgroup_destroy(Task_t, &(*schedulerTask)->levelsTasks, NULL);
	
	FREE_T(schedulerTask, struct SchedulerTask_t);
}

//---------------------------------------------------------

void schedulerTask_openTasks(struct SchedulerTask_t *schedulerTask)
{
	assert_no_null(schedulerTask);
	assert(schedulerTask->canAppendTask == false);
	schedulerTask->canAppendTask = true;
}

//---------------------------------------------------------

void schedulerTask_appendTasks(struct SchedulerTask_t *schedulerTask, struct Task_t *task)
{
	assert_no_null(schedulerTask);
	assert(schedulerTask->canAppendTask == true);
	
	graph_appendVertex(Task_t, schedulerTask->graphTask, task);
}

//---------------------------------------------------------

void schedulerTask_dependTasks(struct SchedulerTask_t *schedulerTask, struct Task_t *taskIn, struct Task_t *taskOut)
{
	assert_no_null(schedulerTask);
	assert(schedulerTask->canAppendTask == true);
	
	graph_appendEdge(Task_t, schedulerTask->graphTask, taskIn, taskOut);
}

//---------------------------------------------------------

void schedulerTask_dependIdTasks(struct SchedulerTask_t *schedulerTask, const char *idTaskIn, const char *idTaskOut)
{
	struct Task_t *taskIn, *taskOut;
	bool exist;
	
	assert_no_null(schedulerTask);
	assert(schedulerTask->canAppendTask == true);
	
	exist = graph_findVertex(
						Task_t, schedulerTask->graphTask, 
						char, idTaskIn, 
						task_isEqualId, &taskIn);
	assert(exist == true);

	exist = graph_findVertex(
						Task_t, schedulerTask->graphTask, 
						char, idTaskOut, 
						task_isEqualId, &taskOut);
	assert(exist == true);
	
	graph_appendEdge(Task_t, schedulerTask->graphTask, taskIn, taskOut);
}

//---------------------------------------------------------

void schedulerTask_closeTasks(struct SchedulerTask_t *schedulerTask)
{
	assert_no_null(schedulerTask);
	assert(schedulerTask->canAppendTask == true);
	
	schedulerTask->canAppendTask = false;
}

//---------------------------------------------------------

void schedulerTask_prepareTasks(struct SchedulerTask_t *schedulerTask)
{
	assert_no_null(schedulerTask);
	assert(schedulerTask->canAppendTask == false);
	assert(schedulerTask->levelsTasks == NULL);
	
	schedulerTask->levelsTasks = graph_levelsGroups(Task_t, schedulerTask->graphTask);
}

//---------------------------------------------------------

static void i_executeTasks(const ArrTipo(Task_t) *tasks, Graph(Task_t) *graphTask, struct Table_t *table)
{
	unsigned long i, num;
	
	num = array_numElementos(Task_t, tasks);
	
	for (i = 0; i < num; i++) 
	{
		struct Task_t *task;
		ArrTipo(Task_t) *fathers;
		
		task = array_Get(Task_t, tasks, i);
		
		fathers = graph_fathers(Task_t, graphTask, task);
		task_execute(task, fathers, table);
		array_Destruye(Task_t, &fathers, NULL);
	}
}

//---------------------------------------------------------

static void i_runLevel(
					Graph(Task_t) *graphTask,
					ArrGroup(Task_t) *levelsTasks,
					struct Table_t *table, 
					unsigned long indLevel)
{
	ArrTipo(Task_t) *tasksLevel;
	
	tasksLevel = arrgroup_Get(Task_t, levelsTasks, indLevel);
	i_executeTasks(tasksLevel, graphTask, table);
}

//---------------------------------------------------------

static void i_resetResultsTasks(ArrTipo(Task_t) *tasks)
{
	unsigned long i, num;
	
	num = array_numElementos(Task_t, tasks);
	
	for (i = 0; i < num; i++)
	{ 
		struct Task_t *task;
		
		task = array_Get(Task_t, tasks, i);
		assert_no_null(task);
		task_resetResult(task);
	}
}

//---------------------------------------------------------

static void i_resetResultsLevels(ArrGroup(Task_t) *levelsTasks)
{
	unsigned long i, num;
	
	num = arrgroup_numGroups(Task_t, levelsTasks);
	
	for (i = 0; i < num; i++)
	{ 
		ArrTipo(Task_t) *tasksLevel;
		
		tasksLevel = arrgroup_Get(Task_t, levelsTasks, i);
		i_resetResultsTasks(tasksLevel);
	}
}

//---------------------------------------------------------

void schedulerTask_executeTask(struct SchedulerTask_t *schedulerTask, struct Table_t *table)
{
	unsigned long i, num;
	
	assert_no_null(schedulerTask);
	
	i_resetResultsLevels(schedulerTask->levelsTasks);
	
	num = arrgroup_numGroups(Task_t, schedulerTask->levelsTasks);
	
	for (i = 0; i < num; i++) 
		i_runLevel(schedulerTask->graphTask, schedulerTask->levelsTasks, table, i);
		
	schedulerTask->indLevelRunningCurrent = 0;
}

//---------------------------------------------------------

void schedulerTask_executeNextLevelTasks(struct SchedulerTask_t *schedulerTask, struct Table_t *table)
{
	unsigned long numLevels; 
	
	assert_no_null(schedulerTask);
	
	if (schedulerTask->indLevelRunningCurrent == 0)
		i_resetResultsLevels(schedulerTask->levelsTasks);

	numLevels = arrgroup_numGroups(Task_t, schedulerTask->levelsTasks); 
	
	i_runLevel(
			schedulerTask->graphTask,
			schedulerTask->levelsTasks,
			table,
			schedulerTask->indLevelRunningCurrent);
			
	schedulerTask->indLevelRunningCurrent = (schedulerTask->indLevelRunningCurrent + 1) % numLevels; 
}

//---------------------------------------------------------

static double i_calculateHeigthMax(ArrGroup(Task_t) *levelsTasks)
{
	unsigned long i, num, numMaxiLevel;
	
	numMaxiLevel = 0;
	num = arrgroup_numGroups(Task_t, levelsTasks);
	
	for (i = 0; i < num; i++) 
	{
		const ArrTipo(Task_t) *level;
		unsigned long numNodeLevel;
		
		level = arrgroup_Get(Task_t, levelsTasks, i);
		
		numNodeLevel = array_numElementos(Task_t, level);
		numMaxiLevel = MAX(numNodeLevel, numMaxiLevel);  
	}
	
	return numMaxiLevel * i_HEIGTH_NODE;
}

//---------------------------------------------------------

static void i_posNode(
					double heigthMax, 
					unsigned long indLevel, unsigned long indNode,
					unsigned long numNodesLevel,
					double *x_posNodeCenter, double *y_posNodeCenter,
					double *x_posOriginNode, double *y_posOriginNode,
					double *x_SizeCell, double *y_SizeCell)
{
	double dy;
	
	assert_no_null(x_posNodeCenter);
	assert_no_null(y_posNodeCenter);
	assert(numNodesLevel > 0);
	
	*x_posNodeCenter = i_WIDTH_NODE * indLevel + i_WIDTH_NODE / 2.;
	
	dy = heigthMax / numNodesLevel; 
	*y_posNodeCenter = dy * indNode + dy / 2.;
	
	if (x_SizeCell != NULL)
	{
		assert_no_null(x_posOriginNode);
		assert_no_null(y_posOriginNode);
		assert_no_null(y_SizeCell);
		
		*x_SizeCell = i_WIDTH_NODE;  
		*y_SizeCell = dy;
		
		*x_posOriginNode = i_WIDTH_NODE * indLevel; 
		*y_posOriginNode = dy * indNode;
	}
	else
	{
		assert(x_posOriginNode == NULL);
		assert(y_posOriginNode == NULL);
		assert(y_SizeCell == NULL);
	}
}

//---------------------------------------------------------

static bool i_calculateDimensionDrawNode(
							const struct Task_t *task,
							task_fptr_drawNode func_drawNode,
							double *xmin, double *ymin, double *zmin,
							double *xmax, double *ymax, double *zmax)
{
	class CGraphicsRect *graphicsLimits;
	bool hasLimits;
	double coefPorcBorder;
	double xmin_loc, ymin_loc, zmin_loc, xmax_loc, ymax_loc, zmax_loc;
	
	graphicsLimits = new CGraphicsRect();
	
	task_drawNode(task, func_drawNode, graphicsLimits);

	coefPorcBorder = 0.05;
	hasLimits = graphicsLimits->getLimits(
					&xmin_loc, &ymin_loc, &zmin_loc, 
					&xmax_loc, &ymax_loc, &zmax_loc);

	CGeometry::rectangleWithProporcional(
					i_WIDTH_NODE,
					i_HEIGTH_NODE,
					coefPorcBorder, 
					xmin_loc, ymin_loc, zmin_loc,
					xmax_loc, ymax_loc, zmax_loc,
					xmin, ymin, zmin,
					xmax, ymax, zmax);
					
	delete graphicsLimits;
					
	return hasLimits;
}

//---------------------------------------------------------

static void i_calculateDataTransformation(
							double xmin, double ymin,
							double xmax, double ymax,
							double *x_center, double *y_center,
							double *dimXDraw, double *dimYDraw)
{
	assert_no_null(x_center);
	assert_no_null(y_center);
	
	assert_no_null(dimXDraw);
	assert_no_null(dimYDraw);
	
	*x_center = (xmin + xmax) / 2.;
	*y_center = (ymin + ymax) / 2.;
	
	*dimXDraw = 1.2 * (xmax - xmin); 
	*dimYDraw = 1.2 * (ymax - ymin);
}

//---------------------------------------------------------

static void i_drawRowNodes(
					const ArrTipo(Task_t) *tasks,
					unsigned long indLevel,
					double heigthMax,
					task_fptr_drawNode func_drawNode,
					class IGraphics *graphics)
{
	unsigned long i, num;
	
	assert_no_null(graphics);

	num = array_numElementos(Task_t, tasks);
	
	for (i = 0; i < num; i++) 
	{
		double xmin, ymin, zmin, xmax, ymax, zmax;
		const struct Task_t *task;
			
		task = array_Get(Task_t, tasks, i);		
		
		if (i_calculateDimensionDrawNode(
							task, func_drawNode,
							&xmin, &ymin, &zmin,
							&xmax, &ymax, &zmax) == true)
		{
			double x_posOriginNode, y_posOriginNode;
			double x_posNodeCenter, y_posNodeCenter, x_SizeCell, y_SizeCell;
			double x_centerDraw, y_centerDraw, dimXDraw, dimYDraw;
			double factorX, factorY, dimEscal;
			
			i_posNode(heigthMax, 
						indLevel, i, num, 
						&x_posNodeCenter, &y_posNodeCenter,
						&x_posOriginNode, &y_posOriginNode, 
						&x_SizeCell, &y_SizeCell);

			i_calculateDataTransformation(
							xmin, ymin, xmax, ymax,
							&x_centerDraw, &y_centerDraw,
							&dimXDraw, &dimYDraw);

			assert(dimXDraw > 0.);
			assert(dimYDraw > 0.);
			
			dimEscal = MAX(dimXDraw, dimYDraw);
							
			factorX = i_WIDTH_NODE / dimEscal;
			factorY = i_HEIGTH_NODE / dimEscal; 
			
			graphics->pushTransformation();
			
			graphics->traslation(x_posNodeCenter, y_posNodeCenter, 0.);
			
			graphics->scale(factorX, factorY, 1.);
			
			graphics->traslation(-x_centerDraw, -y_centerDraw, 0.);
			
			task_drawNode(task, func_drawNode, graphics);
			
			graphics->popTransformation();
			
			graphics->drawRect(
						x_posOriginNode, y_posOriginNode, 
						x_posOriginNode + x_SizeCell, 
						y_posOriginNode + y_SizeCell);
		}
	}
}

//---------------------------------------------------------

static void i_drawNodes(
					const ArrGroup(Task_t) *levelsTasks,
					double heigthMax,
					task_fptr_drawNode func_drawNode, 
					class IGraphics *graphics)
{
	unsigned long i, num;
	
	num = arrgroup_numGroups(Task_t, levelsTasks);
	
	for (i = 0; i < num; i++) 
	{
		const ArrTipo(Task_t) *level;
		
		level = arrgroup_Get(Task_t, levelsTasks, i);		
		i_drawRowNodes(level, i, heigthMax, func_drawNode, graphics);
	}
}

//---------------------------------------------------------

static void i_getLevelAndIndPos(
					const ArrGroup(Task_t) *levelsTasks,
					const struct Task_t *task,
					unsigned long *indLevel,
					unsigned long *indTask)
{
	bool exist;
	
	assert_no_null(indLevel);
	assert_no_null(indTask);
	
	exist = arrgroup_serachGroupAndElement(Task_t, levelsTasks, struct Task_t, task, indLevel, indTask, NULL);
	assert(exist == true);
}

//---------------------------------------------------------

static void i_drawNodeArrows(
					const ArrGroup(Task_t) *levelsTasks,
					const Graph(Task_t) *graphTask,
					const Task_t *task,
					double heigthMax, 
					double x_posNode, double y_posNode,
					class IGraphics *graphics)
{
	ArrTipo(Task_t) *sons;
	unsigned long i, num;
	
	sons = graph_sons(Task_t, graphTask, task);
	num = array_numElementos(Task_t, sons);
	
	for (i = 0; i < num; i++)
	{
		const struct Task_t *taskSon;
		const ArrTipo(Task_t) *level;
		unsigned long indLevelSon, indNodeSon, numNodesLevelSon;
		double x_posTaskSon, y_posTaskSon;
		
		taskSon = array_Get(Task_t, sons, i);
		i_getLevelAndIndPos(levelsTasks, taskSon, &indLevelSon, &indNodeSon);
		
		level = arrgroup_Get(Task_t, levelsTasks, indLevelSon);
		numNodesLevelSon = array_numElementos(Task_t, level);
		
		i_posNode(heigthMax, indLevelSon, indNodeSon, numNodesLevelSon, &x_posTaskSon, &y_posTaskSon, NULL, NULL, NULL, NULL);
		
		graphics->drawLine(x_posNode, y_posNode, 0., x_posTaskSon, y_posTaskSon, 0.);
	}
	
	array_Destruye(Task_t, &sons, NULL);
}

//---------------------------------------------------------

static void i_drawRowArrows(
					const ArrGroup(Task_t) *levelsTasks,
					const Graph(Task_t) *graphTask,
					const ArrTipo(Task_t) *tasks, 
					unsigned long indLevel, 
					double heigthMax, 
					class IGraphics *graphics)
{
	unsigned long i, num;
	
	num = array_numElementos(Task_t, tasks);
	
	for (i = 0; i < num; i++) 
	{
		double x_posNode, y_posNode;
		const struct Task_t *task;
			
		task = array_Get(Task_t, tasks, i);			
		i_posNode(heigthMax, indLevel, i, num, &x_posNode, &y_posNode, NULL, NULL, NULL, NULL);
		i_drawNodeArrows(
					levelsTasks, graphTask, task,
					heigthMax, x_posNode, y_posNode, graphics);
	}
}

//---------------------------------------------------------

static void i_drawArrows( 
					const ArrGroup(Task_t) *levelsTasks,
					Graph(Task_t) *graphTask,
					double heigthMax,
					class IGraphics *graphics)
{
	unsigned long i, num;
	
	num = arrgroup_numGroups(Task_t, levelsTasks);
	
	for (i = 0; i < num; i++) 
	{
		const ArrTipo(Task_t) *level;
		
		level = arrgroup_Get(Task_t, levelsTasks, i);
		i_drawRowArrows(levelsTasks, graphTask, level, i, heigthMax, graphics);
	}
}

//---------------------------------------------------------

void schedulerTask_drawPrivate(
					const struct SchedulerTask_t *schedulerTask,
					const char *nameLayerNodes,
					const char *nameLayerArrows,
					task_fptr_drawNode func_drawNode,
					class IGraphics *graphics)
{
	double heigthMax;
	
	assert_no_null(schedulerTask);
	
	heigthMax = i_calculateHeigthMax(schedulerTask->levelsTasks);
	
	if (heigthMax > 0.)
	{
	    graphics->setMaterial(nameLayerArrows);
		i_drawArrows(schedulerTask->levelsTasks, schedulerTask->graphTask, heigthMax, graphics); 
		
		graphics->setMaterial(nameLayerNodes);
		i_drawNodes(schedulerTask->levelsTasks, heigthMax, func_drawNode, graphics);
	}
} 
