// Define el workspace para la aplicación de mundos virutales.

#include "CWorkspaceWorldVirtual.inl"

#include "CDataWorkspaceWorldVirtual.inl"
#include "CViewWorldVirtual.inl"
#include "CRcpActionView.inl"
#include "CRcpActionAnimation.inl"
#include "CRcpActionRun.inl"

#include "asrtbas.h"
#include "memory.h"
#include "CImg.hpp"

//-----------------------------------------------------------------------

CWorkspaceWorldVirtual::CWorkspaceWorldVirtual(class CDefineWorld **defineWorld)
{
    m_defineWorld = ASSIGN_PP_NO_NULL(defineWorld, class CDefineWorld);
}

//-----------------------------------------------------------------------

CWorkspaceWorldVirtual::~CWorkspaceWorldVirtual()
{
    assert(m_defineWorld == NULL);
}

//-----------------------------------------------------------------------

class CDataWorkspace *CWorkspaceWorldVirtual::createDataWorkspace(void)
{
	return new CDataWorkspaceWorldVirtual(&m_defineWorld);
}

//-----------------------------------------------------------------------

class CView *CWorkspaceWorldVirtual::createView(class CDataWorkspace *dataWorkspace)
{
	return new CViewWorldVirtual();
}

//-----------------------------------------------------------------------

static void i_appendMenuView(class CWorkspaceWorldVirtual *workSpace)
{
    class CRcpOpcAreaDib *rcpOption;
    class CImg *imageIcon;
    
    workSpace->appendMenu("Vistas");
    
    rcpOption = new CRcpActionViewPlaneXY();
    
    imageIcon = new CImg("./imagesWorkspace/planoxy.png");
    workSpace->appendOptionWithIconIntoLastMenu(&imageIcon, rcpOption);
    
    rcpOption = new CRcpActionViewPlaneXZ();
    imageIcon = new CImg("./imagesWorkspace/planoxz.png");
    workSpace->appendOptionWithIconIntoLastMenu(&imageIcon, rcpOption);
    
    rcpOption = new CRcpActionViewPlaneZY();
    imageIcon = new CImg("./imagesWorkspace/planozy.png");
    workSpace->appendOptionWithIconIntoLastMenu(&imageIcon, rcpOption);

    rcpOption = new CRcpActionViewPlaneIso();
    imageIcon = new CImg("./imagesWorkspace/planoiso.png");
    workSpace->appendOptionWithIconIntoLastMenu(&imageIcon, rcpOption);
    
    workSpace->appendSeparatorIntoLastMenu();

    rcpOption = new CRcpActionInitialPosition();
    workSpace->appendOptionIntoLastMenu(rcpOption);
} 

//-----------------------------------------------------------------------

static void i_appendMenuAnimation(class CWorkspaceWorldVirtual *workSpace)
{
    class CRcpOpcAreaDib *rcpOption;
    class CImg *imageIcon;
    
    workSpace->appendMenu("Animación");

    rcpOption = new CRcpActionRun();
    imageIcon = new CImg("./imagesWorkspace/play.png");
    workSpace->appendOptionWithIconIntoLastMenu(&imageIcon, rcpOption);
    
    rcpOption = new CRcpActionPause();
    imageIcon = new CImg("./imagesWorkspace/pausa.png");
    workSpace->appendOptionWithIconIntoLastMenu(&imageIcon, rcpOption);
    
    rcpOption = new CRcpActionStop();
    imageIcon = new CImg("./imagesWorkspace/stop.png");
    workSpace->appendOptionWithIconIntoLastMenu(&imageIcon, rcpOption);
} 

//-----------------------------------------------------------------------

void CWorkspaceWorldVirtual::defineMenu(void)
{
    i_appendMenuView(this);
    i_appendMenuAnimation(this);
}
