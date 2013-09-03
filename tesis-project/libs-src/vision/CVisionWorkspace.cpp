// Workspace para vision.

#include "CVisionWorkspace.hpp"

#include "CVisionView.hpp"
#include "CFactoryProcessGraph.hpp"
#include "CVisionDataWorkspace.hpp"
#include "CProcessGraphImages.hpp"
#include "CRcpOpcRunGraph.hpp"
#include "CRcpOpcRunOneLevel.hpp"

#include "asrtbas.h"
#include "array.h"

//-----------------------------------------------------------------------

CVisionWorkspace::CVisionWorkspace(const class CFactoryProcessGraph *factoryProcessGraph)
{
	m_factoryProcessGraph = factoryProcessGraph;
}

//-----------------------------------------------------------------------

CVisionWorkspace::~CVisionWorkspace()
{
	;
}
		
//-----------------------------------------------------------------------

CDataWorkspace *CVisionWorkspace::createDataWorkspace(void)
{
	class CProcessGraphImages *processGraphImage;
	
	assert_no_null(m_factoryProcessGraph);
	
	processGraphImage = m_factoryProcessGraph->processGraphImages();
	
	return new CVisionDataWorkspace(&processGraphImage);
}

//-----------------------------------------------------------------------

CView *CVisionWorkspace::createView(class CDataWorkspace *dataWorkspace)
{
	return new CVisionView();
}

//-----------------------------------------------------------------------

void CVisionWorkspace::defineMenu(void)
{
	class CRcpOpcAreaDib *optionRun;
	class CRcpOpcAreaDib *optionRunOneLevel;
	
	optionRun = new CRcpOpcRunGraph();
	optionRunOneLevel = new CRcpOpcRunOneLevel();
	
	appendMenu("Ejecutar");
	appendOptionIntoLastMenu(optionRunOneLevel);
	appendOptionIntoLastMenu(optionRun);
}
