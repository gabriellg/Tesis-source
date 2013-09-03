// Receptor de eventos raton

#include "rcpmouse.h"
#include "rcpmouse.inl"

#include "asrtbas.h"
#include "memory.h"
#include "evtmouse.hxx"

#include "evtmouse.h"
#include "float.h"

struct RcpMouse_t
{
	struct rcpmouse_item_data_t *data;

	rpcmouse_FPtr_evtMouse func_evtMouse;
	rcpmouse_FPtr_destroy func_destroy;
};

//---------------------------------------------------------

static struct RcpMouse_t *i_create(
						struct rcpmouse_item_data_t **data,
						rpcmouse_FPtr_evtMouse func_evtMouse,
						rcpmouse_FPtr_destroy func_destroy)
{
	struct RcpMouse_t *rcpMouse;
	
	rcpMouse = MALLOC(struct RcpMouse_t);
	
	rcpMouse->data = ASSIGN_PP(data, struct rcpmouse_item_data_t);
	
	rcpMouse->func_evtMouse = func_evtMouse;
	rcpMouse->func_destroy = func_destroy;
	
	return rcpMouse;
}

//---------------------------------------------------------

struct RcpMouse_t *rcpmouse_createPrivate(
                            struct rcpmouse_item_data_t *data,
                            rpcmouse_FPtr_evtMouse func_evtMouse)
{
	return i_create(&data, func_evtMouse, NULL);
}

//---------------------------------------------------------

struct RcpMouse_t *rcpmouse_createWithDestroyPrivate(
                            struct rcpmouse_item_data_t **data,
                            rpcmouse_FPtr_evtMouse func_evtMouse,
                            rcpmouse_FPtr_destroy func_destroy)
{
	return i_create(data, func_evtMouse, func_destroy);
}

//---------------------------------------------------------

void rcpmouse_destruye(struct RcpMouse_t **rcpMouse)
{
    assert_no_null(rcpMouse);
    assert_no_null(*rcpMouse);
    
    if ((*rcpMouse)->data != NULL && (*rcpMouse)->func_destroy != NULL)
        (*rcpMouse)->func_destroy(&(*rcpMouse)->data);
    
    FREE_T(rcpMouse, struct RcpMouse_t);    
}

//---------------------------------------------------------

void rcpmouse_sendEvent(struct RcpMouse_t *rcpMouse, const struct EvtMouse_t *evtMouse)
{
	assert_no_null(rcpMouse);
	assert_no_null(rcpMouse->func_evtMouse);
	
	rcpMouse->func_evtMouse(evtMouse, rcpMouse->data);
}
