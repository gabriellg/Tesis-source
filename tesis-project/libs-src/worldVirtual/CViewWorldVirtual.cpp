// Definición de la vista.

#include "CViewWorldVirtual.inl"

#include "asrtbas.h"
#include "memory.h"
#include "panel.h"
#include "rcpaccion.h"
#include "evtaccion.h"
#include "rcpmouse.h"
#include "evtmouse.hxx"
#include "rcpkey.h"
#include "evtkey.h"

#include "CPositionCamera.hpp"
#include "CDataWorkspaceWorldVirtual.inl"
#include "IGraphics.hpp"
#include "areadibujo.hpp"
#include "rcpdibujo.hpp"
#include "CMaterial.hpp"
#include "evtmouse.h"

enum EPrvActionView
{
    PRV_NO_ACTION,
    PRV_ROTATE_SCENE,
    PRV_MOVE_EYE_CAMERA
};

struct SDataPrivateViewWorldVirtual
{
    class CDataWorkspaceWorldVirtual *dataWorkspaceWorldVirtual;
    struct areaDibujo_t *areaDibujo;
};

static const char *i_LAYER_AXIS_X = "AXIS_X";
static const char *i_LAYER_AXIS_Y = "AXIS_Y";
static const char *i_LAYER_AXIS_Z = "AXIS_Z";
static const char *i_LAYER_ORIGIN = "ORIGIN";
static const double i_STEP = .3;

//-----------------------------------------------------------------------

static struct SDataPrivateViewWorldVirtual *prv_createDataPrivateViewWorldVirtual(
                        class CDataWorkspaceWorldVirtual **dataWorkspaceWorldVirtual,
                        struct areaDibujo_t **areaDibujo)
{
    struct SDataPrivateViewWorldVirtual *dataPrivate;
    
    dataPrivate = MALLOC(struct SDataPrivateViewWorldVirtual);
    
    dataPrivate->dataWorkspaceWorldVirtual = ASSIGN_PP(dataWorkspaceWorldVirtual, class CDataWorkspaceWorldVirtual);
    dataPrivate->areaDibujo = ASSIGN_PP(areaDibujo, struct areaDibujo_t);
    
    return dataPrivate;
}

//-----------------------------------------------------------------------

CViewWorldVirtual::CViewWorldVirtual()
{
    class CDataWorkspaceWorldVirtual *dataWorkspaceWorldVirtual;
    struct areaDibujo_t *areaDibujo;
    
    dataWorkspaceWorldVirtual = NULL;
    areaDibujo = NULL; 
    m_dataPrivate = prv_createDataPrivateViewWorldVirtual(&dataWorkspaceWorldVirtual, &areaDibujo);
}
                    
//-----------------------------------------------------------------------

CViewWorldVirtual::~CViewWorldVirtual()
{
    FREE_T(&m_dataPrivate, struct SDataPrivateViewWorldVirtual);
}

//-----------------------------------------------------------------------

static void i_defineLayers(const struct SDataPrivateViewWorldVirtual *dataPrivate, class IGraphics *graphics)
{
    assert_no_null(dataPrivate);
    assert_no_null(dataPrivate->dataWorkspaceWorldVirtual);
    dataPrivate->dataWorkspaceWorldVirtual->defineLayers(graphics);
}

//-----------------------------------------------------------------------

static void i_dibuja_vista(
                    const struct SDataPrivateViewWorldVirtual *dataPrivate, 
                    class IGraphics *graphics)
{
    assert_no_null(graphics);

    graphics->pushTransformation();
	
    assert_no_null(dataPrivate);
    assert_no_null(dataPrivate->dataWorkspaceWorldVirtual);
    dataPrivate->dataWorkspaceWorldVirtual->draw(graphics);
    
    graphics->popTransformation();
}

//-----------------------------------------------------------------------

static void i_positionCamera(
                    const struct SDataPrivateViewWorldVirtual *dataPrivate, 
                    class IGraphics *graphics)
{
    assert_no_null(dataPrivate);
    dataPrivate->dataWorkspaceWorldVirtual->positionCamera(graphics);
}

//-----------------------------------------------------------------------

static enum EPrvActionView i_isEventActionScene(const struct EvtMouse_t *evtMouse, double *incrX, double *incrY)
{
    enum EPrvActionView actionView;
    
    assert_no_null(evtMouse);
    
    switch(evtmouse_type(evtMouse))
    {
        case EVTMOUSE_PULSE:
        
            if (evtmouse_hasDrag(evtMouse, incrX, incrY) == true)
            {
                if (evtmouse_isControlPulsed(evtMouse) == true)
                    actionView = PRV_MOVE_EYE_CAMERA;
                else
                    actionView = PRV_ROTATE_SCENE;
            }
            else
                actionView = PRV_NO_ACTION;
            break;
        
        case EVTMOUSE_MOVE:
        case EVTMOUSE_RELEASE:
        case EVTMOUSE_OUT:
        case EVTMOUSE_IN:
        
            actionView = PRV_NO_ACTION;
            break;
            
        default_error();
    }
    
    return actionView;
}

//-----------------------------------------------------------------------

static void i_rotarEscena(
                        struct areaDibujo_t *areaDibujo,
                        class CDataWorkspaceWorldVirtual *dataWorkspaceWorldVirtual,
                        double incrX, double incrY)
{
    unsigned long width, heigth;
    double incrRotateX, incrRotateZ;
    
    assert_no_null(dataWorkspaceWorldVirtual);
    
    areadibujo_size(areaDibujo, &width, &heigth);
    
    incrRotateZ = incrX * (360. / width);
    incrRotateX = incrY * (360. / heigth);
     
    dataWorkspaceWorldVirtual->incrRotateCamera(areaDibujo, incrRotateX, 0., incrRotateZ);
}

//-----------------------------------------------------------------------

static void i_moveEyeCameraEscena(
                        struct areaDibujo_t *areaDibujo,
                        class CDataWorkspaceWorldVirtual *dataWorkspaceWorldVirtual,
                        double incrX, double incrY)
{
    //TODO: TERMINAR;
}

//-----------------------------------------------------------------------

static void i_procesar_wheel(struct SDataPrivateViewWorldVirtual *dataPrivate, const struct EvtMouse_t *event)
{
    enum evtmouse_type_t typeMouse;

    assert_no_null(dataPrivate);

    typeMouse = evtmouse_type(event);

    switch(typeMouse)
    {
        case EVTMOUSE_OUT:
        case EVTMOUSE_IN:
        case EVTMOUSE_MOVE:
        case EVTMOUSE_PULSE:
        case EVTMOUSE_RELEASE:

            break;

        case EVTMOUSE_WHEEL_UP:

            dataPrivate->dataWorkspaceWorldVirtual->backCamera(dataPrivate->areaDibujo, i_STEP);
            break;

        case EVTMOUSE_WHEEL_DOWN:

            dataPrivate->dataWorkspaceWorldVirtual->frontCamera(dataPrivate->areaDibujo, i_STEP);
            break;

        default_error();
    }
}

//-----------------------------------------------------------------------

static void i_onMouse(const struct EvtMouse_t *evtMouse, struct SDataPrivateViewWorldVirtual *dataPrivate)
{
    enum evtmouse_button_t button;

    button = evtmouse_button(evtMouse);

    switch (button)
    {
        case EVTMOUSE_LEFT:
        {
            enum EPrvActionView actionView;
            double incrX, incrY;

            actionView = i_isEventActionScene(evtMouse, &incrX, &incrY);

            switch(actionView)
            {
                case PRV_NO_ACTION:

                    break;

                case PRV_ROTATE_SCENE:

                    i_rotarEscena(dataPrivate->areaDibujo, dataPrivate->dataWorkspaceWorldVirtual, incrX, incrY);
                    break;

                case PRV_MOVE_EYE_CAMERA:

                    i_moveEyeCameraEscena(dataPrivate->areaDibujo, dataPrivate->dataWorkspaceWorldVirtual, incrX, incrY);
                    break;

                default_error();
            }
            break;
        }

        case EVTMOUSE_WHEEL:

            i_procesar_wheel(dataPrivate, evtMouse);
            break;

        case EVTMOUSE_NO_BUTTON:
        case EVTMOUSE_MIDDLE:
        case EVTMOUSE_RIGHT:

            break;

        default_error();
    }
}

//-----------------------------------------------------------------------

static void i_onKey(struct EvtKey_t *evtKey, struct SDataPrivateViewWorldVirtual *dataPrivate)
{
    unsigned int key;
    
    assert_no_null(dataPrivate);
    assert_no_null(dataPrivate->dataWorkspaceWorldVirtual);
    
    key = evtkey_key(evtKey);
    
    switch(key)
    {
        case 'w':
            
            dataPrivate->dataWorkspaceWorldVirtual->frontCamera(dataPrivate->areaDibujo, i_STEP);
            evtkey_endEvent(evtKey);
            break;    
        
        case 'x':
        
            dataPrivate->dataWorkspaceWorldVirtual->backCamera(dataPrivate->areaDibujo, i_STEP);
            evtkey_endEvent(evtKey);
            break;    
        
        default:
            break;
        
    }
}

//-----------------------------------------------------------------------

static void i_defineLayersAxis(const struct SDataPrivateViewWorldVirtual *dataPrivate, class IGraphics *graphics)
{
    class CMaterial *layerAxisX, *layerAxisY, *layerAxisZ, *layerAxisOrigin;

    assert_no_null(graphics);

    layerAxisX = new CMaterial(i_LAYER_AXIS_X, 0., 1., 0., 1.);
    layerAxisY = new CMaterial(i_LAYER_AXIS_Y, 1., 0., 0., 1.);
    layerAxisZ = new CMaterial(i_LAYER_AXIS_Z, 0., 0., 1., 1.);
    layerAxisOrigin = new CMaterial(i_LAYER_ORIGIN, 0.5, 0.5, 0.5, 1.);

    graphics->defineMaterial(layerAxisX);
    graphics->defineMaterial(layerAxisY);
    graphics->defineMaterial(layerAxisZ);
    graphics->defineMaterial(layerAxisOrigin);

    delete layerAxisX;
    delete layerAxisY;
    delete layerAxisZ;
    delete layerAxisOrigin;
}

//-----------------------------------------------------------------------

static void i_drawAxis(class IGraphics *graphics, const char *nameLayer)
{
    assert_no_null(graphics);

    graphics->setMaterial(nameLayer);
    graphics->scale(0.2, 0.2, 2.);
    graphics->drawUnitCylinder(IGraphics::LEVELDETAIL_NORMAL, IGraphics::TYPEDRAW_SOLID);
    graphics->traslation(0.,0.,1.);
    graphics->scale(2., 2., 0.4);

    graphics->drawUnitCone(IGraphics::LEVELDETAIL_NORMAL, IGraphics::TYPEDRAW_SOLID);
    graphics->scale(-1., 1., 1.);
	graphics->drawUnitDisk(IGraphics::LEVELDETAIL_NORMAL, IGraphics::TYPEDRAW_SOLID);
}

//-----------------------------------------------------------------------

static void i_drawAxisX(class IGraphics *graphics)
{
    assert_no_null(graphics);

    graphics->pushTransformation();
	
    graphics->rotation(90., 0., 1., 0.);
	i_drawAxis(graphics, i_LAYER_AXIS_X);
	
	graphics->popTransformation();
}

//-----------------------------------------------------------------------

static void i_drawAxisY(class IGraphics *graphics)
{
    assert_no_null(graphics);

    graphics->pushTransformation();
	
    graphics->rotation(-90., 1., 0., 0.);
	i_drawAxis(graphics, i_LAYER_AXIS_Y);
	
	graphics->popTransformation();
}

//-----------------------------------------------------------------------

static void i_drawAxisZ(class IGraphics *graphics)
{
    assert_no_null(graphics);

    graphics->pushTransformation();
	
	i_drawAxis(graphics, i_LAYER_AXIS_Z);
	
	graphics->popTransformation();
}

//-----------------------------------------------------------------------

static void i_drawAllAxis(const struct SDataPrivateViewWorldVirtual *dataPrivate, class IGraphics *graphics)
{
	assert_no_null(dataPrivate);
	assert_no_null(dataPrivate->dataWorkspaceWorldVirtual);
    assert_no_null(graphics);
	
	graphics->pushTransformation();
	graphics->resetTransformation();
	
	dataPrivate->dataWorkspaceWorldVirtual->makeRotationCamera(graphics);
	
	i_drawAxisX(graphics);
	i_drawAxisY(graphics);
	i_drawAxisZ(graphics);
	
	graphics->popTransformation();
	
	graphics->pushTransformation();
	graphics->scale(0.5, 0.5, 0.5);
	graphics->setMaterial(i_LAYER_ORIGIN);
	graphics->drawUnitSphere(IGraphics::LEVELDETAIL_NORMAL, IGraphics::TYPEDRAW_SOLID);
	graphics->popTransformation();
	
}
//-----------------------------------------------------------------------

static bool i_bounds(
                const struct SDataPrivateViewWorldVirtual *dataPrivate,
                double *ox, double *oy, double *oz,
                double *ex, double *ey, double *ez)
{
	assert_no_null(dataPrivate);
	
	*ox = -2.5; 
	*oy = -2.5; 
	*oz = -2.5;
	
	*ex = 2.5; 
	*ey = 2.5; 
	*ez = 2.5;
	
	return true;
}
//-----------------------------------------------------------------------

static struct panel_t *i_viewWorldVirtual(
                        const class CViewWorldVirtual *view,
                        struct SDataPrivateViewWorldVirtual *dataPrivate,
                        struct areaDibujo_t **areaDibujo)
{
    struct panel_t *panel;
    unsigned long ancho, alto;
    struct rcpdibujo_t *funcion_dibujo;
    struct rcpdibujo_t *funcion_dibujo_ejes;
    struct RcpMouse_t *rcpmouse;
    struct RcpKey_t *rcpkey;
    
    ancho = 200; 
    alto = 400;

    funcion_dibujo = rcpdibujo_crea(i_defineLayers, i_dibuja_vista, SDataPrivateViewWorldVirtual, dataPrivate);

    if (dataPrivate->dataWorkspaceWorldVirtual->isArea3D() == true)
    {
    	panel = areadibujo_crea_modelo3d(
                        ancho, alto,
                        struct SDataPrivateViewWorldVirtual, dataPrivate,
                        i_positionCamera,
                        &funcion_dibujo, areaDibujo);
    
		rcpmouse = rcpmouse_create(SDataPrivateViewWorldVirtual, dataPrivate, i_onMouse);
		areadibujo_appendReceptorMouse(*areaDibujo, &rcpmouse);
		
		rcpkey = rcpkey_create(SDataPrivateViewWorldVirtual, dataPrivate, i_onKey);
		panel_appendReceptorKeys(panel, &rcpkey);

		funcion_dibujo_ejes = rcpdibujo_crea_con_limites(i_defineLayersAxis, i_drawAllAxis, i_bounds, SDataPrivateViewWorldVirtual, dataPrivate);
		areadibujo_appendZonaDibujoIzqInferior(*areaDibujo, 92, 92, &funcion_dibujo_ejes);
    }        
    else
    {
    	panel = areadibujo_crea_modelo2d(
						ancho, alto,
                        &funcion_dibujo, areaDibujo);
    }

    return panel;             
}

//-----------------------------------------------------------------------

struct panel_t *CViewWorldVirtual::panel(class CDataWorkspace *dataWorkspace)
{
    struct panel_t *panel;
	class CDataWorkspaceWorldVirtual *dataWorkspaceWorldVirtual;
    
    assert_no_null(m_dataPrivate);
    assert(m_dataPrivate->dataWorkspaceWorldVirtual == NULL);
    assert(m_dataPrivate->areaDibujo == NULL);

	dataWorkspaceWorldVirtual = (class CDataWorkspaceWorldVirtual *)dataWorkspace;
    m_dataPrivate->dataWorkspaceWorldVirtual = dataWorkspaceWorldVirtual;

	panel = i_viewWorldVirtual(this, m_dataPrivate, &m_dataPrivate->areaDibujo);

    return panel;
}

//-----------------------------------------------------------------------

void CViewWorldVirtual::setRotationCamera(double rotXDegrees, double rotYDegrees, double rotZDegrees)
{
	assert_no_null(m_dataPrivate);
	m_dataPrivate->dataWorkspaceWorldVirtual->setRotationCamera(m_dataPrivate->areaDibujo, rotXDegrees, rotYDegrees, rotZDegrees);
}

//-----------------------------------------------------------------------

void CViewWorldVirtual::setInitialPositionCamera(class CDataWorkspaceWorldVirtual *dataWorkspaceWorldVirtual)
{
    assert_no_null(dataWorkspaceWorldVirtual);
    assert_no_null(m_dataPrivate);
    
	dataWorkspaceWorldVirtual->setInitialPositionCamera();
    areadibujo_redraw(m_dataPrivate->areaDibujo);    
}
