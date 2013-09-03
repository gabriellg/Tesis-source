// Define el mundo para las tormentas.

#include "CWorldStorm.hpp"

#include "CGeneratorEventsTree.inl"
#include "CCreatorElements.inl"
#include "CDisplay3DMetereo.inl"

#include "asrtbas.h"

#include "CFigure.hpp"
#include "CScene.hpp"
#include "CGestorDisplays.hpp"

static unsigned long prv_NUM_ROWS = 10;
static unsigned long prv_NUM_COLS = 10;

//-----------------------------------------------------------------------

CWorldStorm::CWorldStorm()
{
}

//-----------------------------------------------------------------------

CWorldStorm::~CWorldStorm()
{
}

//-----------------------------------------------------------------------

void CWorldStorm::defineLayers(class IGraphics *graphics)
{
    CDisplay3DMetereo::defineLayers(graphics);
}

//-----------------------------------------------------------------------

void CWorldStorm::appendDisplays(class CGestorDisplays *displays)
{
	class IDisplay *display3D;
	
	assert_no_null(displays);

	display3D = CDisplay3DMetereo::createDisplay3d(prv_NUM_ROWS, prv_NUM_COLS);
	displays->appendDisplay(&display3D);
}

//-----------------------------------------------------------------------

void CWorldStorm::appendElementsToScene(class CScene *scene)
{
    class CAgent *agentGenerator;
    class CAgent *agentForrest, *agentCreatorElements;
    
    assert_no_null(scene);
    
    agentGenerator = new CGeneratorEventsTree();
    agentCreatorElements = new CCreatorElements();
    agentGenerator->appendChild(&agentCreatorElements);

    agentForrest = new CFigure(CDisplay3DMetereo::SYMBOL_FORREST);

    scene->appendAgent(&agentForrest);
    scene->appendAgent(&agentGenerator);
} 
