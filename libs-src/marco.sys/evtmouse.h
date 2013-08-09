// Define un evento del raton

#include "evtmouse.hxx"

void evtmouse_position(const struct EvtMouse_t *event, double *x, double *y);

enum evtmouse_type_t evtmouse_type(const struct EvtMouse_t *event);

enum evtmouse_button_t evtmouse_button(const struct EvtMouse_t *event);

class IGraphics *evtmouse_graphics(const struct EvtMouse_t *event);

bool evtmouse_hasDrag(const struct EvtMouse_t *evtMouse, double *incrX_opc, double *incrY_opc);

bool evtmouse_isShiftPulsed(const struct EvtMouse_t *evtMouse);

bool evtmouse_isControlPulsed(const struct EvtMouse_t *evtMouse);
