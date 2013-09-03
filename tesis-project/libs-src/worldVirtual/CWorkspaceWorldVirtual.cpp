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

#include "CWorkspaceWorldVirtual__planoxy.iig"
#include "CWorkspaceWorldVirtual__planoxz.iig"
#include "CWorkspaceWorldVirtual__planozy.iig"
#include "CWorkspaceWorldVirtual__planoiso.iig"
#include "CWorkspaceWorldVirtual__play.iig"
#include "CWorkspaceWorldVirtual__stop.iig"
#include "CWorkspaceWorldVirtual__pausa.iig"

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
    
    imageIcon = cimg_load_local_png(planoxy);
    workSpace->appendOptionWithIconIntoLastMenu(&imageIcon, rcpOption);
    
    rcpOption = new CRcpActionViewPlaneXZ();
    imageIcon = cimg_load_local_png(planoxz);
    workSpace->appendOptionWithIconIntoLastMenu(&imageIcon, rcpOption);
    
    rcpOption = new CRcpActionViewPlaneZY();
    imageIcon = cimg_load_local_png(planozy);
    workSpace->appendOptionWithIconIntoLastMenu(&imageIcon, rcpOption);

    rcpOption = new CRcpActionViewPlaneIso();
    imageIcon = cimg_load_local_png(planoiso);
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
    imageIcon = cimg_load_local_png(play);
    workSpace->appendOptionWithIconIntoLastMenu(&imageIcon, rcpOption);
    
    rcpOption = new CRcpActionPause();
    imageIcon = cimg_load_local_png(pausa);
    workSpace->appendOptionWithIconIntoLastMenu(&imageIcon, rcpOption);
    
    rcpOption = new CRcpActionStop();
    imageIcon = cimg_load_local_png(stop);
    workSpace->appendOptionWithIconIntoLastMenu(&imageIcon, rcpOption);
} 

//-----------------------------------------------------------------------

void CWorkspaceWorldVirtual::defineMenu(void)
{
    i_appendMenuView(this);
    i_appendMenuAnimation(this);
}
