// Receptores para situar la cámara en diferentes vistas 

#include "CRcpActionView.inl"

#include "CViewWorldVirtual.inl"

#include "asrtbas.h"
#include "CEvtOpcAreaDib.hpp"

//---------------------------------------------------------------

static void i_setRotationCamera(
                        class CEvtOpcAreaDib *evtAreaDib,
                        double rotXDegrees, double rotYDegrees, double rotZDegrees)
{
    class CViewWorldVirtual *viewWorld;
    
    assert_no_null(evtAreaDib);
    
    viewWorld = (class CViewWorldVirtual *)evtAreaDib->getViewCurrent();
    assert_no_null(viewWorld);
    viewWorld->setRotationCamera(rotXDegrees, rotYDegrees, rotZDegrees);
} 

//---------------------------------------------------------------

void CRcpActionViewPlaneXZ::action(class CEvtOpcAreaDib *evtAreaDib)
{
    double rotXDegrees, rotYDegrees, rotZDegrees;
    
    rotXDegrees = -90.; 
    rotYDegrees = 0.; 
    rotZDegrees = 0.;
    
    i_setRotationCamera(evtAreaDib, rotXDegrees, rotYDegrees, rotZDegrees);
}       

//---------------------------------------------------------------

void CRcpActionViewPlaneXY::action(class CEvtOpcAreaDib *evtAreaDib)
{
    double rotXDegrees, rotYDegrees, rotZDegrees;
    
    rotXDegrees = 0.; 
    rotYDegrees = 0.; 
    rotZDegrees = 0.;
    
    i_setRotationCamera(evtAreaDib, rotXDegrees, rotYDegrees, rotZDegrees);
}       

//---------------------------------------------------------------

void CRcpActionViewPlaneZY::action(class CEvtOpcAreaDib *evtAreaDib)
{
    double rotXDegrees, rotYDegrees, rotZDegrees;
    
    rotXDegrees = 0.; 
    rotYDegrees = 90.; 
    rotZDegrees = 0.;
    
    i_setRotationCamera(evtAreaDib, rotXDegrees, rotYDegrees, rotZDegrees);
}       

//---------------------------------------------------------------

void CRcpActionViewPlaneIso::action(class CEvtOpcAreaDib *evtAreaDib)
{
    double rotXDegrees, rotYDegrees, rotZDegrees;
    
    rotXDegrees = -45.; 
    rotYDegrees = 0.; 
    rotZDegrees = -45.;
    
    i_setRotationCamera(evtAreaDib, rotXDegrees, rotYDegrees, rotZDegrees);
}       

//---------------------------------------------------------------

void CRcpActionInitialPosition::action(class CEvtOpcAreaDib *evtAreaDib)
{
    class CDataWorkspaceWorldVirtual *dataWorkspaceWorld;
    class CViewWorldVirtual *viewWorld;
    
    assert_no_null(evtAreaDib);
    
    dataWorkspaceWorld = (class CDataWorkspaceWorldVirtual *)evtAreaDib->getDataWorkspace();
    assert_no_null(dataWorkspaceWorld);
    
    viewWorld = (class CViewWorldVirtual *)evtAreaDib->getViewCurrent();
    assert_no_null(viewWorld);
    
    viewWorld->setInitialPositionCamera(dataWorkspaceWorld);    
}       
