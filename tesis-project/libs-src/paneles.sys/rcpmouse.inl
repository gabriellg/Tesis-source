// Receptor de eventos raton

void rcpmouse_destruye(struct RcpMouse_t **rcpraton);

void rcpmouse_sendEvent(struct RcpMouse_t *rcpMouse, const struct EvtMouse_t *evtMouse);
