// Define un evento del raton

#ifndef _EVTMOUSE_HXX
#define _EVTMOUSE_HXX

enum evtmouse_type_t
{
    EVTMOUSE_OUT,
    EVTMOUSE_IN,
    EVTMOUSE_MOVE,
    EVTMOUSE_PULSE,
    EVTMOUSE_RELEASE,
    EVTMOUSE_WHEEL_UP,
    EVTMOUSE_WHEEL_DOWN,
};

enum evtmouse_button_t
{
    EVTMOUSE_NO_BUTTON,
    EVTMOUSE_LEFT,
    EVTMOUSE_MIDDLE,
    EVTMOUSE_RIGHT,
    EVTMOUSE_WHEEL
};

#endif
