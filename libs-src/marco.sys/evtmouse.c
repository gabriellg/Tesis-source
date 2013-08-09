// Define un evento del raton

#include "evtmouse.h"
#include "evtmouse.syh"

#include "asrtbas.h"
#include "memory.h"

#include "CMath.hpp"
#include "IGraphics.hpp"

struct EvtMouse_t
{
    enum evtmouse_type_t type;
    enum evtmouse_button_t button;
    
    bool isShiftPulsed;
    bool isControlPulsed;

    bool hasPreviuos;
    double xPixelPrevious, yPixelPrevius;
    
    double xPixel, yPixel;
    
    class IGraphics *graphics;
};

static const double PRV_PRECISION = 1e-5;

//---------------------------------------------------------

static struct EvtMouse_t *i_create(
                        enum evtmouse_type_t type,
                        enum evtmouse_button_t button,
                        bool hasPreviuos,
                        bool isShiftPulsed,
                        bool isControlPulsed,
                        double xPixelPrevious, double yPixelPrevius,
                        double xPixel, double yPixel, 
                        class IGraphics *graphics)
{
    struct EvtMouse_t *event;
        
    event = MALLOC(struct EvtMouse_t);
        
    event->type = type;
    event->button = button;

    event->isShiftPulsed = isShiftPulsed;
    event->isControlPulsed = isControlPulsed;
    
    event->hasPreviuos = hasPreviuos;
    
    event->xPixelPrevious = xPixelPrevious;  
    event->yPixelPrevius = yPixelPrevius; 
    
    event->xPixel = xPixel;
    event->yPixel = yPixel;
    
    event->graphics = graphics;
        
    return event;
}

//---------------------------------------------------------

static struct EvtMouse_t *i_createWitoutPreviousPoint(
                        enum evtmouse_type_t type,
                        enum evtmouse_button_t button,
                        bool isShiftPulsed,
                        bool isControlPulsed,
                        double xPixel, double yPixel, 
                        class IGraphics *graphics)
{
    bool hasPreviuos;
    double xPixelPrevious, yPixelPrevius;

    hasPreviuos = false;
    xPixelPrevious = CMath::maxDouble();
    yPixelPrevius = CMath::maxDouble();
                        
    return i_create(type, button, 
                        hasPreviuos,
                        isShiftPulsed, isControlPulsed,
                        xPixelPrevious, yPixelPrevius,
                        xPixel, yPixel, graphics);
}

//---------------------------------------------------------

struct EvtMouse_t *evtmouse_createMove(
                        bool isShiftPulsed,
                        bool isControlPulsed,
                        double xPixel, double yPixel, 
                        class IGraphics *graphics)
{
    return i_createWitoutPreviousPoint(
                        EVTMOUSE_MOVE,
                        EVTMOUSE_NO_BUTTON,
                        isShiftPulsed, isControlPulsed,
                        xPixel, yPixel, 
                        graphics);
}

//---------------------------------------------------------

struct EvtMouse_t *evtmouse_createPulse(
                        enum evtmouse_button_t button,
                        bool hasPreviuos,
                        bool isShiftPulsed,
                        bool isControlPulsed,
                        double xPixelPrevious, double yPixelPrevius,
                        double xPixel, double yPixel, 
                        class IGraphics *graphics)
{
    return i_create(EVTMOUSE_PULSE, button,
                        hasPreviuos,
                        isShiftPulsed, isControlPulsed,
                        xPixelPrevious, yPixelPrevius,
                        xPixel, yPixel, graphics);
}

//---------------------------------------------------------

struct EvtMouse_t *evtmouse_createRelease(
                        enum evtmouse_button_t button,
                        bool isShiftPulsed, bool isControlPulsed,
                        double xPixel, double yPixel, 
                        class IGraphics *graphics)
{
    return i_createWitoutPreviousPoint(
                        EVTMOUSE_RELEASE,
                        button, isShiftPulsed, isControlPulsed, xPixel, yPixel, graphics);
}

//---------------------------------------------------------

struct EvtMouse_t *evtmouse_createIn(class IGraphics *graphics)
{
    return i_createWitoutPreviousPoint(
                        EVTMOUSE_IN,
                        EVTMOUSE_NO_BUTTON, false, false, CMath::maxDouble(), CMath::maxDouble(), graphics);
}

//---------------------------------------------------------

struct EvtMouse_t *evtmouse_createOut(class IGraphics *graphics)
{
    return i_createWitoutPreviousPoint(
                        EVTMOUSE_OUT,
                        EVTMOUSE_NO_BUTTON, false, false,
                        CMath::maxDouble(), CMath::maxDouble(), graphics);
}

//---------------------------------------------------------

struct EvtMouse_t *evtmouse_createWheel(enum evtmouse_type_t type, bool isShiftPulsed, bool isControlPulsed, class IGraphics *graphics)
{
    return i_createWitoutPreviousPoint(
                        type,
                        EVTMOUSE_WHEEL, isShiftPulsed, isControlPulsed,
                        CMath::maxDouble(), CMath::maxDouble(), graphics);
}

//---------------------------------------------------------

void evtmouse_destroy(struct EvtMouse_t **event)
{
    FREE_T(event, struct EvtMouse_t);
}

//---------------------------------------------------------

void evtmouse_position(const struct EvtMouse_t *event, double *x, double *y)
{
    assert_no_null(event);
    switch(event->type)
    {
        case EVTMOUSE_MOVE:
        case EVTMOUSE_PULSE:
        case EVTMOUSE_RELEASE:
        
            event->graphics->pointWindowToWorld(event->xPixel, event->yPixel, x, y);
            break;
            
        case EVTMOUSE_WHEEL_UP:
        case EVTMOUSE_WHEEL_DOWN:
        case EVTMOUSE_OUT:
        case EVTMOUSE_IN:
        default_error();
    }
}

//---------------------------------------------------------

enum evtmouse_type_t evtmouse_type(const struct EvtMouse_t *event)
{
    assert_no_null(event);
    return event->type;
}

//---------------------------------------------------------

enum evtmouse_button_t evtmouse_button(const struct EvtMouse_t *event)
{
    assert_no_null(event);
    return event->button;
}

//---------------------------------------------------------

class IGraphics *evtmouse_graphics(const struct EvtMouse_t *event)
{
    assert_no_null(event);
    return event->graphics;
}

//---------------------------------------------------------

bool evtmouse_hasDrag(const struct EvtMouse_t *evtMouse, double *incrX_opc, double *incrY_opc)
{
    assert_no_null(evtMouse);
    
    if (evtMouse->hasPreviuos == true)
    {
        double incrX_loc, incrY_loc;
        
        incrX_loc = evtMouse->xPixel - evtMouse->xPixelPrevious;  
        incrY_loc = evtMouse->yPixel - evtMouse->yPixelPrevius;
        
        if (CMath::isZero(incrX_loc, PRV_PRECISION) == false || CMath::isZero(incrY_loc, PRV_PRECISION) == false)
        {
            if (incrX_opc != NULL)
                *incrX_opc = incrX_loc;
                
            if (incrY_opc != NULL)
                *incrY_opc = incrY_loc;
            
            return true;
        }
    }
    
    return false;
}

//---------------------------------------------------------

bool evtmouse_isShiftPulsed(const struct EvtMouse_t *evtMouse)
{
    assert_no_null(evtMouse);
    return evtMouse->isShiftPulsed;
}

//---------------------------------------------------------

bool evtmouse_isControlPulsed(const struct EvtMouse_t *evtMouse)
{
    assert_no_null(evtMouse);
    return evtMouse->isControlPulsed;
}
