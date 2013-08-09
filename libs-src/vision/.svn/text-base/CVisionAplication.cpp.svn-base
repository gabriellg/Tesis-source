#include "CVisionAplication.hpp"

#include "CVisionWorkspace.hpp"
#include "CFactoryProcessGraph.hpp"
#include "memory.h"

//-----------------------------------------------------------------------

CVisionAplication::CVisionAplication(
						class CFactoryProcessGraph *factoryProcessGraph, 
						const char *titleAplication) : 
						CAplicationGTK(titleAplication)
{
	m_factoryProcessGraph = factoryProcessGraph;
}

//-----------------------------------------------------------------------

CVisionAplication::~CVisionAplication()
{
	delete m_factoryProcessGraph;
}

//-----------------------------------------------------------------------

CWorkspace *CVisionAplication::createWorkspace()
{
	return new CVisionWorkspace(m_factoryProcessGraph);
}
