#include "CProcessGraphImages.hpp"

#include "asrtbas.h"
#include "array.h"
#include "table.h"

#include "task.hpp"
#include "schedulerTask.hpp"
#include "CProcessImage.hpp"
#include "CResultData.hpp"
#include "IGraphics.hpp"
#include "CMaterial.hpp"

//---------------------------------------------------------
	
CProcessGraphImages::CProcessGraphImages()
{
	m_schedulerImages = schedulerTask_create();
	m_allProcess = array_CreaClassPuntero(0, CProcessImage);
}

//---------------------------------------------------------

static void i_destruyeProcessImage(class CProcessImage **processImage)
{
	assert_no_null(processImage);
	assert_no_null(*processImage);
	delete *processImage;
	*processImage = NULL;
}

//---------------------------------------------------------

CProcessGraphImages::~CProcessGraphImages()
{
	schedulerTask_destroy(&m_schedulerImages);
	array_DestruyeClass(CProcessImage, &m_allProcess, i_destruyeProcessImage);
}

//---------------------------------------------------------
	
void CProcessGraphImages::initDefineGraphProcess()
{
	assert(array_numElementosClass(CProcessImage, m_allProcess) == 0);
	schedulerTask_openTasks(m_schedulerImages);
}

//---------------------------------------------------------

static class CResultData *i_executeProcessImage(
						class CProcessImage *processImage, 
						const ArrClass(CResultData) *fathers, 
						struct Table_t *table)
{
	assert_no_null(processImage);
	assert_no_null(table);
	
	return processImage->processImage(fathers);
}

//---------------------------------------------------------
	
static void i_destroyDataResult(class CResultData **resultData)
{
	assert_no_null(resultData);
	delete *resultData;
	*resultData = NULL;
}

//---------------------------------------------------------
	
void CProcessGraphImages::addProcessImage(
						const char *idProcess,
						class CProcessImage **processImage)
{
	struct Task_t *task;
	
	assert_no_null(processImage);
	assert_no_null(*processImage);
	
	task = task_createTaskClass(
						CProcessImage, CResultData, 
						idProcess, 
						*processImage,
						i_executeProcessImage, i_destroyDataResult);
						
	schedulerTask_appendTasks(m_schedulerImages, task);
	array_AnyadirClass(CProcessImage, m_allProcess, *processImage);
	*processImage = NULL;
}

//---------------------------------------------------------
	
void CProcessGraphImages::addDependProcess(const char *idProcessIn, const char *idProcessOut)
{
	schedulerTask_dependIdTasks(m_schedulerImages, idProcessIn, idProcessOut);
}

//---------------------------------------------------------
	
void CProcessGraphImages::closeDefineGraphProcess()
{
	schedulerTask_closeTasks(m_schedulerImages);
	schedulerTask_prepareTasks(m_schedulerImages);
}

//---------------------------------------------------------
	
void CProcessGraphImages::run()
{
	struct Table_t *table;
	
	table = table_create();
	schedulerTask_executeTask(m_schedulerImages, table);
	table_destroy(&table);
}

//---------------------------------------------------------
	
void CProcessGraphImages::runNextLevel()
{
	struct Table_t *table;
	
	table = table_create();
	schedulerTask_executeNextLevelTasks(m_schedulerImages, table);
	table_destroy(&table);
}

//---------------------------------------------------------

void CProcessGraphImages::defineLayers(class IGraphics *graphics) const
{
    class CMaterial *nodosMaterial, *relacionMaterial;

    nodosMaterial = new CMaterial("Nodos",  1., 0., 0., 1.);
    relacionMaterial = new CMaterial("Relacion", 0., 0., 1., 1.);

    graphics->defineMaterial(nodosMaterial);
    graphics->defineMaterial(relacionMaterial);

    delete nodosMaterial;
    delete relacionMaterial;
}	

//---------------------------------------------------------

static void i_drawNode(
					const class CProcessImage *processImage, 
					const class CResultData *dataResult, 
					class IGraphics *graphics)
{
	assert_no_null(processImage);
	processImage->draw(dataResult, graphics);
}

//---------------------------------------------------------
	
void CProcessGraphImages::draw(class IGraphics *graphics) const
{
	scheduleTask_drawClass(
					CProcessImage, CResultData, 
					m_schedulerImages,
					"Nodos", "Relacion", 
					i_drawNode, graphics);
} 
