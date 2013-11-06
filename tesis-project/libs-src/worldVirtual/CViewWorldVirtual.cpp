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
     
    dataWorkspaceWorldVirtual->incrRotateCamera(incrRotateX, 0., incrRotateZ);
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

            dataPrivate->dataWorkspaceWorldVirtual->backCamera(i_STEP);
            break;

        case EVTMOUSE_WHEEL_DOWN:

            dataPrivate->dataWorkspaceWorldVirtual->frontCamera(i_STEP);
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
            
            dataPrivate->dataWorkspaceWorldVirtual->frontCamera(i_STEP);
            evtkey_endEvent(evtKey);
            break;    
        
        case 'x':
        
            dataPrivate->dataWorkspaceWorldVirtual->backCamera(i_STEP);
            evtkey_endEvent(evtKey);
            break;    
        
        default:
            break;
        
    }
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

    panel = areadibujo_crea_modelo3d(
                    ancho, alto,
                    struct SDataPrivateViewWorldVirtual, dataPrivate,
                    i_positionCamera,
                    &funcion_dibujo, areaDibujo);

    rcpmouse = rcpmouse_create(SDataPrivateViewWorldVirtual, dataPrivate, i_onMouse);
    areadibujo_appendReceptorMouse(*areaDibujo, &rcpmouse);
    
    rcpkey = rcpkey_create(SDataPrivateViewWorldVirtual, dataPrivate, i_onKey);
    panel_appendReceptorKeys(panel, &rcpkey);

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

void CViewWorldVirtual::setInitialPositionCamera(class CDataWorkspaceWorldVirtual *dataWorkspaceWorldVirtual)
{
    assert_no_null(dataWorkspaceWorldVirtual);
    assert_no_null(m_dataPrivate);
    
	dataWorkspaceWorldVirtual->setInitialPositionCamera();
    areadibujo_redraw(m_dataPrivate->areaDibujo);    
}
