#include "CVisionDataWorkspace.hpp"
#include "CProcessGraphImages.hpp"

#include "asrtbas.h"
#include "memory.h"

//-----------------------------------------------------------------------

CVisionDataWorkspace::CVisionDataWorkspace(class CProcessGraphImages **processGraphImage) : CDataWorkspace()
{
	m_processGraphImage = ASSIGN_PP_NO_NULL(processGraphImage, CProcessGraphImages);
}

//-----------------------------------------------------------------------

CVisionDataWorkspace::~CVisionDataWorkspace()
{
	delete m_processGraphImage;
	m_processGraphImage = NULL;
}

//-----------------------------------------------------------------------

void CVisionDataWorkspace::runProcessImages()
{
	m_processGraphImage->run();
}

//-----------------------------------------------------------------------

void CVisionDataWorkspace::runNextLevel()
{
	m_processGraphImage->runNextLevel();
}

//-----------------------------------------------------------------------

void CVisionDataWorkspace::defineLayers(class IGraphics *graphics) const
{
	assert_no_null(m_processGraphImage);
	m_processGraphImage->defineLayers(graphics);
}

//-----------------------------------------------------------------------

void CVisionDataWorkspace::draw(class IGraphics *graphics) const
{
	assert_no_null(m_processGraphImage);
	m_processGraphImage->draw(graphics);
}
